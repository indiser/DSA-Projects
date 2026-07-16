.
├── Problems
│   ├── CloneGraph.cpp
│   ├── HitCounter.cpp
│   ├── LRUcache.cpp
│   ├── MedianFromDataStream.cpp
│   ├── MergeKSortedLists.cpp
│   ├── NetworkDelayTime.cpp
│   ├── NumberOfIslands.cpp
│   ├── TaskScheduler.cpp
│   └── TopKFreq.cpp
├── Readme.md
├── architecture.md
├── backend
│   ├── CMakeLists.txt
│   ├── Dockerfile
│   ├── build
│   │   ├── .ninja_deps
│   │   ├── .ninja_log
│   │   ├── CMakeCache.txt
│   │   ├── CMakeFiles
│   │   │   ├── 4.4.0
│   │   │   │   ├── CMakeCXXCompiler.cmake
│   │   │   │   ├── CMakeDetermineCompilerABI_CXX.bin
│   │   │   │   ├── CMakeRCCompiler.cmake
│   │   │   │   ├── CMakeSystem.cmake
│   │   │   │   └── CompilerIdCXX
│   │   │   │       ├── CMakeCXXCompilerId.cpp
│   │   │   │       ├── a.exe
│   │   │   │       └── tmp
│   │   │   ├── CMakeConfigureLog.yaml
│   │   │   ├── InstallScripts.json
│   │   │   ├── TargetDirectories.txt
│   │   │   ├── chat_lib.dir
│   │   │   │   └── src
│   │   │   │       ├── analytics
│   │   │   │       │   ├── metrics.cpp.obj
│   │   │   │       │   ├── monitoring.cpp.obj
│   │   │   │       │   └── rate_limiter.cpp.obj
│   │   │   │       ├── auth
│   │   │   │       │   ├── password_hasher.cpp.obj
│   │   │   │       │   └── session_manager.cpp.obj
│   │   │   │       ├── cache
│   │   │   │       │   ├── cache_manager.cpp.obj
│   │   │   │       │   ├── lru_cache.cpp.obj
│   │   │   │       │   └── redis_client.cpp.obj
│   │   │   │       ├── common
│   │   │   │       │   ├── constants.cpp.obj
│   │   │   │       │   ├── errors.cpp.obj
│   │   │   │       │   ├── types.cpp.obj
│   │   │   │       │   └── utils.cpp.obj
│   │   │   │       ├── database
│   │   │   │       │   ├── db_pool.cpp.obj
│   │   │   │       │   ├── postgres_client.cpp.obj
│   │   │   │       │   └── repositories
│   │   │   │       │       ├── group_repository.cpp.obj
│   │   │   │       │       └── messege_repository.cpp.obj
│   │   │   │       ├── graph
│   │   │   │       │   ├── friendship_graph.cpp.obj
│   │   │   │       │   ├── recommendation_engine.cpp.obj
│   │   │   │       │   └── shortest_path.cpp.obj
│   │   │   │       ├── groups
│   │   │   │       │   ├── group_service.cpp.obj
│   │   │   │       │   └── permissions.cpp.obj
│   │   │   │       ├── logging
│   │   │   │       │   ├── log_formatter.cpp.obj
│   │   │   │       │   ├── log_manager.cpp.obj
│   │   │   │       │   └── logger.cpp.obj
│   │   │   │       ├── messaging
│   │   │   │       │   ├── delivery_manager.cpp.obj
│   │   │   │       │   └── message_dispatcher.cpp.obj
│   │   │   │       ├── notifications
│   │   │   │       ├── presence
│   │   │   │       ├── search
│   │   │   │       └── websocket
│   │   │   ├── chat_server.dir
│   │   │   │   └── src
│   │   │   ├── cmake.check_cache
│   │   │   ├── pkgRedirects
│   │   │   └── rules.ninja
│   │   ├── auth.pb.cc
│   │   ├── auth.pb.h
│   │   ├── build.ninja
│   │   ├── cmake_install.cmake
│   │   ├── compile_commands.json
│   │   ├── groups.pb.cc
│   │   ├── groups.pb.h
│   │   ├── messeging.pb.cc
│   │   ├── messeging.pb.h
│   │   ├── notifications.pb.cc
│   │   ├── notifications.pb.h
│   │   ├── presence.pb.cc
│   │   ├── presence.pb.h
│   │   └── tests
│   │       ├── CMakeFiles
│   │       ├── CTestTestfile.cmake
│   │       └── cmake_install.cmake
│   ├── config
│   │   ├── dev.yaml
│   │   ├── logging.yaml
│   │   ├── prod.yaml
│   │   └── redis.yaml
│   ├── include
│   │   ├── analytics
│   │   │   ├── metrics.hpp
│   │   │   ├── monitoring.hpp
│   │   │   └── rate_limiter.hpp
│   │   ├── auth
│   │   │   ├── auth_service.hpp
│   │   │   ├── jwt_manager.hpp
│   │   │   ├── password_hasher.hpp
│   │   │   └── session_manager.hpp
│   │   ├── cache
│   │   │   ├── cache_manager.hpp
│   │   │   ├── lru_cache.hpp
│   │   │   └── redis_client.hpp
│   │   ├── common
│   │   │   ├── constants.hpp
│   │   │   ├── errors.hpp
│   │   │   ├── types.hpp
│   │   │   └── utils.hpp
│   │   ├── database
│   │   │   ├── db_pool.hpp
│   │   │   ├── postgres_client.hpp
│   │   │   └── repositories
│   │   │       ├── conversation_repository.hpp
│   │   │       ├── group_repository.hpp
│   │   │       ├── messege_repository.hpp
│   │   │       └── user_repository.hpp
│   │   ├── graph
│   │   │   ├── friendship_graph.hpp
│   │   │   ├── recommendation_engine.hpp
│   │   │   └── shortest_path.hpp
│   │   ├── groups
│   │   │   ├── group_manager.hpp
│   │   │   ├── group_service.hpp
│   │   │   └── permissions.hpp
│   │   ├── logging
│   │   │   ├── log_formatter.hpp
│   │   │   ├── log_manager.hpp
│   │   │   └── logger.hpp
│   │   ├── messaging
│   │   │   ├── delivery_manager.hpp
│   │   │   ├── message_dispatcher.hpp
│   │   │   ├── message_ordering.hpp
│   │   │   ├── message_queue.hpp
│   │   │   └── message_service.hpp
│   │   ├── notifications
│   │   │   ├── notification_service.hpp
│   │   │   └── push_manager.hpp
│   │   ├── presence
│   │   │   ├── online_tracker.hpp
│   │   │   ├── presence_service.hpp
│   │   │   └── typing_tracker.hpp
│   │   ├── search
│   │   │   ├── inverted_index.hpp
│   │   │   ├── search_service.hpp
│   │   │   └── trie.hpp
│   │   └── websocket
│   │       ├── connection_manager.hpp
│   │       ├── websocket_server.hpp
│   │       └── websocket_session.hpp
│   ├── logs
│   │   ├── app.log
│   │   ├── auth.log
│   │   ├── database.log
│   │   ├── error.log
│   │   ├── messeges.log
│   │   └── websocket.log
│   ├── migrations
│   │   ├── 001_create_users.sql
│   │   ├── 002_create_conversations.sql
│   │   ├── 003_create_messages.sql
│   │   ├── 004_create_groups.sql
│   │   ├── 005_create_group_members.sql
│   │   └── 006_indexes.sql
│   ├── proto
│   │   ├── auth.proto
│   │   ├── groups.proto
│   │   ├── messeging.proto
│   │   ├── notifications.proto
│   │   └── presence.proto
│   ├── src
│   │   ├── analytics
│   │   │   ├── metrics.cpp
│   │   │   ├── monitoring.cpp
│   │   │   └── rate_limiter.cpp
│   │   ├── auth
│   │   │   ├── auth_service.cpp
│   │   │   ├── jwt_manager.cpp
│   │   │   ├── password_hasher.cpp
│   │   │   └── session_manager.cpp
│   │   ├── cache
│   │   │   ├── cache_manager.cpp
│   │   │   ├── lru_cache.cpp
│   │   │   └── redis_client.cpp
│   │   ├── common
│   │   │   ├── constants.cpp
│   │   │   ├── errors.cpp
│   │   │   ├── types.cpp
│   │   │   └── utils.cpp
│   │   ├── database
│   │   │   ├── db_pool.cpp
│   │   │   ├── postgres_client.cpp
│   │   │   └── repositories
│   │   │       ├── conversation_repository.cpp
│   │   │       ├── group_repository.cpp
│   │   │       ├── messege_repository.cpp
│   │   │       └── user_repository.cpp
│   │   ├── graph
│   │   │   ├── friendship_graph.cpp
│   │   │   ├── recommendation_engine.cpp
│   │   │   └── shortest_path.cpp
│   │   ├── groups
│   │   │   ├── group_manager.cpp
│   │   │   ├── group_service.cpp
│   │   │   └── permissions.cpp
│   │   ├── logging
│   │   │   ├── log_formatter.cpp
│   │   │   ├── log_manager.cpp
│   │   │   └── logger.cpp
│   │   ├── main.cpp
│   │   ├── messaging
│   │   │   ├── delivery_manager.cpp
│   │   │   ├── message_dispatcher.cpp
│   │   │   ├── message_ordering.cpp
│   │   │   ├── message_queue.cpp
│   │   │   └── message_service.cpp
│   │   ├── notifications
│   │   │   ├── notification_service.cpp
│   │   │   └── push_manager.cpp
│   │   ├── presence
│   │   │   ├── online_tracker.cpp
│   │   │   ├── presence_service.cpp
│   │   │   └── typing_tracker.cpp
│   │   ├── search
│   │   │   ├── inverted_index.cpp
│   │   │   ├── search_service.cpp
│   │   │   └── trie.cpp
│   │   └── websocket
│   │       ├── connection_manager.cpp
│   │       ├── websocket_server.cpp
│   │       └── websocket_session.cpp
│   └── tests
│       ├── CMakeLists.txt
│       ├── benchmarks
│       ├── integration
│       ├── load
│       │   ├── 10k_users_test
│       │   ├── 1k_users_test
│       │   └── stress_test
│       └── unit
├── build_guide.md
├── chat_server.jpg
├── docs
│   ├── api.md
│   ├── architecture.md
│   ├── database_design.md
│   └── deployment.md
├── infrastructure
│   ├── docker
│   │   ├── backend.Dockerfile
│   │   ├── postgres.Dockerfile
│   │   └── redis.Dockerfile
│   └── docker-compose.yaml
├── jwt-cpp
│   ├── .clang-format
│   ├── .clang-tidy
│   ├── .cmake-format
│   ├── .editorconfig
│   ├── .gitattributes
│   ├── .github
│   │   ├── FUNDING.yml
│   │   ├── ISSUE_TEMPLATE
│   │   │   ├── bug-report.yml
│   │   │   ├── feature-request.yml
│   │   │   └── support-question.yml
│   │   ├── actions
│   │   │   ├── badge
│   │   │   │   ├── action.yml
│   │   │   │   ├── publish
│   │   │   │   │   └── action.yml
│   │   │   │   └── write
│   │   │   │       └── action.yml
│   │   │   ├── generate-defaults-dot-h
│   │   │   │   ├── action.yml
│   │   │   │   └── configure-traits.sh
│   │   │   ├── install
│   │   │   │   ├── boost-json
│   │   │   │   │   ├── action.yml
│   │   │   │   │   └── install-boost-json.sh
│   │   │   │   ├── cmake
│   │   │   │   │   └── action.yml
│   │   │   │   ├── danielaparker-jsoncons
│   │   │   │   │   ├── action.yml
│   │   │   │   │   └── install-jsoncons.sh
│   │   │   │   ├── glaze-json
│   │   │   │   │   ├── action.yml
│   │   │   │   │   └── install-glaze.sh
│   │   │   │   ├── gtest
│   │   │   │   │   ├── action.yml
│   │   │   │   │   └── install-gtest.sh
│   │   │   │   ├── kazuho-picojson
│   │   │   │   │   └── action.yml
│   │   │   │   ├── libressl
│   │   │   │   │   ├── action.yml
│   │   │   │   │   └── install-libressl.sh
│   │   │   │   ├── nlohmann-json
│   │   │   │   │   └── action.yml
│   │   │   │   ├── open-source-parsers-jsoncpp
│   │   │   │   │   ├── action.yml
│   │   │   │   │   └── install-jsoncpp.sh
│   │   │   │   ├── openssl
│   │   │   │   │   └── action.yml
│   │   │   │   ├── reflectcpp-json
│   │   │   │   │   ├── action.yml
│   │   │   │   │   └── install-reflect-cpp.sh
│   │   │   │   └── wolfssl
│   │   │   │       ├── action.yml
│   │   │   │       └── install-wolfssl.sh
│   │   │   └── process-linting-results
│   │   │       └── action.yml
│   │   ├── logo.svg
│   │   ├── release.md
│   │   ├── security.md
│   │   └── workflows
│   │       ├── cmake.yml
│   │       ├── cross-platform.yml
│   │       ├── documentation.yml
│   │       ├── jwt.yml
│   │       ├── lint.yml
│   │       ├── release.yml
│   │       ├── ssl.yml
│   │       ├── targets.yml
│   │       └── traits.yml
│   ├── .gitignore
│   ├── .vscode
│   │   └── extensions.json
│   ├── CMakeLists.txt
│   ├── CMakePresets.json
│   ├── LICENSE
│   ├── README.md
│   ├── cmake
│   │   ├── CMakePresets.json
│   │   ├── HunterGate.cmake
│   │   ├── code-coverage.cmake
│   │   ├── generate-defaults-h.cmake
│   │   ├── jwt-cpp-config.cmake.in
│   │   └── private-find-kazuho-picojson.cmake
│   ├── docs
│   │   ├── .gitignore
│   │   ├── CMakeLists.txt
│   │   ├── Doxyfile
│   │   ├── faqs.md
│   │   ├── install.md
│   │   ├── overrides.css
│   │   ├── signing.md
│   │   ├── ssl.md
│   │   └── traits.md
│   ├── example
│   │   ├── CMakeLists.txt
│   │   ├── CMakePresets.json
│   │   ├── es256k.cpp
│   │   ├── jwks-verify.cpp
│   │   ├── partial-claim-verifier.cpp
│   │   ├── print-claims.cpp
│   │   ├── private-claims.cpp
│   │   ├── rsa-create.cpp
│   │   ├── rsa-verify.cpp
│   │   └── traits
│   │       ├── CMakeLists.txt
│   │       ├── README.md
│   │       ├── boost-json.cpp
│   │       ├── danielaparker-jsoncons.cpp
│   │       ├── glaze-json.cpp
│   │       ├── kazuho-picojson.cpp
│   │       ├── nlohmann-json.cpp
│   │       ├── open-source-parsers-jsoncpp.cpp
│   │       └── reflectcpp-json.cpp
│   ├── include
│   │   ├── jwt-cpp
│   │   │   ├── base.h
│   │   │   ├── jwt.h
│   │   │   └── traits
│   │   │       ├── boost-json
│   │   │       │   ├── defaults.h
│   │   │       │   └── traits.h
│   │   │       ├── danielaparker-jsoncons
│   │   │       │   ├── defaults.h
│   │   │       │   └── traits.h
│   │   │       ├── defaults.h.in
│   │   │       ├── glaze-json
│   │   │       │   ├── defaults.h
│   │   │       │   └── traits.h
│   │   │       ├── kazuho-picojson
│   │   │       │   ├── defaults.h
│   │   │       │   └── traits.h
│   │   │       ├── nlohmann-json
│   │   │       │   ├── defaults.h
│   │   │       │   └── traits.h
│   │   │       ├── open-source-parsers-jsoncpp
│   │   │       │   ├── defaults.h
│   │   │       │   └── traits.h
│   │   │       └── reflectcpp-json
│   │   │           ├── defaults.h
│   │   │           └── traits.h
│   │   └── picojson
│   │       └── picojson.h
│   ├── nuget
│   │   ├── jwt-cpp.nuspec
│   │   └── jwt-cpp.targets
│   └── tests
│       ├── BaseTest.cpp
│       ├── CMakeLists.txt
│       ├── CMakePresets.json
│       ├── ClaimTest.cpp
│       ├── HelperTest.cpp
│       ├── JwksTest.cpp
│       ├── Keys.cpp
│       ├── OpenSSLErrorTest.cpp
│       ├── TestMain.cpp
│       ├── TokenTest.cpp
│       ├── cmake
│       │   ├── CMakeLists.txt
│       │   ├── base64-is-disabled.cpp
│       │   ├── defaults-enabled.cpp
│       │   ├── libressl-is-used.cpp
│       │   ├── picojson-is-disabled.cpp
│       │   └── wolfssl-is-used.cpp
│       ├── fuzz
│       │   ├── BaseDecodeFuzz.cpp
│       │   ├── BaseEncodeFuzz.cpp
│       │   ├── CMakeLists.txt
│       │   ├── TokenDecodeFuzz.cpp
│       │   ├── decode-corpus
│       │   │   ├── 086a3aa337038cac8a75a05131444f222e48aee8
│       │   │   ├── 8ebaef2304e91465585c8d7fcf4d9f939e08d6b4
│       │   │   ├── ba528234d9f6949ed9c9626c08a782f6e7c15b8b
│       │   │   ├── de1028a3fe87471f027522c3ed9ec02b8364a006
│       │   │   └── e8f531caaa67cecb1c7b162f3e1d4e320d79befd
│       │   └── token-corpus
│       │       ├── 9d891e731f75deae56884d79e9816736b7488080
│       │       ├── ff384e2421a333cd52f259cec14c7f790d595db9
│       │       └── valid-sample
│       └── traits
│           ├── BoostJsonTest.cpp
│           ├── GlazeTest.cpp
│           ├── JsonconsTest.cpp
│           ├── NlohmannTest.cpp
│           ├── OspJsoncppTest.cpp
│           ├── PicoJsonTest.cpp
│           └── ReflectCppJsonTest.cpp
├── run_guide.md
├── scripts
│   ├── build
│   │   ├── build.bat
│   │   ├── clean.bat
│   │   └── rebuild.bat
│   ├── database
│   │   ├── create_db.bat
│   │   ├── run_migrations.bat
│   │   └── seed_data.bat
│   ├── dev
│   │   ├── start_all.bat
│   │   ├── start_android_emulator.bat
│   │   └── start_backend.bat
│   ├── docker
│   │   ├── reset_services.bat
│   │   ├── start_services.bat
│   │   └── stop_services.bat
│   ├── logs
│   │   └── archeive_logs.bat
│   ├── proto
│   │   └── generate_proto.bat
│   └── testing
│       ├── load_tests.bat
│       ├── run_integration_tests.bat
│       └── run_unit_tests.bat
└── server_architecture.md

125 directories, 354 files
