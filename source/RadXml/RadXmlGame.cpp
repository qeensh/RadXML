#include "RadXmlGame.h"
#include "RadXml.h"
#include "RadXmlXmlFile.h"
#include "RadXmlGlobal.h"

#include "RadXmlMovable.h"
#include "RadXmlLevel.h"

#include "RadXmlInput.h"

#include "languages/language.h"

#include "xml/xml.h"

#include "common/mai_radxml_system.h"
#include "common/com_radxml_strconv.h"

#ifdef defAPI_OGRE3D
	#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
		#include <OGRE/OgreResourceManager.h>
		#include <OGRE/OgreMaterialManager.h>
		#include <OGRE/OgreMovableObject.h>
		#include <OGRE/OgreSceneQuery.h>
	#else
		#include <OgreResourceManager.h>
		#include <OgreMaterialManager.h>
		#include <OgreMovableObject.h>
		#include <OgreSceneQuery.h>
	#endif
#endif

namespace RadXml
{
	Game::Game (std::string newName)
		: Level ()
	{
		setName (newName);
	}

	Game::~Game ()
	{
		destroyAllGameObjects ();
	}

	void Game::destroyAllGameObjects ()
	{
		HASHMAP<unsigned long, Level *>::iterator itBegin = hshLevels.begin ();
		HASHMAP<unsigned long, Level *>::iterator itEnd = hshLevels.end ();

		while (itBegin != itEnd)
		{
			Level *objObject = (*itBegin).second;

			if (objObject != 0)
			{
				delete objObject;
				objObject = 0;
			}

			itBegin++;
		}
	}

	void Game::setName (std::string newName)
	{
		gameName = newName;
		gameHash = RadXmlCommon::s13StringToHash (newName);
	}

	void Game::parse (XmlFile *file, XMLElement *element)
	{
		std::vector<XMLElement *> aryResourceLocations = element->findAllTags ("resourcelocation");

		for (unsigned int iJdx = 0; iJdx < aryResourceLocations.size (); iJdx++)
		{
			XMLElement *xmlElement = aryResourceLocations.at (iJdx);
			std::string strDir = xmlElement->getAttributeValue ("dir");
			std::string strArch = xmlElement->getAttributeValue ("arch");
			std::string strType = xmlElement->getAttributeValue ("type");
			std::string strLoad = xmlElement->getAttributeValue ("load");
			strType = RadXmlCommon::s13ASCII_StringToLower (strType);
			bool bLoad = false;

			if (strLoad != "")
				bLoad = RadXmlCommon::s13StringToBool (strLoad);

			strDir = RadXmlCommon::System_NormalizeDirectoryPath (file->getDirectoryPath () + "/" + strDir, 0, false);

			#ifdef defAPI_OGRE3D
				Ogre::ResourceGroupManager::getSingleton ().addResourceLocation (strDir, strArch, strType);
				Ogre::ResourceGroupManager::getSingleton ().initialiseResourceGroup (strType);

				if (bLoad == true)
				{
					Ogre::ResourceManager::ResourceMapIterator rmiIterator =
						Ogre::MaterialManager::getSingletonPtr ()->getResourceIterator ();

					while (rmiIterator.hasMoreElements () == true)
					{
						Ogre::ResourceManager::ResourceHandleMap::mapped_type mtResource = rmiIterator.getNext ();
						std::string strGroupName = mtResource->getGroup ();

						if (strGroupName.find (strType) != std::string::npos)
						{
							mtResource->load ();
							mtResource->touch ();
						}
					}
				}
			#endif
		}

		std::vector<XMLElement *> aryLevels = element->findAllTags ("level");

		for (unsigned int iIdx = 0; iIdx < aryLevels.size (); iIdx++)
		{
			if (iIdx == 0)
				parseLevel (file, aryLevels.at (iIdx));
			else
			{
				Level *lvlLevel = new Level ();
				lvlLevel->parseLevel (file, aryLevels.at (iIdx));
			}
		}
	}

