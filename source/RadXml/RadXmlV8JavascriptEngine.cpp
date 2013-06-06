#include "RadXmlV8JavascriptEngine.h"
#include "RadXml.h"

#include "common/com_radxml_strconv.h"
#include "common/mai_radxml_system.h"

#include "languages/en_us.h"

namespace RadXml
{
	#ifdef defAPI_JAVASCRIPT_V8
	// V8JavascriptEngine
	V8JavascriptEngine::V8JavascriptEngine ()
	{
	}

	V8JavascriptEngine::V8JavascriptEngine (const V8JavascriptEngine &copy)
	{
		cContext = copy.cContext;
		objGlobal = copy.objGlobal;
		objGameMovables = copy.objGameMovables;
		objAppAppObjects = copy.objAppAppObjects;
		objActiveNetworkServers = copy.objActiveNetworkServers;
		objActiveNetworkClients = copy.objActiveNetworkClients;

		//hsHandleScope = copy.hsHandleScope;
		sContextScope = copy.sContextScope;

		#ifdef _DEBUG
			cDebug = copy.cDebug;
		#endif
	}

	V8JavascriptEngine::~V8JavascriptEngine ()
	{
		if (sContextScope != 0)
		{
			delete sContextScope;
			sContextScope = 0;
		}

		#ifdef _DEBUG
			cDebug.Dispose ();
		#endif

		cContext.Dispose ();
	}

	bool V8JavascriptEngine::initializeEngine ()
	{
		if (v8::V8::Initialize () == false)
		{
			RadXmlCommon::s13Error (MSG11);

			return (false);
		}

		#ifdef _DEBUG
			v8::Debug::EnableAgent ("debugger", 7820, false);
			v8::Debug::SetDebugMessageDispatchHandler (RadXML::javascriptDebugger, true);

			cDebug = v8::Context::New ();
		#endif

		//hsHandleScope = new v8::HandleScope ();

		return (true);
	}

	bool V8JavascriptEngine::setupNativeFunctions ()
	{
		JavascriptManager *jmManager = JavascriptManager::getSingletonPtr ();

		std::vector<std::string> aryClassNames;
		std::vector< v8::Handle<v8::FunctionTemplate> > aryClassTemplates;
		std::vector<Variable *> aryObjects;

		for (unsigned int iIdx = 0; iIdx < jmManager->size (); iIdx++)
		{
			Variable *vVar = jmManager->get (iIdx);

			if (vVar->getType () == VT_CLASS)
			{
				v8::Handle<v8::FunctionTemplate> ftNew = v8::FunctionTemplate::New ();
				v8::Local<v8::ObjectTemplate> otNew = ftNew->PrototypeTemplate ();
				Class *cClass = static_cast <Class *> (vVar);

				for (unsigned int iJdx = 0; iJdx < cClass->size (); iJdx++)
				{
					Variable *vVar2 = cClass->get (iJdx);

					if (vVar2->getType () == VT_FUNCTION)
					{
						Function *fFunc2 = static_cast <Function *> (vVar2);
						v8::Local<v8::External> lExt = v8::External::New (fFunc2->getCallback (0));
						otNew->Set (v8::String::New (fFunc2->getName ().c_str ()), v8::FunctionTemplate::New (v8JavascriptFunction, lExt));
					}
					else
					{
						v8::Local<v8::Value> lVar = getJSObjectFromVariable (vVar2);
						otNew->Set (v8::String::New (vVar2->getName ().c_str ()), lVar);
					}
				}

				std::string strName = cClass->getName ();
				aryClassNames.push_back (strName);
				aryClassTemplates.push_back (ftNew);
			}
		}

		objGlobal = v8::ObjectTemplate::New ();

		for (unsigned int iIdx = 0; iIdx < jmManager->size (); iIdx++)
		{
			Variable *vVar = jmManager->get (iIdx);

			if (vVar->getType () == VT_CLASS)
			{
				/// @note Don't do anything...
			}
			else if (vVar->getType () == VT_FUNCTION)
			{
				Function *fFunc2 = static_cast <Function *> (vVar);
				v8::Local<v8::External> lExt = v8::External::New (fFunc2->getCallback (0));
				std::string strName = fFunc2->getName ();

				objGlobal->Set (v8::String::New (strName.c_str ()), v8::FunctionTemplate::New (v8JavascriptFunction, lExt));
			}
			else
				aryObjects.push_back (vVar);
		}

		for (unsigned int iIdx = 0; iIdx < aryClassNames.size (); iIdx++)
		{
			std::string strName = aryClassNames.at (iIdx);
			v8::Handle<v8::FunctionTemplate> ftNew = aryClassTemplates.at (iIdx);

			objGlobal->Set (v8::String::New (strName.c_str ()), ftNew);
		}

		cContext = v8::Context::New (0, objGlobal);
		sContextScope = new v8::Context::Scope (cContext);

		for (unsigned int iIdx = 0; iIdx < aryObjects.size (); iIdx++)
		{
			Variable *vVar = aryObjects.at (iIdx);

			if (vVar->getType () == VT_CLASS_OBJECT)
			{
				v8::Local<v8::Object> otNew = v8::Object::New ();
				Class *cClass = static_cast <Class *> (vVar);

				for (unsigned int iJdx = 0; iJdx < cClass->size (); iJdx++)
				{
					Variable *vVar2 = cClass->get (iJdx);

					if (vVar2->getType () == VT_FUNCTION)
					{
						Function *fFunc2 = static_cast <Function *> (vVar2);
						v8::Local<v8::External> lExt = v8::External::New (fFunc2->getCallback (0));
						otNew->Set (v8::String::New (fFunc2->getName ().c_str ()), 
							v8::FunctionTemplate::New (v8JavascriptFunction, lExt)->GetFunction ());
					}
					else
					{
						v8::Local<v8::Value> lVar = getJSObjectFromVariable (vVar2);
						otNew->Set (v8::String::New (vVar2->getName ().c_str ()), lVar);
					}
				}

				std::string strName = cClass->getName ();
				objGlobal->Set (v8::String::New (strName.c_str ()), otNew);
			}
			else
			{
				v8::Local<v8::Value> loValue = getJSObjectFromVariable (vVar);
				std::string strName = vVar->getName ();

				objGlobal->Set (v8::String::New (strName.c_str ()), loValue);
			}
		}

		return (true);
	}

