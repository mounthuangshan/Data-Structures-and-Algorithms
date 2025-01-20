#include <stdio.h>
#include <stdlib.h>

// ����ڵ���ɫ
#define RED 1
#define BLACK 0

// ���������ڵ�
struct Node {
    int data;
    int color;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
};

// ���� NIL �ڱ��ڵ�
struct Node* NIL = NULL;

// �����½ڵ�
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->color = RED;  // �²���ڵ�Ĭ��Ϊ��ɫ
    newNode->left = NIL;
    newNode->right = NIL;
    newNode->parent = NULL;
    return newNode;
}

// ����
void leftRotate(struct Node** root, struct Node* x) {
    struct Node* y = x->right;
    x->right = y->left;

    if (y->left != NIL) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == NULL) {
        *root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

// ����
void rightRotate(struct Node** root, struct Node* y) {
    struct Node* x = y->left;
    y->left = x->right;

    if (x->right != NIL) {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent == NULL) {
        *root = x;
    }
    else if (y == y->parent->right) {
        y->parent->right = x;
    }
    else {
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

// �޸������
void fixInsert(struct Node** root, struct Node* z) {
    while (z->parent != NULL && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            struct Node* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        }
        else {
            struct Node* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

// ����ڵ�
void insert(struct Node** root, int data) {
    struct Node* z = createNode(data);
    struct Node* y = NULL;
    struct Node* x = *root;

    while (x != NIL) {
        y = x;
        if (z->data < x->data) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }

    z->parent = y;

    if (y == NULL) {
        *root = z;
    }
    else if (z->data < y->data) {
        y->left = z;
    }
    else {
        y->right = z;
    }

    z->left = NIL;
    z->right = NIL;
    z->color = RED;

    fixInsert(root, z);
}

// �������
void inorder(struct Node* root) {
    if (root != NIL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

// ��ʼ�� NIL �ڵ�
void initNILNode() {
    NIL = (struct Node*)malloc(sizeof(struct Node));
    NIL->color = BLACK;
    NIL->left = NIL->right = NIL->parent = NULL;
}

int main() {
    initNILNode();  // ��ʼ�� NIL �ڱ��ڵ�
    struct Node* root = NIL;

    insert(&root, 10);
    insert(&root, 20);
    insert(&root, 30);
    insert(&root, 40);
    insert(&root, 50);
    insert(&root, 25);

    printf("�������: ");
    inorder(root);
    printf("\n");

    return 0;
}