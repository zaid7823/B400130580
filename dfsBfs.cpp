// Parallel DFS and BFS using indirected graph

#include <iostream>
#include <queue>
#include <vector>
#include <omp.h>

using namespace std;

struct Graph {
	int V;
	vector<vector<int>> adj;	// This is the undirected graph

	// Constructor
	Graph(int V) {
		this->V = V;
		adj.resize(V);
	}

	void addEdge(int u, int v) {
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	// BFS is always non-recursive
	void parallelBFS(int startVertex) {
		vector<bool> visited(V, false);	// size of the no of verticesm with all as FALSE
		queue<int> q;

		visited[startVertex] = true;	// current vertex marked as visited
		q.push(startVertex);

		while (!q.empty()) {
			int current = q.front();
			q.pop();
			cout << current << " ";

			#pragma omp parallel for
			for (int i = 0; i < adj[current].size(); i++) {	// traversing neighbours of the current node
				int v = adj[current][i];
				if (!visited[v]) {	// v instead of i because v itself is the node, not the visited or unvisited part
					
					#pragma omp critical
					{
						visited[v] = true;
						q.push(v);
					}
				}
			}
		}
	}
	// ==============================================
	void parallelDFS(int startVertex) {
		vector<bool> visited(V, false);

		#pragma omp parallel	// not "parallel for"
		{
			#pragma omp single nowait	// "single nowait" VERY IMPORTANT
			{
				DFSUtil(startVertex, visited);
			}
		}
	}
	// ==============================================
	void DFSUtil(int u, vector<bool>& visited) {
		
		visited[u] = true;
		cout << u << " ";

		#pragma omp parallel for
		for (int i = 0; i < adj[u].size(); i++) {
			int v = adj[u][i];
			if (!visited[v]) {
				DFSUtil(v, visited);
			}
		}
	}
};
int main(int argc, char const *argv[]) {
	int V;
	cout << "Enter the no. of nodes / vertices: ";
	cin >> V;

	Graph g(V);

	int n, source, destination;
	cout << "Enter the no. of edges: ";
	cin >> n;

	cout << "Enter edges in format \"source destination\":\n";
	for (int i = 0; i < n; i++) {
		cin >> source >> destination;
		g.addEdge(source, destination);
	}

	int v;
	// cout << "Enter the initial vertex: ";
	// cin >> v;

	// g.parallelBFS(v);

	cout << "\nDFS Sequence (start = " << v << " )";
	g.parallelBFS(0);

	cout << "\nBFS Sequence (start = " << v << " )";
	g.parallelDFS(0);

	return 0;
}
