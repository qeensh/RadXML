#ifndef _RADXML_MOVABLE_H_
	#define _RADXML_MOVABLE_H_

	#ifdef defAPI_OGRE3D
		#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
			#include <OGRE/OgreVector3.h>
			#include <OGRE/OgreSceneNode.h>
		#else
			#include <OgreVector3.h>
			#include <OgreSceneNode.h>
		#endif
	#endif

	#include <string>
	#include <vector>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlEvents.h"
	#include "RadXmlNative.h"

	class XMLElement;

	namespace RadXml
	{
		class XmlFile;

		class defRADXML_EXPORT Movable: public Events
		{
			public:
				Movable (std::string newName);
				virtual ~Movable ();

				void init ();

				void setName (std::string newName);
				std::string getName ();
				unsigned long getHash ();

				virtual void setVisible (bool visible) = 0;
				virtual bool getVisible () = 0;
				virtual Movable *clone (std::string newName, Movable *parent = 0) = 0;
				virtual void parseXML (XmlFile *level, XMLElement *element, Movable *parent = 0) = 0;

				void createSceneNode (Ogre::MovableObject *object, Movable *parent = 0);
				void destroySceneNode ();

				void parseMovableXML (XmlFile *level, XMLElement *element, Movable *parent = 0);
				std::string getMovableXML ();

				inline Ogre::SceneNode *getSceneNode ()
				{
					return (snNode);
				}

				inline void setPosition (Ogre::Vector3 newData)
				{
					setPosition (newData.x, newData.y, newData.z);
				}

				void setPosition (double x, double y, double z);
				Ogre::Vector3 getPosition ();
				Ogre::Vector3 getWorldPosition ();

				void translate (double x, double y, double z);

				inline void setScale (Ogre::Vector3 newData)
				{
					setScale (newData.x, newData.y, newData.z);
				}

				void setScale (double x, double y, double z);

				inline Ogre::Vector3 getScale ()
				{
					return (scale);
				}

				inline void setRotation (Ogre::Vector3 newData)
				{
					setScale (newData.x, newData.y, newData.z);
				}

				void setRotation (double yaw, double pitch, double roll);
				Ogre::Vector3 getRotation ();

				void setYaw (double yaw);
				void setPitch (double pitch);
				void setRoll (double roll);

				inline void setVelocity (Ogre::Vector3 newData)
				{
					setVelocity (newData.x, newData.y, newData.z);
				}
				void setVelocity (double x, double y, double z);
				inline Ogre::Vector3 getVelocity ()
				{
					return (v3dVelocity);
				}

				inline void setAcceleration (Ogre::Vector3 newData)
				{
					setAcceleration (newData.x, newData.y, newData.z);
				}
				void setAcceleration (double x, double y, double z);
				inline Ogre::Vector3 getAcceleration ()
				{
					return (v3dAccel);
				}

				inline void setMaxVelocity (Ogre::Vector3 newData)
				{
					setMaxVelocity (newData.x, newData.y, newData.z);
				}
				void setMaxVelocity (double x, double y, double z);
				inline Ogre::Vector3 getMaxVelocity ()
				{
					return (v3dMaxVel);
				}

				inline void setFriction (Ogre::Vector3 newData)
				{
					setFriction (newData.x, newData.y, newData.z);
				}
				void setFriction (double x, double y, double z);
				inline Ogre::Vector3 getFriction ()
				{
					return (v3dFriction);
				}

				Ogre::Quaternion getOrientation ();

				void moveEntity (Ogre::FrameEvent evtEvent);

				void setBoundingBoxVisible (bool value);
				bool getBoundingBoxVisible ();

				inline void setImmovable (bool isImmovable)
				{
					bImmovable = isImmovable;
				}

				inline bool getImmovable ()
				{
					return (bImmovable);
				}

				inline int getType ()
				{
					return (iType);
				}

				inline void setCollides (bool value)
				{
					collides = value;
				}

				inline bool getCollides ()
				{
					return (collides);
				}

				inline void setParent (Movable *parent)
				{
					mParent = parent;

					if (mParent != 0)
						mParent->addChild (this);
				}

				inline Movable *getParent ()
				{
					return (mParent);
				}

				inline void addChild (Movable *child)
				{
					aryChildren.push_back (child);
				}

				inline void removeChild (Movable *child)
				{
					for (unsigned int iIdx = 0; iIdx < aryChildren.size (); iIdx++)
					{
						if (aryChildren.at (iIdx) == child)
						{
							std::vector<Movable *>::iterator itBegin = aryChildren.begin ();
							itBegin += iIdx;
							aryChildren.erase (itBegin);

							break;
						}
					}
				}

				inline void removeAllChildren ()
				{
					aryChildren.clear ();
				}

				inline Movable *getChild (unsigned int index)
				{
					return (aryChildren.at (index));
				}

				inline unsigned int getNumChildren ()
				{
					return (aryChildren.size ());
				}

				void setLookAt (Ogre::Vector3 target, int transformSpace, Ogre::Vector3 localDirVector);

				Ogre::Vector3 scale;
				Ogre::Vector3 rotation;
				Movable *collidedWithObject;
				bool collides;

			protected:
				inline void setType (int newType)
				{
					iType = newType;
				}

				std::string name;
				unsigned long hashName;
				int iType;

				Ogre::SceneNode *snNode;
				Movable *mParent;
				std::vector<Movable *> aryChildren;

				Ogre::Vector3 position;

				bool bVisible;
				bool bImmovable;

				Ogre::Real rAngle;
				Ogre::Vector3 v3dMaxVel;
				Ogre::Vector3 v3dSpeed;
				Ogre::Vector3 v3dAccel;
				Ogre::Vector3 v3dMoveScale;
				Ogre::Vector3 v3dVelocity;
				Ogre::Vector3 v3dFriction;

				Ogre::Vector3 v3dOriginalPos;
				bool bSetOriginalPos;
				Ogre::Vector3 v3dOriginalScale;
				bool bSetOriginalScale;
				Ogre::Vector3 v3dOriginalRotation;
				bool bSetOriginalRotation;
		};

		class MovableClass
		{
			public:
				static void setupMovableClass ();

				static Variable *getName (Variable *thisObject, Variable *arguments);
				static Variable *translate (Variable *thisObject, Variable *arguments);
				static Variable *setPosition (Variable *thisObject, Variable *arguments);
				static Variable *getPosition (Variable *thisObject, Variable *arguments);
				static Variable *getWorldPosition (Variable *thisObject, Variable *arguments);
				static Variable *setScale (Variable *thisObject, Variable *arguments);
				static Variable *getScale (Variable *thisObject, Variable *arguments);
				static Variable *setRotation (Variable *thisObject, Variable *arguments);
				static Variable *setYaw (Variable *thisObject, Variable *arguments);
				static Variable *getYaw (Variable *thisObject, Variable *arguments);
				static Variable *setPitch (Variable *thisObject, Variable *arguments);
				static Variable *getPitch (Variable *thisObject, Variable *arguments);
				static Variable *setRoll (Variable *thisObject, Variable *arguments);
				static Variable *getRoll (Variable *thisObject, Variable *arguments);
				static Variable *getOrientation (Variable *thisObject, Variable *arguments);
				static Variable *setVelocity (Variable *thisObject, Variable *arguments);
				static Variable *getVelocity (Variable *thisObject, Variable *arguments);
				static Variable *setAcceleration (Variable *thisObject, Variable *arguments);
				static Variable *getAcceleration (Variable *thisObject, Variable *arguments);
				static Variable *setFriction (Variable *thisObject, Variable *arguments);
				static Variable *getFriction (Variable *thisObject, Variable *arguments);
				static Variable *setMaxVelocity (Variable *thisObject, Variable *arguments);
				static Variable *getMaxVelocity (Variable *thisObject, Variable *arguments);
				static Variable *clone (Variable *thisObject, Variable *arguments);
				static Variable *setVisible (Variable *thisObject, Variable *arguments);
				static Variable *getVisible (Variable *thisObject, Variable *arguments);
				static Variable *setCollides (Variable *thisObject, Variable *arguments);
				static Variable *getCollides (Variable *thisObject, Variable *arguments);
				static Variable *setBoundingBoxVisible (Variable *thisObject, Variable *arguments);
				static Variable *getBoundingBoxVisible (Variable *thisObject, Variable *arguments);
				static Variable *setLookAt (Variable *thisObject, Variable *arguments);
		};
	}
#endif

