// symbol_table.cpp
#include "symbol_table.h"

void SymbolTable::insert(const std::string& name, const std::string& type) {
    table[name] = type;
}

bool SymbolTable::exists(const std::string& name) const {
    return table.find(name) != table.end();
}

std::string SymbolTable::getType(const std::string& name) const {
    auto it = table.find(name);
    if (it != table.end()) {
        return it->second;
    }
    return "";
}
