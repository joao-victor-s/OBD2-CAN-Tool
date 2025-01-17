#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "CANManager.h"
#include "OBDReader.h"

using namespace std;

#define CAN_INTERFACE "vcan0" // nome da interface CAN virtual

int main()
{

	CANManager manager(CAN_INTERFACE);

	manager.initialize();
	cout << "Socket CAN: " << manager.getSocket() << endl;

	OBDReader obd(&manager);
	obd.begin(&manager);
	obd.vinRead(&manager);
	

	return 0;

}

