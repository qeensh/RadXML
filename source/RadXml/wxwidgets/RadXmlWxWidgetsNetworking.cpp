#include "wxwidgets/RadXmlWxWidgetsNetworking.h"

#include "RadXml.h"

#include "common/mai_radxml_system.h"
#include "common/com_radxml_array.h"
#include "common/mai_radxml_system.h"

#ifndef defNET_MAX_TEXT_SIZE
	#define defNET_MAX_TEXT_SIZE	256
#endif

#ifdef defAPI_WXWIDGETS
	#include <wx/wx.h>
	#include <wx/event.h>

#ifdef defNET_ON

#include "networking/net_global.h"

std::vector<std::string> WXW_GetComputerIPs ()
{
	std::vector<std::string> aryReturn;
	wxIPV4address ipAddress;
	ipAddress.Hostname (wxGetFullHostName ());

	wxString strAddr = ipAddress.IPAddress ();
	aryReturn.push_back (RadXmlCommon::convertwxStringToString (strAddr));

	return (aryReturn);
}

std::vector< std::vector<unsigned long> > WXW_GetMACAddresses ()
{
	std::vector< std::vector<unsigned long> > aryReturn;

	return (aryReturn);
}

// s13WXWServer
s13WXWServer::s13WXWServer ()
{
	wssServer = 0;
	usPort = 0;
	tcpParent = 0;
}

bool s13WXWServer::Init (unsigned short usPortPass)
{
	wxIPV4address ipAddress;
	ipAddress.AnyAddress ();
	ipAddress.Service (usPortPass);

	wssServer = new wxSocketServer (ipAddress);

	if (wssServer->Ok () == false)
	{
		wssServer->Close ();

		if (wssServer != 0)
		{
			delete wssServer;
			wssServer = 0;
		}

		return (false);
	}

	wssServer->SetEventHandler (*(RadXml::RadXML::getSingletonPtr ()->getWXApp ()), defSOCKET_SERVER);
	wssServer->SetNotify (wxSOCKET_CONNECTION_FLAG);
	void *vData = tcpParent->getHiddenData ("socket");
	wssServer->SetClientData (vData);
	wssServer->Notify (true);

	adrServer = ipAddress;
	usPort = usPortPass;

	return (true);
}

bool s13WXWServer::AcceptWithBlocking ()
{
	wxSocketBase *wsbSocket = wssServer->Accept (true);

	if (wsbSocket == 0)
		return (false);

	wsbSocket->SetEventHandler (*(RadXml::RadXML::getSingletonPtr ()->getWXApp ()), defSOCKET_SOCKET);
	wsbSocket->SetNotify (wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	void *vData = tcpParent->getHiddenData ("socket");
	wsbSocket->SetClientData (vData);
	wsbSocket->Notify (true);

	aryClientSockets.push_back (wsbSocket);

	wxIPV4address wsaAddr;
	wsbSocket->GetPeer (wsaAddr);
	aryAddresses.push_back (wsaAddr);

	return (true);
}

bool s13WXWServer::AcceptWithoutBlocking ()
{
	wxSocketBase *wsbSocket = wssServer->Accept (false);

	if (wsbSocket == 0)
		return (false);

	wsbSocket->SetEventHandler (*(RadXml::RadXML::getSingletonPtr ()->getWXApp ()), defSOCKET_SOCKET);
	wsbSocket->SetNotify (wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	void *vData = tcpParent->getHiddenData ("socket");
	wsbSocket->SetClientData (vData);
	wsbSocket->Notify (true);

	aryClientSockets.push_back (wsbSocket);

	wxIPV4address wsaAddr;
	wsbSocket->GetPeer (wsaAddr);
	aryAddresses.push_back (wsaAddr);

	return (true);
}

bool s13WXWServer::SendFloatToAll (float fFloat, std::vector<unsigned int> aryExcept)
{
	unsigned int uiSocketsLength = aryClientSockets.size ();

    for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
		bool bContinue = false;

		for (unsigned int uiJdx = 0; uiJdx < aryExcept.size (); uiJdx++)
		{
			if (uiIdx == aryExcept.at (uiJdx))
			{
				bContinue = true;

				break;
			}
		}

		if (bContinue == true)
			continue;

        if (SendFloat (aryClientSockets.at (uiIdx), fFloat) == false)
			continue;
	}

	return (true);
}

bool s13WXWServer::SendFloat (wxSocketBase *socSocket, float fFloat)
{
	socSocket->Write ((const char *)&fFloat, sizeof (float));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (false);
	}

	return (true);
}

