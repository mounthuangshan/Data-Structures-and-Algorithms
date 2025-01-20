#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//散列表的大小
#define TABLE_SIZE 10

// 哈希表中的节点
typedef struct Node {
    int key;
    int value;
    struct Node* next;
} Node;

// 哈希表
typedef struct HashTable {
    Node* table[TABLE_SIZE];
} HashTable;

// 创建一个新的节点
Node* create_node(int key, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

// 创建哈希表
HashTable* create_table() {
    HashTable* hash_table = (HashTable*)malloc(sizeof(HashTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_table->table[i] = NULL;
    }
    return hash_table;
}

// 哈希函数
int hash_function(int key) {
    return key % TABLE_SIZE;
}

// 插入键值对
void insert(HashTable* hash_table, int key, int value) {
    int index = hash_function(key);
    Node* new_node = create_node(key, value);

    // 如果该位置为空，直接插入
    if (hash_table->table[index] == NULL) {
        hash_table->table[index] = new_node;
    }
    else {
        // 否则在链表的头部插入
        new_node->next = hash_table->table[index];
        hash_table->table[index] = new_node;
    }
}

// 查找键对应的值
int search(HashTable* hash_table, int key) {
    int index = hash_function(key);
    Node* temp = hash_table->table[index];

    // 遍历链表查找键
    while (temp != NULL) {
        if (temp->key == key) {
            return temp->value;
        }
        temp = temp->next;
    }

    // 如果没找到，返回 -1
    return -1;
}

// 删除键值对
void delete(HashTable* hash_table, int key) {
    int index = hash_function(key);
    Node* temp = hash_table->table[index];
    Node* prev = NULL;

    // 查找要删除的节点
    while (temp != NULL && temp->key != key) {
        prev = temp;
        temp = temp->next;
    }

    // 如果未找到要删除的节点
    if (temp == NULL) {
        printf("Key %d not found.\n", key);
        return;
    }

    // 如果找到了要删除的节点
    if (prev == NULL) {
        // 要删除的是第一个节点
        hash_table->table[index] = temp->next;
    }
    else {
        // 要删除的节点在链表中
        prev->next = temp->next;
    }

    free(temp);
    printf("Key %d deleted.\n", key);
}

// 打印哈希表
void print_table(HashTable* hash_table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Index %d: ", i);
        Node* temp = hash_table->table[i];
        while (temp != NULL) {
            printf("(Key: %d, Value: %d) -> ", temp->key, temp->value);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

int main() {
    HashTable* hash_table = create_table();

    insert(hash_table, 1, 10);
    insert(hash_table, 2, 20);
    insert(hash_table, 11, 110); // 产生冲突，将存储在相同的索引位置
    insert(hash_table, 21, 210); // 冲突

    print_table(hash_table);

    int key = 11;
    int value = search(hash_table, key);
    if (value != -1) {
        printf("Key %d has value %d\n", key, value);
    }
    else {
        printf("Key %d not found.\n", key);
    }

    delete(hash_table, 11);
    print_table(hash_table);

    return 0;
}
/*
这段哈希表的代码可以通过以下几种方式进行优化：
** 1.哈希函数改进 * *：当前的哈希函数 key % TABLE_SIZE 比较简单，可能在某些数据分布下会产生较多的冲突。可以考虑使用更复杂的哈希函数以减少冲突，如：
乘法哈希法：使用更复杂的数学运算进行哈希映射。
装载因子控制：根据元素数量动态调整哈希表大小，减少冲突。
链表优化（尾插法）：插入时使用尾插法而不是头插法，可以让链表保持插入顺序，减少冲突时的顺序混乱，方便查找和删除。
* *2.内存管理优化 * *：对于较大规模的哈希表，可以使用动态内存管理来动态调整哈希表大小，避免浪费内存或哈希表太小导致频繁冲突。
* *3.增量扩容 * *：当哈希表中的元素超过一定的装载因子时（如 0.75），可以动态调整哈希表大小并重新哈希已有元素（rehashing），从而降低冲突率，提高性能。
* *4.使用标记删除 * *：对于删除操作，可以考虑使用标记删除而不是物理删除，特别是当哈希表较大时，删除操作可能会影响查找性能。标记删除可以延迟删除操作，等到需要扩容或重建哈希表时再统一清理。
优化后的代码如下：
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_TABLE_SIZE 10
#define LOAD_FACTOR_THRESHOLD 0.75

// 哈希表中的节点
typedef struct Node {
    int key;
    int value;
    struct Node* next;
} Node;

// 哈希表
typedef struct HashTable {
    Node** table;
    int size;     // 当前哈希表大小
    int count;    // 当前表中的元素个数
} HashTable;

// 创建一个新的节点
Node* create_node(int key, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

// 创建哈希表
HashTable* create_table(int size) {
    HashTable* hash_table = (HashTable*)malloc(sizeof(HashTable));
    hash_table->table = (Node**)malloc(sizeof(Node*) * size);
    hash_table->size = size;
    hash_table->count = 0;
    for (int i = 0; i < size; i++) {
        hash_table->table[i] = NULL;
    }
    return hash_table;
}

// 哈希函数 (改进版，使用乘法哈希)
unsigned int hash_function(int key, int size) {
    const double A = 0.6180339887; // 取黄金分割比例
    return (unsigned int)(size * (key * A - (int)(key * A)));
}

// 重新哈希表（扩容）
void resize_table(HashTable* hash_table);

// 插入键值对
void insert(HashTable* hash_table, int key, int value) {
    // 检查装载因子
    if ((double)hash_table->count / hash_table->size >= LOAD_FACTOR_THRESHOLD) {
        resize_table(hash_table);
    }

    int index = hash_function(key, hash_table->size);
    Node* new_node = create_node(key, value);

    // 尾插法插入到链表末尾
    Node* current = hash_table->table[index];
    if (current == NULL) {
        hash_table->table[index] = new_node;
    }
    else {
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    hash_table->count++;
}

// 查找键对应的值
int search(HashTable* hash_table, int key) {
    int index = hash_function(key, hash_table->size);
    Node* temp = hash_table->table[index];

    // 遍历链表查找键
    while (temp != NULL) {
        if (temp->key == key) {
            return temp->value;
        }
        temp = temp->next;
    }
    return -1;
}

// 删除键值对
void delete(HashTable* hash_table, int key) {
    int index = hash_function(key, hash_table->size);
    Node* temp = hash_table->table[index];
    Node* prev = NULL;

    // 查找要删除的节点
    while (temp != NULL && temp->key != key) {
        prev = temp;
        temp = temp->next;
    }

    // 如果未找到要删除的节点
    if (temp == NULL) {
        printf("Key %d not found.\n", key);
        return;
    }

    // 删除节点
    if (prev == NULL) {
        hash_table->table[index] = temp->next;
    }
    else {
        prev->next = temp->next;
    }

    free(temp);
    hash_table->count--;
    printf("Key %d deleted.\n", key);
}

// 重新哈希表（扩容函数）
void resize_table(HashTable* hash_table) {
    int new_size = hash_table->size * 2;
    Node** new_table = (Node**)malloc(sizeof(Node*) * new_size);

    for (int i = 0; i < new_size; i++) {
        new_table[i] = NULL;
    }

    // 重新插入所有旧元素到新表中
    for (int i = 0; i < hash_table->size; i++) {
        Node* current = hash_table->table[i];
        while (current != NULL) {
            Node* next = current->next;
            int new_index = hash_function(current->key, new_size);

            // 采用尾插法将节点插入新表
            current->next = new_table[new_index];
            new_table[new_index] = current;
            current = next;
        }
    }

    free(hash_table->table);  // 释放旧表
    hash_table->table = new_table;
    hash_table->size = new_size;
}

// 打印哈希表
void print_table(HashTable* hash_table) {
    for (int i = 0; i < hash_table->size; i++) {
        printf("Index %d: ", i);
        Node* temp = hash_table->table[i];
        while (temp != NULL) {
            printf("(Key: %d, Value: %d) -> ", temp->key, temp->value);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

int main() {
    HashTable* hash_table = create_table(INITIAL_TABLE_SIZE);

    insert(hash_table, 1, 10);
    insert(hash_table, 2, 20);
    insert(hash_table, 11, 110);
    insert(hash_table, 21, 210);
    insert(hash_table, 31, 310);  // 扩容触发

    print_table(hash_table);

    int key = 11;
    int value = search(hash_table, key);
    if (value != -1) {
        printf("Key %d has value %d\n", key, value);
    }
    else {
        printf("Key %d not found.\n", key);
    }

    delete(hash_table, 11);
    print_table(hash_table);

    return 0;
}
*/