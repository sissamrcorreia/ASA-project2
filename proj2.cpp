// Projeto 2 - Análise de Sistemas de Algoritmos - 2023

// Grupo: AL002
// Cecília Correia - 106827
// Luísa Fernandes - 102460


#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>

using namespace std;

void dfs1(unordered_map<int, vector<int>> &graph, int node, stack<int> &order, unordered_set<int> &visited) {
    visited.insert(node);
    for (int neighbor : graph[node]) {
        if (visited.find(neighbor) == visited.end()) {
            dfs1(graph, neighbor, order, visited);
        }
    }
    order.push(node);
}

void dfs2(unordered_map<int, vector<int>> &reverseGraph, int node, unordered_set<int> &scc, unordered_set<int> &visited) {
    visited.insert(node);
    scc.insert(node);
    for (int neighbor : reverseGraph[node]) {
        if (visited.find(neighbor) == visited.end()) {
            dfs2(reverseGraph, neighbor, scc, visited);
        }
    }
}

int findSCC(unordered_map<int, vector<int>> &graph, unordered_map<int, vector<int>> &reverseGraph, vector<unordered_set<int>> &sccList) {
    stack<int> order;
    unordered_set<int> visited;

    // Primeira DFS para obter a ordem topológica
    for (auto &entry : graph) {
        int node = entry.first;
        if (visited.find(node) == visited.end()) {
            dfs1(graph, node, order, visited);
        }
    }

    visited.clear();

    // Segunda DFS para encontrar SCCs
    while (!order.empty()) {
        int node = order.top();
        order.pop();

        if (visited.find(node) == visited.end()) {
            unordered_set<int> scc;
            dfs2(reverseGraph, node, scc, visited);
            sccList.push_back(scc);
        }
    }

    return sccList.size();
}

int maxJumpsSCC(unordered_map<int, vector<int>> &graph, unordered_map<int, vector<int>> &reverseGraph, int n) {
    vector<unordered_set<int>> sccList;
    findSCC(graph, reverseGraph, sccList);

    // Calcula o número máximo de saltos considerando o tamanho da maior SCC
    int maxJumps = 1;  // Inicializa com 1 para o caso de não haver SCCs maiores que 1
    for (const auto &scc : sccList) {
        if (scc.size() > 1) {
            maxJumps = max(maxJumps, static_cast<int>(scc.size()));
        }
    }

    return maxJumps;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    if (n < 2 || m <= 0) {
        return 0;
    }

    unordered_map<int, vector<int>> graph;
    unordered_map<int, vector<int>> reverseGraph;

    for (int i = 0; i < m; ++i) {
        int x, y;
        scanf("%d %d", &x, &y);
        graph[x].push_back(y);
        reverseGraph[y].push_back(x);
    }

    int maxJumpsResult = maxJumpsSCC(graph, reverseGraph, n);

    printf("%d\n", maxJumpsResult);

    return 0;
}