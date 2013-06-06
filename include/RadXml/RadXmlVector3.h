#ifndef _RADXML_VECTOR3_H_
	#define _RADXML_VECTOR3_H_

	#include "RadXmlNative.h"

	namespace RadXml
	{
		class Vector3Class
		{
			public:
				static void setupVector3Class ();

				static Variable *add (Variable *thisObject, Variable *arguments);
				static Variable *subtract (Variable *thisObject, Variable *arguments);
				static Variable *multiply (Variable *thisObject, Variable *arguments);
				static Variable *divide (Variable *thisObject, Variable *arguments);
				static Variable *dot (Variable *thisObject, Variable *arguments);
				static Variable *cross (Variable *thisObject, Variable *arguments);
				static Variable *length (Variable *thisObject, Variable *arguments);
				static Variable *normalise (Variable *thisObject, Variable *arguments);
				static Variable *squaredLength (Variable *thisObject, Variable *arguments);
				static Variable *absDotProduct (Variable *thisObject, Variable *arguments);
				static Variable *angleBetween (Variable *thisObject, Variable *arguments);
				static Variable *distance (Variable *thisObject, Variable *arguments);
				static Variable *getRotationTo (Variable *thisObject, Variable *arguments);
				static Variable *makeCeil (Variable *thisObject, Variable *arguments);
				static Variable *makeFloor (Variable *thisObject, Variable *arguments);
				static Variable *midPoint (Variable *thisObject, Variable *arguments);
				static Variable *perpendicular (Variable *thisObject, Variable *arguments);
				static Variable *reflect (Variable *thisObject, Variable *arguments);
		};
	}
#endif

