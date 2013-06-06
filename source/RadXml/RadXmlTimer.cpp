#include "RadXmlTimer.h"

#include "RadXml.h"

namespace RadXml
{
	// Timer
	bool Timer::update (unsigned int time)
	{
		if (javascript == "")
			return (true);

		if (current >= end)
		{
			RadXML *ogrGame = RadXML::getSingletonPtr ();

			ogrGame->compileAndRunJSString (javascript);

			return (true);
		}
		else
			addTime (time);

		return (false);
	}
}

