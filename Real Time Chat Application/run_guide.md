# Running the Chat Server
> Windows · Docker Desktop · MSYS2/UCRT64 · Step-by-step

---

## Overview

The strategy is:
- **PostgreSQL + Redis** run inside Docker Desktop (zero config, zero install)
- **C++ backend** is compiled natively on your machine using MSYS2/UCRT64 and run as a normal `.exe`
- Migrations run automatically when the Postgres container first starts

```
Docker Desktop
├── postgres:16  (port 5432)
└── redis:7      (port 6379)

Your Machine (MSYS2/UCRT64)
└── chat_server.exe  (port 8080)  ←── connects to both containers
```

---

## Step 1 — Verify Docker Desktop is Running

Open Docker Desktop. Make sure the whale icon in the taskbar shows **"Docker Desktop is running"**.

Then open a terminal (any — PowerShell, CMD, or MSYS2) and verify:

```bash
docker --version
docker compose version
```

Both must return a version number. If `docker compose` fails, update Docker Desktop — it ships compose built-in since v2.

---

## Step 2 — Open MSYS2 UCRT64

Open the **MSYS2 UCRT64** terminal (not MINGW64, not MSYS — specifically **UCRT64**).

You can find it in Start Menu → MSYS2 → MSYS2 UCRT64.

Update the package database first (do this once):

```bash
pacman -Syu
# If it closes the window, reopen UCRT64 and run again:
pacman -Su
```

---

## Step 3 — Install All C++ Dependencies via MSYS2

Run this single block in the UCRT64 terminal. It installs the compiler, CMake, and every library the project needs:

```bash
pacman -S --needed \
  mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-cmake \
  mingw-w64-ucrt-x86_64-ninja \
  mingw-w64-ucrt-x86_64-boost \
  mingw-w64-ucrt-x86_64-openssl \
  mingw-w64-ucrt-x86_64-protobuf \
  mingw-w64-ucrt-x86_64-spdlog \
  mingw-w64-ucrt-x86_64-yaml-cpp \
  mingw-w64-ucrt-x86_64-libpqxx \
  mingw-w64-ucrt-x86_64-hiredis \
  mingw-w64-ucrt-x86_64-nlohmann-json \
  mingw-w64-ucrt-x86_64-libsodium \
  mingw-w64-ucrt-x86_64-jwt-cpp \
  mingw-w64-ucrt-x86_64-gtest \
  mingw-w64-ucrt-x86_64-pkg-config \
  mingw-w64-ucrt-x86_64-libuuid \
  git
```

Verify the compiler works:

```bash
g++ --version   # should print GCC 13+
cmake --version # should print 3.20+
```

---

## Step 4 — Fix the UUID include in utils.cpp

`libuuid` on MSYS2/UCRT64 has a different header path than Linux. Open
`backend/src/common/utils.cpp` and make sure the top looks like this:

```cpp
#include "common/utils.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <algorithm>
#include <cctype>
#include <regex>

namespace chat::utils {

std::string generate_uuid() {
    boost::uuids::random_generator gen;
    return boost::uuids::to_string(gen());
}

// ... rest of the file unchanged
```

This replaces the Linux-only `libuuid` with Boost.UUID which is already installed.

---

## Step 5 — Start PostgreSQL and Redis via Docker

In **any terminal** (PowerShell or CMD is fine here), navigate to the infrastructure folder:

```powershell
cd "C:\Users\ranab\OneDrive\Desktop\DSA Projects\Real Time Chat Application\infrastructure"
docker compose up -d postgres redis
```

Wait about 15 seconds, then verify both are healthy:

```powershell
docker compose ps
```

You should see:

```
NAME                    STATUS
infrastructure-postgres-1   Up (healthy)
infrastructure-redis-1      Up (healthy)
```

If postgres shows `Up (health: starting)`, wait another 10 seconds and run `docker compose ps` again.

> **Migrations run automatically.** The `docker-compose.yaml` mounts
> `backend/migrations/` into `/docker-entrypoint-initdb.d/` inside the
> Postgres container. All 6 `.sql` files run in filename order on first start.
> You do NOT need to run them manually.

Verify migrations ran:

```powershell
docker exec -it infrastructure-postgres-1 psql -U chat_user -d chat_db -c "\dt"
```

You should see tables: `users`, `conversations`, `conversation_participants`, `messages`, `groups`, `group_members`.

---

## Step 6 — Build the Backend

Switch to the **MSYS2 UCRT64** terminal.

Navigate to the backend folder using the MSYS2 path format:

```bash
cd "/c/Users/ranab/OneDrive/Desktop/DSA Projects/Real Time Chat Application/backend"
```

Configure with CMake (using Ninja for faster builds):

```bash
cmake -B build \
  -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_COMPILER=g++
```

Build:

```bash
cmake --build build
```

