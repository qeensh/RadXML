#ifndef _RADXML_ANIMATION_TRACK_H_
	#define _RADXML_ANIMATION_TRACK_H_

	#ifdef defAPI_OGRE3D
		#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
			#include <OGRE/OgreVector4.h>
			#include <OGRE/OgreAnimation.h>
		#else
			#include <OgreVector4.h>
			#include <OgreAnimation.h>
		#endif
	#endif

	#include <string>
	#include <list>
	#include <vector>

	#ifndef HASHMAP
		#ifdef __GNUC__
			#include <ext/hash_map>

			namespace std
			{
				using namespace __gnu_cxx;
			}

			#define HASHMAP	std::hash_map

			/// @note You may have to use this for upcoming GCC compilers for hash maps
			/// in the mean time, you may want to compile with -Wno-deprecated on compilers
			/// that complain about a deprecated file.
			/// #include <unordered_map>
			/// #define HASHMAP	std::tr1::unordered_map
		#else
			#include <hash_map>

			#define HASHMAP	stdext::hash_map
		#endif
	#endif

	#include "RadXmlPreprocessor.h"
	#include "RadXmlMovable.h"

	namespace RadXml
	{
		class XmlFile;

		enum AnimationTrackStates
		{
			ATS_POSITION			= 1,
			ATS_SCALE				= 2,
			ATS_ROTATE				= 3,
			ATS_FADE				= 4
		};

		class defRADXML_EXPORT AnimationTrack_State
		{
			public:
				inline AnimationTrack_State ()
				{
					iType = 0;
					rKeyFrame = 0;
					v3dData = Ogre::Vector3::ZERO;
				}

				int iType;
				Ogre::Real rKeyFrame;
				Ogre::Vector3 v3dData;
		};

		class defRADXML_EXPORT AnimationTrack
		{
			public:
				AnimationTrack ();
				~AnimationTrack ();

				void createAnimationTrack (std::string newName, Ogre::SceneManager *mSceneMgr, Ogre::SceneNode *parent = 0, bool bCreateNewSceneNode = true);
				void destroyAnimationTrack (Ogre::SceneManager *mSceneMgr);

				inline void setAttach (Ogre::SceneNode *node)
				{
					snAttachedNode = node;
				}

				inline void setInterpolationMode (int imode)
				{
					imMode = imode;
				}

				inline int getInterpolationMode ()
				{
					return (imMode);
				}

				inline void setStart (Ogre::Real start)
				{
					rStart = start;
				}

				inline double getStart ()
				{
					return (rStart);
				}

				inline void setLength (Ogre::Real length)
				{
					rLength = length;
				}

				inline double getLength ()
				{
					return (rLength);
				}

				inline void setEnabled (bool enabled)
				{
					bEnabled = enabled;
				}

				inline bool isEnabled ()
				{
					return (bEnabled);
				}

				inline void setLooping (bool loop)
				{
					bLoop = loop;
				}

				inline bool isLooping ()
				{
					return (bLoop);
				}

				void addPosition (Ogre::Vector4 position);
				void addScale (Ogre::Vector4 scale);
				void addRotation (Ogre::Vector4 rotation);

				void setPositions (std::list<Ogre::Vector4> *positions);

				inline std::list<Ogre::Vector4> *getPositions ()
				{
					return (lstPositions);
				}

				void setScalings (std::list<Ogre::Vector4> *scalings);

				inline std::list<Ogre::Vector4> *getScalings ()
				{
					return (lstScalings);
				}

				void setRotations (std::list<Ogre::Vector4> *rotations);

				inline std::list<Ogre::Vector4> *getRotations ()
				{
					return (lstRotations);
				}

				void setFades (std::list<Ogre::Vector4> *fades);

				inline std::list<Ogre::Vector4> *getFades ()
				{
					return (0);
				}

				inline Ogre::Animation *getAnimation ()
				{
					return (aAnim);
				}

				inline Ogre::AnimationState *getAnimationState ()
				{
					return (asAnimState);
				}

				AnimationTrack *cloneAnimationTrack (std::string newName);

			protected:
				Ogre::SceneNode *snAttachedNode;
				Ogre::Animation *aAnim;
				Ogre::AnimationState *asAnimState;

				Ogre::Real rStart;
				Ogre::Real rLength;
				int imMode;
				bool bLoop;
				bool bEnabled;

				std::list<Ogre::Vector4> *lstPositions;
				std::list<Ogre::Vector4> *lstScalings;
				std::list<Ogre::Vector4> *lstRotations;
		};

		class defRADXML_EXPORT AnimationTrackObject: public Movable
		{
			public:
				AnimationTrackObject (std::string newName);
				~AnimationTrackObject ();

				void parseXML (XmlFile *level, XMLElement *element, Movable *parent = 0);

				void create (Movable *parent = 0);
				void destroy ();

				void setVisible (bool visible);
				bool getVisible ();

				void addNode (Ogre::Vector4 position, Ogre::Vector4 scale, Ogre::Vector4 rotation);
				void editNode (unsigned int index, Ogre::Vector4 position, Ogre::Vector4 scale, Ogre::Vector4 rotation);
				void removeNode (unsigned int index);

				Ogre::Vector4 getPositionNode (unsigned int index);
				Ogre::Vector4 getScaleNode (unsigned int index);
				Ogre::Vector4 getRotationNode (unsigned int index);

				inline unsigned int getNumNodes ()
				{
					return (aryPositions.size ());
				}

				Movable *clone (std::string newName, Movable *parent = 0);

				inline void play ()
				{
					enabled = true;
				}

				inline void stop ()
				{
					enabled = false;
				}

				inline bool isPlaying ()
				{
					return (enabled);
				}

				inline void setAttach (Movable *attach)
				{
					attachObject = attach;
				}

				inline Movable *getAttach ()
				{
					return (attachObject);
				}

				inline void setLoop (bool isLooping)
				{
					loop = isLooping;
				}

				inline bool getLoop ()
				{
					return (loop);
				}

				inline void setLength (Ogre::Real newLength)
				{
					length = newLength;
				}

				inline Ogre::Real getLength ()
				{
					return (length);
				}

				AnimationTrack *atTrack;
				bool firstTimeRunning;
				bool enabled;
				bool loop;
				Movable *attachObject;
				std::vector<Ogre::Vector4> aryPositions;
				std::vector<Ogre::Vector4> aryScales;
				std::vector<Ogre::Vector4> aryRotations;
				Ogre::Real length;
		};

		class AnimationTrackClass
		{
			public:
				static void setupAnimationTrackClass ();

				static Variable *addNode (Variable *thisObject, Variable *arguments);
				static Variable *editNode (Variable *thisObject, Variable *arguments);
				static Variable *removeNode (Variable *thisObject, Variable *arguments);
				static Variable *getPositionNode (Variable *thisObject, Variable *arguments);
				static Variable *getScaleNode (Variable *thisObject, Variable *arguments);
				static Variable *getRotationNode (Variable *thisObject, Variable *arguments);
				static Variable *play (Variable *thisObject, Variable *arguments);
				static Variable *isPlaying (Variable *thisObject, Variable *arguments);
				static Variable *stop (Variable *thisObject, Variable *arguments);
				static Variable *setLength (Variable *thisObject, Variable *arguments);
				static Variable *getLength (Variable *thisObject, Variable *arguments);
				static Variable *setLoop (Variable *thisObject, Variable *arguments);
				static Variable *getLoop (Variable *thisObject, Variable *arguments);
				static Variable *setAttach (Variable *thisObject, Variable *arguments);
				static Variable *getAttach (Variable *thisObject, Variable *arguments);
				static Variable *create (Variable *thisObject, Variable *arguments);
		};
	}
#endif

