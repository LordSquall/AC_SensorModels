#include "sensor.h"

namespace AC_SensorModels
{
	Sensor::Sensor()
	{
		active = true;
		currentValue = 0.0f;

		speed = 0.5f;
		startTime = 0.0f;

		bounceDir = true;
	}

	void Sensor::Frame(float time)
	{

		switch (outputMode)
		{
		case 0:		/* CLAMP */
			ClampUpdateBehaviour(time);
			break;
		case 1:		/* LOOP */
			LoopUpdateBehaviour(time);
			break;
		case 2:		/* BOUNCE */
			BounceUpdateBehaviour(time);
			break;
		}
	}

	void Sensor::ClampUpdateBehaviour(float time)
	{
		currentValue += time;

		if (currentValue < lowerLimit)
		{
			currentValue = lowerLimit;
		}else if (currentValue > upperLimit)
		{
			currentValue = upperLimit;
		}
	}

	void Sensor::LoopUpdateBehaviour(float time)
	{
		currentValue = lowerLimit + time * (upperLimit - lowerLimit);
		//intf("currentValue: %.3f\n", time);
		if (currentValue < lowerLimit)
		{
			currentValue = upperLimit;
		}
		else if (currentValue > upperLimit)
		{
			currentValue = 0.0f;
		}
	}

	void Sensor::BounceUpdateBehaviour(float time)
	{
		float intpart;

		float lower = lowerLimit;
		float upper = upperLimit;

		if (bounceDir == false)
		{
			lower = upperLimit;
			upper = lowerLimit;
		}

		currentValue = lower + modff((time * 0.25f), &intpart) * (upper - lower);
		printf("currentValue: %.3f\n", currentValue);

		if ((currentValue > upperLimit-1) || (currentValue < lowerLimit+1))
		{
			bounceDir = !bounceDir;
		}
	}

}