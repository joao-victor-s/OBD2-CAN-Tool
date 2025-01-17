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

bool CANManager::readCANMessages(int time) {
    
    struct can_frame recv_frame;
    socklen_t len = sizeof(addr);

    // Aplicacao de timeout para monitorar leitura CAN
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
        return false;
    }


    int nbytes = recvfrom(server_socket, &recv_frame, sizeof(struct can_frame), 0, (struct sockaddr*)&addr, &len);

    ifr.ifr_ifindex = addr.can_ifindex;
    ioctl(server_socket, SIOCGIFNAME, &ifr);
    cout << "CAN Frame recebido da interface " << ifr.ifr_name << endl;

   if (nbytes < 0) {
        perror("Erro no select()");
   }
   else if (nbytes == 0) {
        cout << "Aguardando mensagens CAN..." << endl;
    }

    // Exibir a mensagem recebida
    cout << "Mensagem CAN recebida: ID=0x" << hex << recv_frame.can_id
         << " DLC=" << dec << int(recv_frame.can_dlc) << " Dados=";

    for (int i = 0; i < recv_frame.can_dlc; ++i) {
        cout << hex << int(recv_frame.data[i]) << " ";
    }

    cout << dec << endl;
    return true;

}

void CANManager::sendCANMessage(uint8_t mode, uint8_t pid, const char * buffer, size_t buffer_size) const
{
    
    struct can_frame send_can_frame;
    socklen_t addr_len = sizeof(addr);

    if (buffer_size > 5)
        buffer_size = 5;                                                        // Limitador do buffer para CAN Classic

    send_can_frame.can_id = 0x7DF;                                              // ID padrao para mensagens de solicitacao OBD-II
    send_can_frame.can_dlc = 3 + buffer_size;                                   // Data Length Code fixado em 8 para mensagens CAN Classic

    memset(send_can_frame.data, 0x00, sizeof(send_can_frame.data));             // Inicia os dados do frame zerados
    send_can_frame.data[0] = 2 + buffer_size;                                   // Primeiro byte indica o numero de dados (2 + tamanho do buffer)
    send_can_frame.data[1] = mode;                                              // modo de solicitacao PID
    send_can_frame.data[2] = pid;                                               // PID solicitado

    // Copia os dados adicionais do buffer para os campos adicionais
    memcpy(&send_can_frame.data[3], buffer, buffer_size);

    // Envia mensagens CAN
    int nbytes = sendto(server_socket, &send_can_frame, sizeof(struct can_frame),
        0, (struct sockaddr*)&addr, sizeof(addr));
        

    for (int i = 0; i < send_can_frame.len; ++i) {
        cout << hex << int(send_can_frame.data[i]) << " ";
    }

    cout << dec << endl;
    
}

void CANManager::sendCANFDMessage(uint8_t mode, uint8_t pid, const char* buffer, size_t buffer_size) const
{
    // Usa-se CAN FD para lidar com payloads maiores que 8 bytes
    struct canfd_frame send_can_fd_frame;
    socklen_t addr_len = sizeof(addr);

    send_can_fd_frame.can_id = 0x7DF;                                           // ID padrao para mensagens de solicitacao OBD-II

    // Limitador de tamanho do payload
    if (buffer_size < 64)
        send_can_fd_frame.len = buffer_size;                                     
    else
        send_can_fd_frame.len = 64;

    memset(send_can_fd_frame.data, 0x00, sizeof(send_can_fd_frame.data));       // Inicia os dados do frame zerados
    send_can_fd_frame.data[0] = 2 + buffer_size;                                // Primeiro byte indica o numero de dados (2 + tamanho do buffer)
    send_can_fd_frame.data[1] = mode;                                           // Modo de solicitacao PID
    send_can_fd_frame.data[2] = pid;                                            // PID solicitado

    // Copia os dados adicionais do buffer para os campos adicionais
    memcpy(&send_can_fd_frame.data[3], buffer, buffer_size);

    // Envia mensagens CAN
    int nbytes = sendto(server_socket, &send_can_fd_frame, sizeof(struct can_frame),
        0, (struct sockaddr*)&addr, sizeof(addr));


    for (int i = 0; i < send_can_fd_frame.len; ++i) {
        cout << hex << int(send_can_fd_frame.data[i]) << " ";
    }

    cout << dec << endl;
    
}


