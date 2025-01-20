#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

enum vertexColor { White, Gray, Black };

// Dummy Graph class definition for demonstration
class Graph {
public:
    Graph(int n) : adjList(n) {}

    void addEdge(int u, int v) {
        adjList[u].push_back(make_pair(v, 1)); // Example with weight 1
        adjList[v].push_back(make_pair(u, 1)); // Assuming undirected graph
    }

    int numVertices() const { return adjList.size(); }

    typedef vector<pair<int, int>>::const_iterator Vertexlist;

    Vertexlist begin(int u) const { return adjList[u].begin(); }
    Vertexlist end(int u) const { return adjList[u].end(); }

private:
    vector<vector<pair<int, int>>> adjList;
};

void bfs_search(Graph const& graph, int s, vector<int>& dist, vector<int>& pred) {
    const int n = graph.numVertices();
    pred.assign(n, -1);
    dist.assign(n, numeric_limits<int>::max());
    vector<vertexColor> color(n, White);

    dist[s] = 0;
    color[s] = Gray;

    queue<int> q;
    q.push(s);

    while (!q.empty()) {
        int u = q.front();

        for (Graph::Vertexlist ci = graph.begin(u); ci != graph.end(u); ++ci) {
            int v = ci->first;
            if (color[v] == White) {
                dist[v] = dist[u] + 1;
                pred[v] = u;
                color[v] = Gray;
                q.push(v);
            }
        }

        q.pop();
        color[u] = Black;
    }
}

int main() {
    int vertices = 6;
    Graph graph(vertices);

    // Add edges to the graph
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    graph.addEdge(4, 5);

    vector<int> dist;
    vector<int> pred;

    int start_vertex = 0; // Starting BFS from vertex 0
    bfs_search(graph, start_vertex, dist, pred);

    // Output the distance and predecessor arrays
    cout << "Vertex distances from vertex " << start_vertex << ":\n";
    for (int i = 0; i < vertices; ++i) {
        cout << "Vertex " << i << ": Distance = " << dist[i] << ", Predecessor = " << pred[i] << endl;
    }

    return 0;
}
