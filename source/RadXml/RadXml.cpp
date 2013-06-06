#include "RadXml.h"

#include "RadXmlGlobal.h"

#include "RadXmlGame.h"
#include "RadXmlTimer.h"
#include "RadXmlSystem.h"

#include "RadXmlVector2.h"
#include "RadXmlVector3.h"
#include "RadXmlVector4.h"
#include "RadXmlQuaternion.h"

#include "RadXmlInput.h"

#include "RadXmlApp.h"
#include "RadXmlAppObject.h"
#include "RadXmlCombobox.h"

#include "RadXmlMovable.h"
#include "RadXmlEntity.h"
#include "RadXmlAnimationTrack.h"
#include "RadXmlOverlayElement.h"
#include "RadXmlSound.h"

#include "RadXmlSerialIO.h"

#include "RadXmlTextFile.h"

#include "RadXmlNetworking.h"

#include "common/mai_radxml_system.h"
#include "common/com_radxml_strconv.h"

#include "languages/language.h"

#ifdef defSYS_WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#ifdef defAPI_WXWIDGETS
	#include <wx/dynlib.h>

	#include "wxwidgets/RadXmlWxWidgets.h"
#endif

#ifdef defAPI_JAVASCRIPT_V8
	#include "RadXmlV8JavascriptEngine.h"
#endif

namespace RadXml
{
	RadXML *Singleton<RadXML>::rxSingleton = 0;

	RadXML::RadXML ()
		: Singleton<RadXML> ()
	{
		#ifdef defAPI_OGRE3D
			mSceneMgr = 0;
			camMain = 0;
			aryRenderWindows = 0;
		#endif

		#ifdef defAPI_CEGUI
			cegRootWindow = 0;
		#endif

		#ifdef defAPI_WXWIDGETS
			wApp = 0;
			bDestroyWxWidgetsApp = true;
		#endif

		xmlCurrent = 0;
		jsEngine = 0;
		strCurrentScriptDir = "";
	}

	RadXML::RadXML (const RadXML &copy)
	{
		aryTimers = copy.aryTimers;
		hshCallbacks = copy.hshCallbacks;

		aryDLLInstances = copy.aryDLLInstances;

		#ifdef defAPI_OGRE3D
			mSceneMgr = copy.mSceneMgr;
			camMain = copy.camMain;
			aryRenderWindows = copy.aryRenderWindows;
		#endif

		strCurrentScriptDir = copy.strCurrentScriptDir;

		xmlCurrent = copy.xmlCurrent;
		jsEngine = copy.jsEngine;

		#ifdef defAPI_WXWIDGETS
			wApp = copy.wApp;
		#endif

		rxSingleton = copy.rxSingleton;
	}

	RadXML::~RadXML ()
	{
		if (xmlCurrent != 0)
		{
			delete xmlCurrent;
			xmlCurrent = 0;
		}
	}

	#ifdef defAPI_WXWIDGETS
	void RadXML::initialise (wxRadXmlWidgetsApp *app, JavascriptEngine *engine, bool initializeWxWidgets)
	{
		RadXML *rXML = 0;

		if (rxSingleton == 0)
			rXML = new RadXML ();
		else
			rXML = getSingletonPtr ();

		rXML->wApp = app;

		if (initializeWxWidgets == true)
		{
			int iArgs = 0;
			wxChar **cArgs = 0;

			wxApp::SetInstance ((wxAppConsole *)rXML->wApp);
			wxEntryStart (iArgs, cArgs);
			((wxApp *)rXML->wApp)->OnInit ();
		}

		initialise (engine);
	}
	#endif

