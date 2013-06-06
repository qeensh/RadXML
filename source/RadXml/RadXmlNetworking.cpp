#include "RadXmlNetworking.h"

#include "RadXmlNative.h"

#include "networking/net_global.h"

#ifdef defAPI_WXWIDGETS
	#include "wxwidgets/RadXmlWxWidgetsNetworking.h"
#endif

namespace RadXml
{
	#ifdef defNET_ON
	void NetworkServerClass::setupNetworkServerClass ()
	{
		Class *cClass = new Class ("NetworkServer", "NetworkServer");
		cClass->set ("listen", new Function ("listen", 0, NetworkServerClass::listen));
		cClass->set ("send", new Function ("send", 0, NetworkServerClass::send));
		cClass->set ("sendToAll", new Function ("sendToAll", 0, NetworkServerClass::sendToAll));
		cClass->set ("getIPs", new Function ("getIPs", 0, NetworkServerClass::getIPs));
		cClass->set ("getNumIPs", new Function ("getNumIPs", 0, NetworkServerClass::getNumIPs));
		cClass->set ("disconnectClient", new Function ("disconnectClient", 0, NetworkServerClass::disconnectClient));
		cClass->set ("close", new Function ("close", 0, NetworkServerClass::close));

		JavascriptManager::getSingletonPtr ()->set ("NetworkServer", cClass);
	}

	// NetworkServerClass
	Variable *NetworkServerClass::listen (Variable *thisObject, Variable *arguments)
	{
		Boolean *bReturn = new Boolean ("");
		Array *aryArguments = static_cast <Array *> (arguments);
		double dPort = ((Number *)aryArguments->getElement (0))->getValue ();
		s13TcpipServer *netServer = new s13TcpipServer ();

		bReturn->setValue (false);
		netServer->addHiddenData ("socket", thisObject);

		if (netServer->Init (dPort) == false)
			return (bReturn);

		thisObject->addHiddenData ("NetworkServer", netServer);
		bReturn->setValue (true);

		return (bReturn);
	}

	Variable *NetworkServerClass::send (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		double dIndex = ((Number *)aryArguments->getElement (0))->getValue ();
		std::string strText = ((String *)aryArguments->getElement (1))->getValue ();
		s13TcpipServer *netServer = (s13TcpipServer *)thisObject->getHiddenData ("NetworkServer");

		netServer->SendString (dIndex, strText);

		return (0);
	}

	Variable *NetworkServerClass::sendToAll (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		std::string strText = ((String *)aryArguments->getElement (0))->getValue ();
		Array *aryExceptions = (Array *)aryArguments->getElement (1);
		s13TcpipServer *netServer = (s13TcpipServer *)thisObject->getHiddenData ("NetworkServer");
		std::vector<unsigned int> aryExcepts;

		if (aryExceptions != 0)
		{
			for (unsigned int iIdx = 0; iIdx < aryExceptions->length (); iIdx++)
			{
				double dNum = ((Number *)aryExceptions->getElement (iIdx))->getValue ();
				aryExcepts.push_back (dNum);
			}
		}

		netServer->SendStringToAll (strText, aryExcepts);

		return (0);
	}

	Variable *NetworkServerClass::getIPs (Variable *thisObject, Variable *arguments)
	{
		Array *aryReturn = new Array ("");
		Array *aryArguments = static_cast <Array *> (arguments);
		s13TcpipServer *netServer = (s13TcpipServer *)thisObject->getHiddenData ("NetworkServer");
		std::vector<s13Ipv4> aryIPs = netServer->GetConnectedIPs ();

		for (unsigned int iIdx = 0; iIdx < aryIPs.size (); iIdx++)
		{
			std::string strIP = aryIPs.at (iIdx).GetString ();
			aryReturn->addElement (new String ("", strIP));
		}

		return (aryReturn);
	}

	Variable *NetworkServerClass::getNumIPs (Variable *thisObject, Variable *arguments)
	{
		Number *iReturn = new Number ("");
		Array *aryArguments = static_cast <Array *> (arguments);
		s13TcpipServer *netServer = (s13TcpipServer *)thisObject->getHiddenData ("NetworkServer");

		iReturn->setValue (netServer->GetNumConnected ());

		return (iReturn);
	}

	Variable *NetworkServerClass::disconnectClient (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		double dIndex = ((Number *)aryArguments->getElement (0))->getValue ();
		s13TcpipServer *netServer = (s13TcpipServer *)thisObject->getHiddenData ("NetworkServer");

		netServer->CloseClient (dIndex);

		return (0);
	}

	Variable *NetworkServerClass::close (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		s13TcpipServer *netServer = (s13TcpipServer *)thisObject->getHiddenData ("NetworkServer");

		netServer->Close ();

		return (0);
	}

	// NetworkClientClass
	void NetworkClientClass::setupNetworkClientClass ()
	{
		Class *cClass = new Class ("NetworkClient", "NetworkClient");
		cClass->set ("connect", new Function ("connect", 0, NetworkClientClass::connect));
		cClass->set ("send", new Function ("send", 0, NetworkClientClass::send));
		cClass->set ("getServerIP", new Function ("getServerIP", 0, NetworkClientClass::getServerIP));
		cClass->set ("close", new Function ("close", 0, NetworkClientClass::close));

		JavascriptManager::getSingletonPtr ()->set ("NetworkClient", cClass);
	}

	Variable *NetworkClientClass::connect (Variable *thisObject, Variable *arguments)
	{
		Boolean *bReturn = new Boolean ("");
		Array *aryArguments = static_cast <Array *> (arguments);
		double dPort = ((Number *)aryArguments->getElement (0))->getValue ();
		std::string strServerIP = ((String *)aryArguments->getElement (1))->getValue ();
		s13TcpipClient *netClient = new s13TcpipClient ();

		bReturn->setValue (false);
		netClient->addHiddenData ("socket", thisObject);

		if (netClient->Init (dPort) == false)
			return (bReturn);

		netClient->ConnectWithoutBlocking (strServerIP);
		thisObject->addHiddenData ("NetworkClient", netClient);
		bReturn->setValue (true);

		return (bReturn);
	}

	Variable *NetworkClientClass::send (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		std::string strText = ((String *)aryArguments->getElement (0))->getValue ();
		s13TcpipClient *netClient = (s13TcpipClient *)thisObject->getHiddenData ("NetworkClient");

		netClient->SendString (strText);

		return (0);
	}

	Variable *NetworkClientClass::getServerIP (Variable *thisObject, Variable *arguments)
	{
		String *strIP = new String ("");
		s13TcpipClient *netClient = (s13TcpipClient *)thisObject->getHiddenData ("NetworkClient");

		strIP->setValue (netClient->GetServerIP ().GetString ());

		return (strIP);
	}

	Variable *NetworkClientClass::close (Variable *thisObject, Variable *arguments)
	{
		s13TcpipClient *netClient = (s13TcpipClient *)thisObject->getHiddenData ("NetworkClient");
		netClient->Close ();

		return (0);
	}
	#endif
}

