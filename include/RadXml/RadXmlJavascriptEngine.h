#ifndef _RADXML_JAVASCRIPT_ENGINE_H_
	#define _RADXML_JAVASCRIPT_ENGINE_H_

	#include <string>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlNative.h"

	#ifdef defAPI_OGRE3D
		#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
			#include <OGRE/OgreVector2.h>
			#include <OGRE/OgreVector3.h>
			#include <OGRE/OgreVector4.h>
			#include <OGRE/OgreQuaternion.h>
		#else
			#include <OgreVector2.h>
			#include <OgreVector3.h>
			#include <OgreVector4.h>
			#include <OgreQuaternion.h>
		#endif
	#endif

	namespace RadXml
	{
		class defRADXML_EXPORT JavascriptEngine
		{
			public:
				virtual bool initializeEngine () = 0;
				virtual bool setupNativeFunctions () = 0;
				virtual bool finishInitializingEngine () = 0;

				virtual bool compileAndRunJSScript (std::string file) = 0;
				virtual bool compileAndRunJSString (std::string string) = 0;

				virtual Class *getVector2JSClass (double x, double y) = 0;
				virtual Class *getVector3JSClass (double x, double y, double z) = 0;
				virtual Class *getVector4JSClass (double x, double y, double z, double w) = 0;
				virtual Class *getQuaternionJSClass (Ogre::Quaternion rotation) = 0;

				virtual Ogre::Vector2 getVector2FromJSObject (Class *vector);
				virtual Ogre::Vector3 getVector3FromJSObject (Class *vector);
				virtual Ogre::Vector4 getVector4FromJSObject (Class *vector);

				virtual void addAppObjectJSObject (AppObject *object) = 0;
				virtual Class *createAppObjectJSClass (AppObject *object) = 0;
				virtual Class *getAppObjectJSClass (AppObject *object) = 0;
				virtual Class *getAppObjectJSClass (std::string objectName) = 0;
				virtual void removeAppObjectJSObject (AppObject *object) = 0;

				virtual void addMovableJSObject (Movable *object) = 0;
				virtual Class *createMovableJSClass (Movable *object) = 0;
				virtual Class *getMovableJSClass (Movable *object) = 0;
				virtual Class *getMovableJSClass (std::string objectName) = 0;
				virtual void removeMovableJSObject (Movable *object) = 0;

				virtual Variable *getVariableFromPrototype (std::string prototypeName) = 0;
				virtual bool hasGlobalFunction (std::string functionName) = 0;
				virtual Variable *callGlobalFunction (std::string functionName, std::vector<Variable *> passingVariables) = 0;
				virtual Variable *getGlobalVariable (std::string variableName) = 0;

			protected:
		};
	}
#endif

