#include "RadXmlAnimationTrack.h"

#include "RadXml.h"
#include "RadXmlXmlFile.h"
#include "RadXmlGlobal.h"

#include "xml/xml.h"

#include "common/com_radxml_strconv.h"

namespace RadXml
{
	// AnimationTrack
	AnimationTrack::AnimationTrack ()
	{
		aAnim = 0;
		asAnimState = 0;
		snAttachedNode = 0;

		rStart = 0;
		rLength = 0;
		imMode = Ogre::Animation::IM_LINEAR;
		bLoop = false;
		bEnabled = true;

		lstPositions = new std::list<Ogre::Vector4> ();
		lstScalings = new std::list<Ogre::Vector4> ();
		lstRotations = new std::list<Ogre::Vector4> ();
	}

	AnimationTrack::~AnimationTrack ()
	{
		if (lstPositions != 0)
		{
			delete lstPositions;
			lstPositions = 0;
		}

		if (lstScalings != 0)
		{
			delete lstScalings;
			lstScalings = 0;
		}

		if (lstRotations != 0)
		{
			delete lstRotations;
			lstRotations = 0;
		}
	}

	void AnimationTrack::createAnimationTrack (std::string newName, Ogre::SceneManager *mSceneMgr, Ogre::SceneNode *parent, bool bCreateNewSceneNode)
	{
		std::list<Ogre::Vector4> *lstPositions = getPositions ();
		std::list<Ogre::Vector4> *lstScalings = getScalings ();
		std::list<Ogre::Vector4> *lstRotations = getRotations ();
		//std::list<Ogre::Vector4> *lstFades = getFades ();

		/// @bug Possible bug, is using std::map too slow? Might wanna use std::list instead
		std::map<Ogre::Real, std::list <AnimationTrack_State> > hshOrganized;

		{
			{
				std::list<Ogre::Vector4>::iterator itBegin = lstPositions->begin ();
				std::list<Ogre::Vector4>::iterator itEnd = lstPositions->end ();

				while (itBegin != itEnd)
				{
					AnimationTrack_State atsState;
					Ogre::Vector4 v4dObj = (*itBegin);

					atsState.iType = ATS_POSITION;
					atsState.v3dData.x = v4dObj.x;
					atsState.v3dData.y = v4dObj.y;
					atsState.v3dData.z = v4dObj.z;
					atsState.rKeyFrame = v4dObj.w;

					std::map<Ogre::Real, std::list <AnimationTrack_State> >::iterator itFound = hshOrganized.find (atsState.rKeyFrame);
					std::map<Ogre::Real, std::list <AnimationTrack_State> >::iterator itEnd = hshOrganized.end ();

					if (itFound != itEnd)
						((*itFound).second).push_back (atsState);
					else
					{
						std::list <AnimationTrack_State> lstNewStates;
						lstNewStates.push_back (atsState);
						hshOrganized.insert (std::pair <Ogre::Real, std::list <AnimationTrack_State> > (atsState.rKeyFrame, lstNewStates));
					}

					itBegin++;
				}
			}

			{
				std::list<Ogre::Vector4>::iterator itBegin = lstScalings->begin ();
				std::list<Ogre::Vector4>::iterator itEnd = lstScalings->end ();

				while (itBegin != itEnd)
				{
					AnimationTrack_State atsState;
					Ogre::Vector4 v4dObj = (*itBegin);

					atsState.iType = ATS_SCALE;
					atsState.v3dData.x = v4dObj.x;
					atsState.v3dData.y = v4dObj.y;
					atsState.v3dData.z = v4dObj.z;
					atsState.rKeyFrame = v4dObj.w;

					std::map<Ogre::Real, std::list <AnimationTrack_State> >::iterator itFound = hshOrganized.find (atsState.rKeyFrame);
					std::map<Ogre::Real, std::list <AnimationTrack_State> >::iterator itEnd = hshOrganized.end ();

					if (itFound != itEnd)
						((*itFound).second).push_back (atsState);
					else
					{
						std::list <AnimationTrack_State> lstNewStates;
						lstNewStates.push_back (atsState);
						hshOrganized.insert (std::pair <Ogre::Real, std::list <AnimationTrack_State> > (atsState.rKeyFrame, lstNewStates));
					}

					itBegin++;
				}
			}

			{
				std::list<Ogre::Vector4>::iterator itBegin = lstRotations->begin ();
				std::list<Ogre::Vector4>::iterator itEnd = lstRotations->end ();

				while (itBegin != itEnd)
				{
					AnimationTrack_State atsState;
					Ogre::Vector4 v4dObj = (*itBegin);

					atsState.iType = ATS_ROTATE;
					atsState.v3dData.x = v4dObj.x;
					atsState.v3dData.y = v4dObj.y;
					atsState.v3dData.z = v4dObj.z;
					atsState.rKeyFrame = v4dObj.w;

					std::map<Ogre::Real, std::list <AnimationTrack_State> >::iterator itFound = hshOrganized.find (atsState.rKeyFrame);
					std::map<Ogre::Real, std::list <AnimationTrack_State> >::iterator itEnd = hshOrganized.end ();

					if (itFound != itEnd)
						((*itFound).second).push_back (atsState);
					else
					{
						std::list <AnimationTrack_State> lstNewStates;
						lstNewStates.push_back (atsState);
						hshOrganized.insert (std::pair <Ogre::Real, std::list <AnimationTrack_State> > (atsState.rKeyFrame, lstNewStates));
					}

					itBegin++;
				}
			}
		}

		aAnim = mSceneMgr->createAnimation ("ani" + newName, getLength ());
		aAnim->setInterpolationMode (static_cast <Ogre::Animation::InterpolationMode> (getInterpolationMode ()));

		Ogre::SceneNode *snTemp = 0;

		if (snTemp == 0)
			snTemp = snAttachedNode;

		Ogre::NodeAnimationTrack *natTrack = aAnim->createNodeTrack (0, snTemp);
		Ogre::TransformKeyFrame *tknNode = 0;

		std::map<Ogre::Real, std::list <AnimationTrack_State> >::iterator itBegin = hshOrganized.begin ();
		std::map<Ogre::Real, std::list <AnimationTrack_State> >::iterator itEnd = hshOrganized.end ();

		while (itBegin != itEnd)
		{
			Ogre::Real rKeyFrame = (*itBegin).first;
			std::list <AnimationTrack_State> lstStates = (*itBegin).second;
			tknNode = natTrack->createNodeKeyFrame (rKeyFrame);

			std::list <AnimationTrack_State>::iterator itBegin2 = lstStates.begin ();
			std::list <AnimationTrack_State>::iterator itEnd2 = lstStates.end ();

			while (itBegin2 != itEnd2)
			{
				switch ((*itBegin2).iType)
				{
					case ATS_POSITION:
						tknNode->setTranslate ((*itBegin2).v3dData);
						break;
					case ATS_SCALE:
						tknNode->setScale ((*itBegin2).v3dData);
						break;
					case ATS_ROTATE:
						{
							/// @bug I'm thinking this is too slow...
							Ogre::SceneNode *snNode = mSceneMgr->createSceneNode ("temp");
							snNode->pitch (Ogre::Degree ((*itBegin2).v3dData.x));
							snNode->roll (Ogre::Degree ((*itBegin2).v3dData.y));
							snNode->yaw (Ogre::Degree ((*itBegin2).v3dData.z));

							Ogre::Quaternion qRot = snNode->getOrientation ();
							mSceneMgr->destroySceneNode ("temp");

							tknNode->setRotation (qRot);
						}
						break;
				}

				itBegin2++;
			}

			itBegin++;
		}

		asAnimState = mSceneMgr->createAnimationState ("ani" + newName);
		asAnimState->setLoop (isLooping ());
		asAnimState->setEnabled (isEnabled ());
		asAnimState->setTimePosition (getStart ());

		/*if (bCreateNewSceneNode == true)
			createSceneNode (mSceneMgr, parent);

		snSceneNode->attachObject ((Ogre::MovableObject *)aAnim);*/
	}

