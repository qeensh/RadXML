#include "RadXmlWindow.h"

#include "RadXmlApp.h"
#include "RadXml.h"
#include "RadXmlXmlFile.h"

#include "xml/xml.h"

#ifdef defAPI_WXWIDGETS
	#include "common/mai_radxml_system.h"

	IMPLEMENT_CLASS (RXWindow, wxFrame)

	BEGIN_EVENT_TABLE (RXWindow, wxFrame)
		EVT_CLOSE (RXWindow::OnClose)
	END_EVENT_TABLE ()
#endif

namespace RadXml
{
	// Window
	Window::Window (std::string newName)
		: AppObject (newName)
	{
		setType (AOT_WINDOW);
		addEvent (new Event ("onclosed", ""));
	}

	Window::~Window ()
	{
		destroy ();
	}

	void Window::create (int creationType, AppObject *parent)
	{
		if (creationType == AT_RENDER)
		{
			#ifdef defAPI_CEGUI
				wWindow = CEGUI::WindowManager::getSingletonPtr ()->createWindow ("TaharezLook/FrameWindow", name);
				wWindow->setUserData (this);
				subscribeEvent (defGUIFUNC_TYPE_INPUT_CLOSED);
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

				rwWindow = new RXWindow ((wxWindow *)wwParent, -1, wxT (text.c_str ()), wxPoint (v2dPos.x, v2dPos.y), wxSize (v2dSize.x, v2dSize.y));
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

	void Window::destroy ()
	{
	}

	void Window::parseXML (App *level, XMLElement *element, AppObject *parent)
	{
		parseAppObjectXML (level, element, parent);

		std::string strOnClosed = element->getAttributeValue ("onclosed");
		setJavascriptToEvent ("onclosed", strOnClosed);
	}

	AppObject *Window::clone (std::string newName, int creationType, AppObject *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		Window *clone = new Window (*this);

		clone->setName (newName);
		clone->create (creationType, parent);
		ogrGame->getCurrentXMLFile ()->addAppObject (clone);

		return (clone);
	}

	void Window::setNativeText (std::string newText)
	{
		#ifdef defAPI_WXWIDGETS
			if (rwWindow != 0)
				((wxFrame *)rwWindow)->SetLabel (wxT (newText.c_str ()));
		#endif
	}

	std::string Window::getNativeText ()
	{
		#ifdef defAPI_WXWIDGETS
			return (RadXmlCommon::convertwxStringToString (((wxFrame *)rwWindow)->GetLabel ()));
		#endif

		return ("");
	}
}

#ifdef defAPI_WXWIDGETS
	// RXWindow
	void RXWindow::OnClose (wxCloseEvent &WXUNUSED (wceEvent))
	{
		Destroy ();
	}
#endif

