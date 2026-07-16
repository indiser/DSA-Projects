#include "database/postgres_client.hpp"

namespace chat {

PostgresClient::PostgresClient(DBPool& pool) : pool_(pool) {}

pqxx::result PostgresClient::exec(const std::string& query) {
    auto handle = pool_.acquire();
    pqxx::work txn(handle.get());
    auto result = txn.exec(query);
    txn.commit();
    return result;
}

pqxx::result PostgresClient::exec_params(const std::string& query,
                                          const std::vector<std::string>& params) {
    auto handle = pool_.acquire();
    pqxx::work txn(handle.get());
    pqxx::params p;
    for (const auto& param : params) p.append(param);
    auto result = txn.exec_params(query, p);
    txn.commit();
    return result;
}

} // namespace chat