bool s13WXWServer::SendDoubleToAll (double dDouble, std::vector<unsigned int> aryExcept)
{
	unsigned int uiSocketsLength = aryClientSockets.size ();

    for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
		bool bContinue = false;

		for (unsigned int uiJdx = 0; uiJdx < aryExcept.size (); uiJdx++)
		{
			if (uiIdx == aryExcept.at (uiJdx))
			{
				bContinue = true;

				break;
			}
		}

		if (bContinue == true)
			continue;

		if (SendDouble (aryClientSockets.at (uiIdx), dDouble) == false)
			continue;
	}

	return (true);
}

bool s13WXWServer::SendDouble (wxSocketBase *socSocket, double dDouble)
{
    socSocket->Write ((const char *)&dDouble, sizeof (double));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (false);
	}

	return (true);
}

bool s13WXWServer::SendIntToAll (int iInt, std::vector<unsigned int> aryExcept)
{
	unsigned int uiSocketsLength = aryClientSockets.size ();

    for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
		bool bContinue = false;

		for (unsigned int uiJdx = 0; uiJdx < aryExcept.size (); uiJdx++)
		{
			if (uiIdx == aryExcept.at (uiJdx))
			{
				bContinue = true;

				break;
			}
		}

		if (bContinue == true)
			continue;

		if (SendInt (aryClientSockets.at (uiIdx), iInt) == false)
			continue;
	}

	return (true);
}

bool s13WXWServer::SendInt (wxSocketBase *socSocket, int iInt)
{
    socSocket->Write ((const char *)&iInt, sizeof (int));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (false);
	}

	return (true);
}

bool s13WXWServer::SendUIntToAll (unsigned int uiInt, std::vector<unsigned int> aryExcept)
{
	unsigned int uiSocketsLength = aryClientSockets.size ();

    for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
		bool bContinue = false;

		for (unsigned int uiJdx = 0; uiJdx < aryExcept.size (); uiJdx++)
		{
			if (uiIdx == aryExcept.at (uiJdx))
			{
				bContinue = true;

				break;
			}
		}

		if (bContinue == true)
			continue;

		if (SendUInt (aryClientSockets.at (uiIdx), uiInt) == false)
			continue;
	}

	return (true);
}

bool s13WXWServer::SendUInt (wxSocketBase *socSocket, unsigned int uiInt)
{
    socSocket->Write ((const char *)&uiInt, sizeof (unsigned int));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (false);
	}

	return (true);
}

bool s13WXWServer::SendLongToAll (long lLong, std::vector<unsigned int> aryExcept)
{
	unsigned int uiSocketsLength = aryClientSockets.size ();

    for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
		bool bContinue = false;

		for (unsigned int uiJdx = 0; uiJdx < aryExcept.size (); uiJdx++)
		{
			if (uiIdx == aryExcept.at (uiJdx))
			{
				bContinue = true;

				break;
			}
		}

		if (bContinue == true)
			continue;

		if (SendLong (aryClientSockets.at (uiIdx), lLong) == false)
			continue;
	}

	return (true);
}

bool s13WXWServer::SendLong (wxSocketBase *socSocket, long lLong)
{
    socSocket->Write ((const char *)&lLong, sizeof (long));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (false);
	}

	return (true);
}

bool s13WXWServer::SendULongToAll (unsigned long ulLong, std::vector<unsigned int> aryExcept)
{
	unsigned int uiSocketsLength = aryClientSockets.size ();

    for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
		bool bContinue = false;

		for (unsigned int uiJdx = 0; uiJdx < aryExcept.size (); uiJdx++)
		{
			if (uiIdx == aryExcept.at (uiJdx))
			{
				bContinue = true;

				break;
			}
		}

		if (bContinue == true)
			continue;

		if (SendULong (aryClientSockets.at (uiIdx), ulLong) == false)
			continue;
	}

	return (true);
}

bool s13WXWServer::SendULong (wxSocketBase *socSocket, unsigned long ulLong)
{
    socSocket->Write ((const char *)&ulLong, sizeof (unsigned long));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (false);
	}

	return (true);
}

