#include "redisLite.h"

RedisLite::RedisLite() {}
RedisLite::~RedisLite() {}

void RedisLite::set(const std::string& key, const std::string& value) {
    store.set(key, value);
}

std::string RedisLite::get(const std::string& key) const {
    if (store.exist(key)) {
        return store.get(key);
    }
    return "key doesn't exist";
}

void RedisLite::del(const std::string& key) {
    if (store.exist(key)) {
        store.remove(key);
    }
}

bool RedisLite::exist(const std::string& key) const {
    return store.exist(key);
}

void RedisLite::clear() {
    store.clear();
}

int RedisLite::size() const {
    return store.getSize();
}

bool RedisLite::empty() const {
    return store.empty();
}
