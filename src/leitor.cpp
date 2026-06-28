#include "leitor.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

Grafo lerArquivoSTP(const string& caminhoArquivo) {
    Grafo g; // Inicia um grafo vazio (construtor cuida de numNos = 0)

    ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
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

        // Processa a seção do Grafo
        if (secaoAtual == "Graph" || secaoAtual == "GRAPH" || secaoAtual == "graph") {
            if (token == "Nodes" || token == "NODES" || token == "nodes") {
                int n;
                ss >> n;
                g.inicializar(n, 0); // Inicializa com N nós
            } 
            else if (token == "Edges" || token == "EDGES" || token == "edges") {
                int m;
                ss >> m;
                // Apenas lemos a quantidade declarada, a alocação ocorre ao ler os "E"
            } 
            else if (token == "E" || token == "e") {
                int u, v;
                double peso;
                ss >> u >> v >> peso;
                
                // TRAVA DE SEGURANÇA: Evita o crash se os nós não foram inicializados
                if (g.getNumNos() == 0 || u > g.getNumNos() || v > g.getNumNos()) {
                    cerr << "[ERRO CRITICO] Tentativa de ler aresta (" << u << " -> " << v 
                         << ") mas o tamanho do grafo nao foi definido corretamente no arquivo!" << endl;
                    
                    Grafo grafoFalho;
                    return grafoFalho; // força indicar falha retornando grafo zerado
                }
                
                g.adicionarAresta(u, v, peso); // Uso do método encapsulado
            }
        } 
        // Processa a seção de Terminais
        else if (secaoAtual == "Terminals" || secaoAtual == "TERMINALS" || secaoAtual == "terminals") {
            if (token == "TP" || token == "tp") {
                int no;
                double premio;
                ss >> no >> premio;
                
                g.setPremio(no, premio); // Uso do método encapsulado
            }
        }
    }

    arquivo.close();
    return g;
}