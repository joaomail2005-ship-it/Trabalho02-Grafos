#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>
#include "leitor.hpp"
#include "solucao.hpp"

using namespace std;
using namespace std::chrono;

// Função auxiliar para pegar data e hora atual em formato string
string getDataHoraAtual() {
    auto t = system_clock::to_time_t(system_clock::now());
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&t));
    return string(buffer);
}

// Função auxiliar para imprimir instruções de uso
void imprimirUso(const char* nomePrograma) {
    cout << "Uso correto via linha de comando:\n";
    cout << nomePrograma << " <caminho.stp> guloso [semente]\n";
    cout << nomePrograma << " <caminho.stp> randomizado <alpha> <iteracoes> [semente]\n";
    cout << nomePrograma << " <caminho.stp> reativo <iteracoes> <bloco> [semente]\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        imprimirUso(argv[0]);
        return 1;
    }

    string caminhoArquivo = argv[1];
    string modo = argv[2];
    
    Grafo g = lerArquivoSTP(caminhoArquivo);
    if (g.getNumNos() == 0) {
        cerr << "Erro: O grafo nao foi carregado ou esta vazio.\n";
        return 1;
    }

    // --- CONFIGURAÇÃO DA SEMENTE (Única em todo o código) ---
    unsigned int semente;
    // Tenta ler a semente como último argumento opcional
    if (modo == "guloso" && argc == 4) semente = stoi(argv[3]);
    else if (modo == "randomizado" && argc == 6) semente = stoi(argv[5]);
    else if (modo == "reativo" && argc == 6) semente = stoi(argv[5]);
    else {
        // Se não foi informada, gera pela data/hora atual
        semente = static_cast<unsigned int>(steady_clock::now().time_since_epoch().count());
    }
    
    mt19937 rngMain(semente);
    cout << "Semente de Randomizacao: " << semente << "\n";

    Solucao melhor_global;
    melhor_global.custoTotal = 999999999.0; // Inicia com infinito
    
    int iteracoes_totais = 1;
    double alpha_usado = 0.0; // Padrão guloso
    int bloco = 0;
    
    auto tempo_inicio = high_resolution_clock::now();

    // ==========================================
    // SELEÇÃO DO ALGORITMO
    // ==========================================
    if (modo == "guloso") {
        cout << "Executando Heuristica Gulosa (Alpha = 0.0)...\n";
        alpha_usado = 0.0;
        melhor_global = heuristicaSimples(g, alpha_usado, rngMain);
    } 
    else if (modo == "randomizado") {
        if (argc < 5) { cout << "Erro: Faltam parametros.\n"; return 1; }
        
        alpha_usado = stod(argv[3]);
        iteracoes_totais = stoi(argv[4]);
        cout << "Executando Randomizado (Alpha: " << alpha_usado << " | Iteracoes: " << iteracoes_totais << ")...\n";

        for (int i = 0; i < iteracoes_totais; ++i) {
            Solucao sol = heuristicaSimples(g, alpha_usado, rngMain);
            if (sol.custoTotal < melhor_global.custoTotal) {
                melhor_global = sol;
            }
        }
    }
    else if (modo == "reativo") {
        if (argc < 5) { cout << "Erro: Faltam parametros.\n"; return 1; }
        
        iteracoes_totais = stoi(argv[3]);
        bloco = stoi(argv[4]);
        cout << "Executando Reativo (Iteracoes: " << iteracoes_totais << " | Bloco: " << bloco << ")...\n";

        vector<double> alphas = {0.1, 0.2, 0.3, 0.4, 0.5}; // Pool de Alphas
        vector<double> probabilidades(alphas.size(), 1.0 / alphas.size());
        vector<double> soma_custos(alphas.size(), 0.0);
        vector<int> vezes_usado(alphas.size(), 0);

        for (int i = 1; i <= iteracoes_totais; ++i) {
            discrete_distribution<int> roleta(probabilidades.begin(), probabilidades.end());
            int idx = roleta(rngMain);
            double alpha_atual = alphas[idx];

            Solucao sol = heuristicaSimples(g, alpha_atual, rngMain);
            
            if (sol.custoTotal < melhor_global.custoTotal) {
                melhor_global = sol;
                alpha_usado = alpha_atual; // Guarda o alpha que achou a melhor solução
            }

            soma_custos[idx] += sol.custoTotal;
            vezes_usado[idx]++;

            // Atualização do Reativo
            if (i % bloco == 0) {
                double soma_q = 0.0;
                vector<double> q(alphas.size(), 0.0);
                
                for (size_t j = 0; j < alphas.size(); ++j) {
                    if (vezes_usado[j] > 0) {
                        double media = soma_custos[j] / vezes_usado[j];
                        q[j] = 1.0 / media; // Qualidade é o inverso do custo
                    } else {
                        q[j] = 0.000001; // Valor residual
                    }
                    soma_q += q[j];
                }
                
                for (size_t j = 0; j < alphas.size(); ++j) {
                    probabilidades[j] = q[j] / soma_q;
                }

                fill(soma_custos.begin(), soma_custos.end(), 0.0);
                fill(vezes_usado.begin(), vezes_usado.end(), 0);
            }
        }
    } else {
        cout << "Modo desconhecido: " << modo << "\n";
        return 1;
    }

    auto tempo_fim = high_resolution_clock::now();
    double tempo_total = duration<double>(tempo_fim - tempo_inicio).count();

    // ==========================================
    // IMPRESSÃO DE RESULTADOS
    // ==========================================
    cout << "\n====================================\n";
    cout << "          RESULTADO FINAL           \n";
    cout << "====================================\n";
    cout << "Melhor Custo Encontrado: " << melhor_global.custoTotal << "\n";
    cout << "Tamanho da Arvore (Nos): " << melhor_global.nosUsados.size() << "\n";
    cout << "Arestas Utilizadas: " << melhor_global.arestasUsadas.size() << "\n";
    cout << "Tempo Total de Execucao: " << tempo_total << " s\n";
    
    // --- FORMATO CS ACADEMY ---
    cout << "\n[Formato CS Academy - Copie as linhas abaixo]\n";
    for (const auto& a : melhor_global.arestasUsadas) {
        cout << a.origem << " " << a.destino << " " << a.peso << "\n";
    }
    cout << "====================================\n";

    // ==========================================
    // ESCRITA NO ARQUIVO .CSV
    // ==========================================
    string arquivoCSV = "resultados.csv";
    ifstream testaAcesso(arquivoCSV);
    bool arquivoNovo = !testaAcesso.good(); // Verifica se o arquivo não existia
    testaAcesso.close();

    ofstream arquivoLog(arquivoCSV, ios::app); // Modo Append
    
    if (arquivoNovo) {
        // Cria o cabeçalho se o arquivo for novo
        arquivoLog << "Data_Hora,Instancia,Algoritmo,Alpha,Iteracoes,Tamanho_Bloco,Semente,Tempo_s,Melhor_Custo\n";
    }

    // Instância apenas pelo nome (remover pastas se necessário, aqui salva o caminho completo)
    arquivoLog << getDataHoraAtual() << ","
               << caminhoArquivo << ","
               << modo << ","
               << alpha_usado << ","
               << iteracoes_totais << ","
               << bloco << ","
               << semente << ","
               << fixed << setprecision(4) << tempo_total << ","
               << melhor_global.custoTotal << "\n";

    arquivoLog.close();
    cout << "Resultados salvos em " << arquivoCSV << " com sucesso!\n";

    return 0;
}