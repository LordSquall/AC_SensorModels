#ifndef _SENSOR_H_
#define _SENSOR_H_

/* System Includes */
#include <winsock2.h>

namespace AC_SensorModels
{
	class Sensor
	{
	public:
		Sensor();

		void Frame();
		
		char name[25];
		char destinationIPAddress[16];
		int destinationPort;

		int socket;
		struct sockaddr_in socketAddr;
	};
}

#endif