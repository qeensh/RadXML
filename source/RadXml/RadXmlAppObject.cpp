#include "RadXmlAppObject.h"

#include "RadXmlApp.h"
#include "RadXml.h"
#include "RadXmlXmlFile.h"
#include "RadXmlGlobal.h"

#include "xml/xml.h"

#include "common/com_radxml_strconv.h"

namespace RadXml
{
	// AppObject
	AppObject::AppObject (std::string newName)
	{
		setName (newName);
		bVisible = true;
		position = RXVECTOR2::ZERO;
		size = RXVECTOR2::ZERO;
		text = "";
		aoParent = 0;
		#ifdef defAPI_CEGUI
			wWindow = 0;
			wParent = 0;
		#endif
		#ifdef defAPI_WXWIDGETS
			rwWindow = 0;
		#endif
		iType = 0;
		iCreationType = 0;
	}

	AppObject::~AppObject ()
	{
		destroyAppObject ();
	}

	void AppObject::destroyAppObject ()
	{
		#ifdef defAPI_CEGUI
			if (wParent != 0)
				wParent->removeChildWindow (wWindow);

			if (wWindow != 0)
				CEGUI::WindowManager::getSingletonPtr ()->destroyWindow (wWindow);

			wWindow = 0;
		#endif
		#ifdef defAPI_WXWIDGETS
			/// @bug I'm thinking this should be deleting the windows and menus too...
			if ((iType != AOT_WINDOW) && (iType != AOT_MENU))
			{
				if (rwWindow != 0)
				{
					delete rwWindow;
					rwWindow = 0;
				}
			}
		#endif
	}

	void AppObject::setName (std::string newName)
	{
		name = newName;
		hash = RadXmlCommon::s13StringToHash (newName);
	}

	void AppObject::parseAppObjectXML (App *level, XMLElement *element, AppObject *parent)
	{
		std::string strName = element->getAttributeValue ("name");
		std::string strVisible = element->getAttributeValue ("visible");
		std::string strPosition = element->getAttributeValue ("position");
		std::string strX = element->getAttributeValue ("x");
		std::string strY = element->getAttributeValue ("y");
		std::string strSize = element->getAttributeValue ("size");
		std::string strWidth = element->getAttributeValue ("width");
		std::string strHeight = element->getAttributeValue ("height");
		std::string strText = element->getAttributeValue ("text");

		if (strName == "")
		{
			int iRandName = rand ();
			AppObject *objFound = level->getAppObject (iRandName, true);

			while (objFound != 0)
			{
				iRandName = rand ();
				objFound = level->getAppObject (iRandName, true);
			}

			strName = "obj" + RadXmlCommon::s13IntToString (iRandName);
		}

		if (strVisible != "")
			bVisible = RadXmlCommon::s13StringToBool (strVisible);

		if (strPosition != "")
		{
			strPosition = RadXmlCommon::s13RemoveWhitespaces (strPosition);
			std::vector<std::string> aryString = RadXmlCommon::s13StringSplit (strPosition, ",");
			position.x = RadXmlCommon::s13StringToDouble (aryString.at (0));
			position.y = RadXmlCommon::s13StringToDouble (aryString.at (1));
		}

		if (strX != "")
			position.x = RadXmlCommon::s13StringToDouble (strX);

		if (strY != "")
			position.y = RadXmlCommon::s13StringToDouble (strY);

		if (strSize != "")
		{
			strSize = RadXmlCommon::s13RemoveWhitespaces (strSize);
			std::vector<std::string> aryString = RadXmlCommon::s13StringSplit (strSize, ",");
			size.x = RadXmlCommon::s13StringToDouble (aryString.at (0));
			size.y = RadXmlCommon::s13StringToDouble (aryString.at (1));
		}

		if (strWidth != "")
			size.x = RadXmlCommon::s13StringToDouble (strWidth);

		if (strHeight != "")
			size.y = RadXmlCommon::s13StringToDouble (strHeight);

		setName (strName);
		setVisible (bVisible);
		setPosition (position.x, position.y);
		setSize (size.x, size.y);
		setText (strText);
	}

	void AppObject::setParent (AppObject *object)
	{
		if (iCreationType == AT_RENDER)
		{
			#ifdef defAPI_CEGUI
				RadXML *ogrGame = RadXML::getSingletonPtr ();

				if (object != 0)
					wParent = object->getWindow ();
				else
					wParent = ogrGame->getCEGUIRootWindow ();
			#endif
		}

		aoParent = object;
	}

	Ogre::Vector2 AppObject::getPixelSizeFromSize (Ogre::Vector2 size)
	{
		Ogre::Vector2 v2dReturn = Ogre::Vector2::ZERO;

		if (isRelativeSize (size) == true)
			v2dReturn = getPixelSizeFromRelativeSize (size);
		else
			v2dReturn = size;

		return (v2dReturn);
	}