	bool V8JavascriptEngine::finishInitializingEngine ()
	{
		{
			v8::Local<v8::Value> vApp = cContext->Global ()->GetRealNamedPropertyInPrototypeChain (v8::String::New ("App"));
			v8::Local<v8::Object> objApp = v8::Local<v8::Object>::Cast (vApp);

			ClassObject *coApp = (ClassObject *)JavascriptManager::getSingletonPtr ()->get ("App");
			Function *fFunc = (Function *)coApp->get ("getAppObjectByName");
			v8::Local<v8::External> lExt = v8::External::New (fFunc->getCallback (0));
			objApp->Set (v8::String::New ("getAppObjectByName"), (v8::FunctionTemplate::New (v8JavascriptFunction, lExt))->GetFunction ());

			fFunc = (Function *)coApp->get ("deleteObject");
			lExt = v8::External::New (fFunc->getCallback (0));
			objApp->Set (v8::String::New ("deleteObject"), (v8::FunctionTemplate::New (v8JavascriptFunction, lExt))->GetFunction ());

			fFunc = (Function *)coApp->get ("createObject");
			lExt = v8::External::New (fFunc->getCallback (0));
			objApp->Set (v8::String::New ("createObject"), (v8::FunctionTemplate::New (v8JavascriptFunction, lExt))->GetFunction ());

			v8::Local<v8::Value> vAppObjects = objApp->Get (v8::String::New ("AppObjects"));
			objAppAppObjects = v8::Local<v8::Array>::Cast (vAppObjects);
		}

		/// @note Same with the movables array
		{
			v8::Local<v8::Value> vGame = cContext->Global ()->GetRealNamedPropertyInPrototypeChain (v8::String::New ("Game"));
			v8::Local<v8::Object> objGame = v8::Local<v8::Object>::Cast (vGame);

			ClassObject *coApp = (ClassObject *)JavascriptManager::getSingletonPtr ()->get ("Game");
			Function *fFunc = (Function *)coApp->get ("getMovableByName");
			v8::Local<v8::External> lExt = v8::External::New (fFunc->getCallback (0));
			objGame->Set (v8::String::New ("getMovableByName"), (v8::FunctionTemplate::New (v8JavascriptFunction, lExt))->GetFunction ());

			fFunc = (Function *)coApp->get ("deleteObject");
			lExt = v8::External::New (fFunc->getCallback (0));
			objGame->Set (v8::String::New ("deleteObject"), (v8::FunctionTemplate::New (v8JavascriptFunction, lExt))->GetFunction ());

			fFunc = (Function *)coApp->get ("createObject");
			lExt = v8::External::New (fFunc->getCallback (0));
			objGame->Set (v8::String::New ("createObject"), (v8::FunctionTemplate::New (v8JavascriptFunction, lExt))->GetFunction ());

			fFunc = (Function *)coApp->get ("getObjectsFromRayCast");
			lExt = v8::External::New (fFunc->getCallback (0));
			objGame->Set (v8::String::New ("getObjectsFromRayCast"), (v8::FunctionTemplate::New (v8JavascriptFunction, lExt))->GetFunction ());

			v8::Local<v8::Value> vMovables = objGame->Get (v8::String::New ("Movables"));
			objGameMovables = v8::Local<v8::Array>::Cast (vMovables);
		}

		#ifdef defNET_ON
		/// @note Same with the ActiveServers and ActiveClients arrays
		{
			v8::Local<v8::Value> vGame = cContext->Global ()->GetRealNamedPropertyInPrototypeChain (v8::String::New ("Networking"));
			v8::Local<v8::Object> objGame = v8::Local<v8::Object>::Cast (vGame);

			v8::Local<v8::Value> vMovables = objGame->Get (v8::String::New ("ActiveServers"));
			objActiveNetworkServers = v8::Local<v8::Array>::Cast (vMovables);
			vMovables = objGame->Get (v8::String::New ("ActiveClients"));
			objActiveNetworkClients = v8::Local<v8::Array>::Cast (vMovables);
		}
		#endif

		return (true);
	}

	bool V8JavascriptEngine::compileAndRunJSScript (std::string file)
	{
		file = RadXmlCommon::System_NormalizeDirectoryPath (file, 0, false);
		std::string strDir = RadXmlCommon::System_GetParentDirectoryFromFilePath (file);
		RadXML::getSingletonPtr ()->setCurrentScriptDirectory (strDir);

		std::ifstream ifsFile (file.c_str (), (std::ios::in | std::ios::out));

		if (ifsFile.is_open () != true)
		{
			ifsFile.close ();
			RadXmlCommon::s13LogMessage (MSG19 + file);

			return (false);
		}

		RadXmlCommon::s13LogMessage (MSG20 + file);

		int iScoreIndex = 0;
		int iOtherLine = 0;
		int iIndex = 0;
		int iLines = 0;
		std::string strCode = "";

		while (! (ifsFile.eof ()))
		{
			std::string strLine = "";

			getline (ifsFile, strLine);
			iLines++;

			strCode += (strLine + "\n");
		}

		ifsFile.close ();

		v8::TryCatch tcCatch;
		v8::Handle<v8::Script> hScript = v8::Script::Compile (v8::String::New (strCode.c_str (), strCode.size ()));

		if (hScript.IsEmpty () == true)
		{
			std::string strErrorMessage = MSG21 + file + MSG22;
			strErrorMessage += getStringFromStackTrace (&tcCatch);

			RadXmlCommon::s13LogMessage (strErrorMessage);

			return (false);
		}

		v8::Handle<v8::Value> hResult = hScript->Run ();

		if (hResult.IsEmpty () == true)
		{
			std::string strErrorMessage = MSG23 + file + MSG22;
			strErrorMessage += getStringFromStackTrace (&tcCatch);

			RadXmlCommon::s13LogMessage (strErrorMessage);

			return (false);
		}

		return (true);
	}

