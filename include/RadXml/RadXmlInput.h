#ifndef _RADXML_INPUT_H_
	#define _RADXML_INPUT_H_

	#ifdef defAPI_OGRE3D
		#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
			#include <OGRE/OgreVector3.h>
			#include <OGRE/OgreSceneQuery.h>
			#include <OGRE/OgreCamera.h>
		#else
			#include <OgreVector3.h>
			#include <OgreSceneQuery.h>
			#include <OgreCamera.h>
		#endif
	#endif

	#include <vector>

	#include "RadXmlPreprocessor.h"

	namespace RadXml
	{
		void setupInputClass ();

		std::vector<Ogre::RaySceneQueryResultEntry> inputRayQuery (Ogre::Camera *camera, Ogre::Vector3 v3dMouseRelPospass);
		std::vector<Ogre::RaySceneQueryResultEntry> rayQuery (Ogre::Vector3 rayStart, Ogre::Vector3 rayDirection);
	}
#endif

