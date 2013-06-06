#include "RadXmlEntity.h"

#include "RadXml.h"
#include "RadXmlXmlFile.h"
#include "RadXmlGlobal.h"

#include "xml/xml.h"

#include "common/mai_radxml_system.h"
#include "common/com_radxml_strconv.h"

#include "languages/language.h"

#ifdef defAPI_OGRE3D
	#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
		#include <OGRE/OgreMeshManager.h>
	#else
		#include <OgreMeshManager.h>
	#endif
#endif

namespace RadXml
{
	// Entity
	Entity::Entity (std::string newName)
		: Movable (newName)
	{
		eObj = 0;
		model = "cube";
		material = "";

		addEvent (new Event ("oncollision", ""));

		iType = GOT_ENTITY;
	}

	Entity::~Entity ()
	{
		destroy ();
	}

	void Entity::create (Movable *parent)
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();

		if (model == "cube")
			eObj = mSceneMgr->createEntity ("ent" + name, Ogre::SceneManager::PT_CUBE);
		else if (model == "sphere")
			eObj = mSceneMgr->createEntity ("ent" + name, Ogre::SceneManager::PT_SPHERE);
		else if (model.find ("plane") != std::string::npos)
		{
			std::string entity = model;

			if (Ogre::MeshManager::getSingletonPtr ()->getByName (entity).isNull () == true)
			{
				entity = RadXmlCommon::s13ASCII_StringToLower (entity);
				int iPos = entity.find ("x");

				if (iPos == std::string::npos)
				{
					RadXmlCommon::s13LogMessage (MSG14);
					eObj = mSceneMgr->createEntity ("ent" + name, Ogre::SceneManager::PT_CUBE);

					return;
				}

				std::string strWidth = entity.substr (5);
				std::string strHeight = entity.substr (iPos + 1);
				double dWidth = RadXmlCommon::s13StringToDouble (strWidth);
				double dHeight = RadXmlCommon::s13StringToDouble (strHeight);
				double dD = 0;
				int iSegX = 1;
				int iSegY = 1;
				Ogre::Vector3 v3dNormal = Ogre::Vector3::UNIT_Z;
				Ogre::Vector3 v3dUp = Ogre::Vector3::UNIT_Y;

				Ogre::Plane plnPlane;
				plnPlane.normal = v3dNormal;
				plnPlane.d = dD;

				Ogre::MeshManager::getSingletonPtr ()->createPlane (entity,
								Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
								plnPlane, dWidth, dHeight, iSegX, iSegY, true, 1,
								1, 1, v3dUp);
			}

			eObj = mSceneMgr->createEntity ("ent" + name, entity);
		}
		else
			eObj = mSceneMgr->createEntity ("ent" + name, model);

		if (eObj == 0)
			eObj = mSceneMgr->createEntity ("ent" + name, Ogre::SceneManager::PT_CUBE);

		if (material != "")
			eObj->setMaterialName (material);

		eObj->setQueryFlags (defENTITY_OBJECT);
		eObj->setUserAny (Ogre::Any (this));
		eObj->setVisible (bVisible);

		setParent (parent);
		createSceneNode (eObj, parent);
	}

	void Entity::destroy ()
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();

		destroySceneNode ();

		if (eObj != 0)
		{
			mSceneMgr->destroyEntity (eObj);
			eObj = 0;
		}
	}

	void Entity::setVisible (bool visible)
	{
		bVisible = visible;

		if (eObj != 0)
			eObj->setVisible (visible);
	}

	bool Entity::getVisible ()
	{
		return (bVisible);
	}

	Movable *Entity::clone (std::string newName, Movable *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		Entity *clone = new Entity (*this);

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

	void Entity::parseXML (XmlFile *level, XMLElement *element, Movable *parent)
	{
		parseMovableXML (level, element, parent);

		std::string model = element->getAttributeValue ("model");
		std::string material = element->getAttributeValue ("material");
		std::string oncollision = element->getAttributeValue ("oncollision");
		std::string collides = element->getAttributeValue ("collides");

		if (collides == "")
			collides = "true";

		if (model == "")
			model = "cube";

		setModel (model);
		setMaterial (material);
		setJavascriptToEvent ("oncollision", oncollision);

		collides = RadXmlCommon::s13StringToBool (collides);
	}

	void Entity::setModel (std::string newModel)
	{
		model = newModel;

		if (eObj != 0)
		{
			destroy ();
			create (mParent);
		}
	}

	void Entity::setMaterial (std::string newMaterial)
	{
		material = newMaterial;

		if (eObj != 0)
			eObj->setMaterialName (material);
	}

	// EntityClass
	void EntityClass::setupEntityClass ()
	{
		Class *cClass = new Class ("Entity", "Entity");
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

		cClass->set ("setModel", new Function ("setModel", 0, EntityClass::setModel));
		cClass->set ("getModel", new Function ("getModel", 0, EntityClass::getModel));
		cClass->set ("setMaterial", new Function ("setMaterial", 0, EntityClass::setMaterial));
		cClass->set ("getMaterial", new Function ("getMaterial", 0, EntityClass::getMaterial));
		cClass->set ("getCollidingObject", new Function ("getCollidingObject", 0, EntityClass::getCollidingObject));

		JavascriptManager::getSingletonPtr ()->set ("Entity", cClass);
	}

	Variable *EntityClass::setModel (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		std::string strModel = aryArguments->getElement (0)->toString ()->getValue ();

		if (mObject != 0)
		{
			Entity *eEntity = static_cast<Entity *> (mObject);
			eEntity->setModel (strModel);
		}

		return (0);
	}

	Variable *EntityClass::getModel (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		std::string strReturn = "";

		if (mObject != 0)
		{
			Entity *eEntity = static_cast<Entity *> (mObject);
			strReturn = eEntity->getModel ();
		}

		return (String::New (strReturn));
	}

	Variable *EntityClass::setMaterial (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		std::string strMaterial = aryArguments->getElement (0)->toString ()->getValue ();

		if (mObject != 0)
		{
			Entity *eEntity = static_cast<Entity *> (mObject);
			eEntity->setMaterial (strMaterial);
		}

		return (0);
	}

	Variable *EntityClass::getMaterial (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		std::string strReturn = "";

		if (mObject != 0)
		{
			Entity *eEntity = static_cast<Entity *> (mObject);
			strReturn = eEntity->getMaterial ();
		}

		return (String::New (strReturn));
	}

	Variable *EntityClass::getCollidingObject (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		Class *objObject = 0;

		if (mObject != 0)
		{
			objObject = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
							createMovableJSClass (mObject->collidedWithObject);
		}

		return (objObject);
	}
}

