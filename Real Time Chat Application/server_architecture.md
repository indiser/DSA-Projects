# How to Architect the Chat Server
> What goes where, why, and in what order. No code — pure structure and thinking.

---

## The Mental Model

Think in layers. Each layer only talks to the layer directly below it. Never skip layers.

```
Client (mobile/web)
       ↕ WebSocket
  WebSocket Layer        ← handles raw connections, nothing else
       ↕
  Service Layer          ← all business logic lives here
       ↕
  Repository Layer       ← all DB/cache access lives here
       ↕
  Database / Redis
```

If a service is calling another service directly for data — that's a smell. If a repository is making business decisions — that's a bug in your thinking.

---

## Layer 1 — Common (`include/common/`)

**Build this first. Everything else depends on it.**

### `types.hpp`
Define every core data structure here. This is your shared vocabulary.

- `User` — id, username, email, password_hash, is_online, timestamps
- `Message` — id, conversation_id, sender_id, content, delivered, read, timestamp
- `Conversation` — id, list of participant user IDs, optional group_id
- `Group` — id, name, owner_id, list of member IDs
- `UserID`, `MessageID`, `ConversationID`, `GroupID` — just `using X = std::string` (UUIDs)

### `result.hpp` (or inside types.hpp)
A `Result<T>` template that wraps either a value or an error. Every function that can fail returns this — never throw across layer boundaries, never return raw nullptrs.

- `Result<T>::success(value)`
- `Result<T>::fail(ErrorCode, message)`
- `Result<void>` for operations that don't return data

### `errors.hpp`
An `ErrorCode` enum: `OK`, `NOT_FOUND`, `UNAUTHORIZED`, `FORBIDDEN`, `INVALID_INPUT`, `DB_ERROR`, `CACHE_ERROR`, `INTERNAL`

### `constants.hpp`
Magic numbers that appear in more than one place: max message length, max group size, JWT expiry, session TTL, WebSocket ping interval.

### `utils.hpp`
Stateless helper functions: UUID generation, timestamp conversion, string trimming, input sanitisation.

**Rule:** nothing in `common/` depends on anything else in your codebase. Zero imports from other layers.

---

## Layer 2 — Database (`include/database/`)

### `db_pool.hpp` + `db_pool.cpp`
Manages N persistent PostgreSQL connections. Callers `acquire()` a connection, use it, and it auto-returns when it goes out of scope (RAII). Never let callers hold connections longer than one request.

Key decisions:
- Pool size = roughly 2× your thread count to start
- Reconnect automatically if a connection drops
- `acquire()` blocks if all connections are busy — add a timeout later

### Repositories — one file per entity

Each repository does exactly one thing: translate between your C++ structs and SQL rows. No business logic. No decisions. Just CRUD.

**`user_repository`**
- `create(username, email, password_hash)` → `Result<User>`
- `find_by_id(id)` → `Result<User>`
- `find_by_email(email)` → `Result<User>`
- `set_online(id, bool)` → `Result<void>`
- `update_last_seen(id)` → `Result<void>`
- `search(query, limit)` → `Result<vector<User>>`

**`message_repository`**
- `save(message)` → `Result<Message>`
- `get_conversation(conv_id, limit, offset)` → `Result<vector<Message>>`
- `mark_delivered(id)` → `Result<void>`
- `mark_read(id)` → `Result<void>`

**`conversation_repository`**
- `create(participants)` → `Result<Conversation>`
- `find_by_id(id)` → `Result<Conversation>`
- `find_for_user(user_id)` → `Result<vector<Conversation>>`
- `find_dm(user_a, user_b)` → `Result<optional<Conversation>>`

**`group_repository`**
- `create(name, owner_id)` → `Result<Group>`
- `add_member(group_id, user_id)` → `Result<void>`
- `remove_member(group_id, user_id)` → `Result<void>`
- `find_by_id(id)` → `Result<Group>`
- `get_members(group_id)` → `Result<vector<User>>`

**Rule:** every repository takes a `DBPool&` in its constructor. Never own the pool. Never make business decisions. Never call other repositories.

---

## Layer 3 — Cache (`include/cache/`)

