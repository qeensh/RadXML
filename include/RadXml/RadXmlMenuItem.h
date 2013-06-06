#ifndef _RADXML_MENU_ITEM_H_
	#define _RADXML_MENU_ITEM_H_

	#include <string>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlAppObject.h"

	#ifdef defAPI_WXWIDGETS
		#include <wx/wx.h>

		namespace RadXml
		{
			class AppObject;
		}

		class RXMenu: public wxMenu
		{
			public:
				RXMenu (const wxString &text = "", long style = 0)
					: wxMenu (text, style)
				{
				}

				void OnClicked (wxCommandEvent &evtEvent);

				DECLARE_CLASS (RXMenu)
				DECLARE_EVENT_TABLE ()
		};

		class RXMenuItem: public wxMenuItem
		{
			public:
				RXMenuItem (wxMenu *parentMenu = 0, int id = wxID_SEPARATOR, const wxString &text = "", 
					const wxString &helpString = "", wxItemKind kind = wxITEM_NORMAL, wxMenu *subMenu = 0)
					: wxMenuItem (parentMenu, id, text, helpString, kind, subMenu)
				{
				}

				static unsigned int findObjectIndexByMenuId (unsigned int id);
				static RadXml::AppObject *findObjectByMenuId (unsigned int id);

				RadXml::AppObject *aoObject;
				static unsigned int uiNumIds;
				static std::vector<RadXml::AppObject *> aryMenuObjects;
				static std::vector<unsigned int> aryMenuIds;

				DECLARE_CLASS (RXMenuItem)
		};
	#endif

	namespace RadXml
	{
		class XmlFile;

		class defRADXML_EXPORT MenuItem: public AppObject
		{
			public:
				MenuItem (std::string newName);
				~MenuItem ();

				void create (int creationType, AppObject *parent = 0);
				void destroy ();

				void parseXML (App *level, XMLElement *element, AppObject *parent = 0);

				AppObject *clone (std::string newName, int creationType, AppObject *parent = 0);

				int numChildMenuItems;

				#ifdef defAPI_CEGUI
					CEGUI::Window *wPopup;
				#endif

				#ifdef defAPI_WXWIDGETS
					RXMenu *wmMenu;
					RXMenuItem *wmiItem;
				#endif
		};
	}
#endif

