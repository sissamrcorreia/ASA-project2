#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_map>

using namespace std;

// Função para calcular o número máximo de saltos a partir de um nó inicial
int maxJumps(unordered_map<int, vector<int> > & graph, int start) {
    vector<int> distances(graph.size(), -1); // Inicializa todas as distâncias como -1
    queue<int> q;
    q.push(start);
    distances[start] = 0;

    // BFS para calcular distâncias a partir do nó inicial
    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbor : graph[node]) {
            if (distances[neighbor] == -1) {
                distances[neighbor] = distances[node] + 1;
                q.push(neighbor);
            }
        }
    }

    // Retorna o máximo das distâncias calculadas
    return *max_element(distances.begin(), distances.end());
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m); // número de indivíduos (n) e número de relações (m)
    if (n < 2 || m <= 0) { // Verifica se a entrada é válida
        return 0;
    }

    unordered_map<int, vector<int> > graph; // Grafo representado como um mapa de listas de adjacência

    for (int i = 0; i < m; ++i) {
        int x, y;
        scanf("%d %d", &x, &y); // lê as relações
        graph[x].push_back(y); // adiciona a relação ao grafo
    }

    int maxJumpsResult = 0;

    // Para cada nó, calcula o máximo de saltos a partir desse nó
    for (int i = 1; i <= n; ++i) {
        maxJumpsResult = max(maxJumpsResult, maxJumps(graph, i));
    }

    printf("%d\n", maxJumpsResult);

    return 0;
}