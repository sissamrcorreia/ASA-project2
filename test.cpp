#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <limits>
#include <climits>
#include <unordered_set>
#include <algorithm> 
#include <queue>

using namespace std;

/* Função DFS recursiva
void dfs(int node, unordered_map<int, vector<int>>& graph, unordered_set<int>& visited, stack<int>& order) {
    // Marcar o nó como visitado
    visited.insert(node);

    // Visitar todos os vizinhos não visitados
    for (int neighbor : graph[node]) {
        if (visited.find(neighbor) == visited.end()) {
            dfs(neighbor, graph, visited, order);
        }
    }

    // Empilhar o nó após visitar todos os vizinhos
    order.push(node);
}
void dfsReverse(int node, unordered_map<int, vector<int>>& reverseGraph, unordered_set<int>& visited, vector<int>& scc) {
    visited.insert(node);
    scc.push_back(node);

    for (int neighbor : reverseGraph[node]) {
        if (visited.find(neighbor) == visited.end()) {
            dfsReverse(neighbor, reverseGraph, visited, scc);
        }
    }
}
*/
// Função DFS iterativa
void dfs(int start,unordered_map<int, vector<int>> &graph, unordered_set<int>& visited , stack<int> &order )
{
    stack<int> s;
    s.push(start);
    visited.insert(start);

    while (!s.empty())
    {
        int current = s.top();
        bool hasUnvisitedNeighbor = false;

        for (int neighbor : graph[current])
        {

            if (visited.find(neighbor) == visited.end())
            {

                s.push(neighbor);
                visited.insert(neighbor);
                hasUnvisitedNeighbor = true;
                break;
            }
        }

        if (!hasUnvisitedNeighbor)
        {
            s.pop();
            order.push(current);
        }
    }
}

// Função DFS iterativa reversa
void dfsReverse(int start, unordered_map<int, vector<int>>& reverseGraph, unordered_set<int>& visited, vector<int>& scc) {
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



void findSCCs(unordered_map<int, vector<int>>& graph, unordered_map<int, vector<int>>& reverseGraph, const vector<int>& topOrder, vector<vector<int>>& sccs) {
    unordered_set<int> visited;

    // Fase 2: Identificar SCCs
    for (int node : topOrder) {
        if (visited.find(node) == visited.end()) {
            vector<int> scc;
            dfsReverse(node, reverseGraph, visited, scc);
            // Inverter a ordem dos nós dentro de cada SCC
            reverse(scc.begin(), scc.end());
            sccs.push_back(scc);

            /* Imprimir a SCC
            //if(scc.size() > 1){
                cout << "SCC: ";
                for (int n : scc) {
                    cout << n << " ";
                }
                cout << endl;
            //}*/
        }
    }
}

// Função para encontrar a ordenação topológica usando DFS
vector<int> topologicalSort(unordered_map<int, vector<int>>& graph) {
    stack<int> order; // Usada para armazenar a ordem topológica reversa
    unordered_set<int> visited; // Usada para rastrear nós visitados

    // Iterar sobre todos os nós do grafo
    for (const auto& entry : graph) {
        int node = entry.first;
        if (visited.find(node) == visited.end()) {
            dfs(node, graph, visited, order);
        }
    }

    // Criar o vetor com a ordem topológica
    vector<int> result;
    while (!order.empty()) {
        result.push_back(order.top());
        order.pop();
    }

    return result;
}


void dfsLongestPath(vector<vector<bool>>& adjacentSCCs, int node, vector<int>& pathLength) {
    for (long unsigned int i = 0; i < adjacentSCCs.size(); ++i) {
        if (adjacentSCCs[node][i]) {
            pathLength[i] = max(pathLength[i], pathLength[node] + 1);
            dfsLongestPath(adjacentSCCs, i, pathLength);
        }
    }
}

// Função para calcular o caminho mais longo entre SCCs
int calculateLongestPathBetweenSCCs(unordered_map<int, vector<int>>& graph, vector<vector<int>>& sccs) {
    int numSCCs = sccs.size();

    // Construir matriz de adjacência para representar as relações entre as SCCs
    vector<vector<bool>> adjacentSCCs(numSCCs, vector<bool>(numSCCs, false));

    for (int i = 0; i < numSCCs; ++i) {
        for (int node : sccs[i]) {
            for (int neighbor : graph[node]) {
                for (int j = 0; j < numSCCs; ++j) {
                    if (find(sccs[j].begin(), sccs[j].end(), neighbor) != sccs[j].end() && i != j) {
                        adjacentSCCs[i][j] = true;
                    }
                }
            }
        }
    }

    // Calcular caminho mais longo entre SCCs usando DFS
    int longestPath = 0;

    for (int i = 0; i < numSCCs; ++i) {
        vector<int> pathLength(numSCCs, 0);
        dfsLongestPath(adjacentSCCs, i, pathLength);
        longestPath = max(longestPath, *max_element(pathLength.begin(), pathLength.end()));
    }

    return longestPath;
}






int main() {
    

    int n, m;
    
    if (scanf("%d %d", &n, &m) != 2) {
        // Lidar com erro de leitura
        fprintf(stderr, "Erro ao ler os valores de entrada.\n");
        return -1; 
    }

    if (n < 2 || m <= 0) {
        return 0;
    }

    unordered_map<int, vector<int>> graph;
    unordered_map<int, vector<int>> reverseGraph;

    for (int i = 0; i < m; ++i) {
        int x, y;
        //scanf("%d %d", &x, &y);
        if (scanf("%d %d", &x, &y) != 2) {
            // Lidar com erro de leitura
            fprintf(stderr, "Erro ao ler os valores de entrada.\n");
            return -1; 
        }
        graph[x].push_back(y);
        reverseGraph[y].push_back(x);
    }
    
    /*show original graphs
    printf("Original Graph Nodes and Edges:\n");
    for (const auto &entry : graph) {
        int node = entry.first;
        const vector<int> &neighbors = entry.second;
        printf("Node %d -> ", node);
        for (int neighbor : neighbors) {
            printf("%d ", neighbor);
        }
        printf("\n");
    }*/

    // Encontrar a ordenação topológica
    vector<int> result = topologicalSort(graph);

    /* Imprimir a ordenação topológica
    cout << "Ordenação Topológica: ";
    for (int node : result) {
        cout << node << " ";
    }
    cout << endl;*/

    vector<vector<int>> sccs;
    // Encontrar e imprimir SCCs usando a ordem topológica e o grafo reverso
    findSCCs(graph, reverseGraph, result,sccs);

    // Calcular o caminho mais longo entre SCCs
    int longestPath = calculateLongestPathBetweenSCCs(graph, sccs);

    // Imprimir o caminho mais longo entre SCCs
    //cout << "Caminho mais longo entre SCCs: " << longestPath << endl;
    cout << longestPath << endl;

    
    

    return 0;
}