bool s13WXWServer::SendShortToAll (short sShort, std::vector<unsigned int> aryExcept)
{
	unsigned int uiSocketsLength = aryClientSockets.size ();

    for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
		bool bContinue = false;

		for (unsigned int uiJdx = 0; uiJdx < aryExcept.size (); uiJdx++)
		{
			if (uiIdx == aryExcept.at (uiJdx))
			{
				bContinue = true;

				break;
			}
		}

		if (bContinue == true)
			continue;

		if (SendShort (aryClientSockets.at (uiIdx), sShort) == false)
			continue;
	}

	return (true);
}

bool s13WXWServer::SendShort (wxSocketBase *socSocket, short sShort)
{
    socSocket->Write ((const char *)&sShort, sizeof (short));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (false);
	}

	return (true);
}

bool s13WXWServer::SendUShortToAll (unsigned short usShort, std::vector<unsigned int> aryExcept)
{
	unsigned int uiSocketsLength = aryClientSockets.size ();

    for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
		bool bContinue = false;

		for (unsigned int uiJdx = 0; uiJdx < aryExcept.size (); uiJdx++)
		{
			if (uiIdx == aryExcept.at (uiJdx))
			{
				bContinue = true;

				break;
			}
		}

		if (bContinue == true)
			continue;

		if (SendUShort (aryClientSockets.at (uiIdx), usShort) == false)
			continue;
	}

	return (true);
}

bool s13WXWServer::SendUShort (wxSocketBase *socSocket, unsigned short usShort)
{
    socSocket->Write ((const char *)&usShort, sizeof (unsigned short));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (false);
	}

	return (true);
}

bool s13WXWServer::SendCharToAll (char cChar, std::vector<unsigned int> aryExcept)
{
	unsigned int uiSocketsLength = aryClientSockets.size ();

    for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
		bool bContinue = false;

		for (unsigned int uiJdx = 0; uiJdx < aryExcept.size (); uiJdx++)
		{
			if (uiIdx == aryExcept.at (uiJdx))
			{
				bContinue = true;

				break;
			}
		}

		if (bContinue == true)
			continue;

		if (SendChar (aryClientSockets.at (uiIdx), cChar) == false)
			continue;
	}

	return (true);
}

bool s13WXWServer::SendChar (wxSocketBase *socSocket, char cChar)
{
    socSocket->Write ((const char *)&cChar, sizeof (char));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (false);
	}

	return (true);
}

bool s13WXWServer::SendUCharToAll (unsigned char ucChar, std::vector<unsigned int> aryExcept)
{
	unsigned int uiSocketsLength = aryClientSockets.size ();

    for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
		bool bContinue = false;

		for (unsigned int uiJdx = 0; uiJdx < aryExcept.size (); uiJdx++)
		{
			if (uiIdx == aryExcept.at (uiJdx))
			{
				bContinue = true;

				break;
			}
		}

		if (bContinue == true)
			continue;

		if (SendUChar (aryClientSockets.at (uiIdx), ucChar) == false)
			continue;
	}

	return (true);
}

bool s13WXWServer::SendUChar (wxSocketBase *socSocket, unsigned char ucChar)
{
    socSocket->Write ((const char *)&ucChar, sizeof (unsigned char));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (false);
	}

	return (true);
}

bool s13WXWServer::SendStringToAll (std::string strLine, std::vector<unsigned int> aryExcept)
{
	int iError = 0;
	int iLength = ((int)strLine.size () + 1);
	unsigned int uiSocketsLength = aryClientSockets.size ();
	int uiIdx = 0;

	for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
		bool bContinue = false;

		for (unsigned int uiJdx = 0; uiJdx < aryExcept.size (); uiJdx++)
		{
			if (uiIdx == aryExcept.at (uiJdx))
			{
				bContinue = true;

				break;
			}
		}

		if (bContinue == true)
			continue;

		if (SendString (aryClientSockets.at (uiIdx), strLine) == false)
			continue;
	}

	return (true);
}

bool s13WXWServer::SendString (wxSocketBase *socSocket, std::string strLine)
{
	char *cBuf = new char[(strLine.size () + 1)];
	const char *cCopy = strLine.c_str ();

	for (unsigned int iIdx = 0; iIdx < (strLine.size () + 1); iIdx++)
		cBuf[iIdx] = cCopy[iIdx];

	cBuf[strLine.size ()] = '\0';

	//ArrayZero (cBuf, (strLine.size () + 1));
	//ArrayCopy (((char *)strLine.c_str ()), cBuf, (strLine.size () + 1));

	socSocket->SetNotify (wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    socSocket->Write (cBuf, ((strLine.size () + 1) * sizeof (char)));
	socSocket->SetNotify (wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);

	delete []cBuf;
	cBuf = 0;

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (false);
	}

	return (true);
}

