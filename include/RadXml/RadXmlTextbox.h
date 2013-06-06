#ifndef _RADXML_TEXT_BOX_H_
	#define _RADXML_TEXT_BOX_H_

	#include <string>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlAppObject.h"

	#ifdef defAPI_WXWIDGETS
		#include <wx/wx.h>

		class RXTextBox: public wxTextCtrl
		{
			public:
				RXTextBox (wxWindow *parent, wxWindowID id, const wxString &text, 
						const wxPoint &position, const wxSize &size, 
						const long lStyle = 0)
					: wxTextCtrl (parent, id, text, position, size, lStyle)
				{
				}

				void OnText (wxCommandEvent &evtEvent);
				void OnTextEnter (wxCommandEvent &evtEvent);

				DECLARE_CLASS (RXTextBox)
				DECLARE_EVENT_TABLE ()
		};
	#endif

	namespace RadXml
	{
		class XmlFile;

		class defRADXML_EXPORT TextBox: public AppObject
		{
			public:
				TextBox (std::string newName);
				~TextBox ();

				void create (int creationType, AppObject *parent = 0);
				void destroy ();

				void parseXML (App *level, XMLElement *element, AppObject *parent = 0);

				AppObject *clone (std::string newName, int creationType, AppObject *parent = 0);

				void setNativeText (std::string newText);
				std::string getNativeText ();
		};
	}
#endif