	void AnimationTrack::destroyAnimationTrack (Ogre::SceneManager *mSceneMgr)
	{
		if (asAnimState != 0)
		{
			mSceneMgr->destroyAnimationState (asAnimState->getAnimationName ());
			asAnimState = 0;
		}

		if (aAnim != 0)
		{
			mSceneMgr->destroyAnimation (aAnim->getName ());
			aAnim = 0;
		}
	}

	void AnimationTrack::addPosition (Ogre::Vector4 position)
	{
		lstPositions->push_back (position);
	}

	void AnimationTrack::addScale (Ogre::Vector4 scale)
	{
		lstScalings->push_back (scale);
	}

	void AnimationTrack::addRotation (Ogre::Vector4 rotation)
	{
		lstRotations->push_back (rotation);
	}

	void AnimationTrack::setPositions (std::list<Ogre::Vector4> *positions)
	{
		delete lstPositions;
		lstPositions = 0;

		lstPositions = positions;
	}

	void AnimationTrack::setScalings (std::list<Ogre::Vector4> *scalings)
	{
		delete lstScalings;
		lstScalings = 0;

		lstScalings = scalings;
	}

	void AnimationTrack::setRotations (std::list<Ogre::Vector4> *rotations)
	{
		delete lstRotations;
		lstRotations = 0;

		lstRotations = rotations;
	}

