#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <type_traits>

//////////////////////////////////////////////////////////////
// NodeKind, SemanticTag
//////////////////////////////////////////////////////////////

enum class NodeKind {
    Literal,
    Symbol,
    Assign,
    Binary,
    Call,
    Block
};

enum class SemanticTag {
    None,
    Dynamic,
    Static,
    Lazy,
    Eager
};

//////////////////////////////////////////////////////////////
// Node structure
//////////////////////////////////////////////////////////////

struct Node {
    NodeKind kind;
    SemanticTag tag;
    std::string value;
    std::vector<Node*> children;

    Node(NodeKind k, SemanticTag t = SemanticTag::None, std::string v = "")
        : kind(k), tag(t), value(v) {}
};

//////////////////////////////////////////////////////////////
// Factory utilities (문법없는 Node 생성 보조)
//////////////////////////////////////////////////////////////

Node* Literal(std::string v) {
    return new Node(NodeKind::Literal, SemanticTag::None, v);
}

Node* Symbol(std::string v) {
    return new Node(NodeKind::Symbol, SemanticTag::None, v);
}

Node* Assign(Node* left, Node* right) {
    Node* n = new Node(NodeKind::Assign);
    n->children.push_back(left);
    n->children.push_back(right);
    return n;
}

Node* Binary(Node* a, Node* b) {
    Node* n = new Node(NodeKind::Binary);
    n->children.push_back(a);
    n->children.push_back(b);
    return n;
}

Node* Call(std::string name, std::vector<Node*> args) {
    Node* n = new Node(NodeKind::Call, SemanticTag::None, name);
    n->children = args;
    return n;
}

Node* Block(std::vector<Node*> stmts) {
    Node* n = new Node(NodeKind::Block);
    n->children = stmts;
    return n;
}

//////////////////////////////////////////////////////////////
// Normalizer (Spongelang 의미 정규화)
//////////////////////////////////////////////////////////////

class Normalizer {
public:
    void normalize(Node* n) {
        if (!n) return;

        // 예: 호출(Call)에 동적 의미 부여
        if (n->kind == NodeKind::Call && n->tag == SemanticTag::None) {
            n->tag = SemanticTag::Dynamic;
        }

        for (auto* c : n->children)
            normalize(c);
    }
};

//////////////////////////////////////////////////////////////
// Rust Emitter (Spongelang IR → Rust)
//////////////////////////////////////////////////////////////

class RustEmitter {
public:
    std::string emit(Node* n) {
        if (!n) return "";

        switch (n->kind) {
            case NodeKind::Literal:
                return n->value;

            case NodeKind::Symbol:
                return n->value;

            case NodeKind::Assign:
                return n->children[0]->value + " = " + emit(n->children[1]) + ";";

            case NodeKind::Binary:
                return emit(n->children[0]) + " + " + emit(n->children[1]);

            case NodeKind::Call:
                return emitCall(n);

            case NodeKind::Block:
                return emitBlock(n);

            default:
                return "";
        }
    }

private:
    std::string emitCall(Node* n) {
        std::string out = n->value + "(";
        for (size_t i = 0; i < n->children.size(); i++) {
            out += emit(n->children[i]);
            if (i + 1 < n->children.size()) out += ", ";
        }
        out += ");";
        return out;
    }

    std::string emitBlock(Node* n) {
        std::string out = "{\n";
        for (auto* c : n->children) {
            out += "  " + emit(c) + "\n";
        }
        out += "}\n";
        return out;
    }
};

//////////////////////////////////////////////////////////////
// MAIN — 실제 Spongelang 엔진 테스트
//////////////////////////////////////////////////////////////

int main() {

    // Ruby 코드 예시:
    // x = 5
    // puts x + 2

    Node* ast = Block({
        Assign(Symbol("x"), Literal("5")),
        Call("println", { Binary(Symbol("x"), Literal("2")) })
    });

    Normalizer norm;
    norm.normalize(ast);

    RustEmitter emitter;
    std::string rustCode = emitter.emit(ast);

    std::cout << "=== Rust Output ===\n";
    std::cout << rustCode << "\n";

    return 0;
}
