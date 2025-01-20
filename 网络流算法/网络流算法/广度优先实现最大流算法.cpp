#include <vector>
#include <list>
#include <memory>
#include <iostream>

// �ߵ���Ϣ�ṹ��
struct EdgeInfo {
    int start;  // ���
    int end;    // �յ�
    int capacity; // ����
    int flow;    // ��ǰ����

    EdgeInfo(int s, int e, int c, int f = 0)
        : start(s), end(e), capacity(c), flow(f) {
    }
};

// ������Ϣ�ṹ��
struct VertexInfo {
    int previous;  // ǰ���ڵ�
    bool forward;  // �Ƿ���ǰ���

    VertexInfo(int prev = -1, bool fwd = true)
        : previous(prev), forward(fwd) {
    }
};

// ����ı߽ṹ��
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

        // ʹ��˫������洢·��
        std::list<int> path;
        path.push_back(sourceIndex);

        // ��·����Ϊ��ʱ��������
        while (!path.empty()) {
            int u = path.front();
            path.pop_front();

            // ����ǰ���
            for (auto it = network[u].forward(); it != network[u].forward_end(); ++it) {
                int v = it->end;

                // �������δ��������ʣ��������׼��������
                if (!vertices[v] && it->capacity > it->flow) {
                    vertices[v] = std::make_unique<VertexInfo>(u, FORWARD);
                    if (v == sinkIndex) {
                        return true;  // �ҵ�����·
                    }
                    path.push_back(v);  // ����·��β��
                }
            }

            // ��������
            for (auto it = network[u].backward(); it != network[u].backward_end(); ++it) {
                int v = it->start;

                // �������δ�����ҿ��Լ�����
                if (!vertices[v] && it->flow > 0) {
                    vertices[v] = std::make_unique<VertexInfo>(u, BACKWARD);
                    path.push_back(v);  // ����·��β��
                }
            }
        }

        return false;  // δ�ҵ�����·
    }
};

int main() {
    // ����ʾ������
    const int n = 4;  // ��������
    std::vector<VertexStructure> network(n);

    // ��ӱ�
    // ʾ�������һЩ�ߵ�������
    network[0].addForwardEdge(EdgeInfo(0, 1, 10));
    network[0].addForwardEdge(EdgeInfo(0, 2, 8));
    network[1].addForwardEdge(EdgeInfo(1, 2, 5));
    network[1].addForwardEdge(EdgeInfo(1, 3, 7));
    network[2].addForwardEdge(EdgeInfo(2, 3, 10));

    // ����������Ϣ����
    std::vector<std::unique_ptr<VertexInfo>> vertices(n);

    // ����·��������
    PathFinder pathFinder(network, 0, 3);  // Դ��Ϊ0�����Ϊ3

    // ��������·
    bool found = pathFinder.findAugmentingPath(vertices);

    // ������
    if (found) {
        std::cout << "�ҵ�����·��" << std::endl;

        // ��ӡ·��
        int v = 3;  // �ӻ�㿪ʼ
        std::cout << "·��: ";
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