#ifndef _RADXML_H_
	#define _RADXML_H_

	#ifdef defAPI_WXWIDGETS
		#include <wx/app.h>
	#endif

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

	#ifdef defAPI_WXWIDGETS
		class wxDynamicLibrary;
	#endif

	#ifdef defAPI_OGRE3D
		#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
			#include <OGRE/OgreVector2.h>
			#include <OGRE/OgreVector3.h>
			#include <OGRE/OgreVector4.h>
			#include <OGRE/OgreSceneManager.h>
			#include <OGRE/OgreRenderWindow.h>
		#else
			#include <OgreVector2.h>
			#include <OgreVector3.h>
			#include <OgreVector4.h>
			#include <OgreSceneManager.h>
			#include <OgreRenderWindow.h>
		#endif
	#endif

	#ifdef defAPI_CEGUI
		#ifdef defAPI_CEGUI_NOT_IN_DIRECTORY
			#include <CEGUI/CEGUI.h>
		#else
			#include <CEGUI.h>
		#endif
	#endif

	#include <string>

	#include "RadXmlSingleton.h"
	#include "RadXmlJavascriptEngine.h"

	#include "RadXmlPreprocessor.h"

	#include "RadXmlNative.h"
	#include "RadXmlDatatypes.h"

	#include "RadXmlXmlFile.h"

	#include "RadXmlApp.h"
	#include "RadXmlAppObject.h"
	#include "RadXmlWindow.h"
	#include "RadXmlButton.h"
	#include "RadXmlComboBox.h"
	#include "RadXmlLabel.h"
	#include "RadXmlMenu.h"
	#include "RadXmlTextbox.h"
	#include "RadXmlMenuItem.h"

	#include "RadXmlGame.h"
	#include "RadXmlEntity.h"
	#include "RadXmlMovable.h"
	#include "RadXmlLight.h"
	#include "RadXmlParticleSystem.h"
	#include "RadXmlAnimationTrack.h"
	#include "RadXmlSound.h"
	#include "RadXmlAudioListener.h"
	#include "RadXmlOverlay.h"
	#include "RadXmlOverlayElement.h"

	#define defRADXML_MIN_VERSION			0.1
	#define defRADXML_VERSION				0.2

	#ifndef defAPI_WXWIDGETS
		#ifdef defSYS_WIN32
			typedef HINSTANCE;
		#endif
	#endif

	namespace RadXml
	{
		class Variable;
		class AppObject;
		class Movable;
		class Timer;
		class XmlFile;

		enum EntityMovables
		{
			defENTITY_NOTHING		=		(1 << 0),
			defENTITY_WORLD			=		(1 << 1),
			defENTITY_BUTTON		=		(1 << 2),
			defENTITY_OBJECT		=		(1 << 3)
		};

		enum JavascriptCallbacks
		{
			JCE_QUIT,
		};

		typedef bool (__stdcall *RadXmlFunction) (...);
		typedef void *(__stdcall *RadXmlFunctionReturn) (...);

		#ifdef defAPI_WXWIDGETS
			class wxRadXmlWidgetsApp: public wxApp
			{
				public:
			};
		#endif

		class defRADXML_EXPORT RadXML: public Singleton<RadXML>
		{
			public:
				static void initialise (JavascriptEngine *engine = 0);
				#ifdef defAPI_WXWIDGETS
					static void initialise (wxRadXmlWidgetsApp *app, JavascriptEngine *engine = 0, bool initializeWxWidgets = true);
				#endif
				static void destroy ();

				bool compileAndRunJSScript (std::string file);
				bool compileAndRunJSString (std::string string);

				static Ogre::ColourValue parseColourValue (std::string color);

				Timer *createTimer (std::string javascript, unsigned int milliseconds);
				void executeTimers (double timeSinceLastFrame);

				bool loadXml (std::string file);

				typedef Variable *(*EventCallback) (Variable *arguments);

				void setCallback (int jsEvent, EventCallback eventCallback);
				Variable *callCallback (int jsEvent, Variable *passingVariables);

				int loadLibrary (std::string filename);
				#ifndef defAPI_WXWIDGETS
					#ifdef defSYS_WIN32
						FARPROC getDLLFunction (int dllIndex, std::string functionName);
					#endif
				#else
					void *getDLLFunction (int dllIndex, std::string functionName);
				#endif
				bool unloadLibrary (int index);

				inline void setCurrentScriptDirectory (std::string current)
				{
					strCurrentScriptDir = current;
				}

				inline std::string getCurrentScriptDirectory ()
				{
					return (strCurrentScriptDir);
				}

				#ifdef defAPI_OGRE3D
					inline void setRenderWindows (std::vector<Ogre::RenderWindow *> *renderWindows)
					{
						aryRenderWindows = renderWindows;
					}

					inline std::vector<Ogre::RenderWindow *> *getRenderWindows ()
					{
						return (aryRenderWindows);
					}

					inline void setSceneManager (Ogre::SceneManager *manager)
					{
						mSceneMgr = manager;
					}

					inline Ogre::SceneManager *getSceneManager ()
					{
						return (mSceneMgr);
					}

					void setupCamera (Ogre::Camera *camCamera, Ogre::ColourValue color);

					inline void setCurrentCamera (Ogre::Camera *camera)
					{
						camMain = camera;
					}

					inline Ogre::Camera *getCurrentCamera ()
					{
						return (camMain);
					}
				#endif

				#ifdef defAPI_CEGUI
					inline void setCEGUIRootWindow (CEGUI::Window *window)
					{
						cegRootWindow = window;
					}

					inline CEGUI::Window *getCEGUIRootWindow ()
					{
						return (cegRootWindow);
					}
				#endif

				#ifdef defAPI_WXWIDGETS
					inline void setWXApp (wxRadXmlWidgetsApp *app)
					{
						wApp = app;
					}

					inline wxRadXmlWidgetsApp *getWXApp ()
					{
						return (wApp);
					}

					inline void setDestroyWxWidgets (bool destroy)
					{
						bDestroyWxWidgetsApp = destroy;
					}

					inline bool getDestroyWxWidgets ()
					{
						return (bDestroyWxWidgetsApp);
					}
				#endif

				inline void setCurrentXMLFile (XmlFile *file)
				{
					xmlCurrent = file;
				}

				inline XmlFile *getCurrentXMLFile ()
				{
					XmlFile *xmlFile = xmlCurrent;

					return (xmlFile);
				}

				inline void getCurrentXMLFile (XmlFile *file)
				{
					file = xmlCurrent;
				}

				static RXVECTOR2 getMonitorSize (int monitorNumber);

				void loadPlugins ();

				inline void setJavascriptEngine (JavascriptEngine *engine)
				{
					jsEngine = engine;
				}

				inline JavascriptEngine *getJavascriptEngine ()
				{
					return (jsEngine);
				}

				static void javascriptDebugger ();

			protected:
				RadXML ();
				RadXML (const RadXML &copy);
				~RadXML ();

				bool setupJavascript (JavascriptEngine *engine = 0);

				#ifdef defAPI_OGRE3D
					Ogre::SceneManager *mSceneMgr;
					Ogre::Camera *camMain;
					std::vector<Ogre::RenderWindow *> *aryRenderWindows;
				#endif

				#ifdef defAPI_CEGUI
					CEGUI::Window *cegRootWindow;
				#endif

				#ifdef defAPI_WXWIDGETS
					wxRadXmlWidgetsApp *wApp;
					bool bDestroyWxWidgetsApp;
				#endif

				XmlFile *xmlCurrent;
				std::string strCurrentScriptDir;

				#ifndef defAPI_WXWIDGETS
					#ifdef defSYS_WIN32
						std::vector<HINSTANCE> aryDLLInstances;
					#endif
				#else
					std::vector<wxDynamicLibrary *> aryDLLInstances;
				#endif

				std::vector<Timer *> aryTimers;
				HASHMAP<int, EventCallback> hshCallbacks;
				JavascriptEngine *jsEngine;
		};
	}
#endif

