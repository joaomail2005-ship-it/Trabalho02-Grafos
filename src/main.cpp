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

    // Validação usando o Getter
    if (g.getNumNos() == 0) {
        cerr << "Erro: O grafo nao foi carregado ou esta vazio." << endl;
        return 1;
    }

    cout << "\n====================================\n";
    cout << "      TESTE DE LEITURA DO GRAFO      \n";
    cout << "====================================\n";
    cout << "Nos declarados no arquivo: " << g.getNumNos() << "\n";
    cout << "Arestas declaradas no arquivo: " << g.getNumArestas() << "\n";

    // Criamos uma referência constante para o vetor de nós para não precisar 
    // chamar o g.getNos() a cada iteração do loop
    const auto& nos = g.getNos();
    int qtdTerminais = 0;
    
    // Arrays no arquivo .stp começam no índice 1
    for (int i = 1; i <= g.getNumNos(); ++i) {
        if (nos[i].premio > 0.0) qtdTerminais++;
    }
    
    cout << "Total de nos Terminais (com premio > 0): " << qtdTerminais << "\n";
    cout << "Total de nos Nao-Terminais (Steiner): " << (g.getNumNos() - qtdTerminais) << "\n";

    cout << "\n[Amostra] Estrutura dos Primeiros 5 Nos:\n";
    int limiteAmostra = min(g.getNumNos(), 5);
    for (int i = 1; i <= limiteAmostra; ++i) {
        // Usa o vetor constante "nos" e acessa a lista de adjacência pelo getter
        cout << "No " << i << " -> Premio: " << nos[i].premio 
             << " | Grau (Arestas conectadas): " << g.getAdj()[i].size() << "\n";
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