#ifndef _ALTIMETER_H_
#define _ALTIMETER_H_

#include <stdio.h>

#include "sensor.h"

namespace AC_SensorModels
{
	class Altimeter : public Sensor
	{
	public:
		Altimeter();

		void Initialise();
		void Update();
		void Send();
	};
}

#endif