	void RadXML::initialise (JavascriptEngine *engine)
	{
		RadXML *rXML = 0;

		if (rxSingleton == 0)
			rXML = new RadXML ();
		else
			rXML = getSingletonPtr ();

		#ifdef defAPI_WXWIDGETS
			if (rXML->wApp == 0)
			{
				int iArgs = 0;
				wxChar **cArgs = 0;

				rXML->wApp = new wxWidgetsRadXml ();

				wxApp::SetInstance ((wxAppConsole *)rXML->wApp);
				wxEntryStart (iArgs, cArgs);
				((wxApp *)rXML->wApp)->OnInit ();
			}
		#endif

		JavascriptManager::initialise ();
		PluginManager::initialise ();

		rXML->loadPlugins ();
		rXML->setupJavascript ();
	}

	void RadXML::destroy ()
	{
		#ifdef defAPI_WXWIDGETS
			if (rxSingleton->bDestroyWxWidgetsApp == true)
				wxEntryCleanup ();
		#endif

		JavascriptManager::destroy ();
		PluginManager::destroy ();

		if (rxSingleton != 0)
		{
			delete rxSingleton;
			rxSingleton = 0;
		}
	}

	void RadXML::loadPlugins ()
	{
		unsigned int iSize = PluginManager::getSingletonPtr ()->getNumPlugins ();

		for (unsigned int iIdx = 0; iIdx < iSize; iIdx++)
		{
			Plugin *plgPlugin = PluginManager::getSingletonPtr ()->getPlugin (iIdx);
			int iIndex = loadLibrary (plgPlugin->getFilePath ());

			if (iIndex == -1)
			{
				RadXmlCommon::s13LogMessage (MSG33 + plgPlugin->getFilePath ());

				continue;
			}

			#ifndef defAPI_WXWIDGETS
				#ifdef defSYS_WIN32
					getDLLFunction (iIdx, "pluginLoaded") ();
				#endif
			#else
				RadXmlFunctionReturn rRet = (RadXmlFunctionReturn)getDLLFunction (iIdx, "pluginInformation");

				if (rRet == 0)
				{
					RadXmlCommon::s13LogMessage (MSG34 + plgPlugin->getFilePath ());

					continue;
				}

				Plugin *plgInfo = (Plugin *)(rRet ());

				RadXmlCommon::s13LogMessage (MSG31 + plgInfo->getName () + MSG32 + RadXmlCommon::s13DoubleToString (plgInfo->getVersion ()));

				RadXmlFunction rFunc = (RadXmlFunction)getDLLFunction (iIdx, "pluginLoaded");

				if (rFunc == 0)
				{
					RadXmlCommon::s13LogMessage (MSG35 + plgPlugin->getFilePath ());

					continue;
				}

				rFunc ();
			#endif
		}
	}

