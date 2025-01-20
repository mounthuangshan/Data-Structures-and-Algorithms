#include <vector>
#include <stack>
#include <memory>
#include <iostream>

// ����Ϣ�ṹ��
struct EdgeInfo {
    int start;    // ���
    int end;      // �յ�
    int capacity; // ����
    int flow;     // ��ǰ����

    EdgeInfo(int s, int e, int c, int f = 0)
        : start(s), end(e), capacity(c), flow(f) {
    }
};

// ������Ϣ�ṹ��
struct VertexInfo {
    int previous;  // ǰ���ڵ�
    bool forward;  // �Ƿ�Ϊǰ���

    VertexInfo(int prev = -1, bool fwd = true)
        : previous(prev), forward(fwd) {
    }
};

// ����߽ṹ��
class VertexStructure {
private:
    std::vector<EdgeInfo> forwardEdges;   // ǰ����б�
    std::vector<EdgeInfo> backwardEdges;  // ������б�

public:
    // ��ȡǰ��ߵ�����
    auto forward() { return forwardEdges.begin(); }
    auto forward_end() { return forwardEdges.end(); }

    // ��ȡ����ߵ�����
    auto backward() { return backwardEdges.begin(); }
    auto backward_end() { return backwardEdges.end(); }

    // ��ӱ�
    void addForwardEdge(const EdgeInfo& edge) {
        forwardEdges.push_back(edge);
    }

    void addBackwardEdge(const EdgeInfo& edge) {
        backwardEdges.push_back(edge);
    }
};

class PathFinder {
private:
    std::vector<VertexStructure>& network;  // ����ṹ
    int sourceIndex;  // Դ������
    int sinkIndex;   // �������
    const bool FORWARD = true;    // ǰ��߱�־
    const bool BACKWARD = false;  // ����߱�־

public:
    PathFinder(std::vector<VertexStructure>& net, int source, int sink)
        : network(net), sourceIndex(source), sinkIndex(sink) {
    }

    bool findAugmentingPath(std::vector<std::unique_ptr<VertexInfo>>& vertices) {
        // ��ʼ��Դ����Ϣ
        vertices[sourceIndex] = std::make_unique<VertexInfo>(-1);

        // ʹ��ջ�洢·��
        std::stack<int> path;
        path.push(sourceIndex);

        // ��ջ��Ϊ��ʱ��������
        while (!path.empty()) {
            int u = path.top();
            path.pop();

            // ����ǰ���
            for (auto it = network[u].forward(); it != network[u].forward_end(); ++it) {
                int v = it->end;

                // �������δ��������ʣ��������׼��������
                if (!vertices[v] && it->capacity > it->flow) {
                    vertices[v] = std::make_unique<VertexInfo>(u, FORWARD);
                    if (v == sinkIndex) {
                        return true;  // �ҵ�����·
                    }
                    path.push(v);
                }
            }

            // ��������
            for (auto it = network[u].backward(); it != network[u].backward_end(); ++it) {
                int v = it->start;

                // �������δ�����ҿ��Լ�����
                if (!vertices[v] && it->flow > 0) {
                    vertices[v] = std::make_unique<VertexInfo>(u, BACKWARD);
                    path.push(v);
                }
            }
        }

        return false;  // δ�ҵ�����·
    }
};

int main() {
    // ����ʾ������
    const int n = 6;  // ��������
    std::vector<VertexStructure> network(n);

    // ���ʾ����
    // ��ǰ����б���ӱ�
    network[0].addForwardEdge(EdgeInfo(0, 1, 10));
    network[0].addForwardEdge(EdgeInfo(0, 2, 8));
    network[1].addForwardEdge(EdgeInfo(1, 2, 5));
    network[1].addForwardEdge(EdgeInfo(1, 3, 5));
    network[2].addForwardEdge(EdgeInfo(2, 4, 10));
    network[3].addForwardEdge(EdgeInfo(3, 4, 7));
    network[3].addForwardEdge(EdgeInfo(3, 5, 8));
    network[4].addForwardEdge(EdgeInfo(4, 5, 10));

    // �������б���Ӷ�Ӧ�ķ����
    network[1].addBackwardEdge(EdgeInfo(1, 0, 10));
    network[2].addBackwardEdge(EdgeInfo(2, 0, 8));
    network[2].addBackwardEdge(EdgeInfo(2, 1, 5));
    network[3].addBackwardEdge(EdgeInfo(3, 1, 5));
    network[4].addBackwardEdge(EdgeInfo(4, 2, 10));
    network[4].addBackwardEdge(EdgeInfo(4, 3, 7));
    network[5].addBackwardEdge(EdgeInfo(5, 3, 8));
    network[5].addBackwardEdge(EdgeInfo(5, 4, 10));

    // ����������Ϣ����
    std::vector<std::unique_ptr<VertexInfo>> vertices(n);

    // ����·����������Դ��Ϊ0�����Ϊ5��
    PathFinder pathFinder(network, 0, 5);

    // ��������·��������
    if (pathFinder.findAugmentingPath(vertices)) {
        std::cout << "�ҵ�����·��" << std::endl;

        // ��ӡ����·��
        std::cout << "����·��: ";
        int v = 5;  // �ӻ�㿪ʼ
        while (v != -1) {
            std::cout << v;
            if (vertices[v] && vertices[v]->previous != -1) {
                std::cout << " <- ";
            }
            v = vertices[v] ? vertices[v]->previous : -1;
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "δ�ҵ�����·��" << std::endl;
    }

    return 0;
}