#include "RadXmlInput.h"

#include "RadXml.h"

#ifdef defAPI_OGRE3D
	#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
		#include <OGRE/OgreSceneManager.h>
		#include <OGRE/OgreMovableObject.h>
	#else
		#include <OgreSceneManager.h>
		#include <OgreMovableObject.h>
	#endif
#endif

namespace RadXml
{
	void setupInputClass ()
	{
		Class *cClass = new Class ("InputEvent", "InputEvent");
		cClass->set ("key", new Number ("key", 0));
		cClass->set ("buttons", new Array ("buttons"));
		cClass->set ("x", new Number ("x", 0));
		cClass->set ("y", new Number ("y", 0));
		cClass->set ("z", new Number ("z", 0));
		cClass->set ("relX", new Number ("relX", 0));
		cClass->set ("relY", new Number ("relY", 0));
		cClass->set ("relZ", new Number ("relZ", 0));

		JavascriptManager::getSingletonPtr ()->set ("InputEvent", cClass);
	}

	std::vector<Ogre::RaySceneQueryResultEntry> inputRayQuery (Ogre::Camera *camera, Ogre::Vector3 v3dMouseRelPospass)
	{
		Ogre::MovableObject *moObject = 0;
		std::vector<Ogre::RaySceneQueryResultEntry> aryReturn;
		float fMX = v3dMouseRelPospass.x;
		float fMY = v3dMouseRelPospass.y;

		Ogre::Ray rayRay = camera->getCameraToViewportRay (fMX, fMY);

		Ogre::RaySceneQuery *rsqRayQuery = RadXML::getSingletonPtr ()->getSceneManager ()->createRayQuery (rayRay);
		rsqRayQuery->setRay (rayRay);
		rsqRayQuery->setSortByDistance (true);

		Ogre::RaySceneQueryResult rqrResult = rsqRayQuery->execute ();
		Ogre::RaySceneQueryResult::iterator iResult = rqrResult.begin ();
		Ogre::RaySceneQueryResult::iterator iResultEnd = rqrResult.end ();

		while (iResult != iResultEnd)
		{
			moObject = iResult->movable;

			if (moObject != 0)
			{
				if (moObject->getQueryFlags () == defENTITY_OBJECT)
					aryReturn.push_back ((*iResult));
			}

			iResult++;
		}

		rsqRayQuery->clearResults ();
		RadXML::getSingletonPtr ()->getSceneManager ()->destroyQuery (rsqRayQuery);

		return (aryReturn);
	}

	std::vector<Ogre::RaySceneQueryResultEntry> rayQuery (Ogre::Vector3 rayStart, Ogre::Vector3 rayDirection)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		Ogre::MovableObject *moObject = 0;
		std::vector<Ogre::RaySceneQueryResultEntry> aryReturn;
		Ogre::Ray rayRay (rayStart, rayDirection);

		Ogre::RaySceneQuery *rsqRayQuery = ogrGame->getSceneManager ()->createRayQuery (rayRay);
		rsqRayQuery->setRay (rayRay);
		rsqRayQuery->setSortByDistance (true);

		Ogre::RaySceneQueryResult rqrResult = rsqRayQuery->execute ();
		Ogre::RaySceneQueryResult::iterator iResult = rqrResult.begin ();
		Ogre::RaySceneQueryResult::iterator iResultEnd = rqrResult.end ();

		while (iResult != iResultEnd)
		{
			moObject = iResult->movable;

			if (moObject != 0)
			{
				if (moObject->getQueryFlags () == defENTITY_OBJECT)
					aryReturn.push_back ((*iResult));
			}

			iResult++;
		}

		rsqRayQuery->clearResults ();
		ogrGame->getSceneManager ()->destroyQuery (rsqRayQuery);

		return (aryReturn);
	}
}