### `redis_client.hpp` + `redis_client.cpp`
Thin wrapper around hiredis. Exposes `get`, `set`, `del`, `expire`, `publish`, `subscribe`. Handles reconnection. Everything else is built on top of this.

### `cache_manager.hpp` + `cache_manager.cpp`
Higher-level cache operations specific to your domain. This is where you decide *what* gets cached and *for how long*.

What to cache:
- **User sessions** — `session:{token}` → user_id, TTL = session expiry
- **Online presence** — `online:{user_id}` → 1, TTL = 30s (refresh on ping)
- **Recent messages** — `messages:{conv_id}` → last 50 messages, TTL = 10min
- **Typing indicators** — `typing:{conv_id}:{user_id}` → 1, TTL = 5s

### `lru_cache.hpp`
In-memory LRU cache for things too hot even for Redis. Use for: recently fetched User structs, active conversation metadata. Cap size, evict least-recently-used. Standard `unordered_map` + doubly-linked list.

**Rule:** cache layer never writes to DB. DB layer never touches cache. The *service* layer coordinates both.

---

## Layer 4 — Auth (`include/auth/`)

### `password_hasher.hpp`
Two functions: `hash(password)` → string, `verify(password, hash)` → bool. Use libsodium's `crypto_pwhash` (Argon2id). Never roll your own.

### `jwt_manager.hpp`
- `generate(user_id, expiry)` → token string
- `verify(token)` → `Result<UserID>`

Token payload: user_id, issued_at, expires_at. Sign with HS256 using a secret from config. Reject expired tokens. Reject tokens with invalid signatures.

### `session_manager.hpp`
Maps session tokens to users. Backed by Redis.

- `create_session(user_id)` → session token
- `get_user(token)` → `Result<UserID>`
- `invalidate(token)` → logout
- `refresh(token)` → extends TTL

### `auth_service.hpp`
The only auth entry point the rest of the system calls.

- `register_user(username, email, password)` → `Result<User>`
  - validates input → hashes password → calls UserRepository → returns user
- `login(email, password)` → `Result<string>` (JWT token)
  - fetches user → verifies password → creates session → returns token
- `logout(token)` → `Result<void>`
- `authenticate(token)` → `Result<User>` — called by WebSocket on every connection

**Rule:** auth_service is the only thing that calls both UserRepository and SessionManager. WebSocket layer calls auth_service. Nothing else handles credentials.

---

## Layer 5 — Messaging (`include/messaging/`)

### `message_service.hpp`
Core business logic for sending and retrieving messages.

- `send_message(sender_id, conv_id, content)` → `Result<Message>`
  - validate sender is in conversation
  - validate content (not empty, not too long)
  - save to DB via MessageRepository
  - cache invalidation
  - hand off to MessageDispatcher
- `get_history(user_id, conv_id, limit, offset)` → `Result<vector<Message>>`
  - check cache first
  - fall back to DB
  - re-populate cache

### `message_dispatcher.hpp`
Figures out *who* to deliver a message to and routes it.

- `dispatch(message)` — looks up all participants in the conversation, checks who's online via PresenceService, pushes to their WebSocket connections via ConnectionManager
- For offline users: queues for push notification

### `message_queue.hpp`
In-process queue (or Redis-backed for multi-instance). Buffers messages between receipt and delivery. Prevents the dispatcher from blocking the WebSocket thread.

- Single producer, multiple consumer pattern
- `enqueue(message)`
- Worker threads call `dequeue()` in a loop

### `message_ordering.hpp`
Assigns sequence numbers to messages per conversation. Clients use these to detect gaps and request re-sync. Simple atomic counter per conversation stored in Redis.

### `delivery_manager.hpp`
Tracks delivery acknowledgements.

- Client sends `ACK {message_id}` over WebSocket
- DeliveryManager calls `mark_delivered` on MessageRepository
- Retries unacknowledged messages after timeout

---

## Layer 6 — Presence (`include/presence/`)

### `presence_service.hpp`
Single source of truth for who's online.

