// symbol_table.h
#pragma once
#include <string>
#include <unordered_map>

class SymbolTable {
private:
    std::unordered_map<std::string, std::string> table; // varName -> type

public:
    void insert(const std::string& name, const std::string& type);
    bool exists(const std::string& name) const;
    std::string getType(const std::string& name) const;
};

