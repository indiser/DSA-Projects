<div align="center">

# 💬 Real-Time Chat Server

[![C++20](https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Boost.Beast](https://img.shields.io/badge/Boost.Beast-WebSocket-F7901E?style=for-the-badge)](https://www.boost.org/doc/libs/release/libs/beast/)
[![PostgreSQL](https://img.shields.io/badge/PostgreSQL-16-336791?style=for-the-badge&logo=postgresql&logoColor=white)](https://www.postgresql.org/)
[![Redis](https://img.shields.io/badge/Redis-7-DC382D?style=for-the-badge&logo=redis&logoColor=white)](https://redis.io/)
[![Docker](https://img.shields.io/badge/Docker-Compose-2496ED?style=for-the-badge&logo=docker&logoColor=white)](https://docs.docker.com/compose/)
[![Status](https://img.shields.io/badge/Status-In%20Progress-FFA500?style=for-the-badge)]()

**A production-grade, multi-threaded WebSocket chat backend written in modern C++20.**  
*Persistent messaging · Real-time presence · Group chats · JWT auth · Redis pub/sub · Horizontal scaling*

[Architecture](#-architecture) • [Features](#-features) • [Project Structure](#-project-structure) • [Build](#-building) • [API](#-websocket-api) • [Database](#-database-schema) • [Scaling](#-scaling-strategy)

</div>

---

## 🧭 Overview

This is **Project #16** of the [DSA Projects roadmap](../README.md) — a FAANG-prep system design project implementing a real-world chat backend from scratch. No frameworks. No shortcuts. Every layer is hand-built in C++20 using industry-standard libraries.

```
What this is NOT:  a toy echo server or a tutorial clone
What this IS:      a layered, testable, scalable backend you could actually deploy
```

### Core DSA & Systems Concepts Demonstrated

| Concept | Where Applied |
|:--------|:-------------|
| **Graph algorithms** (BFS, shortest path) | `FriendshipGraph`, `ShortestPath`, `RecommendationEngine` |
| **LRU Cache** (doubly-linked list + hash map) | `LRUCache<K,V>` template, `CacheManager` |
| **Priority Queue / Min-Heap** | `MessageQueue`, Boost.Beast async I/O internals |
| **Trie** | `Trie` — username prefix autocomplete |
| **Inverted Index** | `InvertedIndex` — keyword → username lookup |
| **Consistent Hashing / Redis Pub/Sub** | Cross-node message routing for horizontal scale |
| **Producer-Consumer Queue** | `MessageQueue` with mutex + condition variable |
| **Sliding Window Rate Limiting** | `RateLimiter` via Redis atomic counters |
| **Merkle-style sequence IDs** | `MessageOrdering` — monotonic per-conversation counters |

---

## ✨ Features

```
✅ WebSocket connections     — Boost.Beast async I/O, multi-threaded io_context
✅ JWT Authentication        — HS256 tokens, session management via Redis
✅ Password hashing          — Argon2id via libsodium (never bcrypt-lite)
✅ Real-time messaging       — fan-out to all conversation participants
✅ Message ordering          — monotonic sequence IDs per conversation (Redis INCR)
✅ Delivery receipts         — delivered + read acknowledgements
✅ Presence system           — online/offline with 30s TTL heartbeat
✅ Typing indicators         — 5s auto-expiring Redis keys
✅ Group chats               — owner/member permissions, group conversations
✅ Message history           — paginated, cache-first (Redis → PostgreSQL fallback)
✅ Full-text search          — PostgreSQL tsvector + GIN index
✅ Username autocomplete     — in-memory Trie + InvertedIndex
✅ Friend recommendations    — BFS on FriendshipGraph (mutual connections)
✅ Rate limiting             — per-user sliding window via Redis
✅ Metrics                   — Prometheus-format counters (messages, connections, cache)
✅ Health checks             — /health endpoint (DB + Redis liveness)
✅ Structured logging        — spdlog, per-subsystem log files
✅ Docker Compose            — postgres + redis + backend, one command
✅ Horizontal scaling        — Redis Pub/Sub for cross-node fan-out
```

---

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                        Clients (mobile/web)                      │
└──────────────────────────┬──────────────────────────────────────┘
                           │ WebSocket (ws://)
┌──────────────────────────▼──────────────────────────────────────┐
│                     Load Balancer (nginx)                         │
└──────────┬───────────────────────────────────┬──────────────────┘
           │                                   │
┌──────────▼──────────┐             ┌──────────▼──────────┐
│   C++ Node 1        │             │   C++ Node 2        │
│  WebSocketServer    │             │  WebSocketServer    │
│  (Boost.Beast)      │             │  (Boost.Beast)      │
│                     │             │                     │
│  ┌───────────────┐  │             │  ┌───────────────┐  │
│  │ AuthService   │  │             │  │ AuthService   │  │
│  │ MessageService│  │             │  │ MessageService│  │
│  │PresenceService│  │             │  │PresenceService│  │
│  └───────┬───────┘  │             │  └───────┬───────┘  │
└──────────┼──────────┘             └──────────┼──────────┘
           │                                   │
           └──────────────┬────────────────────┘
                          │
          ┌───────────────┼───────────────┐
          │               │               │
┌─────────▼──────┐ ┌──────▼──────┐ ┌─────▼──────────┐
│  PostgreSQL    │ │    Redis    │ │  Redis Pub/Sub  │
│  (persistent  │ │  (sessions, │ │  (cross-node    │
│   messages,   │ │   presence, │ │   fan-out)      │
│   users,      │ │   cache,    │ │                 │
│   groups)     │ │   typing)   │ │                 │
└───────────────┘ └─────────────┘ └─────────────────┘
```

### Layer Architecture (Strict Dependency Direction)

```
websocket/  ──→  services/  ──→  repositories/  ──→  db_pool
                     │
                     └──→  cache_manager  ──→  redis_client
                     │
                     └──→  common/ (types, errors, utils)
```

**Rule:** dependencies only flow downward. No layer imports from a layer above it.

---

## 📁 Project Structure

```
Real Time Chat Application/
├── backend/
│   ├── CMakeLists.txt              # Build config (C++20, vcpkg-compatible)
│   ├── Dockerfile                  # Multi-stage: builder → runtime
│   ├── include/
│   │   ├── common/                 # types.hpp · errors.hpp · utils.hpp · constants.hpp
│   │   ├── auth/                   # auth_service · jwt_manager · password_hasher · session_manager
│   │   ├── cache/                  # redis_client · cache_manager · lru_cache
│   │   ├── database/
│   │   │   ├── db_pool.hpp         # RAII connection pool (pqxx)
│   │   │   ├── postgres_client.hpp
│   │   │   └── repositories/       # user · message · conversation · group
│   │   ├── messaging/              # message_service · dispatcher · queue · ordering · delivery
│   │   ├── presence/               # presence_service · online_tracker · typing_tracker
│   │   ├── websocket/              # websocket_server · websocket_session · connection_manager
│   │   ├── groups/                 # group_service · group_manager · permissions
│   │   ├── graph/                  # friendship_graph · shortest_path · recommendation_engine
│   │   ├── search/                 # search_service · trie · inverted_index
│   │   ├── notifications/          # notification_service · push_manager
│   │   ├── analytics/              # metrics · monitoring · rate_limiter
│   │   └── logging/                # logger · log_manager · log_formatter
│   ├── src/                        # Mirror of include/ — all .cpp implementations
│   ├── proto/                      # Protobuf definitions (messaging · auth · groups · presence)
│   ├── migrations/                 # 001–006 SQL migration files
│   ├── config/                     # dev.yaml · prod.yaml · redis.yaml · logging.yaml
│   ├── logs/                       # app · auth · websocket · database · messages · error
│   └── tests/
│       ├── unit/                   # Service-layer tests (mock repositories)
│       ├── integration/            # Real DB + Redis tests
│       └── load/                   # 1k / 10k / stress test clients
├── infrastructure/
│   ├── docker/                     # postgres.Dockerfile · redis.Dockerfile · backend.Dockerfile
│   └── docker-compose.yaml
├── scripts/
│   ├── build/                      # build.bat · clean.bat · rebuild.bat
│   ├── database/                   # create_db.bat · run_migrations.bat · seed_data.bat
│   ├── dev/                        # start_all.bat · start_backend.bat
│   ├── docker/                     # start_services.bat · stop_services.bat · reset_services.bat
│   ├── proto/                      # generate_proto.bat
│   └── testing/                    # run_unit_tests.bat · run_integration_tests.bat · load_tests.bat
├── docs/
│   ├── api.md                      # WebSocket event payload reference
│   ├── architecture.md             # System components & data flow
│   ├── database_design.md          # PostgreSQL schema overview
│   ├── deployment.md               # Local + Docker deployment
│   ├── scaling_strategy.md         # Horizontal scaling via Redis Pub/Sub
│   └── websocket_protocol.md
├── Problems/                       # LeetCode problems relevant to this project
│   ├── LRUcache.cpp                # LRU Cache (LC 146)
│   ├── HitCounter.cpp              # Hit Counter (LC 362)
│   ├── MedianFromDataStream.cpp    # Median from Data Stream (LC 295)
│   ├── MergeKSortedLists.cpp       # Merge K Sorted Lists (LC 23)
│   ├── NetworkDelayTime.cpp        # Network Delay Time / Dijkstra (LC 743)
│   ├── NumberOfIslands.cpp         # Number of Islands BFS/DFS (LC 200)
│   ├── CloneGraph.cpp              # Clone Graph (LC 133)
│   ├── TaskScheduler.cpp           # Task Scheduler (LC 621)
│   └── TopKFreq.cpp                # Top K Frequent Elements (LC 347)
├── architecture.md                 # Full directory tree
├── build_guide.md                  # Step-by-step build instructions
└── server_architecture.md          # Layer-by-layer design rationale
```

---

## 🔩 Component Deep-Dive

### `common/` — Shared Vocabulary

Everything depends on this. Nothing here depends on anything else.

```cpp
// types.hpp — core domain types
using UserID = std::string;   // UUID strings throughout

struct Message {
    MessageID      id;
    ConversationID conversation_id;
    UserID         sender_id;
    std::string    content;
    bool           delivered = false;
    bool           read      = false;
    int64_t        timestamp = 0;
};

// Result<T> — every fallible function returns this, never throws across layers
template<typename T>
struct Result {
    bool        ok;
    T           value;
    ErrorCode   error = ErrorCode::OK;
    std::string message;

    static Result success(T val);
    static Result fail(ErrorCode ec, std::string msg = {});
};
```

`ErrorCode` enum: `OK · NOT_FOUND · UNAUTHORIZED · FORBIDDEN · INVALID_INPUT · DB_ERROR · CACHE_ERROR · INTERNAL`

---

### `database/` — Persistence Layer

**`DBPool`** — RAII connection pool over `libpqxx`. Callers `acquire()` a `Handle`; the connection auto-returns on scope exit. Pool size defaults to 10 (configurable).

**Repositories** — one class per entity, pure CRUD, zero business logic:

| Repository | Key Methods |
|:-----------|:-----------|
| `UserRepository` | `create` · `find_by_id` · `find_by_email` · `set_online` · `search` |
| `MessageRepository` | `save` · `get_conversation` · `mark_delivered` · `mark_read` |
| `ConversationRepository` | `create` · `find_for_user` · `find_dm` |
| `GroupRepository` | `create` · `add_member` · `remove_member` · `get_members` |

---

### `cache/` — Redis Layer

**`RedisClient`** — thin hiredis wrapper: `get · set · del · expire · publish · subscribe`

**`CacheManager`** — domain-aware cache operations:

| Cache Key Pattern | TTL | Purpose |
|:-----------------|:----|:--------|
| `session:{token}` | 24h | Maps JWT → user_id |
| `online:{user_id}` | 30s | Presence (refreshed on ping) |
| `messages:{conv_id}` | 10min | Last 50 messages |
| `typing:{conv_id}:{user_id}` | 5s | Typing indicator |
| `rl:{action}:{user_id}` | window | Rate limit counter |

**`LRUCache<K,V>`** — in-memory template cache (doubly-linked list + `unordered_map`). Thread-safe via `std::mutex`. Used for hot User structs and conversation metadata.

---

### `auth/` — Authentication

```
register_user  →  validate input  →  hash password (Argon2id)  →  UserRepository::create
login          →  find_by_email   →  verify hash               →  SessionManager::create_session  →  JWT token
authenticate   →  SessionManager::get_user  →  UserRepository::find_by_id
logout         →  SessionManager::invalidate
```

- **`PasswordHasher`** — libsodium `crypto_pwhash` (Argon2id). Random salt per hash. `hash()` + `verify()`.
- **`JwtManager`** — HS256 sign/verify. Payload: `user_id · iat · exp`. Rejects expired or tampered tokens.
- **`SessionManager`** — Redis-backed. `create_session` → stores `session:{token} = user_id` with TTL. `refresh` extends TTL.

---

### `messaging/` — Message Pipeline

```
Client  →  WebSocketSession::on_message
              │
              ▼
        MessageService::send_message
              │  validate sender in conversation
              │  validate content length (≤ 4096)
              │  MessageRepository::save
              │  CacheManager::invalidate_messages
              │
              ▼
        MessageDispatcher::dispatch
              │  look up conversation participants
              │  PresenceService::get_online_users
              │  ConnectionManager::send_to_user  (online)
              └─ NotificationService::send_push   (offline)
```

- **`MessageQueue`** — thread-safe producer-consumer queue (`std::queue` + `mutex` + `condition_variable`). Decouples receipt from delivery.
- **`MessageOrdering`** — Redis `INCR` on `seq:{conv_id}` → monotonic sequence numbers. Clients detect gaps and request re-sync.
- **`DeliveryManager`** — handles `ACK` and `READ` events from clients → `MessageRepository::mark_delivered/read`.

---

### `presence/` — Online Status

- **`PresenceService`** — sets `online:{user_id}` Redis key (TTL 30s) on connect, deletes on disconnect. Batch `get_online_users` for fan-out decisions.
- **`OnlineTracker`** — called on every WebSocket ping. Refreshes Redis TTL so the key doesn't expire while the user is active.
- **`TypingTracker`** — `start_typing` sets `typing:{conv_id}:{user_id}` (TTL 5s) and broadcasts to conversation. Auto-expires without explicit `stop_typing`.

---

### `websocket/` — Network Layer

**`WebSocketServer`** — owns `boost::asio::io_context` and TCP acceptor. Spawns `N-1` worker threads + runs on the main thread. Each accepted connection becomes a `WebSocketSession`.

**`WebSocketSession`** lifecycle:
```
1. HTTP upgrade  →  WebSocket handshake
2. First message MUST be {"type":"auth","token":"..."}
3. AuthService::authenticate  →  reject if invalid, close socket
4. Register with ConnectionManager + PresenceService::user_connected
5. Async read loop  →  route by message type
6. Ping every 20s  →  close if no pong within 10s
7. On disconnect   →  PresenceService::user_disconnected + ConnectionManager::remove_session
```

**`ConnectionManager`** — global registry of `UserID → weak_ptr<WebSocketSession>`. `send_to_user` and `send_to_conversation` (with optional exclude). Weak pointers prevent keeping dead sessions alive.

---

### `groups/` — Group Management

```
GroupManager::create_group_with_conversation
    → GroupService::create_group          (creates groups row)
    → ConversationRepository::create      (creates linked conversation)

GroupService::add_member
    → Permissions::can_add_member         (pure logic, no I/O)
    → GroupRepository::add_member
```

`Permissions` is a pure static class — no I/O, no state. All authorization decisions are isolated here.

---

### `graph/` — Social Graph

- **`FriendshipGraph`** — adjacency list (`unordered_map<UserID, unordered_set<UserID>>`). `add_edge · remove_edge · neighbors · are_connected`.
- **`ShortestPath`** — BFS on `FriendshipGraph`. Returns `optional<vector<UserID>>` — the shortest friendship chain between two users.
- **`RecommendationEngine`** — suggests friends via mutual connection count (BFS depth-2, ranked by overlap).

---

### `search/` — Search & Autocomplete

- **`SearchService`** — `search_users` (PostgreSQL `ILIKE`), `search_messages` (PostgreSQL `tsvector` full-text via GIN index).
- **`Trie`** — prefix-tree for username autocomplete. `insert · remove · search_prefix(prefix, limit)`. Populated from DB on startup.
- **`InvertedIndex`** — token → set of usernames. Keyword lookup across tokenized usernames.

---

### `analytics/` — Observability

- **`Metrics`** — atomic counters: `messages_sent · active_connections · cache_hits · cache_misses`. `serialize_prometheus()` outputs Prometheus text format.
- **`Monitoring`** — `is_healthy()` pings PostgreSQL (`SELECT 1`) and Redis. Returns `false` if either is unreachable. Exposed as `/health`.
- **`RateLimiter`** — sliding window via Redis. `check_and_increment(user_id, action, limit, window_seconds)`. Applied to message sends, login attempts, search.

---

## 🗄️ Database Schema

```sql
users                          conversations
─────────────────────          ──────────────────────────
id          UUID PK            id         UUID PK
username    VARCHAR(50) UNIQUE group_id   UUID → groups.id
email       VARCHAR(255) UNIQUE created_at TIMESTAMPTZ
password_hash TEXT
is_online   BOOLEAN            conversation_participants
created_at  TIMESTAMPTZ        ──────────────────────────
last_seen   TIMESTAMPTZ        conversation_id UUID FK
                               user_id         UUID FK
groups                         PRIMARY KEY (conv_id, user_id)
─────────────────────
id         UUID PK             messages
name       VARCHAR(100)        ──────────────────────────
owner_id   UUID → users.id     id              UUID PK
created_at TIMESTAMPTZ         conversation_id UUID FK
                               sender_id       UUID FK
group_members                  content         TEXT
─────────────────────          delivered       BOOLEAN
group_id  UUID FK              read            BOOLEAN
user_id   UUID FK              sequence_id     BIGINT
joined_at TIMESTAMPTZ          content_tsv     TSVECTOR (generated)
PRIMARY KEY (group_id, user_id) created_at     TIMESTAMPTZ
```

**Key indexes:**
- `idx_messages_conv_created` — `(conversation_id, created_at DESC)` — primary history query path
- `idx_messages_fts` — GIN on `content_tsv` — full-text search
- `idx_group_members_user` — fast "which groups does this user belong to"
- `idx_messages_undelivered` — partial index `WHERE delivered = FALSE`

**Migration order:** `001_users → 002_conversations → 003_messages → 004_groups → 005_group_members → 006_indexes`

---

## 🔌 WebSocket API

All messages are JSON. Connection is stateful — first message must always be `auth`.

### Client → Server

```jsonc
// Authenticate (must be first message)
{ "type": "auth", "token": "<jwt>" }

// Send a message
{ "type": "send_message",
  "payload": { "conversation_id": "<uuid>", "content": "hello" } }

// Typing indicators
{ "type": "typing_start", "payload": { "conversation_id": "<uuid>" } }
{ "type": "typing_stop",  "payload": { "conversation_id": "<uuid>" } }

// Receipts
{ "type": "message_ack",  "payload": { "message_id": "<uuid>" } }
{ "type": "message_read", "payload": { "message_id": "<uuid>" } }

// Heartbeat (keep presence alive)
{ "type": "pong" }
```

### Server → Client

```jsonc
// Auth result
{ "type": "auth_ok", "user_id": "<uuid>" }

// Incoming message
{ "type": "new_message",
  "payload": { "message_id": "<uuid>", "sender_id": "<uuid>",
               "conversation_id": "<uuid>", "content": "hello",
               "server_sequence_id": 42 } }

// Presence events
{ "type": "user_online",  "user_id": "<uuid>" }
{ "type": "user_offline", "user_id": "<uuid>" }

// Typing
{ "type": "typing", "user_id": "<uuid>", "conversation_id": "<uuid>" }

// Delivery receipts
{ "type": "message_delivered", "message_id": "<uuid>" }
{ "type": "message_read",      "message_id": "<uuid>" }

// Errors
{ "type": "error", "message": "invalid payload" }

// Server heartbeat
{ "type": "ping" }   // client must respond with pong within 10s
```

---

## 🛠️ Building

### Prerequisites

| Tool | Version | Purpose |
|:-----|:--------|:--------|
| C++ compiler | GCC 11+ / Clang 13+ / MSVC 2022 | C++20 support |
| CMake | ≥ 3.20 | Build system |
| vcpkg | latest | C++ package manager (Windows) |
| Docker Desktop | latest | PostgreSQL + Redis |

### Dependencies

```bash
# vcpkg (Windows / cross-platform)
vcpkg install boost-beast libpqxx hiredis openssl protobuf \
              spdlog yaml-cpp nlohmann-json jwt-cpp libsodium gtest

# apt (Ubuntu/Debian)
sudo apt install -y libboost-all-dev libpqxx-dev libhiredis-dev \
  libssl-dev libspdlog-dev libyaml-cpp-dev nlohmann-json3-dev \
  libsodium-dev protobuf-compiler libprotobuf-dev libgtest-dev
```

### Build

```bash
# 1. Start infrastructure
cd infrastructure
docker compose up -d postgres redis

# 2. Configure & build (Linux/macOS)
cd backend
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -- -j$(nproc)

# Windows (PowerShell)
cmake -B build -DCMAKE_TOOLCHAIN_FILE="C:\vcpkg\scripts\buildsystems\vcpkg.cmake"
cmake --build build --config Release

# 3. Run migrations
for f in migrations/*.sql; do psql -U chat_user -d chat_db -f "$f"; done

# 4. Run the server
export DB_HOST=localhost DB_USER=chat_user DB_PASS=your_password DB_NAME=chat_db
export REDIS_HOST=localhost
./build/chat_server --config config/dev.yaml
```

### Docker (all-in-one)

```bash
cd infrastructure
docker compose up -d        # starts postgres + redis + backend
docker compose logs -f backend
```

### Run Tests

```bash
cmake --build build --target run_tests
cd build && ctest --output-on-failure

# Specific suite
./build/run_tests --gtest_filter="AuthService.*"
```

---

## ⚙️ Configuration

`config/dev.yaml` (key fields):

```yaml
server:
  port: 8080
  threads: 4
  max_connections: 1000

jwt:
  secret: "dev_secret_change_in_prod"
  expiry_seconds: 86400

messaging:
  max_message_length: 4096
  rate_limit_per_minute: 120

presence:
  ttl_seconds: 30          # Redis key TTL
  refresh_seconds: 20      # heartbeat interval

websocket:
  ping_interval_seconds: 20
  pong_timeout_seconds: 10
```

All sensitive values (`DB_PASS`, `JWT_SECRET`, `FCM_KEY`) are read from environment variables at runtime — never hardcoded.

---

## 📈 Scaling Strategy

**The problem:** WebSocket connections are stateful. User A on Node 1 and User B on Node 2 can't communicate via shared memory.

**The solution:** Redis Pub/Sub as the message bus between nodes.

```
Node 1 receives message for Conversation X
    │
    ├─ Fan out to locally connected members (ConnectionManager)
    │
    └─ Publish to Redis channel "chat:message:{conv_id}"
              │
              ▼
    All other nodes subscribed to that channel
              │
              └─ Fan out to their locally connected members
```

**Presence** is also Redis-backed — any node can check `online:{user_id}` regardless of which node the user is connected to.

**Horizontal scale checklist:**
- [x] Stateless application logic (all state in PostgreSQL + Redis)
- [x] Redis Pub/Sub for cross-node fan-out
- [x] Session tokens validated against Redis (not in-memory)
- [x] Connection pool per node (not shared)
- [ ] Redis Cluster for Redis HA (future)
- [ ] Read replicas for PostgreSQL (future)

---

## 🧪 LeetCode Problems — Applied Here

The `Problems/` directory contains standalone solutions to LeetCode problems that directly map to components in this project:

| Problem | LC # | Applied In |
|:--------|:----:|:-----------|
| LRU Cache | 146 | `LRUCache<K,V>`, `CacheManager` |
| Hit Counter | 362 | `RateLimiter` (sliding window) |
| Median from Data Stream | 295 | `MessageQueue` ordering concepts |
| Merge K Sorted Lists | 23 | Multi-source message merging |
| Network Delay Time | 743 | `ShortestPath` (BFS/Dijkstra on friendship graph) |
| Number of Islands | 200 | `FriendshipGraph` BFS traversal |
| Clone Graph | 133 | `FriendshipGraph` deep copy |
| Task Scheduler | 621 | `MessageDispatcher` fan-out scheduling |
| Top K Frequent Elements | 347 | `RecommendationEngine` top-N friends |

---

## 🗺️ Build Order (What to Write First)

```
Week 1  ──  common/types · common/utils · db_pool
            user_repository · message_repository · conversation_repository
            password_hasher · jwt_manager · session_manager · auth_service

Week 2  ──  redis_client · cache_manager
            presence_service · online_tracker
            message_service · message_dispatcher · message_queue · delivery_manager

Week 3  ──  websocket_server · websocket_session · connection_manager
            typing_tracker
            group_service · permissions · group_manager

Week 4  ──  notification_service · push_manager
            rate_limiter
            search_service (ILIKE only)

Later   ──  metrics · monitoring · inverted_index · trie · recommendation_engine
```

> Don't touch Week 4 until Weeks 1–3 have passing unit tests.

---

## 📋 Known Issues / TODO

- [ ] Fix typos: `messeging` → `messaging`, `infastructure` → `infrastructure` (see `build_guide.md`)
- [ ] Fill empty proto files: `auth.proto`, `groups.proto`, `notifications.proto`, `presence.proto`
- [ ] Implement load test clients in `tests/load/`
- [ ] Add JWT refresh endpoint
- [ ] Redis Cluster support for HA
- [ ] `start_backend.bat` script is empty — needs cmake + run commands
- [ ] `backend.Dockerfile` and `postgres.Dockerfile` in `infrastructure/docker/` are empty stubs

---

<div align="center">

**Part of the [DSA Projects Roadmap](../README.md) — Project 16 / 24**

*Phase 4: FAANG Interview Preparation · System Design · ⭐⭐⭐⭐*

</div>
