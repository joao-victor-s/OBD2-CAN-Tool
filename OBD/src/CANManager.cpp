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

can_frame CANManager::readCANMessages(int time) {
    struct can_frame recv_frame;
    memset(&recv_frame, 0, sizeof(recv_frame));

    socklen_t len = sizeof(addr);

    //// Aplicacao de timeout para monitorar leitura CAN
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(server_socket, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = time / 1000;
    timeout.tv_usec = (time % 1000) * 1000;   

    // select() para monitorar socket para aplicar timeout
    int ret = select(server_socket + 1, &read_fds, nullptr, nullptr, &timeout);

    if (ret == 0)
    {
        cerr << "Timeout atingido, nenhuma mensagem CAN detectada." << endl;
        return recv_frame;
    }

    int nbytes = recvfrom(server_socket, &recv_frame, sizeof(struct can_frame), 0, (struct sockaddr*)&addr, &len);

    ifr.ifr_ifindex = addr.can_ifindex;
    ioctl(server_socket, SIOCGIFNAME, &ifr);
    cout << "CAN Frame recebido da interface " << ifr.ifr_name << endl;

    if (nbytes <= 0) {
        cerr << "Erro ao receber frame CAN." << endl;
    }
    else {
        cout << "Frame CAN recebido: ID=0x" << hex << recv_frame.can_id
            << " DLC = " << dec << int(recv_frame.can_dlc) << " Dados = ";
        for (int i = 0; i < recv_frame.can_dlc; ++i) {
            cout << hex << int(recv_frame.data[i]) << " ";
        }
        cout << dec << endl;
    }

    return recv_frame;

}

bool CANManager::sendCANFrame(struct can_frame frame) const
{    
    socklen_t addr_len = sizeof(addr);

    // Aplicacao de timeout para monitorar leitura CAN
    fd_set write_fds;
    FD_ZERO(&write_fds);
    FD_SET(server_socket, &write_fds);

    struct timeval timeout;
    timeout.tv_sec = 6 / 1000;
    timeout.tv_usec = 0;

    // Monitoramento de Socket para avaliar disponibilidade de escrita
    int ready = select(server_socket + 1, nullptr, &write_fds, nullptr, &timeout);

    if (ready == 0) {
        cout << "Timeout 60ms atingido para send." << endl;
        return false;
    }

    int nbytes = sendto(server_socket, &frame, sizeof(struct can_frame),
        0, (struct sockaddr*)&addr, sizeof(addr));
    
    if (nbytes != sizeof(frame)) {
        cerr << "Erro ao enviar frame CAN." << endl;
        return false;
    }

    return true;
}




