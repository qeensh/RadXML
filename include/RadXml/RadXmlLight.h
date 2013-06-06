#ifndef _RADXML_LIGHT_H_
	#define _RADXML_LIGHT_H_

	#ifdef defAPI_OGRE3D
		#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
			#include <OGRE/OgreLight.h>
		#else
			#include <OgreLight.h>
		#endif
	#endif

	#include <string>
	#include <vector>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlMovable.h"

	namespace RadXml
	{
		class XmlFile;

		enum LightTypes
		{
			LT_POINT		= 1, 
			LT_SPOTLIGHT	= 2, 
			LT_DIRECTIONAL	= 3
		};

		class defRADXML_EXPORT Light: public Movable
		{
			public:
				Light (std::string newName);
				~Light ();

				void parseXML (XmlFile *level, XMLElement *element, Movable *parent = 0);

				void create (Movable *parent = 0);
				void destroy ();

				void setVisible (bool visible);
				bool getVisible ();

				Movable *clone (std::string newName, Movable *parent = 0);

				Ogre::Light *lLight;
				int type;
				Ogre::ColourValue diffuse;
				Ogre::ColourValue specular;
		};
	}
#endif

