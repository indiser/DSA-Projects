# 🐳 Valkeyrie Docker Deployment Guide

## 📋 Prerequisites

- Docker installed (version 20.10+)
- Docker Compose (optional, for easier management)

## 🚀 Quick Start

### Option 1: Using Docker Compose (Recommended)

```bash
# Build and start
docker-compose up -d

# View logs
docker-compose logs -f

# Stop
docker-compose down
```

### Option 2: Using Docker CLI

```bash
# Build the image
docker build -t valkeyrie:latest .

# Run the container
docker run -d \
  --name valkeyrie \
  -p 6379:6379 \
  -e VALKEYRIE_CAPACITY=10000 \
  -e VALKEYRIE_POLICY=LRU \
  -e VALKEYRIE_THREADS=4 \
  valkeyrie:latest

# View logs
docker logs -f valkeyrie

# Stop container
docker stop valkeyrie
docker rm valkeyrie
```

## 🔧 Configuration

Environment variables:

| Variable | Default | Description |
|----------|---------|-------------|
| `VALKEYRIE_HOST` | `0.0.0.0` | Server host address |
| `VALKEYRIE_PORT` | `6379` | Server port |
| `VALKEYRIE_CAPACITY` | `10000` | Cache capacity (entries) |
| `VALKEYRIE_POLICY` | `LRU` | Eviction policy (LRU or LFU) |
| `VALKEYRIE_THREADS` | `4` | Number of worker threads |

### Custom Configuration Example

```bash
docker run -d \
  --name valkeyrie-custom \
  -p 8080:6379 \
  -e VALKEYRIE_PORT=6379 \
  -e VALKEYRIE_CAPACITY=50000 \
  -e VALKEYRIE_POLICY=LFU \
  -e VALKEYRIE_THREADS=8 \
  valkeyrie:latest
```

## 🧪 Testing

### From Host Machine

```bash
# Using redis-cli
redis-cli -p 6379

# Using telnet
telnet localhost 6379

# Using netcat
echo -e '*1\r\n$4\r\nPING\r\n' | nc localhost 6379
```

### From Another Container

```bash
# Create a test container
docker run -it --rm \
  --network valkeyrie-network \
  redis:alpine \
  redis-cli -h valkeyrie -p 6379

# Test commands
PING
SET mykey "Hello Docker"
GET mykey
```

## 📊 Docker Compose with Multiple Instances

For a distributed setup:

```yaml
version: '3.8'

services:
  valkeyrie-node1:
    build: .
    ports:
      - "6379:6379"
    environment:
      - VALKEYRIE_PORT=6379

  valkeyrie-node2:
    build: .
    ports:
      - "6380:6379"
    environment:
      - VALKEYRIE_PORT=6379

  valkeyrie-node3:
    build: .
    ports:
      - "6381:6379"
    environment:
      - VALKEYRIE_PORT=6379

networks:
  default:
    driver: bridge
```

Run with:
```bash
docker-compose -f docker-compose-cluster.yml up -d
```

## 🔍 Monitoring

### Check Container Status

```bash
# Container stats
docker stats valkeyrie

# Container health
docker inspect valkeyrie | grep Health -A 10

# View logs
docker logs valkeyrie --tail 100 -f
```

### Access Container Shell

```bash
docker exec -it valkeyrie sh
```

## 📦 Publishing to Docker Hub

```bash
# Tag image
docker tag valkeyrie:latest yourusername/valkeyrie:latest
docker tag valkeyrie:latest yourusername/valkeyrie:v1.0.0

# Push to Docker Hub
docker push yourusername/valkeyrie:latest
docker push yourusername/valkeyrie:v1.0.0
```

## 🎯 Production Deployment

### Kubernetes Deployment

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: valkeyrie
spec:
  replicas: 3
  selector:
    matchLabels:
      app: valkeyrie
  template:
    metadata:
      labels:
        app: valkeyrie
    spec:
      containers:
      - name: valkeyrie
        image: yourusername/valkeyrie:latest
        ports:
        - containerPort: 6379
        env:
        - name: VALKEYRIE_CAPACITY
          value: "50000"
        - name: VALKEYRIE_POLICY
          value: "LFU"
        resources:
          limits:
            memory: "512Mi"
            cpu: "500m"
---
apiVersion: v1
kind: Service
metadata:
  name: valkeyrie-service
spec:
  selector:
    app: valkeyrie
  ports:
  - port: 6379
    targetPort: 6379
  type: LoadBalancer
```

### Docker Swarm

```bash
# Initialize swarm
docker swarm init

# Deploy stack
docker stack deploy -c docker-compose.yml valkeyrie-stack

# Scale service
docker service scale valkeyrie-stack_valkeyrie=5

# View services
docker service ls
docker service ps valkeyrie-stack_valkeyrie
```

## 🛡️ Security Best Practices

1. **Run as non-root user** (already configured)
2. **Limit resources**:
```bash
docker run -d \
  --name valkeyrie \
  --memory="512m" \
  --cpus="1.0" \
  -p 6379:6379 \
  valkeyrie:latest
```

3. **Use secrets for sensitive config**:
```bash
echo "mysecretconfig" | docker secret create valkeyrie_config -
```

## 📈 Performance Tuning

```bash
# High-performance setup
docker run -d \
  --name valkeyrie-perf \
  --memory="2g" \
  --cpus="4.0" \
  -e VALKEYRIE_CAPACITY=100000 \
  -e VALKEYRIE_THREADS=16 \
  -p 6379:6379 \
  valkeyrie:latest
```

## 🐛 Troubleshooting

```bash
# Check if port is in use
netstat -an | grep 6379

# View detailed logs
docker logs valkeyrie --timestamps

# Restart container
docker restart valkeyrie

# Remove and recreate
docker-compose down
docker-compose up --build -d
```

## 🎉 Success!

Your Valkeyrie cache is now running in Docker!

Test it:
```bash
redis-cli -p 6379
127.0.0.1:6379> PING
PONG
127.0.0.1:6379> SET docker "awesome"
OK
127.0.0.1:6379> GET docker
"awesome"
```