std::vector<float> s13WXWServer::RecvFloat ()
{
    std::vector<float> aryReturn;
	unsigned int uiSocketsLength = aryClientSockets.size ();

	aryReceivedFrom.clear ();

	for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
        float adtReturn = RecvFloat (aryClientSockets.at (uiIdx));

		aryReturn.push_back (adtReturn);
		aryReceivedFrom.push_back (uiIdx);
	}

	return (aryReturn);
}

float s13WXWServer::RecvFloat (wxSocketBase *socSocket)
{
    float fFloat = 0.0f;
	socSocket->Read ((char *)&fFloat, sizeof (float));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (0.0f);
	}

	return (fFloat);
}

std::vector<double> s13WXWServer::RecvDouble ()
{
    std::vector<double> aryReturn;
	unsigned int uiSocketsLength = aryClientSockets.size ();

	aryReceivedFrom.clear ();

	for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
        double adtReturn = RecvDouble (aryClientSockets.at (uiIdx));

		aryReturn.push_back (adtReturn);
		aryReceivedFrom.push_back (uiIdx);
	}

	return (aryReturn);
}

double s13WXWServer::RecvDouble (wxSocketBase *socSocket)
{
    double dDouble = 0.0;
	socSocket->Read ((char *)&dDouble, sizeof (double));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (0.0);
	}

	return (dDouble);
}

std::vector<int> s13WXWServer::RecvInt ()
{
    std::vector<int> aryReturn;
	unsigned int uiSocketsLength = aryClientSockets.size ();

	aryReceivedFrom.clear ();

	for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
        int adtReturn = RecvInt (aryClientSockets.at (uiIdx));

		aryReturn.push_back (adtReturn);
		aryReceivedFrom.push_back (uiIdx);
	}

	return (aryReturn);
}

int s13WXWServer::RecvInt (wxSocketBase *socSocket)
{
    int iInt = 0;
	socSocket->Read ((char *)&iInt, sizeof (int));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (0);
	}

	return (iInt);
}

std::vector<unsigned int> s13WXWServer::RecvUInt ()
{
    std::vector<unsigned int> aryReturn;
	unsigned int uiSocketsLength = aryClientSockets.size ();

	aryReceivedFrom.clear ();

	for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
        unsigned int adtReturn = RecvUInt (aryClientSockets.at (uiIdx));

		aryReturn.push_back (adtReturn);
		aryReceivedFrom.push_back (uiIdx);
	}

	return (aryReturn);
}

unsigned int s13WXWServer::RecvUInt (wxSocketBase *socSocket)
{
    unsigned int uiInt = 0;
	socSocket->Read ((char *)&uiInt, sizeof (unsigned int));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (0);
	}

	return (uiInt);
}

std::vector<long> s13WXWServer::RecvLong ()
{
    std::vector<long> aryReturn;
	unsigned int uiSocketsLength = aryClientSockets.size ();

	aryReceivedFrom.clear ();

	for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
        long adtReturn = RecvLong (aryClientSockets.at (uiIdx));

		aryReturn.push_back (adtReturn);
		aryReceivedFrom.push_back (uiIdx);
	}

	return (aryReturn);
}

long s13WXWServer::RecvLong (wxSocketBase *socSocket)
{
    long lLong = 0;
	socSocket->Read ((char *)&lLong, sizeof (long));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (0);
	}

	return (lLong);
}

std::vector<unsigned long> s13WXWServer::RecvULong ()
{
    std::vector<unsigned long> aryReturn;
	unsigned int uiSocketsLength = aryClientSockets.size ();

	aryReceivedFrom.clear ();

	for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
        unsigned long adtReturn = RecvULong (aryClientSockets.at (uiIdx));

		aryReturn.push_back (adtReturn);
		aryReceivedFrom.push_back (uiIdx);
	}

	return (aryReturn);
}

unsigned long s13WXWServer::RecvULong (wxSocketBase *socSocket)
{
    unsigned long ulLong = 0;
	socSocket->Read ((char *)&ulLong, sizeof (unsigned long));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (0);
	}

	return (ulLong);
}

std::vector<short> s13WXWServer::RecvShort ()
{
    std::vector<short> aryReturn;
	unsigned int uiSocketsLength = aryClientSockets.size ();

	aryReceivedFrom.clear ();

	for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
        short adtReturn = RecvShort (aryClientSockets.at (uiIdx));

		aryReturn.push_back (adtReturn);
		aryReceivedFrom.push_back (uiIdx);
	}

	return (aryReturn);
}

