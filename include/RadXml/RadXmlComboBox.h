#ifndef _RADXML_COMBOBOX_H_
	#define _RADXML_COMBOBOX_H_

	#include <string>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlAppObject.h"

	#ifdef defAPI_WXWIDGETS
		#include <wx/wx.h>

		class RXComboBox: public wxComboBox
		{
			public:
				RXComboBox (wxWindow *parent, wxWindowID id, const wxString &text, 
						const wxPoint &position, const wxSize &size)
					: wxComboBox (parent, id, text, position, size)
				{
				}

				DECLARE_CLASS (RXComboBox)
				DECLARE_EVENT_TABLE ()
		};
	#endif

	namespace RadXml
	{
		class XmlFile;

		class defRADXML_EXPORT Combobox: public AppObject
		{
			public:
				Combobox (std::string newName);
				~Combobox ();

				void create (int creationType, AppObject *parent = 0);
				void destroy ();

				void parseXML (App *level, XMLElement *element, AppObject *parent = 0);

				AppObject *clone (std::string newName, int creationType, AppObject *parent = 0);

				void addItem (std::string text);
				void setItem (unsigned int index, std::string text);
				std::string getItem (unsigned int index);
				void removeItem (unsigned int index);
				unsigned int getNumItems ();

				void setNativeText (std::string newText);
				std::string getNativeText ();
		};

		class ComboBoxClass
		{
			public:
				static void setupComboBox ();

				static Variable *addItem (Variable *thisObject, Variable *arguments);
				static Variable *setItem (Variable *thisObject, Variable *arguments);
				static Variable *getItem (Variable *thisObject, Variable *arguments);
				static Variable *removeItem (Variable *thisObject, Variable *arguments);
				static Variable *getNumItems (Variable *thisObject, Variable *arguments);
		};
	}
#endif

