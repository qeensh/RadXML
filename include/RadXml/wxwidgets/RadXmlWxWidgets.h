#ifndef _RADXML_WXWIDGETS_H_
	#define _RADXML_WXWIDGETS_H_

	#ifdef defAPI_WXWIDGETS
		#include <string>
		#include <vector>

		#include <wx/wx.h>

		class wxSocketEvent;

		#include "RadXml.h"

		namespace RadXml
		{
			class wxWidgetsRadXml: public wxRadXmlWidgetsApp
			{
				public:
					/*wxWidgetsRadXml ();

					bool OnInit ();
					int OnRun ();
					void OnUnhandledException ();
					void OnFatalException ();
					int OnExit ();

					// Networking
					#ifdef defNET_ON
						void ServerEvent (wxSocketEvent &evtEvent);

						void SocketEvent (wxSocketEvent &evtEvent);
					#endif

					void runSystem ();

					std::vector<wxThread *> aryThreads;
					wxCriticalSection wcsCritical;

				private:
					DECLARE_EVENT_TABLE ()*/
			};

			/*class wxWidgetsRadXml: public wxEvtHandler
			{
				public:
					virtual bool OnInit () = 0;
					virtual int OnRun () = 0;
					virtual void OnUnhandledException () = 0;
					virtual void OnFatalException () = 0;
					virtual int OnExit () = 0;

					// Networking
					#ifdef defNET_ON
						virtual void ServerEvent (wxSocketEvent &evtEvent) = 0;

						virtual void SocketEvent (wxSocketEvent &evtEvent) = 0;
					#endif

					virtual void runSystem () = 0;
			};*/
		}
	#endif
#endif