	bool V8JavascriptEngine::compileAndRunJSString (std::string string)
	{
		v8::TryCatch tcCatch;
		v8::Handle<v8::Script> hScript = v8::Script::Compile (v8::String::New (string.c_str (), string.size ()));

		if (hScript.IsEmpty () == true)
		{
			std::string strErrorMessage = MSG24;
			strErrorMessage += getStringFromStackTrace (&tcCatch);

			RadXmlCommon::s13LogMessage (strErrorMessage);

			return (false);
		}

		v8::Handle<v8::Value> hResult = hScript->Run ();

		if (hResult.IsEmpty () == true)
		{
			std::string strErrorMessage = MSG25;
			strErrorMessage += getStringFromStackTrace (&tcCatch);

			RadXmlCommon::s13LogMessage (strErrorMessage);

			return (false);
		}

		return (true);
	}

	std::string V8JavascriptEngine::convertV8StringToString (v8::Local<v8::String> string)
	{
		std::string strReturn = "";
		int iLength = string->Length ();

		char *cBuffer = new char [(iLength + 1)];
		cBuffer[iLength] = '\0';
		string->WriteAscii (cBuffer, 0, iLength);

		strReturn = cBuffer;

		delete []cBuffer;
		cBuffer = 0;

		return (strReturn);
	}

