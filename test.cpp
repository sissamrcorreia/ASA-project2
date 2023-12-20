#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

// Função DFS iterativa para ordenação topológica
void dfsTopoSort(int start, unordered_map<int, vector<int>> &graph, unordered_set<int> &visited, stack<int> &order) {
    stack<int> s;
    s.push(start);
    visited.insert(start);

    while (!s.empty()) {
        int current = s.top();
        bool hasUnvisitedNeighbor = false;

        for (int neighbor : graph[current]) {
            if (visited.find(neighbor) == visited.end()) {
                s.push(neighbor);
                visited.insert(neighbor);
                hasUnvisitedNeighbor = true;
                break;
            }
        }

        if (!hasUnvisitedNeighbor) {
            s.pop();
            order.push(current);
        }
    }
}

// Função DFS iterativa reversa para encontrar SCCs
void dfsReverse(int start, unordered_map<int, vector<int>> &reverseGraph, unordered_set<int> &visited, vector<int> &scc) {
    stack<int> s;
    s.push(start);
    visited.insert(start);

    while (!s.empty()) {
        int current = s.top();
        bool hasUnvisitedNeighbor = false;

        for (int neighbor : reverseGraph[current]) {
            if (visited.find(neighbor) == visited.end()) {
                s.push(neighbor);
                visited.insert(neighbor);
                hasUnvisitedNeighbor = true;
                break;
            }
        }

        if (!hasUnvisitedNeighbor) {
            s.pop();
            scc.push_back(current);
        }
    }
}

int calculateLongestPathBetweenSCCs(unordered_map<int, vector<int>> &graph, vector<vector<int>> &sccs) {
    int numSCCs = sccs.size();
    int longestPath = 0;

    for (int i = 0; i < numSCCs; ++i) {
        for (int j = 0; j < numSCCs; ++j) {
            if (i != j) {
                vector<int> &scc_i = sccs[i];
                vector<int> &scc_j = sccs[j];

                // Verificar se há uma aresta entre as SCCs
                for (int node_i : scc_i) {
                    for (int neighbor : graph[node_i]) {
                        if (find(scc_j.begin(), scc_j.end(), neighbor) != scc_j.end()) {
                            longestPath = max(longestPath, j - i);
                        }
                    }
                }
            }
        }
    }

    return longestPath;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    unordered_map<int, vector<int>> graph;
    unordered_map<int, vector<int>> reverseGraph;

    for (int i = 0; i < m; ++i) {
        int x, y;
        scanf("%d %d", &x, &y);
        graph[x].push_back(y);
        reverseGraph[y].push_back(x);
    }

    stack<int> topoOrder;
    unordered_set<int> visited;

    // Encontrar a ordenação topológica
    for (const auto &entry : graph) {
        int node = entry.first;
        if (visited.find(node) == visited.end()) {
            dfsTopoSort(node, graph, visited, topoOrder);
        }
    }

    // Inicializar SCCs
    vector<vector<int>> sccs;

    // Identificar SCCs usando a ordem topológica e o grafo reverso
    visited.clear();
    while (!topoOrder.empty()) {
        int node = topoOrder.top();
        topoOrder.pop();

        if (visited.find(node) == visited.end()) {
            vector<int> scc;
            dfsReverse(node, reverseGraph, visited, scc);
            // Inverter a ordem dos nós dentro de cada SCC
            reverse(scc.begin(), scc.end());
            sccs.push_back(scc);
        }
    }

    // Calcular o caminho mais longo entre SCCs
    int longestPath = calculateLongestPathBetweenSCCs(graph, sccs);

    // Imprimir o caminho mais longo entre SCCs
    cout << longestPath + 1 << endl;

    return 0;
}