#include "RadXmlCamera.h"

#include "RadXml.h"
#include "RadXmlXmlFile.h"
#include "RadXml.h"

#include "xml/xml.h"

#include "common/com_radxml_strconv.h"

#ifdef defAPI_OGRE3D
	#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
		#include <OGRE/OgreRenderWindow.h>
		#include <OGRE/OgreCamera.h>
	#else
		#include <OgreRenderWindow.h>
		#include <OgreCamera.h>
	#endif
#endif

namespace RadXml
{
	void RadXML::setupCamera (Ogre::Camera *camCamera, Ogre::ColourValue color)
	{
		aryRenderWindows->at (0)->removeViewport (0);
		Ogre::Viewport* vp1 = aryRenderWindows->at (0)->addViewport(camCamera);
		vp1->setBackgroundColour(color);

		Ogre::Real rAspectRatio = static_cast <Ogre::Real> (vp1->getActualWidth ()) / 
									static_cast <Ogre::Real> (vp1->getActualHeight ());
		camCamera->setAspectRatio(rAspectRatio);
		camCamera->setFOVy (Ogre::Degree (90) / rAspectRatio);
	}

	// Camera
	Camera::Camera (std::string newName)
		: Movable (newName)
	{
		camCamera = 0;
		numMipMaps = 3;
		focalLength = 1;
		nearClipDistance = 1.0;
		farClipDistance = 10000000000.0;
		enabled = true;

		iType = GOT_CAMERA;
	}

	Camera::~Camera ()
	{
		destroy ();
	}

	void Camera::parseXML (XmlFile *level, XMLElement *element, Movable *parent)
	{
		parseMovableXML (level, element, parent);

		std::string strBgColor = element->getAttributeValue ("bgcolor");
		std::string strFocallength = element->getAttributeValue ("focallength");
		std::string strNearClip = element->getAttributeValue ("nearclip");
		std::string strFarClip = element->getAttributeValue ("farclip");
		std::string srtNumMipMaps = element->getAttributeValue ("nummipmaps");
		std::string strEnabled = element->getAttributeValue ("enabled");
		Ogre::ColourValue cvBgColor = RadXML::parseColourValue (strBgColor);
		bool bEnabled = true;

		if (strEnabled != "")
			bEnabled = RadXmlCommon::s13StringToBool (strEnabled);

		bgColor = cvBgColor;

		if (strFocallength != "")
			focalLength = RadXmlCommon::s13StringToDouble (strFocallength);

		if (strNearClip != "")
			nearClipDistance = RadXmlCommon::s13StringToDouble (strNearClip);

		if (strFarClip != "")
			farClipDistance = RadXmlCommon::s13StringToDouble (strFarClip);

		if (srtNumMipMaps != "")
			numMipMaps = RadXmlCommon::s13StringToInt (srtNumMipMaps);
	}

	void Camera::setVisible (bool visible)
	{
		bVisible = visible;
	}

	bool Camera::getVisible ()
	{
		return (bVisible);
	}

	Movable *Camera::clone (std::string newName, Movable *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		Camera *clone = new Camera (*this);

		clone->setName (newName);
		clone->create (0, parent);
		clone->removeAllChildren ();

		for (unsigned int iIdx = 0; iIdx < getNumChildren (); iIdx++)
		{
			Movable *mChild = getChild (iIdx);
			Movable *mSubClone = mChild->clone (clone->getName () + "_sub", clone);
		}

		ogrGame->getCurrentXMLFile ()->addMovable (clone);

		return (clone);
	}

	void Camera::create (int iWindow, Movable *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		Ogre::SceneManager *mSceneMgr = ogrGame->getSceneManager ();

		camCamera = mSceneMgr->createCamera ("cam" + name);
		camCamera->setFocalLength (focalLength);
		camCamera->setNearClipDistance (nearClipDistance);
		camCamera->setFarClipDistance (farClipDistance);
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps (numMipMaps);

		if (enabled == true)
		{
			ogrGame->setupCamera (camCamera, bgColor);
			ogrGame->setCurrentCamera (camCamera);
		}

		setParent (parent);
		createSceneNode (camCamera, parent);
	}

	void Camera::destroy ()
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();

		destroySceneNode ();

		if (camCamera != 0)
		{
			mSceneMgr->destroyCamera (camCamera);
			camCamera = 0;
		}
	}
}

