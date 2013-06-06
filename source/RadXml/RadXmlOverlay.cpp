#include "RadXmlOverlay.h"

#include "RadXml.h"
#include "RadXmlXmlFile.h"

#include "xml/xml.h"

namespace RadXml
{
	// Overlay
	Overlay::Overlay (std::string newName)
		: Movable (newName)
	{
		iType = GOT_OVERLAY;
		overlay = "";
		oOverlay = 0;
	}

	Overlay::~Overlay ()
	{
		destroy ();
	}

	void Overlay::parseXML (XmlFile *level, XMLElement *element, Movable *parent)
	{
		parseMovableXML (level, element, parent);
		overlay = element->getAttributeValue ("overlay");
	}

	void Overlay::create (Movable *parent)
	{
		oOverlay = Ogre::OverlayManager::getSingletonPtr ()->getByName (overlay);

		setVisible (bVisible);
		setParent (parent);
	}

	void Overlay::destroy ()
	{
		//Ogre::OverlayManager::getSingletonPtr ()->destroy (overlay);
	}

	void Overlay::setVisible (bool visible)
	{
		bVisible = visible;

		if (oOverlay != 0)
		{
			if (bVisible == true)
				oOverlay->show ();
			else
				oOverlay->hide ();
		}
	}

	bool Overlay::getVisible ()
	{
		return (bVisible);
	}

	Movable *Overlay::clone (std::string newName, Movable *parent)
	{
		Overlay *oClone = new Overlay (*this);
		oClone->create ();
		oClone->removeAllChildren ();

		for (unsigned int iIdx = 0; iIdx < getNumChildren (); iIdx++)
		{
			Movable *mChild = getChild (iIdx);
			Movable *mSubClone = mChild->clone (oClone->getName () + "_sub", oClone);
		}

		return (oClone);
	}
}

