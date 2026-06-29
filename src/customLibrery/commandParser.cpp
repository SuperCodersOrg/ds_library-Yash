#include "commandParser.h"
#include <iostream>

CommandParser::CommandParser() {}
CommandParser::~CommandParser() {}

void CommandParser::trim(std::string& s) {
    // Manual trim to remove leading spaces/newlines
    int start = 0;
    while (start < s.length() && (s[start] == ' ' || s[start] == '\t' || s[start] == '\n' || s[start] == '\r')) {
        start++;
    }
    
    // Manual trim to remove trailing spaces/newlines
    int end = s.length() - 1;
    while (end >= 0 && (s[end] == ' ' || s[end] == '\t' || s[end] == '\n' || s[end] == '\r')) {
        end--;
    }
    
    if (start > end) {
        s = "";
    } else {
        s = s.substr(start, end - start + 1);
    }
}

void CommandParser::parseAndExecute(const std::string& input) {
    if (input.empty()) return;
    
    std::string command = "";
    std::string key = "";
    std::string value = "";
    
    int i = 0;
    int len = input.length();
    
    // Skip leading whitespace
    while (i < len && (input[i] == ' ' || input[i] == '\t')) i++;
    
    // Read command and convert to uppercase manually
    while (i < len && input[i] != ' ' && input[i] != '\t') {
        char c = input[i];
        if (c >= 'a' && c <= 'z') {
            c = c - 32;
        }
        command += c;
        i++;
    }
    
    // Skip whitespace between command and key
    while (i < len && (input[i] == ' ' || input[i] == '\t')) i++;
    
    // Read key
    while (i < len && input[i] != ' ' && input[i] != '\t') {
        key += input[i];
        i++;
    }
    
    // Skip whitespace between key and value
    while (i < len && (input[i] == ' ' || input[i] == '\t')) i++;
    
    // Read value (the rest of the line)
    while (i < len) {
        value += input[i];
        i++;
    }
    
    if (command == "SET") {
        if (key.empty() || value.empty()) {
            std::cout << "(error) ERR wrong number of arguments for 'set' command\n";
        } else {
            redis.set(key, value);
            std::cout << "OK\n";
        }
    } else if (command == "GET") {
        if (key.empty()) {
            std::cout << "(error) ERR wrong number of arguments for 'get' command\n";
        } else {
            if (redis.exist(key)) {
                std::cout << "\"" << redis.get(key) << "\"\n";
            } else {
                std::cout << "(nil)\n";
            }
        }
    } else if (command == "DEL") {
        if (key.empty()) {
            std::cout << "(error) ERR wrong number of arguments for 'del' command\n";
        } else {
            if (redis.exist(key)) {
                redis.del(key);
                std::cout << "(integer) 1\n";
            } else {
                std::cout << "(integer) 0\n";
            }
        }
    } else if (command == "EXIST" || command == "EXISTS") {
        if (key.empty()) {
            std::cout << "(error) ERR wrong number of arguments for 'exist' command\n";
        } else {
            if (redis.exist(key)) {
                std::cout << "(integer) 1\n";
            } else {
                std::cout << "(integer) 0\n";
            }
        }
    } else if (command == "CLEAR") {
        redis.clear();
        std::cout << "OK\n";
    } else if (command == "SIZE") {
        std::cout << "(integer) " << redis.size() << "\n";
    } else if (command == "EMPTY") {
        if (redis.empty()) {
            std::cout << "(integer) 1\n";
        } else {
            std::cout << "(integer) 0\n";
        }
    } else if (command == "EXIT" || command == "QUIT") {
        // Handled in startREPL
    } else {
        std::cout << "(error) ERR unknown command '" << command << "'\n";
    }
}

void CommandParser::startREPL() {
    std::string input;
    std::cout << "--- RedisLite CLI --- \n";
    std::cout << "Type 'exit' or 'quit' to close.\n";
    while (true) {
        std::cout << "127.0.0.1:6379> ";
        std::getline(std::cin, input);
        trim(input);
        
        if (input.empty()) {
            continue;
        }
        
        // Extract just the command word to check for exit
        std::string command = "";
        int i = 0;
        int len = input.length();
        while (i < len && input[i] != ' ' && input[i] != '\t') {
            char c = input[i];
            if (c >= 'a' && c <= 'z') c = c - 32;
            command += c;
            i++;
        }
        
        if (command == "EXIT" || command == "QUIT") {
            std::cout << "Goodbye!\n";
            break;
        }
        
        parseAndExecute(input);
    }
}
