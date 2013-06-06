#ifndef _RADXML_VECTOR4_H_
	#define _RADXML_VECTOR4_H_

	#include "RadXmlNative.h"

	namespace RadXml
	{
		class Vector4Class
		{
			public:
				static void setupVector4Class ();

				static Variable *add (Variable *thisObject, Variable *arguments);
				static Variable *subtract (Variable *thisObject, Variable *arguments);
				static Variable *multiply (Variable *thisObject, Variable *arguments);
				static Variable *divide (Variable *thisObject, Variable *arguments);
				static Variable *dot (Variable *thisObject, Variable *arguments);
		};
	}
#endif

