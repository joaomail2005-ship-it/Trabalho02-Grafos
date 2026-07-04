Estrutura do Projeto

* `include/`: Arquivos de cabeçalho (`.hpp`). Contém as estruturas de dados (Grafo, Aresta, Solução) e as declarações das funções.
* `src/`: Arquivos de código-fonte (`.cpp`). Contém o *parser* das instâncias (`leitor.cpp`), a função principal (`main.cpp`) e as futuras implementações dos algoritmos.
* `instances/`: Diretório onde os arquivos de instâncias `.stp` devem ser alocados.
* `build/`: Diretório gerado automaticamente para arquivos de compilação e o executável final.

Para compilar :
* **CMake** Versão 3.10 ou superior
* Compilador C++ com suporte a C++17 (ex: **GCC/MinGW** no Windows ou **G++** no Linux).

Como Compilar
### No Windows (via PowerShell com MinGW)
1. Crie e acesse a pasta build: `mkdir build; cd build`
2. Configure o CMake para MinGW: `cmake -G "MinGW Makefiles" ..`
3. Compile o projeto: `cmake --build .`

### No Linux / WSL
1. Crie e acesse a pasta build: `mkdir build && cd build`
2. Configure o CMake: `cmake ..`
3. Compile o projeto: `make` (ou `cmake --build .`)

## 🏃 Como Executar

O programa recebe o caminho do arquivo de instância `.stp` como argumento via linha de comando.

**No Windows:**
```powershell
.\pcstp_solver.exe ..\instances\NOME_DO_ARQUIVO.stp

#SCRIPT PARA RODAR VARIAS INSTANCIAS E ESCOLHER PASTAS PELO TERMINAL
# =======================================================
# SCRIPT DE EXECUÇÃO EM LOTE - PCSTP SOLVER
# =======================================================

# 1. Caminho do seu executável
$exe = ".\pcstp_solver.exe"

# 2. Caminho base onde ficam as instâncias
$pastaBase = "..\instances"

# 3. Nome das subpastas que você quer testar 
# (Se estiver tudo direto na pasta 'instances', deixe apenas "")
$subpastas = @("", "grupoB", "grupoC") 

# 4. Quantidade de instâncias para rodar de CADA pasta
$qtdPorPasta = 5 

# 5. Parâmetros dos Algoritmos
$alpha = "0.3"
$iteracoes = "500"
$tamanhoBloco = "50"

Write-Host "Iniciando a bateria de testes..." -ForegroundColor Green

# Loop pelas subpastas
foreach ($pasta in $subpastas) {
    # Monta o caminho da pasta atual
    $caminhoBusca = Join-Path $pastaBase $pasta
    
    # Verifica se a pasta existe antes de tentar ler
    if (Test-Path $caminhoBusca) {
        
        # Pega os arquivos .stp e limita a quantidade definida na variável $qtdPorPasta
        $instancias = Get-ChildItem -Path $caminhoBusca -Filter "*.stp" | Select-Object -First $qtdPorPasta
        
        foreach ($arq in $instancias) {
            $caminhoStp = $arq.FullName
            Write-Host "======================================" -ForegroundColor Cyan
            Write-Host "Testando Instância: $($arq.Name)" -ForegroundColor Yellow
            Write-Host "======================================" -ForegroundColor Cyan
            
            # =======================================================
            # ESCOLHA QUAL ALGORITMO RODAR
            # (Descomente removendo o '#' da linha que quer executar)
            # =======================================================
            
            # MODO 1: GULOSO
            # & $exe $caminhoStp "guloso"
            
            # MODO 2: RANDOMIZADO
            # & $exe $caminhoStp "randomizado" $alpha $iteracoes
            
            # MODO 3: REATIVO
            & $exe $caminhoStp "reativo" $iteracoes $tamanhoBloco
            
        }
    } else {
        Write-Host "Aviso: Pasta '$caminhoBusca' não encontrada. Pulando..." -ForegroundColor Red
    }
}

Write-Host "Bateria de testes finalizada! Verifique o seu 'resultados.csv'." -ForegroundColor Green
