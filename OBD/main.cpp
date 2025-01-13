#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "CANManager/CANManager.h"

using namespace std;

#define CAN_INTERFACE "vcan0" // nome da interface CAN virtual

int main()
{
	CANManager manager(CAN_INTERFACE);

	manager.initialize();
	cout << manager.getSocket() << endl;
	return 0;

}


//int main() {
//    int server_socket; //valor int do server 
//    struct sockaddr_can addr; //struct que armazena o endereco com os dados do socket
//    struct ifreq ifr; //
//
//    // Cria o socket CAN
//    server_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW); //usa o socket CAN_RAW -> monitoramento software automotivo
//    if (server_socket < 0) {
//        perror("Erro ao criar o socket CAN");
//        return 1;
//    }
//
//    // Configura a interface CAN
//    strcpy(ifr.ifr_name, CAN_INTERFACE);
//    if (ioctl(server_socket, SIOCGIFINDEX, &ifr) < 0) //ioctl -> config dispositivos linux (net interface) 
//    {   //SIOCGIFINDEX: Comando para obter o indice da interface de rede
//        // &ifr ponteiro para struct ifr onde o indice sera armazenado
//        perror("Erro ao configurar a interface CAN"); //perror exibe o motivo da falha
//        close(server_socket);
//        return 1;
//    }
//
//    addr.can_family = AF_CAN; //define ipv4 para can
//    addr.can_ifindex = ifr.ifr_ifindex; //passa o indice da estrutura can associada para o endereco
//
//    // Vincular o socket a interface CAN usando o bind
//    if (bind(server_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
//        perror("Erro ao vincular o socket CAN");
//        close(server_socket);
//        return 1;
//    }
//
//    cout << "Servidor CAN escutando na interface " << CAN_INTERFACE << "..." << endl;
//
//    // Mensagens CAN para enviar
//    struct can_frame frame;
//    frame.can_id = 0x123; // ID CAN
//    frame.can_dlc = 8;    // Comprimento dos dados (maximo 8 bytes)
//    memcpy(frame.data, "\x11\x22\x33\x44\x55\x66\x77\x88", 8);
//
//    // Enviar a mensagem CAN
//    if (write(server_socket, &frame, sizeof(frame)) < 0) {
//        perror("Erro ao enviar mensagem CAN");
//        close(server_socket);
//        return 1;
//    }
//    cout << "Mensagem CAN enviada: ID=0x123 Dados=11 22 33 44 55 66 77 88" << endl;
//
//    // Receber mensagens CAN
//    while (true) {
//        struct can_frame recv_frame;
//        int nbytes = read(server_socket, &recv_frame, sizeof(recv_frame));
//        if (nbytes < 0) {
//            perror("Erro ao receber mensagem CAN");
//            break;
//        }
//
//        // Exibir a mensagem recebida
//        cout << "Mensagem CAN recebida: ID=0x" << hex << recv_frame.can_id
//            << " DLC=" << dec << int(recv_frame.can_dlc) << " Dados=";
//
//        for (int i = 0; i < recv_frame.can_dlc; ++i) {
//            cout << hex << int(recv_frame.data[i]) << " ";
//        }
//        cout << dec << endl;
//    }
//
//    // Fechar o socket
//    close(server_socket);
//
//    cout << "Conexão CAN encerrada." << endl;
//    return 0;
//}
