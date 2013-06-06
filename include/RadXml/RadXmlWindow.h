#ifndef _RADXML_WINDOW_H_
	#define _RADXML_WINDOW_H_

	#include <string>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlAppObject.h"

	#ifdef defAPI_WXWIDGETS
		#include <wx/wx.h>

		class RXWindow: public wxFrame
		{
			public:
				RXWindow (wxWindow *parent, wxWindowID id, const wxString &title, 
						const wxPoint &position, const wxSize &size, 
						const long lStyle = wxDEFAULT_FRAME_STYLE)
					: wxFrame (parent, id, title, position, size, lStyle)
				{
				}

				void OnClose (wxCloseEvent &WXUNUSED (wceEvent));

				DECLARE_CLASS (RXWindow)
				DECLARE_EVENT_TABLE ()
		};
	#endif

	namespace RadXml
	{
		class XmlFile;

		class defRADXML_EXPORT Window: public AppObject
		{
			public:
				Window (std::string newName);
				~Window ();

				void create (int creationType, AppObject *parent = 0);
				void destroy ();

				void parseXML (App *level, XMLElement *element, AppObject *parent = 0);

				AppObject *clone (std::string newName, int creationType, AppObject *parent = 0);

				void setNativeText (std::string newText);
				std::string getNativeText ();
		};
	}
#endif

