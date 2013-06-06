#include "RadXmlApp.h"
#include "RadXmlAppObject.h"

#include "RadXml.h"
#include "RadXmlXmlFile.h"
#include "RadXmlGlobal.h"

#include "RadXmlWindow.h"
#include "RadXmlButton.h"
#include "RadXmlComboBox.h"
#include "RadXmlLabel.h"
#include "RadXmlMenu.h"
#include "RadXmlMenuItem.h"
#include "RadXmlTextbox.h"

#include "xml/xml.h"

#include "common/mai_radxml_system.h"
#include "common/com_radxml_strconv.h"

#include "languages/language.h"

namespace RadXml
{
	// App
	App::App (std::string newName)
	{
		setName (newName);
	}

	App::~App ()
	{
		destroyAllAppObjects ();
	}

	void App::destroyAllAppObjects ()
	{
		HASHMAP<unsigned long, AppObject *>::iterator itBegin = hshAppObjects.begin ();
		HASHMAP<unsigned long, AppObject *>::iterator itEnd = hshAppObjects.end ();
		//std::vector<AppObject *> aryDeleteLast;

		while (itBegin != itEnd)
		{
			AppObject *objObject = (*itBegin).second;

			if (objObject != 0)
			{
				/*if ((objObject->getType () == AOT_WINDOW) || (objObject->getType () == AOT_MENU))
					aryDeleteLast.push_back (objObject);
				else*/
				{
					delete objObject;
					objObject = 0;
				}
			}

			itBegin++;
		}

		/*for (unsigned int iIdx = 0; iIdx < aryDeleteLast.size (); iIdx++)
		{
			AppObject *objObject = aryDeleteLast.at (iIdx);

			if (objObject != 0)
			{
				delete objObject;
				objObject = 0;
			}
		}*/
	}

	void App::setName (std::string newName)
	{
		appName = newName;
		appHash = RadXmlCommon::s13StringToHash (newName);
	}

	void App::parseApp (XmlFile *file, int type, XMLElement *element)
	{
		parseXMLAppObjects (type, element);
	}

	void App::parseXMLAppObjects (int creationType, XMLElement *element, AppObject *parent)
	{
		for (int iIdx = 0; iIdx < 7; iIdx++)
		{
			std::string strType = "";
			int iType = (iIdx + 1);

			switch (iIdx)
			{
				case 0:
					strType = "window";
					break;
				case 1:
					strType = "menu";
					break;
				case 2:
					strType = "menuitem";
					break;
				case 3:
					strType = "label";
					break;
				case 4:
					strType = "textbox";
					break;
				case 5:
					strType = "combobox";
					break;
				case 6:
					strType = "button";
					break;
			}

			std::vector<XMLElement *> aryTags = element->findAllTags (strType, 0, false);

			for (unsigned int iJdx = 0; iJdx < aryTags.size (); iJdx++)
			{
				try
				{
					parseXMLAppObject (creationType, iType, aryTags.at (iJdx), parent);
				}
				#ifdef defAPI_CEGUI
					catch (CEGUI::Exception &ex)
					{
						RadXmlCommon::s13LogMessage ((std::string)"CEGUI Error: " + (std::string)ex.getMessage ().c_str ());
					}
				#else
					catch (std::exception &ex)
					{
						RadXmlCommon::s13LogMessage ((std::string)"Error: " + ex.what ());
					}
				#endif
			}
		}
	}

	void App::parseXMLAppObject (int creationType, int type, XMLElement *element, AppObject *parent)
	{
		AppObject *objObject = createAppObject (this, creationType, type, "", parent, element);

		if (element->content != "")
			parseXMLAppObjects (creationType, element, objObject);
	}

	AppObject *App::createAppObject (App *file, int creationType, int type, std::string name, AppObject *parent, XMLElement *element)
	{
		AppObject *objObject = 0;

		if (type == AOT_WINDOW)
		{
			Window *wWindow = new Window (name);

			if (element != 0)
				wWindow->parseXML (file, element, parent);

			wWindow->create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AOT_MENU)
		{
			Menu *wWindow = new Menu (name);

			if (element != 0)
				wWindow->parseXML (file, element, parent);

			wWindow->create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AOT_MENU_ITEM)
		{
			MenuItem *wWindow = new MenuItem (name);

			if (element != 0)
				wWindow->parseXML (file, element, parent);

			wWindow->create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AOT_LABEL)
		{
			Label *wWindow = new Label (name);

			if (element != 0)
				wWindow->parseXML (file, element, parent);

			wWindow->create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AOT_TEXT_BOX)
		{
			TextBox *wWindow = new TextBox (name);

			if (element != 0)
				wWindow->parseXML (file, element, parent);

			wWindow->create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AOT_COMBO_BOX)
		{
			Combobox *wWindow = new Combobox (name);

			if (element != 0)
				wWindow->parseXML (file, element, parent);

			wWindow->create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AOT_BUTTON)
		{
			Button *wWindow = new Button (name);

			if (element != 0)
				wWindow->parseXML (file, element, parent);

			wWindow->create (creationType, parent);
			objObject = wWindow;
		}

		addAppObject (objObject);

		return (objObject);
	}

