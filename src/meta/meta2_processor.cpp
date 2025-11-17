#include "meta_engine.hpp"

#include "meta2_processor.hpp"
#include <fstream>
#include <stdexcept>

namespace sponge {

MetaAbsorbLoader::LangPack
Meta2Processor::parseMetaWithSchema(
        const std::string& schemaPath,
        const std::string& metaPath)
{
    schema.load(schemaPath);

    std::ifstream f(metaPath);
    if (!f.is_open())
        throw std::runtime_error("cannot load meta: " + metaPath);

    MetaAbsorbLoader::LangPack pack;

    std::string line;
    while (std::getline(f, line)) {
        for (auto& rule : schema.entries) {
            if (line.starts_with(rule.key + ":")) {

                std::string value = line.substr(line.find(":")+1);

                // language
                if (rule.key == "language")
                    pack.name = value;

                // tokens
                if (rule.key == "tokens" && rule.type == "map")
                    pack.tokens["number"] = "[0-9]+";

                // operators
                if (rule.key == "operators" && rule.type == "map") {
                    // 가벼운 예시:  "+: 10" 같은 라인 읽기
                    if (value.find("+") != std::string::npos)
                        pack.precedence["+"] = 10;
                    if (value.find("-") != std::string::npos)
                        pack.precedence["-"] = 10;
                }

                // evaluate
                if (rule.key == "evaluate")
                    pack.evalRules["+"] = "a + b";

                // bytecode
                if (rule.key == "bytecode")
                    pack.bytecode["+"] = "ADD";
            }
        }
    }
    return pack;
}

} // namespace sponge
