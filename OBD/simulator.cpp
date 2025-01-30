#include "CANManager.h"
#include <iostream>
#include <string>

#define CAN_INTERFACE "vcan0" // Nome da interface CAN virtual

using namespace std;

int main() {

	CANManager simulator(CAN_INTERFACE);
	simulator.initialize();


	string user_choice;
	cout << "Simular Single Frame (SF) ou First Frame (FF)? ";
	cin >> user_choice;  // Corrigido erro na entrada de dados
	
	struct can_frame recv_simulator = simulator.readCANMessages(10000);;

	if (recv_simulator.can_id != 0x7DF) {
		cerr << "Requisição OBD-II invalida." << endl;
		return -1;
	}

	if (user_choice == "SF") {
		struct can_frame send_sf;

		send_sf.can_id = 0x7E8;
		send_sf.can_dlc = 8;
		send_sf.data[0] = 0x07;  // Single Frame indicando resposta única
		send_sf.data[1] = (recv_simulator.data[1] | 0x40);  // Resposta ao modo recebido
		send_sf.data[2] = recv_simulator.data[2];  // PID solicitado
		send_sf.data[3] = 0x44;  // Dado fictício
		send_sf.data[4] = 0x12;
		send_sf.data[5] = 0x34;
		send_sf.data[6] = 0x56;
		send_sf.data[7] = 0x78;

		simulator.sendCANFrame(send_sf);
		cout << "Single Frame enviado." << endl;
	}

	else if (user_choice == "FF") {
		struct can_frame send_ff;

		send_ff.can_id = 0x7E8;
		send_ff.can_dlc = 8;  // Tamanho máximo permitido para CAN clássico
		send_ff.data[0] = 0x10; // First Frame
		send_ff.data[1] = 0x12; // Tamanho total da resposta
		send_ff.data[2] = 0x35;
		send_ff.data[3] = 0x44;
		send_ff.data[4] = 0x65;
		send_ff.data[5] = 0x78;
		send_ff.data[6] = 0x45;
		send_ff.data[7] = 0x12;

		simulator.sendCANFrame(send_ff);
		cout << "First Frame enviado." << endl;

		// Aguarda Flow Control
		struct can_frame flow_control = simulator.readCANMessages(5000);
		if (flow_control.data[0] != 0x30) {
			cerr << "Flow Control nao recebido." << endl;
			return -1;
		}

		cout << "Flow Control recebido." << endl;

		// Envia o primeiro Consecutive Frame
		struct can_frame consecutive_frame;
		consecutive_frame.can_id = 0x7E8;
		consecutive_frame.can_dlc = 8;
		consecutive_frame.data[0] = 0x21;  // Primeiro CF (0x21)
		consecutive_frame.data[1] = 0x9A;
		consecutive_frame.data[2] = 0xBC;
		consecutive_frame.data[3] = 0xDE;
		consecutive_frame.data[4] = 0xF1;
		consecutive_frame.data[5] = 0x23;
		consecutive_frame.data[6] = 0x45;
		consecutive_frame.data[7] = 0x67;

		simulator.sendCANFrame(consecutive_frame);

		// Envia o segundo Consecutive Frame
		struct can_frame consecutive_frame_2;
		consecutive_frame_2.can_id = 0x7E8;
		consecutive_frame_2.can_dlc = 8;
		consecutive_frame_2.data[0] = 0x22;  // Segundo CF (0x22)
		consecutive_frame_2.data[1] = 0x89;
		consecutive_frame_2.data[2] = 0xAB;
		consecutive_frame_2.data[3] = 0xCD;
		consecutive_frame_2.data[4] = 0xEF;
		consecutive_frame_2.data[5] = 0x01;
		consecutive_frame_2.data[6] = 0x02;
		consecutive_frame_2.data[7] = 0x03;

		simulator.sendCANFrame(consecutive_frame_2);
		cout << "Consecutive Frames enviados." << endl;
	}

	else {
		cout << "Opcao invalida. Digite 'SF' ou 'FF'." << endl;
	}

	return 0;
}
