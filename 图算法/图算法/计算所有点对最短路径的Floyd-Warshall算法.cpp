#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <utility> // for std::pair

using namespace std;

class Graph {
public:
	using VertexList = vector<pair<int, int>>; // pair<vertex, weight>
	vector<VertexList> adjacencyList;

	Graph(int n) : adjacencyList(n) {}

	void addEdge(int u, int v, int weight) {
		adjacencyList[u].emplace_back(v, weight);
	}

	int numVertices() const {
		return adjacencyList.size();
	}

	VertexList::const_iterator begin(int u) const {
		return adjacencyList[u].begin();
	}

	VertexList::const_iterator end(int u) const {
		return adjacencyList[u].end();
	}
};

void allPairsShortest(Graph const& graph, vector<vector<int>>& dist, vector<vector<int>>& pred) {
	int n = graph.numVertices();
	//���Խ����ϵ�dist[][]����Ϊ0�����û�бߵĻ�����ΪINFINITY
	//dist[u][v]��ֵ���Ǳߣ�u��v����Ȩʹ��ͬ���İ취����ʼ��pred����
	for (int u = 0; u < n; u++) {
		dist[u].assign(n, numeric_limits<int>::max());
		pred[u].assign(n, -1); // Change this to -1 for proper initialization
		dist[u][u] = 0;
		for (auto ci = graph.begin(u); ci != graph.end(u); ++ci) {
			int v = ci->first;
			dist[u][v] = ci->second;
			pred[u][v] = u;
		}
	}	
	for (int k = 0; k < n; k++) {
		for (int i = 0; i < n; i++) {
			if (dist[i][k] == numeric_limits<int>::max()){
				continue;
			}
			//����ܹ��ҵ�һ�����پ����·��������dist[][]
	        //ʹ�ó����ͱ������
			for (int j = 0; j < n; j++) {
				long newLen = static_cast<long>(dist[i][k]) + dist[k][j];
				if (newLen < dist[i][j]) {
					dist[i][j] = newLen;
					pred[i][j] = pred[k][j];
				}
			}
		}
	}
}

/*
Floyd - Warshall�����dist[i][j]������������Ȩͼ�д�vi��vj�����·����
������������������ܹ������pred�������õ���������֮���ʵ�����·����
��������򵥵ĺ����õ���һ����vs��vt��ʵ�ʵ����·����Ҳ���и��ࣩ�����
��������pred�����е����������������·����
*/

//����֪��pred[][]���������·������

//�����s��t�����·����s��t��������Ч�����������������s��t����ͨ����ô���ؿ�·��

void constructShortestPath(int s, int t, vector<vector<int>> const& pred, list<int>& path) {
	path.clear();
	if (t < 0 || t >= (int)pred.size() || s < 0 || s >= (int)pred.size()) {
		return;
	}
	//������չ·����ֱ�����Ǵﵽԭ��s���ߵõ���ʾû��·����-1
	path.push_front(t);
	while (t != s) {
		t = pred[s][t];
		if (t == -1) {
			path.clear();
			return;
		}
		path.push_front(t);
	}
}

int main() {
	Graph graph(5);
	graph.addEdge(0, 1, 3);
	graph.addEdge(0, 2, 5);
	graph.addEdge(1, 2, 1);
	graph.addEdge(1, 3, 7);
	graph.addEdge(2, 3, 2);
	graph.addEdge(3, 4, 1);

	vector<vector<int>> dist(graph.numVertices(), vector<int>(graph.numVertices()));
	vector<vector<int>> pred(graph.numVertices(), vector<int>(graph.numVertices()));

	allPairsShortest(graph, dist, pred);

	int s = 0; // Start vertex
	int t = 4; // Target vertex

	list<int> path;
	constructShortestPath(s, t, pred, path);

	if (!path.empty()) {
		cout << "Shortest path from " << s << " to " << t << ": ";
		for (int vertex : path) {
			cout << vertex << " ";
		}
		cout << endl;
	}
	else {
		cout << "No path from " << s << " to " << t << "." << endl;
	}

	return 0;
}
