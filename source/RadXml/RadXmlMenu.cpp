#include "RadXmlMenu.h"

#include "RadXmlApp.h"
#include "RadXml.h"
#include "RadXmlXmlFile.h"

namespace RadXml
{
	// Menu
	Menu::Menu (std::string newName)
		: AppObject (newName)
	{
		setType (AOT_MENU);
		size.x = 1;
		size.y = 0.06;

		menuWindow = 0;
	}

	Menu::~Menu ()
	{
		destroy ();
	}

	void Menu::create (int creationType, AppObject *parent)
	{
		if (creationType == AT_RENDER)
		{
			#ifdef defAPI_CEGUI
				wWindow = CEGUI::WindowManager::getSingletonPtr ()->createWindow ("TaharezLook/Menubar", name);
				wWindow->setMinSize (CEGUI::UVector2 (CEGUI::UDim (size.x, 0), CEGUI::UDim (size.y, 0)));
				wWindow->setMaxSize (CEGUI::UVector2 (CEGUI::UDim (size.x, 0), CEGUI::UDim (size.y, 0)));
				wWindow->setUserData (this);
			#endif
		}

		if (creationType == AT_NATIVE)
		{
			#ifdef defAPI_WXWIDGETS
				rwWindow = new wxMenuBar ();
				rwWindow->Show (bVisible);
				rwWindow->SetClientData (this);

				if (parent == 0)
				{
					menuWindow = new Window (name + "-menuWindow");
					menuWindow->setText ("Menu");
					menuWindow->setSize (590, 75);
					menuWindow->create (creationType);
					parent = menuWindow;
				}

				if (parent != 0)
				{
					if (parent->getType () == AOT_WINDOW)
					{
						wxFrame *wwWindow = (wxFrame *)parent->getNativeWindow ();
						wwWindow->SetMenuBar ((wxMenuBar *)rwWindow);
					}
				}
			#endif
		}

		setCreationType (creationType);

		setParent (parent);
		setVisible (bVisible);
		setPosition (position.x, position.y);
		setSize (size.x, size.y);
		setText (text);
	}

	void Menu::destroy ()
	{
		if (menuWindow != 0)
		{
			delete menuWindow;
			menuWindow = 0;
		}
	}

	void Menu::parseXML (App *level, XMLElement *element, AppObject *parent)
	{
		parseAppObjectXML (level, element, parent);
	}

	AppObject *Menu::clone (std::string newName, int creationType, AppObject *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		Menu *clone = new Menu (*this);

		clone->setName (newName);
		clone->create (creationType, parent);
		ogrGame->getCurrentXMLFile ()->addAppObject (clone);

		return (clone);
	}
}

