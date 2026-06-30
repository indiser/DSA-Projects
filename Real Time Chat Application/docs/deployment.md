# deployment.md & scaling_strategy.md

## Local Deployment
1. Run `docker-compose up -d` to start Redis and PostgreSQL.
2. Run `./scripts/dev/start_backend.sh` to compile via CMake and execute the C++ binary.

## Scaling Strategy (Horizontal)
- **The Problem:** WebSocket connections are stateful. If User A is on Node 1, and User B is on Node 2, they cannot communicate directly via memory.
- **The Solution:** Redis Pub/Sub. 
  - Node 1 receives a message for Group X.
  - Node 1 publishes the message to Redis channel `group_x`.
  - All nodes subscribed to `group_x` receive the payload and push it to their locally connected clients.