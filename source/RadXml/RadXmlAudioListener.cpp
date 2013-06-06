#include "RadXmlAudioListener.h"

#include "RadXml.h"
#include "RadXmlXmlFile.h"

#include "xml/xml.h"

#include "common/com_radxml_strconv.h"

#ifdef defAPI_OPENAL
	#ifdef defAPI_OPENAL_NOT_IN_DIRECTORY
		#if ((!defined (defSYS_IPHONE)) && (!defined (defSYS_MACOSX)))
			#include <AL/al.h>
			#include <AL/alc.h>
		#else
			#include <OpenAL/al.h>
			#include <OpenAL/alc.h>
		#endif
	#else
		#include <al.h>
		#include <alc.h>
	#endif
#endif

namespace RadXml
{
	// AudioListener
	AudioListener::AudioListener (std::string newName)
		: Movable (newName)
	{
		maxDistance = 100000.0;
		gain = 1.0;
		minGain = 0.0;
		maxGain = 1.0;

		iType = GOT_AUDIO_LISTENER;
	}

	AudioListener::~AudioListener ()
	{
		destroy ();
	}

	void AudioListener::parseXML (XmlFile *level, XMLElement *element, Movable *parent)
	{
		parseMovableXML (level, element, parent);

		std::string strGain = element->getAttributeValue ("gain");
		std::string strMingain = element->getAttributeValue ("mingain");
		std::string strMaxgain = element->getAttributeValue ("maxgain");
		std::string strMaxdistance = element->getAttributeValue ("maxdistance");

		if (strGain != "")
			gain = RadXmlCommon::s13StringToDouble (strGain);

		if (strMingain != "")
			minGain = RadXmlCommon::s13StringToDouble (strMingain);

		if (strMaxgain != "")
			maxGain = RadXmlCommon::s13StringToDouble (strMaxgain);

		if (strMaxdistance != "")
			maxDistance = RadXmlCommon::s13StringToDouble (strMaxdistance);
	}

	void AudioListener::setVisible (bool visible)
	{
		bVisible = visible;
	}

	bool AudioListener::getVisible ()
	{
		return (bVisible);
	}

	Movable *AudioListener::clone (std::string newName, Movable *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		AudioListener *clone = new AudioListener (*this);

		clone->setName (newName);
		clone->create (parent);
		clone->removeAllChildren ();

		for (unsigned int iIdx = 0; iIdx < getNumChildren (); iIdx++)
		{
			Movable *mChild = getChild (iIdx);
			Movable *mSubClone = mChild->clone (clone->getName () + "_sub", clone);
		}

		ogrGame->getCurrentXMLFile ()->addMovable (clone);

		return (clone);
	}

	void AudioListener::create (Movable *parent)
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();

		/// @note Create the SceneNode here and keep it updated.
		updateSettings ();
		setParent (parent);
	}

	void AudioListener::destroy ()
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();

		destroySceneNode ();
	}

	void AudioListener::updateSettings ()
	{
		Ogre::Vector3 v3dPosition = getPosition ();

		#ifdef defAPI_OPENAL
			alListenerf (AL_MAX_DISTANCE, maxDistance);
			alListenerf (AL_GAIN, gain);
			alListenerf (AL_MIN_GAIN, minGain);
			alListenerf (AL_MAX_GAIN, maxGain);
			alListener3f (AL_POSITION, v3dPosition.x, v3dPosition.y, v3dPosition.z);
			ALfloat aryDir[] = {Ogre::Degree (rotation.x).valueRadians (), Ogre::Degree (rotation.y).valueRadians (),
								Ogre::Degree (rotation.z).valueRadians ()};

			alListenerfv (AL_ORIENTATION, aryDir);
		#endif
	}
}

