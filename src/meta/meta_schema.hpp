#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

namespace sponge {

struct SchemaEntry {
    std::string key;
    std::string type;      // string, map, int, expression
    std::string valueType; // for map: string, int, expression
};

class MetaSchema {
public:
    std::vector<SchemaEntry> entries;

    void load(const std::string& path);
};

} // namespace sponge
