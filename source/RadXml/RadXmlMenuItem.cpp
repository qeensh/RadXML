#include "RadXmlMenuItem.h"

#include "RadXmlApp.h"
#include "RadXml.h"
#include "RadXmlXmlFile.h"

#include "xml/xml.h"

#ifdef defAPI_WXWIDGETS
	#include "common/mai_radxml_system.h"

	IMPLEMENT_CLASS (RXMenu, wxMenu)

	BEGIN_EVENT_TABLE (RXMenu, wxMenu)
		//EVT_MENU (-1, RXMenu::OnClicked)
	END_EVENT_TABLE ()

	IMPLEMENT_CLASS (RXMenuItem, wxMenuItem)

	//BEGIN_EVENT_TABLE (RXMenuItem, wxMenuItem)
	//END_EVENT_TABLE ()

	unsigned int RXMenuItem::uiNumIds = 200;
	std::vector<RadXml::AppObject *> RXMenuItem::aryMenuObjects;
	std::vector<unsigned int> RXMenuItem::aryMenuIds;
#endif

namespace RadXml
{
	// MenuItem
	MenuItem::MenuItem (std::string newName)
		: AppObject (newName)
	{
		setType (AOT_MENU_ITEM);

		numChildMenuItems = 0;

		#ifdef defAPI_CEGUI
			wPopup = 0;
		#endif

		#ifdef defAPI_WXWIDGETS
			wmMenu = 0;
			wmiItem = 0;
		#endif

		addEvent (new Event ("onclick", ""));
	}

	MenuItem::~MenuItem ()
	{
		destroy ();
	}

	void MenuItem::create (int creationType, AppObject *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();

		if (creationType == AT_RENDER)
		{
			#ifdef defAPI_CEGUI
				if (parent != 0)
				{
					if (parent->getType () == AOT_MENU_ITEM)
					{
						MenuItem *miParent = static_cast <MenuItem *> (parent);

						if (miParent->numChildMenuItems == 0)
						{
							miParent->wPopup = CEGUI::WindowManager::getSingletonPtr ()->createWindow ("TaharezLook/PopupMenu", (name + "popup"));

							if (miParent->getWindow () != 0)
							{
								miParent->getWindow ()->addChildWindow (miParent->wPopup);
								wParent = miParent->wPopup;
							}
							else
							{
								ogrGame->getCEGUIRootWindow ()->addChildWindow (miParent->wPopup);
								wParent = miParent->wPopup;
							}
						}
						else
							wParent = miParent->wPopup;

						miParent->numChildMenuItems++;
					}
					else
						wParent = parent->getWindow ();
				}
				else
					wParent = ogrGame->getCEGUIRootWindow ();

				wWindow = CEGUI::WindowManager::getSingletonPtr ()->createWindow ("TaharezLook/MenuItem", name);
				wWindow->setUserData (this);
				subscribeEvent (defGUIFUNC_TYPE_MOUSE_CLICK);
			#endif
		}

		if (creationType == AT_NATIVE)
		{
			#ifdef defAPI_WXWIDGETS
				if (parent != 0)
				{
					if (parent->getType () == AOT_MENU)
					{
						wxMenuBar *mnuBar = (wxMenuBar *)parent->getNativeWindow ();
						wmMenu = new RXMenu ();
						wmMenu->SetClientData (this);
						AppObject *aoParent = parent->getParent ();
						wxWindow *wParent = aoParent->getNativeWindow ();
						wParent->Connect (wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&RXMenu::OnClicked);
						mnuBar->Append (wmMenu, wxT (text.c_str ()));
					}

					if (parent->getType () == AOT_MENU_ITEM)
					{
						MenuItem *miParent = ((MenuItem *)parent);
						wxMenu *mnuMenu = miParent->wmMenu;
						wxMenuItem *mnuItem = miParent->wmiItem;

						if (mnuMenu != 0)
						{
							wmiItem = new RXMenuItem (mnuMenu, -1, wxT (text.c_str ()));
							wmiItem->aoObject = this;
							wmiItem->SetId (RXMenuItem::uiNumIds);
							RXMenuItem::aryMenuObjects.push_back (this);
							RXMenuItem::aryMenuIds.push_back (RXMenuItem::uiNumIds);
							RXMenuItem::uiNumIds++;
							mnuMenu->Append (wmiItem);
						}

						if (mnuItem != 0)
						{
							wxMenu *mnuParent = mnuItem->GetMenu ();
							mnuParent->Delete (mnuItem);
							miParent->wmiItem = 0;

							if (miParent->wmMenu != 0)
							{
								delete miParent->wmMenu;
								miParent->wmMenu = 0;
							}

							miParent->wmMenu = new RXMenu ();
							miParent->wmMenu->SetClientData (this);
							miParent->wmMenu->Append (-1, wxT (text.c_str ()));
							mnuParent->Append (-1, wxT (miParent->getText ().c_str ()), miParent->wmMenu);
						}
					}
				}
			#endif
		}

		setCreationType (creationType);

		setVisible (bVisible);
		setPosition (position.x, position.y);
		setSize (size.x, size.y);
		setText (text);
	}

	void MenuItem::destroy ()
	{
		#ifdef defAPI_WXWIDGETS
			/// @note Delete this menu? It crashes whenever I do.
			/*if (wmMenu != 0)
			{
				delete wmMenu;
				wmMenu = 0;
			}*/

			if (wmiItem != 0)
			{
				delete wmiItem;
				wmiItem = 0;
			}
		#endif
	}

	void MenuItem::parseXML (App *level, XMLElement *element, AppObject *parent)
	{
		parseAppObjectXML (level, element, parent);

		std::string onclick = element->getAttributeValue ("onclick");
		setJavascriptToEvent ("onclick", onclick);
	}

	AppObject *MenuItem::clone (std::string newName, int creationType, AppObject *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		MenuItem *clone = new MenuItem (*this);

		clone->setName (newName);
		clone->create (creationType, parent);
		ogrGame->getCurrentXMLFile ()->addAppObject (clone);

		return (clone);
	}
}

// RXMenu
void RXMenu::OnClicked (wxCommandEvent &evtEvent)
{
	int iId = evtEvent.GetId ();
	RadXml::MenuItem *rmiItem = (RadXml::MenuItem *)RXMenuItem::findObjectByMenuId (iId);
	rmiItem->executeJavascriptFromEvent ("onclick");
}

// RXMenuItem
unsigned int RXMenuItem::findObjectIndexByMenuId (unsigned int id)
{
	for (unsigned int iIdx = 0; iIdx < RXMenuItem::aryMenuIds.size (); iIdx++)
	{
		unsigned int iIndex = RXMenuItem::aryMenuIds.at (iIdx);

		if (iIndex == id)
			return (iIdx);
	}

	return (-1);
}

RadXml::AppObject *RXMenuItem::findObjectByMenuId (unsigned int id)
{
	unsigned int iIndex = findObjectIndexByMenuId (id);

	if (iIndex == -1)
		return (0);

	return (RXMenuItem::aryMenuObjects.at (iIndex));
}