short s13WXWServer::RecvShort (wxSocketBase *socSocket)
{
    short sShort = 0;
	socSocket->Read ((char *)&sShort, sizeof (short));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (0);
	}

	return (sShort);
}

std::vector<unsigned short> s13WXWServer::RecvUShort ()
{
    std::vector<unsigned short> aryReturn;
	unsigned int uiSocketsLength = aryClientSockets.size ();

	aryReceivedFrom.clear ();

	for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
        unsigned short adtReturn = RecvUShort (aryClientSockets.at (uiIdx));

		aryReturn.push_back (adtReturn);
		aryReceivedFrom.push_back (uiIdx);
	}

	return (aryReturn);
}

unsigned short s13WXWServer::RecvUShort (wxSocketBase *socSocket)
{
    unsigned short usShort = 0;
	socSocket->Read ((char *)&usShort, sizeof (unsigned short));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (0);
	}

	return (usShort);
}

std::vector<char> s13WXWServer::RecvChar ()
{
    std::vector<char> aryReturn;
	unsigned int uiSocketsLength = aryClientSockets.size ();

	aryReceivedFrom.clear ();

	for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
        char adtReturn = RecvChar (aryClientSockets.at (uiIdx));

		aryReturn.push_back (adtReturn);
		aryReceivedFrom.push_back (uiIdx);
	}

	return (aryReturn);
}

char s13WXWServer::RecvChar (wxSocketBase *socSocket)
{
    char cChar = 0x00;
	socSocket->Read ((char *)&cChar, sizeof (char));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (0);
	}

	return (cChar);
}

std::vector<unsigned char> s13WXWServer::RecvUChar ()
{
    std::vector<unsigned char> aryReturn;
	unsigned int uiSocketsLength = aryClientSockets.size ();

	aryReceivedFrom.clear ();

	for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
        unsigned char adtReturn = RecvUChar (aryClientSockets.at (uiIdx));

		aryReturn.push_back (adtReturn);
		aryReceivedFrom.push_back (uiIdx);
	}

	return (aryReturn);
}

unsigned char s13WXWServer::RecvUChar (wxSocketBase *socSocket)
{
    unsigned char ucChar = 0x00;
	socSocket->Read ((char *)&ucChar, sizeof (unsigned char));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (0);
	}

	return (ucChar);
}

std::vector<char *> s13WXWServer::RecvString (int iLength)
{
	std::vector<char *> aryReturn;
	unsigned int uiSocketsLength = aryClientSockets.size ();

	aryReceivedFrom.clear ();

	for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
		char *cString = RecvString (aryClientSockets.at (uiIdx), (iLength + 1));

		aryReturn.push_back (cString);
		aryReceivedFrom.push_back (uiIdx);
	}

	return (aryReturn);
}

char *s13WXWServer::RecvString (wxSocketBase *socSocket, int iLength)
{
	char *cString = new char [(iLength + 1)];
	//ArraySet (cString, (iLength + 1), '\0');

	for (unsigned int iIdx = 0; iIdx < (iLength + 1); iIdx++)
		cString[iIdx] = '\0';

	socSocket->Read (cString, (iLength + 1));

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return (0);
	}

	return (cString);
}

std::vector<std::string> s13WXWServer::RecvString ()
{
	std::vector<std::string> aryReturn;
	unsigned int uiSocketsLength = aryClientSockets.size ();

	aryReceivedFrom.clear ();

	for (unsigned int uiIdx = 0; uiIdx < uiSocketsLength; uiIdx++)
	{
		std::string strString = RecvString (aryClientSockets.at (uiIdx));

		aryReturn.push_back (strString);
		aryReceivedFrom.push_back (uiIdx);
	}

	return (aryReturn);
}

std::string s13WXWServer::RecvString (wxSocketBase *socSocket)
{
	char *cString = new char [(defNET_MAX_TEXT_SIZE + 1)];
	//ArraySet (cString, (defNET_MAX_TEXT_SIZE + 1), '\0');

	for (unsigned int iIdx = 0; iIdx < (defNET_MAX_TEXT_SIZE + 1); iIdx++)
		cString[iIdx] = '\0';

	socSocket->Read (cString, defNET_MAX_TEXT_SIZE);

	std::string strString = cString;
	delete []cString;
	cString = 0;

	if (socSocket->Error () == true)
	{
		CloseClientSocket (socSocket);

		return ("");
	}

	return (strString);
}