- `user_connected(user_id)` — called by WebSocket on connect
- `user_disconnected(user_id)` — called on disconnect
- `is_online(user_id)` → bool — checks Redis key
- `get_online_users(user_ids)` → `vector<UserID>` — batch check

Implementation: set a Redis key `online:{user_id}` with 30s TTL on connect. Refresh every 20s via heartbeat. Delete on disconnect.

### `online_tracker.hpp`
Manages the heartbeat loop. Every connected session sends a WebSocket ping every 20s. OnlineTracker refreshes the Redis TTL on each ping received.

### `typing_tracker.hpp`
- `start_typing(user_id, conv_id)` — sets Redis key, TTL 5s, broadcasts to conversation
- `stop_typing(user_id, conv_id)` — deletes key, broadcasts
- Auto-expires after 5s without refresh (user stopped typing without sending)

---

## Layer 7 — WebSocket (`include/websocket/`)

This is the outermost layer. It touches the network. It knows nothing about business logic.

### `websocket_server.hpp`
Owns the `io_context` and acceptor (Boost.Beast). Spawns one `WebSocketSession` per accepted connection. Manages the thread pool that drives `io_context`.

Startup sequence:
1. Load config
2. Init DBPool
3. Init RedisClient
4. Init all services (AuthService, MessageService, PresenceService, etc.)
5. Start acceptor loop

### `websocket_session.hpp`
One instance per connected client. Owns the socket. Lifecycle:

1. **Handshake** — upgrade HTTP → WebSocket
2. **Auth** — first message must be `{"type":"auth","token":"..."}`. Call AuthService. Reject if invalid.
3. **Read loop** — async read → parse JSON → route to handler → async write response
4. **Heartbeat** — send ping every 20s, close if no pong within 10s
5. **Cleanup** — on disconnect, call PresenceService.user_disconnected, release all resources

Message types to handle (incoming from client):
- `auth` — authenticate session
- `send_message` — route to MessageService
- `typing_start` / `typing_stop` — route to TypingTracker
- `message_ack` — route to DeliveryManager
- `pong` — refresh presence TTL

Message types to send (outgoing to client):
- `message` — new message received
- `message_delivered` — your message was delivered
- `message_read` — your message was read
- `user_online` / `user_offline` — presence event
- `typing` — someone is typing
- `error` — something failed

### `connection_manager.hpp`
Global registry of active WebSocket sessions.

- `register_session(user_id, session_ptr)`
- `remove_session(user_id)`
- `send_to_user(user_id, payload)` — dispatcher calls this
- `send_to_conversation(conv_id, payload, exclude_user_id)` — broadcast

Use `shared_ptr` for sessions. Weak references in the map to avoid keeping dead sessions alive.

**Rule:** WebSocket layer calls services. Services never call WebSocket layer directly — they go through ConnectionManager.

---

## Layer 8 — Groups (`include/groups/`)

### `group_service.hpp`
- `create_group(owner_id, name)` → `Result<Group>`
- `add_member(group_id, requester_id, target_user_id)` → `Result<void>`
- `remove_member(group_id, requester_id, target_user_id)` → `Result<void>`
- `get_group(group_id, requester_id)` → `Result<Group>`

### `permissions.hpp`
Pure logic — no I/O.
- `can_add_member(requester, group)` → bool
- `can_remove_member(requester, target, group)` → bool
- `can_delete_group(requester, group)` → bool

### `group_manager.hpp`
Coordinates GroupService + MessageService for group-specific flows: creating a group also creates a conversation, adding a member also grants conversation access.

---

## Layer 9 — Notifications (`include/notifications/`)

### `notification_service.hpp`
Called by MessageDispatcher when target user is offline.

- `send_push(user_id, message)` → enqueues push notification
- Integrates with FCM (Android) / APNs (iOS)
- Store device tokens in DB, per user

### `push_manager.hpp`
Handles the actual HTTP calls to FCM/APNs. Retries on failure. Batch sends where possible.

---

## Layer 10 — Analytics (`include/analytics/`)

Build this last. Don't touch it until the core works.

### `rate_limiter.hpp`
Per-user rate limiting via Redis sliding window.
- `check_and_increment(user_id, action, limit, window_seconds)` → bool
- Apply to: message sends, login attempts, search queries

