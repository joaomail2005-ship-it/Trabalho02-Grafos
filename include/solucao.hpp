#ifndef SOLUCAO_HPP
#define SOLUCAO_HPP
#include <random>

using namespace std;

#include "grafo.hpp"
struct Solucao {
  vector<int> nosUsados;
  vector<Aresta> arestasUsadas;
  double custoTotal = 0.0;
  double tempo = 0.0;
};

Solucao heuristicaSimples(const Grafo &g, double alpha, std::mt19937 &rng);

#endif // SOLUCAO_HPP