	void AnimationTrack::setFades (std::list<Ogre::Vector4> *fades)
	{
	}

	// AnimationTrackObject
	AnimationTrackObject::AnimationTrackObject (std::string newName)
		: Movable (newName)
	{
		atTrack = 0;
		attachObject = 0;
		length = 0;
		enabled = true;
		loop = false;
		firstTimeRunning = true;

		iType = GOT_ANIMATION_TRACK;

		addEvent (new Event ("onstart", ""));
		addEvent (new Event ("onend", ""));
	}

	AnimationTrackObject::~AnimationTrackObject ()
	{
		destroy ();
	}

	void AnimationTrackObject::parseXML (XmlFile *level, XMLElement *element, Movable *parent)
	{
		parseMovableXML (level, element, parent);

		std::string strAttachObject = element->getAttributeValue ("attach");
		std::string strLength = element->getAttributeValue ("length");
		std::string strLoop = element->getAttributeValue ("loop");
		std::string strEnabled = element->getAttributeValue ("enabled");
		std::string strOnstart = element->getAttributeValue ("onstart");
		std::string strOnend = element->getAttributeValue ("onend");
		std::vector<XMLElement *> aryNodes = element->findAllTags ("animationnode");

		for (unsigned int iIdx = 0; iIdx < aryNodes.size (); iIdx++)
		{
			XMLElement *xmlElement = aryNodes.at (iIdx);
			std::string subName2 = xmlElement->getAttributeValue ("name");
			std::string position2 = xmlElement->getAttributeValue ("position");
			std::string scale2 = xmlElement->getAttributeValue ("scale");
			std::string rotation2 = xmlElement->getAttributeValue ("rotation");
			std::string x2 = xmlElement->getAttributeValue ("x");
			std::string y2 = xmlElement->getAttributeValue ("y");
			std::string z2 = xmlElement->getAttributeValue ("z");
			std::string sx2 = xmlElement->getAttributeValue ("scalex");
			std::string sy2 = xmlElement->getAttributeValue ("scaley");
			std::string sz2 = xmlElement->getAttributeValue ("scalez");
			std::string yaw2 = xmlElement->getAttributeValue ("yaw");
			std::string pitch2 = xmlElement->getAttributeValue ("pitch");
			std::string roll2 = xmlElement->getAttributeValue ("roll");
			std::string keyframe = xmlElement->getAttributeValue ("keyframe");
			double dX2 = RadXmlCommon::s13StringToDouble (x2);
			double dY2 = RadXmlCommon::s13StringToDouble (y2);
			double dZ2 = RadXmlCommon::s13StringToDouble (z2);
			double dSX2 = 1;
			double dSY2 = 1;
			double dSZ2 = 1;
			double dYaw2 = RadXmlCommon::s13StringToDouble (yaw2);
			double dPitch2 = RadXmlCommon::s13StringToDouble (pitch2);
			double dRoll2 = RadXmlCommon::s13StringToDouble (roll2);
			double dKeyFrame = RadXmlCommon::s13StringToDouble (keyframe);

			if (position2 != "")
			{
				position2 = RadXmlCommon::s13RemoveWhitespaces (position2);
				std::vector<std::string> aryString = RadXmlCommon::s13StringSplit (position2, ",");
				dX2 = RadXmlCommon::s13StringToDouble (aryString.at (0));
				dY2 = RadXmlCommon::s13StringToDouble (aryString.at (1));
				dZ2 = RadXmlCommon::s13StringToDouble (aryString.at (2));
			}

			if (scale2 != "")
			{
				scale2 = RadXmlCommon::s13RemoveWhitespaces (scale2);
				std::vector<std::string> aryString = RadXmlCommon::s13StringSplit (scale2, ",");
				dSX2 = RadXmlCommon::s13StringToDouble (aryString.at (0));
				dSY2 = RadXmlCommon::s13StringToDouble (aryString.at (1));
				dSZ2 = RadXmlCommon::s13StringToDouble (aryString.at (2));
			}

			if (rotation2 != "")
			{
				rotation2 = RadXmlCommon::s13RemoveWhitespaces (rotation2);
				std::vector<std::string> aryString = RadXmlCommon::s13StringSplit (rotation2, ",");
				dYaw2 = RadXmlCommon::s13StringToDouble (aryString.at (0));
				dPitch2 = RadXmlCommon::s13StringToDouble (aryString.at (1));
				dRoll2 = RadXmlCommon::s13StringToDouble (aryString.at (2));
			}

			if (sx2 != "")
				dSX2 = RadXmlCommon::s13StringToDouble (sx2);

			if (sy2 != "")
				dSY2 = RadXmlCommon::s13StringToDouble (sy2);

			if (sz2 != "")
				dSZ2 = RadXmlCommon::s13StringToDouble (sz2);

			Ogre::Vector4 positionFrame (dX2, dY2, dZ2, dKeyFrame);
			Ogre::Vector4 scaleFrame (dSX2, dSY2, dSZ2, dKeyFrame);
			Ogre::Vector4 rotationFrame (dYaw2, dPitch2, dRoll2, dKeyFrame);

			aryPositions.push_back (positionFrame);
			aryScales.push_back (scaleFrame);
			aryRotations.push_back (rotationFrame);
		}

		if (strAttachObject != "")
			attachObject = level->getMovable (strAttachObject);

		if (strLength != "")
			length = RadXmlCommon::s13StringToDouble (strLength);

		if (strLoop != "")
			loop = RadXmlCommon::s13StringToBool (strLoop, false);

		if (strEnabled != "")
			enabled = RadXmlCommon::s13StringToBool (strEnabled, false);

		if (strOnstart != "")
			setJavascriptToEvent ("onstart", strOnstart);

		if (strOnend != "")
			setJavascriptToEvent ("onend", strOnend);
	}

