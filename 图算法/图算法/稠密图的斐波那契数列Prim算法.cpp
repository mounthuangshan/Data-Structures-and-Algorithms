#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <utility>
#include <unordered_map>
#include <cmath>

using namespace std;

// �ڽӱ�Ķ���
using VertexList = unordered_map<int, int>; // int: �ڽӶ���, int: ��Ȩ��
using AdjacencyList = vector<VertexList>;

// Graph �ඨ��
class Graph {
public:
    Graph(int vertices) : adjList(vertices) {}

    void addEdge(int u, int v, int weight) {
        adjList[u][v] = weight;
        adjList[v][u] = weight; // ����ͼ
    }

    int numVertices() const {
        return (int)adjList.size();
    }

    VertexList::const_iterator begin(int u) const {
        return adjList[u].begin();
    }

    VertexList::const_iterator end(int u) const {
        return adjList[u].end();
    }

private:
    AdjacencyList adjList;
};

// ------------------------- 쳲������ѵ�ʵ�� -------------------------
// 쳲������ѽڵ㶨��
struct FibNode {
    int key;
    int vertex;
    FibNode* parent;
    FibNode* child;
    FibNode* left;
    FibNode* right;
    int degree;
    bool mark;

    FibNode(int k, int v)
        : key(k), vertex(v), parent(nullptr), child(nullptr),
        left(this), right(this), degree(0), mark(false) {
    }
};

// 쳲��������ඨ��
class FibonacciHeap {
public:
    FibonacciHeap() : minNode(nullptr), nodeCount(0) {}

    // �����½ڵ㣬ʱ�临�Ӷ�O(1)
    FibNode* insert(int key, int vertex) {
        FibNode* node = new FibNode(key, vertex);
        // ���½ڵ���������
        if (minNode == nullptr) {
            minNode = node;
        }
        else {
            insertNodeToRootList(node, minNode);
            if (node->key < minNode->key) {
                minNode = node;
            }
        }
        nodeCount++;
        return node;
    }

    // ���ص�ǰ��С�ڵ�
    FibNode* minimum() const {
        return minNode;
    }

    // �ж϶��Ƿ�Ϊ��
    bool isEmpty() const {
        return minNode == nullptr;
    }

    // �ϲ�����쳲�������
    void merge(FibonacciHeap& other) {
        if (other.minNode == nullptr)
            return;

        if (minNode == nullptr) {
            minNode = other.minNode;
            nodeCount = other.nodeCount;
        }
        else {
            // ������������ϲ�
            concatenateRootList(minNode, other.minNode);
            if (other.minNode->key < minNode->key)
                minNode = other.minNode;
            nodeCount += other.nodeCount;
        }

        other.minNode = nullptr;
        other.nodeCount = 0;
    }

    // �Ӷ����Ƴ���С�ڵ㲢�������Ӧ����
    int extractMin() {
        FibNode* z = minNode;
        if (z != nullptr) {
            // ��z�ĺ���ȫ�����������
            FibNode* x = z->child;
            if (x != nullptr) {
                FibNode* start = x;
                do {
                    FibNode* next = x->right;
                    x->parent = nullptr;
                    insertNodeToRootList(x, minNode);
                    x = next;
                } while (x != start);
            }

            // ��z�Ӹ������Ƴ�
            removeNodeFromRootList(z);
            if (z == z->right) {
                minNode = nullptr;
            }
            else {
                minNode = z->right;
                consolidate();
            }
            nodeCount--;
            int v = z->vertex;
            delete z;
            return v;
        }
        return -1; // �ն�
    }

    // ��Сָ���ڵ�ļ�ֵ��ʱ�临�Ӷ� O(1) ̯��
    void decreaseKey(FibNode* x, int k) {
        if (k > x->key) {
            return; // �¼�ֵ���ô��ڵ�ǰ��ֵ
        }
        x->key = k;
        FibNode* y = x->parent;
        if (y != nullptr && x->key < y->key) {
            cut(x, y);
            cascadingCut(y);
        }
        if (x->key < minNode->key) {
            minNode = x;
        }
    }

    int size() const {
        return nodeCount;
    }

private:
    FibNode* minNode;
    int nodeCount;

