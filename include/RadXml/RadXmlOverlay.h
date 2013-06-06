#ifndef _RADXML_OVERLAY_H_
	#define _RADXML_OVERLAY_H_

	#ifdef defAPI_OGRE3D
		#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
			#include <OGRE/OgreOverlay.h>
			#include <OGRE/OgreOverlayManager.h>
		#else
			#include <OgreOverlay.h>
			#include <OgreOverlayManager.h>
		#endif
	#endif

	#include <string>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlMovable.h"

	class XMLElement;

	namespace RadXml
	{
		class XmlFile;

		class defRADXML_EXPORT Overlay: public Movable
		{
			public:
				Overlay (std::string newName);
				~Overlay ();

				void parseXML (XmlFile *level, XMLElement *element, Movable *parent = 0);

				void create (Movable *parent = 0);
				void destroy ();

				void setVisible (bool visible);
				bool getVisible ();

				Movable *clone (std::string newName, Movable *parent = 0);

				inline Ogre::Overlay *getOverlay ()
				{
					return (oOverlay);
				}

			protected:
				std::string overlay;
				Ogre::Overlay *oOverlay;
		};
	}
#endif