	void AnimationTrackObject::addNode (Ogre::Vector4 position, Ogre::Vector4 scale, Ogre::Vector4 rotation)
	{
		aryPositions.push_back (position);
		aryScales.push_back (scale);
		aryRotations.push_back (rotation);
	}

	void AnimationTrackObject::editNode (unsigned int index, Ogre::Vector4 position, Ogre::Vector4 scale, Ogre::Vector4 rotation)
	{
		aryPositions.at (index) = position;
		aryScales.at (index) = scale;
		aryRotations.at (index) = rotation;
	}

	void AnimationTrackObject::removeNode (unsigned int index)
	{
		std::vector<Ogre::Vector4>::iterator itBegin = aryPositions.begin ();
		itBegin += index;
		aryPositions.erase (itBegin);

		itBegin = aryScales.begin ();
		itBegin += index;
		aryScales.erase (itBegin);

		itBegin = aryRotations.begin ();
		itBegin += index;
		aryRotations.erase (itBegin);
	}

	Ogre::Vector4 AnimationTrackObject::getPositionNode (unsigned int index)
	{
		return (aryPositions.at (index));
	}

	Ogre::Vector4 AnimationTrackObject::getScaleNode (unsigned int index)
	{
		return (aryScales.at (index));
	}

	Ogre::Vector4 AnimationTrackObject::getRotationNode (unsigned int index)
	{
		return (aryRotations.at (index));
	}

	void AnimationTrackObject::setVisible (bool visible)
	{
		bVisible = visible;
	}

	bool AnimationTrackObject::getVisible ()
	{
		return (bVisible);
	}

	Movable *AnimationTrackObject::clone (std::string newName, Movable *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		AnimationTrackObject *clone = new AnimationTrackObject (*this);

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

	void AnimationTrackObject::create (Movable *parent)
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();

		atTrack = new AnimationTrack ();
		atTrack->setAttach (attachObject->getSceneNode ());

		for (unsigned int iIdx = 0; iIdx < aryPositions.size (); iIdx++)
			atTrack->addPosition (aryPositions.at (iIdx));

		for (unsigned int iIdx = 0; iIdx < aryScales.size (); iIdx++)
			atTrack->addScale (aryScales.at (iIdx));

		for (unsigned int iIdx = 0; iIdx < aryRotations.size (); iIdx++)
			atTrack->addRotation (aryRotations.at (iIdx));

		atTrack->setEnabled (enabled);
		atTrack->setLooping (loop);
		atTrack->setLength (length);
		atTrack->createAnimationTrack ("ani" + name, mSceneMgr);

		setParent (parent);
	}

	void AnimationTrackObject::destroy ()
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();

		destroySceneNode ();

