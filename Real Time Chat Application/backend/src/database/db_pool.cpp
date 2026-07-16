#include "database/db_pool.hpp"
#include <vector>
#include <mutex>
#include <condition_variable>
#include <stdexcept>

namespace chat {

// --- DBConnection ---

DBConnection::DBConnection(const std::string& conn_str)
    : conn_(std::make_unique<pqxx::connection>(conn_str)) {}

DBConnection::~DBConnection() = default;

pqxx::connection& DBConnection::get() { return *conn_; }

// --- DBPool::Impl ---

struct DBPool::Impl {
    std::vector<std::shared_ptr<DBConnection>> pool;
    std::mutex                                 mu;
    std::condition_variable                    cv;
};

DBPool::DBPool(const DBConfig& config) : impl_(std::make_unique<Impl>()) {
    std::string conn_str =
        "host="     + config.host     +
        " port="    + std::to_string(config.port) +
        " user="    + config.user     +
        " password="+ config.password +
        " dbname="  + config.dbname;

    for (int i = 0; i < config.pool_size; ++i)
        impl_->pool.push_back(std::make_shared<DBConnection>(conn_str));
}

DBPool::~DBPool() = default;

DBPool::Handle DBPool::acquire() {
    std::unique_lock<std::mutex> lock(impl_->mu);
    impl_->cv.wait(lock, [this] { return !impl_->pool.empty(); });
    auto conn = impl_->pool.back();
    impl_->pool.pop_back();
    return Handle(*this, conn);
}

// --- DBPool::Handle ---

DBPool::Handle::Handle(DBPool& pool, std::shared_ptr<DBConnection> conn)
    : pool_(pool), conn_(std::move(conn)) {}

DBPool::Handle::~Handle() {
    std::lock_guard<std::mutex> lock(pool_.impl_->mu);
    pool_.impl_->pool.push_back(conn_);
    pool_.impl_->cv.notify_one();
}

pqxx::connection& DBPool::Handle::get() { return conn_->get(); }

} // namespace chat
