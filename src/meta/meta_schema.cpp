#include "meta_schema.hpp"
#include <fstream>
#include <sstream>

namespace sponge {

void MetaSchema::load(const std::string& path) {
    std::ifstream f(path);
    std::string line;

    while (std::getline(f, line)) {
        if (line.find("key:") != std::string::npos) {
            SchemaEntry e;
            e.key = line.substr(line.find(":")+1);
            std::getline(f, line); // type
            e.type = line.substr(line.find(":")+1);
            std::getline(f, line); // value
            e.valueType = line.substr(line.find(":")+1);
            entries.push_back(e);
        }
    }
}

} // namespace sponge