    // ���ڵ�node���뵽a��b���ڵ�˫�������У���a��b����λ�ã�
    static void insertNodeToRootList(FibNode* node, FibNode* root) {
        node->left = root;
        node->right = root->right;
        root->right->left = node;
        root->right = node;
    }

    // �Ӹ������Ƴ��ڵ�node
    static void removeNodeFromRootList(FibNode* node) {
        node->left->right = node->right;
        node->right->left = node->left;
    }

    // ������˫��ѭ������ϲ�
    static void concatenateRootList(FibNode* a, FibNode* b) {
        if (a == nullptr || b == nullptr) return;
        FibNode* aRight = a->right;
        FibNode* bLeft = b->left;

        a->right = b;
        b->left = a;
        aRight->left = bLeft;
        bLeft->right = aRight;
    }

    // �ϲ����б��е�������֤ͬ�����������ظ�
    void consolidate() {
        int D = (int)floor(log2(nodeCount)) + 1;
        vector<FibNode*> A(D, nullptr);

        vector<FibNode*> rootList;
        {
            // ����ǰ���б������нڵ��ռ���rootList��
            FibNode* x = minNode;
            if (x != nullptr) {
                do {
                    rootList.push_back(x);
                    x = x->right;
                } while (x != minNode);
            }
        }

        for (FibNode* w : rootList) {
            FibNode* x = w;
            int d = x->degree;
            while (A[d] != nullptr) {
                FibNode* y = A[d];
                if (y->key < x->key) {
                    FibNode* temp = x;
                    x = y;
                    y = temp;
                }
                link(y, x);
                A[d] = nullptr;
                d++;
            }
            A[d] = x;
        }

        minNode = nullptr;
        for (FibNode* a : A) {
            if (a != nullptr) {
                if (minNode == nullptr) {
                    a->left = a;
                    a->right = a;
                    minNode = a;
                }
                else {
                    insertNodeToRootList(a, minNode);
                    if (a->key < minNode->key) {
                        minNode = a;
                    }
                }
            }
        }
    }

    // ���ڵ�y���ӵ�x�ϣ���Ϊx���ӽڵ�
    void link(FibNode* y, FibNode* x) {
        // �Ӹ��������Ƴ�y
        removeNodeFromRootList(y);
        // ��y��Ϊx���ӽڵ�
        if (x->child == nullptr) {
            x->child = y;
            y->left = y;
            y->right = y;
        }
        else {
            insertNodeToRootList(y, x->child);
        }
        y->parent = x;
        x->degree++;
        y->mark = false;
    }

    // �Ӹ��ڵ�y����x������x���������
    void cut(FibNode* x, FibNode* y) {
        // ��x��y�����������Ƴ�
        if (x->right == x) {
            y->child = nullptr;
        }
        else {
            x->left->right = x->right;
            x->right->left = x->left;
            if (y->child == x) {
                y->child = x->right;
            }
        }
        y->degree--;

        // x���������
        x->parent = nullptr;
        x->left = x;
        x->right = x;
        x->mark = false;
        insertNodeToRootList(x, minNode);
    }

    // ��������
    void cascadingCut(FibNode* y) {
        FibNode* z = y->parent;
        if (z != nullptr) {
            if (!y->mark) {
                y->mark = true;
            }
            else {
                cut(y, z);
                cascadingCut(z);
            }
        }
    }
};

// ------------------------- Prim �㷨ʵ�� -------------------------
// ʹ��쳲������Ѱ汾�����ȶ���ʵ�ֵ� Prim
void mst_prim(const Graph& graph, vector<int>& pred) {
    int n = graph.numVertices();
    pred.assign(n, -1);
    vector<int> key(n, numeric_limits<int>::max());
    key[0] = 0;

    FibonacciHeap fh;
    vector<FibNode*> nodePtr(n, nullptr);

    // ��ʼ�����ȶ���
    for (int v = 0; v < n; v++) {
        nodePtr[v] = fh.insert(key[v], v);
    }

    // ��ǽڵ��Ƿ������ȶ�����
    vector<bool> inQueue(n, true);

    while (!fh.isEmpty()) {
        int u = fh.extractMin();
        inQueue[u] = false;
        for (auto ci = graph.begin(u); ci != graph.end(u); ++ci) {
            int v = ci->first;
            int w = ci->second;
            if (inQueue[v] && w < key[v]) {
                key[v] = w;
                pred[v] = u;
                fh.decreaseKey(nodePtr[v], w);
            }
        }
    }
}