		if (atTrack != 0)
		{
			atTrack->destroyAnimationTrack (mSceneMgr);
			delete atTrack;
			atTrack = 0;
		}
	}

	// AnimationTrackClass
	void AnimationTrackClass::setupAnimationTrackClass ()
	{
		Class *cClass = new Class ("AnimationTrack", "AnimationTrack");
		cClass->set ("getName", new Function ("getName", 0, MovableClass::getName));
		cClass->set ("clone", new Function ("clone", 0, MovableClass::clone));
		cClass->set ("addNode", new Function ("addNode", 0, AnimationTrackClass::addNode));
		cClass->set ("editNode", new Function ("editNode", 0, AnimationTrackClass::editNode));
		cClass->set ("removeNode", new Function ("removeNode", 0, AnimationTrackClass::removeNode));
		cClass->set ("getPositionNode", new Function ("getPositionNode", 0, AnimationTrackClass::getPositionNode));
		cClass->set ("getScaleNode", new Function ("getScaleNode", 0, AnimationTrackClass::getScaleNode));
		cClass->set ("getRotationNode", new Function ("getRotationNode", 0, AnimationTrackClass::getRotationNode));
		cClass->set ("play", new Function ("play", 0, AnimationTrackClass::play));
		cClass->set ("isPlaying", new Function ("isPlaying", 0, AnimationTrackClass::isPlaying));
		cClass->set ("stop", new Function ("stop", 0, AnimationTrackClass::stop));
		cClass->set ("setLength", new Function ("setLength", 0, AnimationTrackClass::setLength));
		cClass->set ("getLength", new Function ("getLength", 0, AnimationTrackClass::getLength));
		cClass->set ("setLoop", new Function ("setLoop", 0, AnimationTrackClass::setLoop));
		cClass->set ("getLoop", new Function ("getLoop", 0, AnimationTrackClass::getLoop));
		cClass->set ("setAttach", new Function ("setAttach", 0, AnimationTrackClass::setAttach));
		cClass->set ("getAttach", new Function ("getAttach", 0, AnimationTrackClass::getAttach));
		cClass->set ("create", new Function ("create", 0, AnimationTrackClass::create));

		JavascriptManager::getSingletonPtr ()->set ("AnimationTrack", cClass);
	}

	Variable *AnimationTrackClass::addNode (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		RadXML *ogrGame = RadXML::getSingletonPtr ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));
		Ogre::Vector4 v4dPosition = Ogre::Vector4::ZERO;
		Ogre::Vector4 v4dScale = Ogre::Vector4 (1, 1, 1, 0);
		Ogre::Vector4 v4dRotation = Ogre::Vector4::ZERO;

		Class *objPosVector = aryArguments->getElement (0)->toClass ();
		Class *objScaleVector = aryArguments->getElement (1)->toClass ();
		Class *objRotationVector = aryArguments->getElement (2)->toClass ();

		v4dPosition.x = objPosVector->get ("x")->toNumber ()->getValue ();
		v4dPosition.y = objPosVector->get ("y")->toNumber ()->getValue ();
		v4dPosition.z = objPosVector->get ("z")->toNumber ()->getValue ();
		v4dPosition.w = objPosVector->get ("w")->toNumber ()->getValue ();

		v4dScale.x = objScaleVector->get ("x")->toNumber ()->getValue ();
		v4dScale.y = objScaleVector->get ("y")->toNumber ()->getValue ();
		v4dScale.z = objScaleVector->get ("z")->toNumber ()->getValue ();
		v4dScale.w = objScaleVector->get ("w")->toNumber ()->getValue ();

		v4dRotation.x = objRotationVector->get ("x")->toNumber ()->getValue ();
		v4dRotation.y = objRotationVector->get ("y")->toNumber ()->getValue ();
		v4dRotation.z = objRotationVector->get ("z")->toNumber ()->getValue ();
		v4dRotation.w = objRotationVector->get ("w")->toNumber ()->getValue ();

		mObject->addNode (v4dPosition, v4dScale, v4dRotation);

		return (0);
	}

	Variable *AnimationTrackClass::editNode (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));
		int iIndex = 0;
		Ogre::Vector4 v4dPosition = Ogre::Vector4::ZERO;
		Ogre::Vector4 v4dScale = Ogre::Vector4 (1, 1, 1, 0);
		Ogre::Vector4 v4dRotation = Ogre::Vector4::ZERO;

		iIndex = aryArguments->getElement (0)->toNumber ()->getValue ();

		Class *objPosVector = aryArguments->getElement (1)->toClass ();
		Class *objScaleVector = aryArguments->getElement (2)->toClass ();
		Class *objRotationVector = aryArguments->getElement (3)->toClass ();

		v4dPosition.x = objPosVector->get ("x")->toNumber ()->getValue ();
		v4dPosition.y = objPosVector->get ("y")->toNumber ()->getValue ();
		v4dPosition.z = objPosVector->get ("z")->toNumber ()->getValue ();
		v4dPosition.w = objPosVector->get ("w")->toNumber ()->getValue ();

		v4dScale.x = objScaleVector->get ("x")->toNumber ()->getValue ();
		v4dScale.y = objScaleVector->get ("y")->toNumber ()->getValue ();
		v4dScale.z = objScaleVector->get ("z")->toNumber ()->getValue ();
		v4dScale.w = objScaleVector->get ("w")->toNumber ()->getValue ();

		v4dRotation.x = objRotationVector->get ("x")->toNumber ()->getValue ();
		v4dRotation.y = objRotationVector->get ("y")->toNumber ()->getValue ();
		v4dRotation.z = objRotationVector->get ("z")->toNumber ()->getValue ();
		v4dRotation.w = objRotationVector->get ("w")->toNumber ()->getValue ();

		mObject->editNode (iIndex, v4dPosition, v4dScale, v4dRotation);

		return (0);
	}

	Variable *AnimationTrackClass::removeNode (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));
		int iIndex = aryArguments->getElement (0)->toNumber ()->getValue ();

		mObject->removeNode (iIndex);

		return (0);
	}

	Variable *AnimationTrackClass::getPositionNode (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));
		int iIndex = aryArguments->getElement (0)->toNumber ()->getValue ();
		Ogre::Vector4 v4dPosition = mObject->getPositionNode (iIndex);
		Class *objObject = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
			getVector4JSClass (v4dPosition.x, v4dPosition.y, v4dPosition.z, v4dPosition.w);

		return (objObject);
	}

	Variable *AnimationTrackClass::getScaleNode (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));
		int iIndex = aryArguments->getElement (0)->toNumber ()->getValue ();
		Ogre::Vector4 v4dScale = mObject->getScaleNode (iIndex);
		Class *objObject = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
			getVector4JSClass (v4dScale.x, v4dScale.y, v4dScale.z, v4dScale.w);

		return (objObject);
	}

	Variable *AnimationTrackClass::getRotationNode (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));
		int iIndex = aryArguments->getElement (0)->toNumber ()->getValue ();
		Ogre::Vector4 v4dRotation = mObject->getRotationNode (iIndex);
		Class *objObject = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
			getVector4JSClass (v4dRotation.x, v4dRotation.y, v4dRotation.z, v4dRotation.w);

		return (objObject);
	}

	Variable *AnimationTrackClass::play (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));

		mObject->play ();

		return (0);
	}

	Variable *AnimationTrackClass::isPlaying (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));
		bool bReturn = mObject->isPlaying ();

		return (Boolean::New (bReturn));
	}

	Variable *AnimationTrackClass::stop (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));

		mObject->stop ();

		return (0);
	}

	Variable *AnimationTrackClass::setLength (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));
		Ogre::Real rLength = aryArguments->getElement (0)->toNumber ()->getValue ();

		mObject->setLength (rLength);

		return (0);
	}

	Variable *AnimationTrackClass::getLength (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));

		return (Number::New (mObject->getLength ()));
	}

	Variable *AnimationTrackClass::setLoop (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));
		bool bLoop = aryArguments->getElement (0)->toBoolean ()->getValue ();

		mObject->setLoop (bLoop);

		return (0);
	}

	Variable *AnimationTrackClass::getLoop (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));

		return (Boolean::New (mObject->getLoop ()));
	}

	Variable *AnimationTrackClass::setAttach (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));
		Movable *mAttach = static_cast<Movable *> (aryArguments->getElement (0)->toClass ()->getHiddenData ("gameObject"));

		mObject->setAttach (mAttach);

		return (0);
	}

	Variable *AnimationTrackClass::getAttach (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));
		Movable *mAttach = mObject->getAttach ();

		return (RadXML::getSingletonPtr ()->getJavascriptEngine ()->createMovableJSClass (mAttach));
	}

	Variable *AnimationTrackClass::create (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AnimationTrackObject *mObject = static_cast<AnimationTrackObject *> (objThis->getHiddenData ("gameObject"));

		mObject->create (mObject->getParent ());

		return (0);
	}
}

