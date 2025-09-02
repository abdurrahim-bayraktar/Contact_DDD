//
// Created by user on 9/2/25.
//

#ifndef UNTITLED_CONNECTIONPOOL_H
#define UNTITLED_CONNECTIONPOOL_H

#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>
#include <pqxx/pqxx>
#include <string>
#include <utility>

using namespace std;

class connectionPool
{
    queue <shared_ptr<pqxx::connection>> pool_;
    mutex mutex_;
    condition_variable cond_;
    string connectionInfo_;
    size_t maxConnections_;

public:
    connectionPool(string  connInfo, size_t maxConnections);
    shared_ptr<pqxx::connection> acquire();
    void release(shared_ptr<pqxx::connection> connection);
};

inline connectionPool::connectionPool(string  connInfo, size_t maxConnections) : connectionInfo_(std::move(connInfo)),
                                                                                 maxConnections_(maxConnections)
{
    for (size_t i = 0; i < maxConnections; ++i)
    {
        shared_ptr<pqxx::connection> connection = make_shared<pqxx::connection>(connectionInfo_);
        pool_.push(connection);
    }
}


inline shared_ptr<pqxx::connection> connectionPool::acquire()
        {
            unique_lock<mutex> lock(mutex_);
            cond_.wait(lock, [this]() { return !pool_.empty(); });

            auto connection = move(pool_.front());
            pool_.pop();
            return connection;
        }

inline void connectionPool::release(shared_ptr<pqxx::connection> connection)
{
    lock_guard<mutex> lock(mutex_);
    pool_.push(move(connection));
    cond_.notify_one();
};

#endif //UNTITLED_CONNECTIONPOOL_H