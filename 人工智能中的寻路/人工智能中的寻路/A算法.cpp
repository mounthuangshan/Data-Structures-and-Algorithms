#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <memory>
#include <list>
#include <iterator>
#include <cmath>


// Forward declarations
class IMove;
class INode;
class Solution;
class DepthTransition;

// Interface for scoring function
class IScoringFunction {
public:
    virtual void score(std::shared_ptr<INode> node) = 0;
    virtual ~IScoringFunction() = default;
};

// Interface for Node
class INode {
public:
    virtual std::shared_ptr<INode> copy() = 0;
    virtual bool equals(std::shared_ptr<INode> other) = 0;
    virtual double score() const = 0;
    virtual void setScore(double score) = 0;
    virtual std::list<std::shared_ptr<IMove>> validMoves() = 0;
    virtual void storedData(std::shared_ptr<DepthTransition> data) = 0;
    virtual std::shared_ptr<DepthTransition> storedData() const = 0;
    virtual ~INode() = default;
};

// Move interface
class IMove {
public:
    virtual void execute(std::shared_ptr<INode> node) = 0;
    virtual ~IMove() = default;
};

// 前置声明
class PuzzleNode;
class PuzzleMove;

class DepthTransition {
public:
    std::shared_ptr<IMove> move;
    std::shared_ptr<INode> previousNode;
    int depth;
    DepthTransition(std::shared_ptr<IMove> m, std::shared_ptr<INode> prev, int d)
        : move(m), previousNode(prev), depth(d) {
    }
};

// 先声明 PuzzleNode，稍后在类外实现 validMoves
class PuzzleNode : public INode {
private:
    std::vector<int> board;
    double scoreValue;
    std::shared_ptr<DepthTransition> transitionData;
public:
    PuzzleNode(std::vector<int> initial) : board(initial), scoreValue(0.0) {}

    std::shared_ptr<INode> copy() override {
        auto newNode = std::make_shared<PuzzleNode>(board);
        newNode->scoreValue = scoreValue;
        newNode->transitionData = transitionData;
        return newNode;
    }

    bool equals(std::shared_ptr<INode> other) override {
        auto o = std::dynamic_pointer_cast<PuzzleNode>(other);
        return o && (board == o->board);
    }

    double score() const override {
        return scoreValue;
    }

    void setScore(double score) override {
        scoreValue = score;
    }

    void storedData(std::shared_ptr<DepthTransition> data) override {
        transitionData = data;
    }

    std::shared_ptr<DepthTransition> storedData() const override {
        return transitionData;
    }

    // 先声明 validMoves
    std::list<std::shared_ptr<IMove>> validMoves() override;

    const std::vector<int>& getBoard() const {
        return board;
    }

    int getEmptyPos() const {
        for (int i = 0; i < 9; i++) {
            if (board[i] == 0) return i;
        }
        return -1;
    }
};

// 定义 PuzzleMove
class PuzzleMove : public IMove {
private:
    int fromPos;
    int toPos;
public:
    PuzzleMove(int f, int t) : fromPos(f), toPos(t) {}

    void execute(std::shared_ptr<INode> node) override {
        auto puzzleNode = std::dynamic_pointer_cast<PuzzleNode>(node);
        if (!puzzleNode) return;
        auto& b = const_cast<std::vector<int>&>(puzzleNode->getBoard());
        std::swap(b[fromPos], b[toPos]);
    }
};

// 在类外实现 PuzzleNode::validMoves，使用 PuzzleMove
std::list<std::shared_ptr<IMove>> PuzzleNode::validMoves() {
    std::list<std::shared_ptr<IMove>> moves;
    int emptyPos = getEmptyPos();
    int row = emptyPos / 3;
    int col = emptyPos % 3;

    // 上
    if (row > 0) moves.push_back(std::make_shared<PuzzleMove>(emptyPos, emptyPos - 3));
    // 下
    if (row < 2) moves.push_back(std::make_shared<PuzzleMove>(emptyPos, emptyPos + 3));
    // 左
    if (col > 0) moves.push_back(std::make_shared<PuzzleMove>(emptyPos, emptyPos - 1));
    // 右
    if (col < 2) moves.push_back(std::make_shared<PuzzleMove>(emptyPos, emptyPos + 1));

    return moves;
}




