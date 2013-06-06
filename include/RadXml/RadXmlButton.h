#ifndef _RADXML_BUTTON_H_
	#define _RADXML_BUTTON_H_

	#include <string>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlAppObject.h"

	#ifdef defAPI_WXWIDGETS
		#include <wx/wx.h>

		class RXButton: public wxButton
		{
			public:
				RXButton (wxWindow *parent, wxWindowID id, const wxString &text, 
						const wxPoint &position, const wxSize &size, 
						const long lStyle = 0)
					: wxButton (parent, id, text, position, size, lStyle)
				{
				}

				void OnClicked (wxCommandEvent &evtEvent);

				DECLARE_CLASS (RXButton)
				DECLARE_EVENT_TABLE ()
		};
	#endif

	namespace RadXml
	{
		class XmlFile;

		class defRADXML_EXPORT Button: public AppObject
		{
			public:
				Button (std::string newName);
				~Button ();

				void create (int creationType, AppObject *parent = 0);
				void destroy ();

				void parseXML (App *level, XMLElement *element, AppObject *parent = 0);

				AppObject *clone (std::string newName, int creationType, AppObject *parent = 0);

				void setNativeText (std::string newText);
				std::string getNativeText ();
		};
	}
#endif

