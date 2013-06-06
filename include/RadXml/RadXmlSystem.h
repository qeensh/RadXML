#ifndef _RADXML_SYSTEM_H_
	#define _RADXML_SYSTEM_H_

	#include "RadXmlPreprocessor.h"
	#include "RadXmlNative.h"

	namespace RadXml
	{
		class SystemClass
		{
			public:
				static void setupSystemClass ();

				static Variable *loadExternal (Variable *thisObject, Variable *arguments);
				static Variable *executeExternal (Variable *thisObject, Variable *arguments);
				static Variable *unloadExternal (Variable *thisObject, Variable *arguments);
		};
	}
#endif

