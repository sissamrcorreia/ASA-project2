// Projeto 2 - Análise de Sistemas de Algoritmos - 2023

// Grupo: AL002
// Cecília Correia - 106827
// Luísa Fernandes - 102460

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

// Função main
int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    vector<vector<int>> graph(n + 1);
    vector<vector<int>> reverseGraph(n + 1);

    for (int i = 0; i < m; ++i) {
        int x, y;
        scanf("%d %d", &x, &y);
        graph[x].push_back(y);
        reverseGraph[y].push_back(x);
    }

    // DFS para obter a ordem topológica
    stack<int> order;
    vector<bool> visited1(n + 1, false);

    for (int i = 1; i <= n; ++i) {
        if (!visited1[i]) {
            stack<int> s;
            s.push(i);
            visited1[i] = true;

            while (!s.empty()) {
                int current = s.top();
                bool hasUnvisitedNeighbor = false;

                for (int neighbor : graph[current]) {
                    if (!visited1[neighbor]) {
                        s.push(neighbor);
                        visited1[neighbor] = true;
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
    vector<vector<int>> sccList;
    vector<bool> visited2(n + 1, false);

    while (!order.empty()) {
        int node = order.top();
        order.pop();

        if (!visited2[node]) {
            stack<int> s;
            s.push(node);
            visited2[node] = true;
            vector<int> scc;

            while (!s.empty()) {
                int current = s.top();
                s.pop();
                scc.push_back(current);

                for (int neighbor : reverseGraph[current]) {
                    if (!visited2[neighbor]) {
                        s.push(neighbor);
                        visited2[neighbor] = true;
                    }
                }
            }

            sccList.push_back(scc);
        }
    }

    // Transformar o grafo em uma semi DAG 
    vector<vector<int>> dag(n + 1);

    vector<int> nodeToSCC(n + 1);
    for (size_t i = 0; i < sccList.size(); ++i) {
        const vector<int>& scc = sccList[i];
        for (int node : scc) {
            nodeToSCC[node] = i + 1;  // Ajuste dos índices
        }
    }

    for (int fromNode = 1; fromNode <= n; ++fromNode) {
        for (int toNode : graph[fromNode]) {
            if (nodeToSCC[fromNode] != nodeToSCC[toNode]) {
                dag[nodeToSCC[fromNode]].push_back(nodeToSCC[toNode]);
            }
        }
    }

    // Calcular o número máximo de saltos
    vector<int> number(n + 1);
    for (int i = 1; i <= n; ++i) {
        for (int entry : dag[i]) {
            number[entry] = max(number[entry], number[i] + 1);
        }
    }

    int jumps = *max_element(number.begin(), number.end()) - *number.begin();

    // Imprimir o resultado
    printf("%d\n", jumps);

    return 0;
}