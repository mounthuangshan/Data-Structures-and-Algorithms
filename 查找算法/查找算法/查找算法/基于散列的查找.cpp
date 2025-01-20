#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//ɢ�б�Ĵ�С
#define TABLE_SIZE 10

// ��ϣ���еĽڵ�
typedef struct Node {
    int key;
    int value;
    struct Node* next;
} Node;

// ��ϣ��
typedef struct HashTable {
    Node* table[TABLE_SIZE];
} HashTable;

// ����һ���µĽڵ�
Node* create_node(int key, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

// ������ϣ��
HashTable* create_table() {
    HashTable* hash_table = (HashTable*)malloc(sizeof(HashTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_table->table[i] = NULL;
    }
    return hash_table;
}

// ��ϣ����
int hash_function(int key) {
    return key % TABLE_SIZE;
}

// �����ֵ��
void insert(HashTable* hash_table, int key, int value) {
    int index = hash_function(key);
    Node* new_node = create_node(key, value);

    // �����λ��Ϊ�գ�ֱ�Ӳ���
    if (hash_table->table[index] == NULL) {
        hash_table->table[index] = new_node;
    }
    else {
        // �����������ͷ������
        new_node->next = hash_table->table[index];
        hash_table->table[index] = new_node;
    }
}

// ���Ҽ���Ӧ��ֵ
int search(HashTable* hash_table, int key) {
    int index = hash_function(key);
    Node* temp = hash_table->table[index];

    // ����������Ҽ�
    while (temp != NULL) {
        if (temp->key == key) {
            return temp->value;
        }
        temp = temp->next;
    }

    // ���û�ҵ������� -1
    return -1;
}

// ɾ����ֵ��
void delete(HashTable* hash_table, int key) {
    int index = hash_function(key);
    Node* temp = hash_table->table[index];
    Node* prev = NULL;

    // ����Ҫɾ���Ľڵ�
    while (temp != NULL && temp->key != key) {
        prev = temp;
        temp = temp->next;
    }

    // ���δ�ҵ�Ҫɾ���Ľڵ�
    if (temp == NULL) {
        printf("Key %d not found.\n", key);
        return;
    }

    // ����ҵ���Ҫɾ���Ľڵ�
    if (prev == NULL) {
        // Ҫɾ�����ǵ�һ���ڵ�
        hash_table->table[index] = temp->next;
    }
    else {
        // Ҫɾ���Ľڵ���������
        prev->next = temp->next;
    }

    free(temp);
    printf("Key %d deleted.\n", key);
}

// ��ӡ��ϣ��
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
    insert(hash_table, 11, 110); // ������ͻ�����洢����ͬ������λ��
    insert(hash_table, 21, 210); // ��ͻ

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
��ι�ϣ��Ĵ������ͨ�����¼��ַ�ʽ�����Ż���
** 1.��ϣ�����Ľ� * *����ǰ�Ĺ�ϣ���� key % TABLE_SIZE �Ƚϼ򵥣�������ĳЩ���ݷֲ��»�����϶�ĳ�ͻ�����Կ���ʹ�ø����ӵĹ�ϣ�����Լ��ٳ�ͻ���磺
�˷���ϣ����ʹ�ø����ӵ���ѧ������й�ϣӳ�䡣
װ�����ӿ��ƣ�����Ԫ��������̬������ϣ���С�����ٳ�ͻ��
�����Ż���β�巨��������ʱʹ��β�巨������ͷ�巨�������������ֲ���˳�򣬼��ٳ�ͻʱ��˳����ң�������Һ�ɾ����
* *2.�ڴ�����Ż� * *�����ڽϴ��ģ�Ĺ�ϣ������ʹ�ö�̬�ڴ��������̬������ϣ���С�������˷��ڴ���ϣ��̫С����Ƶ����ͻ��
* *3.�������� * *������ϣ���е�Ԫ�س���һ����װ������ʱ���� 0.75�������Զ�̬������ϣ���С�����¹�ϣ����Ԫ�أ�rehashing�����Ӷ����ͳ�ͻ�ʣ�������ܡ�
* *4.ʹ�ñ��ɾ�� * *������ɾ�����������Կ���ʹ�ñ��ɾ������������ɾ�����ر��ǵ���ϣ��ϴ�ʱ��ɾ���������ܻ�Ӱ��������ܡ����ɾ�������ӳ�ɾ���������ȵ���Ҫ���ݻ��ؽ���ϣ��ʱ��ͳһ����
�Ż���Ĵ������£�
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_TABLE_SIZE 10
#define LOAD_FACTOR_THRESHOLD 0.75

// ��ϣ���еĽڵ�
typedef struct Node {
    int key;
    int value;
    struct Node* next;
} Node;

// ��ϣ��
typedef struct HashTable {
    Node** table;
    int size;     // ��ǰ��ϣ���С
    int count;    // ��ǰ���е�Ԫ�ظ���
} HashTable;

// ����һ���µĽڵ�
Node* create_node(int key, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

// ������ϣ��
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

// ��ϣ���� (�Ľ��棬ʹ�ó˷���ϣ)
unsigned int hash_function(int key, int size) {
    const double A = 0.6180339887; // ȡ�ƽ�ָ����
    return (unsigned int)(size * (key * A - (int)(key * A)));
}

// ���¹�ϣ�����ݣ�
void resize_table(HashTable* hash_table);

// �����ֵ��
void insert(HashTable* hash_table, int key, int value) {
    // ���װ������
    if ((double)hash_table->count / hash_table->size >= LOAD_FACTOR_THRESHOLD) {
        resize_table(hash_table);
    }

    int index = hash_function(key, hash_table->size);
    Node* new_node = create_node(key, value);

    // β�巨���뵽����ĩβ
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

// ���Ҽ���Ӧ��ֵ
int search(HashTable* hash_table, int key) {
    int index = hash_function(key, hash_table->size);
    Node* temp = hash_table->table[index];

    // ����������Ҽ�
    while (temp != NULL) {
        if (temp->key == key) {
            return temp->value;
        }
        temp = temp->next;
    }
    return -1;
}

// ɾ����ֵ��
void delete(HashTable* hash_table, int key) {
    int index = hash_function(key, hash_table->size);
    Node* temp = hash_table->table[index];
    Node* prev = NULL;

    // ����Ҫɾ���Ľڵ�
    while (temp != NULL && temp->key != key) {
        prev = temp;
        temp = temp->next;
    }

    // ���δ�ҵ�Ҫɾ���Ľڵ�
    if (temp == NULL) {
        printf("Key %d not found.\n", key);
        return;
    }

    // ɾ���ڵ�
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

// ���¹�ϣ�����ݺ�����
void resize_table(HashTable* hash_table) {
    int new_size = hash_table->size * 2;
    Node** new_table = (Node**)malloc(sizeof(Node*) * new_size);

    for (int i = 0; i < new_size; i++) {
        new_table[i] = NULL;
    }

    // ���²������о�Ԫ�ص��±���
    for (int i = 0; i < hash_table->size; i++) {
        Node* current = hash_table->table[i];
        while (current != NULL) {
            Node* next = current->next;
            int new_index = hash_function(current->key, new_size);

            // ����β�巨���ڵ�����±�
            current->next = new_table[new_index];
            new_table[new_index] = current;
            current = next;
        }
    }

    free(hash_table->table);  // �ͷžɱ�
    hash_table->table = new_table;
    hash_table->size = new_size;
}

// ��ӡ��ϣ��
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
    insert(hash_table, 31, 310);  // ���ݴ���

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