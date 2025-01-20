#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ���峣��
#define MAX_MOVES 10
#define MAX_STATES 100

// ����ṹ��
typedef struct INode INode;
typedef struct IMove IMove;
typedef struct Solution Solution;
typedef struct DepthTransition DepthTransition;
typedef struct INodeSet INodeSet;

// INode �ṹ��
struct INode {
    char state[20]; // ����״̬��һ���ַ���
    IMove* validMoves[MAX_MOVES];
    int moveCount;
    DepthTransition* storedData;
    int score; // ���ڴ洢����
};

// IMove �ṹ��
struct IMove {
    char nextState[20]; // �����ƶ���ı�״̬
};

// Solution �ṹ��
struct Solution {
    INode* initialState;
    INode* goalState;
    int solved;
};

// DepthTransition �ṹ��
struct DepthTransition {
    IMove* move;
    INode* parent;
    int depth;
};

// INodeSet �ṹ��
struct INodeSet {
    INode* nodes[MAX_STATES];
    int count;
};

// ��������
INode* createNode(const char* state);
IMove* createMove(const char* nextState);
Solution* createSolution(INode* initial, INode* goal, int solved);
DepthTransition* createDepthTransition(IMove* move, INode* parent, int depth);
INodeSet* createNodeSet();
void insertNode(INodeSet* set, INode* node);
INode* removeNode(INodeSet* set);
int isEmpty(INodeSet* set);
INode* containsNode(INodeSet* set, INode* node);
void scoreNode(INode* node); // ģ�����ֺ���
Solution* search(INode* initial, INode* goal);

// ���� INode
INode* createNode(const char* state) {
    INode* node = (INode*)malloc(sizeof(INode));
    strcpy(node->state, state);
    node->moveCount = 0;
    node->storedData = NULL;
    node->score = 0;
    return node;
}

// ���� IMove
IMove* createMove(const char* nextState) {
    IMove* move = (IMove*)malloc(sizeof(IMove));
    strcpy(move->nextState, nextState);
    return move;
}

// ���� Solution
Solution* createSolution(INode* initial, INode* goal, int solved) {
    Solution* solution = (Solution*)malloc(sizeof(Solution));
    solution->initialState = initial;
    solution->goalState = goal;
    solution->solved = solved;
    return solution;
}

// ���� DepthTransition
DepthTransition* createDepthTransition(IMove* move, INode* parent, int depth) {
    DepthTransition* trans = (DepthTransition*)malloc(sizeof(DepthTransition));
    trans->move = move;
    trans->parent = parent;
    trans->depth = depth;
    return trans;
}

// ���� INodeSet
INodeSet* createNodeSet() {
    INodeSet* set = (INodeSet*)malloc(sizeof(INodeSet));
    set->count = 0;
    return set;
}

// ����ڵ㵽 INodeSet
void insertNode(INodeSet* set, INode* node) {
    if (set->count < MAX_STATES) {
        set->nodes[set->count++] = node;
    }
}

// �� INodeSet �Ƴ��ڵ�
INode* removeNode(INodeSet* set) {
    if (set->count > 0) {
        return set->nodes[--set->count];
    }
    return NULL;
}

// ��� INodeSet �Ƿ�Ϊ��
int isEmpty(INodeSet* set) {
    return set->count == 0;
}

// ��� INodeSet �Ƿ����ĳ���ڵ�
INode* containsNode(INodeSet* set, INode* node) {
    for (int i = 0; i < set->count; i++) {
        if (strcmp(set->nodes[i]->state, node->state) == 0) {
            return set->nodes[i];
        }
    }
    return NULL;
}

// ģ�����ֺ���
void scoreNode(INode* node) {
    // ������Ը��ݾ�������ʵ�������߼�
    node->score = rand() % 100; // �������һ������
}

// ��������
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
                // �ӱպϼ����Ƴ�
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

// ������
int main() {
    // ������ʼ״̬��Ŀ��״̬
    INode* initial = createNode("state1");
    INode* goal = createNode("state3");

    // ���� validMoves
    initial->validMoves[initial->moveCount++] = createMove("state2");

    INode* state2 = createNode("state2");
    state2->validMoves[state2->moveCount++] = createMove("state3");

    // ������������
    Solution* result = search(initial, goal);

    // ������
    if (result->solved) {
        printf("Solution found!\n");
        printf("Initial state: %s\n", result->initialState->state);
        printf("Goal state: %s\n", result->goalState->state);
    } else {
        printf("No solution found.\n");
    }

    // �ͷ��ڴ�
    free(initial);
    free(goal);
    free(state2);
    free(result);

    return 0;
}