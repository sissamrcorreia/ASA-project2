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

// Função DFS para obter a ordem topológica
void dfs1(unordered_map<int, vector<int>> &graph, int start, stack<int> &order, unordered_set<int> &visited) {
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


// Função DFS para encontrar as Componentes Fortemente Conectadas (SCCs)
void dfs2(unordered_map<int, vector<int>> &reverseGraph, int start, unordered_set<int> &scc, unordered_set<int> &visited) {
    stack<int> s;
    s.push(start);
    visited.insert(start);

    while (!s.empty()) {
        int current = s.top();
        s.pop();
        scc.insert(current);

        for (int neighbor : reverseGraph[current]) {
            if (visited.find(neighbor) == visited.end()) {
                s.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }
}


// Função para encontrar as SCCs em um grafo direcionado
void findSCCs(unordered_map<int, vector<int>> &graph, unordered_map<int, vector<int>> &reverseGraph, vector<unordered_set<int>> &sccList) {
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
}


// Função para transformar o grafo em uma DAG, não contamos com saltos de grupos de pessoas que se conhecem mutuamente (ECCs)
unordered_map<int, vector<int>> makeGraphDAG(unordered_map<int, vector<int>> &graph, const vector<unordered_set<int>> &sccList) {
    unordered_map<int, vector<int>> dag;

    // Mapeia cada nó para SCC correspondente
    unordered_map<int, int> nodeToSCC;

    // Preenche o mapeamento nodeToSCC
    for (int i = 0; i < sccList.size(); ++i) {
        const unordered_set<int> &scc = sccList[i];
        for (int node : scc) {
            nodeToSCC[node] = i;
        }
    }

    // Para cada aresta no grafo original, adiciona uma aresta ao DAG
    for (auto &entry : graph) {
        int fromNode = entry.first;
        for (int toNode : entry.second) {
            // Verifica se as arestas pertencem a diferentes SCCs
            if (nodeToSCC[fromNode] != nodeToSCC[toNode]) {
                dag[nodeToSCC[fromNode]].push_back(nodeToSCC[toNode]);
            }
        }
    }

    return dag;
}


int maxJumpsDAG(unordered_map<int, vector<int>> &dag, int n) {
    vector<int> inDegree(n, 0);

    // Calcula os pesos de entrada dos nós no DAG
    for (auto &entry : dag) {
        for (int neighbor : entry.second) {
            inDegree[neighbor]++;
        }
    }

    // Inicializa uma fila para nós com peso de entrada zero
    queue<int> zeroWeight;

    for (int i = 0; i < n; ++i) {
        if (inDegree[i] == 0) {
            zeroWeight.push(i);
        }
    }

    int jumps = 0;

    // Realiza uma procura (BFS) no DAG
    while (!zeroWeight.empty()) {
        int size = zeroWeight.size();

        // Processa todos os nós com peso de entrada zero
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

    // Retorna o número total de saltos necessários
    return jumps;
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

    // Encontrar SCCs
    vector<unordered_set<int>> sccList;
    findSCCs(graph, reverseGraph, sccList);

    // Transformar o grafo em uma DAG
    unordered_map<int, vector<int>> dag = makeGraphDAG(graph, sccList);

    // Calcular o número máximo de saltos na DAG resultante
    int maxJumpsResult = maxJumpsDAG(dag, n);

    printf("%d\n", maxJumpsResult);

    return 0;
}