	Ogre::Vector2 AppObject::getPixelSizeFromSize (Ogre::Vector2 parentSize, Ogre::Vector2 size)
	{
		Ogre::Vector2 v2dReturn = Ogre::Vector2::ZERO;

		if (isRelativeSize (size) == true)
			v2dReturn = getPixelSizeFromRelativeSize (parentSize, size);
		else
			v2dReturn = size;

		return (v2dReturn);
	}

	Ogre::Vector2 AppObject::getPixelSizeFromRelativeSize (Ogre::Vector2 size)
	{
		Ogre::Vector2 v2dReturn = Ogre::Vector2::ZERO;
		Ogre::Vector2 v2dMonitorSize = RadXml::RadXML::getMonitorSize (0);

		v2dReturn.x = v2dMonitorSize.x * size.x;
		v2dReturn.y = v2dMonitorSize.y * size.y;

		return (v2dReturn);
	}

	Ogre::Vector2 AppObject::getPixelSizeFromRelativeSize (Ogre::Vector2 parentSize, Ogre::Vector2 size)
	{
		Ogre::Vector2 v2dReturn = Ogre::Vector2::ZERO;

		v2dReturn.x = parentSize.x * size.x;
		v2dReturn.y = parentSize.y * size.y;

		return (v2dReturn);
	}

	void AppObject::setWindowVisible ()
	{
		#ifdef defAPI_CEGUI
			if (wWindow != 0)
			{
				if (bVisible == true)
				{
					wWindow->show ();
					wParent->addChildWindow (wWindow);
					wWindow->activate ();
				}
				else
				{
					wWindow->hide ();
					wParent->removeChildWindow (wWindow);
				}
			}
		#endif

		#ifdef defAPI_WXWIDGETS
			if (rwWindow != 0)
			{
				if (bVisible == true)
					rwWindow->Show ();
				else
					rwWindow->Hide ();
			}
		#endif
	}

	void AppObject::setWindowPosition ()
	{
		#ifdef defAPI_CEGUI
			if (wWindow != 0)
			{
				wWindow->setXPosition (CEGUI::UDim (position.x, 0));
				wWindow->setYPosition (CEGUI::UDim (position.y, 0));
			}
		#endif

		#ifdef defAPI_WXWIDGETS
			if (rwWindow != 0)
			{
				Ogre::Vector2 v2dPos = Ogre::Vector2::ZERO;

				if (aoParent != 0)
				{
					wxWindow *rwParent = aoParent->getNativeWindow ();
					Ogre::Vector2 v2dParentSize (rwParent->GetSize ().x, rwParent->GetSize ().y);
					v2dPos = getPixelSizeFromSize (v2dParentSize, position);
				}
				else
					v2dPos = getPixelSizeFromSize (position);

				wxPoint wxpPosition (v2dPos.x, v2dPos.y);
				rwWindow->SetPosition (wxpPosition);
			}
		#endif
	}

	void AppObject::setWindowSize ()
	{
		#ifdef defAPI_CEGUI
			if (wWindow != 0)
			{
				wWindow->setWidth (CEGUI::UDim (size.x, 0));
				wWindow->setHeight (CEGUI::UDim (size.y, 0));
			}
		#endif

		#ifdef defAPI_WXWIDGETS
			if (rwWindow != 0)
			{
				Ogre::Vector2 v2dSize = Ogre::Vector2::ZERO;

				if (aoParent != 0)
				{
					wxWindow *rwParent = aoParent->getNativeWindow ();
					Ogre::Vector2 v2dParentSize (rwParent->GetSize ().x, rwParent->GetSize ().y);
					v2dSize = getPixelSizeFromSize (v2dParentSize, size);
				}
				else
					v2dSize = getPixelSizeFromSize (size);

				rwWindow->SetSize (v2dSize.x, v2dSize.y);
			}
		#endif
	}

	void AppObject::setWindowText ()
	{
		#ifdef defAPI_CEGUI
			if (wWindow != 0)
				wWindow->setText (text);
		#endif

		#ifdef defAPI_WXWIDGETS
			if (rwWindow != 0)
				setNativeText (text);
		#endif
	}

	void AppObject::setVisible (bool visibility)
	{
		bVisible = visibility;

		setWindowVisible ();
	}

	void AppObject::setPosition (double x, double y)
	{
		position.x = x;
		position.y = y;

		setWindowPosition ();
	}

	void AppObject::setSize (double width, double height)
	{
		size.x = width;
		size.y = height;

		setWindowSize ();
	}

	void AppObject::setText (std::string newText)
	{
		text = newText;

		setWindowText ();
	}

