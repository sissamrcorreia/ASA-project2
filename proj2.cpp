// Projeto 2 - Análise de Sistemas de Algoritmos - 2023

// Grupo: AL002
// Cecília Correia - 106827
// Luísa Fernandes - 102460

#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

// Função main
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

    // DFS para obter a ordem topológica
    stack<int> order;
    unordered_set<int> visited1;

    for (auto &entry : graph) {
        int node = entry.first;
        if (visited1.find(node) == visited1.end()) {
            stack<int> s;
            s.push(node);
            visited1.insert(node);

            while (!s.empty()) {
                int current = s.top();
                bool hasUnvisitedNeighbor = false;

                for (int neighbor : graph[current]) {
                    if (visited1.find(neighbor) == visited1.end()) {
                        s.push(neighbor);
                        visited1.insert(neighbor);
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
    }

    // DFS para encontrar as Componentes Fortemente Conectadas (SCCs)
    vector<unordered_set<int>> sccList;
    unordered_set<int> visited2;

    while (!order.empty()) {
        int node = order.top();
        order.pop();

        if (visited2.find(node) == visited2.end()) {
            stack<int> s;
            s.push(node);
            visited2.insert(node);
            unordered_set<int> scc;

            while (!s.empty()) {
                int current = s.top();
                s.pop();
                scc.insert(current);

                for (int neighbor : reverseGraph[current]) {
                    if (visited2.find(neighbor) == visited2.end()) {
                        s.push(neighbor);
                        visited2.insert(neighbor);
                    }
                }
            }

            sccList.push_back(scc);
        }
    }

    // Transformar o grafo em uma DAG
    unordered_map<int, vector<int>> dag;

    unordered_map<int, int> nodeToSCC;
    for (size_t i = 0; i < sccList.size(); ++i) {
        const unordered_set<int> &scc = sccList[i];
        for (int node : scc) {
            nodeToSCC[node] = i;
        }
    }

    for (auto &entry : graph) {
        int fromNode = entry.first;
        for (int toNode : entry.second) {
            if (nodeToSCC[fromNode] != nodeToSCC[toNode]) {
                dag[nodeToSCC[fromNode]].push_back(nodeToSCC[toNode]);
            }
        }
    }

    // Calcular o número máximo de saltos na DAG resultante
    vector<int> inDegree(n, 0);
    for (auto &entry : dag) {
        for (int neighbor : entry.second) {
            inDegree[neighbor]++;
        }
    }

    queue<int> zeroWeight;

    for (int i = 0; i < n; ++i) {
        if (inDegree[i] == 0) {
            zeroWeight.push(i);
        }
    }

    int jumps = 0;

    while (!zeroWeight.empty()) {
        int size = zeroWeight.size();

        for (int i = 0; i < size; ++i) {
            int node = zeroWeight.front();
            zeroWeight.pop();

            for (int neighbor : dag[node]) {
                inDegree[neighbor]--;

                if (inDegree[neighbor] == 0) {
                    zeroWeight.push(neighbor);
                }
            }
        }

        jumps++;
    }

    // Imprimir o resultado
    printf("%d\n", jumps - 1);

    return 0;
}