	v8::Local<v8::Object> V8JavascriptEngine::getVector2JSObject (double x, double y)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		v8::Handle<v8::Value> hNull[1];
		v8::Local<v8::Value> vClass = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
			getJavascriptEngine ())->getContext ()->Global ()->GetRealNamedPropertyInPrototypeChain (v8::String::New ("Vector2"));
		v8::Local<v8::Object> objClass = v8::Local<v8::Object>::Cast (vClass);
		v8::Local<v8::Value> vClone = objClass->CallAsConstructor (0, hNull);
		v8::Local<v8::Object> objClone = v8::Local<v8::Object>::Cast (vClone);
		objClone->Set (v8::String::New ("x"), v8::Number::New (x));
		objClone->Set (v8::String::New ("y"), v8::Number::New (y));

		return (objClone);
	}

	v8::Local<v8::Object> V8JavascriptEngine::getVector3JSObject (double x, double y, double z)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		v8::Handle<v8::Value> hNull[1];
		v8::Local<v8::Value> vClass = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
			getJavascriptEngine ())->getContext ()->Global ()->GetRealNamedPropertyInPrototypeChain (v8::String::New ("Vector3"));
		v8::Local<v8::Object> objClass = v8::Local<v8::Object>::Cast (vClass);
		v8::Local<v8::Value> vClone = objClass->CallAsConstructor (0, hNull);
		v8::Local<v8::Object> objClone = v8::Local<v8::Object>::Cast (vClone);
		objClone->Set (v8::String::New ("x"), v8::Number::New (x));
		objClone->Set (v8::String::New ("y"), v8::Number::New (y));
		objClone->Set (v8::String::New ("z"), v8::Number::New (z));

		return (objClone);
	}

	v8::Local<v8::Object> V8JavascriptEngine::getVector4JSObject (double x, double y, double z, double w)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		v8::Handle<v8::Value> hNull[1];
		v8::Local<v8::Value> vClass = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
			getJavascriptEngine ())->getContext ()->Global ()->GetRealNamedPropertyInPrototypeChain (v8::String::New ("Vector4"));
		v8::Local<v8::Object> objClass = v8::Local<v8::Object>::Cast (vClass);
		v8::Local<v8::Value> vClone = objClass->CallAsConstructor (0, hNull);
		v8::Local<v8::Object> objClone = v8::Local<v8::Object>::Cast (vClone);
		objClone->Set (v8::String::New ("x"), v8::Number::New (x));
		objClone->Set (v8::String::New ("y"), v8::Number::New (y));
		objClone->Set (v8::String::New ("z"), v8::Number::New (z));
		objClone->Set (v8::String::New ("w"), v8::Number::New (w));

		return (objClone);
	}

	v8::Local<v8::Object> V8JavascriptEngine::getQuaternionJSObject (Ogre::Quaternion rotation)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		v8::Handle<v8::Value> hNull[1];
		v8::Local<v8::Value> vClass = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
			getJavascriptEngine ())->getContext ()->Global ()->GetRealNamedPropertyInPrototypeChain (v8::String::New ("Quaternion"));
		v8::Local<v8::Object> objClass = v8::Local<v8::Object>::Cast (vClass);
		v8::Local<v8::Value> vClone = objClass->CallAsConstructor (0, hNull);
		v8::Local<v8::Object> objClone = v8::Local<v8::Object>::Cast (vClone);
		objClone->Set (v8::String::New ("w"), v8::Number::New (rotation.w));
		objClone->Set (v8::String::New ("x"), v8::Number::New (rotation.x));
		objClone->Set (v8::String::New ("y"), v8::Number::New (rotation.y));
		objClone->Set (v8::String::New ("z"), v8::Number::New (rotation.z));

		return (objClone);
	}

	Ogre::Vector2 V8JavascriptEngine::getVector2FromJSObject (v8::Local<v8::Object> vector)
	{
		Ogre::Vector2 returnVector = Ogre::Vector2::ZERO;

		returnVector.x = vector->Get (v8::String::New ("x"))->ToNumber ()->Value ();
		returnVector.y = vector->Get (v8::String::New ("y"))->ToNumber ()->Value ();

		return (returnVector);
	}

	Ogre::Vector3 V8JavascriptEngine::getVector3FromJSObject (v8::Local<v8::Object> vector)
	{
		Ogre::Vector3 returnVector = Ogre::Vector3::ZERO;

		returnVector.x = vector->Get (v8::String::New ("x"))->ToNumber ()->Value ();
		returnVector.y = vector->Get (v8::String::New ("y"))->ToNumber ()->Value ();
		returnVector.z = vector->Get (v8::String::New ("z"))->ToNumber ()->Value ();

		return (returnVector);
	}

	Ogre::Vector4 V8JavascriptEngine::getVector4FromJSObject (v8::Local<v8::Object> vector)
	{
		Ogre::Vector4 returnVector = Ogre::Vector4::ZERO;

		returnVector.x = vector->Get (v8::String::New ("x"))->ToNumber ()->Value ();
		returnVector.y = vector->Get (v8::String::New ("y"))->ToNumber ()->Value ();
		returnVector.z = vector->Get (v8::String::New ("z"))->ToNumber ()->Value ();
		returnVector.w = vector->Get (v8::String::New ("w"))->ToNumber ()->Value ();

		return (returnVector);
	}

	v8::Local<v8::Object> V8JavascriptEngine::createAppObjectJSObject (AppObject *object)
	{
		v8::Local<v8::Object> objObject;

		if (object != 0)
		{
			RadXML *ogrGame = RadXML::getSingletonPtr ();
			v8::Handle<v8::Value> hNull[1];
			std::vector<std::string> *aryHiddenNames = new std::vector<std::string> ();

			if (object->getType () == AOT_COMBO_BOX)
			{
				v8::Local<v8::Value> vClass = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
					getJavascriptEngine ())->getContext ()->Global ()->GetRealNamedPropertyInPrototypeChain (v8::String::New ("ComboBox"));
				v8::Local<v8::Object> objClass = v8::Local<v8::Object>::Cast (vClass);
				v8::Local<v8::Value> vClone = objClass->CallAsConstructor (0, hNull);
				objObject = v8::Local<v8::Object>::Cast (vClone);
				objObject->SetHiddenValue (v8::String::New ("appObject"), v8::External::New (object));
			}
			else
			{
				v8::Local<v8::Value> vClass = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
					getJavascriptEngine ())->getContext ()->Global ()->GetRealNamedPropertyInPrototypeChain (v8::String::New ("AppObject"));
				v8::Local<v8::Object> objClass = v8::Local<v8::Object>::Cast (vClass);
				v8::Local<v8::Value> vClone = objClass->CallAsConstructor (0, hNull);
				objObject = v8::Local<v8::Object>::Cast (vClone);
				objObject->SetHiddenValue (v8::String::New ("appObject"), v8::External::New (object));
			}

			aryHiddenNames->push_back ("appObject");
			objObject->SetHiddenValue (v8::String::New ("hiddenNames"), v8::External::New (aryHiddenNames));
		}

		return (objObject);
	}

	AppObject *V8JavascriptEngine::getAppObjectFromJSObject (v8::Local<v8::Value> object)
	{
		if (object.IsEmpty () == true)
			return (0);

		if (object->IsNull () == true)
			return (0);

		if (object->IsObject () == false)
			return (0);

		v8::Local<v8::Object> objObject = object->ToObject ();
		v8::Local<v8::External> extExternal = v8::Local<v8::External>::Cast(objObject->GetHiddenValue (v8::String::New ("appObject")));
		AppObject *mObject = static_cast<AppObject *> (extExternal->Value ());

		return (mObject);
	}

	v8::Local<v8::Object> V8JavascriptEngine::createMovableJSObject (Movable *object)
	{
		v8::Local<v8::Object> objObject;

		if (object != 0)
		{
			RadXML *ogrGame = RadXML::getSingletonPtr ();
			v8::Handle<v8::Value> hNull[1];
			std::vector<std::string> *aryHiddenNames = new std::vector<std::string> ();

			if (object->getType () == GOT_ENTITY)
			{
				v8::Local<v8::Value> vClass = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
					getJavascriptEngine ())->getContext ()->Global ()->GetRealNamedPropertyInPrototypeChain (v8::String::New ("Entity"));
				v8::Local<v8::Object> objClass = v8::Local<v8::Object>::Cast (vClass);
				v8::Local<v8::Value> vClone = objClass->CallAsConstructor (0, hNull);
				objObject = v8::Local<v8::Object>::Cast (vClone);
				objObject->SetHiddenValue (v8::String::New ("gameObject"), v8::External::New (object));
			}
			else if (object->getType () == GOT_ANIMATION_TRACK)
			{
				v8::Local<v8::Value> vClass = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
					getJavascriptEngine ())->getContext ()->Global ()->GetRealNamedPropertyInPrototypeChain (v8::String::New ("AnimationTrack"));
				v8::Local<v8::Object> objClass = v8::Local<v8::Object>::Cast (vClass);
				v8::Local<v8::Value> vClone = objClass->CallAsConstructor (0, hNull);
				objObject = v8::Local<v8::Object>::Cast (vClone);
				objObject->SetHiddenValue (v8::String::New ("gameObject"), v8::External::New (object));
			}
			else if (object->getType () == GOT_SOUND)
			{
				v8::Local<v8::Value> vClass = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
					getJavascriptEngine ())->getContext ()->Global ()->GetRealNamedPropertyInPrototypeChain (v8::String::New ("Sound"));
				v8::Local<v8::Object> objClass = v8::Local<v8::Object>::Cast (vClass);
				v8::Local<v8::Value> vClone = objClass->CallAsConstructor (0, hNull);
				objObject = v8::Local<v8::Object>::Cast (vClone);
				objObject->SetHiddenValue (v8::String::New ("gameObject"), v8::External::New (object));
			}
			else if (object->getType () == GOT_OVERLAY_ELEMENT)
			{
				v8::Local<v8::Value> vClass = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
					getJavascriptEngine ())->getContext ()->Global ()->GetRealNamedPropertyInPrototypeChain (v8::String::New ("OverlayElement"));
				v8::Local<v8::Object> objClass = v8::Local<v8::Object>::Cast (vClass);
				v8::Local<v8::Value> vClone = objClass->CallAsConstructor (0, hNull);
				objObject = v8::Local<v8::Object>::Cast (vClone);
				objObject->SetHiddenValue (v8::String::New ("gameObject"), v8::External::New (object));
			}
			else
			{
				v8::Local<v8::Value> vClass = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
					getJavascriptEngine ())->getContext ()->Global ()->GetRealNamedPropertyInPrototypeChain (v8::String::New ("Movable"));
				v8::Local<v8::Object> objClass = v8::Local<v8::Object>::Cast (vClass);
				v8::Local<v8::Value> vClone = objClass->CallAsConstructor (0, hNull);
				objObject = v8::Local<v8::Object>::Cast (vClone);
				objObject->SetHiddenValue (v8::String::New ("gameObject"), v8::External::New (object));
			}

			aryHiddenNames->push_back ("gameObject");
			objObject->SetHiddenValue (v8::String::New ("hiddenNames"), v8::External::New (aryHiddenNames));
		}

		return (objObject);
	}

	Movable *V8JavascriptEngine::getMovableFromJSObject (v8::Local<v8::Value> object)
	{
		if (object.IsEmpty () == true)
			return (0);

		if (object->IsNull () == true)
			return (0);

		if (object->IsObject () == false)
			return (0);

		v8::Local<v8::Object> objObject = object->ToObject ();
		v8::Local<v8::External> extExternal = v8::Local<v8::External>::Cast(objObject->GetHiddenValue (v8::String::New ("gameObject")));
		Movable *mObject = static_cast<Movable *> (extExternal->Value ());

		return (mObject);
	}

	void V8JavascriptEngine::addAppObjectJSObject (AppObject *object)
	{
		std::string strName = object->getName ();
		v8::Local<v8::Object> objAppObject = createAppObjectJSObject (object);
		objAppAppObjects->Set (v8::String::New (strName.c_str (), strName.size ()), objAppObject);
	}

	JSVALUE V8JavascriptEngine::getAppObjectJSObject (AppObject *object)
	{
		std::string strName = object->getName ();
		return (getAppObjectJSObject (strName));
	}

	JSVALUE V8JavascriptEngine::getAppObjectJSObject (std::string objectName)
	{
		v8::Local<v8::Value> objAppObject;

		if (objAppAppObjects->Has (v8::String::New (objectName.c_str (), objectName.size ())) == false)
			return (objAppObject);

		v8::Local<v8::Value> objReturn = objAppAppObjects->Get (v8::String::New (objectName.c_str (), objectName.size ()));

		if (objReturn.IsEmpty () == false)
		{
			if (objReturn->IsNull () == false)
				objAppObject = objReturn->ToObject ();
		}

		return (objAppObject);
	}

	void V8JavascriptEngine::removeAppObjectJSObject (AppObject *object)
	{
		std::string strName = object->getName ();
		objAppAppObjects->Delete (v8::String::New (strName.c_str (), strName.size ()));
	}

	void V8JavascriptEngine::addMovableJSObject (Movable *object)
	{
		std::string strName = object->getName ();
		v8::Local<v8::Object> objMovable = createMovableJSObject (object);
		objGameMovables->Set (v8::String::New (strName.c_str (), strName.size ()), objMovable);
	}

	v8::Local<v8::Value> V8JavascriptEngine::getMovableJSObject (Movable *object)
	{
		std::string strName = object->getName ();
		return (getMovableJSObject (strName));
	}

	v8::Local<v8::Value> V8JavascriptEngine::getMovableJSObject (std::string objectName)
	{
		v8::Local<v8::Value> objMovable;

		if (objGameMovables->Has (v8::String::New (objectName.c_str (), objectName.size ())) == false)
			return (objMovable);

		v8::Local<v8::Value> objReturn = objGameMovables->Get (v8::String::New (objectName.c_str (), objectName.size ()));

		if (objReturn.IsEmpty () == false)
		{
			if (objReturn->IsNull () == false)
				objMovable = objReturn->ToObject ();
		}

		return (objMovable);
	}

	void V8JavascriptEngine::removeMovableJSObject (Movable *object)
	{
		std::string strName = object->getName ();
		objGameMovables->Delete (v8::String::New (strName.c_str (), strName.size ()));
	}

	v8::Local<v8::Value> V8JavascriptEngine::getActiveNetworkServerJSObject (unsigned int index)
	{
		v8::Local<v8::Value> objNetworkServer;

		if (objActiveNetworkServers->Has (index) == false)
			return (objNetworkServer);

		v8::Local<v8::Value> objReturn = objActiveNetworkServers->Get (index);

		if (objReturn.IsEmpty () == false)
		{
			if (objReturn->IsNull () == false)
				objNetworkServer = objReturn->ToObject ();
		}

		return (objNetworkServer);
	}

	v8::Local<v8::Value> V8JavascriptEngine::getActiveNetworkClientJSObject (unsigned int index)
	{
		v8::Local<v8::Value> objNetworkClient;

		if (objActiveNetworkClients->Has (index) == false)
			return (objNetworkClient);

		v8::Local<v8::Value> objReturn = objActiveNetworkClients->Get (index);

		if (objReturn.IsEmpty () == false)
		{
			if (objReturn->IsNull () == false)
				objNetworkClient = objReturn->ToObject ();
		}

		return (objNetworkClient);
	}

	std::string V8JavascriptEngine::getStringFromStackTrace (v8::TryCatch *stackTrace)
	{
		std::string strReturn = "";

		v8::String::AsciiValue avString1 (stackTrace->Exception ());
		v8::Handle<v8::Message> msgException = stackTrace->Message ();

		strReturn += MSG26 + (std::string)(*avString1) + "\n";

		v8::String::AsciiValue avString2 (msgException->GetSourceLine ());
		int iLineNum = msgException->GetLineNumber ();
		int iColStartNum = msgException->GetStartColumn ();
		int iColEndNum = msgException->GetStartColumn ();

		strReturn += (*avString2);
		strReturn += MSG27 + RadXmlCommon::s13IntToString (iLineNum) + MSG28;
		strReturn += RadXmlCommon::s13IntToString (iColStartNum) + "-" + RadXmlCommon::s13IntToString (iColEndNum) + "\n";

		return (strReturn);
	}

	v8::Handle<v8::Value> V8JavascriptEngine::v8JavascriptFunction (const v8::Arguments &arguments)
	{
		v8::Local<v8::External> extExternal = v8::Local<v8::External>::Cast (arguments.Data ());
		NativeCallback ncNativeCallback = static_cast<NativeCallback> (extExternal->Value ());
		Array *aryArray = new Array ("array");

		for (unsigned int iIdx = 0; iIdx < arguments.Length (); iIdx++)
		{
			aryArray->addElement (((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
				getJavascriptEngine ())->getVariableFromJSObject (arguments[iIdx]));
		}

		Variable *vThis = 0;
		v8::Local<v8::Object> vlThis = arguments.This ();

		if (vlThis->IsNull () == false)
		{
			v8::Handle<v8::Value> hNull;
			v8::Local<v8::Value> vlVal = vlThis->GetHiddenValue (v8::String::New ("variable"));

			if (vlVal == hNull)
			{
				vThis = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
					getJavascriptEngine ())->getVariableFromJSObject (vlThis);
				vlThis->SetHiddenValue (v8::String::New ("variable"), v8::External::New (vThis));
			}
			else
				vThis = static_cast<Variable *> (v8::Local<v8::External>::Cast (vlVal)->Value ());

			std::string strText = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
				getJavascriptEngine ())->convertV8StringToString (vlThis->ToDetailString ());

			if (strText == "#<NetworkServer>")
			{
				unsigned int iLength = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
								getJavascriptEngine ())->objActiveNetworkServers->Length ();
				((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
					getJavascriptEngine ())->objActiveNetworkServers->Set (iLength, vlThis);
				vThis->addHiddenData ("networkserverindex", new unsigned int (iLength));
			}

			if (strText == "#<NetworkClient>")
			{
				unsigned int iLength = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
								getJavascriptEngine ())->objActiveNetworkClients->Length ();
				((V8JavascriptEngine *)RadXML::getSingletonPtr ()->getJavascriptEngine ())->\
												objActiveNetworkClients->Set (iLength, vlThis);
				vThis->addHiddenData ("networkclientindex", new unsigned int (iLength));
			}
		}

		Variable *vReturn = ncNativeCallback (vThis, aryArray);
		v8::Local<v8::Value> lReturn = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
								getJavascriptEngine ())->getJSObjectFromVariable (vReturn);

		delete aryArray;
		aryArray = 0;

		if (vThis != 0)
			vlThis->SetHiddenValue (v8::String::New ("variable"), v8::External::New (vThis));

		return (lReturn);
	}

	Variable *V8JavascriptEngine::getVariableFromPrototype (std::string prototypeName)
	{
		v8::Handle<v8::Value> hNull[1];
		v8::Local<v8::Value> vClass;
		v8::Local<v8::Value> vClone;

		vClass = cContext->Global ()->\
			GetRealNamedPropertyInPrototypeChain (v8::String::New (prototypeName.c_str (), prototypeName.size ()));

		if (vClass->IsFunction () == true)
		{
			v8::Local<v8::Object> objClass = v8::Local<v8::Object>::Cast (vClass);
			vClone = objClass->CallAsConstructor (0, hNull);
			//v8::Local<v8::Function> objClass = v8::Local<v8::Function>::Cast (vClass);
			//vClone = objClass->NewInstance ();
		}
		else
			vClone = vClass;

		RadXml::Variable *vReturn = getVariableFromJSObject (vClone, "", prototypeName);

		return (vReturn);
	}

	bool V8JavascriptEngine::hasGlobalFunction (std::string functionName)
	{
		v8::Local<v8::Value> vValue = cContext->Global ()->\
			GetRealNamedPropertyInPrototypeChain (v8::String::New (functionName.c_str (), functionName.size ()));

		if (vValue.IsEmpty () == false)
			return (true);

		return (false);
	}

	v8::Local<v8::Value> V8JavascriptEngine::callGlobalJSFunction (std::string functionName, std::vector<Variable *> passingVariables)
	{
		v8::Local<v8::Value> vReturn;
		v8::Local<v8::Value> vValue = cContext->Global ()->\
			GetRealNamedPropertyInPrototypeChain (v8::String::New (functionName.c_str (), functionName.size ()));

		if (vValue.IsEmpty () == false)
		{
			v8::Local<v8::Object> objObject = v8::Local<v8::Object>::Cast (vValue);

			if (vValue->IsFunction () == true)
			{
				v8::Handle<v8::Value> *aryVariables = new v8::Handle<v8::Value> [passingVariables.size ()];

				for (unsigned int iIdx = 0; iIdx < passingVariables.size (); iIdx++)
				{
					Variable *vVar = passingVariables.at (iIdx);
					v8::Local<v8::Value> vJSVar = ((V8JavascriptEngine *)RadXML::getSingletonPtr ()->\
												getJavascriptEngine ())->getJSObjectFromVariable (vVar);
					aryVariables[iIdx] = vJSVar;
				}

				vReturn = objObject->CallAsFunction (cContext->Global (), passingVariables.size (), aryVariables);

				delete []aryVariables;
				aryVariables = 0;
			}
		}

		return (vReturn);
	}

	Variable *V8JavascriptEngine::callGlobalFunction (std::string functionName, std::vector<Variable *> passingVariables)
	{
		return (getVariableFromJSObject (callGlobalJSFunction (functionName, passingVariables)));
	}

	v8::Local<v8::Value> V8JavascriptEngine::getGlobalJSVariable (std::string variableName)
	{
		v8::Local<v8::Value> vReturn = cContext->Global ()->Get (v8::String::New (variableName.c_str (), variableName.size ()));

		if (vReturn.IsEmpty () == false)
			return (vReturn);

		return (vReturn);
	}

	Variable *V8JavascriptEngine::getGlobalVariable (std::string variableName)
	{
		v8::Local<v8::Value> vReturn = cContext->Global ()->Get (v8::String::New (variableName.c_str (), variableName.size ()));

		if (vReturn.IsEmpty () == false)
			return (getVariableFromJSObject (vReturn));

		return (0);
	}

	v8::Local<v8::Value> V8JavascriptEngine::executeFunction (std::string functionName, v8::Local<v8::Value> variable, 
															std::vector< v8::Local<v8::Value> > passingVariables)
	{
		v8::Local<v8::Value> vReturn;

		if (variable->IsNull () == true)
			return (vReturn);

		if (variable->IsUndefined () == true)
			return (vReturn);

		v8::Local<v8::Object> objObject = v8::Local<v8::Object>::Cast (variable);

		v8::Local<v8::Value> vFound = objObject->Get (v8::String::New (functionName.c_str (), functionName.size ()));

		if (vFound->IsFunction () == true)
		{
			v8::Local<v8::Object> objFound = v8::Local<v8::Object>::Cast (vFound);
			v8::Handle<v8::Value> *aryVariables = new v8::Handle<v8::Value> [passingVariables.size ()];

			for (unsigned int iIdx = 0; iIdx < passingVariables.size (); iIdx++)
				aryVariables[iIdx] = passingVariables.at (iIdx);

			vReturn = objFound->CallAsFunction (cContext->Global (), passingVariables.size (), aryVariables);

			delete []aryVariables;
			aryVariables = 0;
		}

		return (vReturn);
	}

	v8::Local<v8::Value> V8JavascriptEngine::getJSObjectFromVariable (Variable *object)
	{
		v8::Local<v8::Value> objReturn;

		if (object == 0)
			return (objReturn);

		switch (object->getType ())
		{
			case VT_BOOLEAN:
				objReturn = v8::Boolean::New (static_cast<Boolean *> (object)->getValue ())->ToBoolean ();
				break;
			case VT_NUMBER:
				objReturn = v8::Number::New (static_cast<Number *> (object)->getValue ());
				break;
			case VT_STRING:
				{
					std::string strNew = static_cast<String *> (object)->getValue ();
					objReturn = v8::String::New (strNew.c_str (), strNew.size ());
				}
				break;
			case VT_ARRAY:
				{
					Array *aryArray = static_cast<Array *> (object);
					v8::Local<v8::Array> aryNew = v8::Array::New (aryArray->size ());

					for (unsigned int iJdx = 0; iJdx < aryArray->size (); iJdx++)
					{
						Variable *vVar2 = aryArray->getElement (iJdx);
						v8::Local<v8::Value> objVar = getJSObjectFromVariable (vVar2);

						aryNew->Set (iJdx, objVar);
					}

					objReturn = aryNew;
				}
				break;
			case VT_CLASS:
			case VT_CLASS_OBJECT:
				{
					Class *cClass = static_cast<Class *> (object);

					v8::Handle<v8::Value> hNull[1];
					v8::Local<v8::Value> vObj = cContext->Global ()->GetRealNamedPropertyInPrototypeChain (
														v8::String::New (cClass->getTypeName ().c_str ()));
					v8::Local<v8::Object> objToClone = v8::Local<v8::Object>::Cast (vObj);
					v8::Local<v8::Value> vNew = objToClone->CallAsConstructor (0, hNull);
					v8::Local<v8::Object> objNew = v8::Local<v8::Object>::Cast (vNew);
					std::vector<std::string> *aryHiddenNames = new std::vector<std::string> ();

					for (unsigned int iIdx = 0; iIdx < cClass->size (); iIdx++)
					{
						Variable *vVar = cClass->get (iIdx);

						if (vVar == 0)
							continue;

						if (vVar->getType () == VT_FUNCTION)
							continue;

						std::string strName = vVar->getName ();
						v8::Local<v8::Value> vValue = getJSObjectFromVariable (vVar);

						objNew->Set (v8::String::New (strName.c_str (), strName.size ()), vValue);
					}

					for (unsigned int iIdx = 0; iIdx < cClass->getNumHiddenData (); iIdx++)
					{
						void *vData = cClass->getHiddenDataByIndex (iIdx);
						std::string strDataName = cClass->getHiddenDataNameByIndex (iIdx);

						objNew->SetHiddenValue (v8::String::New (strDataName.c_str ()), v8::External::New (vData));
						aryHiddenNames->push_back (strDataName);
					}

					objNew->SetHiddenValue (v8::String::New ("hiddenNames"), v8::External::New (aryHiddenNames));
					objReturn = objNew;
				}
				break;
			case VT_OBJECT:
				{
					RadXml::RadXmlObject *robjObject = static_cast<RadXml::RadXmlObject *> (object);
					RadXml::AppObject *aoObject = robjObject->getAppObject ();
					RadXml::Movable *mObject = robjObject->getMovableObject ();

					if (aoObject != 0)
						objReturn = getAppObjectJSObject (aoObject);

					if (mObject != 0)
						objReturn = getMovableJSObject (mObject);
				}
				break;
		}

		return (objReturn);
	}

	std::vector<std::string> *V8JavascriptEngine::getHiddenNamesFromJSObject (JSVALUE object)
	{
		std::vector<std::string> *aryReturn = 0;

		if ((object->IsObject () == true) || (object->IsFunction () == true))
		{
			v8::Local<v8::Object> objObject = object->ToObject ();
			v8::Local<v8::External> extExternal = v8::Local<v8::External>::Cast(objObject->GetHiddenValue (v8::String::New ("hiddenNames")));

			if (extExternal.IsEmpty () == false)
				aryReturn = static_cast<std::vector<std::string> *> (extExternal->Value ());
		}

		return (aryReturn);
	}

	Variable *V8JavascriptEngine::getVariableFromJSObject (v8::Local<v8::Value> object, std::string name, std::string typeName)
	{
		Variable *vReturn = 0;
		std::string strVarName = name;

		if (object.IsEmpty () == true)
			return (vReturn);

		if (object->IsBoolean () == true)
		{
			v8::Local<v8::Boolean> bVal = object->ToBoolean ();

			vReturn = new Boolean (strVarName, bVal->Value ());
		}

		if (object->IsNumber () == true)
		{
			v8::Local<v8::Number> nVal = object->ToNumber ();

			vReturn = new Number (strVarName, nVal->Value ());
		}

		if (object->IsString () == true)
		{
			v8::Local<v8::String> vString = object->ToString ();
			std::string strString = convertV8StringToString (vString);

			vReturn = new String (strVarName, strString);
		}

		if (object->IsArray () == true)
		{
			Array *aryArray = new Array (strVarName);
			v8::Local<v8::Array> objArray = v8::Local<v8::Array>::Cast (object);

			for (unsigned int iIdx = 0; iIdx < objArray->Length (); iIdx++)
			{
				v8::Local<v8::Value> vVal = objArray->Get (iIdx);
				Variable *vVar = getVariableFromJSObject (vVal);
				aryArray->addElement (vVar);
			}

			vReturn = aryArray;
		}
		else if (object->IsFunction () == true)
		{
			Function *fFunc = new Function (strVarName);
			v8::Local<v8::Function> objObject = v8::Local<v8::Function>::Cast (object);
			v8::Local<v8::Array> aryOwnProperties = objObject->GetOwnPropertyNames ();
			v8::Local<v8::Array> aryProperties = objObject->GetPropertyNames ();
			std::vector<std::string> *aryHiddenNames = getHiddenNamesFromJSObject (objObject);
			int iCount = objObject->GetIndexedPropertiesExternalArrayDataLength ();
			std::vector<std::string> aryOwnPropertyNames;
			std::vector<std::string> aryPropertyNames;
			std::string strTypeName = convertV8StringToString (objObject->GetConstructorName ());
			std::string strTypeName2 = convertV8StringToString (objObject->GetName ()->ToString ());

			fFunc->setTypeName (strTypeName);

			if (strTypeName2 != "")
				fFunc->setTypeName (strTypeName2);

			if (aryHiddenNames != 0)
			{
				for (unsigned int iIdx = 0; iIdx < aryHiddenNames->size (); iIdx++)
				{
					std::string strName = aryHiddenNames->at (iIdx);
					v8::Local<v8::External> extExternal = v8::Local<v8::External>::Cast (
							objObject->GetHiddenValue (v8::String::New (strName.c_str ())));
					void *vData = extExternal->Value ();
					fFunc->addHiddenData (strName, vData);
				}
			}

			for (unsigned int iIdx = 0; iIdx < aryOwnProperties->Length (); iIdx++)
			{
				v8::Local<v8::Value> vVal = aryOwnProperties->Get (iIdx);
				std::string strVarName2 = convertV8StringToString (vVal->ToDetailString ());
				aryOwnPropertyNames.push_back (strVarName2);
			}

			for (unsigned int iIdx = 0; iIdx < aryProperties->Length (); iIdx++)
			{
				v8::Local<v8::Value> vVal = aryProperties->Get (iIdx);
				std::string strVarName2 = convertV8StringToString (vVal->ToDetailString ());
				aryPropertyNames.push_back (strVarName2);
			}

			for (unsigned int iIdx = 0; iIdx < aryPropertyNames.size (); iIdx++)
			{
				std::string strVarName2 = aryPropertyNames.at (iIdx);
				v8::Local<v8::Value> vVal = objObject->Get (v8::String::New (strVarName2.c_str (), strVarName2.size ()));
				Variable *vVar = getVariableFromJSObject (vVal, strVarName2);

				fFunc->set (strVarName2, vVar);
			}

			vReturn = fFunc;
		}
		else if (object->IsObject () == true)
		{
			v8::Local<v8::Object> objObject = object->ToObject ();
			v8::Local<v8::Array> aryProperties = objObject->GetPropertyNames ();
			std::vector<std::string> *aryHiddenNames = getHiddenNamesFromJSObject (objObject);
			std::vector<std::string> aryPropertyNames;
			std::string strTypeName = convertV8StringToString (objObject->GetConstructorName ());

			if (typeName != "")
				strTypeName = typeName;

			Class *vClass = new Class (strVarName, strTypeName);

			if (aryHiddenNames != 0)
			{
				for (unsigned int iIdx = 0; iIdx < aryHiddenNames->size (); iIdx++)
				{
					std::string strName = aryHiddenNames->at (iIdx);
					v8::Local<v8::External> extExternal = v8::Local<v8::External>::Cast (
							objObject->GetHiddenValue (v8::String::New (strName.c_str ())));
					void *vData = extExternal->Value ();
					vClass->addHiddenData (strName, vData);
				}
			}

			for (unsigned int iIdx = 0; iIdx < aryProperties->Length (); iIdx++)
			{
				v8::Local<v8::Value> vVal = aryProperties->Get (iIdx);
				std::string strVarName2 = convertV8StringToString (vVal->ToDetailString ());
				aryPropertyNames.push_back (strVarName2);
			}

			for (unsigned int iIdx = 0; iIdx < aryPropertyNames.size (); iIdx++)
			{
				std::string strVarName2 = aryPropertyNames.at (iIdx);
				v8::Local<v8::Value> vVal = objObject->Get (v8::String::New (strVarName2.c_str (), strVarName2.size ()));
				Variable *vVar = getVariableFromJSObject (vVal, strVarName2);

				vClass->set (strVarName2, vVar);
			}

			vReturn = vClass;
		}

		return (vReturn);
	}

	Class *V8JavascriptEngine::getVector2JSClass (double x, double y)
	{
		return ((Class *)getVariableFromJSObject (getVector2JSObject (x, y)));
	}

	Class *V8JavascriptEngine::getVector3JSClass (double x, double y, double z)
	{
		return ((Class *)getVariableFromJSObject (getVector3JSObject (x, y, z)));
	}

	Class *V8JavascriptEngine::getVector4JSClass (double x, double y, double z, double w)
	{
		return ((Class *)getVariableFromJSObject (getVector4JSObject (x, y, z, w)));
	}

	Class *V8JavascriptEngine::getQuaternionJSClass (Ogre::Quaternion rotation)
	{
		return ((Class *)getVariableFromJSObject (getQuaternionJSObject (rotation)));
	}

	Class *V8JavascriptEngine::createAppObjectJSClass (AppObject *object)
	{
		return ((Class *)getVariableFromJSObject (createAppObjectJSObject (object)));
	}

	Class *V8JavascriptEngine::getAppObjectJSClass (AppObject *object)
	{
		return ((Class *)getVariableFromJSObject (getAppObjectJSObject (object)));
	}

	Class *V8JavascriptEngine::getAppObjectJSClass (std::string objectName)
	{
		return ((Class *)getVariableFromJSObject (getAppObjectJSObject (objectName)));
	}

	Class *V8JavascriptEngine::createMovableJSClass (Movable *object)
	{
		return ((Class *)getVariableFromJSObject (createMovableJSObject (object)));
	}

	Class *V8JavascriptEngine::getMovableJSClass (Movable *object)
	{
		return ((Class *)getVariableFromJSObject (getMovableJSObject (object)));
	}

	Class *V8JavascriptEngine::getMovableJSClass (std::string objectName)
	{
		return ((Class *)getVariableFromJSObject (getMovableJSObject (objectName)));
	}
#endif
}

