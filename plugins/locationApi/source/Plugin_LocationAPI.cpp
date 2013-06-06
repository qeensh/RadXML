#include "Plugin_LocationAPI.h"

defRADXML_PLUGIN RadXml::Plugin *pluginInformation ()
{
	RadXml::Plugin *plgInfo = new RadXml::Plugin ("Location API", RadXml::PLT_JAVASCRIPT_API, "RadXmlLocationAPI");
	plgInfo->setDescription ("Javascript API that allows for geolocation services. This API is for Windows 7 or higher only.");
	plgInfo->setCopyright ("Copyright 2012, Higher Edge Software, LLC");
	plgInfo->setDeveloper ("Higher Edge Software, LLC");
	plgInfo->setVersion (1.0);

	return (plgInfo);
}

defRADXML_PLUGIN bool pluginLoaded ()
{
	return (true);
}

namespace RadXml
{
	LocationAPI *LocationAPI::rxSingleton = 0;

	// LocationAPI
	LocationAPI::LocationAPI ()
	{
		#ifdef defSYS_WIN32
			CComPtr<ILocation> cpLocation;
			hrLocation = cpLocation.CoCreateInstance(CLSID_Location);
		#endif
	}

	LocationAPI::~LocationAPI ()
	{
	}

	void LocationAPI::setupLocationAPI ()
	{
		
	}

	void LocationAPI::initialise ()
	{
		if (rxSingleton == 0)
			rxSingleton = new LocationAPI ();
		else
			rxSingleton = getSingletonPtr ();
	}

	void LocationAPI::destroy ()
	{
		if (rxSingleton != 0)
		{
			delete rxSingleton;
			rxSingleton = 0;
		}
	}
}

