/* system includes */
#include <stdio.h>
#include <ctime>

/* local project includes */
#include "ac_sensormodelsconfig.h"
#include "framework.h"
#include "timer.h"
#include "sensors\sensor.h"
#include "sensors\altimeter.h"

using namespace AC_SensorModels;

int main()
{
	/* Local variables */
	bool running				= true;	/* termination flag */
	unsigned int sensorCount	= 0;	/* number of sensors */


	/* Print version information to screen */
	printf("AC Sensor Model - Version %d.%d\n", AC_SensorModels_VERSION_MAJOR, AC_SensorModels_VERSION_MINOR);

	/* Create Framework */
	Framework* framework = new Framework();

	/* Initialise the framework */
	framework->Initialise();

	while (running == true)
	{
		/* Update framework timings */
		framework->Frame();
	}
	float currentTime = 0.0f;

	/* enter infinite loop */
	while (true)
	{
		if (clock() % 2000 == 0)
		{
			printf("foobar\n");
		}
	}

	// TEMP allow console to remain open in IDE
	getchar();
}