#pragma once
#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include "redisLite.h"

class CommandParser {
private:
    RedisLite redis;
    void trim(std::string& s);
public:
    CommandParser();
    ~CommandParser();
    
    // Starts the interactive REPL (Read-Eval-Print Loop)
    void startREPL();
    
    // Parses a single line of command and executes it on the redis instance
    void parseAndExecute(const std::string& input);
};

#endif // COMMAND_PARSER_H