	bool RadXML::setupJavascript (JavascriptEngine *engine)
	{
		if (engine == 0)
		{
			#ifdef defAPI_JAVASCRIPT_V8
				engine = new V8JavascriptEngine ();
			#endif
		}

		jsEngine = engine;

		if (jsEngine->initializeEngine () == false)
			return (false);

		Vector2Class::setupVector2Class ();
		Vector3Class::setupVector3Class ();
		Vector4Class::setupVector4Class ();
		QuaternionClass::setupQuaternionClass ();

		AppObjectClass::setupAppObject ();
		ComboBoxClass::setupComboBox ();

		SystemClass::setupSystemClass ();

		MovableClass::setupMovableClass ();
		EntityClass::setupEntityClass ();
		AnimationTrackClass::setupAnimationTrackClass ();
		OverlayElementClass::setupOverlayElementClass ();

		SerialIOClass::setupSerialIOClass ();
		TextFileClass::setupTextFileClass ();

		NetworkServerClass::setupNetworkServerClass ();
		NetworkClientClass::setupNetworkClientClass ();

		SoundClass::setupSoundClass ();

		GameClass::setupGameClass ();
		AppClass::setupAppJS ();

		GlobalClass::setupGlobalClass ();

		//setupInputClass ();

		if (jsEngine->setupNativeFunctions () == false)
			return (false);

		jsEngine->compileAndRunJSString ((std::string)"function InputEvent ()\n " +
(std::string)"{\n"+
(std::string)"	this.key = 0;\n"+
(std::string)"	this.buttons = new Array ();\n"+
(std::string)"	this.x = 0;\n"+
(std::string)"	this.y = 0;\n"+
(std::string)"	this.z = 0;\n"+
(std::string)"	this.relX = 0;\n"+
(std::string)"	this.relY = 0;\n"+
(std::string)"	this.relZ = 0;\n"+
(std::string)"}\n");
		/*jsEngine->compileAndRunJSString ((std::string)"var InputEvent = " +
(std::string)"{"+
(std::string)"	key: 0, "+
(std::string)"	buttons: new Array (), "+
(std::string)"	x: 0, "+
(std::string)"	y: 0, "+
(std::string)"	z: 0, "+
(std::string)"	relX: 0, "+
(std::string)"	relY: 0, "+
(std::string)"	relZ: 0"+
(std::string)"};");*/

		jsEngine->compileAndRunJSString ((std::string)"var Game = "+
(std::string)"{"+
(std::string)"	Movables: new Array (), "+
(std::string)"	ENTITY: 1, "+
(std::string)"	CAMERA: 2, "+
(std::string)"	PARTICLE_SYSTEM: 3, "+
(std::string)"	LIGHT: 4, "+
(std::string)"	ANIMATION_TRACK: 5, "+
(std::string)"	AUDIO_LISTENER: 6, "+
(std::string)"	SOUND: 7"+
(std::string)"};");

		jsEngine->compileAndRunJSString ((std::string)"var Networking = "+
(std::string)"{"+
(std::string)"	ActiveServers: new Array (), "+
(std::string)"	ActiveClients: new Array ()"+
(std::string)"};");

		jsEngine->compileAndRunJSString ((std::string)"var App = "+
(std::string)"{"+
(std::string)"	AppObjects: new Array (), "+
(std::string)"};");

		jsEngine->compileAndRunJSString ((std::string)"var Key = \n"+
(std::string)"{\n"+
	(std::string)"UNASSIGNED  : 0x00,\n"+
	(std::string)"ESCAPE      : 0x01,\n"+
	(std::string)"_1           : 0x02,\n"+
	(std::string)"_2           : 0x03,\n"+
	(std::string)"_3           : 0x04,\n"+
	(std::string)"_4           : 0x05,\n"+
	(std::string)"_5           : 0x06,\n"+
	(std::string)"_6           : 0x07,\n"+
	(std::string)"_7           : 0x08,\n"+
	(std::string)"_8           : 0x09,\n"+
	(std::string)"_9           : 0x0A,\n"+
	(std::string)"_0           : 0x0B,\n"+
	(std::string)"MINUS       : 0x0C,    // - on main keyboard\n"+
	(std::string)"EQUALS      : 0x0D,\n"+
	(std::string)"BACK        : 0x0E,    // backspace\n"+
	(std::string)"TAB         : 0x0F,\n"+
	(std::string)"Q           : 0x10,\n"+
	(std::string)"W           : 0x11,\n"+
	(std::string)"E           : 0x12,\n"+
	(std::string)"R           : 0x13,\n"+
	(std::string)"T           : 0x14,\n"+
	(std::string)"Y           : 0x15,\n"+
	(std::string)"U           : 0x16,\n"+
	(std::string)"I           : 0x17,\n"+
	(std::string)"O           : 0x18,\n"+
	(std::string)"P           : 0x19,\n"+
	(std::string)"LBRACKET    : 0x1A,\n"+
	(std::string)"RBRACKET    : 0x1B,\n"+
	(std::string)"RETURN      : 0x1C,    // Enter on main keyboard\n"+
	(std::string)"LCONTROL    : 0x1D,\n"+
	(std::string)"A           : 0x1E,\n"+
	(std::string)"S           : 0x1F,\n"+
	(std::string)"D           : 0x20,\n"+
	(std::string)"F           : 0x21,\n"+
	(std::string)"G           : 0x22,\n"+
	(std::string)"H           : 0x23,\n"+
	(std::string)"J           : 0x24,\n"+
	(std::string)"K           : 0x25,\n"+
	(std::string)"L           : 0x26,\n"+
	(std::string)"SEMICOLON   : 0x27,\n"+
	(std::string)"APOSTROPHE  : 0x28,\n"+
	(std::string)"GRAVE       : 0x29,    // accent\n"+
	(std::string)"LSHIFT      : 0x2A,\n"+
	(std::string)"BACKSLASH   : 0x2B,\n"+
	(std::string)"Z           : 0x2C,\n"+
	(std::string)"X           : 0x2D,\n"+
	(std::string)"C           : 0x2E,\n"+
	(std::string)"V           : 0x2F,\n"+
	(std::string)"B           : 0x30,\n"+
	(std::string)"N           : 0x31,\n"+
	(std::string)"M           : 0x32,\n"+
	(std::string)"COMMA       : 0x33,\n"+
	(std::string)"PERIOD      : 0x34,    // . on main keyboard\n"+
	(std::string)"SLASH       : 0x35,    // / on main keyboard\n"+
	(std::string)"RSHIFT      : 0x36,\n"+
	(std::string)"MULTIPLY    : 0x37,    // * on numeric keypad\n"+
	(std::string)"LMENU       : 0x38,    // left Alt\n"+
	(std::string)"SPACE       : 0x39,\n"+
	(std::string)"CAPITAL     : 0x3A,\n"+
	(std::string)"F1          : 0x3B,\n"+
	(std::string)"F2          : 0x3C,\n"+
	(std::string)"F3          : 0x3D,\n"+
	(std::string)"F4          : 0x3E,\n"+
	(std::string)"F5          : 0x3F,\n"+
	(std::string)"F6          : 0x40,\n"+
	(std::string)"F7          : 0x41,\n"+
	(std::string)"F8          : 0x42,\n"+
	(std::string)"F9          : 0x43,\n"+
	(std::string)"F10         : 0x44,\n"+
	(std::string)"NUMLOCK     : 0x45,\n"+
	(std::string)"SCROLL      : 0x46,    // Scroll Lock\n"+
	(std::string)"NUMPAD7     : 0x47,\n"+
	(std::string)"NUMPAD8     : 0x48,\n"+
	(std::string)"NUMPAD9     : 0x49,\n"+
	(std::string)"SUBTRACT    : 0x4A,    // - on numeric keypad\n"+
	(std::string)"NUMPAD4     : 0x4B,\n"+
	(std::string)"NUMPAD5     : 0x4C,\n"+
	(std::string)"NUMPAD6     : 0x4D,\n"+
	(std::string)"ADD         : 0x4E,    // + on numeric keypad\n"+
	(std::string)"NUMPAD1     : 0x4F,\n"+
	(std::string)"NUMPAD2     : 0x50,\n"+
	(std::string)"NUMPAD3     : 0x51,\n"+
	(std::string)"NUMPAD0     : 0x52,\n"+
	(std::string)"DECIMAL     : 0x53,    // . on numeric keypad\n"+
	(std::string)"OEM_102     : 0x56,    // < > | on UK/Germany keyboards\n"+
	(std::string)"F11         : 0x57,\n"+
	(std::string)"F12         : 0x58,\n"+
	(std::string)"F13         : 0x64,    //                     (NEC PC98)\n"+
	(std::string)"F14         : 0x65,    //                     (NEC PC98)\n"+
	(std::string)"F15         : 0x66,    //                     (NEC PC98)\n"+
	(std::string)"KANA        : 0x70,    // (Japanese keyboard)\n"+
	(std::string)"ABNT_C1     : 0x73,    // / ? on Portugese (Brazilian) keyboards\n"+
	(std::string)"CONVERT     : 0x79,    // (Japanese keyboard)\n"+
	(std::string)"NOCONVERT   : 0x7B,    // (Japanese keyboard)\n"+
	(std::string)"YEN         : 0x7D,    // (Japanese keyboard)\n"+
	(std::string)"ABNT_C2     : 0x7E,    // Numpad . on Portugese (Brazilian) keyboards\n"+
	(std::string)"NUMPADEQUALS: 0x8D,    // : on numeric keypad (NEC PC98)\n"+
	(std::string)"PREVTRACK   : 0x90,    // Previous Track (CIRCUMFLEX on Japanese keyboard)\n"+
	(std::string)"AT          : 0x91,    //                     (NEC PC98)\n"+
	(std::string)"COLON       : 0x92,    //                     (NEC PC98)\n"+
	(std::string)"UNDERLINE   : 0x93,    //                     (NEC PC98)\n"+
	(std::string)"KANJI       : 0x94,    // (Japanese keyboard)\n"+
	(std::string)"STOP        : 0x95,    //                     (NEC PC98)\n"+
	(std::string)"AX          : 0x96,    //                     (Japan AX)\n"+
	(std::string)"UNLABELED   : 0x97,    //                        (J3100)\n"+
	(std::string)"NEXTTRACK   : 0x99,    // Next Track\n"+
	(std::string)"NUMPADENTER : 0x9C,    // Enter on numeric keypad\n"+
	(std::string)"RCONTROL    : 0x9D,\n"+
	(std::string)"MUTE        : 0xA0,    // Mute\n"+
	(std::string)"CALCULATOR  : 0xA1,    // Calculator\n"+
	(std::string)"PLAYPAUSE   : 0xA2,    // Play / Pause\n"+
	(std::string)"MEDIASTOP   : 0xA4,    // Media Stop\n"+
	(std::string)"VOLUMEDOWN  : 0xAE,    // Volume -\n"+
	(std::string)"VOLUMEUP    : 0xB0,    // Volume +\n"+
	(std::string)"WEBHOME     : 0xB2,    // Web home\n"+
	(std::string)"NUMPADCOMMA : 0xB3,    // , on numeric keypad (NEC PC98)\n"+
	(std::string)"DIVIDE      : 0xB5,    // / on numeric keypad\n"+
	(std::string)"SYSRQ       : 0xB7,\n"+
	(std::string)"RMENU       : 0xB8,    // right Alt\n"+
	(std::string)"PAUSE       : 0xC5,    // Pause\n"+
	(std::string)"HOME        : 0xC7,    // Home on arrow keypad\n"+
	(std::string)"UP          : 0xC8,    // UpArrow on arrow keypad\n"+
	(std::string)"PGUP        : 0xC9,    // PgUp on arrow keypad\n"+
	(std::string)"LEFT        : 0xCB,    // LeftArrow on arrow keypad\n"+
	(std::string)"RIGHT       : 0xCD,    // RightArrow on arrow keypad\n"+
	(std::string)"END         : 0xCF,    // End on arrow keypad\n"+
	(std::string)"DOWN        : 0xD0,    // DownArrow on arrow keypad\n"+
	(std::string)"PGDOWN      : 0xD1,    // PgDn on arrow keypad\n"+
	(std::string)"INSERT      : 0xD2,    // Insert on arrow keypad\n"+
	(std::string)"DELETE      : 0xD3,    // Delete on arrow keypad\n"+
	(std::string)"LWIN        : 0xDB,    // Left Windows key\n"+
	(std::string)"RWIN        : 0xDC,    // Right Windows key\n"+
	(std::string)"APPS        : 0xDD,    // AppMenu key\n"+
	(std::string)"POWER       : 0xDE,    // System Power\n"+
	(std::string)"SLEEP       : 0xDF,    // System Sleep\n"+
	(std::string)"WAKE        : 0xE3,    // System Wake\n"+
	(std::string)"WEBSEARCH   : 0xE5,    // Web Search\n"+
	(std::string)"WEBFAVORITES: 0xE6,    // Web Favorites\n"+
	(std::string)"WEBREFRESH  : 0xE7,    // Web Refresh\n"+
	(std::string)"WEBSTOP     : 0xE8,    // Web Stop\n"+
	(std::string)"WEBFORWARD  : 0xE9,    // Web Forward\n"+
	(std::string)"WEBBACK     : 0xEA,    // Web Back\n"+
	(std::string)"MYCOMPUTER  : 0xEB,    // My Computer\n"+
	(std::string)"MAIL        : 0xEC,    // Mail\n"+
	(std::string)"MEDIASELECT : 0xED     // Media Select\n"+
(std::string)"};");

	jsEngine->compileAndRunJSString ((std::string)"function newVector2 (newX, newY)\n"+
(std::string)"{\n"+
	(std::string)"var vReturn = new Vector2 ();\n"+
	(std::string)"vReturn.x = newX;\n"+
	(std::string)"vReturn.y = newY;\n"+
	(std::string)"return (vReturn);\n"+
(std::string)"}\n"+
(std::string)"function newVector3 (newX, newY, newZ)\n"+
(std::string)"{\n"+
	(std::string)"var vReturn = new Vector3 ();\n"+
	(std::string)"vReturn.x = newX;\n"+
	(std::string)"vReturn.y = newY;\n"+
	(std::string)"vReturn.z = newZ;\n"+
	(std::string)"return (vReturn);\n"+
(std::string)"}\n"+
(std::string)"function newVector4 (newX, newY, newZ, newW)\n"+
(std::string)"{\n"+
	(std::string)"var vReturn = new Vector4 ();\n"+
	(std::string)"vReturn.x = newX;\n"+
	(std::string)"vReturn.y = newY;\n"+
	(std::string)"vReturn.z = newZ;\n"+
	(std::string)"vReturn.w = newW;\n"+
	(std::string)"return (vReturn);\n"+
(std::string)"}\n");

		if (jsEngine->finishInitializingEngine () == false)
			return (false);

		return (true);
	}

