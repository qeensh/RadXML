#ifndef _RADXML_LABEL_H_
	#define _RADXML_LABEL_H_

	#include <string>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlAppObject.h"

	#ifdef defAPI_WXWIDGETS
		#include <wx/wx.h>

		class RXLabel: public wxStaticText
		{
			public:
				RXLabel (wxWindow *parent, wxWindowID id, const wxString &text, 
						const wxPoint &position, const wxSize &size, 
						const long lStyle = 0)
					: wxStaticText (parent, id, text, position, size, lStyle)
				{
				}

				DECLARE_CLASS (RXLabel)
				DECLARE_EVENT_TABLE ()
		};
	#endif

	namespace RadXml
	{
		class XmlFile;

		class defRADXML_EXPORT Label: public AppObject
		{
			public:
				Label (std::string newName);
				~Label ();

				void create (int creationType, AppObject *parent = 0);
				void destroy ();

				void parseXML (App *level, XMLElement *element, AppObject *parent = 0);

				AppObject *clone (std::string newName, int creationType, AppObject *parent = 0);

				void setNativeText (std::string newText);
				std::string getNativeText ();
		};
	}
#endif

