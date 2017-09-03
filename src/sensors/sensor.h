#ifndef _SENSOR_H_
#define _SENSOR_H_

/* System Includes */
#include <math.h>
#include <stdio.h>
#include <winsock2.h>

/* Typedefs */
typedef char Byte;

namespace AC_SensorModels
{
	class Sensor
	{
	public:
		Sensor();

		void Frame(float time);
		
		float currentValue;

		bool active;
		int samplerate;
		char name[25];
		char destinationIPAddress[16];
		int destinationPort;

		int outputMode;	
		float lowerLimit;
		float upperLimit;


		int socket;
		struct sockaddr_in socketAddr;
		Byte buffer[4];

	private:
		bool bounceDir;

	private:
		void ClampUpdateBehaviour(float time);
		void LoopUpdateBehaviour(float time);
		void BounceUpdateBehaviour(float time);
	};
}

#endif