	void RadXML::javascriptDebugger ()
	{
	}

	bool RadXML::compileAndRunJSScript (std::string file)
	{
		return (jsEngine->compileAndRunJSScript (file));
	}

	bool RadXML::compileAndRunJSString (std::string string)
	{
		return (jsEngine->compileAndRunJSString (string));
	}

	Ogre::ColourValue RadXML::parseColourValue (std::string color)
	{
		Ogre::ColourValue cvValue = Ogre::ColourValue::Black;

		if (color == "")
			return (cvValue);

		unsigned int iPos = 0;
		iPos = color.find ("#");
		color = RadXmlCommon::s13ASCII_StringToLower (color);

		if (color == "black")
			cvValue = Ogre::ColourValue::Black;

		if (color == "white")
			cvValue = Ogre::ColourValue::White;

		if (color == "red")
			cvValue = Ogre::ColourValue::Red;

		if (color == "green")
			cvValue = Ogre::ColourValue::Green;

		if (color == "blue")
			cvValue = Ogre::ColourValue::Blue;

		if (iPos != std::string::npos)
			iPos++;

		std::string strR = color.substr ((iPos + 0), 2);
		std::string strG = color.substr ((iPos + 2), 2);
		std::string strB = color.substr ((iPos + 4), 2);
		int iR = RadXmlCommon::s13HexStringToInt (strR);
		int iG = RadXmlCommon::s13HexStringToInt (strG);
		int iB = RadXmlCommon::s13HexStringToInt (strB);
		double dR = (iR / 255.0);
		double dG = (iG / 255.0);
		double dB = (iB / 255.0);

		cvValue.r = dR;
		cvValue.g = dG;
		cvValue.b = dB;

		return (cvValue);
	}