	void App::addAppObject (AppObject *newAppObject)
	{
		hshAppObjects.insert (std::pair<unsigned long, AppObject *> (newAppObject->getHash (), newAppObject));
		RadXML::getSingletonPtr ()->getJavascriptEngine ()->addAppObjectJSObject (newAppObject);
	}

	AppObject *App::getAppObject (std::string findName, bool skipErrorMessage)
	{
		HASHMAP<unsigned long, AppObject *>::iterator itBegin = hshAppObjects.find (RadXmlCommon::s13StringToHash (findName));
		HASHMAP<unsigned long, AppObject *>::iterator itEnd = hshAppObjects.end ();

		if (itBegin != itEnd)
			return ((*itBegin).second);
		else
		{
			if (skipErrorMessage == false)
				RadXmlCommon::s13LogMessage (MSG12 + findName);
		}

		return (0);
	}

	AppObject *App::getAppObject (unsigned long hash, bool skipErrorMessage)
	{
		HASHMAP<unsigned long, AppObject *>::iterator itBegin = hshAppObjects.find (hash);
		HASHMAP<unsigned long, AppObject *>::iterator itEnd = hshAppObjects.end ();

		if (itBegin != itEnd)
			return ((*itBegin).second);
		else
		{
			if (skipErrorMessage == false)
				RadXmlCommon::s13LogMessage (MSG13);
		}

		return (0);
	}

	void App::removeAppObject (std::string findName)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		HASHMAP<unsigned long, AppObject *>::iterator itBegin = hshAppObjects.find (RadXmlCommon::s13StringToHash (findName));
		HASHMAP<unsigned long, AppObject *>::iterator itEnd = hshAppObjects.end ();

		if (itBegin != itEnd)
		{
			ogrGame->getJavascriptEngine ()->removeAppObjectJSObject ((*itBegin).second);
			hshAppObjects.erase (itBegin);
		}
	}

	void App::removeAppObject (unsigned long hash)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		HASHMAP<unsigned long, AppObject *>::iterator itBegin = hshAppObjects.find (hash);
		HASHMAP<unsigned long, AppObject *>::iterator itEnd = hshAppObjects.end ();

		if (itBegin != itEnd)
		{
			ogrGame->getJavascriptEngine ()->removeAppObjectJSObject ((*itBegin).second);
			hshAppObjects.erase (itBegin);
		}
	}

	void App::addAppObjectForDeletion (AppObject *object)
	{
		for (unsigned int iIdx = 0; iIdx < aryAppObjectsToDelete.size (); iIdx++)
		{
			AppObject *mObject = aryAppObjectsToDelete.at (iIdx);

			if (mObject != 0)
			{
				if (mObject == object)
					return;
			}
		}

		aryAppObjectsToDelete.push_back (object);
	}

	void App::processDeleteAppObjects ()
	{
		for (unsigned int iIdx = 0; iIdx < aryAppObjectsToDelete.size (); iIdx++)
		{
			AppObject *mObject = aryAppObjectsToDelete.at (iIdx);

			if (mObject != 0)
			{
				removeAppObject (mObject->getName ());

				delete mObject;
				mObject = 0;
			}
		}

		aryAppObjectsToDelete.clear ();
	}

	// AppClass
	void AppClass::setupAppJS ()
	{
		ClassObject *cClass = new ClassObject ("App", "App");
		cClass->set ("getAppObjectByName", new Function ("getAppObjectByName", 0, AppClass::getAppObjectByName));
		cClass->set ("deleteObject", new Function ("deleteObject", 0, AppClass::deleteObject));
		cClass->set ("createObject", new Function ("createObject", 0, AppClass::createObject));
		cClass->set ("AppObjects", new Array ("AppObjects"));

		JavascriptManager::getSingletonPtr ()->set ("App", cClass);
	}

	Variable *AppClass::getAppObjectByName (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strString = aryArguments->getElement (0)->toString ()->getValue ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		Class *objAppObject = ogrGame->getJavascriptEngine ()->getAppObjectJSClass (strString);

		return (objAppObject);
	}

	Variable *AppClass::createObject (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		double dCreationType = aryArguments->getElement (0)->toNumber ()->getValue ();
		double dNumber = aryArguments->getElement (1)->toNumber ()->getValue ();
		std::string strString = aryArguments->getElement (2)->toString ()->getValue ();
		AppObject *parent = 0;

		if (aryArguments->length () > 3)
			parent = (AppObject *)aryArguments->getElement (3)->toClass ()->getHiddenData ("appObject");

		ogrGame->getCurrentXMLFile ()->createAppObject (0, (int)dCreationType, (int)dNumber, strString, parent);

		return (0);
	}

	Variable *AppClass::deleteObject (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		std::string strString = aryArguments->getElement (0)->toString ()->getValue ();
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		XmlFile *lvlCurrent = ogrGame->getCurrentXMLFile ();
		AppObject *mObject = lvlCurrent->getAppObject (strString, true);
		lvlCurrent->addAppObjectForDeletion (mObject);

		return (0);
	}
}