void s13WXWServer::SetPort (unsigned short usPortPass)
{
	usPort = usPortPass;
}

unsigned short s13WXWServer::GetPort ()
{
	return (usPort);
}

void s13WXWServer::SetServerAddr (wxIPV4address adrServerPass)
{
	adrServer = adrServerPass;
}

wxIPV4address s13WXWServer::GetServerAddr ()
{
	return (adrServer);
}

void s13WXWServer::SetServerSocket (wxSocketServer *arySocketsPass)
{
	wssServer = arySocketsPass;
}

wxSocketServer *s13WXWServer::GetServerSocket ()
{
	return (wssServer);
}

void s13WXWServer::SetClientSockets (std::vector<wxSocketBase *> arySocketsPass)
{
	aryClientSockets = arySocketsPass;
}

std::vector<wxSocketBase *> s13WXWServer::GetClientSockets ()
{
	return (aryClientSockets);
}

wxSocketBase *s13WXWServer::GetClientSocket (unsigned int uiId)
{
	return (aryClientSockets.at (uiId));
}

std::string s13WXWServer::GetLastError ()
{
	int iError = -1;

	return ("");
}

std::vector<unsigned int> s13WXWServer::GetLastReceivedFromList ()
{
	return (aryReceivedFrom);
}

std::vector<std::string> s13WXWServer::GetConnectedIPs ()
{
	std::vector<std::string> aryIPs;

	for (unsigned int uiIdx = 0; uiIdx < aryAddresses.size (); uiIdx++)
		aryIPs.push_back (RadXmlCommon::convertwxStringToString (aryAddresses.at (uiIdx).IPAddress ()));

	return (aryIPs);
}

std::string s13WXWServer::GetConnectedIp (unsigned int uiId)
{
	return (RadXmlCommon::convertwxStringToString (aryAddresses.at (uiId).IPAddress ()));
}

std::string s13WXWServer::GetLastConnectedIp ()
{
	return (RadXmlCommon::convertwxStringToString (aryAddresses.at ((aryAddresses.size () - 1)).IPAddress ()));
}

bool s13WXWServer::IsSocketOpen (wxSocketBase *socSocket)
{
	bool bReturn = socSocket->IsConnected ();

	return (bReturn);
}

bool s13WXWServer::CloseServerSocket ()
{
	return (wssServer->Close ());
}

bool s13WXWServer::CloseClientSocket (wxSocketBase *socSocket)
{
	for (unsigned int uiIdx = 0; uiIdx < aryClientSockets.size (); uiIdx++)
	{
		if (socSocket == aryClientSockets.at (uiIdx))
		{
			aryClientSockets.at (uiIdx)->Close ();

			std::vector<wxSocketBase *>::iterator itBegin = aryClientSockets.begin ();
			itBegin += uiIdx;
			aryClientSockets.erase (itBegin);

			break;
		}
	}

	return (true);
}

bool s13WXWServer::CloseAllSockets ()
{
	unsigned int uiNumSockets = aryClientSockets.size ();

	for (unsigned int uiIdx = 0; uiIdx < uiNumSockets; uiIdx++)
		aryClientSockets.at (uiIdx)->Close ();

	wssServer->Close ();

	return (true);
}

// s13WXWClient
s13WXWClient::s13WXWClient ()
{
	socSocket = 0;
	usPort = 0;
	tcpParent = 0;
}

