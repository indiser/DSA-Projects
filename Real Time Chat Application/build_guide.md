# Chat Server — Full Build Guide
> C++ WebSocket backend · PostgreSQL · Redis · Docker · All platforms

---

## Table of Contents

1. [Fix Typos First](#0-fix-typos-first)
2. [Prerequisites](#1-prerequisites)
3. [Project Structure](#2-project-structure)
4. [Dependencies](#3-dependencies)
5. [CMake Setup](#4-cmake-setup)
6. [Build the Backend](#5-build-the-backend)
7. [Database Setup](#6-database-setup)
8. [Redis Setup](#7-redis-setup)
9. [Protobuf Generation](#8-protobuf-generation)
10. [Docker](#9-docker)
11. [Running the Server](#10-running-the-server)
12. [Testing](#11-testing)
13. [Cross-Platform Scripts](#12-cross-platform-scripts)
14. [Common Errors](#13-common-errors)

---

## 0. Fix Typos First

**Do this before anything else.** These typos are baked into filenames, class names, proto files, and logs. Fix now or pay compounding interest forever.

```bash
# Linux/macOS — rename directories
mv backend/include/messeging backend/include/messaging
mv backend/src/messeging     backend/src/messaging
mv infastructure             infrastructure

# Rename files (run from repo root)
find . -name "*messege*" | while read f; do
  mv "$f" "$(echo "$f" | sed 's/messege/message/g')"
done

find . -name "*messeging*" | while read f; do
  mv "$f" "$(echo "$f" | sed 's/messeging/messaging/g')"
done

# Fix script typo
mv scripts/logs/archeive_logs.bat scripts/logs/archive_logs.bat
```

```powershell
# Windows PowerShell — rename directories
Rename-Item backend\include\messeging  messaging
Rename-Item backend\src\messeging      messaging
Rename-Item infastructure              infrastructure
Rename-Item scripts\logs\archeive_logs.bat archive_logs.bat
```

Then do a **global find-and-replace** in your IDE:
- `messege` → `message`
- `messeging` → `messaging`
- `infastructure` → `infrastructure`

---

## 1. Prerequisites

### Windows

```powershell
# Install Chocolatey (run as Administrator)
Set-ExecutionPolicy Bypass -Scope Process -Force
iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))

# Install tools
choco install cmake git visualstudio2022buildtools \
  postgresql redis nodejs protobuf -y

# Install vcpkg (C++ package manager)
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
setx VCPKG_ROOT C:\vcpkg
```

### Linux (Ubuntu/Debian)

```bash
sudo apt update && sudo apt install -y \
  build-essential cmake git \
  libssl-dev libpq-dev libhiredis-dev \
  protobuf-compiler libprotobuf-dev \
  postgresql postgresql-client \
  redis-server \
  nodejs npm \
  pkg-config
```

### macOS

```bash
# Install Homebrew if missing
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

brew install cmake git openssl libpq hiredis \
  protobuf postgresql redis node pkg-config
```

### Docker (all platforms — easiest path)

```bash
# Install Docker Desktop from https://www.docker.com/products/docker-desktop
docker --version   # verify
docker compose version  # verify
```

---

## 2. Project Structure

```
chat-server/
├── backend/
│   ├── CMakeLists.txt
│   ├── include/          # .hpp headers
│   ├── src/              # .cpp sources
│   ├── proto/            # .proto definitions
│   ├── config/           # yaml configs
│   ├── migrations/       # SQL migrations
│   └── tests/
├── infrastructure/       # (fixed typo)
│   ├── docker/
│   └── docker-compose.yaml
├── scripts/
│   ├── build/
│   ├── database/
│   ├── dev/
│   └── docker/
└── docs/
```

---

## 3. Dependencies

Core C++ libraries needed:

| Library | Purpose | Version |
|---------|---------|---------|
| Boost.Beast | WebSocket server | ≥ 1.82 |
| libpqxx | PostgreSQL client | ≥ 7.7 |
| hiredis | Redis client | ≥ 1.1 |
| OpenSSL | TLS / JWT signing | ≥ 3.0 |
| Protobuf | Serialization | ≥ 23 |
| spdlog | Logging | ≥ 1.12 |
| yaml-cpp | Config parsing | ≥ 0.7 |
| nlohmann/json | JSON handling | ≥ 3.11 |
| jwt-cpp | JWT tokens | ≥ 0.6 |
| bcrypt / libsodium | Password hashing | latest |
| Google Test | Unit tests | ≥ 1.14 |
| Google Benchmark | Benchmarks | ≥ 1.8 |

### Install via vcpkg (Windows & cross-platform)

```bash
vcpkg install boost-beast libpqxx hiredis openssl \
  protobuf spdlog yaml-cpp nlohmann-json \
  jwt-cpp libsodium gtest benchmark
```

### Install via apt (Linux)

```bash
sudo apt install -y \
  libboost-all-dev libpqxx-dev libhiredis-dev \
  libssl-dev libspdlog-dev libyaml-cpp-dev \
  nlohmann-json3-dev libsodium-dev \
  libgtest-dev libbenchmark-dev
```

### Install via brew (macOS)

```bash
brew install boost libpqxx hiredis openssl@3 \
  spdlog yaml-cpp nlohmann-json libsodium \
  googletest google-benchmark
```

---

## 4. CMake Setup

Your `backend/CMakeLists.txt` — minimum viable config:

```cmake
cmake_minimum_required(VERSION 3.20)
project(ChatServer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Find packages ---
find_package(Boost       REQUIRED COMPONENTS system thread)
find_package(OpenSSL     REQUIRED)
find_package(Protobuf    REQUIRED)
find_package(spdlog      REQUIRED)
find_package(yaml-cpp    REQUIRED)
find_package(libpqxx     REQUIRED)
find_package(GTest       REQUIRED)

# --- Protobuf generation ---
file(GLOB PROTO_FILES "${CMAKE_SOURCE_DIR}/proto/*.proto")
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

# --- Collect sources ---
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

# --- Library target (reused by tests) ---
add_library(chat_lib ${SOURCES} ${PROTO_SRCS})
target_include_directories(chat_lib PUBLIC
  include
  ${CMAKE_CURRENT_BINARY_DIR}   # for generated proto headers
  ${Boost_INCLUDE_DIRS}
)
target_link_libraries(chat_lib PUBLIC
  Boost::system Boost::thread
  OpenSSL::SSL OpenSSL::Crypto
  protobuf::libprotobuf
  spdlog::spdlog
  yaml-cpp
  pqxx
  hiredis
)

# --- Executable ---
add_executable(chat_server src/main.cpp)
target_link_libraries(chat_server PRIVATE chat_lib)

# --- Tests ---
add_subdirectory(tests)

# --- Compiler warnings ---
if(MSVC)
  target_compile_options(chat_lib PRIVATE /W4 /WX)
else()
  target_compile_options(chat_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()
```

`backend/tests/CMakeLists.txt`:

```cmake
enable_testing()

file(GLOB_RECURSE TEST_SOURCES "unit/*.cpp" "integration/*.cpp")

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests PRIVATE chat_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)
```

---

## 5. Build the Backend

### Linux / macOS

```bash
cd backend

# Configure (Debug)
cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug

# Configure (Release)
cmake -B build/release -DCMAKE_BUILD_TYPE=Release

# Build (use all CPU cores)
cmake --build build/debug   -- -j$(nproc)
cmake --build build/release -- -j$(nproc)

# Output binary
./build/release/chat_server
```

### Windows

```powershell
cd backend

# Configure with vcpkg toolchain
cmake -B build `
  -DCMAKE_TOOLCHAIN_FILE="C:\vcpkg\scripts\buildsystems\vcpkg.cmake" `
  -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Output
.\build\Release\chat_server.exe
```

### Clean rebuild

```bash
# Linux/macOS
rm -rf backend/build && cmake -B backend/build -DCMAKE_BUILD_TYPE=Debug

# Windows
Remove-Item -Recurse -Force backend\build
```

---

## 6. Database Setup

### Local PostgreSQL

```bash
# Linux — start service
sudo systemctl start postgresql
sudo systemctl enable postgresql

# macOS
brew services start postgresql@16

# Windows
net start postgresql-x64-16
```

### Create database and user

```sql
-- Run as postgres superuser
psql -U postgres

CREATE USER chat_user WITH PASSWORD 'your_password';
CREATE DATABASE chat_db OWNER chat_user;
GRANT ALL PRIVILEGES ON DATABASE chat_db TO chat_user;
\q
```

### Run migrations

```bash
# Linux/macOS
cd backend
for f in migrations/*.sql; do
  echo "Running $f..."
  psql -U chat_user -d chat_db -f "$f"
done

# Windows
cd backend
for %f in (migrations\*.sql) do psql -U chat_user -d chat_db -f "%f"
```

### Migration files (expected order)

```
001_create_users.sql
002_create_conversations.sql
003_create_messages.sql          ← fix: was messege
004_create_groups.sql
005_create_group_members.sql
006_indexes.sql
```

### Sample `001_create_users.sql`

```sql
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

CREATE TABLE users (
  id          UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
  username    VARCHAR(50)  UNIQUE NOT NULL,
  email       VARCHAR(255) UNIQUE NOT NULL,
  password_hash TEXT NOT NULL,
  created_at  TIMESTAMPTZ DEFAULT NOW(),
  updated_at  TIMESTAMPTZ DEFAULT NOW(),
  last_seen   TIMESTAMPTZ,
  is_online   BOOLEAN DEFAULT FALSE
);

CREATE INDEX idx_users_username ON users(username);
CREATE INDEX idx_users_email    ON users(email);
```

---

## 7. Redis Setup

### Start Redis

```bash
# Linux
sudo systemctl start redis-server

# macOS
brew services start redis

# Windows
redis-server --service-start
```

### Verify

```bash
redis-cli ping   # should return: PONG
```

### `config/redis.yaml`

```yaml
redis:
  host: localhost
  port: 6379
  password: ""          # set in prod
  db: 0
  pool_size: 10
  connect_timeout_ms: 1000
  socket_timeout_ms: 2000
  max_retries: 3
```

---

## 8. Protobuf Generation

### Linux / macOS

```bash
cd backend
mkdir -p src/generated

protoc \
  --cpp_out=src/generated \
  --proto_path=proto \
  proto/auth.proto \
  proto/messaging.proto \
  proto/groups.proto \
  proto/notifications.proto \
  proto/presence.proto
```

### Windows

```powershell
cd backend
mkdir src\generated -Force

protoc `
  --cpp_out=src\generated `
  --proto_path=proto `
  proto\auth.proto `
  proto\messaging.proto `
  proto\groups.proto `
  proto\notifications.proto `
  proto\presence.proto
```

### Sample `proto/messaging.proto`

```proto
syntax = "proto3";
package chat;

message SendMessageRequest {
  string conversation_id = 1;
  string sender_id       = 2;
  string content         = 3;
  int64  timestamp       = 4;
}

message MessageResponse {
  string message_id = 1;
  bool   delivered  = 2;
  int64  timestamp  = 3;
}
```

---

## 9. Docker

### `infrastructure/docker-compose.yaml`

```yaml
version: "3.9"

services:
  postgres:
    build:
      context: .
      dockerfile: docker/postgres.Dockerfile
    environment:
      POSTGRES_USER: chat_user
      POSTGRES_PASSWORD: your_password
      POSTGRES_DB: chat_db
    ports:
      - "5432:5432"
    volumes:
      - postgres_data:/var/lib/postgresql/data
      - ../backend/migrations:/docker-entrypoint-initdb.d
    healthcheck:
      test: ["CMD-SHELL", "pg_isready -U chat_user -d chat_db"]
      interval: 10s
      timeout: 5s
      retries: 5

  redis:
    build:
      context: .
      dockerfile: docker/redis.Dockerfile
    ports:
      - "6379:6379"
    healthcheck:
      test: ["CMD", "redis-cli", "ping"]
      interval: 10s
      timeout: 3s
      retries: 3

  backend:
    build:
      context: ../backend
      dockerfile: Dockerfile
    ports:
      - "8080:8080"
    environment:
      - ENV=dev
      - DB_HOST=postgres
      - DB_PORT=5432
      - DB_USER=chat_user
      - DB_PASS=your_password
      - DB_NAME=chat_db
      - REDIS_HOST=redis
      - REDIS_PORT=6379
    depends_on:
      postgres:
        condition: service_healthy
      redis:
        condition: service_healthy
    volumes:
      - ../backend/config:/app/config
      - ../backend/logs:/app/logs

volumes:
  postgres_data:
```

### `backend/Dockerfile`

```dockerfile
# --- Build stage ---
FROM ubuntu:24.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential cmake git \
    libboost-all-dev libpqxx-dev libhiredis-dev \
    libssl-dev libspdlog-dev libyaml-cpp-dev \
    nlohmann-json3-dev libsodium-dev \
    protobuf-compiler libprotobuf-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build -- -j$(nproc)

# --- Runtime stage ---
FROM ubuntu:24.04 AS runtime

RUN apt-get update && apt-get install -y \
    libpq5 libhiredis0.14 libssl3 libsodium23 \
    libboost-system1.83.0 libprotobuf32 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /app/build/chat_server .
COPY --from=builder /app/config ./config

EXPOSE 8080
CMD ["./chat_server", "--config", "config/prod.yaml"]
```

### Start / stop services

```bash
cd infrastructure

# Start all
docker compose up -d

# Start only dependencies (postgres + redis), run backend locally
docker compose up -d postgres redis

# Stop all
docker compose down

# Stop and wipe volumes (nuclear reset)
docker compose down -v

# View logs
docker compose logs -f backend
```

---

## 10. Running the Server

### Local (no Docker)

```bash
# Set env vars
export DB_HOST=localhost
export DB_PORT=5432
export DB_USER=chat_user
export DB_PASS=your_password
export DB_NAME=chat_db
export REDIS_HOST=localhost
export REDIS_PORT=6379

# Run
./backend/build/debug/chat_server --config backend/config/dev.yaml
```

### Windows

```powershell
$env:DB_HOST="localhost"; $env:DB_PORT="5432"
$env:DB_USER="chat_user"; $env:DB_PASS="your_password"
$env:DB_NAME="chat_db"; $env:REDIS_HOST="localhost"; $env:REDIS_PORT="6379"

.\backend\build\Release\chat_server.exe --config backend\config\dev.yaml
```

### `config/dev.yaml` skeleton

```yaml
server:
  host: 0.0.0.0
  port: 8080
  threads: 4
  max_connections: 1000

database:
  host: ${DB_HOST}
  port: ${DB_PORT}
  user: ${DB_USER}
  password: ${DB_PASS}
  dbname: ${DB_NAME}
  pool_size: 10

redis:
  host: ${REDIS_HOST}
  port: ${REDIS_PORT}
  pool_size: 10

jwt:
  secret: "change_me_in_prod"
  expiry_seconds: 86400

logging:
  level: debug
  file: logs/app.log
```

---

## 11. Testing

### Unit tests

```bash
# Build and run
cmake --build backend/build --target run_tests
cd backend/build && ctest --output-on-failure

# Run specific test suite
./backend/build/run_tests --gtest_filter="AuthService.*"
```

### Example unit test (write this — don't leave tests empty)

```cpp
// backend/tests/unit/auth_test.cpp
#include <gtest/gtest.h>
#include "auth/password_hasher.hpp"

TEST(PasswordHasher, HashAndVerify) {
  PasswordHasher hasher;
  std::string password = "SecurePass123!";
  std::string hash     = hasher.hash(password);

  EXPECT_NE(hash, password);
  EXPECT_TRUE(hasher.verify(password, hash));
  EXPECT_FALSE(hasher.verify("WrongPass", hash));
}

TEST(PasswordHasher, DifferentHashesSamePassword) {
  PasswordHasher hasher;
  std::string h1 = hasher.hash("password");
  std::string h2 = hasher.hash("password");
  EXPECT_NE(h1, h2);  // bcrypt uses random salt
}
```

### Load tests

```bash
# 1k users
./backend/tests/load/1k_users_test --host localhost --port 8080 --duration 60s

# 10k users
./backend/tests/load/10k_users_test --host localhost --port 8080 --duration 120s

# Stress (run until failure)
./backend/tests/load/stress_test --host localhost --port 8080
```

> **These folders are empty right now.** Use [k6](https://k6.io/) or write custom C++ load test clients with Boost.Beast. Don't pretend load tests exist when they don't.

---

## 12. Cross-Platform Scripts

Replace `.bat`-only scripts with cross-platform `Makefile`:

```makefile
# Makefile (repo root)
.PHONY: build clean rebuild start stop test migrate proto

build:
	cmake -B backend/build -DCMAKE_BUILD_TYPE=Debug
	cmake --build backend/build -- -j$(shell nproc 2>/dev/null || echo 4)

clean:
	rm -rf backend/build

rebuild: clean build

start:
	docker compose -f infrastructure/docker-compose.yaml up -d

stop:
	docker compose -f infrastructure/docker-compose.yaml down

migrate:
	@for f in backend/migrations/*.sql; do \
	  echo "Running $$f..."; \
	  psql -U chat_user -d chat_db -f $$f; \
	done

proto:
	protoc --cpp_out=backend/src/generated \
	  --proto_path=backend/proto \
	  backend/proto/*.proto

test:
	cmake --build backend/build --target run_tests
	cd backend/build && ctest --output-on-failure

test-load:
	./backend/tests/load/1k_users_test
```

Windows users: use `nmake` or WSL2, or keep `.bat` as wrappers that call `cmake` directly.

---

## 13. Common Errors

| Error | Cause | Fix |
|-------|-------|-----|
| `Could not find Boost` | Boost not installed or not in PATH | `apt install libboost-all-dev` or set `BOOST_ROOT` |
| `libpq not found` | PostgreSQL dev headers missing | `apt install libpq-dev` |
| `hiredis/hiredis.h: No such file` | hiredis missing | `apt install libhiredis-dev` |
| `protoc: command not found` | Protobuf compiler not installed | `apt install protobuf-compiler` |
| `Connection refused 5432` | PostgreSQL not running | `sudo systemctl start postgresql` |
| `PONG not received` | Redis not running | `sudo systemctl start redis-server` |
| `jwt-cpp not found` | Not in vcpkg or system | `vcpkg install jwt-cpp` |
| CMake can't find vcpkg packages | Toolchain file missing | Add `-DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake` |
| WebSocket handshake failure | Boost.Beast version mismatch | Ensure Boost ≥ 1.82 |
| `duplicate symbol` linker error | `main.cpp` included in library target | Remove `src/main.cpp` from `GLOB_RECURSE` in CMake |

---

## Priority Build Order

Build in this sequence — each layer depends on the one before:

```
1. Fix typos          ← do this RIGHT NOW
2. CMake + compile    ← verify the project builds clean
3. Postgres up        ← run migrations, verify schema
4. Redis up           ← verify connection
5. Proto generation   ← build .pb.cc/.pb.h files
6. Backend runs       ← connects to DB + Redis on startup
7. WebSocket test     ← connect a client, send a message
8. Write unit tests   ← auth, message, presence at minimum
9. Docker compose     ← everything in containers
10. Load tests        ← only after above all pass
```

Don't skip steps. Don't run load tests on untested code.
