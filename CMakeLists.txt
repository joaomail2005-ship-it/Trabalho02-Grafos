cmake_minimum_required(VERSION 3.10)

# Nome do projeto e linguagem
project(PCSTP_Solver CXX)

# Exigir C++17 (muito útil para trabalhar com caminhos de arquivos)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- SOLUÇÃO PARA O ERRO 0xC000007B ---
# Se estiver no Windows, força o compilador a embutir as DLLs do C++ dentro do .exe
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()

# Mostra onde estão os cabeçalhos (.hpp)
include_directories(include)

# Diz ao CMake para compilar todos os .cpp da pasta src e gerar o executável "pcstp_solver"
file(GLOB SOURCES "src/*.cpp")
add_executable(pcstp_solver ${SOURCES})