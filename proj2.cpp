// Projeto 2 - Análise de Sistemas de Algoritmos - 2023

// Grupo: AL002
// Cecília Correia - 106827
// Luísa Fernandes - 102460


#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>

using namespace std;

// Função para encontrar a distância máxima em uma DAG
int maxDistanceInDAG(unordered_map<int, vector<int>>& graph, int start) {
    int n = graph.size();
    vector<int> distance(n, -1);
    queue<int> q;

    q.push(start);
    distance[start] = 0;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbor : graph[node]) {
            if (distance[neighbor] == -1) {
                distance[neighbor] = distance[node] + 1;
                q.push(neighbor);
            }
        }
    }

    return *max_element(distance.begin(), distance.end());
}

// Função para encontrar todas as SCCs do grafo
vector<vector<int>> findSCCs(unordered_map<int, vector<int>>& graph) {
    int n = graph.size();
    vector<int> disc(n, -1), low(n, -1);
    stack<int> s;
    unordered_set<int> inStack;
    vector<vector<int>> sccs;
    int time = 0;

    function<void(int)> tarjanSCC = [&](int node) {
        disc[node] = low[node] = ++time;
        s.push(node);
        inStack.insert(node);

        for (int neighbor : graph[node]) {
            if (disc[neighbor] == -1) {
                tarjanSCC(neighbor);
                low[node] = min(low[node], low[neighbor]);
            } else if (inStack.count(neighbor) > 0) {
                low[node] = min(low[node], disc[neighbor]);
            }
        }

        if (low[node] == disc[node]) {
            // Constroir uma nova SCC
            vector<int> scc;
            while (true) {
                int v = s.top();
                s.pop();
                inStack.erase(v);
                scc.push_back(v);
                if (v == node) break;
            }
            sccs.push_back(scc);
        }
    };

    for (auto& entry : graph) {
        int node = entry.first;
        if (disc[node] == -1) {
            tarjanSCC(node);
        }
    }

    return sccs;
}

int main() {
    int n, m; // n numero de individuos, m numero de relacoes
    scanf("%d %d", &n, &m);

    unordered_map<int, vector<int>> graph;

    // Preenche o grafo com as relações fornecidas
    for (int i = 0; i < m; ++i) {
        int x, y;
        scanf("%d %d", &x, &y);
        graph[x].push_back(y);
    }

    vector<vector<int>> sccs = findSCCs(graph);
    int maxJumpsResult = 0;

    // Para cada SCC, calcular a distância máxima dentro da SCC
    for (const auto& scc : sccs) {
        int maxDistanceInSCC = 0;
        for (int node : scc) {
            int distance = maxDistanceInDAG(graph, node);
            maxDistanceInSCC = max(maxDistanceInSCC, distance);
        }
        maxJumpsResult = max(maxJumpsResult, maxDistanceInSCC);
    }

    // resultado final
    printf("%d\n", maxJumpsResult);

    return 0;
}