	void Game::addMovableForDeletion (Movable *object)
	{
		for (unsigned int iIdx = 0; iIdx < aryMovablesToDelete.size (); iIdx++)
		{
			Movable *mObject = aryMovablesToDelete.at (iIdx);

			if (mObject != 0)
			{
				if (mObject == object)
					return;
			}
		}

		aryMovablesToDelete.push_back (object);
	}

	// GameClass
	void GameClass::setupGameClass ()
	{
		ClassObject *cClass = new ClassObject ("Game", "Game");
		cClass->set ("getMovableByName", new Function ("getMovableByName", 0, GameClass::getMovableByName));
		cClass->set ("deleteObject", new Function ("deleteObject", 0, GameClass::deleteObject));
		cClass->set ("createObject", new Function ("createObject", 0, GameClass::createObject));
		cClass->set ("getObjectsFromRayCast", new Function ("getObjectsFromRayCast", 0, GameClass::getObjectsFromRayCast));
		cClass->set ("Movables", new Array ("Movables"));

		JavascriptManager::getSingletonPtr ()->set ("Game", cClass);
	}

	Variable *GameClass::getMovableByName (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strString = aryArguments->getElement (0)->toString ()->getValue ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		Class *objMovable = ogrGame->getJavascriptEngine ()->getMovableJSClass (strString);

		return (objMovable);
	}

	Variable *GameClass::createObject (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		double dNumber = aryArguments->getElement (0)->toNumber ()->getValue ();
		std::string strString = aryArguments->getElement (1)->toString ()->getValue ();
		Movable *parent = (Movable *)aryArguments->getElement (2)->toClass ()->getHiddenData ("gameObject");
		Movable *mObject = 0;

		if (dNumber == GOT_ANIMATION_TRACK)
			mObject = ogrGame->getCurrentXMLFile ()->createMovable (ogrGame->getCurrentXMLFile (), (int)dNumber, strString, parent, 0, false);
		else
			mObject = ogrGame->getCurrentXMLFile ()->createMovable (ogrGame->getCurrentXMLFile (), (int)dNumber, strString, parent);

		Class *objReturn = ogrGame->getJavascriptEngine ()->getMovableJSClass (mObject);

		return (objReturn);
	}

	Variable *GameClass::deleteObject (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strString = aryArguments->getElement (0)->toString ()->getValue ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		XmlFile *lvlCurrent = ogrGame->getCurrentXMLFile ();
		Movable *mObject = lvlCurrent->getMovable (strString, true);
		lvlCurrent->addMovableForDeletion (mObject);

		return (0);
	}

	Variable *GameClass::getObjectsFromRayCast (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		RadXML *ogrGame = RadXML::getSingletonPtr ();
		double dX = aryArguments->getElement (0)->toNumber ()->getValue ();
		double dY = aryArguments->getElement (1)->toNumber ()->getValue ();
		double dZ = aryArguments->getElement (2)->toNumber ()->getValue ();
		double dDirX = aryArguments->getElement (3)->toNumber ()->getValue ();
		double dDirY = aryArguments->getElement (4)->toNumber ()->getValue ();
		double dDirZ = aryArguments->getElement (5)->toNumber ()->getValue ();
		std::vector<Ogre::RaySceneQueryResultEntry> aryQuery;
		Array *aryArray = new Array ("");

		aryQuery = rayQuery (Ogre::Vector3 (dX, dY, dZ), Ogre::Vector3 (dDirX, dDirY, dDirZ));

		if (aryQuery.size () > 0)
		{
			for (unsigned int iQdx = 0; iQdx < aryQuery.size (); iQdx++)
			{
				Ogre::MovableObject *eEnt = aryQuery.at (iQdx).movable;

				if (eEnt == 0)
					continue;

				if (eEnt->getUserAny ().isEmpty () == false)
				{
					Movable *objMovable = Ogre::any_cast<Movable *> (eEnt->getUserAny ());
					Class *objObj = ogrGame->getJavascriptEngine ()->getMovableJSClass (objMovable);

					aryArray->addElement (objObj);
				}
			}
		}

		return (aryArray);
	}
}

