#include "leitor.hpp"
#include <fstream>
#include <iostream>
#include <sstream>


using namespace std;

Grafo lerArquivoSTP(const string &caminhoArquivo) {
  Grafo g; // Inicia um grafo vazio (construtor cuida de numNos = 0)

  ifstream arquivo(caminhoArquivo);
  if (!arquivo.is_open()) {
    cerr << "Erro critico: Nao foi possivel abrir o arquivo: " << caminhoArquivo
         << endl;
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

    // ve se é uma seção
    if (token == "SECTION" || token == "Section" || token == "section") {
      ss >> secaoAtual;
      continue;
    }
    if (token == "END" || token == "End" || token == "end") {
      secaoAtual = "";
      continue;
    }

    // Processa a seção do Grafo
    if (secaoAtual == "Graph" || secaoAtual == "GRAPH" ||
        secaoAtual == "graph") {
      if (token == "Nodes" || token == "NODES" || token == "nodes") {
        int n;
        ss >> n;
        g.inicializar(n, 0); // inicializa com n
      } else if (token == "Edges" || token == "EDGES" || token == "edges") {
        int m;
        ss >> m;
        // le a quantidade declarada, a alocação ocorre ao ler os "E"
      } else if (token == "E" || token == "e") {
        int u, v;
        double peso;
        ss >> u >> v >> peso;

        // ve se os nós foram inicializados
        if (g.getNumNos() == 0 || u > g.getNumNos() || v > g.getNumNos()) {
          cerr << "[ERRO CRITICO] Tentativa de ler aresta (" << u << " -> " << v
               << ") mas o tamanho do grafo nao foi definido corretamente no "
                  "arquivo!"
               << endl;

          Grafo grafoFalho;
          return grafoFalho; // força indicar falha retornando grafo zerado
        }

        g.adicionarAresta(u, v, peso); // uso do método encapsulado
      }
    }

    // olha os vertices terminais
    else if (secaoAtual == "Terminals" || secaoAtual == "TERMINALS" ||
             secaoAtual == "terminals") {
      if (token == "TP" || token == "tp") {
        int no;
        double premio;
        ss >> no >> premio;

        g.setPremio(no, premio); // define o premio
      }
    }
  }

  arquivo.close();
  return g;
}