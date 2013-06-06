#include "RadXmlComboBox.h"
#include "RadXmlAppObject.h"

#include "RadXmlApp.h"
#include "RadXmlXmlFile.h"
#include "RadXml.h"
#include "RadXmlGlobal.h"

#include "xml/xml.h"

#ifdef defAPI_WXWIDGETS
	#include "common/mai_radxml_system.h"

	IMPLEMENT_CLASS (RXComboBox, wxComboBox)

	BEGIN_EVENT_TABLE (RXComboBox, wxComboBox)
	END_EVENT_TABLE ()
#endif

namespace RadXml
{
	// Combobox
	Combobox::Combobox (std::string newName)
		: AppObject (newName)
	{
		setType (AOT_COMBO_BOX);
		addEvent (new Event ("onclick", ""));
		size.x = 0.2;
		size.y = 0.2;
	}

	Combobox::~Combobox ()
	{
		destroy ();
	}

	void Combobox::create (int creationType, AppObject *parent)
	{
		if (creationType == AT_RENDER)
		{
			#ifdef defAPI_CEGUI
				wWindow = CEGUI::WindowManager::getSingletonPtr ()->createWindow ("TaharezLook/Combobox", name);
				wWindow->setMinSize (CEGUI::UVector2 (CEGUI::UDim (size.x, 0), CEGUI::UDim (size.y, 0)));
				wWindow->setMaxSize (CEGUI::UVector2 (CEGUI::UDim (size.x, 0), CEGUI::UDim (size.y, 0)));
				wWindow->setUserData (this);
				subscribeEvent (defGUIFUNC_TYPE_MOUSE_CLICK);
			#endif
		}

		if (creationType == AT_NATIVE)
		{
			#ifdef defAPI_WXWIDGETS
				Ogre::Vector2 v2dPos = getPixelSizeFromSize (position);
				Ogre::Vector2 v2dSize = getPixelSizeFromSize (size);
				RXWindow *wwParent = 0;

				if (parent != 0)
				{
					wwParent = (RXWindow *)parent->getNativeWindow ();
					Ogre::Vector2 v2dParentSize (wwParent->GetSize ().x, wwParent->GetSize ().y);

					v2dPos = getPixelSizeFromSize (v2dParentSize, position);
					v2dSize = getPixelSizeFromSize (v2dParentSize, size);
				}

				rwWindow = new RXComboBox ((wxWindow *)wwParent, -1, wxT (text.c_str ()), wxPoint (v2dPos.x, v2dPos.y), wxSize (v2dSize.x, v2dSize.y));
				rwWindow->Show (bVisible);
				rwWindow->SetClientData (this);
			#endif
		}

		setCreationType (creationType);

		setParent (parent);
		setVisible (bVisible);
		setPosition (position.x, position.y);
		setSize (size.x, size.y);
		setText (text);
	}

	void Combobox::destroy ()
	{
	}

	void Combobox::parseXML (App *level, XMLElement *element, AppObject *parent)
	{
		parseAppObjectXML (level, element, parent);

		std::string onclick = element->getAttributeValue ("onclick");
		setJavascriptToEvent ("onclick", onclick);
	}

	AppObject *Combobox::clone (std::string newName, int creationType, AppObject *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		Combobox *clone = new Combobox (*this);

		clone->setName (newName);
		clone->create (creationType, parent);
		ogrGame->getCurrentXMLFile ()->addAppObject (clone);

		return (clone);
	}

	void Combobox::addItem (std::string text)
	{
		if (iCreationType == AT_RENDER)
		{
			#ifdef defAPI_CEGUI
				CEGUI::Combobox *lbBox = static_cast<CEGUI::Combobox *> (wWindow);
				unsigned int iCount = lbBox->getItemCount ();
				lbBox->addItem (new CEGUI::ListboxTextItem (text, iCount));
			#endif
		}

		if (iCreationType == AT_NATIVE)
		{
			#ifdef defAPI_WXWIDGETS
				((wxComboBox *)rwWindow)->Append (wxT(text.c_str ()));
			#endif
		}
	}

	void Combobox::setItem (unsigned int index, std::string text)
	{
		if (iCreationType == AT_RENDER)
		{
			#ifdef defAPI_CEGUI
				CEGUI::ListboxItem *ltiItem = (static_cast<CEGUI::Combobox *> (wWindow))->getListboxItemFromIndex (index);
				ltiItem->setText (text);
			#endif
		}

		if (iCreationType == AT_NATIVE)
		{
			#ifdef defAPI_WXWIDGETS
				((wxComboBox *)rwWindow)->SetString (index, wxT(text.c_str ()));
			#endif
		}
	}

