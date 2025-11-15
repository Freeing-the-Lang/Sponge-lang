#include "meta_absorb_loader.hpp"
#include "meta_engine.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace sponge {

// ----------------------------------------------------------
// 유틸: trim
// ----------------------------------------------------------
static inline std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

// ----------------------------------------------------------
// 1) .meta 파일 → LangPack 로드
// ----------------------------------------------------------
MetaAbsorbLoader::LangPack MetaAbsorbLoader::loadFromFile(
        const std::string& path)
{
    std::ifstream f(path);
    if (!f.is_open()) {
        throw std::runtime_error(
            "[MetaAbsorbLoader] Cannot open meta file: " + path
        );
    }

    LangPack pack;
    std::string line;

    enum class Section {
        NONE,
        TOKENS,
        OPERATORS,
        EVALUATE,
        IR,
        BYTECODE
    };

    Section cur = Section::NONE;

    while (std::getline(f, line)) {
        line = trim(line);
        if (line.empty()) continue;

        // language: rust
        if (line.rfind("language:", 0) == 0) {
            pack.name = trim(line.substr(line.find(':') + 1));
            continue;
        }

        // Section detection
        if (line == "tokens:"   ) { cur = Section::TOKENS;   continue; }
        if (line == "operators:") { cur = Section::OPERATORS; continue; }
        if (line == "evaluate:")  { cur = Section::EVALUATE;  continue; }
        if (line == "ir:")        { cur = Section::IR;        continue; }
        if (line == "bytecode:")  { cur = Section::BYTECODE;  continue; }

        // key: value 파싱
        if (line.find(':') != std::string::npos) {
            auto key = trim(line.substr(0, line.find(':')));
            auto val = trim(line.substr(line.find(':') + 1));

            // remove quotes if exist
            if (!val.empty() && val.front() == '"' && val.back() == '"')
                val = val.substr(1, val.size() - 2);

            // switch by section
            switch (cur) {
            case Section::TOKENS:
                pack.tokens[key] = val;
                break;

            case Section::OPERATORS:
                pack.precedence[key] = std::stoi(val);
                break;

            case Section::EVALUATE:
                pack.evalRules[key] = val;
                break;

            case Section::IR:
                pack.irRules[key] = val;
                break;

            case Section::BYTECODE:
                pack.bytecode[key] = val;
                break;

            default:
                // ignore
                break;
            }
        }
    }

    return pack;
}


// ----------------------------------------------------------
// 2) LangPack → SpongeMetaEngine mount
// ----------------------------------------------------------
void MetaAbsorbLoader::mount(
        SpongeMetaEngine& eng,
        const LangPack& pack)
{
    // -------------------------
    // 2-1) absorb() 호출 준비
    // -------------------------
    std::unordered_map<std::string,std::string> tokenMap = pack.tokens;

    std::unordered_map<std::string,int> precMap = pack.precedence;

    // evalRules[op] = "a + b" 같은 식 → (a,b)-> 결과
    std::unordered_map<std::string,
        std::function<double(double,double)>> evalOps;

    for (auto& kv : pack.evalRules) {
        const std::string& op = kv.first;
        const std::string& expr = kv.second;  // "a + b"

        // 아주 단순한 식 해석기: a+b, a-b, a*b, a/b
        if (expr == "a + b")
            evalOps[op] = [](double a,double b){ return a + b; };
        else if (expr == "a - b")
            evalOps[op] = [](double a,double b){ return a - b; };
        else if (expr == "a * b")
            evalOps[op] = [](double a,double b){ return a * b; };
        else if (expr == "a / b")
            evalOps[op] = [](double a,double b){ return a / b; };
        else {
            // 확장 가능: expression 파서를 붙여
            // 실제 문자열에 따른 계산식 생성 가능
            evalOps[op] = [](double a,double b){ return 0; };
        }
    }

    // -------------------------
    // 2-2) SpongeMetaEngine에 반영
    // -------------------------
    eng.absorb(pack.name, tokenMap, precMap, evalOps);

    // -------------------------
    // 2-3) IR 규칙 반영
    // -------------------------
    // (필요 시 실제 IR 빌더/트랜스파일러 확장 가능)
    for (auto& kv : pack.irRules) {
        // 예: literal → "literal"
        //     binary  → "binary"
        // 이 규칙을 트랜스파일러에게도 자동 등록 가능.
        // (지금은 최소 버전만 넣어둠)
    }

    // -------------------------
    // 2-4) Bytecode 규칙 반영
    // -------------------------
    // pack.bytecode["+"] = "ADD" 같은 것
    // VM opcode 출력용으로 사용 가능
    // (현재 VM은 단일 opcode enum만 제공하므로 확장 가능)
}

} // namespace sponge
