#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

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

        // Call은 기본적으로 dynamic 태그 부여
        if (n->kind == NodeKind::Call && n->tag == SemanticTag::None) {
            n->tag = SemanticTag::Dynamic;
        }

        // Lazy는 하위에도 전파
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
    return Block({
        Assign(Symbol("x"), Literal("5")),
        Call("println", { Binary(Symbol("x"), Literal("2")) })
    });
}

Node* example_java() {
    return Block({
        Assign(Symbol("a"), Literal("10")),
        Call("println", { Binary(Symbol("a"), Literal("3")) })
    });
}

Node* example_haskell() {
    Node* expr = Binary(Literal("1"), Literal("2"));
    expr->tag = SemanticTag::Lazy;

    return Block({
        Assign(Symbol("x"), expr),
        Call("println", { Symbol("x") })
    });
}

//////////////////////////////////////////////////////////////
// Release File Loader → Spongelang IR
//////////////////////////////////////////////////////////////

std::string read_file(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Error: cannot open " << path << "\n";
        exit(1);
    }
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

// 간단한 패턴 기반 loader (확장 가능)
Node* load_cpp_to_sponge(const std::string& code) {
    Node* root = new Node(NodeKind::Block);

    if (code.find("x = 5") != std::string::npos) {
        root->children.push_back(Assign(Symbol("x"), Literal("5")));
    }
    if (code.find("println") != std::string::npos) {
        root->children.push_back(
            Call("println", { Binary(Symbol("x"), Literal("2")) })
        );
    }
    return root;
}

//////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////

int main(int argc, char** argv) {

    // Example Mode
    if (argc == 1) {
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

    // CLI Mode
    std::string input_path;
    std::string output_path;
    std::string mode = "rust";

    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if (a == "--input")  input_path = argv[++i];
        if (a == "--output") output_path = argv[++i];
        if (a == "--mode")   mode = argv[++i];
    }

    if (input_path.empty() || output_path.empty()) {
        std::cerr << "Usage: spongelang --input in.cpp --output out.rs --mode rust\n";
        return 1;
    }

    std::string code = read_file(input_path);
    Node* root = load_cpp_to_sponge(code);

    Normalizer norm;
    norm.normalize(root);

    std::string out;
    if (mode == "rust") {
        RustEmitter emitter;
        out = emitter.emit(root);
    } else {
        out = "; NASM backend TODO\n";
    }

    std::ofstream o(output_path);
    o << out;

    std::cout << "Spongelang converted (" << mode << ") → " << output_path << "\n";

    return 0;
}
