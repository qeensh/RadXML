#ifndef _RADXML_MENU_H_
	#define _RADXML_MENU_H_

	#include <string>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlAppObject.h"

	namespace RadXml
	{
		class XmlFile;
		class Window;

		class defRADXML_EXPORT Menu: public AppObject
		{
			public:
				Menu (std::string newName);
				~Menu ();

				void create (int creationType, AppObject *parent = 0);
				void destroy ();

				void parseXML (App *level, XMLElement *element, AppObject *parent = 0);

				AppObject *clone (std::string newName, int creationType, AppObject *parent = 0);

				Window *menuWindow;
		};
	}
#endif

