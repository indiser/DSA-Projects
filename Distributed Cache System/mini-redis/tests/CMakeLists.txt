include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_lru test_lru.cpp
    ../src/cache/LRUPolicy.cpp
)
target_link_libraries(test_lru gtest_main)
add_test(NAME test_lru COMMAND test_lru)

add_executable(test_lfu test_lfu.cpp
    ../src/cache/LFUPolicy.cpp
)
target_link_libraries(test_lfu gtest_main)
add_test(NAME test_lfu COMMAND test_lfu)

add_executable(test_cache_engine test_cache_engine.cpp
    ../src/cache/CacheEngine.cpp
    ../src/cache/LRUPolicy.cpp
    ../src/cache/LFUPolicy.cpp
    ../src/storage/InMemoryStorage.cpp
)
target_link_libraries(test_cache_engine gtest_main)
add_test(NAME test_cache_engine COMMAND test_cache_engine)

add_executable(test_storage test_storage.cpp
    ../src/storage/InMemoryStorage.cpp
)
target_link_libraries(test_storage gtest_main)
add_test(NAME test_storage COMMAND test_storage)

add_executable(test_protocol test_protocol.cpp
    ../src/network/Protocol.cpp
)
target_link_libraries(test_protocol gtest_main)
add_test(NAME test_protocol COMMAND test_protocol)

add_executable(test_consistent_hash test_consistent_hash.cpp
    ../src/utils/ConsistentHash.cpp
)
target_link_libraries(test_consistent_hash gtest_main)
add_test(NAME test_consistent_hash COMMAND test_consistent_hash)
