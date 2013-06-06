#ifndef _RADXML_SOUND_H_
	#define _RADXML_SOUND_H_

	#include <string>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlMovable.h"

	namespace RadXmlCommon
	{
		class s13Sound;
	}

	namespace RadXml
	{
		class XmlFile;

		class defRADXML_EXPORT Sound: public Movable
		{
			public:
				Sound (std::string newName);
				~Sound ();

				void parseXML (XmlFile *level, XMLElement *element, Movable *parent = 0);

				void create (Movable *parent = 0);
				void destroy ();

				void setVisible (bool visible);
				bool getVisible ();

				Movable *clone (std::string newName, Movable *parent = 0);

				void updateSettings ();

				void play ();
				void stop ();
				bool isPlaying ();
				bool isStreaming ();
				bool isLoaded ();

				inline void setSound (RadXmlCommon::s13Sound *sound)
				{
					sSound = sound;
				}

				inline RadXmlCommon::s13Sound *getSound ()
				{
					return (sSound);
				}

			protected:
				RadXmlCommon::s13Sound *sSound;
				bool load;
				bool streaming;
				bool bPlay;
				std::string file;
				Ogre::Real gain;
		};

		class SoundClass
		{
			public:
				static void setupSoundClass ();

				static Variable *play (Variable *thisObject, Variable *arguments);
				static Variable *stop (Variable *thisObject, Variable *arguments);
		};
	}
#endif

