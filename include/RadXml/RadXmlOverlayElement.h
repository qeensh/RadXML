#ifndef _RADXML_OVERLAY_ELEMENT_H_
	#define _RADXML_OVERLAY_ELEMENT_H_

	#ifdef defAPI_OGRE3D
		#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
			#include <OGRE/OgreOverlay.h>
			#include <OGRE/OgreOverlayManager.h>
		#else
			#include <OgreOverlay.h>
			#include <OgreOverlayManager.h>
			#include <OgreOverlayElement.h>
		#endif
	#endif

	#include <string>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlMovable.h"

	namespace RadXml
	{
		class XmlFile;

		class defRADXML_EXPORT OverlayElement: public Movable
		{
			public:
				OverlayElement (std::string newName);
				~OverlayElement ();

				void parseXML (XmlFile *level, XMLElement *element, Movable *parent = 0);

				void create (Movable *parent = 0);
				void destroy ();

				void setVisible (bool visible);
				bool getVisible ();

				Movable *clone (std::string newName, Movable *parent = 0);

				inline Ogre::OverlayElement *getOverlayElement ()
				{
					return (oeElm);
				}

				void setCaption (std::string text);
				std::string getCaption ();

			protected:
				std::string elementName;
				Ogre::OverlayElement *oeElm;
		};

		class OverlayElementClass
		{
			public:
				static void setupOverlayElementClass ();

				static Variable *setCaption (Variable *thisObject, Variable *arguments);
				static Variable *getCaption (Variable *thisObject, Variable *arguments);
				static Variable *setSize (Variable *thisObject, Variable *arguments);
				static Variable *getSize (Variable *thisObject, Variable *arguments);
		};
	}
#endif

