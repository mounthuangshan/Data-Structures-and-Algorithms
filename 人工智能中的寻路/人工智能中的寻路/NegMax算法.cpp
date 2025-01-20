#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>

// Forward declarations
class IGameState;
class IGameMove;
class IPlayer;
class IEvaluation;

// Interface for game state
class IGameState {
public:
    virtual std::shared_ptr<IGameState> copy() = 0;
    virtual ~IGameState() = default;
};

// Interface for game move
class IGameMove {
public:
    virtual void execute(std::shared_ptr<IGameState> state) = 0;
    virtual void undo(std::shared_ptr<IGameState> state) = 0;
    virtual ~IGameMove() = default;
};

// Interface for player
class IPlayer {
public:
    virtual std::vector<std::shared_ptr<IGameMove>> validMoves(std::shared_ptr<IGameState> state) = 0;
    virtual int eval(std::shared_ptr<IGameState> state) = 0;
    virtual ~IPlayer() = default;
};

// Interface for evaluation
class IEvaluation {
public:
    virtual std::shared_ptr<IGameMove> bestMove(std::shared_ptr<IGameState> s,
        std::shared_ptr<IPlayer> player,
        std::shared_ptr<IPlayer> opponent) = 0;
    virtual ~IEvaluation() = default;
};

// Move evaluation class
class MoveEvaluation {
public:
    std::shared_ptr<IGameMove> move;
    int score;

    // Constructor for leaf nodes (just score)
    explicit MoveEvaluation(int score) : move(nullptr), score(score) {}

    // Constructor for internal nodes (move and score)
    MoveEvaluation(std::shared_ptr<IGameMove> m, int s) : move(m), score(s) {}

    static int minimum() {
        return std::numeric_limits<int>::min();
    }
};

// NegMax evaluation class
class NegMaxEvaluation : public IEvaluation {
private:
    std::shared_ptr<IGameState> state;
    int ply;

    MoveEvaluation negmax(int ply, std::shared_ptr<IPlayer> player,
        std::shared_ptr<IPlayer> opponent) {
        // Get valid moves for current player
        auto moves = player->validMoves(state);

        // If no moves or leaf node, return evaluation
        if (ply == 0 || moves.empty()) {
            return MoveEvaluation(player->eval(state));
        }

        // Initialize best move with minimum possible score
        MoveEvaluation best(MoveEvaluation::minimum());

        // Try all possible moves
        for (const auto& move : moves) {
            // Make move
            move->execute(state);

            // Recursively evaluate position from opponent's perspective
            MoveEvaluation me = negmax(ply - 1, opponent, player);

            // Undo move
            move->undo(state);

            // Update best move if we found a better score
            if (-me.score > best.score) {
                best = MoveEvaluation(move, -me.score);
            }
        }

        return best;
    }

public:
    explicit NegMaxEvaluation(int searchPly) : ply(searchPly) {}

    std::shared_ptr<IGameMove> bestMove(std::shared_ptr<IGameState> s,
        std::shared_ptr<IPlayer> player,
        std::shared_ptr<IPlayer> opponent) override {
        state = s->copy();
        MoveEvaluation me = negmax(ply, player, opponent);
        return me.move;
    }
};

// Example concrete implementations for testing
class SimpleGameState : public IGameState {
public:
    std::vector<int> board;

    SimpleGameState() : board(9, 0) {} // 3x3 board initialized with zeros

    std::shared_ptr<IGameState> copy() override {
        auto newState = std::make_shared<SimpleGameState>();
        newState->board = this->board;
        return newState;
    }
};

class SimpleGameMove : public IGameMove {
public:
    int position;
    int player; // 1 or -1

    SimpleGameMove(int pos, int p) : position(pos), player(p) {}

    void execute(std::shared_ptr<IGameState> state) override {
        auto gameState = std::dynamic_pointer_cast<SimpleGameState>(state);
        gameState->board[position] = player;
    }

    void undo(std::shared_ptr<IGameState> state) override {
        auto gameState = std::dynamic_pointer_cast<SimpleGameState>(state);
        gameState->board[position] = 0;
    }
};

class SimplePlayer : public IPlayer {
private:
    int playerMark; // 1 or -1

public:
    explicit SimplePlayer(int mark) : playerMark(mark) {}

    std::vector<std::shared_ptr<IGameMove>> validMoves(std::shared_ptr<IGameState> state) override {
        auto gameState = std::dynamic_pointer_cast<SimpleGameState>(state);
        std::vector<std::shared_ptr<IGameMove>> moves;

        for (int i = 0; i < 9; i++) {
            if (gameState->board[i] == 0) {
                moves.push_back(std::make_shared<SimpleGameMove>(i, playerMark));
            }
        }
        return moves;
    }

    int eval(std::shared_ptr<IGameState> state) override {
        // Simple evaluation: count pieces
        auto gameState = std::dynamic_pointer_cast<SimpleGameState>(state);
        int score = 0;
        for (int val : gameState->board) {
            if (val == playerMark) score++;
            else if (val == -playerMark) score--;
        }
        return score;
    }
};

int main() {
    // Create game state
    auto gameState = std::make_shared<SimpleGameState>();

    // Create players
    auto player1 = std::make_shared<SimplePlayer>(1);
    auto player2 = std::make_shared<SimplePlayer>(-1);

    // Create evaluator with search depth of 3
    NegMaxEvaluation evaluator(3);

    // Get best move for player1
    auto bestMove = evaluator.bestMove(gameState, player1, player2);

    if (bestMove) {
        auto move = std::dynamic_pointer_cast<SimpleGameMove>(bestMove);
        std::cout << "Best move for Player 1: position " << move->position << std::endl;
    }
    else {
        std::cout << "No valid moves available" << std::endl;
    }

    return 0;
}