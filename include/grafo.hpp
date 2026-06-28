#pragma once

#include <vector>

using namespace std;

class No {
public:
    int id;
    double premio;
    int grau;
    bool visitado;

    // Construtor padrão do Nó
    No(int id = 0, double premio = 0.0) : id(id), premio(premio), grau(0), visitado(false) {}
};

class Aresta {
public:
    int origem;
    int destino;
    double peso;

    Aresta(int o, int d, double p) : origem(o), destino(d), peso(p) {}
};

// Estrutura para devolver a resposta do algoritmo
struct Solucao {
    vector<int> nosUsados;
    vector<Aresta> arestasUsadas;
    double custoTotal = 0.0;
    double tempo = 0.0;
};

class Grafo {
private:
    int numNos;
    int numArestas;
    vector<No> nos;
    vector<Aresta> arestas;
    vector<vector<Aresta>> adj; // Lista de adjacência mantida para facilitar heurísticas

public:
    Grafo() : numNos(0), numArestas(0) {}

    // Métodos para o Leitor popular o grafo
    void inicializar(int n, int m);
    void adicionarAresta(int u, int v, double peso);
    void setPremio(int no_id, double premio);

    // Getters para o main.cpp e solucao.cpp acessarem os dados com segurança
    int getNumNos() const { return numNos; }
    int getNumArestas() const { return arestas.size(); }
    const vector<No>& getNos() const { return nos; }
    const vector<Aresta>& getArestas() const { return arestas; }
    const vector<vector<Aresta>>& getAdj() const { return adj; }

    // O seu Kruskal
    void kruskal();
};

// struct Aresta {
//     int to;
//     double pesos;
// };

// struct Grafo {
//     int numNos;
//     int numArestas;
//   // Lista de adjacência: Grafo[u] = lista de arestas saindo de u
//     vector<vector<Aresta>> adj; 
    
//     // array de prêmios: prizes[u] = valor do prêmio/penalidade do nó u
//     vector<double> premios; 
// };

// // Estrutura para devolver a resposta do seu algoritmo
// struct Solucao {
//     vector<int> nosUsados;  // Nós que você decidiu incluir
//     vector<Aresta> arestasUsadas; // Arestas que conectam esses nós
//     double custoTotal;              // Custo final da FO (Função Objetivo)
//     double tempo;          // Tempo gasto para encontrar a solução
// };
