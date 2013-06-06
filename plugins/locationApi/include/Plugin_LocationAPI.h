#ifndef _PLUGIN_LOCATIONAPI_H_
	#define _PLUGIN_LOCATIONAPI_H_

	#include <RadXml/RadXmlNative.h>

	#ifdef defSYS_WIN32
		#include <windows.h>
		#include <atlbase.h>
		#include <atlcom.h>
		#include <Objbase.h>
		#include <locationapi.h>
	#endif

	#ifdef __cplusplus
	extern "C"
	{
	#endif

	defRADXML_PLUGIN RadXml::Plugin *pluginInformation ();
	defRADXML_PLUGIN bool pluginLoaded ();

	#ifdef __cplusplus
	}
	#endif

	namespace RadXml
	{
		class LocationAPI
		{
			public:
				static void setupLocationAPI ();

				static void initialise ();
				static void destroy ();

				inline static LocationAPI getSingleton ()
				{
					return (*rxSingleton);
				}

				inline static LocationAPI *getSingletonPtr ()
				{
					return (rxSingleton);
				}

			protected:
				LocationAPI ();
				~LocationAPI ();

				static LocationAPI *rxSingleton;

				#ifdef defSYS_WIN32
					HRESULT hrLocation;
				#endif
		};
	}
#endif

