#include "solucao.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <chrono>
#include <functional> // Para lambdas recursivos

using namespace std;
using namespace std::chrono;

Solucao heuristicaSimples(const Grafo &g)
{
    // começa a contar o tempo
    auto inicio = high_resolution_clock::now();
    Solucao sol;

    int V = g.getNumNos();
    if (V == 0)
        return sol;

    // kruskal
    vector<Aresta> todas_arestas = g.getArestas();

    // ordena as arestas pelo peso usando uma função lambda
    sort(todas_arestas.begin(), todas_arestas.end(), [](const Aresta &a, const Aresta &b)
         { return a.peso < b.peso; });


    // trecho pra verificar se tem ciclos

    vector<int> pai(V + 1);
    for (int i = 1; i <= V; ++i)
    {
        pai[i] = i;
    }

    function<int(int)> find = [&](int i)
    {
        if (pai[i] == i)
            return i;
        return pai[i] = find(pai[i]); // Path compression
    };

    auto unite = [&](int i, int j)
    {
        int raizI = find(i);
        int raizJ = find(j);
        if (raizI != raizJ)
            pai[raizI] = raizJ;
    };

    // Estruturas de controle para a Árvore Geradora Mínima (MST)
    vector<Aresta> mst_arestas;
    vector<vector<Aresta>> mst_adj(V + 1);
    vector<int> grau_mst(V + 1, 0);
    vector<bool> na_arvore(V + 1, false);

    for (const auto &aresta : todas_arestas)
    {
        if (find(aresta.origem) != find(aresta.destino))
        {
            unite(aresta.origem, aresta.destino);
            mst_arestas.push_back(aresta);

            // Mapeia a adjacência e o grau apenas das arestas que entraram na MST
            mst_adj[aresta.origem].push_back(aresta);
            // Cria a aresta de volta, pois o grafo é não-direcionado
            Aresta inversa(aresta.destino, aresta.origem, aresta.peso);
            mst_adj[aresta.destino].push_back(inversa);

            grau_mst[aresta.origem]++;
            grau_mst[aresta.destino]++;

            na_arvore[aresta.origem] = true;
            na_arvore[aresta.destino] = true;
        }
    }


    


    // verificação de prejuízo nos vértices
    // Processo de poda
    queue<int> folhas;
    const auto &nos_grafo = g.getNos();

    // Mapeia as folhas iniciais (Grau 1)
    for (int i = 1; i <= V; ++i)
    {
        if (na_arvore[i] && grau_mst[i] == 1)
        {
            folhas.push(i);
        }
    }

    while (!folhas.empty())
    {
        int u = folhas.front();
        folhas.pop();

        if (!na_arvore[u])
            continue; // Prevenção de segurança

        int pai_no = -1;
        double peso_aresta = 0.0;

        // Localiza qual é a única aresta ativa que liga a folha à árvore
        for (const auto &a : mst_adj[u])
        {
            int vizinho = (a.origem == u) ? a.destino : a.origem;
            if (na_arvore[vizinho])
            {
                pai_no = vizinho;
                peso_aresta = a.peso;
                break;
            }
        }

        if (pai_no != -1)
        {
            // REGRA: Custo da Aresta estritamente maior que o Prêmio do Nó Folha
            if (peso_aresta > nos_grafo[u].premio)
            {
                na_arvore[u] = false; // Poda o nó
                grau_mst[u]--;
                grau_mst[pai_no]--; // Atualiza o grau do pai na árvore

                // Se o pai perdeu a conexão e virou uma folha, entra na fila (Efeito Cascata)
                if (grau_mst[pai_no] == 1)
                {
                    folhas.push(pai_no);
                }
            }
        }
    }

    // ==========================================
    // CÁLCULO DA FUNÇÃO OBJETIVO E RETORNO
    // ==========================================
    double soma_custos_arestas = 0.0;
    for (const auto &a : mst_arestas)
    {
        if (na_arvore[a.origem] && na_arvore[a.destino])
        {
            sol.arestasUsadas.push_back(a);
            soma_custos_arestas += a.peso;
        }
    }

    double soma_premios_perdidos = 0.0;
    for (int i = 1; i <= V; ++i)
    {
        if (na_arvore[i])
        {
            sol.nosUsados.push_back(i);
        }
        else
        {
            // A Função Objetivo do PCSTP pune vértices que ficaram fora da árvore
            soma_premios_perdidos += nos_grafo[i].premio;
        }
    }

    sol.custoTotal = soma_custos_arestas + soma_premios_perdidos;

    // Para o cronômetro
    auto fim = high_resolution_clock::now();
    duration<double> duracao = fim - inicio;
    sol.tempo = duracao.count(); // Salva o tempo em segundos

    return sol;
}