	void AppObject::setNativeText (std::string newText)
	{
	}

	std::string AppObject::getText ()
	{
		#ifdef defAPI_CEGUI
			if (wWindow != 0)
				text = wWindow->getText ().c_str ();
		#endif

		#ifdef defAPI_WXWIDGETS
			if (rwWindow != 0)
				return (getNativeText ());
		#endif

		return (text);
	}

	std::string AppObject::getNativeText ()
	{
		return ("");
	}

	#ifdef defAPI_CEGUI
	void AppObject::subscribeEvent (int functionType)
	{
		RadXML *game = RadXML::getSingletonPtr ();
		CEGUI::String strType = "";
		CEGUI::Event::Subscriber *sSubscriber = 0;

		switch (functionType)
		{
			case defGUIFUNC_TYPE_KEY_UP:
				strType = CEGUI::Window::EventKeyUp;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_KEY_DOWN:
				strType = CEGUI::Window::EventKeyDown;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_MOUSE_CLICK:
				strType = CEGUI::Window::EventMouseClick;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_MOUSE_DOWN:
				strType = CEGUI::Window::EventMouseButtonDown;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_MOUSE_UP:
				strType = CEGUI::Window::EventMouseButtonUp;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_MOUSE_ENTERS:
				strType = CEGUI::Window::EventMouseEnters;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_MOUSE_LEAVES:
				strType = CEGUI::Window::EventMouseLeaves;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_SLIDER:
				strType = CEGUI::Scrollbar::EventScrollPositionChanged;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_COMBOBOX:
				strType = CEGUI::Combobox::EventListSelectionAccepted;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_ACTIVATED:
				strType = CEGUI::Window::EventActivated;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_DEACTIVATED:
				strType = CEGUI::Window::EventDeactivated;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_INPUT_GAINED:
				strType = CEGUI::Window::EventInputCaptureGained;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_INPUT_LOST:
				strType = CEGUI::Window::EventInputCaptureLost;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_INPUT_ENABLED:
				strType = CEGUI::Window::EventEnabled;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_INPUT_DISABLED:
				strType = CEGUI::Window::EventDisabled;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_INPUT_CLOSED:
				strType = CEGUI::FrameWindow::EventCloseClicked;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleCloseEvent, this);
				break;
			case defGUIFUNC_TYPE_LISTBOX_SELECTED:
				strType = CEGUI::Listbox::EventSelectionChanged;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_WINDOW_SHOW:
				strType = CEGUI::Window::EventShown;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
			case defGUIFUNC_TYPE_WINDOW_HIDE:
				strType = CEGUI::Window::EventHidden;
				sSubscriber = new CEGUI::Event::Subscriber (&AppObject::CEGUIHandleInputEvent, this);
				break;
		}

		wWindow->subscribeEvent (strType, sSubscriber);