// Manhattan distance heuristic scoring function
class ManhattanScorer : public IScoringFunction {
public:
    void score(std::shared_ptr<INode> node) override {
        auto puzzleNode = std::dynamic_pointer_cast<PuzzleNode>(node);
        if (!puzzleNode) return;

        const auto& board = puzzleNode->getBoard();
        int manhattan = 0;

        // Calculate Manhattan distance for each tile
        for (int i = 0; i < 9; i++) {
            if (board[i] == 0) continue;
            int targetRow = (board[i] - 1) / 3;
            int targetCol = (board[i] - 1) % 3;
            int currentRow = i / 3;
            int currentCol = i % 3;
            manhattan += std::abs(targetRow - currentRow) + std::abs(targetCol - currentCol);
        }

        // Add depth to break ties
        auto trans = puzzleNode->storedData();
        int depth = trans ? trans->depth : 0;

        puzzleNode->setScore(manhattan + depth * 0.001);
    }
};




// Solution class
class Solution {
public:
    Solution(std::shared_ptr<INode> initial, std::shared_ptr<INode> goal, bool success = true)
        : initialNode(initial), goalNode(goal), isSuccess(success) {
    }

    std::shared_ptr<INode> initialNode;
    std::shared_ptr<INode> goalNode;
    bool isSuccess;
};

// Main search function
class SearchSolution {
private:
    std::shared_ptr<IScoringFunction> scoringFunction;

public:
    SearchSolution(std::shared_ptr<IScoringFunction> scorer) : scoringFunction(scorer) {}

    Solution search(std::shared_ptr<INode> initial, std::shared_ptr<INode> goal) {
        // Priority queue for open set
        auto compareNodes = [](std::shared_ptr<INode> a, std::shared_ptr<INode> b) {
            return a->score() > b->score();
            };
        std::priority_queue<std::shared_ptr<INode>,
            std::vector<std::shared_ptr<INode>>,
            decltype(compareNodes)> open(compareNodes);

        // Initialize with initial state
        auto copy = initial->copy();
        scoringFunction->score(copy);
        open.push(copy);

        // Hash set for closed set
        std::unordered_set<std::shared_ptr<INode>> closed;

        while (!open.empty()) {
            // Remove node with lowest score
            auto n = open.top();
            open.pop();
            closed.insert(n);

            // Check if goal state reached
            if (n->equals(goal)) {
                return Solution(initial, n);
            }

            // Calculate successor moves
            auto trans = n->storedData();
            int depth = 1;
            if (trans) {
                depth = trans->depth + 1;
            }

            auto moves = n->validMoves();
            for (auto& move : moves) {
                // Make move and evaluate new state
                auto successor = n->copy();
                move->execute(successor);

                // Store transition data
                successor->storedData(std::make_shared<DepthTransition>(move, n, depth));
                scoringFunction->score(successor);

                // Check if state was previously visited
                auto it = closed.find(successor);
                if (it != closed.end()) {
                    if (successor->score() >= (*it)->score()) {
                        continue;
                    }
                    closed.erase(it);
                }

                // Add to open set
                open.push(successor);
            }
        }

        // No solution found
        return Solution(initial, goal, false);
    }
};

int main() {
    // Create initial state (example: 8-puzzle)
    std::vector<int> initialBoard = { 1, 2, 3, 4, 0, 6, 7, 5, 8 };  // 0 represents empty space
    std::vector<int> goalBoard = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };

    auto initial = std::make_shared<PuzzleNode>(initialBoard);
    auto goal = std::make_shared<PuzzleNode>(goalBoard);

    // Create scorer and search solver
    auto scorer = std::make_shared<ManhattanScorer>();
    auto searchSolver = SearchSolution(scorer);

    // Find solution
    Solution solution = searchSolver.search(initial, goal);

    if (solution.isSuccess) {
        std::cout << "Solution found!\n";

        // Reconstruct path
        std::vector<std::shared_ptr<INode>> path;
        auto current = solution.goalNode;
        while (current) {
            path.push_back(current);
            auto trans = current->storedData();
            if (!trans) break;
            current = trans->previousNode;
        }

        // Print solution path
        std::cout << "Number of moves: " << path.size() - 1 << "\n";
        std::cout << "Path from goal to start:\n";
        for (const auto& node : path) {
            auto puzzleNode = std::dynamic_pointer_cast<PuzzleNode>(node);
            if (!puzzleNode) continue;

            const auto& board = puzzleNode->getBoard();
            for (int i = 0; i < 9; i++) {
                std::cout << board[i] << " ";
                if (i % 3 == 2) std::cout << "\n";
            }
            std::cout << "---\n";
        }
    }
    else {
        std::cout << "No solution exists.\n";
    }

    return 0;
}