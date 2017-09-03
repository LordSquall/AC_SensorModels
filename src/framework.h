#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

/* System includes */
#include <vector>
#include <thread>
#include <stdio.h>
#include <winsock2.h>

/* Libraries */
#pragma comment(lib, "ws2_32.lib")

/* Local project includes */
#include "sensors\sensor.h"
#include "sensors\altimeter.h"
#include "tinydir\tinydir.h"
#include "tinyxml2\tinyxml2.h"

namespace AC_SensorModels
{
	class Framework
	{
	public:
		Framework();

		bool Initialise();
		bool Frame();

	private:
		bool ProcessDataDirectory(const char* dataDir);

		void SensorFunction(Sensor* sensor);

	private:
		INT64 frequency;
		float ticksPerSecond;
		INT64 frameworkStartTime;

		std::vector<Sensor*> _sensors;
		std::vector<std::thread> _threadHandles;
		WSADATA _wsa;
	};
}
#endif
