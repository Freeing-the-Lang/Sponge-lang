#include "sponge_meta.hpp"
#include <iostream>

int main() {
    using namespace sponge;

    SpongeMetaEngine eng;

    // 기본 연산 흡수
    eng.absorb("base",
        {
            {"num", "[0-9]+"}
        },
        {
            {"+", 10}, {"-", 10}
        },
        {
            {"+", [](double a, double b){ return a + b; }},
            {"-", [](double a, double b){ return a - b; }}
        }
    );

    // evaluator rule for literal number
    eng.evaluator.addRule("Number", [](const MetaNode* n){
        return std::stod(n->value);
    });

    std::cout << "[Run] 3 → "  << eng.run("3") << "\n";
    std::cout << "[Go]  3 → "  << eng.toGo("3") << "\n";

    return 0;
}
