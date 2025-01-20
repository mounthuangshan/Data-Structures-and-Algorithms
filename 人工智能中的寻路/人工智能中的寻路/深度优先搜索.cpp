#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <memory>
#include <list>

// Forward declarations
class IMove;
class INode;

// Helper struct to store transition information
struct DepthTransition {
    std::shared_ptr<IMove> move;
    std::shared_ptr<INode> previousNode;
    int depth;

    DepthTransition(std::shared_ptr<IMove> m, std::shared_ptr<INode> prev, int d)
        : move(m), previousNode(prev), depth(d) {
    }
};

// Solution class to store search results
class Solution {
public:
    std::shared_ptr<INode> initial;
    std::shared_ptr<INode> goal;
    bool success;
    std::vector<std::shared_ptr<IMove>> path;

    Solution(std::shared_ptr<INode> init, std::shared_ptr<INode> g, bool s = true)
        : initial(init), goal(g), success(s) {
    }
};

// Interface for moves
class IMove {
public:
    virtual void execute(std::shared_ptr<INode> node) = 0;
    virtual std::string getName() const = 0;
    virtual ~IMove() = default;
};

// Interface for nodes
class INode {
public:
    virtual bool equals(std::shared_ptr<INode> other) = 0;
    virtual std::shared_ptr<INode> copy() = 0;
    virtual std::list<std::shared_ptr<IMove>> validMoves() = 0;
    virtual void storedData(std::shared_ptr<DepthTransition> data) = 0;
    virtual std::shared_ptr<DepthTransition> storedData() const = 0;
    virtual ~INode() = default;
};

// Concrete Move: Add 1
class AddOneMove : public IMove {
public:
    void execute(std::shared_ptr<INode> node) override;
    std::string getName() const override {
        return "Add 1";
    }
};

// Concrete Move: Multiply by 2
class MultiplyTwoMove : public IMove {
public:
    void execute(std::shared_ptr<INode> node) override;
    std::string getName() const override {
        return "Multiply by 2";
    }
};

// Concrete Node
class ConcreteNode : public INode {
public:
    int value;
    std::shared_ptr<DepthTransition> data;

    ConcreteNode(int val) : value(val), data(nullptr) {}

    bool equals(std::shared_ptr<INode> other) override {
        auto o = std::dynamic_pointer_cast<ConcreteNode>(other);
        return o && (this->value == o->value);
    }

    std::shared_ptr<INode> copy() override {
        return std::make_shared<ConcreteNode>(value);
    }

    std::list<std::shared_ptr<IMove>> validMoves() override {
        return { std::make_shared<AddOneMove>(), std::make_shared<MultiplyTwoMove>() };
    }

    void storedData(std::shared_ptr<DepthTransition> d) override {
        data = d;
    }

    std::shared_ptr<DepthTransition> storedData() const override {
        return data;
    }
};

// Implement execute for AddOneMove
void AddOneMove::execute(std::shared_ptr<INode> node) {
    auto concreteNode = std::dynamic_pointer_cast<ConcreteNode>(node);
    if (concreteNode) {
        concreteNode->value += 1;
    }
}

// Implement execute for MultiplyTwoMove
void MultiplyTwoMove::execute(std::shared_ptr<INode> node) {
    auto concreteNode = std::dynamic_pointer_cast<ConcreteNode>(node);
    if (concreteNode) {
        concreteNode->value *= 2;
    }
}

// NodeSet class to manage states
class INodeSet {
    std::unordered_set<int> nodes;
    std::stack<std::shared_ptr<INode>> stack;

public:
    void insert(std::shared_ptr<INode> node) {
        auto concreteNode = std::dynamic_pointer_cast<ConcreteNode>(node);
        if (concreteNode) {
            nodes.insert(concreteNode->value);
            stack.push(node);
        }
    }

    std::shared_ptr<INode> remove() {
        if (stack.empty()) return nullptr;
        auto node = stack.top();
        stack.pop();
        return node;
    }

    bool contains(std::shared_ptr<INode> node) {
        auto concreteNode = std::dynamic_pointer_cast<ConcreteNode>(node);
        if (concreteNode) {
            return nodes.find(concreteNode->value) != nodes.end();
        }
        return false;
    }

    bool isEmpty() const {
        return stack.empty();
    }
};

// Main search function
Solution search(std::shared_ptr<INode> initial, std::shared_ptr<INode> goal, int depthBound) {
    // If initial state is goal state, return current state
    if (initial->equals(goal)) {
        return Solution(initial, goal);
    }

    INodeSet open;
    open.insert(initial->copy());
    INodeSet closed;

    while (!open.isEmpty()) {
        auto n = open.remove();
        closed.insert(n);
        auto trans = n->storedData();

        // Get valid moves and process successors
        auto moves = n->validMoves();
        for (const auto& move : moves) {
            auto successor = n->copy();
            move->execute(successor);

            // Skip if already visited
            if (closed.contains(successor)) {
                continue;
            }

            int depth = 1;
            if (trans != nullptr) {
                depth = trans->depth + 1;
            }

            // Store transition data
            successor->storedData(std::make_shared<DepthTransition>(move, n, depth));

            if (successor->equals(goal)) {
                Solution sol(initial, successor);
                // 回溯路径
                auto current = successor;
                while (current->storedData() != nullptr) {
                    sol.path.push_back(current->storedData()->move);
                    current = current->storedData()->previousNode;
                }
                return sol;
            }

            if (depth < depthBound) {
                open.insert(successor);
            }
        }
    }

    return Solution(initial, goal, false); // No solution found
}

// Example main function
int main() {
    // 创建初始节点和值
    auto initial = std::make_shared<ConcreteNode>(1);
    auto goal = std::make_shared<ConcreteNode>(10);

    // 设置搜索深度限制
    int depthBound = 10;

    // 执行搜索
    Solution sol = search(initial, goal, depthBound);

    if (sol.success) {
        std::cout << "找到解答！路径如下：" << std::endl;
        // 由于路径是逆序存储，需反转输出
        for (auto it = sol.path.rbegin(); it != sol.path.rend(); ++it) {
            std::cout << (*it)->getName() << " -> ";
        }
        std::cout << "达成目标 " << std::dynamic_pointer_cast<ConcreteNode>(sol.goal)->value << std::endl;
    }
    else {
        std::cout << "未找到解答。" << std::endl;
    }

    return 0;
}
