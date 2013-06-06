#include "RadXmlLabel.h"

#include "RadXmlApp.h"
#include "RadXml.h"
#include "RadXmlXmlFile.h"

#include "xml/xml.h"

#ifdef defAPI_WXWIDGETS
	#include "common/mai_radxml_system.h"

	IMPLEMENT_CLASS (RXLabel, wxStaticText)

	BEGIN_EVENT_TABLE (RXLabel, wxStaticText)
	END_EVENT_TABLE ()
#endif

namespace RadXml
{
	// Label
	Label::Label (std::string newName)
		: AppObject (newName)
	{
		setType (AOT_LABEL);
		addEvent (new Event ("onclick", ""));
		size.x = 0.15;
		size.y = 0.06;
	}

	Label::~Label ()
	{
		destroy ();
	}

	void Label::create (int creationType, AppObject *parent)
	{
		if (creationType == AT_RENDER)
		{
			#ifdef defAPI_CEGUI
				wWindow = CEGUI::WindowManager::getSingletonPtr ()->createWindow ("TaharezLook/StaticText", name);
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

				rwWindow = new RXLabel ((wxWindow *)wwParent, -1, wxT (text.c_str ()), wxPoint (v2dPos.x, v2dPos.y), wxSize (v2dSize.x, v2dSize.y));
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

	void Label::destroy ()
	{
	}

	void Label::parseXML (App *level, XMLElement *element, AppObject *parent)
	{
		parseAppObjectXML (level, element, parent);

		std::string onclick = element->getAttributeValue ("onclick");
		setJavascriptToEvent ("onclick", onclick);
	}

	AppObject *Label::clone (std::string newName, int creationType, AppObject *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		Label *clone = new Label (*this);

		clone->setName (newName);
		clone->create (creationType, parent);
		ogrGame->getCurrentXMLFile ()->addAppObject (clone);

		return (clone);
	}

	void Label::setNativeText (std::string newText)
	{
		#ifdef defAPI_WXWIDGETS
			if (rwWindow != 0)
				((wxStaticText *)rwWindow)->SetLabel (wxT (newText.c_str ()));
		#endif
	}

	std::string Label::getNativeText ()
	{
		#ifdef defAPI_WXWIDGETS
			return (RadXmlCommon::convertwxStringToString (((wxStaticText *)rwWindow)->GetLabel ()));
		#endif

		return ("");
	}
}