	std::string Combobox::getItem (unsigned int index)
	{
		std::string strReturn = "";

		if (iCreationType == AT_RENDER)
		{
			#ifdef defAPI_CEGUI
				CEGUI::ListboxItem *ltiItem = (static_cast<CEGUI::Combobox *> (wWindow))->getListboxItemFromIndex (index);
				strReturn = ltiItem->getText ().c_str ();
			#endif
		}

		if (iCreationType == AT_NATIVE)
		{
			#ifdef defAPI_WXWIDGETS
				strReturn = RadXmlCommon::convertwxStringToString (((wxComboBox *)rwWindow)->GetString (index));
			#endif
		}

		return (strReturn);
	}

	void Combobox::removeItem (unsigned int index)
	{
		if (iCreationType == AT_RENDER)
		{
			#ifdef defAPI_CEGUI
				CEGUI::ListboxItem *ltiItem = (static_cast<CEGUI::Combobox *> (wWindow))->getListboxItemFromIndex (index);
				static_cast<CEGUI::Listbox *> (wWindow)->removeItem (ltiItem);
			#endif
		}

		if (iCreationType == AT_NATIVE)
		{
			#ifdef defAPI_WXWIDGETS
				((wxComboBox *)rwWindow)->Delete (index);
			#endif
		}
	}

	unsigned int Combobox::getNumItems ()
	{
		unsigned int iReturn = 0;

		if (iCreationType == AT_RENDER)
		{
			#ifdef defAPI_CEGUI
				iReturn = (static_cast<CEGUI::Combobox *> (wWindow))->getItemCount ();
			#endif
		}

		if (iCreationType == AT_NATIVE)
		{
			#ifdef defAPI_WXWIDGETS
				iReturn = ((wxComboBox *)rwWindow)->GetCount ();
			#endif
		}

		return (iReturn);
	}

	void Combobox::setNativeText (std::string newText)
	{
		#ifdef defAPI_WXWIDGETS
			if (rwWindow != 0)
				((wxComboBox *)rwWindow)->SetValue (wxT (newText.c_str ()));
		#endif
	}

	std::string Combobox::getNativeText ()
	{
		#ifdef defAPI_WXWIDGETS
			return (RadXmlCommon::convertwxStringToString (((wxComboBox *)rwWindow)->GetValue ()));
		#endif
	}

	// ComboBoxClass
	void ComboBoxClass::setupComboBox ()
	{
		Class *cClass = new Class ("ComboBox", "ComboBox");
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

		cClass->set ("addItem", new Function ("addItem", 0, ComboBoxClass::addItem));
		cClass->set ("setItem", new Function ("setItem", 0, ComboBoxClass::setItem));
		cClass->set ("getItem", new Function ("getItem", 0, ComboBoxClass::getItem));
		cClass->set ("removeItem", new Function ("removeItem", 0, ComboBoxClass::removeItem));
		cClass->set ("getNumItems", new Function ("getNumItems", 0, ComboBoxClass::getNumItems));

		JavascriptManager::getSingletonPtr ()->set ("ComboBox", cClass);
	}

	Variable *ComboBoxClass::addItem (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));
		Combobox *lbBox = static_cast<Combobox *> (mObject);

		std::string strText = aryArguments->getElement (0)->toString ()->getValue ();

		lbBox->addItem (strText);

		return (0);
	}

	Variable *ComboBoxClass::setItem (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));
		Combobox *lbBox = static_cast<Combobox *> (mObject);
		double dNumber = aryArguments->getElement (0)->toNumber ()->getValue ();
		std::string strText = aryArguments->getElement (1)->toString ()->getValue ();

		lbBox->setItem (dNumber, strText);

		return (0);
	}

	Variable *ComboBoxClass::getItem (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));
		Combobox *lbBox = static_cast<Combobox *> (mObject);
		double dNumber = aryArguments->getElement (0)->toNumber ()->getValue ();
		std::string strText = lbBox->getItem (dNumber);

		return (String::New (strText));
	}

	Variable *ComboBoxClass::removeItem (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));
		Combobox *lbBox = static_cast<Combobox *> (mObject);
		double dNumber = aryArguments->getElement (0)->toNumber ()->getValue ();

		lbBox->removeItem (dNumber);

		return (0);
	}

	Variable *ComboBoxClass::getNumItems (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		AppObject *mObject = static_cast<AppObject *> (objThis->getHiddenData ("appObject"));
		Combobox *lbBox = static_cast<Combobox *> (mObject);

		return (Number::New ("", lbBox->getNumItems ()));
	}
}

