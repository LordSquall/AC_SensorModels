#include "sensor.h"

namespace AC_SensorModels
{
	Sensor::Sensor()
	{
		active = true;
		currentValue = 0.0f;

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
		currentValue += time;

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
		if (bounceDir == true)
		{
			currentValue += time;
		}
		else
		{
			currentValue -= time;
		}

		if ((currentValue > upperLimit) || (currentValue < lowerLimit))
		{
			bounceDir = !bounceDir;
		}
	}

}