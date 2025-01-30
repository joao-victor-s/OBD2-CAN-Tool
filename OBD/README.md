# **Comunica��o OBD-II via CAN**

## **Vis�o Geral**
Este projeto implementa um sistema de comunica��o OBD-II utilizando a rede CAN (Controller Area Network), permitindo a troca de informa��es entre um leitor OBD-II e uma ECU simulada. O objetivo � testar e validar a comunica��o CAN sem a necessidade de um ve�culo real, utilizando um ambiente virtual.

A aplica��o � dividida em tr�s componentes principais:
1. **CANManager** � Gerencia a comunica��o CAN, enviando e recebendo mensagens.
2. **OBDReader** � Solicita informa��es da ECU simulada via protocolo OBD-II.
3. **Simulator** � Simula uma ECU, respondendo �s mensagens enviadas pelo OBDReader.

---

## **Estrutura do Projeto**
```
OBD/
??? include/                  # Arquivos de cabe�alho
?   ??? CANManager.h          # Gerenciamento da comunica��o CAN
?   ??? OBDReader.h           # Leitura de dados OBD-II
??? src/                      # Implementa��o dos m�dulos principais
?   ??? CANManager.cpp        # Envio e recep��o de mensagens CAN
?   ??? OBDReader.cpp         # Intera��o com a ECU via OBD-II
??? simulator/                # C�digo do simulador de ECU
?   ??? simulator.cpp         # Simula��o de respostas OBD-II
??? CMakeLists.txt            # Arquivo de configura��o do CMake
??? main.cpp                  # C�digo principal para leitura OBD-II
??? README.md                 # Documenta��o do projeto
```

---

## **Como Configurar e Executar**
### **1. Instalar Depend�ncias**
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
Isso criar� uma interface chamada **vcan0**, que ser� usada para a comunica��o CAN virtual.

### **3. Compilar o Projeto**
No terminal, execute:
```bash
mkdir build && cd build
cmake ..
make
```
Isso gerar� os execut�veis necess�rios para rodar os testes.

### **4. Executar a Simula��o**
1. **Rodar o simulador de ECU**:
```bash
./simulator
```
Isso inicia um sistema que responde �s mensagens OBD-II simulando uma ECU real.

2. **Rodar o leitor OBD-II**:
```bash
./OBD
```
Aqui, o sistema OBDReader enviar� requisi��es � ECU simulada e processar� as respostas.

---

## **Funcionamento do Projeto**
### **CANManager**
- Respons�vel por inicializar a interface CAN, enviar e receber mensagens.
- Estabelece um **socket CAN** para comunica��o entre dispositivos.
- Gerencia **timeouts** para garantir respostas dentro do tempo esperado.

### **OBDReader**
- Solicita **PIDs OBD-II** (Par�metros de Diagn�stico) para obter dados do ve�culo.
- Verifica se a resposta vem em um **Single Frame** (resposta �nica) ou em **First Frame + Consecutive Frames** (respostas fragmentadas).
- Processa as respostas da ECU simulada e retorna os valores solicitados.

### **Simulator**
- Atua como uma ECU, escutando as mensagens recebidas via **CAN** e respondendo conforme o protocolo OBD-II.
- Suporta dois tipos de resposta:
  - **Single Frame (SF):** Quando a resposta cabe em um �nico pacote CAN.
  - **First Frame (FF) + Consecutive Frames (CF):** Para respostas que excedem o tamanho de um �nico frame, exigindo m�ltiplas mensagens.

---

## **Fluxo de Comunica��o**
1. O **OBDReader** envia uma solicita��o CAN para a ECU simulada (**Simulator**).  
2. O **Simulator** processa a requisi��o e responde com os dados solicitados.  
3. Se os dados couberem em um �nico frame CAN, o **Simulator** envia um **Single Frame (SF)**.  
4. Se os dados forem grandes, o **Simulator** inicia um **First Frame (FF)** e espera um **Flow Control (FC)** do OBDReader.  
5. Ap�s receber o **Flow Control**, o **Simulator** envia os **Consecutive Frames (CF)** at� completar a resposta.  
6. O **OBDReader** l� os dados e os exibe no terminal.  

