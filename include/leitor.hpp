#ifndef LEITOR_HPP
#define LEITOR_HPP
using namespace std;
#include <string>
#include "grafo.hpp" // Inclui as suas estruturas (Grafo, Aresta, etc.)

// Declaração da função que vai ler o arquivo e retornar o objeto Grafo populado
Grafo lerArquivoSTP(const std::string& caminhoArquivo);

#endif // LEITOR_HPP