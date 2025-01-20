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
	//将对角线上的dist[][]设置为0，如果没有边的话设置为INFINITY
	//dist[u][v]的值就是边（u，v）的权使用同样的办法来初始化pred数组
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
			//如果能够找到一条减少距离的路径，更新dist[][]
	        //使用长整型避免溢出
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
Floyd - Warshall计算出dist[i][j]，即在有向有权图中从vi到vj的最短路径，
按照这个方法，我们能够计算出pred矩阵，来得到两个顶点之间的实际最短路径。
下面这个简单的函数得到了一条从vs到vt的实际的最短路径（也许有更多）。这个
函数根据pred矩阵中的数据来构建出最短路径。
*/

//从已知的pred[][]构建出最短路径代码

//输出从s到t的最短路劲，s和t必须是有效顶点描述符，如果在s和t不连通，那么返回空路径

void constructShortestPath(int s, int t, vector<vector<int>> const& pred, list<int>& path) {
	path.clear();
	if (t < 0 || t >= (int)pred.size() || s < 0 || s >= (int)pred.size()) {
		return;
	}
	//不断扩展路径，直到我们达到原点s或者得到表示没有路径的-1
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
