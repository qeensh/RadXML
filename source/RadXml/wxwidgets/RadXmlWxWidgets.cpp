#include "wxwidgets/RadXmlWxWidgets.h"

#ifdef defAPI_WXWIDGETS
	#include <wx/wxprec.h>
	#include <wx/wx.h>
	#include <wx/evtloop.h>
	#include <wx/display.h>
	#include <wx/stdpaths.h>
	#include <wx/string.h>

	#ifdef defNET_ON
		#include "wxwidgets/RadXmlWxWidgetsNetworking.h"
	#endif

	#ifdef defSYS_LINUX
		#define GSocket GlibGSocket
		#include <gdk/gdk.h>
		#include <gtk/gtk.h>
		#include <wx/gtk/win_gtk.h>
		#include <gdk/gdkx.h>
		#include <GL/glx.h>
		#undef GSocket
	#endif

#include "common/mai_radxml_system.h"

namespace RadXml
{
	/*BEGIN_EVENT_TABLE (wxWidgetsRadXml, wxApp)
		#ifdef defNET_ON
			EVT_SOCKET (defSOCKET_SERVER, wxWidgetsRadXml::ServerEvent)

			EVT_SOCKET (defSOCKET_SOCKET, wxWidgetsRadXml::SocketEvent)
		#endif
	END_EVENT_TABLE ()

	IMPLEMENT_APP_NO_MAIN(wxWidgetsRadXml)

	// wxWidgetsRadXml
	wxWidgetsRadXml::wxWidgetsRadXml ()
	{
	}

	bool wxWidgetsRadXml::OnInit ()
	{
		return (true);
	}

	int wxWidgetsRadXml::OnRun ()
	{
		//for (int iIdx = 0; iIdx < (int)aryWindows.size (); iIdx++)
		//{
		//	wxPaintEvent evtEvent;
		//	aryWindows.at (iIdx)->OnPaint (evtEvent);
		//}

		return (wxApp::OnRun ());
	}

	void wxWidgetsRadXml::OnUnhandledException ()
	{
	}

	void wxWidgetsRadXml::OnFatalException ()
	{
	}

	int wxWidgetsRadXml::OnExit ()
	{
		return (0);
	}

	void wxWidgetsRadXml::runSystem ()
	{
		wxEventLoop *evtLoop = new wxEventLoop ();
		wxEventLoop *evtOldLoop = wxEventLoop::GetActive ();
		wxEventLoop::SetActive (evtLoop);

		if (Pending () == true)
			Dispatch ();

		wxEventLoop::SetActive (evtOldLoop);

		delete evtLoop;
		evtLoop = 0;
	}

	#ifdef defNET_ON
	void wxWidgetsRadXml::ServerEvent (wxSocketEvent &evtEvent)
	{
		switch (evtEvent.GetSocketEvent ())
		{
			case wxSOCKET_CONNECTION:
				//scbMultiplayer_PuzzleCallback->Execute (defNET_HANDLE_TYPE_ACCEPT, -1);
				break;
		}
	}

	void wxWidgetsRadXml::SocketEvent (wxSocketEvent &evtEvent)
	{
		switch (evtEvent.GetSocketEvent ())
		{
			case wxSOCKET_CONNECTION:
				//scbMultiplayer_PuzzleCallback->Execute (defNET_HANDLE_TYPE_CONNECT, -1);
				break;
			case wxSOCKET_INPUT:
				//scbMultiplayer_PuzzleCallback->Execute (defNET_HANDLE_TYPE_READ, -1);
				break;
			case wxSOCKET_LOST:
				//scbMultiplayer_PuzzleCallback->Execute (defNET_HANDLE_TYPE_ERROR, -1);
				break;
		}
	}
	#endif*/
}
#endif

