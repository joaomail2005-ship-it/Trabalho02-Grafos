#include <iostream>
#include <algorithm>
#include "leitor.hpp"
#include "solucao.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <caminho_da_instancia.stp>" << endl;
        return 1;
    }

    string caminhoArquivo = argv[1];
    cout << "Carregando a instancia: " << caminhoArquivo << "..." << endl;

    Grafo g = lerArquivoSTP(caminhoArquivo);

    if (g.numNos == 0) {
        cerr << "Erro: O grafo nao foi carregado ou esta vazio." << endl;
        return 1;
    }

    cout << "\n====================================\n";
    cout << "      TESTE DE LEITURA DO GRAFO      \n";
    cout << "====================================\n";
    cout << "Nos declarados no arquivo: " << g.numNos << "\n";
    cout << "Arestas declaradas no arquivo: " << g.numArestas << "\n";

    int qtdTerminais = 0;
    for (int i = 1; i <= g.numNos; ++i) {
        if (g.premios[i] > 0.0) qtdTerminais++;
    }
    
    cout << "Total de nos Terminais (com premio > 0): " << qtdTerminais << "\n";
    cout << "Total de nos Nao-Terminais (Steiner): " << (g.numNos - qtdTerminais) << "\n";

    cout << "\n[Amostra] Estrutura dos Primeiros 5 Nos:\n";
    int limiteAmostra = min(g.numNos, 5);
    for (int i = 1; i <= limiteAmostra; ++i) {
        cout << "No " << i << " -> Premio: " << g.premios[i] 
             << " | Grau (Arestas conectadas): " << g.adj[i].size() << "\n";
    }
    cout << "====================================\n";
    cout << "Leitura validada com sucesso!\n";


    // --- CHAMADA DO SOLVER ---
    Solucao sol = heuristicaSimples(g);

    cout << "\n====================================\n";
    cout << "          RESULTADO FINAL           \n";
    cout << "====================================\n";
    cout << "Tamanho da Arvore (Nos): " << sol.nosUsados.size() << "\n";
    cout << "Arestas Utilizadas: " << sol.arestasUsadas.size() << "\n";
    cout << "Custo Total (Funcao Objetivo): " << sol.custoTotal << "\n";
    cout << "Tempo de Execucao: " << sol.tempo << " s\n";
    cout << "====================================\n";

    return 0;
}