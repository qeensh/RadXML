#ifndef _RADXML_V8_JAVASCRIPT_ENGINE_H_
	#define _RADXML_V8_JAVASCRIPT_ENGINE_H_

	#include "RadXmlPreprocessor.h"
	#include "RadXmlV8JavascriptTypes.h"
	#include "RadXmlJavascriptEngine.h"

	#ifdef defAPI_JAVASCRIPT_V8
		#include <v8.h>

		#ifdef _DEBUG
			#include <v8-debug.h>
		#endif
	#endif

	namespace RadXml
	{
		#ifdef defAPI_JAVASCRIPT_V8
			class defRADXML_EXPORT V8JavascriptEngine: public JavascriptEngine
			{
				public:
					V8JavascriptEngine ();
					V8JavascriptEngine (const V8JavascriptEngine &copy);
					~V8JavascriptEngine ();

					bool initializeEngine ();
					bool setupNativeFunctions ();
					bool finishInitializingEngine ();

					bool compileAndRunJSScript (std::string file);
					bool compileAndRunJSString (std::string string);

					std::string convertV8StringToString (JSSTRING string);
					JSOBJECT getVector2JSObject (double x, double y);
					JSOBJECT getVector3JSObject (double x, double y, double z);
					JSOBJECT getVector4JSObject (double x, double y, double z, double w);
					JSOBJECT getQuaternionJSObject (Ogre::Quaternion rotation);
					Ogre::Vector2 getVector2FromJSObject (JSOBJECT vector);
					Ogre::Vector3 getVector3FromJSObject (JSOBJECT vector);
					Ogre::Vector4 getVector4FromJSObject (JSOBJECT vector);

					JSOBJECT createAppObjectJSObject (AppObject *object);
					AppObject *getAppObjectFromJSObject (JSVALUE object);

					JSOBJECT createMovableJSObject (Movable *object);
					Movable *getMovableFromJSObject (JSVALUE object);

					void addAppObjectJSObject (AppObject *object);
					JSVALUE getAppObjectJSObject (AppObject *object);
					JSVALUE getAppObjectJSObject (std::string objectName);
					void removeAppObjectJSObject (AppObject *object);

					void addMovableJSObject (Movable *object);
					JSVALUE getMovableJSObject (Movable *object);
					JSVALUE getMovableJSObject (std::string objectName);
					void removeMovableJSObject (Movable *object);

					JSVALUE getActiveNetworkServerJSObject (unsigned int index);
					JSVALUE getActiveNetworkClientJSObject (unsigned int index);

					Variable *getVariableFromPrototype (std::string prototypeName);
					bool hasGlobalFunction (std::string functionName);
					JSVALUE callGlobalJSFunction (std::string functionName, std::vector<Variable *> passingVariables);
					Variable *callGlobalFunction (std::string functionName, std::vector<Variable *> passingVariables);
					JSVALUE getGlobalJSVariable (std::string variableName);
					Variable *getGlobalVariable (std::string variableName);
					JSVALUE executeFunction (std::string functionName, JSVALUE variable, std::vector< JSVALUE > passingVariables);

					std::vector<std::string> *getHiddenNamesFromJSObject (JSVALUE object);
					JSVALUE getJSObjectFromVariable (Variable *object);
					Variable *getVariableFromJSObject (JSVALUE object, std::string name = "", std::string typeName = "");

					Class *getVector2JSClass (double x, double y);
					Class *getVector3JSClass (double x, double y, double z);
					Class *getVector4JSClass (double x, double y, double z, double w);
					Class *getQuaternionJSClass (Ogre::Quaternion rotation);

					Class *createAppObjectJSClass (AppObject *object);
					Class *getAppObjectJSClass (AppObject *object);
					Class *getAppObjectJSClass (std::string objectName);

					Class *createMovableJSClass (Movable *object);
					Class *getMovableJSClass (Movable *object);
					Class *getMovableJSClass (std::string objectName);

					static v8::Handle<v8::Value> v8JavascriptFunction (const v8::Arguments &arguments);

					inline v8::Persistent<v8::Context> getContext ()
					{
						return (cContext);
					}

				protected:
					std::string getStringFromStackTrace (v8::TryCatch *stackTrace);

					v8::Persistent<v8::Context> cContext;
					v8::HandleScope hsHandleScope;
					v8::Context::Scope *sContextScope;
					v8::Handle<v8::ObjectTemplate> objGlobal;
					v8::Local<v8::Array> objGameMovables;
					v8::Local<v8::Array> objAppAppObjects;

					#ifdef defNET_ON
						v8::Local<v8::Array> objActiveNetworkServers;
						v8::Local<v8::Array> objActiveNetworkClients;
					#endif

					#ifdef _DEBUG
						v8::Persistent<v8::Context> cDebug;
					#endif
			};
		#endif
	}
#endif

