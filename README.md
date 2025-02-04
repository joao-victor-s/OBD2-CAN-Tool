# **Comunicação OBD-II via CAN**

## **Visão Geral**
Este projeto implementa um sistema de comunicação OBD-II utilizando a rede CAN (Controller Area Network), permitindo a troca de informações entre um leitor OBD-II e uma ECU simulada. O objetivo é testar e validar a comunicação CAN sem a necessidade de um veículo real, utilizando um ambiente virtual.

A aplicação é dividida em três componentes principais:
1. **CANManager** – Gerencia a comunicação CAN, enviando e recebendo mensagens.
2. **OBDReader** – Solicita informações da ECU simulada via protocolo OBD-II.
3. **Simulator** – Simula uma ECU, respondendo às mensagens enviadas pelo OBDReader.

---

## **Estrutura do Projeto**
```
OBD/
│── include/                  # Arquivos de cabeçalho
│   ├── CANManager.h          # Gerenciamento da comunicação CAN
│   ├── OBDReader.h           # Leitura de dados OBD-II
│── src/                      # Implementação dos módulos principais
│   ├── CANManager.cpp        # Envio e recepção de mensagens CAN
│   ├── OBDReader.cpp         # Interação com a ECU via OBD-II
│── simulator/                # Código do simulador de ECU
│   ├── simulator.cpp         # Simulação de respostas OBD-II
│── CMakeLists.txt            # Arquivo de configuração do CMake
│── main.cpp                  # Código principal para leitura OBD-II
│── README.md                 # Documentação do projeto
```

---

## **Como Configurar e Executar**
### **1. Instalar Dependências**
Antes de rodar o projeto, certifique-se de ter instalado:
- **CMake** (`sudo apt install cmake`)
- **G++** (`sudo apt install g++`)
- **SocketCAN** (`sudo apt install can-utils`)

### **2. Criar uma Interface CAN Virtual**
Para simular um ambiente CAN no Linux:
```bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```
Isso criará uma interface chamada **vcan0**, que será usada para a comunicação CAN virtual.

### **3. Compilar o Projeto**
No terminal, execute:
```bash
mkdir build && cd build
cmake ..
make
```
Isso gerará os executáveis necessários para rodar os testes.

### **4. Executar a Simulação**
1. **Rodar o simulador de ECU**:
```bash
./simulator
```
Isso inicia um sistema que responde às mensagens OBD-II simulando uma ECU real.

2. **Rodar o leitor OBD-II**:
```bash
./OBD
```
Aqui, o sistema OBDReader enviará requisições à ECU simulada e processará as respostas.

---

## **Funcionamento do Projeto**
### **CANManager**
- Responsável por inicializar a interface CAN, enviar e receber mensagens.
- Estabelece um **socket CAN** para comunicação entre dispositivos.
- Gerencia **timeouts** para garantir respostas dentro do tempo esperado.

### **OBDReader**
- Solicita **PIDs OBD-II** (Parâmetros de Diagnóstico) para obter dados do veículo.
- Verifica se a resposta vem em um **Single Frame** (resposta única) ou em **First Frame + Consecutive Frames** (respostas fragmentadas).
- Processa as respostas da ECU simulada e retorna os valores solicitados.

### **Simulator**
- Atua como uma ECU, escutando as mensagens recebidas via **CAN** e respondendo conforme o protocolo OBD-II.
- Suporta dois tipos de resposta:
  - **Single Frame (SF):** Quando a resposta cabe em um único pacote CAN.
  - **First Frame (FF) + Consecutive Frames (CF):** Para respostas que excedem o tamanho de um único frame, exigindo múltiplas mensagens.

---

## **Fluxo de Comunicação**
1. O **OBDReader** envia uma solicitação CAN para a ECU simulada (**Simulator**).  
2. O **Simulator** processa a requisição e responde com os dados solicitados.  
3. Se os dados couberem em um único frame CAN, o **Simulator** envia um **Single Frame (SF)**.  
4. Se os dados forem grandes, o **Simulator** inicia um **First Frame (FF)** e espera um **Flow Control (FC)** do OBDReader.  
5. Após receber o **Flow Control**, o **Simulator** envia os **Consecutive Frames (CF)** até completar a resposta.  
6. O **OBDReader** lê os dados e os exibe no terminal.  


## Futuras Melhorias

Este projeto está em desenvolvimento e novas funcionalidades estão sendo planejadas para aumentar a eficiência da comunicação CAN e a robustez do leitor OBD-II. Algumas melhorias previstas incluem:

- **Implementação de multithreading** para leitura e escrita simultânea de mensagens CAN, permitindo uma comunicação mais eficiente sem bloqueios.  
- **Otimização do gerenciamento de buffers CAN**, reduzindo a latência na recepção e envio de pacotes.  
- **Melhoria na detecção e tratamento de erros**, garantindo que mensagens inválidas ou com falha de sequência sejam corretamente descartadas e retransmitidas quando necessário.  
- **Implementação de logs e debugging avançado**, facilitando a análise de falhas no fluxo de mensagens CAN.  
- **Suporte a mais PIDs OBD-II**, expandindo a compatibilidade com diferentes ECUs e veículos.  
