#include "RadXmlTextbox.h"

#include "RadXmlApp.h"
#include "RadXml.h"
#include "RadXmlXmlFile.h"

#include "xml/xml.h"

#ifdef defAPI_WXWIDGETS
	#include "common/mai_radxml_system.h"

	IMPLEMENT_CLASS (RXTextBox, wxTextCtrl)

	BEGIN_EVENT_TABLE (RXTextBox, wxTextCtrl)
		EVT_TEXT (-1, RXTextBox::OnText)
		EVT_TEXT_ENTER (-1, RXTextBox::OnTextEnter)
	END_EVENT_TABLE ()
#endif

namespace RadXml
{
	// TextBox
	TextBox::TextBox (std::string newName)
		: AppObject (newName)
	{
		setType (AOT_TEXT_BOX);
		addEvent (new Event ("onclick", ""));
		addEvent (new Event ("ontextchanged", ""));
		addEvent (new Event ("onenter", ""));
		size.x = 0.2;
		size.y = 0.06;
	}

	TextBox::~TextBox ()
	{
		destroy ();
	}

	void TextBox::create (int creationType, AppObject *parent)
	{
		if (creationType == AT_RENDER)
		{
			#ifdef defAPI_CEGUI
				wWindow = CEGUI::WindowManager::getSingletonPtr ()->createWindow ("TaharezLook/Editbox", name);
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

				rwWindow = new RXTextBox ((wxWindow *)wwParent, -1, wxT (text.c_str ()), 
					wxPoint (v2dPos.x, v2dPos.y), wxSize (v2dSize.x, v2dSize.y), wxTE_PROCESS_ENTER);
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

	void TextBox::destroy ()
	{
	}

	void TextBox::parseXML (App *level, XMLElement *element, AppObject *parent)
	{
		parseAppObjectXML (level, element, parent);

		std::string onclick = element->getAttributeValue ("onclick");
		setJavascriptToEvent ("onclick", onclick);
	}

	AppObject *TextBox::clone (std::string newName, int creationType, AppObject *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		TextBox *clone = new TextBox (*this);

		clone->setName (newName);
		clone->create (creationType, parent);
		ogrGame->getCurrentXMLFile ()->addAppObject (clone);

		return (clone);
	}

	void TextBox::setNativeText (std::string newText)
	{
		#ifdef defAPI_WXWIDGETS
			if (rwWindow != 0)
				((wxTextCtrl *)rwWindow)->SetValue (wxT (newText.c_str ()));
		#endif
	}

	std::string TextBox::getNativeText ()
	{
		#ifdef defAPI_WXWIDGETS
			return (RadXmlCommon::convertwxStringToString (((wxTextCtrl *)rwWindow)->GetValue ()));
		#endif

		return ("");
	}
}

#ifdef defAPI_WXWIDGETS
	// RXTextBox
	void RXTextBox::OnText (wxCommandEvent &evtEvent)
	{
		RadXml::TextBox *txtBox = (RadXml::TextBox *)this->GetClientData ();
		txtBox->executeJavascriptFromEvent ("ontextchanged");
	}

	void RXTextBox::OnTextEnter (wxCommandEvent &evtEvent)
	{
		RadXml::TextBox *txtBox = (RadXml::TextBox *)this->GetClientData ();
		txtBox->executeJavascriptFromEvent ("onenter");
	}
#endif

