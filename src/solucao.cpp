#include "solucao.hpp"
#include <algorithm>
#include <chrono>
#include <functional> // Para lambdas recursivos
#include <iostream>
#include <queue>
#include <random>
#include <vector>


using namespace std;
using namespace std::chrono;

Solucao heuristicaSimples(const Grafo &g, double alpha, std::mt19937 &rng) {
  // começa a contar o tempo
  auto inicio = high_resolution_clock::now();

  Solucao sol;

  int V = g.getNumNos();
  if (V == 0)
    return sol;

  vector<Aresta> todas_arestas = g.getArestas();

  // vetor pra nao deletar elementos da lista original
  vector<bool> aresta_usada(todas_arestas.size(), false);
  int arestas_adicionadas = 0;

  // ordena as arestas pelo peso usando uma função lambda
  sort(todas_arestas.begin(), todas_arestas.end(),
       [](const Aresta &a, const Aresta &b) { return a.peso < b.peso; });

  // trecho pra verificar se tem ciclos
  vector<int> pai(V + 1);

  for (int i = 1; i <= V; ++i) {
    pai[i] = i;
  }

  // algoritmo de componentes conexas
  function<int(int)> find = [&](int i) {
    if (pai[i] == i)
      return i;
    return pai[i] = find(pai[i]);
  };

  // une dois componentes conexas
  auto unite = [&](int i, int j) {
    int raizI = find(i);
    int raizJ = find(j);
    if (raizI != raizJ)
      pai[raizI] = raizJ;
  };

  // estruturas de controle para a arvore geradora minima (mst)
  vector<Aresta> mst_arestas;
  vector<vector<Aresta>> mst_adj(V + 1);
  vector<int> grau_mst(V + 1, 0);
  vector<bool> na_arvore(V + 1, false);

  // loop principal do kruskal randomizado (baseado em valor/limiar)
  while (arestas_adicionadas < V - 1) {
    double custoMin = -1.0;
    double custoMax = -1.0;

    // 1. Encontrar Cmin e Cmax de arestas validas
    for (size_t i = 0; i < todas_arestas.size(); ++i) {
      if (!aresta_usada[i]) {
        // procura o ciclo
        if (find(todas_arestas[i].origem) != find(todas_arestas[i].destino)) {
          if (custoMin < 0) {
            custoMin =
                todas_arestas[i]
                    .peso; // A primeira válida é a mínima (pois está ordenado)
          }
          custoMax = todas_arestas[i]
                         .peso; // Continua atualizando, a última será a máxima
        }
      }
    }

    // grafo desconexo
    if (custoMin < 0)
      break;

    // 2. Calcular o Limiar de Custo
    double limiar = custoMin + alpha * (custoMax - custoMin);

    // 3. preenche a RCL com todas as arestas válidas <= Limiar
    vector<int> rcl_indices;
    for (size_t i = 0; i < todas_arestas.size(); ++i) {
      if (!aresta_usada[i]) {
        // Como o vetor está ordenado, se o peso passar do limiar, podemos parar
        // de procurar
        if (todas_arestas[i].peso > limiar)
          break;

        if (find(todas_arestas[i].origem) != find(todas_arestas[i].destino)) {
          rcl_indices.push_back(i);
        }
      }
    }

    // 4. sorteia uma das opções na RCL
    uniform_int_distribution<int> dist(0, rcl_indices.size() - 1);
    int indice_escolhido = rcl_indices[dist(rng)];

    // marca a aresta como usada
    aresta_usada[indice_escolhido] = true;
    Aresta aresta = todas_arestas[indice_escolhido];

    // processo padrão de união
    unite(aresta.origem, aresta.destino);
    mst_arestas.push_back(aresta);

    mst_adj[aresta.origem].push_back(aresta);
    Aresta inversa(aresta.destino, aresta.origem, aresta.peso);
    mst_adj[aresta.destino].push_back(inversa);

    grau_mst[aresta.origem]++;
    grau_mst[aresta.destino]++;

    na_arvore[aresta.origem] = true;
    na_arvore[aresta.destino] = true;

    arestas_adicionadas++;
  }

  // verificação de prejuízo nos vértices
  // Processo de poda
  queue<int> folhas;
  const auto &nos_grafo = g.getNos();

  // Mapeia as folhas iniciais (Grau 1)
  for (int i = 1; i <= V; ++i) {
    if (na_arvore[i] && grau_mst[i] == 1) {
      folhas.push(i);
    }
  }

  while (!folhas.empty()) {
    int u = folhas.front();
    folhas.pop();

    if (!na_arvore[u])
      continue;

    int pai_no = -1;
    double peso_aresta = 0.0;

    // Localiza qual é a única aresta ativa que liga a folha à árvore
    for (const auto &a : mst_adj[u]) {
      int vizinho = (a.origem == u) ? a.destino : a.origem;
      if (na_arvore[vizinho]) {
        pai_no = vizinho;
        peso_aresta = a.peso;
        break;
      }
    }

    if (pai_no != -1) {
      // custo da aresta estritamente maior que o premio do no folha
      if (peso_aresta > nos_grafo[u].premio) {
        na_arvore[u] = false; // poda o no
        grau_mst[u]--;
        grau_mst[pai_no]--; // atualiza o grau do pai

        // Se o pai perdeu a conexão e virou uma folha, entra na fila (Efeito
        // Cascata)
        if (grau_mst[pai_no] == 1) {
          folhas.push(pai_no);
        }
      }
    }
  }

  // calculo da função objetivo e retorno
  double soma_custos_arestas = 0.0;
  for (const auto &a : mst_arestas) {
    if (na_arvore[a.origem] && na_arvore[a.destino]) {
      sol.arestasUsadas.push_back(a);
      soma_custos_arestas += a.peso;
    }
  }

  double soma_premios_perdidos = 0.0;
  for (int i = 1; i <= V; ++i) {
    if (na_arvore[i]) {
      sol.nosUsados.push_back(i);
    } else {
      soma_premios_perdidos += nos_grafo[i].premio;
    }
  }

  sol.custoTotal = soma_custos_arestas + soma_premios_perdidos;

  // cronômetro
  auto fim = high_resolution_clock::now();
  duration<double> duracao = fim - inicio;
  sol.tempo = duracao.count();

  return sol;
}