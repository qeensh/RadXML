#ifndef _RADXML_VECTOR2_H_
	#define _RADXML_VECTOR2_H_

	#include "RadXmlNative.h"

	namespace RadXml
	{
		class Vector2Class
		{
			public:
				static void setupVector2Class ();

				static Variable *add (Variable *thisObject, Variable *arguments);
				static Variable *subtract (Variable *thisObject, Variable *arguments);
				static Variable *multiply (Variable *thisObject, Variable *arguments);
				static Variable *divide (Variable *thisObject, Variable *arguments);
				static Variable *dot (Variable *thisObject, Variable *arguments);
		};
	}
#endif

