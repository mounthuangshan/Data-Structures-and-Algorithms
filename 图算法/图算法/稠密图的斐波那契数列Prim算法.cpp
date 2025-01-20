#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <utility>
#include <unordered_map>
#include <cmath>

using namespace std;

// 邻接表的定义
using VertexList = unordered_map<int, int>; // int: 邻接顶点, int: 边权重
using AdjacencyList = vector<VertexList>;

// Graph 类定义
class Graph {
public:
    Graph(int vertices) : adjList(vertices) {}

    void addEdge(int u, int v, int weight) {
        adjList[u][v] = weight;
        adjList[v][u] = weight; // 无向图
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

// ------------------------- 斐波那契堆的实现 -------------------------
// 斐波那契堆节点定义
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

// 斐波那契堆类定义
class FibonacciHeap {
public:
    FibonacciHeap() : minNode(nullptr), nodeCount(0) {}

    // 插入新节点，时间复杂度O(1)
    FibNode* insert(int key, int vertex) {
        FibNode* node = new FibNode(key, vertex);
        // 将新节点加入根链表
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

    // 返回当前最小节点
    FibNode* minimum() const {
        return minNode;
    }

    // 判断堆是否为空
    bool isEmpty() const {
        return minNode == nullptr;
    }

    // 合并两个斐波那契堆
    void merge(FibonacciHeap& other) {
        if (other.minNode == nullptr)
            return;

        if (minNode == nullptr) {
            minNode = other.minNode;
            nodeCount = other.nodeCount;
        }
        else {
            // 将两个根链表合并
            concatenateRootList(minNode, other.minNode);
            if (other.minNode->key < minNode->key)
                minNode = other.minNode;
            nodeCount += other.nodeCount;
        }

        other.minNode = nullptr;
        other.nodeCount = 0;
    }

    // 从堆中移除最小节点并返回其对应顶点
    int extractMin() {
        FibNode* z = minNode;
        if (z != nullptr) {
            // 将z的孩子全部加入根链表
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

            // 将z从根链表移除
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
        return -1; // 空堆
    }

    // 减小指定节点的键值，时间复杂度 O(1) 摊还
    void decreaseKey(FibNode* x, int k) {
        if (k > x->key) {
            return; // 新键值不得大于当前键值
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

    // 将节点node插入到a和b所在的双向链表中（在a和b相邻位置）
    static void insertNodeToRootList(FibNode* node, FibNode* root) {
        node->left = root;
        node->right = root->right;
        root->right->left = node;
        root->right = node;
    }

    // 从根链表移除节点node
    static void removeNodeFromRootList(FibNode* node) {
        node->left->right = node->right;
        node->right->left = node->left;
    }

    // 将两个双向循环链表合并
    static void concatenateRootList(FibNode* a, FibNode* b) {
        if (a == nullptr || b == nullptr) return;
        FibNode* aRight = a->right;
        FibNode* bLeft = b->left;

        a->right = b;
        b->left = a;
        aRight->left = bLeft;
        bLeft->right = aRight;
    }

    // 合并根列表中的树，保证同度数的树不重复
    void consolidate() {
        int D = (int)floor(log2(nodeCount)) + 1;
        vector<FibNode*> A(D, nullptr);

        vector<FibNode*> rootList;
        {
            // 将当前根列表中所有节点收集到rootList中
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

    // 将节点y链接到x上，成为x的子节点
    void link(FibNode* y, FibNode* x) {
        // 从根链表中移除y
        removeNodeFromRootList(y);
        // 将y作为x的子节点
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

    // 从父节点y分离x，并将x加入根链表
    void cut(FibNode* x, FibNode* y) {
        // 将x从y的子链表中移除
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

        // x加入根链表
        x->parent = nullptr;
        x->left = x;
        x->right = x;
        x->mark = false;
        insertNodeToRootList(x, minNode);
    }

    // 级联剪切
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

// ------------------------- Prim 算法实现 -------------------------
// 使用斐波那契堆版本的优先队列实现的 Prim
void mst_prim(const Graph& graph, vector<int>& pred) {
    int n = graph.numVertices();
    pred.assign(n, -1);
    vector<int> key(n, numeric_limits<int>::max());
    key[0] = 0;

    FibonacciHeap fh;
    vector<FibNode*> nodePtr(n, nullptr);

    // 初始化优先队列
    for (int v = 0; v < n; v++) {
        nodePtr[v] = fh.insert(key[v], v);
    }

    // 标记节点是否还在优先队列中
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

// ------------------------- 测试代码 -------------------------
int main() {
    // 创建图的实例
    Graph graph(5); // 例如 5 个顶点

    // 添加边
    graph.addEdge(0, 1, 2);
    graph.addEdge(0, 3, 6);
    graph.addEdge(1, 2, 3);
    graph.addEdge(1, 3, 8);
    graph.addEdge(1, 4, 5);
    graph.addEdge(2, 4, 7);
    graph.addEdge(3, 4, 9);

    // 存储最小生成树的前驱数组
    vector<int> pred;

    // 调用 Prim 算法（斐波那契堆版本）
    mst_prim(graph, pred);

    // 打印结果
    cout << "最小生成树的边：\n";
    for (int i = 1; i < (int)pred.size(); i++) {
        if (pred[i] != -1) {
            cout << "边: (" << pred[i] << ", " << i << ")\n";
        }
    }

    return 0;
}

//旧的代码
/*
#include <iostream>
#include <vector>
#include <limits>
#include <unordered_map>

using namespace std;

// 邻接表的定义
using VertexList = unordered_map<int, int>; // int: 邻接顶点, int: 边权重
using AdjacencyList = vector<VertexList>; // 定义图的邻接表

// Graph 类定义
class Graph {
public:
    // 构造函数，初始化邻接表
    Graph(int vertices) : adjList(vertices) {}

    // 添加边的方法
    void addEdge(int u, int v, int weight) {
        adjList[u][v] = weight; // 从 u 到 v 的边
        adjList[v][u] = weight; // 从 v 到 u 的边（无向图）
    }

    // 获取图中顶点的数量
    int numVertices() const {
        return adjList.size();
    }

    // 获取顶点 u 的邻接表开始迭代器
    VertexList::const_iterator begin(int u) const {
        return adjList[u].begin();
    }

    // 获取顶点 u 的邻接表结束迭代器
    VertexList::const_iterator end(int u) const {
        return adjList[u].end();
    }

private:
    AdjacencyList adjList; // 邻接表
};

// FibonacciHeapNode 类定义
class FibonacciHeapNode {
public:
    int val; // 节点的值
    int degree; // 子节点的数量
    FibonacciHeapNode* child; // 指向子节点的指针
    FibonacciHeapNode* parent; // 指向父节点的指针
    bool marked; // 标记节点是否被删除
    FibonacciHeapNode* left; // 指向前一个节点的指针
    FibonacciHeapNode* right; // 指向后一个节点的指针

    // 构造函数
    FibonacciHeapNode(int value) : val(value), degree(0), child(nullptr), parent(nullptr), marked(false) {
        left = this; // 初始时，节点的左指针指向自己
        right = this; // 初始时，节点的右指针指向自己
    }
};

// FibonacciHeap 类定义
class FibonacciHeap {
public:
    FibonacciHeapNode* min_node; // 指向最小节点的指针
    int count; // 节点的数量

    // 构造函数
    FibonacciHeap() : min_node(nullptr), count(0) {}

    // 插入新节点的方法
    FibonacciHeapNode* insert(int val) {
        FibonacciHeapNode* new_node = new FibonacciHeapNode(val); // 创建新节点
        if (min_node == nullptr) {
            min_node = new_node; // 如果堆为空，设置新节点为最小节点
        }
        else {
            // 将新节点插入到根列表中
            new_node->left = min_node;
            new_node->right = min_node->right;
            min_node->right->left = new_node;
            min_node->right = new_node;
            if (val < min_node->val) {
                min_node = new_node; // 更新最小节点
            }
        }
        count++; // 增加节点计数
        return new_node; // 返回新插入的节点
    }

    // 提取最小节点的方法
    FibonacciHeapNode* extract_min() {
        FibonacciHeapNode* z = min_node; // 保存最小节点
        if (z != nullptr) {
            // 如果最小节点有子节点，将子节点插入到根列表中
            if (z->child != nullptr) {
                FibonacciHeapNode* child = z->child;
                do {
                    FibonacciHeapNode* next_child = child->right;
                    min_node->right->left = child;
                    child->right = min_node->right;
                    child->left = min_node;
                    min_node->right = child;
                    if (child->val < min_node->val) {
                        min_node = child; // 更新最小节点
                    }
                    child = next_child;
                } while (child != z->child);
            }
            // 从根列表中移除最小节点
            z->left->right = z->right;
            z->right->left = z->left;
            if (z == z->right) {
                min_node = nullptr; // 如果只有一个节点，清空最小节点
            }
            else {
                min_node = z->right; // 更新最小节点为右边的节点
                consolidate(); // 合并相同度数的根节点
            }
            count--; // 减少节点计数
        }
        return z; // 返回被提取的最小节点
    }

    // 合并相同度数的根节点的方法
    void consolidate() {
        unordered_map<int, FibonacciHeapNode*> degree_table; // 存储不同度数的节点
        vector<FibonacciHeapNode*> to_visit; // 待访问节点的列表
        FibonacciHeapNode* current = min_node;
        do {
            to_visit.push_back(current); // 将当前节点添加到待访问列表
            current = current->right; // 访问下一个节点
        } while (current != min_node);

        // 合并同样度数的节点
        for (FibonacciHeapNode* w : to_visit) {
            FibonacciHeapNode* x = w; // 当前节点
            int d = x->degree; // 当前节点的度数
            while (degree_table.find(d) != degree_table.end()) { // 查找同度数的节点
                FibonacciHeapNode* y = degree_table[d]; // 取出该度数的节点
                if (x->val > y->val) {
                    swap(x, y); // 确保 x 为最小值
                }
                fib_heap_link(y, x); // 将 y 链接到 x
                degree_table.erase(d); // 从表中移除度数
                d++; // 增加度数
            }
            degree_table[d] = x; // 将当前节点加入度数表
        }

        // 更新最小节点
        min_node = nullptr;
        for (const auto& pair : degree_table) {
            FibonacciHeapNode* w = pair.second;
            if (min_node == nullptr) {
                min_node = w; // 如果最小节点为空，设置为当前节点
            }
            else {
                // 将当前节点插入到根列表
                w->left->right = w->right;
                w->right->left = w->left;
                w->left = min_node;
                w->right = min_node->right;
                min_node->right->left = w;
                min_node->right = w;
                if (w->val < min_node->val) {
                    min_node = w; // 更新最小节点
                }
            }
        }
    }

    // 连接两个节点的方法
    void fib_heap_link(FibonacciHeapNode* y, FibonacciHeapNode* x) {
        // 从根列表中移除 y
        y->left->right = y->right;
        y->right->left = y->left;
        y->parent = x; // 设置 x 为 y 的父节点
        if (x->child == nullptr) {
            x->child = y; // 如果 x 没有子节点，设置 y 为子节点
            y->right = y; // y 的右指针指向自己
            y->left = y; // y 的左指针指向自己
        }
        else {
            // 将 y 插入到 x 的子节点列表中
            y->left = x->child;
            y->right = x->child->right;
            x->child->right->left = y;
            x->child->right = y;
        }
        x->degree++; // 增加 x 的度数
        y->marked = false; // 将 y 标记为未删除
    }

    // 检查堆是否为空
    bool isEmpty() const {
        return min_node == nullptr; // 如果 min_node 为空，堆是空的
    }

    // 减小节点键值的方法
    void decreaseKey(int v, int newKey) {
        // 此处假设我们可以通过节点值找到节点
        FibonacciHeapNode* node = findNode(v); // 通过值查找节点
        if (node != nullptr && newKey < node->val) {
            node->val = newKey; // 更新节点的值
            FibonacciHeapNode* minNode = extract_min(); // 从堆中提取最小值
            if (node != minNode) {
                // 如果更新后的节点不是最小节点，重新插入
                insert(node->val); // 重新插入更新后的节点
            }
        }
    }

    // 查找指定值的节点
    FibonacciHeapNode* findNode(int val) {
        if (min_node == nullptr) return nullptr; // 如果堆为空，返回 nullptr
        FibonacciHeapNode* current = min_node;
        do {
            if (current->val == val) return current; // 如果找到值，返回节点
            current = current->right; // 访问下一个节点
        } while (current != min_node);
        return nullptr; // 未找到，返回 nullptr
    }
};

// Prim 算法实现最小生成树
void mst_prim(Graph const& graph, vector<int>& pred) {
    const int n = graph.numVertices(); // 获取顶点数量
    pred.assign(n, -1); // 初始化前驱数组
    vector<int> key(n, numeric_limits<int>::max()); // 初始化键值数组
    key[0] = 0; // 起点的键值为 0
    FibonacciHeap pq; // 创建优先队列
    vector<bool> inQueue(n, true); // 记录顶点是否在队列中

    // 初始化优先队列
    for (int v = 0; v < n; v++) {
        pq.insert(key[v]); // 将所有顶点插入到优先队列
    }

    while (!pq.isEmpty()) { // 当优先队列不为空
        FibonacciHeapNode* minNode = pq.extract_min(); // 提取最小值节点
        if (minNode == nullptr) {
            break; // 如果没有节点了，跳出循环
        }
        int u = minNode->val; // 获取节点值

        // 确保 u 在有效范围内
        if (u < 0 || u >= n) {
            continue; // 忽略无效的 u
        }

        inQueue[u] = false; // 标记 u 为已处理
        for (VertexList::const_iterator ci = graph.begin(u); ci != graph.end(u); ++ci) {
            int v = ci->first; // 邻接顶点
            if (inQueue[v]) { // 如果 v 还在队列中
                int w = ci->second; // 边权重
                if (w < key[v]) { // 如果找到更小的键值
                    pred[v] = u; // 更新前驱
                    key[v] = w; // 更新键值
                    pq.decreaseKey(v, w); // 更新优先队列中的键值
                }
            }
        }
    }
}

// 主函数
int main() {
    Graph graph(5); // 创建一个包含 5 个顶点的图

    // 添加边
    graph.addEdge(0, 1, 2);
    graph.addEdge(0, 3, 6);
    graph.addEdge(1, 2, 3);
    graph.addEdge(1, 3, 8);
    graph.addEdge(1, 4, 5);
    graph.addEdge(2, 4, 7);
    graph.addEdge(3, 4, 9);

    // 存储最小生成树的前驱数组
    vector<int> pred;

    // 调用 Prim 算法
    mst_prim(graph, pred);

    // 打印结果
    cout << "最小生成树的边：\n";
    for (int i = 1; i < pred.size(); i++) {
        if (pred[i] != -1) {
            cout << "边: (" << pred[i] << ", " << i << ")\n"; // 输出每条边
        }
    }

    return 0; // 程序结束
}
*/





