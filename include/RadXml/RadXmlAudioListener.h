#ifndef _RADXML_AUDIO_LISTENER_H_
	#define _RADXML_AUDIO_LISTENER_H_

	#include <string>
	#include <vector>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlMovable.h"

	namespace RadXml
	{
		class XmlFile;

		class defRADXML_EXPORT AudioListener: public Movable
		{
			public:
				AudioListener (std::string newName);
				~AudioListener ();

				void parseXML (XmlFile *level, XMLElement *element, Movable *parent = 0);

				void create (Movable *parent = 0);
				void destroy ();

				void setVisible (bool visible);
				bool getVisible ();

				Movable *clone (std::string newName, Movable *parent = 0);

				void updateSettings ();
				
				Ogre::Real maxDistance;
				Ogre::Real gain;
				Ogre::Real minGain;
				Ogre::Real maxGain;
		};
	}
#endif