A successful build ends with:

```
[100%] Linking CXX executable chat_server.exe
[100%] Built target chat_server
```

If you see errors, check the [Troubleshooting](#troubleshooting) section at the bottom.

---

## Step 7 — Run the Server

Still in the MSYS2 UCRT64 terminal, set environment variables and start the server:

```bash
export DB_HOST=127.0.0.1
export DB_PORT=5433
export DB_USER=chat_user
export DB_PASS=your_password
export DB_NAME=chat_db
export REDIS_HOST=127.0.0.1
export REDIS_PORT=6379

./build/chat_server.exe --config config/dev.yaml
```

You should see:

```
[info] Starting chat server
[info] Listening on 0.0.0.0:8080
```

The server is now running. Keep this terminal open.

---

## Step 8 — Test the WebSocket Connection

Open a **new terminal** (PowerShell or CMD) and install `wscat`:

```powershell
npm install -g wscat
```

Connect to the server:

```powershell
wscat -c ws://localhost:8080
```

You should see `Connected (press CTRL+C to quit)`.

Now send the auth message (you need a valid session token — for now test the error path):

```json
{"type":"auth","token":"invalid_token"}
```

The server should close the connection (expected — no valid token yet).

To get a real token you need to call `AuthService::login` — this will be wired to an HTTP endpoint or a dedicated auth WebSocket message in a future iteration.

---

## Step 9 — Verify Postgres and Redis Directly

**Check Postgres:**

```powershell
docker exec -it infrastructure-postgres-1 psql -U chat_user -d chat_db
```

```sql
SELECT COUNT(*) FROM users;
\q
```

**Check Redis:**

```powershell
docker exec -it infrastructure-redis-1 redis-cli
```

```
PING          -- returns PONG
KEYS *        -- shows any keys set by the server
```

---

## Stopping Everything

```powershell
# Stop the server
# Press CTRL+C in the MSYS2 terminal running chat_server.exe

# Stop Docker containers (keeps data)
cd "C:\Users\ranab\OneDrive\Desktop\DSA Projects\Real Time Chat Application\infrastructure"
docker compose down

# Stop and wipe all data (fresh start)
docker compose down -v
```

---

## Daily Workflow

Once everything is set up, your daily workflow is just 3 commands:

```powershell
# Terminal 1 — start infrastructure
cd "...\infrastructure"
docker compose up -d postgres redis
```

```bash
# Terminal 2 — MSYS2 UCRT64 — build and run
cd "/c/Users/ranab/OneDrive/Desktop/DSA Projects/Real Time Chat Application/backend"
cmake --build build
export DB_HOST=localhost DB_PORT=5432 DB_USER=chat_user DB_PASS=your_password DB_NAME=chat_db REDIS_HOST=localhost REDIS_PORT=6379
./build/chat_server.exe --config config/dev.yaml
```

---

## Troubleshooting

### `Could not find spdlog / yaml-cpp / hiredis`

CMake is not finding MSYS2 packages. Add the MSYS2 prefix path explicitly:

```bash
cmake -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_PREFIX_PATH="/ucrt64" \
  -DCMAKE_C_COMPILER=gcc \
  -DCMAKE_CXX_COMPILER=g++
```

### `pkg-config not finding libsodium`

```bash
export PKG_CONFIG_PATH="/ucrt64/lib/pkgconfig"
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH="/ucrt64"
```

### `libpqxx not found`

```bash
pacman -S mingw-w64-ucrt-x86_64-libpqxx
```

### `jwt-cpp not found`

```bash
pacman -S mingw-w64-ucrt-x86_64-jwt-cpp
```

If not in pacman, install manually:

```bash
git clone https://github.com/Thalhammer/jwt-cpp.git /tmp/jwt-cpp
cd /tmp/jwt-cpp
cmake -B build -G Ninja -DCMAKE_INSTALL_PREFIX=/ucrt64 -DJWT_BUILD_EXAMPLES=OFF
cmake --build build
cmake --install build
```

### `Connection refused` on port 5432 or 6379

Docker containers are not running. Check:

```powershell
docker compose ps
docker compose logs postgres
docker compose logs redis
```

### `duplicate symbol` linker error

`src/main.cpp` is being included in `chat_lib`. Check that `CMakeLists.txt` has:

```cmake
list(REMOVE_ITEM ALL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
```

### Postgres container exits immediately

The migrations folder path in `docker-compose.yaml` uses a relative path. Make sure you run `docker compose` from inside the `infrastructure/` folder, not from the repo root.

### Clean rebuild

```bash
rm -rf build
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH="/ucrt64"
cmake --build build
```

---

## Port Reference

| Service | Host Port | Container Port |
|---------|-----------|----------------|
| Chat Server | 8080 | — (native) |
| PostgreSQL | 5432 | 5432 |
| Redis | 6379 | 6379 |
