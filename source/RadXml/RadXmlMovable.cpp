#include "RadXmlMovable.h"

#include "RadXml.h"
#include "RadXmlXmlFile.h"
#include "RadXmlGlobal.h"

#include "xml/xml.h"

#include "common/mai_radxml_system.h"
#include "common/com_radxml_strconv.h"

#include "languages/language.h"

namespace RadXml
{
	// Movable
	Movable::Movable (std::string newName)
		: Events ()
	{
		init ();
		setName (newName);
	}

	Movable::~Movable ()
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		destroySceneNode ();

		if (mParent != 0)
			mParent->removeChild (this);

		for (unsigned int iIdx = 0; iIdx < getNumChildren (); iIdx++)
		{
			Movable *mChild = getChild (iIdx);

			if (mChild != 0)
			{
				ogrGame->getCurrentXMLFile ()->removeMovable (mChild->name);

				delete mChild;
				mChild = 0;
			}
		}

		if (collidedWithObject != 0)
		{
			if (collidedWithObject->collidedWithObject == this)
				collidedWithObject->collidedWithObject = 0;
		}
	}

	void Movable::init ()
	{
		name = "";
		hashName = 0;
		iType = 0;

		addEvent (new Event ("onclick", ""));

		position = Ogre::Vector3::ZERO;
		scale = Ogre::Vector3 (1, 1, 1);
		rotation = Ogre::Vector3::ZERO;

		bVisible = true;
		bImmovable = false;
		collides = true;

		collidedWithObject = 0;

		snNode = 0;
		mParent = 0;
		rAngle = 0;
		v3dMaxVel = Ogre::Vector3 (150, 150, 150);
		v3dSpeed = Ogre::Vector3::ZERO;
		v3dAccel = Ogre::Vector3::ZERO;
		v3dMoveScale = Ogre::Vector3::ZERO;
		v3dVelocity = Ogre::Vector3::ZERO;
		v3dFriction = Ogre::Vector3 (15, 15, 15);

		v3dOriginalPos = Ogre::Vector3::ZERO;
		bSetOriginalPos = true;
		v3dOriginalScale = Ogre::Vector3::ZERO;
		bSetOriginalScale = true;
		v3dOriginalRotation = Ogre::Vector3::ZERO;
		bSetOriginalRotation = true;
	}

	void Movable::setName (std::string newName)
	{
		name = newName;
		hashName = RadXmlCommon::s13StringToHash (newName);
	}

	std::string Movable::getName ()
	{
		return (name);
	}

	unsigned long Movable::getHash ()
	{
		return (hashName);
	}

	void Movable::createSceneNode (Ogre::MovableObject *object, Movable *parent)
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();

		if (parent == 0)
			snNode = mSceneMgr->getRootSceneNode ()->createChildSceneNode ("snNode" + name);
		else
			snNode = parent->getSceneNode ()->createChildSceneNode ("snNode" + name);

		setPosition (position);
		setScale (scale.x, scale.y, scale.z);
		setRotation (rotation.x, rotation.y, rotation.z);
		snNode->attachObject (object);
	}

	void Movable::destroySceneNode ()
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();

		if (snNode != 0)
		{
			mSceneMgr->destroySceneNode (snNode);
			snNode = 0;
		}
	}

	void Movable::setPosition (double x, double y, double z)
	{
		position.x = x;
		position.y = y;
		position.z = z;

		if (snNode != 0)
			snNode->setPosition (x, y, z);

		if (bSetOriginalPos == true)
		{
			v3dOriginalPos = position;
			bSetOriginalPos = false;
		}
	}

	Ogre::Vector3 Movable::getPosition ()
	{
		Ogre::Vector3 v3dReturn = position;

		if (snNode != 0)
			v3dReturn = snNode->getPosition ();

		return (v3dReturn);
	}

	Ogre::Vector3 Movable::getWorldPosition ()
	{
		Ogre::Vector3 v3dReturn = position;

		if (snNode != 0)
			v3dReturn = snNode->_getDerivedPosition ();

		return (v3dReturn);
	}

	void Movable::translate (double x, double y, double z)
	{
		position.x += x;
		position.y += y;
		position.z += z;

		setPosition (position);
	}

	void Movable::setScale (double x, double y, double z)
	{
		scale = Ogre::Vector3 (x, y, z);

		if (snNode != 0)
			snNode->setScale (x, y, z);

		if (bSetOriginalScale == true)
		{
			v3dOriginalScale = scale;
			bSetOriginalScale = false;
		}
	}

	void Movable::setRotation (double yaw, double pitch, double roll)
	{
		rotation = Ogre::Vector3 (yaw, pitch, roll);

		if (snNode != 0)
		{
			snNode->yaw (Ogre::Degree (yaw));
			snNode->pitch (Ogre::Degree (pitch));
			snNode->roll (Ogre::Degree (roll));
		}

		if (bSetOriginalRotation == true)
		{
			v3dOriginalRotation = rotation;
			bSetOriginalRotation = false;
		}
	}

	Ogre::Vector3 Movable::getRotation ()
	{
		Ogre::Vector3 v3dRotation = Ogre::Vector3::ZERO;

		v3dRotation.x = snNode->getOrientation ().getYaw ().valueDegrees ();
		v3dRotation.y = snNode->getOrientation ().getPitch ().valueDegrees ();
		v3dRotation.z = snNode->getOrientation ().getRoll ().valueDegrees ();

		return (v3dRotation);
	}

	void Movable::setYaw (double yaw)
	{
		if (snNode != 0)
			snNode->yaw (Ogre::Degree (yaw));

		rotation.x = yaw;
	}

	void Movable::setPitch (double pitch)
	{
		if (snNode != 0)
			snNode->pitch (Ogre::Degree (pitch));

		rotation.y = pitch;
	}

	void Movable::setRoll (double roll)
	{
		if (snNode != 0)
			snNode->roll (Ogre::Degree (roll));

		rotation.z = roll;
	}

	void Movable::setVelocity (double x, double y, double z)
	{
		v3dVelocity.x = x;
		v3dVelocity.y = y;
		v3dVelocity.z = z;
	}

	void Movable::setAcceleration (double x, double y, double z)
	{
		v3dAccel.x = x;
		v3dAccel.y = y;
		v3dAccel.z = z;
	}

	void Movable::setMaxVelocity (double x, double y, double z)
	{
		v3dMaxVel.x = x;
		v3dMaxVel.y = y;
		v3dMaxVel.z = z;
	}

	void Movable::setFriction (double x, double y, double z)
	{
		v3dFriction.x = x;
		v3dFriction.y = y;
		v3dFriction.z = z;
	}

	Ogre::Quaternion Movable::getOrientation ()
	{
		Ogre::Quaternion qRot = Ogre::Quaternion::IDENTITY;

		if (snNode != 0)
			qRot = snNode->getOrientation ();

		return (qRot);
	}

	void Movable::moveEntity (Ogre::FrameEvent evtEvent)
	{
		if (snNode == 0)
			return;

		if (bImmovable == true)
			return;

		v3dSpeed.x += v3dAccel.x;
		v3dSpeed.y += v3dAccel.y;
		v3dSpeed.z += v3dAccel.z;

		if (v3dSpeed.x <= -v3dMaxVel.x)
			v3dSpeed.x = -v3dMaxVel.x;

		if (v3dSpeed.x >= v3dMaxVel.x)
			v3dSpeed.x = v3dMaxVel.x;

		if (v3dSpeed.y <= -v3dMaxVel.y)
			v3dSpeed.y = -v3dMaxVel.y;

		if (v3dSpeed.y >= v3dMaxVel.y)
			v3dSpeed.y = v3dMaxVel.y;

		if (v3dSpeed.z <= -v3dMaxVel.z)
			v3dSpeed.z = -v3dMaxVel.z;

		if (v3dSpeed.z >= v3dMaxVel.z)
			v3dSpeed.z = v3dMaxVel.z;

		v3dVelocity.x = v3dSpeed.x;
		v3dVelocity.y = v3dSpeed.y;
		v3dVelocity.z = v3dSpeed.z;

		if (v3dSpeed.x > 0)
			v3dSpeed.x -= v3dFriction.x;

		if (v3dSpeed.x < 0)
			v3dSpeed.x += v3dFriction.x;

		if (v3dSpeed.y > 0)
			v3dSpeed.y -= v3dFriction.y;

		if (v3dSpeed.y < 0)
			v3dSpeed.y += v3dFriction.y;

		if (v3dSpeed.z > 0)
			v3dSpeed.z -= v3dFriction.z;

		if (v3dSpeed.z < 0)
			v3dSpeed.z += v3dFriction.z;

		Ogre::Vector3 v3dPos = snNode->getPosition ();
		v3dPos.x += (v3dVelocity.x * evtEvent.timeSinceLastFrame);
		v3dPos.y += (v3dVelocity.y * evtEvent.timeSinceLastFrame);
		v3dPos.z += (v3dVelocity.z * evtEvent.timeSinceLastFrame);

		setPosition (v3dPos.x, v3dPos.y, v3dPos.z);
	}

	void Movable::setBoundingBoxVisible (bool value)
	{
		if (snNode != 0)
			snNode->showBoundingBox (value);
	}

	bool Movable::getBoundingBoxVisible ()
	{
		if (snNode != 0)
			return (snNode->getShowBoundingBox ());

		return (false);
	}

	void Movable::setLookAt (Ogre::Vector3 target, int transformSpace, Ogre::Vector3 localDirVector)
	{
		if (snNode != 0)
			snNode->lookAt (target, (Ogre::Node::TransformSpace)transformSpace, localDirVector);
	}

	void Movable::parseMovableXML (XmlFile *level, XMLElement *element, Movable *parent)
	{
		std::string subName = element->getAttributeValue ("name");
		std::string strPosition = element->getAttributeValue ("position");
		std::string strScale = element->getAttributeValue ("scale");
		std::string strRotation = element->getAttributeValue ("rotation");
		std::string x = element->getAttributeValue ("x");
		std::string y = element->getAttributeValue ("y");
		std::string z = element->getAttributeValue ("z");
		std::string sx = element->getAttributeValue ("scalex");
		std::string sy = element->getAttributeValue ("scaley");
		std::string sz = element->getAttributeValue ("scalez");
		std::string yaw = element->getAttributeValue ("yaw");
		std::string pitch = element->getAttributeValue ("pitch");
		std::string roll = element->getAttributeValue ("roll");
		std::string visible = element->getAttributeValue ("visible");
		std::string immovable = element->getAttributeValue ("immovable");
		std::string strCollides = element->getAttributeValue ("collides");
		std::string onclick = element->getAttributeValue ("onclick");
		double dX = RadXmlCommon::s13StringToDouble (x);
		double dY = RadXmlCommon::s13StringToDouble (y);
		double dZ = RadXmlCommon::s13StringToDouble (z);
		double dSX = 1;
		double dSY = 1;
		double dSZ = 1;
		double dYaw = RadXmlCommon::s13StringToDouble (yaw);
		double dPitch = RadXmlCommon::s13StringToDouble (pitch);
		double dRoll = RadXmlCommon::s13StringToDouble (roll);
		bool bImmovable = RadXmlCommon::s13StringToBool (immovable);
		bool bVisible = true;
		bool bCollides = true;
		Movable *objObject = 0;

		if (strCollides != "")
			bCollides = RadXmlCommon::s13StringToBool (strCollides);

		if (visible != "")
			bVisible = RadXmlCommon::s13StringToBool (visible);

		if (strPosition != "")
		{
			strPosition = RadXmlCommon::s13RemoveWhitespaces (strPosition);
			std::vector<std::string> aryString = RadXmlCommon::s13StringSplit (strPosition, ",");
			dX = RadXmlCommon::s13StringToDouble (aryString.at (0));
			dY = RadXmlCommon::s13StringToDouble (aryString.at (1));
			dZ = RadXmlCommon::s13StringToDouble (aryString.at (2));
		}

		if (strScale != "")
		{
			strScale = RadXmlCommon::s13RemoveWhitespaces (strScale);
			std::vector<std::string> aryString = RadXmlCommon::s13StringSplit (strScale, ",");
			dSX = RadXmlCommon::s13StringToDouble (aryString.at (0));
			dSY = RadXmlCommon::s13StringToDouble (aryString.at (1));
			dSZ = RadXmlCommon::s13StringToDouble (aryString.at (2));
		}

		if (strRotation != "")
		{
			strRotation = RadXmlCommon::s13RemoveWhitespaces (strRotation);
			std::vector<std::string> aryString = RadXmlCommon::s13StringSplit (strRotation, ",");
			dYaw = RadXmlCommon::s13StringToDouble (aryString.at (0));
			dPitch = RadXmlCommon::s13StringToDouble (aryString.at (1));
			dRoll = RadXmlCommon::s13StringToDouble (aryString.at (2));
		}

		if (sx != "")
			dSX = RadXmlCommon::s13StringToDouble (sx);

		if (sy != "")
			dSY = RadXmlCommon::s13StringToDouble (sy);

		if (sz != "")
			dSZ = RadXmlCommon::s13StringToDouble (sz);

		if (subName == "")
		{
			int iRandName = rand ();
			Movable *objFound = level->getMovable (iRandName, true);

			while (objFound != 0)
			{
				iRandName = rand ();
				objFound = level->getMovable (iRandName, true);
			}

			subName = "obj" + RadXmlCommon::s13IntToString (iRandName);
		}

		scale.x = dSX;
		scale.y = dSY;
		scale.z = dSZ;
		rotation.x = dYaw;
		rotation.y = dPitch;
		rotation.z = dRoll;

		setJavascriptToEvent ("onclick", onclick);
		setPosition (dX, dY, dZ);
		setImmovable (bImmovable);
		setVisible (bVisible);
		setCollides (bCollides);
		setName (subName);
	}

	std::string Movable::getMovableXML ()
	{
		std::string strReturn = "";

		

		return (strReturn);
	}

	// MovableClass
	void MovableClass::setupMovableClass ()
	{
		Class *cClass = new Class ("Movable", "Movable");
		cClass->set ("getName", new Function ("getName", 0, MovableClass::getName));
		cClass->set ("clone", new Function ("clone", 0, MovableClass::clone));
		cClass->set ("translate", new Function ("translate", 0, MovableClass::translate));
		cClass->set ("setPosition", new Function ("setPosition", 0, MovableClass::setPosition));
		cClass->set ("getPosition", new Function ("getPosition", 0, MovableClass::getPosition));
		cClass->set ("getWorldPosition", new Function ("getWorldPosition", 0, MovableClass::getWorldPosition));
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

		JavascriptManager::getSingletonPtr ()->set ("Movable", cClass);
	}

	Variable *MovableClass::getName (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		std::string strName = mObject->getName ();

		return (String::New (strName));
	}

	Variable *MovableClass::translate (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		double dX = 0;
		double dY = 0;
		double dZ = 0;

		if (aryArguments->length () == 1)
		{
			Class *cClass = aryArguments->getElement (0)->toClass ();

			dX = cClass->get ("x")->toNumber ()->getValue ();
			dY = cClass->get ("y")->toNumber ()->getValue ();
			dZ = cClass->get ("z")->toNumber ()->getValue ();
		}
		else
		{
			dX = aryArguments->getElement (0)->toNumber ()->getValue ();
			dY = aryArguments->getElement (1)->toNumber ()->getValue ();
			dZ = aryArguments->getElement (2)->toNumber ()->getValue ();
		}

		mObject->translate (dX, dY, dZ);

		return (0);
	}

	Variable *MovableClass::setPosition (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		double dX = 0;
		double dY = 0;
		double dZ = 0;

		if (aryArguments->length () == 1)
		{
			Class *cClass = aryArguments->getElement (0)->toClass ();

			dX = cClass->get ("x")->toNumber ()->getValue ();
			dY = cClass->get ("y")->toNumber ()->getValue ();
			dZ = cClass->get ("z")->toNumber ()->getValue ();
		}
		else
		{
			dX = aryArguments->getElement (0)->toNumber ()->getValue ();
			dY = aryArguments->getElement (1)->toNumber ()->getValue ();
			dZ = aryArguments->getElement (2)->toNumber ()->getValue ();
		}

		mObject->setPosition (dX, dY, dZ);

		return (0);
	}

	Variable *MovableClass::getPosition (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		RadXML *ogrGame = RadXML::getSingletonPtr ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		Ogre::Vector3 v3dPosition = mObject->getPosition ();
		Class *objClone = ogrGame->getJavascriptEngine ()->\
			getVector3JSClass (v3dPosition.x, v3dPosition.y, v3dPosition.z);

		return (objClone);
	}

	Variable *MovableClass::getWorldPosition (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		Ogre::Vector3 v3dPosition = mObject->getWorldPosition ();
		Class *objClone = ogrGame->getJavascriptEngine ()->getVector3JSClass (v3dPosition.x, v3dPosition.y, v3dPosition.z);

		return (objClone);
	}

	Variable *MovableClass::setScale (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		double dX = 0;
		double dY = 0;
		double dZ = 0;

		if (aryArguments->length () == 1)
		{
			Class *cClass = aryArguments->getElement (0)->toClass ();

			dX = cClass->get ("x")->toNumber ()->getValue ();
			dY = cClass->get ("y")->toNumber ()->getValue ();
			dZ = cClass->get ("z")->toNumber ()->getValue ();
		}
		else
		{
			dX = aryArguments->getElement (0)->toNumber ()->getValue ();
			dY = aryArguments->getElement (1)->toNumber ()->getValue ();
			dZ = aryArguments->getElement (2)->toNumber ()->getValue ();
		}

		mObject->setScale (dX, dY, dZ);

		return (0);
	}

	Variable *MovableClass::getScale (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		Class *objClone = ogrGame->getJavascriptEngine ()->getVector3JSClass (mObject->scale.x, mObject->scale.y, mObject->scale.z);

		return (objClone);
	}

	Variable *MovableClass::setRotation (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		double dX = 0;
		double dY = 0;
		double dZ = 0;

		if (aryArguments->length () == 1)
		{
			Class *cClass = aryArguments->getElement (0)->toClass ();

			dX = cClass->get ("x")->toNumber ()->getValue ();
			dY = cClass->get ("y")->toNumber ()->getValue ();
			dZ = cClass->get ("z")->toNumber ()->getValue ();
		}
		else
		{
			dX = aryArguments->getElement (0)->toNumber ()->getValue ();
			dY = aryArguments->getElement (1)->toNumber ()->getValue ();
			dZ = aryArguments->getElement (2)->toNumber ()->getValue ();
		}

		mObject->setRotation (dX, dY, dZ);

		return (0);
	}

	Variable *MovableClass::setYaw (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		double dX = aryArguments->getElement (0)->toNumber ()->getValue ();
		mObject->setYaw (dX);

		return (0);
	}

	Variable *MovableClass::getYaw (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		Ogre::Vector3 v3dRotation = mObject->getRotation ();

		return (Number::New (v3dRotation.x));
	}

	Variable *MovableClass::setPitch (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		double dX = aryArguments->getElement (0)->toNumber ()->getValue ();
		mObject->setPitch (dX);

		return (0);
	}

	Variable *MovableClass::getPitch (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		Ogre::Vector3 v3dRotation = mObject->getRotation ();

		return (Number::New (v3dRotation.y));
	}

	Variable *MovableClass::setRoll (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		double dX = aryArguments->getElement (0)->toNumber ()->getValue ();
		mObject->setRoll (dX);

		return (0);
	}

	Variable *MovableClass::getRoll (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		Ogre::Vector3 v3dRotation = mObject->getRotation ();

		return (Number::New (v3dRotation.z));
	}

	Variable *MovableClass::getOrientation (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->getQuaternionJSClass (mObject->getOrientation ());

		return (objReturn);
	}

	Variable *MovableClass::setVelocity (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		double dX = aryArguments->getElement (0)->toNumber ()->getValue ();
		double dY = aryArguments->getElement (1)->toNumber ()->getValue ();
		double dZ = aryArguments->getElement (2)->toNumber ()->getValue ();
		mObject->setVelocity (dX, dY, dZ);

		return (0);
	}

	Variable *MovableClass::getVelocity (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		Ogre::Vector3 v3dVelocity = mObject->getVelocity ();
		Class *objClone = ogrGame->getJavascriptEngine ()->getVector3JSClass (v3dVelocity.x, v3dVelocity.y, v3dVelocity.z);

		return (objClone);
	}

	Variable *MovableClass::setAcceleration (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));

		double dX = 0;
		double dY = 0;
		double dZ = 0;

		if (aryArguments->length () == 1)
		{
			Class *cClass = aryArguments->getElement (0)->toClass ();

			dX = cClass->get ("x")->toNumber ()->getValue ();
			dY = cClass->get ("y")->toNumber ()->getValue ();
			dZ = cClass->get ("z")->toNumber ()->getValue ();
		}
		else
		{
			dX = aryArguments->getElement (0)->toNumber ()->getValue ();
			dY = aryArguments->getElement (1)->toNumber ()->getValue ();
			dZ = aryArguments->getElement (2)->toNumber ()->getValue ();
		}

		mObject->setAcceleration (dX, dY, dZ);

		return (0);
	}

	Variable *MovableClass::getAcceleration (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		Ogre::Vector3 v3dVelocity = mObject->getAcceleration ();
		Class *objClone = ogrGame->getJavascriptEngine ()->getVector3JSClass (v3dVelocity.x, v3dVelocity.y, v3dVelocity.z);

		return (objClone);
	}

	Variable *MovableClass::setFriction (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		double dX = 0;
		double dY = 0;
		double dZ = 0;

		if (aryArguments->length () == 1)
		{
			Class *cClass = aryArguments->getElement (0)->toClass ();

			dX = cClass->get ("x")->toNumber ()->getValue ();
			dY = cClass->get ("y")->toNumber ()->getValue ();
			dZ = cClass->get ("z")->toNumber ()->getValue ();
		}
		else
		{
			dX = aryArguments->getElement (0)->toNumber ()->getValue ();
			dY = aryArguments->getElement (1)->toNumber ()->getValue ();
			dZ = aryArguments->getElement (2)->toNumber ()->getValue ();
		}

		mObject->setFriction (dX, dY, dZ);

		return (0);
	}

	Variable *MovableClass::getFriction (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		Ogre::Vector3 v3dData = mObject->getFriction ();
		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->getVector3JSClass (v3dData.x, v3dData.y, v3dData.z);

		return (objReturn);
	}

	Variable *MovableClass::setMaxVelocity (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		double dX = 0;
		double dY = 0;
		double dZ = 0;

		if (aryArguments->length () == 1)
		{
			Class *cClass = aryArguments->getElement (0)->toClass ();

			dX = cClass->get ("x")->toNumber ()->getValue ();
			dY = cClass->get ("y")->toNumber ()->getValue ();
			dZ = cClass->get ("z")->toNumber ()->getValue ();
		}
		else
		{
			dX = aryArguments->getElement (0)->toNumber ()->getValue ();
			dY = aryArguments->getElement (1)->toNumber ()->getValue ();
			dZ = aryArguments->getElement (2)->toNumber ()->getValue ();
		}

		mObject->setMaxVelocity (dX, dY, dZ);

		return (0);
	}

	Variable *MovableClass::getMaxVelocity (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		Ogre::Vector3 v3dData = mObject->getMaxVelocity ();
		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->getVector3JSClass (v3dData.x, v3dData.y, v3dData.z);

		return (objReturn);
	}

	Variable *MovableClass::clone (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		Movable *mParent = 0;
		std::string strName = aryArguments->getElement (0)->toString ()->getValue ();
		std::string strParent = "";

		if (aryArguments->length () > 1)
			strParent = aryArguments->getElement (1)->toString ()->getValue ();

		if (strParent != "")
		{
			XmlFile *lvlCurrent = ogrGame->getCurrentXMLFile ();
			mParent = lvlCurrent->getMovable (strParent, true);
		}

		Class *objReturn = 0;

		try
		{
			Movable *objClone = mObject->clone (strName, mParent);
			objReturn = ogrGame->getJavascriptEngine ()->getMovableJSClass (objClone);
		}
		catch (Ogre::Exception &ex)
		{
			RadXmlCommon::s13LogMessage ((std::string)MSG15 + ex.getFullDescription () + 
									(std::string)MSG16 + Ogre::StringConverter::toString (ex.getLine ()) + 
									(std::string)MSG17 + ex.getSource ());
		}

		return (objReturn);
	}

	Variable *MovableClass::setVisible (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		bool bVisible = aryArguments->getElement (0)->toBoolean ()->getValue ();

		mObject->setVisible (bVisible);

		return (0);
	}

	Variable *MovableClass::getVisible (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));

		return (Boolean::New (mObject->getVisible ()));
	}

	Variable *MovableClass::setCollides (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		bool bVisible = aryArguments->getElement (0)->toBoolean ()->getValue ();

		mObject->setCollides (bVisible);

		return (0);
	}

	Variable *MovableClass::getCollides (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));

		return (Boolean::New (mObject->getCollides ()));
	}

	Variable *MovableClass::setBoundingBoxVisible (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		bool bVisible = aryArguments->getElement (0)->toBoolean ()->getValue ();

		mObject->setBoundingBoxVisible (bVisible);

		return (0);
	}

	Variable *MovableClass::getBoundingBoxVisible (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));

		return (Boolean::New (mObject->getBoundingBoxVisible ()));
	}

	Variable *MovableClass::setLookAt (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		Movable *mObject = static_cast<Movable *> (objThis->getHiddenData ("gameObject"));
		Ogre::Vector3 target = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3FromJSObject (aryArguments->getElement (0)->toClass ());
		double dValue = Ogre::Node::TS_WORLD;
		Ogre::Vector3 localDir = Ogre::Vector3::NEGATIVE_UNIT_Z;

		if (aryArguments->length () > 1)
			dValue = aryArguments->getElement (1)->toNumber ()->getValue ();

		if (aryArguments->length () > 2)
		{
			localDir = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
				getVector3FromJSObject (aryArguments->getElement (2)->toClass ());
		}

		mObject->setLookAt (target, dValue, localDir);

		return (0);
	}
}

