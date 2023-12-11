// Projeto 2 - Análise de Sistemas de Algoritmos - 2023

// Grupo: AL002
// Cecília Correia - 106827
// Luísa Fernandes - 102460


#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

// Função para encontrar as SCCs
void tarjanSCC(int node, unordered_map<int, vector<int>>& graph, vector<int>& disc, vector<int>& low, stack<int>& s, unordered_set<int>& inStack, vector<vector<int>>& sccs, int& time) {
    disc[node] = low[node] = ++time;
    s.push(node);
    inStack.insert(node);

    for (int neighbor : graph[node]) {
        if (disc[neighbor] == -1) {
            tarjanSCC(neighbor, graph, disc, low, s, inStack, sccs, time);
            low[node] = min(low[node], low[neighbor]);
        } else if (inStack.count(neighbor) > 0) {
            low[node] = min(low[node], disc[neighbor]);
        }
    }

    if (low[node] == disc[node]) {
    // Constrói uma nova SCC
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
}

// Função para encontrar todas as SCCs do grafo
vector<vector<int>> findSCCs(unordered_map<int, vector<int>>& graph) {
    int n = graph.size();
    vector<int> disc(n, -1), low(n, -1);
    stack<int> s;
    unordered_set<int> inStack;
    vector<vector<int>> sccs;
    int time = 0;

    for (auto& entry : graph) {
        int node = entry.first;
        if (disc[node] == -1) {
            tarjanSCC(node, graph, disc, low, s, inStack, sccs, time);
        }
    }

    return sccs;
}

int main() {
    int n, m;
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
    
    for (const auto& scc : sccs) {
        // Considera apenas as SCCs que não são ciclos (tamanho maior que 1)
        if (scc.size() > 1) {
            // Calcula o comprimento do caminho mais longo dentro da SCC
            int maxDistanceInSCC = 0;
            for (int node : scc) {
                for (int neighbor : graph[node]) {
                    if (find(scc.begin(), scc.end(), neighbor) == scc.end()) {
                        // O vizinho está fora da SCC
                        maxDistanceInSCC = max(maxDistanceInSCC, 1);
                    }
                }
            }
            maxJumpsResult = max(maxJumpsResult, maxDistanceInSCC);
        }
    }

    printf("%d\n", maxJumpsResult);

    return 0;
}