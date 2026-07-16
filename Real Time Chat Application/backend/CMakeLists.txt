cmake_minimum_required(VERSION 3.20)
project(ChatServer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Suppress FindBoost deprecation warning (Boost 1.82+ ships its own config)
cmake_policy(SET CMP0167 NEW)

# --- Find packages ---
find_package(Boost        REQUIRED COMPONENTS system thread)
find_package(OpenSSL      REQUIRED)
find_package(Protobuf     REQUIRED)
find_package(spdlog       REQUIRED)
find_package(yaml-cpp     REQUIRED)
find_package(libpqxx      REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(GTest        REQUIRED)

# hiredis ships its own cmake config in newer versions; fall back to pkg-config
find_package(hiredis QUIET)
if(NOT hiredis_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(hiredis REQUIRED IMPORTED_TARGET hiredis)
    add_library(hiredis::hiredis ALIAS PkgConfig::hiredis)
endif()

# libsodium has no official cmake config; use pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(libsodium REQUIRED IMPORTED_TARGET libsodium)

# --- Protobuf generation (only if .proto files exist and are non-empty) ---
file(GLOB PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto")
set(PROTO_SRCS "")
set(PROTO_HDRS "")
if(PROTO_FILES)
    protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
endif()

# --- Collect sources (exclude main.cpp) ---
file(GLOB_RECURSE ALL_SOURCES "src/*.cpp")
list(REMOVE_ITEM ALL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Filter out empty stub files at configure time is not possible,
# but we ensure the library always has at least one translation unit.
set(SOURCES ${ALL_SOURCES} ${PROTO_SRCS})

# --- Library target (reused by tests) ---
add_library(chat_lib ${SOURCES})
target_include_directories(chat_lib PUBLIC
    include
    ${CMAKE_CURRENT_BINARY_DIR}   # generated proto headers
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/../jwt-cpp/include
)
target_link_libraries(chat_lib PUBLIC
    Boost::system
    Boost::thread
    OpenSSL::SSL
    OpenSSL::Crypto
    protobuf::libprotobuf
    spdlog::spdlog
    yaml-cpp::yaml-cpp
    libpqxx::pqxx
    hiredis::hiredis
    hiredis::hiredis_ssl
    PkgConfig::libsodium
    nlohmann_json::nlohmann_json
)

# --- Executable ---
add_executable(chat_server src/main.cpp)
target_link_libraries(chat_server PRIVATE chat_lib)

# --- Tests ---
add_subdirectory(tests)

# --- Compiler warnings ---
if(MSVC)
    target_compile_options(chat_lib PRIVATE /W4)
    target_compile_options(chat_server PRIVATE /W4)
else()
    target_compile_options(chat_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(chat_server PRIVATE -Wall -Wextra -Wpedantic)
endif()
