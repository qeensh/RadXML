#ifndef _RADXML_PARTICLE_SYSTEM_H_
	#define _RADXML_PARTICLE_SYSTEM_H_

	#ifdef defAPI_OGRE3D
		#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
			#include <OGRE/OgreParticleSystem.h>
		#else
			#include <OgreParticleSystem.h>
		#endif
	#endif

	#include <string>
	#include <vector>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlMovable.h"

	namespace RadXml
	{
		class XmlFile;

		class defRADXML_EXPORT ParticleSystem: public Movable
		{
			public:
				ParticleSystem (std::string newName);
				~ParticleSystem ();

				void parseXML (XmlFile *level, XMLElement *element, Movable *parent = 0);

				void create (Movable *parent = 0);
				void destroy ();

				void setVisible (bool visible);
				bool getVisible ();

				Movable *clone (std::string newName, Movable *parent = 0);

				Ogre::ParticleSystem *psSystem;
				std::string particleSystemName;
		};
	}
#endif

