#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

struct Graph {
	int V;
	vector<vector<int>> adj;

	Graph(int V) {
		this->V = V;
		adj.resize(V);
	}

	void addEdge(int u, int v) {
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	// ====================================
	void parallelBFS(int startVertex) {
		vector<bool> visited(V, false);
		queue<int> q;
		visited[startVertex] = true;
		q.push(startVertex);

		while (!q.empty()) {
			int current = q.front();
			q.pop();
			cout << current << " ";
			#pragma omp parallel for
			for (int i = 0; i < adj[current].size(); i++) {
				int v = adj[current][i];
				#pragma omp critical
				{
					if (!visited[v]) {
						q.push(v);
						visited[v] = true;
					}
				}
			}
		}
	}
	// ====================================
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
	// ====================================
	void parallelDFS(int startVertex) {
		vector<bool> visited(V, false);
		#pragma omp parallel
		{
			#pragma omp single nowait
			{
				DFSUtil(startVertex, visited);
			}
		}
	}
	// ====================================
};
int main(int argc, char const* argv[]) {
	int V;
	cout << "Enter no. of vertices: ";
	cin >> V;

	Graph g(V);
	int n, source, destination;
	cout << "Enter the no. of edges: ";
	cin >> n;

	cout << "Enter edges (source destination):\n";
	for (int i = 0; i < n; i++) {
		cout << ">> ";
		cin >> source >> destination;
		g.addEdge(source, destination);
	}
	/*
	int startVertex;
	cout << "Enter the start vertex: ";
	cin >> startVertex;
	*/
	cout << "Parallel BFS: ";
	g.parallelBFS(0);

	cout << "\nParallel DFS: ";
	g.parallelDFS(0);
	
	return 0;
}