	Timer *RadXML::createTimer (std::string javascript, unsigned int milliseconds)
	{
		Timer *tTimer = new Timer (javascript, milliseconds);
		aryTimers.push_back (tTimer);

		return (tTimer);
	}

	void RadXML::executeTimers (double timeSinceLastFrame)
	{
		for (unsigned int iIdx = 0; iIdx < aryTimers.size (); iIdx++)
		{
			Timer *tTimer = aryTimers.at (iIdx);

			if (tTimer->update (timeSinceLastFrame * 1000.0) == true)
			{
				delete tTimer;
				tTimer = 0;

				std::vector<Timer *>::iterator itBegin = aryTimers.begin ();
				itBegin += iIdx;
				aryTimers.erase (itBegin);

				continue;
			}
		}
	}

	bool RadXML::loadXml (std::string file)
	{
		xmlCurrent = new XmlFile (file);

		return (true);
	}

	void RadXML::setCallback (int jsEvent, EventCallback eventCallback)
	{
		hshCallbacks.insert (std::pair<int, EventCallback> (jsEvent, eventCallback));
	}

	Variable *RadXML::callCallback (int jsEvent, Variable *passingVariables)
	{
		HASHMAP<int, EventCallback>::iterator itBegin = hshCallbacks.find (jsEvent);
		HASHMAP<int, EventCallback>::iterator itEnd = hshCallbacks.end ();

		if (itBegin != itEnd)
		{
			EventCallback evtEvent = (*itBegin).second;
			Variable *vReturn = evtEvent (passingVariables);

			return (vReturn);
		}

		return (0);
	}

