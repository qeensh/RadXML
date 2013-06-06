#ifndef _RADXML_CAMERA_H_
	#define _RADXML_CAMERA_H_

	#ifdef defAPI_OGRE3D
		#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
			#include <OGRE/OgreCamera.h>
		#else
			#include <OgreCamera.h>
		#endif
	#endif

	#include <string>
	#include <vector>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlMovable.h"

	namespace RadXml
	{
		class XmlFile;

		class defRADXML_EXPORT Camera: public Movable
		{
			public:
				Camera (std::string newName);
				~Camera ();

				void parseXML (XmlFile *level, XMLElement *element, Movable *parent = 0);

				void create (int iWindow = 0, Movable *parent = 0);
				void destroy ();

				void setVisible (bool visible);
				bool getVisible ();

				Movable *clone (std::string newName, Movable *parent = 0);

				Ogre::Camera *camCamera;
				bool enabled;

				Ogre::ColourValue bgColor;
				Ogre::Real numMipMaps;
				Ogre::Real focalLength;
				Ogre::Real nearClipDistance;
				Ogre::Real farClipDistance;
		};
	}
#endif

