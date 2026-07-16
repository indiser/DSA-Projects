#pragma once

#include "db_pool.hpp"
#include <string>

namespace chat {

class PostgresClient {
public:
    explicit PostgresClient(DBPool& pool);

    pqxx::result exec(const std::string& query);
    pqxx::result exec_params(const std::string& query, const std::vector<std::string>& params);

private:
    DBPool& pool_;
};

} // namespace chat
