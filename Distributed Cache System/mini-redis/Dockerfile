# Multi-stage build for Valkeyrie Distributed Cache System
FROM gcc:13 AS builder

# Install dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source files
COPY include/ ./include/
COPY src/ ./src/
COPY tests/ ./tests/
COPY CMakeLists.txt .

# Create build directory and build
RUN mkdir build && cd build && \
    cmake .. && \
    cmake --build . -j$(nproc)

# Final stage - minimal image
FROM debian:bookworm-slim

# Install runtime dependencies only
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user
RUN useradd -m -u 1000 valkeyrie

# Copy binary from builder
COPY --from=builder /app/build/valkeyrie /usr/local/bin/valkeyrie

# Switch to non-root user
USER valkeyrie

# Expose default port
EXPOSE 6379

# Set default configuration
ENV VALKEYRIE_HOST=0.0.0.0 \
    VALKEYRIE_PORT=6379 \
    VALKEYRIE_CAPACITY=10000 \
    VALKEYRIE_POLICY=LRU \
    VALKEYRIE_THREADS=4

# Start Valkeyrie
CMD ["sh", "-c", "valkeyrie --host $VALKEYRIE_HOST --port $VALKEYRIE_PORT --capacity $VALKEYRIE_CAPACITY --policy $VALKEYRIE_POLICY --threads $VALKEYRIE_THREADS"]
