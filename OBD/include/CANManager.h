#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

#include <string>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdint>
using namespace std;

//Responsabilidade: Gerenciar a comunicacao com a interface CAN (socket CAN).

class CANManager {
private:
    int server_socket;              // Descritor do socket CAN
    string interface_name;          // Nome da interface CAN 
    struct sockaddr_can addr;       // Struct que armazena o endereco do socket CAN
    struct ifreq ifr;               // Struct para operacoes ioctl

public:
    CANManager(const string& can_interface);
    ~CANManager();

    bool initialize();
    can_frame readCANMessages(int time);
    bool sendCANFrame(struct can_frame frame) const;

    int getSocket() const;
};

#endif
