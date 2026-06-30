# architecture.md

## System Components

1. **Client**: Establishes persistent WebSocket connection.
2. **Load Balancer**: Routes WebSocket traffic to active C++ nodes.
3. **C++ WebSocket Server (The Engine)**: 
   - Manages epoll event loop.
   - Holds in-memory map of `UserID -> SocketFD`.
4. **Priority Queue**: Buffers incoming messages and sorts by timestamp/sequence ID to ensure strict ordering.
5. **Redis (Pub/Sub & Cache)**: 
   - Pub/Sub for cross-node message routing (if scaling beyond one node).
   - Caches active user presence and group graph relationships.
6. **PostgreSQL**: Persistent source of truth for chat history.

## Data Flow (Group Message)
1. User A sends message to Group B.
2. Server receives message, assigns monotonic Sequence ID.
3. Server queries Redis for Group B members.
4. Server cross-references members with active WebSocket connections.
5. Server fans out message to all connected members in O(V) time.
6. Server asynchronously flushes message to PostgreSQL.