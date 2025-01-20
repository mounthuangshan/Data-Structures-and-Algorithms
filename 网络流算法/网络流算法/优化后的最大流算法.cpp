#include <vector>
#include <climits>
#include <queue>
#include <iostream>

// �����ǰ������
class FlowNetwork {
protected:
    int numVertices;
public:
    FlowNetwork(int n, int s, int t) : numVertices(n) {}
};

// �ߵ���Ϣ�ṹ��
struct EdgeInfo {
    int start;
    int end;
    int capacity;
    EdgeInfo(int s, int e, int c) : start(s), end(e), capacity(c) {}
};

class Optimized : public FlowNetwork {
private:
    std::vector<std::vector<int>> capacity;  // ��������
    std::vector<std::vector<int>> flow;      // ��������
    std::vector<int> previous;               // ǰ���ڵ���Ϣ
    std::vector<int> visited;                // ����״̬
    const int QUEUE_SIZE;                    // ���е�����С
    std::vector<int> queue;                  // ѭ������

protected:
    int processPath(int source, int sink) {
        // ����������ӵ�����
        int increment = INT_MAX;
        int v = sink;

        while (previous[v] != -1) {
            int unit = capacity[previous[v]][v] - flow[previous[v]][v];
            if (unit < increment) {
                increment = unit;
            }
            v = previous[v];
        }

        // ��������
        v = sink;
        while (previous[v] != -1) {
            flow[previous[v]][v] += increment;    // �����
            flow[v][previous[v]] -= increment;    // �����
            v = previous[v];
        }

        return increment;
    }

public:
    // ���캯��
    Optimized(int n, int s, int t, const std::vector<EdgeInfo>& edges)
        : FlowNetwork(n, s, t),
        QUEUE_SIZE(n),
        capacity(n, std::vector<int>(n, 0)),
        flow(n, std::vector<int>(n, 0)),
        previous(n, -1),
        visited(n, 0),
        queue(n, 0) {

        // ��ʼ������Ϊ0����������
        for (const auto& ei : edges) {
            capacity[ei.start][ei.end] = ei.capacity;
        }
    }

    int compute(int source, int sink) {
        int maxFlow = 0;
        while (search(source, sink)) {
            maxFlow += processPath(source, sink);
        }
        return maxFlow;
    }

    bool search(int source, int sink) {
        // �������״̬��0-δ���ʣ�1-�ڶ����У�2-�ѷ���
        std::fill(visited.begin(), visited.end(), 0);

        // ����ѭ������
        queue[0] = source;
        int head = 0, tail = 1;
        previous[source] = -1;
        visited[source] = 1;

        while (head != tail) {
            int u = queue[head];
            head = (head + 1) % QUEUE_SIZE;
            visited[u] = 2;

            // ������ʣ�����������ڽڵ�������
            for (int v = 0; v < numVertices; v++) {
                if (visited[v] == 0 && capacity[u][v] > flow[u][v]) {
                    queue[tail] = v;
                    tail = (tail + 1) % QUEUE_SIZE;
                    visited[v] = 1;  // ���Ϊ�ڶ�����
                    previous[v] = u;
                }
            }
        }

        return visited[sink] != 0;  // �Ƿ�����˻�㣿
    }
};

int main() {
    // ʾ���÷�
    const int n = 6;  // ��������
    const int source = 0;
    const int sink = 5;

    // ����ʾ���߼�������
    std::vector<EdgeInfo> edges = {
        EdgeInfo(0, 1, 10),
        EdgeInfo(0, 2, 8),
        EdgeInfo(1, 2, 5),
        EdgeInfo(1, 3, 5),
        EdgeInfo(2, 4, 10),
        EdgeInfo(3, 4, 7),
        EdgeInfo(3, 5, 8),
        EdgeInfo(4, 5, 10)
    };

    // �����Ż���������
    Optimized network(n, source, sink, edges);

    // ���������
    int maxFlow = network.compute(source, sink);

    std::cout << "��Դ�� " << source
        << " ����� " << sink << " �������: "
        << maxFlow << std::endl;

    return 0;
}