#pragma once

#include <string>
#include <memory>
#include <pqxx/pqxx>

namespace chat {

struct DBConfig {
    std::string host;
    int         port     = 5432;
    std::string user;
    std::string password;
    std::string dbname;
    int         pool_size = 10;
};

class DBConnection {
public:
    explicit DBConnection(const std::string& conn_str);
    ~DBConnection();

    pqxx::connection& get();

private:
    std::unique_ptr<pqxx::connection> conn_;
};

class DBPool {
public:
    explicit DBPool(const DBConfig& config);
    ~DBPool();

    // RAII handle — returns connection to pool on destruction
    class Handle {
    public:
        Handle(DBPool& pool, std::shared_ptr<DBConnection> conn);
        ~Handle();
        pqxx::connection& get();
    private:
        DBPool&                        pool_;
        std::shared_ptr<DBConnection>  conn_;
    };

    Handle acquire();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace chat
