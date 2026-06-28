#include "grafo.hpp"
#include <iostream>

void Grafo::inicializar(int n, int m) {
    numNos = n;
    numArestas = m;
    nos.resize(n + 1); // +1 porque os nós começam do 1 no arquivo STP
    adj.resize(n + 1);
    
    for (int i = 0; i <= n; ++i) {
        nos[i].id = i;
        nos[i].premio = 0.0;
    }
}

void Grafo::adicionarAresta(int u, int v, double peso) {
    Aresta a1(u, v, peso);
    Aresta a2(v, u, peso); // Grafo não direcionado
    
    arestas.push_back(a1); // Adiciona na lista geral
    
    adj[u].push_back(a1);  // Adiciona na lista de adjacência
    adj[v].push_back(a2);
    
    nos[u].grau++;
    nos[v].grau++;
}

void Grafo::setPremio(int no_id, double premio) {
    if (no_id > 0 && no_id <= numNos) {
        nos[no_id].premio = premio;
    }
}