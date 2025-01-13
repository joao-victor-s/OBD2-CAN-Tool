cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(OBD)

# Definir o padrão C++17
set(CMAKE_CXX_STANDARD 17)

# Adicionar os diretórios com os cabeçalhos
include_directories(
    ${PROJECT_SOURCE_DIR}/CANManager
)

# Adicionar os arquivos fonte
add_executable(OBD
    main.cpp
    CANManager/CANManager.cpp
)
