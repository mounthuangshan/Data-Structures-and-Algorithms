#include <iostream>
#include <vector>
#include <list>

using namespace std;

// 定义边的类型枚举
enum edgeType { Tree, Backward, Forward, Cross };

// 定义顶点颜色枚举
enum class vertexColor { white, gray, black };

// 定义边标签结构
struct EdgeLabel {
    int u, v;
    edgeType type;
    EdgeLabel(int _u, int _v, edgeType _type) : u(_u), v(_v), type(_type) {}
};

// 定义图的类型（使用邻接表表示）
class Graph {
public:
    vector<vector<int>> adj;
    Graph(int n) : adj(n) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    const vector<int>& operator[](int u) const {
        return adj[u];
    }

    auto begin(int u) const {
        return adj[u].begin();
    }

    auto end(int u) const {
        return adj[u].end();
    }
};

void dfs_visit(Graph const& graph, int u, vector<int>& d, vector<int>& f, vector<int>& pred, vector<vertexColor>& color, int& ctr, list<EdgeLabel>& labels);

// 访问顶点 u 并更新信息
void dfs_visit(Graph const& graph, int u, vector<int>& d, vector<int>& f, vector<int>& pred, vector<vertexColor>& color, int& ctr, list<EdgeLabel>& labels)
{
    color[u] = vertexColor::gray; // 将顶点 u 标记为灰色（正在访问）
    d[u] = ++ctr; // 记录发现时间

    for (int v : graph[u]) {
        if (color[v] == vertexColor::white) { // 如果顶点 v 仍然是白色（未访问）
            pred[v] = u; // 更新前驱
            dfs_visit(graph, v, d, f, pred, color, ctr, labels); // 递归访问 v
        }
    }

    color[u] = vertexColor::black; // 将顶点 u 标记为黑色（完成访问）
    f[u] = ++ctr; // 记录完成时间

    // 处理所有邻接顶点
    for (auto ci = graph.begin(u); ci != graph.end(u); ++ci) {
        int v = *ci;
        edgeType type = Cross; // 初始化边的类型为交叉边
        if (color[v] == vertexColor::white) {
            type = Tree; // 如果 v 是白色，则为树边
        }
        else if (color[v] == vertexColor::gray) {
            type = Backward; // 如果 v 是灰色，则为回边
        }
        else {
            if (d[u] < d[v]) {
                type = Forward; // 如果 d[u] < d[v]，则为前向边
            }
        }
        labels.push_back(EdgeLabel(u, v, type)); // 添加边标签
    }
}

// 从顶点 s 开始执行深度优先搜索
void dfs_search(Graph const& graph, int n, vector<int>& d, vector<int>& f, vector<int>& pred, vector<vertexColor>& color, list<EdgeLabel>& labels) {
    int ctr = 0; // 发现和完成时间的计数器
    for (int u = 0; u < n; u++) {
        if (color[u] == vertexColor::white) { // 如果顶点 u 是白色
            dfs_visit(graph, u, d, f, pred, color, ctr, labels); // 开始访问
        }
    }
}

int main() {
    // 创建一个包含 5 个顶点的图
    Graph graph(5);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(1, 4);
    graph.addEdge(2, 4);

    int n = 5; // 顶点数量
    vector<int> d(n, 0); // 发现时间
    vector<int> f(n, 0); // 完成时间
    vector<int> pred(n, -1); // 前驱
    vector<vertexColor> color(n, vertexColor::white); // 顶点颜色
    list<EdgeLabel> labels; // 边标签

    // 从顶点 0 开始深度优先搜索
    dfs_search(graph, n, d, f, pred, color, labels);

    // 输出发现和完成时间
    cout << "顶点\t发现时间\t完成时间\n";
    for (int i = 0; i < n; i++) {
        cout << i << "\t" << d[i] << "\t\t" << f[i] << "\n";
    }

    // 输出边标签
    cout << "\n边标签:\n";
    for (const auto& label : labels) {
        cout << "(" << label.u << ", " << label.v << ") : " << static_cast<int>(label.type) << "\n";
    }

    return 0;
}


/*
void dfs_visit(int u, int& time, vector<int>& d, vector<int>& f, vector<int>& color, vector<vector<int>>& G) {
    color[u] = 1;
    d[u] = ++time;
    for (int v : G[u]) {
        if (color[v] == 0) {
            dfs_visit(v, time, d, f, color, G);
        }
    }
    color[u] = 2;
    f[u] = ++time;
}
*/

/*
这两个 dfs_visit 函数在功能上都是执行深度优先搜索（DFS），但实现上有一些明显的差异。以下是对比和说明：

1. 参数类型和数量
上段代码中的 dfs_visit:

接受的参数有多个，包括图的结构、前驱节点数组、颜色数组、时间计数器、边标签列表等。
使用的颜色类型是枚举 vertexColor，用来表示未访问、正在访问和已完成。
新代码中的 dfs_visit:

参数较少，主要是顶点 u、时间 time、发现时间数组 d、完成时间数组 f、颜色数组 color 和图 G。
颜色使用整数（0, 1, 2）来表示状态，分别对应未访问、正在访问和已完成。
2. 颜色管理
上段代码:
使用 vertexColor 枚举来表示顶点的状态，赋值 gray 表示正在访问，赋值 black 表示已完成。
新代码:
使用简单的整数来表示状态，color[u] = 1 代表正在访问，color[u] = 2 代表已完成。
3. 边标签处理
上段代码:

在遍历邻接点时，记录边的类型（树边、回边、前向边、交叉边），并将这些信息存储在边标签列表中。
新代码:

没有处理边标签，专注于顶点的发现和完成时间。
4. 递归调用
上段代码:

在递归调用 dfs_visit 之前更新前驱节点数组 pred。
新代码:

没有更新前驱节点的信息，这可能影响对DFS树的重建或后续操作。
5. 代码风格
上段代码:
使用更丰富的结构体和类型定义，增加了代码的可读性和维护性。
新代码:
相对简单直接，适合快速实现DFS，但可能在后续分析中缺少某些信息。
总结
这两个函数的主要目的是相同的，都是实现深度优先搜索。上段代码提供了更多的功能和信息处理（如前驱记录和边标签），而新代码则相对简单，适用于基本的DFS实现。如果你的目标是构建DFS树或分析图的结构，上段代码可能更适合；如果只需要基本的DFS遍历，新代码足够用。
*/






