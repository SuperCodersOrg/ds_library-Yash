#pragma once
#ifndef REDISLITE_H
#define REDISLITE_H

#include <string>
#include "SuperCodersCollections.h"

class RedisLite {
private:
    HashMap<std::string, std::string> store;

public:
    RedisLite();
    ~RedisLite();

    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key) const;
    void del(const std::string& key);
    bool exist(const std::string& key) const;
    void clear();
    int size() const;
    bool empty() const;
};

#endif 
