#include <iostream>
#include <limits>

using namespace std;

// 从源顶点s找到最短路径的函数
void singleSourceShortest(int n, int** const weight, int s, int* dist, int* pred) {
    // 初始化dist[]和pred[]数组，从顶点s开始，将dist[]设置为0，所有的顶点都未访问
    bool* visited = new bool[n];
    for (int v = 0; v < n; v++) {
        dist[v] = numeric_limits<int>::max();
        pred[v] = -1;
        visited[v] = false;
    }
    dist[s] = 0;

    // 寻找s到所有未访问顶点的最短距离
    while (true) {
        int u = -1;
        int sd = numeric_limits<int>::max();
        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] < sd) {
                sd = dist[i];
                u = i;
            }
        }
        if (u == -1) {
            break;
        }

        // 标记顶点u为已访问
        visited[u] = true;

        // 遍历u的所有邻接节点
        for (int v = 0; v < n; v++) {
            int w = weight[u][v]; // 权重
            if (w == 0) { // 如果没有边，跳过
                continue;
            }
            long newLen = dist[u] + w; // 新路径长度
            if (newLen < dist[v]) {
                dist[v] = newLen;
                pred[v] = u;
            }
        }
    }
    delete[] visited; // 释放内存
}

// 主函数
int main() {
    int n = 5; // 顶点数量
    // 创建一个权重矩阵（邻接矩阵）
    int** weight = new int* [n];
    for (int i = 0; i < n; i++) {
        weight[i] = new int[n];
        for (int j = 0; j < n; j++) {
            weight[i][j] = (i == j) ? 0 : numeric_limits<int>::max(); // 自身到自身的距离为0，其余为无穷大
        }
    }

    // 添加边及其权重
    weight[0][1] = 10;
    weight[0][2] = 3;
    weight[1][2] = 1;
    weight[1][3] = 2;
    weight[2][1] = 4;
    weight[2][3] = 8;
    weight[2][4] = 2;
    weight[3][4] = 7;
    weight[4][3] = 9;

    int* dist = new int[n];
    int* pred = new int[n];
    int source = 0; // 起始顶点

    singleSourceShortest(n, weight, source, dist, pred);

    // 打印结果
    cout << "从源顶点 " << source << " 到各顶点的距离:\n";
    for (int i = 0; i < n; ++i) {
        cout << "顶点 " << i << ": " << dist[i] << ", 前驱: " << pred[i] << endl;
    }

    // 释放内存
    delete[] dist;
    delete[] pred;
    for (int i = 0; i < n; i++) {
        delete[] weight[i];
    }
    delete[] weight;

    return 0;
}
