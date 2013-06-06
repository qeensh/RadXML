#include "RadXmlLevel.h"
#include "RadXmlXmlFile.h"

#include "RadXml.h"

#include "RadXmlMovable.h"
#include "RadXmlEntity.h"
#include "RadXmlCamera.h"
#include "RadXmlLight.h"
#include "RadXmlParticleSystem.h"
#include "RadXmlAudioListener.h"
#include "RadXmlSound.h"
#include "RadXmlAnimationTrack.h"
#include "RadXmlOverlay.h"
#include "RadXmlOverlayElement.h"

#include "xml/xml.h"

#include "common/mai_radxml_system.h"
#include "common/com_radxml_strconv.h"

#include "languages/language.h"

#ifdef defAPI_OGRE3D
	#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
		#include <OGRE/OgreException.h>
		#include <OGRE/OgreColourValue.h>
		#include <OGRE/OgreSceneManager.h>
	#else
		#include <OgreException.h>
		#include <OgreColourValue.h>
		#include <OgreSceneManager.h>
	#endif
#endif

namespace RadXml
{
	Level::Level ()
	{
		name = "";
		hashName = 0;

		addEvent (new Event ("onload", ""));
	}

	Level::~Level ()
	{
	}

	void Level::setName (std::string newName)
	{
		name = newName;
		hashName = RadXmlCommon::s13StringToHash (newName);
	}

	void Level::addMusic (Sound *music)
	{
		aryMusic.push_back (music);
	}

	void Level::parseLevel (XmlFile *file, XMLElement *element)
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();

		name = element->getAttributeValue ("name");
		setJavascriptToEvent ("onload", element->getAttributeValue ("onload"));

		std::string strSkyBox = element->getAttributeValue ("skybox");
		std::string strAmbientColor = element->getAttributeValue ("ambientcolor");
		std::string strShadowColor = element->getAttributeValue ("shadowcolor");

		if (name == "")
		{
			int iRandName = rand ();
			setName (RadXmlCommon::s13IntToString (iRandName));
		}

		if (strSkyBox != "")
			mSceneMgr->setSkyBox (true, strSkyBox);

		if (strAmbientColor != "")
		{
			Ogre::ColourValue cvColor = RadXML::parseColourValue (strAmbientColor);
			mSceneMgr->setAmbientLight (cvColor);
		}

		if (strShadowColor != "")
		{
			Ogre::ColourValue cvColor = RadXML::parseColourValue (strAmbientColor);
			mSceneMgr->setShadowColour (cvColor);
		}

