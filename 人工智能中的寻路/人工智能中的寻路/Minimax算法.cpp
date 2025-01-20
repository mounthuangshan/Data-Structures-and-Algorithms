#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义常量
#define MAX_MOVES 10
#define MAX_STATES 100

// 定义结构体
typedef struct INode INode;
typedef struct IMove IMove;
typedef struct Solution Solution;
typedef struct DepthTransition DepthTransition;
typedef struct INodeSet INodeSet;

// INode 结构体
struct INode {
    char state[20]; // 假设状态是一个字符串
    IMove* validMoves[MAX_MOVES];
    int moveCount;
    DepthTransition* storedData;
    int score; // 用于存储评分
};

// IMove 结构体
struct IMove {
    char nextState[20]; // 假设移动会改变状态
};

// Solution 结构体
struct Solution {
    INode* initialState;
    INode* goalState;
    int solved;
};

// DepthTransition 结构体
struct DepthTransition {
    IMove* move;
    INode* parent;
    int depth;
};

// INodeSet 结构体
struct INodeSet {
    INode* nodes[MAX_STATES];
    int count;
};

// 函数声明
INode* createNode(const char* state);
IMove* createMove(const char* nextState);
Solution* createSolution(INode* initial, INode* goal, int solved);
DepthTransition* createDepthTransition(IMove* move, INode* parent, int depth);
INodeSet* createNodeSet();
void insertNode(INodeSet* set, INode* node);
INode* removeNode(INodeSet* set);
int isEmpty(INodeSet* set);
INode* containsNode(INodeSet* set, INode* node);
void scoreNode(INode* node); // 模拟评分函数
Solution* search(INode* initial, INode* goal);

// 创建 INode
INode* createNode(const char* state) {
    INode* node = (INode*)malloc(sizeof(INode));
    strcpy(node->state, state);
    node->moveCount = 0;
    node->storedData = NULL;
    node->score = 0;
    return node;
}

// 创建 IMove
IMove* createMove(const char* nextState) {
    IMove* move = (IMove*)malloc(sizeof(IMove));
    strcpy(move->nextState, nextState);
    return move;
}

// 创建 Solution
Solution* createSolution(INode* initial, INode* goal, int solved) {
    Solution* solution = (Solution*)malloc(sizeof(Solution));
    solution->initialState = initial;
    solution->goalState = goal;
    solution->solved = solved;
    return solution;
}

// 创建 DepthTransition
DepthTransition* createDepthTransition(IMove* move, INode* parent, int depth) {
    DepthTransition* trans = (DepthTransition*)malloc(sizeof(DepthTransition));
    trans->move = move;
    trans->parent = parent;
    trans->depth = depth;
    return trans;
}

// 创建 INodeSet
INodeSet* createNodeSet() {
    INodeSet* set = (INodeSet*)malloc(sizeof(INodeSet));
    set->count = 0;
    return set;
}

// 插入节点到 INodeSet
void insertNode(INodeSet* set, INode* node) {
    if (set->count < MAX_STATES) {
        set->nodes[set->count++] = node;
    }
}

// 从 INodeSet 移除节点
INode* removeNode(INodeSet* set) {
    if (set->count > 0) {
        return set->nodes[--set->count];
    }
    return NULL;
}

// 检查 INodeSet 是否为空
int isEmpty(INodeSet* set) {
    return set->count == 0;
}

// 检查 INodeSet 是否包含某个节点
INode* containsNode(INodeSet* set, INode* node) {
    for (int i = 0; i < set->count; i++) {
        if (strcmp(set->nodes[i]->state, node->state) == 0) {
            return set->nodes[i];
        }
    }
    return NULL;
}

// 模拟评分函数
void scoreNode(INode* node) {
    // 这里可以根据具体需求实现评分逻辑
    node->score = rand() % 100; // 随机生成一个分数
}

// 搜索函数
Solution* search(INode* initial, INode* goal) {
    INodeSet* open = createNodeSet();
    INode* copy = createNode(initial->state);
    scoreNode(copy);
    insertNode(open, copy);

    INodeSet* closed = createNodeSet();
    while (!isEmpty(open)) {
        INode* n = removeNode(open);
        insertNode(closed, n);

        if (strcmp(n->state, goal->state) == 0) {
            return createSolution(initial, n, 1);
        }

        DepthTransition* trans = n->storedData;
        int depth = 1;
        if (trans != NULL) {
            depth = trans->depth + 1;
        }

        for (int i = 0; i < n->moveCount; i++) {
            IMove* move = n->validMoves[i];
            INode* successor = createNode(n->state);
            strcpy(successor->state, move->nextState);

            successor->storedData = createDepthTransition(move, n, depth);
            scoreNode(successor);

            INode* past = containsNode(closed, successor);
            if (past != NULL) {
                if (successor->score >= past->score) {
                    free(successor);
                    continue;
                }
                // 从闭合集中移除
                for (int j = 0; j < closed->count; j++) {
                    if (closed->nodes[j] == past) {
                        closed->nodes[j] = closed->nodes[--closed->count];
                        break;
                    }
                }
            }

            insertNode(open, successor);
        }
    }

    return createSolution(initial, goal, 0);
}

// 主函数
int main() {
    // 创建初始状态和目标状态
    INode* initial = createNode("state1");
    INode* goal = createNode("state3");

    // 设置 validMoves
    initial->validMoves[initial->moveCount++] = createMove("state2");

    INode* state2 = createNode("state2");
    state2->validMoves[state2->moveCount++] = createMove("state3");

    // 调用搜索函数
    Solution* result = search(initial, goal);

    // 输出结果
    if (result->solved) {
        printf("Solution found!\n");
        printf("Initial state: %s\n", result->initialState->state);
        printf("Goal state: %s\n", result->goalState->state);
    } else {
        printf("No solution found.\n");
    }

    // 释放内存
    free(initial);
    free(goal);
    free(state2);
    free(result);

    return 0;
}