#include "RadXmlLight.h"

#include "RadXml.h"
#include "RadXmlXmlFile.h"

#include "xml/xml.h"

#include "common/com_radxml_strconv.h"

namespace RadXml
{
	// Light
	Light::Light (std::string newName)
		: Movable (newName)
	{
		lLight = 0;
		type = LT_POINT;
		diffuse = Ogre::ColourValue::White;
		specular = Ogre::ColourValue::White;

		iType = GOT_LIGHT;
	}

	Light::~Light ()
	{
		destroy ();
	}

	void Light::parseXML (XmlFile *level, XMLElement *element, Movable *parent)
	{
		parseMovableXML (level, element, parent);

		std::string strType = RadXmlCommon::s13ASCII_StringToLower (element->getAttributeValue ("type"));
		std::string strDiffuse = element->getAttributeValue ("diffuse");
		std::string strSpecular = element->getAttributeValue ("specular");

		if (strDiffuse != "")
			diffuse = RadXML::parseColourValue (strDiffuse);

		if (strSpecular != "")
			specular = RadXML::parseColourValue (strSpecular);

		if (strType == "point")
			type = LT_POINT;
		else if (strType == "spotlight")
			type = LT_SPOTLIGHT;
		else if (strType == "directional")
			type = LT_DIRECTIONAL;
	}

	void Light::setVisible (bool visible)
	{
		bVisible = visible;

		if (lLight != 0)
			lLight->setVisible (visible);
	}

	bool Light::getVisible ()
	{
		return (bVisible);
	}

	Movable *Light::clone (std::string newName, Movable *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		Light *clone = new Light (*this);

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

	void Light::create (Movable *parent)
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();
		lLight = mSceneMgr->createLight ("lLight" + name);

		if (type == LT_POINT)
			lLight->setType (Ogre::Light::LT_POINT);
		else if (type == LT_SPOTLIGHT)
			lLight->setType (Ogre::Light::LT_SPOTLIGHT);
		else if (type == LT_DIRECTIONAL)
			lLight->setType (Ogre::Light::LT_DIRECTIONAL);

		lLight->setDiffuseColour (diffuse);
		lLight->setSpecularColour (specular);
		lLight->setVisible (bVisible);
		lLight->setQueryFlags (defENTITY_OBJECT);
		lLight->setUserAny (Ogre::Any (this));

		setParent (parent);
		createSceneNode (lLight, parent);
	}

	void Light::destroy ()
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();

		destroySceneNode ();

		if (lLight != 0)
		{
			mSceneMgr->destroyLight (lLight);
			lLight = 0;
		}
	}
}

