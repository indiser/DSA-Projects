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