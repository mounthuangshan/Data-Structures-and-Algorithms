#include <iostream>
#include <limits>

using namespace std;

// ��Դ����s�ҵ����·���ĺ���
void singleSourceShortest(int n, int** const weight, int s, int* dist, int* pred) {
    // ��ʼ��dist[]��pred[]���飬�Ӷ���s��ʼ����dist[]����Ϊ0�����еĶ��㶼δ����
    bool* visited = new bool[n];
    for (int v = 0; v < n; v++) {
        dist[v] = numeric_limits<int>::max();
        pred[v] = -1;
        visited[v] = false;
    }
    dist[s] = 0;

    // Ѱ��s������δ���ʶ������̾���
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

        // ��Ƕ���uΪ�ѷ���
        visited[u] = true;

        // ����u�������ڽӽڵ�
        for (int v = 0; v < n; v++) {
            int w = weight[u][v]; // Ȩ��
            if (w == 0) { // ���û�бߣ�����
                continue;
            }
            long newLen = dist[u] + w; // ��·������
            if (newLen < dist[v]) {
                dist[v] = newLen;
                pred[v] = u;
            }
        }
    }
    delete[] visited; // �ͷ��ڴ�
}

// ������
int main() {
    int n = 5; // ��������
    // ����һ��Ȩ�ؾ����ڽӾ���
    int** weight = new int* [n];
    for (int i = 0; i < n; i++) {
        weight[i] = new int[n];
        for (int j = 0; j < n; j++) {
            weight[i][j] = (i == j) ? 0 : numeric_limits<int>::max(); // ��������ľ���Ϊ0������Ϊ�����
        }
    }

    // ��ӱ߼���Ȩ��
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
    int source = 0; // ��ʼ����

    singleSourceShortest(n, weight, source, dist, pred);

    // ��ӡ���
    cout << "��Դ���� " << source << " ��������ľ���:\n";
    for (int i = 0; i < n; ++i) {
        cout << "���� " << i << ": " << dist[i] << ", ǰ��: " << pred[i] << endl;
    }

    // �ͷ��ڴ�
    delete[] dist;
    delete[] pred;
    for (int i = 0; i < n; i++) {
        delete[] weight[i];
    }
    delete[] weight;

    return 0;
}
