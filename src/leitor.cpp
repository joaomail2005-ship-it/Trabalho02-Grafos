#include "leitor.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

Grafo lerArquivoSTP(const string& caminhoArquivo) {
    Grafo g;
    g.numNos = 0;
    g.numArestas = 0;

    ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        // std::endl força a saída imediata na tela sem depender de buffer
        cerr << "Erro critico: Nao foi possivel abrir o arquivo: " << caminhoArquivo << endl;
        return g;
    }

    string linha;
    string secaoAtual = "";

    while (getline(arquivo, linha)) {
        if (linha.empty() || linha[0] == '#') {
            continue;
        }

        stringstream ss(linha);
        string token;
        ss >> token;

        // Torna a leitura de SECTION e END insensível a maiúsculas/minúsculas
        if (token == "SECTION" || token == "Section" || token == "section") {
            ss >> secaoAtual;
            continue;
        }
        if (token == "END" || token == "End" || token == "end") {
            secaoAtual = "";
            continue;
        }

        // Processa a seção do Grafo de forma robusta
        if (secaoAtual == "Graph" || secaoAtual == "GRAPH" || secaoAtual == "graph") {
            if (token == "Nodes" || token == "NODES" || token == "nodes") {
                ss >> g.numNos;
                g.adj.resize(g.numNos + 1);
                g.premios.assign(g.numNos + 1, 0.0);
            } 
            else if (token == "Edges" || token == "EDGES" || token == "edges") {
                ss >> g.numArestas;
            } 
            else if (token == "E" || token == "e") {
                int u, v;
                double peso;
                ss >> u >> v >> peso;
                
                // TRAVA DE SEGURANÇA: Evita o crash se os nós não foram inicializados antes
                if (g.adj.empty() || u >= (int)g.adj.size() || v >= (int)g.adj.size()) {
                    cerr << "[ERRO CRITICO] Tentativa de ler aresta (" << u << " -> " << v 
                              << ") mas o tamanho do grafo nao foi definido corretamente no arquivo!" << endl;
                    g.numNos = 0; // força indicar falha
                    return g;
                }
                
                g.adj[u].push_back({v, peso});
                g.adj[v].push_back({u, peso});
            }
        } 
        // Processa a seção de Terminais de forma robusta
        else if (secaoAtual == "Terminals" || secaoAtual == "TERMINALS" || secaoAtual == "terminals") {
            if (token == "TP" || token == "tp") {
                int no;
                double premio;
                ss >> no >> premio;
                
                if (!g.premios.empty() && no < (int)g.premios.size()) {
                    g.premios[no] = premio;
                }
            }
        }
    }

    arquivo.close();
    return g;
}