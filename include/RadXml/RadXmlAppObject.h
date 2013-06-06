#ifndef _RADXML_APPOBJECT_H_
	#define _RADXML_APPOBJECT_H_

	#ifdef defAPI_OGRE3D
		#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
			#include <OGRE/OgreVector2.h>
		#else
			#include <OgreVector2.h>
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

	#include "RadXmlPreprocessor.h"
	#include "RadXmlEvents.h"
	#include "RadXmlNative.h"

	class XMLElement;
	class wxWindow;

	namespace RadXml
	{
		class App;
		class XmlFile;

		enum GUIWindowFunctionTypes
		{
			defGUIFUNC_TYPE_KEY_UP					= 0, 
			defGUIFUNC_TYPE_KEY_DOWN				= 1, 
			defGUIFUNC_TYPE_MOUSE_CLICK				= 2, 
			defGUIFUNC_TYPE_MOUSE_DOWN				= 3, 
			defGUIFUNC_TYPE_MOUSE_UP				= 4, 
			defGUIFUNC_TYPE_MOUSE_ENTERS			= 5, 
			defGUIFUNC_TYPE_MOUSE_LEAVES			= 6, 
			defGUIFUNC_TYPE_SLIDER					= 7, 
			defGUIFUNC_TYPE_COMBOBOX				= 8, 
			defGUIFUNC_TYPE_ACTIVATED				= 9, 
			defGUIFUNC_TYPE_DEACTIVATED				= 10, 
			defGUIFUNC_TYPE_INPUT_GAINED			= 11, 
			defGUIFUNC_TYPE_INPUT_LOST				= 12, 
			defGUIFUNC_TYPE_INPUT_ENABLED			= 13, 
			defGUIFUNC_TYPE_INPUT_DISABLED			= 14, 
			defGUIFUNC_TYPE_INPUT_CLOSED			= 15, 
			defGUIFUNC_TYPE_LISTBOX_SELECTED		= 16, 
			defGUIFUNC_TYPE_WINDOW_SHOW				= 17, 
			defGUIFUNC_TYPE_WINDOW_HIDE				= 18
		};

		class defRADXML_EXPORT AppObject:public Events
		{
			public:
				AppObject (std::string newName);
				virtual ~AppObject ();

				void setName (std::string newName);

				inline std::string getName ()
				{
					return (name);
				}

				inline unsigned long getHash ()
				{
					return (hash);
				}

				virtual void create (int creationType, AppObject *parent = 0) = 0;
				virtual void destroy () = 0;
				void destroyAppObject ();

				virtual AppObject *clone (std::string newName, int creationType, AppObject *parent = 0) = 0;

				void setVisible (bool visibility);
				inline bool getVisible ()
				{
					return (bVisible);
				}

				void setPosition (double x, double y);
				inline Ogre::Vector2 getPosition ()
				{
					return (position);
				}

				void setSize (double width, double height);
				inline Ogre::Vector2 getSize ()
				{
					return (size);
				}

				void parseAppObjectXML (App *level, XMLElement *element, AppObject *parent = 0);
				virtual void parseXML (App *level, XMLElement *element, AppObject *parent = 0) = 0;

				void setParent (AppObject *object);
				inline AppObject *getParent ()
				{
					return (aoParent);
				}

				void setText (std::string newText);
				std::string getText ();

				virtual void setNativeText (std::string newText);
				virtual std::string getNativeText ();

				#ifdef defAPI_CEGUI
					inline CEGUI::Window *getWindow ()
					{
						return (wWindow);
					}

					inline CEGUI::Window *getParentWindow ()
					{
						return (wParent);
					}
				#endif

				inline int getType ()
				{
					return (iType);
				}

				inline int getCreationType ()
				{
					return (iCreationType);
				}

				#ifdef defAPI_WXWIDGETS
					inline wxWindow *getNativeWindow ()
					{
						return (rwWindow);
					}
				#endif

				inline static bool isRelativeSize (Ogre::Vector2 size)
				{
					if ((size.x < 1) || (size.y < 1))
						return (true);

					return (false);
				}

				static Ogre::Vector2 getPixelSizeFromSize (Ogre::Vector2 size);
				static Ogre::Vector2 getPixelSizeFromSize (Ogre::Vector2 parentSize, Ogre::Vector2 size);
				static Ogre::Vector2 getPixelSizeFromRelativeSize (Ogre::Vector2 size);
				static Ogre::Vector2 getPixelSizeFromRelativeSize (Ogre::Vector2 parentSize, Ogre::Vector2 size);

			protected:
				inline void setType (int newType)
				{
					iType = newType;
				}

				inline void setCreationType (int newType)
				{
					iCreationType = newType;
				}

				void setWindowVisible ();
				void setWindowPosition ();
				void setWindowSize ();
				void setWindowText ();

				#ifdef defAPI_CEGUI
					void subscribeEvent (int functionType);
					AppObject *getCEGUIWindowFromKeyMouseHandleEvent (const CEGUI::EventArgs &evtEvent, std::string *nameReturn);
					bool CEGUIHandleInputEvent (const CEGUI::EventArgs &evtEvent);
					bool CEGUIHandleCloseEvent (const CEGUI::EventArgs &evtEvent);
				#endif

				std::string name;
				unsigned long hash;
				int iType;
				int iCreationType;
				AppObject *aoParent;

				bool bVisible;

				Ogre::Vector2 position;
				Ogre::Vector2 size;

				std::string text;

				#ifdef defAPI_CEGUI
					CEGUI::Window *wWindow;
					CEGUI::Window *wParent;
				#endif

				#ifdef defAPI_WXWIDGETS
					wxWindow *rwWindow;
				#endif
		};

		class AppObjectClass
		{
			public:
				static void setupAppObject ();

				static Variable *getName (Variable *thisObject, Variable *arguments);
				static Variable *setPosition (Variable *thisObject, Variable *arguments);
				static Variable *getPosition (Variable *thisObject, Variable *arguments);
				static Variable *setSize (Variable *thisObject, Variable *arguments);
				static Variable *getSize (Variable *thisObject, Variable *arguments);
				static Variable *clone (Variable *thisObject, Variable *arguments);
				static Variable *setVisible (Variable *thisObject, Variable *arguments);
				static Variable *getVisible (Variable *thisObject, Variable *arguments);
				static Variable *setText (Variable *thisObject, Variable *arguments);
				static Variable *getText (Variable *thisObject, Variable *arguments);
		};
	}
#endif