	int RadXML::loadLibrary (std::string filename)
	{
		#ifndef defAPI_WXWIDGETS
			#ifdef defSYS_WIN32
				HINSTANCE hInstanceTemp = LoadLibrary (filename.c_str ());

				if (hInstanceTemp == 0)
					return (-1);

				aryDLLInstances.push_back (hInstanceTemp);

				return (aryDLLInstances.size () - 1);
			#endif
		#else
			wxDynamicLibrary *wlLibrary = new wxDynamicLibrary ();

			wlLibrary->Load (wxT (filename.c_str ()));

			if (wlLibrary->IsLoaded () == false)
				return (-1);

			aryDLLInstances.push_back (wlLibrary);

			return (aryDLLInstances.size () - 1);
		#endif

		return (-1);
	}

	#ifndef defAPI_WXWIDGETS
		#ifdef defSYS_WIN32
			FARPROC RadXML::getDLLFunction (int dllIndex, std::string functionName)
			{
				return (GetProcAddress (aryDLLInstances.at (dllIndex), functionName.c_str ()));
			}
		#endif
	#else
		void *RadXML::getDLLFunction (int dllIndex, std::string functionName)
		{
			wxDynamicLibrary *dlLib = aryDLLInstances.at (dllIndex);

			if (dlLib->HasSymbol (wxT (functionName.c_str ())) == false)
				return (0);

			return (dlLib->GetSymbol (wxT (functionName.c_str ())));
		}
	#endif

	bool RadXML::unloadLibrary (int index)
	{
		#ifndef defAPI_WXWIDGETS
			#ifdef defSYS_WIN32
				FreeLibrary (aryDLLInstances.at (index));

				std::vector<HINSTANCE>::iterator itBegin = aryDLLInstances.begin ();
				itBegin += index;
				aryDLLInstances.erase (itBegin);
			#endif
		#endif

		return (true);
	}

	RXVECTOR2 RadXML::getMonitorSize (int monitorNumber)
	{
		RXVECTOR2 v2dReturn = RXVECTOR2::ZERO;

		/// @note This only returns the size of the current monitor for now.

		#ifdef defSYS_WIN32
			v2dReturn.x = GetSystemMetrics (SM_CXFULLSCREEN);
			v2dReturn.y = GetSystemMetrics (SM_CYFULLSCREEN);
		#else
			#ifdef defAPI_WXWIDGETS
				wxVideoMode wvmVideo = wxDefaultVideoMode;
				v2dReturn.x = wvmVideo.GetWidth ();
				v2dReturn.y = wvmVideo.GetHeight ();
			#endif
		#endif

		return (v2dReturn);
	}
}