		parseXMLMovables (file, element);
	}

	void Level::parseXMLMovables (XmlFile *file, XMLElement *element, Movable *parent)
	{
		for (int iIdx = 0; iIdx < 9; iIdx++)
		{
			std::string strType = "";
			int iType = (iIdx + 1);

			switch (iIdx)
			{
				case 0:
					strType = "entity";
					break;
				case 1:
					strType = "camera";
					break;
				case 2:
					strType = "particlesystem";
					break;
				case 3:
					strType = "light";
					break;
				case 4:
					strType = "animationtrack";
					break;
				case 5:
					strType = "audiolistener";
					break;
				case 6:
					strType = "sound";
					break;
				case 7:
					strType = "overlay";
					break;
				case 8:
					strType = "overlayelement";
					break;
			}

			std::vector<XMLElement *> aryTags = element->findAllTags (strType, 0, false);

			for (unsigned int iJdx = 0; iJdx < aryTags.size (); iJdx++)
			{
				try
				{
					parseXMLMovable (file, iType, aryTags.at (iJdx), parent);
				}
				catch (Ogre::Exception &ex)
				{
					RadXmlCommon::s13LogMessage ((std::string)MSG15 + ex.getFullDescription () + 
											(std::string)MSG16 + Ogre::StringConverter::toString (ex.getLine ()) + 
											(std::string)MSG17 + ex.getSource ());
				}
			}
		}
	}

	void Level::parseXMLMovable (XmlFile *file, int type, XMLElement *element, Movable *parent)
	{
		Movable *objObject = createMovable (file, type, "", parent, element);

		if (element->content != "")
			parseXMLMovables (file, element, objObject);
	}

	Movable *Level::createMovable (XmlFile *file, int type, std::string name, Movable *parent, XMLElement *element, bool createNow)
	{
		Movable *objObject = 0;

		if (type == GOT_ENTITY)
		{
			Entity *eEntity = new Entity (name);

			if (element != 0)
				eEntity->parseXML (file, element, parent);

			if (createNow == true)
				eEntity->create (parent);

			objObject = eEntity;
		}

		if (type == GOT_CAMERA)
		{
			Camera *cCamera = new Camera (name);

			if (element != 0)
				cCamera->parseXML (file, element, parent);

			if (createNow == true)
				cCamera->create (0, parent);

			objObject = cCamera;
		}

		if (type == GOT_PARTICLE_SYSTEM)
		{
			ParticleSystem *psSystem = new ParticleSystem (name);

			if (element != 0)
				psSystem->parseXML (file, element, parent);

			if (createNow == true)
				psSystem->create (parent);

			objObject = psSystem;
		}

		if (type == GOT_LIGHT)
		{
			Light *lLight = new Light (name);

			if (element != 0)
				lLight->parseXML (file, element, parent);

			if (createNow == true)
				lLight->create (parent);

			objObject = lLight;
		}

		if (type == GOT_ANIMATION_TRACK)
		{
			AnimationTrackObject *atoObject = new AnimationTrackObject (name);

			if (element != 0)
				atoObject->parseXML (file, element, parent);

			if (createNow == true)
				atoObject->create (parent);

			objObject = atoObject;
			aryAnimations.push_back (atoObject);
		}

		if (type == GOT_AUDIO_LISTENER)
		{
			AudioListener *alListener = new AudioListener (name);

			if (element != 0)
				alListener->parseXML (file, element, parent);

			if (createNow == true)
				alListener->create (parent);

			objObject = alListener;
		}

		if (type == GOT_SOUND)
		{
			Sound *sSound = new Sound (name);

			if (element != 0)
				sSound->parseXML (file, element, parent);

			if (createNow == true)
				sSound->create (parent);

			objObject = sSound;
		}

		if (type == GOT_OVERLAY)
		{
			Overlay *oOverlay = new Overlay (name);

			if (element != 0)
				oOverlay->parseXML (file, element, parent);

			if (createNow == true)
				oOverlay->create (parent);

			aryOverlays.push_back (oOverlay);
			objObject = oOverlay;
		}

		if (type == GOT_OVERLAY_ELEMENT)
		{
			OverlayElement *oOverlayElement = new OverlayElement (name);

			if (element != 0)
				oOverlayElement->parseXML (file, element, parent);

			if (createNow == true)
				oOverlayElement->create (parent);

			aryOverlayElements.push_back (oOverlayElement);
			objObject = oOverlayElement;
		}

		addMovable (objObject);

		return (objObject);
	}

	void Level::addMovable (Movable *newMovable)
	{
		hshMovables.insert (std::pair<unsigned long, Movable *> (newMovable->getHash (), newMovable));
		RadXML::getSingletonPtr ()->getJavascriptEngine ()->addMovableJSObject (newMovable);
	}

	Movable *Level::getMovable (std::string findName, bool skipErrorMessage)
	{
		HASHMAP<unsigned long, Movable *>::iterator itBegin = hshMovables.find (RadXmlCommon::s13StringToHash (findName));
		HASHMAP<unsigned long, Movable *>::iterator itEnd = hshMovables.end ();

		if (itBegin != itEnd)
			return ((*itBegin).second);
		else
		{
			if (skipErrorMessage == false)
				RadXmlCommon::s13LogMessage (MSG29 + findName);
		}

		return (0);
	}

	Movable *Level::getMovable (unsigned long hash, bool skipErrorMessage)
	{
		HASHMAP<unsigned long, Movable *>::iterator itBegin = hshMovables.find (hash);
		HASHMAP<unsigned long, Movable *>::iterator itEnd = hshMovables.end ();

		if (itBegin != itEnd)
			return ((*itBegin).second);
		else
		{
			if (skipErrorMessage == false)
				RadXmlCommon::s13LogMessage (MSG30);
		}

		return (0);
	}

	void Level::removeMovable (std::string findName)
	{
		HASHMAP<unsigned long, Movable *>::iterator itBegin = hshMovables.find (RadXmlCommon::s13StringToHash (findName));
		HASHMAP<unsigned long, Movable *>::iterator itEnd = hshMovables.end ();

		if (itBegin != itEnd)
		{
			RadXML::getSingletonPtr ()->getJavascriptEngine ()->removeMovableJSObject ((*itBegin).second);
			hshMovables.erase (itBegin);
		}
	}

	void Level::removeMovable (unsigned long hash)
	{
		HASHMAP<unsigned long, Movable *>::iterator itBegin = hshMovables.find (hash);
		HASHMAP<unsigned long, Movable *>::iterator itEnd = hshMovables.end ();

		if (itBegin != itEnd)
		{
			RadXML::getSingletonPtr ()->getJavascriptEngine ()->removeMovableJSObject ((*itBegin).second);
			hshMovables.erase (itBegin);
		}
	}
}

