cmake_minimum_required(VERSION 3.14)
project(Valkeyrie VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_executable(valkeyrie
    src/main.cpp
    src/cache/LRUPolicy.cpp
    src/cache/LFUPolicy.cpp
    src/cache/CacheEngine.cpp
    src/storage/InMemoryStorage.cpp
    src/network/Server.cpp
    src/network/Protocol.cpp
    src/utils/ThreadPool.cpp
    src/utils/ConsistentHash.cpp
)

if(WIN32)
    target_link_libraries(valkeyrie ws2_32)
endif()

enable_testing()
add_subdirectory(tests)
