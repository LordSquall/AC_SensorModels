#include "framework.h"

namespace AC_SensorModels
{
	Framework::Framework()
	{

	}

	bool Framework::Initialise(char* dataDirectory)
	{
		/* Local variables */
		bool result = false;

		/* Initialise timing functions */
		/* Get system class frequency */
		QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

		/* Work out clock ticks per second */
		ticksPerSecond = (float)(frequency / 1000);

		/* Get the start time for the framework */
		QueryPerformanceCounter((LARGE_INTEGER*)&frameworkStartTime);

		/* Process the data directory to build data set */
		result = ProcessDataDirectory(dataDirectory);
		if (result == false)
		{
			return result;
		}

		/* Initialise networking */
		printf("Initialising Winsock\n");
		if (WSAStartup(MAKEWORD(2, 2), &_wsa) != 0)
		{
			return false;
		}

		/* Initialise each sensor */
		for (std::vector<Sensor*>::iterator it = _sensors.begin(); it != _sensors.end(); ++it)
		{
			Sensor* sensor = (Sensor*)*it;

			/* Print out sensor information */
			printf("Sensor: %s IPAddr: %s:%d\n", sensor->name, sensor->destinationIPAddress, sensor->destinationPort);
		
			/* Open Sockets for each sensor */
			sensor->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (sensor->socket == SOCKET_ERROR)
			{
				printf("ERROR: Unable to open Socket\n");
				return false;
			}

			/* Setup address structure */
			memset((char *)&sensor->socketAddr, 0, sizeof(sensor->socketAddr));
			sensor->socketAddr.sin_family = AF_INET;
			sensor->socketAddr.sin_port = htons(sensor->destinationPort);
			InetPton(AF_INET, sensor->destinationIPAddress, &sensor->socketAddr.sin_addr);
						
			/* Create Threads for each sensor */
			std::thread* thread = new std::thread(&Framework::SensorFunction, this, sensor);
		}

		return result;
	}

	bool Framework::Frame()
	{
		return true;
	}

	void Framework::SensorFunction(Sensor* sensor)
	{
		/* Timing variables */
		INT64 startSampleTime = 0;				/* start of sample frame */
		INT64 startFrameTime = 0;				/* start of frame */
		INT64 currentTime = 0;					/* current system time */
		float frameTime = 0.0f;					/* frame time scaled against frequency */
		float sampleFrameTime = 0.0f;			/* sample frame time scaled against frequency */

		/* Flow variables */
		bool running = true;					/* termination flag */

		/* Networking variables */
		int slen = sizeof(sensor->socketAddr);	/* socket address length */

		/* Get the start time for the first sample frame */
		QueryPerformanceCounter((LARGE_INTEGER*)&startSampleTime);

		/* Match start times */
		startFrameTime = startSampleTime;

		/* Run sensor Loop */
		while (running)
		{
			/* Get current time */
			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

			/* Calculate time difference since last sample frame and scale to frequency */
			sampleFrameTime = ((float)(currentTime - startSampleTime)) / ticksPerSecond;
			
			//printf("SampleFrameTime: %.3f\n", sampleFrameTime / 1000.0f);
			/* Calculate time difference since last frame and scale to frequency */
			frameTime = ((float)(currentTime - startFrameTime)) / ticksPerSecond;

			//printf("FrameTime: %.3f\n", frameTime);
			/* Update the sensor model */
			sensor->Frame(sampleFrameTime / 1000.0f);

			/* At update rate, send over the network */
			if (fmod(sampleFrameTime, sensor->samplerate) == 0.0f)
			{
				/* Copy sensor value in to byte array */
				memcpy(sensor->buffer, &sensor->currentValue, sizeof(sensor->currentValue));
				
				/* Send over the network */
				if (sendto(sensor->socket, sensor->buffer, sizeof(sensor->currentValue), 0, (struct sockaddr *) &sensor->socketAddr, slen) == SOCKET_ERROR)
				{
					printf("ERROR: sendto() failed with error code : %d", WSAGetLastError());
					running = false;
				}

				/* Update the start sample frame time */
				startSampleTime = currentTime;
			}

			/* Update the frame start time */
			startFrameTime = currentTime;
		}
	}

	bool Framework::ProcessDataDirectory(const char* dataDirectory)
	{
		/* look for sensor config files in directory */
		tinydir_dir dir;
		tinydir_open(&dir, dataDirectory);

		/* Process each file in turn */
		while (dir.has_next)
		{
			/* Get file information */
			tinydir_file dirFile;
			tinydir_readfile(&dir, &dirFile);

			/* Open the file as XML Document*/
			if (dirFile.is_dir == false)
			{
				
				tinyxml2::XMLDocument doc;
				doc.LoadFile(dirFile.path);

				/* Get Sensor element */
				tinyxml2::XMLElement* sensorElement = doc.FirstChildElement("sensor");

				/* Check to make sure the root tag is sensor */
				if (sensorElement != NULL)
				{
					/* Sensor config found */
					Sensor* newSensor = new Sensor();

					/* name */
					strcpy_s(newSensor->name, sensorElement->FirstChildElement("name")->GetText());

					/* Check to make sure the sensor is enabled */
					if (sensorElement->BoolAttribute("enabled") == TRUE)
					{

						/* Get input element */
						tinyxml2::XMLElement* inputElement = sensorElement->FirstChildElement("input");
						/* samplerate */
						newSensor->samplerate = inputElement->IntAttribute("samplerate");

						/* Get output element */
						tinyxml2::XMLElement* outputElement = sensorElement->FirstChildElement("output");
						/* mode */
						newSensor->outputMode = outputElement->IntAttribute("mode");
						/* bounds lower limit */
						newSensor->lowerLimit = outputElement->FirstChildElement("bounds")->FloatAttribute("lower");
						/* bounds upper limit */
						newSensor->upperLimit = outputElement->FirstChildElement("bounds")->FloatAttribute("upper");

						/* Get network element */
						tinyxml2::XMLElement* networkElement = sensorElement->FirstChildElement("network");

						/* ip address */
						strcpy_s(newSensor->destinationIPAddress, networkElement->Attribute("ipaddress"));
						/* ip port */
						newSensor->destinationPort = networkElement->IntAttribute("port");

						/* Add new sensor to framework */
						_sensors.push_back(newSensor);
					}
				}
			}
			tinydir_next(&dir);
		}

		tinydir_close(&dir);

		return true;
	}
}