		if (functionType == defGUIFUNC_TYPE_INPUT_CLOSED)
			static_cast<CEGUI::FrameWindow *> (wWindow)->setCloseButtonEnabled (true);
	}

	AppObject *AppObject::getCEGUIWindowFromKeyMouseHandleEvent (const CEGUI::EventArgs &evtEvent, std::string *nameReturn)
	{
		CEGUI::Window *wWin = ((CEGUI::WindowEventArgs &)evtEvent).window;
		AppObject *cegCEGUI = static_cast <AppObject *> (wWin->getUserData ());
		(*nameReturn) = wWin->getName ().c_str ();

		if (cegCEGUI == 0)
		{
			while (wWin->getParent () != 0)
			{
				wWin = wWin->getParent ();
				cegCEGUI = static_cast <AppObject *> (wWin->getUserData ());

				if (cegCEGUI != 0)
					break;
			}
		}

		return (cegCEGUI);
	}

	bool AppObject::CEGUIHandleInputEvent (const CEGUI::EventArgs &evtEvent)
	{
		std::string strName = "";
		AppObject *cegCEGUI = getCEGUIWindowFromKeyMouseHandleEvent (evtEvent, &strName);

		if (cegCEGUI != 0)
		{
			if (typeid (evtEvent) == typeid (const CEGUI::KeyEventArgs &))
			{
				const CEGUI::KeyEventArgs &keaArgs = (const CEGUI::KeyEventArgs &)evtEvent;
			}

			if (typeid (evtEvent) == typeid (const CEGUI::MouseEventArgs &))
			{
				const CEGUI::MouseEventArgs &meaArgs = (const CEGUI::MouseEventArgs &)evtEvent;

				executeJavascriptFromEvent ("onclick");
			}
		}

		return (true);
	}

	bool AppObject::CEGUIHandleCloseEvent (const CEGUI::EventArgs &evtEvent)
	{
		std::string strName = "";
		AppObject *cegCEGUI = getCEGUIWindowFromKeyMouseHandleEvent (evtEvent, &strName);

		if (cegCEGUI != 0)
			executeJavascriptFromEvent ("onclosed");

		return (true);
	}
	#endif

	// AppObjectClass
	void AppObjectClass::setupAppObject ()
	{
		Class *cClass = new Class ("AppObject", "AppObject");
		cClass->set ("getName", new Function ("getName", 0, AppObjectClass::getName));
		cClass->set ("setPosition", new Function ("setPosition", 0, AppObjectClass::setPosition));
		cClass->set ("getPosition", new Function ("getPosition", 0, AppObjectClass::getPosition));
		cClass->set ("setSize", new Function ("setSize", 0, AppObjectClass::setSize));
		cClass->set ("getSize", new Function ("getSize", 0, AppObjectClass::getSize));
		cClass->set ("clone", new Function ("clone", 0, AppObjectClass::clone));
		cClass->set ("setVisible", new Function ("setVisible", 0, AppObjectClass::setVisible));
		cClass->set ("getVisible", new Function ("getVisible", 0, AppObjectClass::getVisible));
		cClass->set ("setText", new Function ("setText", 0, AppObjectClass::setText));
		cClass->set ("getText", new Function ("getText", 0, AppObjectClass::getText));

		JavascriptManager::getSingletonPtr ()->set ("AppObject", cClass);
	}

	Variable *AppObjectClass::getName (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));
		std::string strName = mObject->getName ();

		return (String::New (strName));
	}

	Variable *AppObjectClass::setPosition (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));
		double dX = 0;
		double dY = 0;

		if (aryArguments->length () == 1)
		{
			Class *cClass = aryArguments->getElement (0)->toClass ();

			dX = cClass->get ("x")->toNumber ()->getValue ();
			dY = cClass->get ("y")->toNumber ()->getValue ();
		}
		else
		{
			dX = aryArguments->getElement (0)->toNumber ()->getValue ();
			dY = aryArguments->getElement (1)->toNumber ()->getValue ();
		}

		mObject->setPosition (dX, dY);

		return (0);
	}

	Variable *AppObjectClass::getPosition (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));
		RXVECTOR2 v2dPosition = mObject->getPosition ();
		Class *objClone = ogrGame->getJavascriptEngine ()->getVector2JSClass (v2dPosition.x, v2dPosition.y);

		return (objClone);
	}

	Variable *AppObjectClass::setSize (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));
		double dX = 0;
		double dY = 0;

		if (aryArguments->length () == 1)
		{
			Class *cClass = aryArguments->getElement (0)->toClass ();

			dX = cClass->get ("x")->toNumber ()->getValue ();
			dY = cClass->get ("y")->toNumber ()->getValue ();
		}
		else
		{
			dX = aryArguments->getElement (0)->toNumber ()->getValue ();
			dY = aryArguments->getElement (1)->toNumber ()->getValue ();
		}

		mObject->setSize (dX, dY);

		return (0);
	}

	Variable *AppObjectClass::getSize (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));
		RXVECTOR2 v2dSize = mObject->getSize ();
		Class *objClone = ogrGame->getJavascriptEngine ()->getVector2JSClass (v2dSize.x, v2dSize.y);

		return (objClone);
	}

	Variable *AppObjectClass::clone (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));
		AppObject *mParent = 0;
		std::string strName = aryArguments->getElement (0)->toString ()->getValue ();
		double dCreationType = aryArguments->getElement (1)->toNumber ()->getValue ();
		std::string strParent = "";

		if (aryArguments->length () > 1)
			strParent = aryArguments->getElement (2)->toString ()->getValue ();

		if (strParent != "")
		{
			XmlFile *lvlCurrent = ogrGame->getCurrentXMLFile ();
			mParent = lvlCurrent->getAppObject (strParent, true);
		}

		AppObject *objClone = mObject->clone (strName, (int)dCreationType, mParent);
		Class *objReturn = ogrGame->getJavascriptEngine ()->getAppObjectJSClass (objClone);

		return (objReturn);
	}

	Variable *AppObjectClass::setVisible (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));
		bool bVisible = aryArguments->getElement (0)->toBoolean ()->getValue ();

		mObject->setVisible (bVisible);

		return (0);
	}

	Variable *AppObjectClass::getVisible (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));

		return (Boolean::New (mObject->getVisible ()));
	}

	Variable *AppObjectClass::setText (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));
		std::string strText = aryArguments->getElement (0)->toString ()->getValue ();

		mObject->setText (strText);

		return (0);
	}

	Variable *AppObjectClass::getText (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));
		std::string strText = mObject->getText ();

		return (String::New (strText));
	}
}

