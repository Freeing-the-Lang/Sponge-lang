#include <iostream>
#include <string>
#include <vector>

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
// Factory utilities
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

        // 의미 규칙 자동 부여: Call → dynamic
        if (n->kind == NodeKind::Call && n->tag == SemanticTag::None) {
            n->tag = SemanticTag::Dynamic;
        }

        // lazy propagation
        if (n->tag == SemanticTag::Lazy) {
            for (auto* c : n->children)
                c->tag = SemanticTag::Lazy;
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
// EXAMPLES
//////////////////////////////////////////////////////////////

Node* example_ruby() {
    // Ruby:
    // x = 5
    // puts x + 2
    return Block({
        Assign(Symbol("x"), Literal("5")),
        Call("println", { Binary(Symbol("x"), Literal("2")) })
    });
}

Node* example_java() {
    // Java:
    // int a = 10;
    // System.out.println(a * 3);
    return Block({
        Assign(Symbol("a"), Literal("10")),
        Call("println", { Binary(Symbol("a"), Literal("3")) })
    });
}

Node* example_haskell() {
    // Haskell:
    // x = 1 + 2  (lazy)
    Node* expr = Binary(Literal("1"), Literal("2"));
    expr->tag = SemanticTag::Lazy;

    return Block({
        Assign(Symbol("x"), expr),
        Call("println", { Symbol("x") })
    });
}

//////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////

int main() {

    std::cout << "=== Spongelang Example Runner ===\n";

    std::vector<Node*> examples = {
        example_ruby(),
        example_java(),
        example_haskell()
    };

    Normalizer norm;
    RustEmitter emitter;

    int idx = 1;
    for (auto* e : examples) {
        std::cout << "\n--- Example " << idx++ << " ---\n";
        norm.normalize(e);
        std::cout << emitter.emit(e) << "\n";
    }

    return 0;
}