// ------------------------- ���Դ��� -------------------------
int main() {
    // ����ͼ��ʵ��
    Graph graph(5); // ���� 5 ������

    // ��ӱ�
    graph.addEdge(0, 1, 2);
    graph.addEdge(0, 3, 6);
    graph.addEdge(1, 2, 3);
    graph.addEdge(1, 3, 8);
    graph.addEdge(1, 4, 5);
    graph.addEdge(2, 4, 7);
    graph.addEdge(3, 4, 9);

    // �洢��С��������ǰ������
    vector<int> pred;

    // ���� Prim �㷨��쳲������Ѱ汾��
    mst_prim(graph, pred);

    // ��ӡ���
    cout << "��С�������ıߣ�\n";
    for (int i = 1; i < (int)pred.size(); i++) {
        if (pred[i] != -1) {
            cout << "��: (" << pred[i] << ", " << i << ")\n";
        }
    }

    return 0;
}

//�ɵĴ���
/*
#include <iostream>
#include <vector>
#include <limits>
#include <unordered_map>

using namespace std;

// �ڽӱ�Ķ���
using VertexList = unordered_map<int, int>; // int: �ڽӶ���, int: ��Ȩ��
using AdjacencyList = vector<VertexList>; // ����ͼ���ڽӱ�

// Graph �ඨ��
class Graph {
public:
    // ���캯������ʼ���ڽӱ�
    Graph(int vertices) : adjList(vertices) {}

    // ��ӱߵķ���
    void addEdge(int u, int v, int weight) {
        adjList[u][v] = weight; // �� u �� v �ı�
        adjList[v][u] = weight; // �� v �� u �ıߣ�����ͼ��
    }

    // ��ȡͼ�ж��������
    int numVertices() const {
        return adjList.size();
    }

    // ��ȡ���� u ���ڽӱ�ʼ������
    VertexList::const_iterator begin(int u) const {
        return adjList[u].begin();
    }

    // ��ȡ���� u ���ڽӱ����������
    VertexList::const_iterator end(int u) const {
        return adjList[u].end();
    }

private:
    AdjacencyList adjList; // �ڽӱ�
};

// FibonacciHeapNode �ඨ��
class FibonacciHeapNode {
public:
    int val; // �ڵ��ֵ
    int degree; // �ӽڵ������
    FibonacciHeapNode* child; // ָ���ӽڵ��ָ��
    FibonacciHeapNode* parent; // ָ�򸸽ڵ��ָ��
    bool marked; // ��ǽڵ��Ƿ�ɾ��
    FibonacciHeapNode* left; // ָ��ǰһ���ڵ��ָ��
    FibonacciHeapNode* right; // ָ���һ���ڵ��ָ��

    // ���캯��
    FibonacciHeapNode(int value) : val(value), degree(0), child(nullptr), parent(nullptr), marked(false) {
        left = this; // ��ʼʱ���ڵ����ָ��ָ���Լ�
        right = this; // ��ʼʱ���ڵ����ָ��ָ���Լ�
    }
};

// FibonacciHeap �ඨ��
class FibonacciHeap {
public:
    FibonacciHeapNode* min_node; // ָ����С�ڵ��ָ��
    int count; // �ڵ������

    // ���캯��
    FibonacciHeap() : min_node(nullptr), count(0) {}

    // �����½ڵ�ķ���
    FibonacciHeapNode* insert(int val) {
        FibonacciHeapNode* new_node = new FibonacciHeapNode(val); // �����½ڵ�
        if (min_node == nullptr) {
            min_node = new_node; // �����Ϊ�գ������½ڵ�Ϊ��С�ڵ�
        }
        else {
            // ���½ڵ���뵽���б���
            new_node->left = min_node;
            new_node->right = min_node->right;
            min_node->right->left = new_node;
            min_node->right = new_node;
            if (val < min_node->val) {
                min_node = new_node; // ������С�ڵ�
            }
        }
        count++; // ���ӽڵ����
        return new_node; // �����²���Ľڵ�
    }

    // ��ȡ��С�ڵ�ķ���
    FibonacciHeapNode* extract_min() {
        FibonacciHeapNode* z = min_node; // ������С�ڵ�
        if (z != nullptr) {
            // �����С�ڵ����ӽڵ㣬���ӽڵ���뵽���б���
            if (z->child != nullptr) {
                FibonacciHeapNode* child = z->child;
                do {
                    FibonacciHeapNode* next_child = child->right;
                    min_node->right->left = child;
                    child->right = min_node->right;
                    child->left = min_node;
                    min_node->right = child;
                    if (child->val < min_node->val) {
                        min_node = child; // ������С�ڵ�
                    }
                    child = next_child;
                } while (child != z->child);
            }
            // �Ӹ��б����Ƴ���С�ڵ�
            z->left->right = z->right;
            z->right->left = z->left;
            if (z == z->right) {
                min_node = nullptr; // ���ֻ��һ���ڵ㣬�����С�ڵ�
            }
            else {
                min_node = z->right; // ������С�ڵ�Ϊ�ұߵĽڵ�
                consolidate(); // �ϲ���ͬ�����ĸ��ڵ�
            }
            count--; // ���ٽڵ����
        }
        return z; // ���ر���ȡ����С�ڵ�
    }

    // �ϲ���ͬ�����ĸ��ڵ�ķ���
    void consolidate() {
        unordered_map<int, FibonacciHeapNode*> degree_table; // �洢��ͬ�����Ľڵ�
        vector<FibonacciHeapNode*> to_visit; // �����ʽڵ���б�
        FibonacciHeapNode* current = min_node;
        do {
            to_visit.push_back(current); // ����ǰ�ڵ���ӵ��������б�
            current = current->right; // ������һ���ڵ�
        } while (current != min_node);

        // �ϲ�ͬ�������Ľڵ�
        for (FibonacciHeapNode* w : to_visit) {
            FibonacciHeapNode* x = w; // ��ǰ�ڵ�
            int d = x->degree; // ��ǰ�ڵ�Ķ���
            while (degree_table.find(d) != degree_table.end()) { // ����ͬ�����Ľڵ�
                FibonacciHeapNode* y = degree_table[d]; // ȡ���ö����Ľڵ�
                if (x->val > y->val) {
                    swap(x, y); // ȷ�� x Ϊ��Сֵ
                }
                fib_heap_link(y, x); // �� y ���ӵ� x
                degree_table.erase(d); // �ӱ����Ƴ�����
                d++; // ���Ӷ���
            }
            degree_table[d] = x; // ����ǰ�ڵ���������
        }

        // ������С�ڵ�
        min_node = nullptr;
        for (const auto& pair : degree_table) {
            FibonacciHeapNode* w = pair.second;
            if (min_node == nullptr) {
                min_node = w; // �����С�ڵ�Ϊ�գ�����Ϊ��ǰ�ڵ�
            }
            else {
                // ����ǰ�ڵ���뵽���б�
                w->left->right = w->right;
                w->right->left = w->left;
                w->left = min_node;
                w->right = min_node->right;
                min_node->right->left = w;
                min_node->right = w;
                if (w->val < min_node->val) {
                    min_node = w; // ������С�ڵ�
                }
            }
        }
    }

    // ���������ڵ�ķ���
    void fib_heap_link(FibonacciHeapNode* y, FibonacciHeapNode* x) {
        // �Ӹ��б����Ƴ� y
        y->left->right = y->right;
        y->right->left = y->left;
        y->parent = x; // ���� x Ϊ y �ĸ��ڵ�
        if (x->child == nullptr) {
            x->child = y; // ��� x û���ӽڵ㣬���� y Ϊ�ӽڵ�
            y->right = y; // y ����ָ��ָ���Լ�
            y->left = y; // y ����ָ��ָ���Լ�
        }
        else {
            // �� y ���뵽 x ���ӽڵ��б���
            y->left = x->child;
            y->right = x->child->right;
            x->child->right->left = y;
            x->child->right = y;
        }
        x->degree++; // ���� x �Ķ���
        y->marked = false; // �� y ���Ϊδɾ��
    }

    // �����Ƿ�Ϊ��
    bool isEmpty() const {
        return min_node == nullptr; // ��� min_node Ϊ�գ����ǿյ�
    }

    // ��С�ڵ��ֵ�ķ���
    void decreaseKey(int v, int newKey) {
        // �˴��������ǿ���ͨ���ڵ�ֵ�ҵ��ڵ�
        FibonacciHeapNode* node = findNode(v); // ͨ��ֵ���ҽڵ�
        if (node != nullptr && newKey < node->val) {
            node->val = newKey; // ���½ڵ��ֵ
            FibonacciHeapNode* minNode = extract_min(); // �Ӷ�����ȡ��Сֵ
            if (node != minNode) {
                // ������º�Ľڵ㲻����С�ڵ㣬���²���
                insert(node->val); // ���²�����º�Ľڵ�
            }
        }
    }

    // ����ָ��ֵ�Ľڵ�
    FibonacciHeapNode* findNode(int val) {
        if (min_node == nullptr) return nullptr; // �����Ϊ�գ����� nullptr
        FibonacciHeapNode* current = min_node;
        do {
            if (current->val == val) return current; // ����ҵ�ֵ�����ؽڵ�
            current = current->right; // ������һ���ڵ�
        } while (current != min_node);
        return nullptr; // δ�ҵ������� nullptr
    }
};

// Prim �㷨ʵ����С������
void mst_prim(Graph const& graph, vector<int>& pred) {
    const int n = graph.numVertices(); // ��ȡ��������
    pred.assign(n, -1); // ��ʼ��ǰ������
    vector<int> key(n, numeric_limits<int>::max()); // ��ʼ����ֵ����
    key[0] = 0; // ���ļ�ֵΪ 0
    FibonacciHeap pq; // �������ȶ���
    vector<bool> inQueue(n, true); // ��¼�����Ƿ��ڶ�����

    // ��ʼ�����ȶ���
    for (int v = 0; v < n; v++) {
        pq.insert(key[v]); // �����ж�����뵽���ȶ���
    }

    while (!pq.isEmpty()) { // �����ȶ��в�Ϊ��
        FibonacciHeapNode* minNode = pq.extract_min(); // ��ȡ��Сֵ�ڵ�
        if (minNode == nullptr) {
            break; // ���û�нڵ��ˣ�����ѭ��
        }
        int u = minNode->val; // ��ȡ�ڵ�ֵ

        // ȷ�� u ����Ч��Χ��
        if (u < 0 || u >= n) {
            continue; // ������Ч�� u
        }

        inQueue[u] = false; // ��� u Ϊ�Ѵ���
        for (VertexList::const_iterator ci = graph.begin(u); ci != graph.end(u); ++ci) {
            int v = ci->first; // �ڽӶ���
            if (inQueue[v]) { // ��� v ���ڶ�����
                int w = ci->second; // ��Ȩ��
                if (w < key[v]) { // ����ҵ���С�ļ�ֵ
                    pred[v] = u; // ����ǰ��
                    key[v] = w; // ���¼�ֵ
                    pq.decreaseKey(v, w); // �������ȶ����еļ�ֵ
                }
            }
        }
    }
}

// ������
int main() {
    Graph graph(5); // ����һ������ 5 �������ͼ

    // ��ӱ�
    graph.addEdge(0, 1, 2);
    graph.addEdge(0, 3, 6);
    graph.addEdge(1, 2, 3);
    graph.addEdge(1, 3, 8);
    graph.addEdge(1, 4, 5);
    graph.addEdge(2, 4, 7);
    graph.addEdge(3, 4, 9);

    // �洢��С��������ǰ������
    vector<int> pred;

    // ���� Prim �㷨
    mst_prim(graph, pred);

    // ��ӡ���
    cout << "��С�������ıߣ�\n";
    for (int i = 1; i < pred.size(); i++) {
        if (pred[i] != -1) {
            cout << "��: (" << pred[i] << ", " << i << ")\n"; // ���ÿ����
        }
    }

    return 0; // �������
}
*/





