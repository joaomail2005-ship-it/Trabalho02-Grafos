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
