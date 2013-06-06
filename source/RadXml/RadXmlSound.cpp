#include "RadXmlSound.h"

#include "RadXml.h"
#include "RadXmlXmlFile.h"
#include "RadXmlGlobal.h"

#include "xml/xml.h"

#include "common/mai_radxml_system.h"
#include "common/com_radxml_sound.h"
#include "common/com_radxml_strconv.h"

namespace RadXml
{
	// Sound
	Sound::Sound (std::string newName)
		: Movable (newName)
	{
		sSound = 0;
		load = true;
		streaming = false;
		file = "";
		gain = 1.0;
		bPlay = true;

		iType = GOT_SOUND;

		addEvent (new Event ("onstart", ""));
		addEvent (new Event ("onend", ""));
	}

	Sound::~Sound ()
	{
		destroy ();
	}

	void Sound::parseXML (XmlFile *level, XMLElement *element, Movable *parent)
	{
		parseMovableXML (level, element, parent);

		std::string strSrc = element->getAttributeValue ("src");
		std::string strLoad = element->getAttributeValue ("load");
		std::string strStreaming = element->getAttributeValue ("streaming");
		std::string strGain = element->getAttributeValue ("gain");
		std::string strPlay = element->getAttributeValue ("play");

		if (strLoad != "")
			load = RadXmlCommon::s13StringToBool (strLoad);

		if (strStreaming != "")
			streaming = RadXmlCommon::s13StringToBool (strStreaming);

		if (strGain != "")
			gain = RadXmlCommon::s13StringToDouble (strGain);

		if (strPlay != "")
			bPlay = RadXmlCommon::s13StringToBool (strPlay);

		if (strSrc != "")
		{
			strSrc = RadXmlCommon::System_NormalizeDirectoryPath (level->getDirPath () + "/" + strSrc, 0, false);
			file = strSrc;
		}

		if (streaming == true)
			level->addMusic (this);
	}

	void Sound::setVisible (bool visible)
	{
		bVisible = visible;
	}

	bool Sound::getVisible ()
	{
		return (bVisible);
	}

	Movable *Sound::clone (std::string newName, Movable *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		Sound *clone = new Sound (*this);

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

	void Sound::create (Movable *parent)
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();

		if (load == true)
		{
			sSound = new RadXmlCommon::s13Sound ();
			sSound->strPath = file;
			sSound->bStreaming = streaming;
			sSound->dGain = gain;
			sSound->LoadPath ();

			if (sSound->IsLoaded () == false)
				RadXmlCommon::s13LogMessage ("Unable to load audio file: " + file);

			if (bPlay == true)
				sSound->Play ();
		}

		/// @note Create the SceneNode here and keep it updated.
		updateSettings ();
		setParent (parent);
	}

	void Sound::destroy ()
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();

		destroySceneNode ();

		if (sSound != 0)
		{
			delete sSound;
			sSound = 0;
		}
	}

	void Sound::updateSettings ()
	{
	}

	void Sound::play ()
	{
		if (sSound == 0)
			return;

		if (sSound->bStreaming == true)
		{
			sSound->LoadPath ();

			if (sSound->IsPlaying () == false)
				sSound->Play ();
		}
		else
			sSound->Play ();

		bPlay = true;
	}

	void Sound::stop ()
	{
		sSound->Stop ();

		bPlay = false;
	}

	bool Sound::isPlaying ()
	{
		return (sSound->IsPlaying ());
	}

	bool Sound::isStreaming ()
	{
		return (sSound->bStreaming);
	}

	bool Sound::isLoaded ()
	{
		if (sSound == 0)
			return (false);

		return (sSound->IsLoaded ());
	}

	// SoundClass
	void SoundClass::setupSoundClass ()
	{
		Class *cClass = new Class ("Sound", "Sound");
		cClass->set ("getName", new Function ("getName", 0, MovableClass::getName));
		cClass->set ("clone", new Function ("clone", 0, MovableClass::clone));
		cClass->set ("translate", new Function ("translate", 0, MovableClass::translate));
		cClass->set ("setPosition", new Function ("setPosition", 0, MovableClass::setPosition));
		cClass->set ("getPosition", new Function ("getPosition", 0, MovableClass::getPosition));
		cClass->set ("setScale", new Function ("setScale", 0, MovableClass::setScale));
		cClass->set ("getScale", new Function ("getScale", 0, MovableClass::getScale));
		cClass->set ("setRotation", new Function ("setRotation", 0, MovableClass::setRotation));
		cClass->set ("setYaw", new Function ("setYaw", 0, MovableClass::setYaw));
		cClass->set ("getYaw", new Function ("getYaw", 0, MovableClass::getYaw));
		cClass->set ("setPitch", new Function ("setPitch", 0, MovableClass::setPitch));
		cClass->set ("getPitch", new Function ("getPitch", 0, MovableClass::getPitch));
		cClass->set ("setRoll", new Function ("setRoll", 0, MovableClass::setRoll));
		cClass->set ("getRoll", new Function ("getRoll", 0, MovableClass::getRoll));
		cClass->set ("getOrientation", new Function ("getOrientation", 0, MovableClass::getOrientation));
		cClass->set ("setVelocity", new Function ("setVelocity", 0, MovableClass::setVelocity));
		cClass->set ("getVelocity", new Function ("getVelocity", 0, MovableClass::getVelocity));
		cClass->set ("setAcceleration", new Function ("setAcceleration", 0, MovableClass::setAcceleration));
		cClass->set ("getAcceleration", new Function ("getAcceleration", 0, MovableClass::getAcceleration));
		cClass->set ("setFriction", new Function ("setFriction", 0, MovableClass::setFriction));
		cClass->set ("getFriction", new Function ("getFriction", 0, MovableClass::getFriction));
		cClass->set ("setMaxVelocity", new Function ("setMaxVelocity", 0, MovableClass::setMaxVelocity));
		cClass->set ("getMaxVelocity", new Function ("getMaxVelocity", 0, MovableClass::getMaxVelocity));
		cClass->set ("setVisible", new Function ("setVisible", 0, MovableClass::setVisible));
		cClass->set ("getVisible", new Function ("getVisible", 0, MovableClass::getVisible));
		cClass->set ("setCollides", new Function ("setCollides", 0, MovableClass::setCollides));
		cClass->set ("getCollides", new Function ("getCollides", 0, MovableClass::getCollides));
		cClass->set ("setBoundingBoxVisible", new Function ("setBoundingBoxVisible", 0, MovableClass::setBoundingBoxVisible));
		cClass->set ("getBoundingBoxVisible", new Function ("getBoundingBoxVisible", 0, MovableClass::getBoundingBoxVisible));
		cClass->set ("setLookAt", new Function ("setLookAt", 0, MovableClass::setLookAt));

		cClass->set ("play", new Function ("play", 0, SoundClass::play));
		cClass->set ("stop", new Function ("stop", 0, SoundClass::stop));

		JavascriptManager::getSingletonPtr ()->set ("Sound", cClass);
	}

	Variable *SoundClass::play (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Sound *sSound = static_cast<Sound *> (objThis->getHiddenData ("gameObject"));
		sSound->play ();

		return (0);
	}

	Variable *SoundClass::stop (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Sound *sSound = static_cast<Sound *> (objThis->getHiddenData ("gameObject"));
		sSound->stop ();

		return (0);
	}
}

