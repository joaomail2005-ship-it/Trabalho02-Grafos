#pragma once

#include <vector>
using namespace std;

struct Aresta {
    int to;
    double pesos;
};

struct Grafo {
    int numNos;
    int numArestas;
  // Lista de adjacência: Grafo[u] = lista de arestas saindo de u
    vector<vector<Aresta>> adj; 
    
    // array de prêmios: prizes[u] = valor do prêmio/penalidade do nó u
    vector<double> premios; 
};

// Estrutura para devolver a resposta do seu algoritmo
struct Solucao {
    vector<int> nosUsados;  // Nós que você decidiu incluir
    vector<Aresta> arestasUsadas; // Arestas que conectam esses nós
    double custoTotal;              // Custo final da FO (Função Objetivo)
    double tempo;          // Tempo gasto para encontrar a solução
};