bool s13WXWClient::Init (unsigned short usPortPass)
{
	socSocket = new wxSocketClient ();

	/*if (socSocket->Ok () == false)
	{
		if (socSocket != 0)
		{
			delete socSocket;
			socSocket = 0;
		}

		return (false);
	}*/

	socSocket->SetEventHandler (*(RadXml::RadXML::getSingletonPtr ()->getWXApp ()), defSOCKET_SOCKET);
	socSocket->SetNotify (wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	void *vData = tcpParent->getHiddenData ("socket");
	socSocket->SetClientData (vData);
	socSocket->Notify (true);

	usPort = usPortPass;

	return (true);
}

bool s13WXWClient::ConnectWithBlocking (std::string strConnectTo)
{
	wxIPV4address wiaAddr;
	wiaAddr.Hostname (wxString::FromAscii (strConnectTo.c_str ()));
	wiaAddr.Service (usPort);
	socSocket->Connect (wiaAddr, false);
	socSocket->WaitOnConnect (10);

	if (socSocket->IsConnected () == false)
	{
		if (socSocket->Error () == true)
		{
		}

		socSocket->Close ();

		if (socSocket != 0)
		{
			delete socSocket;
			socSocket = 0;
		}

		return (false);
	}

	strServerIp = strConnectTo;

	return (true);
}

bool s13WXWClient::ConnectWithoutBlocking (std::string strConnectTo)
{
	wxIPV4address wiaAddr;
	wiaAddr.Hostname (wxString::FromAscii (strConnectTo.c_str ()));
	wiaAddr.Service (usPort);
	socSocket->Connect (wiaAddr, false);

	/*if (socSocket->IsConnected () == false)
	{
		if (socSocket->Error () == true)
		{
		}

		socSocket->Close ();

		if (socSocket != 0)
		{
			delete socSocket;
			socSocket = 0;
		}

		return (false);
	}*/

	strServerIp = strConnectTo;

	return (true);
}

bool s13WXWClient::SendFloat (float fFloat)
{
    socSocket->Write ((const char *)&fFloat, sizeof (float));

	if (socSocket->Error () == true)
	{
		Close ();

		return (false);
	}

	return (true);
}

bool s13WXWClient::SendDouble (double dDouble)
{
    socSocket->Write ((const char *)&dDouble, sizeof (double));

	if (socSocket->Error () == true)
	{
		Close ();

		return (false);
	}

	return (true);
}

bool s13WXWClient::SendInt (int iInt)
{
    socSocket->Write ((const char *)&iInt, sizeof (int));

	if (socSocket->Error () == true)
	{
		Close ();

		return (false);
	}

	return (true);
}

bool s13WXWClient::SendUInt (unsigned int uiInt)
{
    socSocket->Write ((const char *)&uiInt, sizeof (unsigned int));

	if (socSocket->Error () == true)
	{
		Close ();

		return (false);
	}

	return (true);
}

bool s13WXWClient::SendLong (long lLong)
{
    socSocket->Write ((const char *)&lLong, sizeof (long));

	if (socSocket->Error () == true)
	{
		Close ();

		return (false);
	}

	return (true);
}

bool s13WXWClient::SendULong (unsigned long ulLong)
{
    socSocket->Write ((const char *)&ulLong, sizeof (unsigned long));

	if (socSocket->Error () == true)
	{
		Close ();

		return (false);
	}

	return (true);
}

bool s13WXWClient::SendShort (short sShort)
{
    socSocket->Write ((const char *)&sShort, sizeof (short));

	if (socSocket->Error () == true)
	{
		Close ();

		return (false);
	}

	return (true);
}

bool s13WXWClient::SendUShort (unsigned short usShort)
{
    socSocket->Write ((const char *)&usShort, sizeof (unsigned short));

	if (socSocket->Error () == true)
	{
		Close ();

		return (false);
	}

	return (true);
}

bool s13WXWClient::SendChar (char cChar)
{
    socSocket->Write ((const char *)&cChar, sizeof (char));

	if (socSocket->Error () == true)
	{
		Close ();

		return (false);
	}

	return (true);
}

bool s13WXWClient::SendUChar (unsigned char ucChar)
{
    socSocket->Write ((const char *)&ucChar, sizeof (unsigned char));

	if (socSocket->Error () == true)
	{
		Close ();

		return (false);
	}

	return (true);
}

bool s13WXWClient::SendString (std::string strLine)
{
	char *cBuf = new char[(strLine.size () + 1)];
	const char *cCopy = strLine.c_str ();

	for (unsigned int iIdx = 0; iIdx < (strLine.size () + 1); iIdx++)
		cBuf[iIdx] = cCopy[iIdx];

	cBuf[strLine.size ()] = '\0';

	//ArrayZero (cBuf, (strLine.size () + 1));
	//ArrayCopy (((char *)strLine.c_str ()), cBuf, (strLine.size () + 1));

	socSocket->SetNotify (wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    socSocket->Write (cBuf, ((strLine.size () + 1) * sizeof (char)));
	socSocket->SetNotify (wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);

	delete []cBuf;
	cBuf = 0;

	if (socSocket->Error () == true)
	{
		Close ();

		return (false);
	}

	return (true);
}

float s13WXWClient::RecvFloat ()
{
    float fFloat = 0.0f;
	socSocket->Read ((char *)&fFloat, sizeof (float));

	if (socSocket->Error () == true)
	{
		Close ();

		return (0.0f);
	}

	return (fFloat);
}

double s13WXWClient::RecvDouble ()
{
    double dDouble = 0.0;
	socSocket->Read ((char *)&dDouble, sizeof (double));

	if (socSocket->Error () == true)
	{
		Close ();

		return (0.0f);
	}

	return (dDouble);
}

int s13WXWClient::RecvInt ()
{
    int iInt = 0;
	socSocket->Read ((char *)&iInt, sizeof (int));

	if (socSocket->Error () == true)
	{
		Close ();

		return (0);
	}

	return (iInt);
}

unsigned int s13WXWClient::RecvUInt ()
{
    unsigned int uiInt = 0;
	socSocket->Read ((char *)&uiInt, sizeof (unsigned int));

	if (socSocket->Error () == true)
	{
		Close ();

		return (0);
	}

	return (uiInt);
}

long s13WXWClient::RecvLong ()
{
    long lLong = 0;
	socSocket->Read ((char *)&lLong, sizeof (long));

	if (socSocket->Error () == true)
	{
		Close ();

		return (0);
	}

	return (lLong);
}

unsigned long s13WXWClient::RecvULong ()
{
    unsigned long ulLong = 0;
	socSocket->Read ((char *)&ulLong, sizeof (unsigned long));

	if (socSocket->Error () == true)
	{
		Close ();

		return (0);
	}

	return (ulLong);
}

short s13WXWClient::RecvShort ()
{
    short sShort = 0;
	socSocket->Read ((char *)&sShort, sizeof (short));

	if (socSocket->Error () == true)
	{
		Close ();

		return (0);
	}

	return (sShort);
}

unsigned short s13WXWClient::RecvUShort ()
{
    unsigned short usShort = 0;
	socSocket->Read ((char *)&usShort, sizeof (unsigned short));

	if (socSocket->Error () == true)
	{
		Close ();

		return (0);
	}

	return (usShort);
}

char s13WXWClient::RecvChar ()
{
    char cChar = 0x00;
	socSocket->Read ((char *)&cChar, sizeof (char));

	if (socSocket->Error () == true)
	{
		Close ();

		return (0x00);
	}

	return (cChar);
}

unsigned char s13WXWClient::RecvUChar ()
{
    unsigned char ucChar = 0;
	socSocket->Read ((char *)&ucChar, sizeof (unsigned char));

	if (socSocket->Error () == true)
	{
		Close ();

		return (0x00);
	}

	return (ucChar);
}

char *s13WXWClient::RecvString (int iLength)
{
	char *cString = new char [(iLength + 1)];
	//ArraySet<char> (cString, (iLength + 1), 0x00);

	for (unsigned int iIdx = 0; iIdx < (iLength + 1); iIdx++)
		cString[iIdx] = '\0';

	socSocket->Read (cString, iLength);

	if (socSocket->Error () == true)
	{
		Close ();

		return ("");
	}

	return (cString);
}

std::string s13WXWClient::RecvString ()
{
	std::string strReturn = "";
	char *cString = new char [(defNET_MAX_TEXT_SIZE + 1)];
	//ArraySet<char> (cString, (defNET_MAX_TEXT_SIZE + 1), 0x00);

	for (unsigned int iIdx = 0; iIdx < (defNET_MAX_TEXT_SIZE + 1); iIdx++)
		cString[iIdx] = '\0';

	socSocket->Read (cString, defNET_MAX_TEXT_SIZE);

	strReturn = cString;
	delete []cString;
	cString = 0;

	if (socSocket->Error () == true)
	{
		Close ();

		return ("");
	}

	return (strReturn);
}

void s13WXWClient::SetPort (unsigned short usPortPass)
{
	usPort = usPortPass;
}

unsigned short s13WXWClient::GetPort ()
{
	return (usPort);
}

void s13WXWClient::SetSocket (wxSocketClient *socSocketPass)
{
	socSocket = socSocketPass;
}

wxSocketClient *s13WXWClient::GetSocket ()
{
	return (socSocket);
}

std::string s13WXWClient::GetServerIP ()
{
	wxIPV4address ipAddr;
	socSocket->GetPeer (ipAddr);
	std::string strReturn = RadXmlCommon::convertwxStringToString (ipAddr.IPAddress ());

	return (strReturn);
}

std::string s13WXWClient::GetLastError ()
{
	return ("");
}

bool s13WXWClient::Close ()
{
	return (socSocket->Close ());
}
#endif
#endif