### `metrics.hpp`
Counters and gauges: messages sent/sec, active connections, DB pool utilisation, cache hit rate. Expose via HTTP `/metrics` endpoint for Prometheus scraping.

### `monitoring.hpp`
Health check endpoint `/health`. Returns 200 if DB + Redis reachable, 503 otherwise.

---

## Layer 11 — Search (`include/search/`)

Also build last.

### `search_service.hpp`
- `search_users(query)` → delegates to UserRepository (ILIKE for now)
- `search_messages(user_id, query)` → full-text search via PostgreSQL `tsvector`

### `inverted_index.hpp` + `trie.hpp`
In-memory structures for autocomplete on usernames. Populate on startup from DB. Invalidate on new user registration. Trie for prefix matching, inverted index for keyword lookup.

Only build these when PostgreSQL ILIKE is too slow under real load. Don't build prematurely.

---

## `main.cpp` — Wiring Everything Together

This file should be short. Its only job is dependency injection — construct everything in the right order, pass dependencies down.

Order of construction:
1. Load config from YAML
2. Init logger (spdlog)
3. Construct DBPool
4. Construct RedisClient
5. Construct CacheManager(redis)
6. Construct repositories (pass DBPool)
7. Construct services (pass repositories + cache)
8. Construct ConnectionManager
9. Construct WebSocketServer (pass services + connection manager)
10. Start server — blocks here

No globals. No singletons. Pass everything via constructor. This makes testing trivial.

---

## `.hpp` File Rules

Every header must:
- Start with `#pragma once`
- Only `#include` what it directly uses — nothing more
- Declare classes with private members at the bottom
- Keep method bodies out — definitions go in `.cpp`
- Have no mutable global state

What belongs in a header:
- Class declaration
- `struct` definitions
- `using` type aliases
- `enum class`
- Template definitions (templates must be in headers)

What does NOT belong in a header:
- Method implementations (except templates)
- `using namespace std;`
- Heavy includes like `<pqxx/pqxx>` in files that don't need the full type

---

## `.cpp` File Rules

Every source file must:
- Include its own header first
- Include only what it uses
- Put all code inside `namespace chat { }`
- Never `using namespace std;` — write `std::` explicitly

---

## Dependency Direction (Strict)

```
websocket → services → repositories → db_pool
websocket → services → cache_manager → redis_client
websocket → auth_service → password_hasher, jwt_manager, session_manager
services  → common (types, errors, utils)
```

If you find yourself going backwards (repository calling a service, service importing websocket types) — stop. You've made a design error. Fix the abstraction, not the include.

---

## Build Order (What to Write First)

```
Week 1:  common/types, common/errors, common/utils
         db_pool
         user_repository, message_repository, conversation_repository
         auth/password_hasher, auth/jwt_manager, auth/session_manager
         auth_service

Week 2:  redis_client, cache_manager
         presence_service, online_tracker
         message_service, message_dispatcher, message_queue
         delivery_manager

Week 3:  websocket_server, websocket_session, connection_manager
         typing_tracker
         group_service, permissions, group_manager

Week 4:  notification_service, push_manager
         rate_limiter
         search_service (basic ILIKE only)

Later:   metrics, monitoring, inverted_index, trie, recommendation_engine
```

Don't touch Week 4 until Week 1–3 have passing unit tests.

---

## Testing Strategy Per Layer

| Layer | Test type | What to test |
|-------|-----------|-------------|
| common | Unit | Result propagation, type conversions, utils |
| repositories | Integration | Real DB, CRUD roundtrip, constraint violations |
| auth_service | Unit + Integration | Hash/verify, JWT sign/verify, session lifecycle |
| message_service | Unit (mock repos) | Validation logic, dispatch triggers |
| presence | Integration (real Redis) | TTL expiry, online/offline transitions |
| websocket | Integration | Full connect → auth → send → receive → disconnect |
| rate_limiter | Integration (real Redis) | Window resets, limit enforcement |

Mock repositories in service-layer unit tests. Never mock the DB in integration tests — use a real test database.