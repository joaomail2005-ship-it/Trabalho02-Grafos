#include "grafo.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <tuple>      // Para usar std::tuple
#include <functional> // Para podermos usar recursão no lambda do "find"

using namespace std; 

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

void Grafo::kruskal(const Grafo &g) {
    //tupla no formato {peso, origem, destino}
    // Mudamos os chars para inteiros, pois os nós nos arquivos STP são numéricos
    vector<tuple<double, int, int>> todas_arestas;

    // Passo A: Preencher o vetor com as arestas reais do grafo
    for (const Aresta& a : g.getArestas()) {
        todas_arestas.push_back(make_tuple(a.peso, a.origem, a.destino));
    }

    sort(todas_arestas.begin(), todas_arestas.end());

    // O Union-Find agora usa inteiros
    unordered_map<int, int> pai;
    
    for (const No& no : g.getNos()) {
        if (no.id != 0) { // Ignora o nó 0 que não é usado nos STPs
            pai[no.id] = no.id;
        }
    }

    function<int(int)> find = [&](int v) {
        if (pai[v] == v) return v;
        return pai[v] = find(pai[v]); 
    };

    auto unite = [&](int a, int b) {
        int raizA = find(a);
        int raizB = find(b);
        if (raizA != raizB) {
            pai[raizA] = raizB;
        }
    };
   
    vector<tuple<double, int, int>> arvore_geradora_minima;
    double custo_total = 0.0;

    for(const auto& aresta : todas_arestas) {
        
        double peso = get<0>(aresta);
        int origem = get<1>(aresta);
        int destino = get<2>(aresta);

        // Se origem e destino estão em componentes diferentes, não forma ciclo
        if (find(origem) != find(destino)) {
            arvore_geradora_minima.push_back(aresta);
            custo_total += peso;
            unite(origem, destino); // Junta as componentes
        }
    }

    // Passo E: Exibir o resultado
    cout << "--- Arvore Geradora Minima (Kruskal) ---" << endl;
    cout << "Arestas Usadas: " << arvore_geradora_minima.size() << endl;
    cout << "Custo total da arvore (Kruskal): " << custo_total << endl;
    
    // (Descomente o trecho abaixo se quiser imprimir cada aresta na tela)
    // for (const auto& a : arvore_geradora_minima) {
    //     cout << get<1>(a) << " - " << get<2>(a) << " : peso " << get<0>(a) << endl;
    // }
}