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

// Evaluation interface
class IEvaluation {
public:
    virtual std::shared_ptr<IGameMove> bestMove(std::shared_ptr<IGameState> state,
        std::shared_ptr<IPlayer> player,
        std::shared_ptr<IPlayer> opponent) = 0;
    virtual ~IEvaluation() = default;
};

// Move evaluation class
class MoveEvaluation {
public:
    std::shared_ptr<IGameMove> move;
    int score;

    explicit MoveEvaluation(int score) : move(nullptr), score(score) {}

    MoveEvaluation(std::shared_ptr<IGameMove> m, int s) : move(m), score(s) {}

    static int minimum() { return std::numeric_limits<int>::min(); }
    static int maximum() { return std::numeric_limits<int>::max(); }
};

// AlphaBeta evaluation class
class AlphaBetaEvaluation : public IEvaluation {
private:
    std::shared_ptr<IGameState> state;
    int ply;

    MoveEvaluation alphabeta(int ply, std::shared_ptr<IPlayer> player,
        std::shared_ptr<IPlayer> opponent,
        int alpha, int beta) {
        // Get valid moves
        auto moves = player->validMoves(state);

        // If leaf node or no moves available, return evaluation
        if (ply == 0 || moves.empty()) {
            return MoveEvaluation(player->eval(state));
        }

        // Initialize best move with alpha
        MoveEvaluation best(alpha);

        // Try each move and update alpha
        for (const auto& move : moves) {
            // Execute move
            move->execute(state);

            // Recursively evaluate position
            MoveEvaluation me = alphabeta(ply - 1, opponent, player, -beta, -alpha);

            // Undo move
            move->undo(state);

            // Update alpha if better move found
            if (-me.score > alpha) {
                alpha = -me.score;
                best = MoveEvaluation(move, alpha);
            }

            // Pruning condition
            if (alpha >= beta) {
                return best;
            }
        }

        return best;
    }

public:
    explicit AlphaBetaEvaluation(int searchPly) : ply(searchPly) {}

    std::shared_ptr<IGameMove> bestMove(std::shared_ptr<IGameState> s,
        std::shared_ptr<IPlayer> player,
        std::shared_ptr<IPlayer> opponent) override {
        state = s->copy();
        MoveEvaluation move = alphabeta(ply, player, opponent,
            MoveEvaluation::minimum(),
            MoveEvaluation::maximum());
        return move.move;
    }
};

// Example implementations for testing
class TicTacToeState : public IGameState {
public:
    std::vector<int> board;

    TicTacToeState() : board(9, 0) {}

    std::shared_ptr<IGameState> copy() override {
        auto newState = std::make_shared<TicTacToeState>();
        newState->board = this->board;
        return newState;
    }
};

class TicTacToeMove : public IGameMove {
public:
    int position;
    int player; // 1 or -1

    TicTacToeMove(int pos, int p) : position(pos), player(p) {}

    void execute(std::shared_ptr<IGameState> state) override {
        auto gameState = std::dynamic_pointer_cast<TicTacToeState>(state);
        gameState->board[position] = player;
    }

    void undo(std::shared_ptr<IGameState> state) override {
        auto gameState = std::dynamic_pointer_cast<TicTacToeState>(state);
        gameState->board[position] = 0;
    }
};

class TicTacToePlayer : public IPlayer {
private:
    int playerMark;

public:
    explicit TicTacToePlayer(int mark) : playerMark(mark) {}

    std::vector<std::shared_ptr<IGameMove>> validMoves(std::shared_ptr<IGameState> state) override {
        auto gameState = std::dynamic_pointer_cast<TicTacToeState>(state);
        std::vector<std::shared_ptr<IGameMove>> moves;

        for (int i = 0; i < 9; i++) {
            if (gameState->board[i] == 0) {
                moves.push_back(std::make_shared<TicTacToeMove>(i, playerMark));
            }
        }
        return moves;
    }

    int eval(std::shared_ptr<IGameState> state) override {
        auto gameState = std::dynamic_pointer_cast<TicTacToeState>(state);

        // Check for win conditions
        const std::vector<std::vector<int>> lines = {
            {0,1,2}, {3,4,5}, {6,7,8}, // rows
            {0,3,6}, {1,4,7}, {2,5,8}, // columns
            {0,4,8}, {2,4,6}           // diagonals
        };

        for (const auto& line : lines) {
            int sum = 0;
            for (int pos : line) {
                sum += gameState->board[pos];
            }
            if (sum == 3 * playerMark) return 1000;
            if (sum == -3 * playerMark) return -1000;
        }

        return 0;
    }
};

int main() {
    // Create initial game state
    auto gameState = std::make_shared<TicTacToeState>();

    // Create players
    auto player1 = std::make_shared<TicTacToePlayer>(1);  // X player
    auto player2 = std::make_shared<TicTacToePlayer>(-1); // O player

    // Create evaluator with search depth of 4
    AlphaBetaEvaluation evaluator(4);

    // Make some moves to set up a game state
    gameState->board = {
        1, 0, -1,
        0, 1, 0,
        -1, 0, 0
    };

    // Get best move for player1
    auto bestMove = evaluator.bestMove(gameState, player1, player2);

    if (bestMove) {
        auto move = std::dynamic_pointer_cast<TicTacToeMove>(bestMove);
        std::cout << "Best move for Player 1 (X): position " << move->position << std::endl;

        // Execute the move to show the resulting board
        move->execute(gameState);
        std::cout << "\nResulting board:\n";
        for (int i = 0; i < 9; i++) {
            char symbol = gameState->board[i] == 1 ? 'X' :
                gameState->board[i] == -1 ? 'O' : '.';
            std::cout << symbol;
            if (i % 3 == 2) std::cout << "\n";
        }
    }
    else {
        std::cout << "No valid moves available\n";
    }

    return 0;
}