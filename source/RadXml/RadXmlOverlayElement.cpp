#include "RadXmlOverlayElement.h"

#include "RadXml.h"
#include "RadXmlXmlFile.h"
#include "RadXmlGlobal.h"

#include "xml/xml.h"

namespace RadXml
{
	// OverlayElement
	OverlayElement::OverlayElement (std::string newName)
		: Movable (newName)
	{
		iType = GOT_OVERLAY_ELEMENT;
		elementName = "";
		oeElm = 0;
	}

	OverlayElement::~OverlayElement ()
	{
		destroy ();
	}

	void OverlayElement::parseXML (XmlFile *level, XMLElement *element, Movable *parent)
	{
		parseMovableXML (level, element, parent);
		elementName = element->getAttributeValue ("element");
	}

	void OverlayElement::create (Movable *parent)
	{
		oeElm = Ogre::OverlayManager::getSingletonPtr ()->getOverlayElement (elementName);
		oeElm->setUserAny (Ogre::Any (this));

		setVisible (bVisible);
		setParent (parent);
	}

	void OverlayElement::destroy ()
	{
		//Ogre::OverlayManager::getSingletonPtr ()->destroyOverlayElement (oeElm);
	}

	void OverlayElement::setVisible (bool visible)
	{
		bVisible = visible;
	}

	bool OverlayElement::getVisible ()
	{
		return (bVisible);
	}

	Movable *OverlayElement::clone (std::string newName, Movable *parent)
	{
		OverlayElement *oClone = new OverlayElement (*this);
		oClone->create ();
		oClone->removeAllChildren ();

		for (unsigned int iIdx = 0; iIdx < getNumChildren (); iIdx++)
		{
			Movable *mChild = getChild (iIdx);
			Movable *mSubClone = mChild->clone (oClone->getName () + "_sub", oClone);
		}

		return (oClone);
	}

	void OverlayElement::setCaption (std::string text)
	{
		if (oeElm != 0)
			oeElm->setCaption (text);
	}

	std::string OverlayElement::getCaption ()
	{
		std::string strReturn = "";

		if (oeElm != 0)
			strReturn = oeElm->getCaption ();

		return (strReturn);
	}

	// OverlayElementClass
	void OverlayElementClass::setupOverlayElementClass ()
	{
		Class *cClass = new Class ("OverlayElement", "OverlayElement");
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

		cClass->set ("setCaption", new Function ("setCaption", 0, OverlayElementClass::setCaption));
		cClass->set ("getCaption", new Function ("getCaption", 0, OverlayElementClass::getCaption));
		cClass->set ("setSize", new Function ("setSize", 0, OverlayElementClass::setSize));
		cClass->set ("getSize", new Function ("getSize", 0, OverlayElementClass::getSize));

		JavascriptManager::getSingletonPtr ()->set ("OverlayElement", cClass);
	}

	Variable *OverlayElementClass::setCaption (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		OverlayElement *oeElm = static_cast<OverlayElement *> (objThis->getHiddenData ("gameObject"));
		std::string strString = aryArguments->getElement (0)->toString ()->getValue ();
		oeElm->setCaption (strString);

		return (0);
	}

	Variable *OverlayElementClass::getCaption (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		OverlayElement *oeElm = static_cast<OverlayElement *> (objThis->getHiddenData ("gameObject"));
		std::string strText = oeElm->getCaption ();

		return (String::New (strText));
	}

	Variable *OverlayElementClass::setSize (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		OverlayElement *oeElm = static_cast<OverlayElement *> (objThis->getHiddenData ("gameObject"));
		double dX = 0;
		double dY = 0;

		if (aryArguments->length () == 1)
		{
			Class *cClass = aryArguments->getElement (0)->toClass ();

			dX = cClass->get ("x")->toNumber ()->getValue ();
			dY = cClass->get ("y")->toNumber ()->getValue ();
		}
		else
		{
			dX = aryArguments->getElement (0)->toNumber ()->getValue ();
			dY = aryArguments->getElement (1)->toNumber ()->getValue ();
		}

		Ogre::OverlayElement *oeElement = oeElm->getOverlayElement ();
		oeElement->setWidth (dX);
		oeElement->setHeight (dY);

		return (0);
	}

	Variable *OverlayElementClass::getSize (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		OverlayElement *oeElm = static_cast<OverlayElement *> (objThis->getHiddenData ("gameObject"));

		Ogre::OverlayElement *oeElment = oeElm->getOverlayElement ();
		double dX = oeElment->getHeight ();
		double dY = oeElment->getHeight ();
		Ogre::Vector2 v2dPosition (dX, dY);
		Class *objClone = RadXML::getSingletonPtr ()->getJavascriptEngine ()->getVector2JSClass (v2dPosition.x, v2dPosition.y);

		return (objClone);
	}
}

