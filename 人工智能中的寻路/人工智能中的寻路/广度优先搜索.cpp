#include <iostream>
#include <queue>
#include <unordered_set>
#include <list>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>

// Forward declarations
class IMove;
class INode;
class Solution;
class Transition;

// Interface for moves
class IMove {
public:
    virtual ~IMove() = default;
    virtual void execute(std::shared_ptr<INode> node) = 0;
    virtual std::string getName() const = 0;
    virtual std::unique_ptr<IMove> clone() const = 0;
};

// Interface for nodes
class INode {
public:
    virtual ~INode() = default;
    virtual bool equals(const std::shared_ptr<INode> other) const = 0;
    virtual std::shared_ptr<INode> copy() const = 0;
    virtual std::list<std::unique_ptr<IMove>> validMoves() const = 0;
    virtual void storedData(std::shared_ptr<Transition> transition) = 0;
    virtual std::shared_ptr<Transition> storedData() const = 0;
};

// Transition class to store move history
class Transition {
public:
    Transition(std::shared_ptr<IMove> m, std::shared_ptr<INode> prevNode)
        : move(m), previousNode(prevNode) {
    }

    std::shared_ptr<IMove> move;
    std::shared_ptr<INode> previousNode;
};

// Solution class to store the result
class Solution {
public:
    Solution(std::shared_ptr<INode> init, std::shared_ptr<INode> g, bool s = true)
        : initial(init), goal(g), success(s) {
    }

    std::shared_ptr<INode> initial;
    std::shared_ptr<INode> goal;
    bool success;
    std::vector<std::shared_ptr<IMove>> path;
};

// Concrete Move: Add 1
class AddOneMove : public IMove {
public:
    void execute(std::shared_ptr<INode> node) override;
    std::string getName() const override {
        return "Add 1";
    }
    std::unique_ptr<IMove> clone() const override {
        return std::make_unique<AddOneMove>();
    }
};

// Concrete Move: Multiply by 2
class MultiplyTwoMove : public IMove {
public:
    void execute(std::shared_ptr<INode> node) override;
    std::string getName() const override {
        return "Multiply by 2";
    }
    std::unique_ptr<IMove> clone() const override {
        return std::make_unique<MultiplyTwoMove>();
    }
};

// Concrete Node
class ConcreteNode : public INode, public std::enable_shared_from_this<ConcreteNode> {
public:
    int value;
    std::shared_ptr<Transition> data;

    ConcreteNode(int val) : value(val), data(nullptr) {}

    bool equals(const std::shared_ptr<INode> other) const override {
        auto o = std::dynamic_pointer_cast<ConcreteNode>(other);
        return o && (this->value == o->value);
    }

    std::shared_ptr<INode> copy() const override {
        return std::make_shared<ConcreteNode>(value);
    }

    std::list<std::unique_ptr<IMove>> validMoves() const override {
        std::list<std::unique_ptr<IMove>> moves;
        moves.emplace_back(std::make_unique<AddOneMove>());
        moves.emplace_back(std::make_unique<MultiplyTwoMove>());
        return moves;
    }

    void storedData(std::shared_ptr<Transition> transition) override {
        data = transition;
    }

    std::shared_ptr<Transition> storedData() const override {
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

// Hash function for ConcreteNode to be used in unordered_set
struct NodeHash {
    std::size_t operator()(const std::shared_ptr<INode>& node) const {
        auto concreteNode = std::dynamic_pointer_cast<ConcreteNode>(node);
        return concreteNode ? std::hash<int>()(concreteNode->value) : 0;
    }
};

// Equality function for ConcreteNode to be used in unordered_set
struct NodeEqual {
    bool operator()(const std::shared_ptr<INode>& lhs, const std::shared_ptr<INode>& rhs) const {
        return lhs->equals(rhs);
    }
};

// Main search function (Breadth-First Search)
Solution search(std::shared_ptr<INode> initial, std::shared_ptr<INode> goal) {
    // If initial state is the solution, return
    if (initial->equals(goal)) {
        return Solution(initial, goal, true);
    }

    // Start from initial state
    std::queue<std::shared_ptr<INode>> open;
    open.push(initial->copy());

    // States we've already visited
    std::unordered_set<std::shared_ptr<INode>, NodeHash, NodeEqual> closed;

    while (!open.empty()) {
        std::shared_ptr<INode> current = open.front();
        open.pop();

        // Insert into closed set
        closed.insert(current);

        // Get all valid moves and process successors
        std::list<std::unique_ptr<IMove>> moves = current->validMoves();
        for (auto& move : moves) {
            // Create a copy
            std::shared_ptr<INode> successor = current->copy();

            // Execute the move
            move->execute(successor);

            // Check if we've already visited this state
            if (closed.find(successor) != closed.end()) {
                continue;
            }

            // Store the transition information
            std::shared_ptr<Transition> transition = std::make_shared<Transition>(move->clone(), current);
            successor->storedData(transition);

            // Check if we've found the goal
            if (successor->equals(goal)) {
                Solution sol(initial, successor, true);

                // 回溯路径
                std::shared_ptr<INode> step = successor;
                while (step->storedData() != nullptr) {
                    sol.path.push_back(step->storedData()->move);
                    step = step->storedData()->previousNode;
                }

                // 反转路径以从初始状态到目标状态
                std::reverse(sol.path.begin(), sol.path.end());

                return sol;
            }

            // Add to open set
            open.push(successor);
        }
    }

    // No solution found
    return Solution(initial, goal, false);
}

// Main function
int main() {
    // 创建初始和目标节点
    auto initial = std::make_shared<ConcreteNode>(1);
    auto goal = std::make_shared<ConcreteNode>(10);

    // 执行搜索
    Solution solution = search(initial, goal);

    // 检查是否找到解答
    if (solution.success) {
        std::cout << "找到解答！路径如下：" << std::endl;
        for (const auto& move : solution.path) {
            std::cout << move->getName() << " -> ";
        }
        auto concreteGoal = std::dynamic_pointer_cast<ConcreteNode>(solution.goal);
        if (concreteGoal) {
            std::cout << "达成目标 " << concreteGoal->value << std::endl;
        }
    }
    else {
        std::cout << "未找到解答。" << std::endl;
    }

    return 0;
}
