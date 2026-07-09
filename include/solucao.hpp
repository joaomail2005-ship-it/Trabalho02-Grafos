#ifndef SOLUCAO_HPP
#define SOLUCAO_HPP
#include <random>

using namespace std;

#include "grafo.hpp"
// Estrutura para devolver a resposta do algoritmo
struct Solucao {
    vector<int> nosUsados;
    vector<Aresta> arestasUsadas;
    double custoTotal = 0.0;
    double tempo = 0.0;
};
// Declaração da nossa função simples de resolução
Solucao heuristicaSimples(const Grafo& g, double alpha, std::mt19937 &rng);
// Solucao kruskal(); 

#endif // SOLUCAO_HPP