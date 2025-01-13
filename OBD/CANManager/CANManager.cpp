#include "CANManager.h"

CANManager::CANManager(const std::string& can_interface) : interface_name(can_interface), server_socket(-1) {}

CANManager::~CANManager() {
    if (server_socket > 0) {
        close(server_socket);
        cout << "Socket fechado." << endl;
    }
}

bool CANManager::initialize() 
{
    // Cria SocketCAN
    server_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (server_socket < 0) {
        cerr << "Erro ao criar o socket CAN." << endl;
        return false;
    }

    // Configura o nome da interface
    strncpy(ifr.ifr_name, interface_name.c_str(), IFNAMSIZ - 1);
    if (ioctl(server_socket, SIOCGIFINDEX, &ifr) < 0) {
        cerr << "Erro ao obter o índice da interface CAN." << endl;
        return false;
    }

    // Obtem o indice da interface CAN
    if (ioctl(server_socket, SIOCGIFINDEX, &ifr) < 0) {
        cerr << "Erro ao obter o índice da interface CAN." << endl;
        close(server_socket);
        server_socket = -1;
        return false;
    }

    // Configura o endereco do Socket
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // Bind -> Associa socket a interface can
    if (bind(server_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        cerr << "Erro ao associar o socket à interface CAN." << endl;
        return false;
    }

    cout << "CAN Inicializada com sucesso " << interface_name << endl;
    return true;
}

int CANManager::getSocket() const {
    return server_socket;
}

