#include "networking/net_global.h"
#include "networking/net_netclass.h"

#include "common/com_radxml_strconv.h"

#ifdef defNET_ON
void s13NetworkingClass::addHiddenData (std::string nameOfData, void *data)
{
	unsigned long ulHash = RadXmlCommon::s13StringToHash (nameOfData);
	void *vData = getHiddenData (ulHash);

	if (vData == 0)
		hshHidden.insert (std::pair<unsigned long, void *> (ulHash, data));
}

void *s13NetworkingClass::getHiddenData (unsigned long nameOfData)
{
	HASHMAP<unsigned long, void *>::iterator itBegin = hshHidden.find (nameOfData);
	HASHMAP<unsigned long, void *>::iterator itEnd = hshHidden.end ();

	if (itBegin != itEnd)
		return ((*itBegin).second);

	return (0);
}

void *s13NetworkingClass::getHiddenData (std::string nameOfData)
{
	unsigned long ulHash = RadXmlCommon::s13StringToHash (nameOfData);
	HASHMAP<unsigned long, void *>::iterator itBegin = hshHidden.find (ulHash);
	HASHMAP<unsigned long, void *>::iterator itEnd = hshHidden.end ();

	if (itBegin != itEnd)
		return ((*itBegin).second);

	return (0);
}

bool s13NetworkingClass::hasHiddenData (unsigned long nameOfData)
{
	HASHMAP<unsigned long, void *>::iterator itBegin = hshHidden.find (nameOfData);
	HASHMAP<unsigned long, void *>::iterator itEnd = hshHidden.end ();

	if (itBegin != itEnd)
		return (true);

	return (false);
}

bool s13NetworkingClass::hasHiddenData (std::string nameOfData)
{
	unsigned long ulHash = RadXmlCommon::s13StringToHash (nameOfData);
	HASHMAP<unsigned long, void *>::iterator itBegin = hshHidden.find (ulHash);
	HASHMAP<unsigned long, void *>::iterator itEnd = hshHidden.end ();

	if (itBegin != itEnd)
		return (true);

	return (false);
}

void s13NetworkingClass::removeHiddenData (std::string nameOfData)
{
	unsigned long ulHash = RadXmlCommon::s13StringToHash (nameOfData);
	HASHMAP<unsigned long, void *>::iterator itBegin = hshHidden.find (ulHash);
	HASHMAP<unsigned long, void *>::iterator itEnd = hshHidden.end ();

	if (itBegin != itEnd)
		hshHidden.erase (itBegin);
}

// s13Ipv4
s13Ipv4::s13Ipv4 ()
{
	for (unsigned short usIdx = 0; usIdx < 4; usIdx++)
		usOctets[usIdx] = 0;
}

s13Ipv4::s13Ipv4 (std::string strIp)
{
	GetIPfromString (strIp);
}

void s13Ipv4::GetIPfromString (std::string strLine)
{
	std::string strOctet = "";
	int iPos1 = 0, iPos2 = (int)strLine.find (".");

	strOctet = strLine.substr (iPos1, iPos2);
	usOctets[0] = RadXmlCommon::s13StringToInt (strOctet);

	iPos1 = (iPos2 + 1); iPos2 = (int)strLine.find (".", (iPos2 + 1));
	strOctet = strLine.substr (iPos1, iPos2);
	usOctets[1] = RadXmlCommon::s13StringToInt (strOctet);

	iPos1 = (iPos2 + 1); iPos2 = (int)strLine.find (".", (iPos2 + 1));
	strOctet = strLine.substr (iPos1, iPos2);
	usOctets[2] = RadXmlCommon::s13StringToInt (strOctet);

	iPos1 = (iPos2 + 1); iPos2 = (int)strLine.size ();
	strOctet = strLine.substr (iPos1, iPos2);
	usOctets[3] = RadXmlCommon::s13StringToInt (strOctet);
}

std::string s13Ipv4::GetString ()
{
    std::string strIpReturn = "";
	strIpReturn += RadXmlCommon::s13IntToString ((int)usOctets[0]);
	strIpReturn += ".";
	strIpReturn += RadXmlCommon::s13IntToString ((int)usOctets[1]);
	strIpReturn += ".";
	strIpReturn += RadXmlCommon::s13IntToString ((int)usOctets[2]);
	strIpReturn += ".";
	strIpReturn += RadXmlCommon::s13IntToString ((int)usOctets[3]);

	return (strIpReturn);
}

std::string s13Ipv4::GetString (s13Ipv4 ipIPpass)
{
	return (ipIPpass.GetString ());
}

s13Ipv4 s13Ipv4::GetIP (std::string strLine)
{
	s13Ipv4 ipReturn;
	ipReturn.GetIPfromString (strLine);
	return (ipReturn);
}

s13Ipv4 &s13Ipv4::operator= (std::string strIP)
{
	GetIPfromString (strIP);

	return (*this);
}

bool s13Ipv4::operator== (s13Ipv4 ipIp)
{
	for (short sIdx = 0; sIdx < 4; sIdx++)
	{
		if (usOctets[sIdx] != ipIp.usOctets[sIdx])
			return (false);
	}

	return (true);
}

bool s13Ipv4::operator== (std::string strIp)
{
	s13Ipv4 ipIp = strIp;

	for (short sIdx = 0; sIdx < 4; sIdx++)
	{
		if (usOctets[sIdx] != ipIp.usOctets[sIdx])
			return (false);
	}

	return (true);
}

// s13Ipv6
s13Ipv6::s13Ipv6 ()
{
	for (unsigned short usIdx = 0; usIdx < 8; usIdx++)
	{
		for (unsigned short usJdx = 0; usJdx < 4; usJdx++)
			ucOctets[usIdx][usJdx] = 0x00;
	}
}

// s13TcpipServer
s13TcpipServer::s13TcpipServer ()
{
	swsSocket = 0;
}
s13TcpipServer::~s13TcpipServer ()
{
	if (swsSocket != 0)
	{
		delete swsSocket;
		swsSocket = 0;
	}
}

bool s13TcpipServer::Init (unsigned short usPort)
{
	#ifdef defSYS_WIN32
		#ifndef defAPI_WXWIDGETS
			swsSocket = new s13WinSockServer ();
		#endif
	#endif

	#ifdef defAPI_WXWIDGETS
		swsSocket = new s13WXWServer ();
		swsSocket->setParent (this);
	#endif

	return (swsSocket->Init (usPort));
}

bool s13TcpipServer::AcceptWithBlocking ()
{
	bool bReturn = swsSocket->AcceptWithBlocking ();
	//aryIps.push_back (swsSocket->GetLastConnectedIp ());

	return (bReturn);
}

bool s13TcpipServer::AcceptWithoutBlocking ()
{
	bool bReturn = swsSocket->AcceptWithoutBlocking ();
	//aryIps.push_back (swsSocket->GetLastConnectedIp ());

	return (bReturn);
}

bool s13TcpipServer::SendFloatToAll (float fFloat, std::vector<unsigned int> aryExcept)
{
	return (swsSocket->SendFloatToAll (fFloat, aryExcept));
}

bool s13TcpipServer::SendFloat (s13Ipv4 ipIp, float fFloat)
{
	#ifdef defSYS_WIN32
		#ifndef defAPI_WXWIDGETS
			std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
			std::vector<s13Ipv4> aryIps;
			std::vector<SOCKET> arySockets = swsSocket->GetClientSockets ();

			aryIps.resize (aryIpStrings.size ());

			for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
				aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

			for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
			{
				if (aryIps.at (uiIdx) == ipIp)
					return (swsSocket->SendFloat (arySockets.at (uiIdx), fFloat));
			}

			return (false);
		#endif
	#endif

	#ifdef defAPI_WXWIDGETS
		std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
		std::vector<s13Ipv4> aryIps;
		std::vector<wxSocketBase *> arySockets = swsSocket->GetClientSockets ();

		aryIps.resize (aryIpStrings.size ());

		for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
			aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

		for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
		{
			if (aryIps.at (uiIdx) == ipIp)
				return (swsSocket->SendFloat (arySockets.at (uiIdx), fFloat));
		}

		return (false);
	#endif
}

bool s13TcpipServer::SendFloat (unsigned int uiId, float fFloat)
{
	return (swsSocket->SendFloat (swsSocket->GetClientSocket (uiId), fFloat));
}

bool s13TcpipServer::SendDoubleToAll (double dDouble, std::vector<unsigned int> aryExcept)
{
	return (swsSocket->SendDoubleToAll (dDouble, aryExcept));
}

bool s13TcpipServer::SendDouble (s13Ipv4 ipIp, double dDouble)
{
	#ifdef defSYS_WIN32
		#ifndef defAPI_WXWIDGETS
			std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
			std::vector<s13Ipv4> aryIps;
			std::vector<SOCKET> arySockets = swsSocket->GetClientSockets ();

			aryIps.resize (aryIpStrings.size ());

			for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
				aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

			for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
			{
				if (aryIps.at (uiIdx) == ipIp)
					return (swsSocket->SendDouble (arySockets.at (uiIdx), dDouble));
			}

			return (false);
		#endif
	#endif

	#ifdef defAPI_WXWIDGETS
		std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
		std::vector<s13Ipv4> aryIps;
		std::vector<wxSocketBase *> arySockets = swsSocket->GetClientSockets ();

		aryIps.resize (aryIpStrings.size ());

		for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
			aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

		for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
		{
			if (aryIps.at (uiIdx) == ipIp)
				return (swsSocket->SendDouble (arySockets.at (uiIdx), dDouble));
		}

		return (false);
	#endif
}

bool s13TcpipServer::SendDouble (unsigned int uiId, double dDouble)
{
	return (swsSocket->SendDouble (swsSocket->GetClientSocket (uiId), dDouble));
}

bool s13TcpipServer::SendIntToAll (int iInt, std::vector<unsigned int> aryExcept)
{
	return (swsSocket->SendIntToAll (iInt, aryExcept));
}

bool s13TcpipServer::SendInt (s13Ipv4 ipIp, int iInt)
{
	#ifdef defSYS_WIN32
		#ifndef defAPI_WXWIDGETS
			std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
			std::vector<s13Ipv4> aryIps;
			std::vector<SOCKET> arySockets = swsSocket->GetClientSockets ();

			aryIps.resize (aryIpStrings.size ());

			for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
				aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

			for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
			{
				if (aryIps.at (uiIdx) == ipIp)
					return (swsSocket->SendInt (arySockets.at (uiIdx), iInt));
			}

			return (false);
		#endif
	#endif

	#ifdef defAPI_WXWIDGETS
		std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
		std::vector<s13Ipv4> aryIps;
		std::vector<wxSocketBase *> arySockets = swsSocket->GetClientSockets ();

		aryIps.resize (aryIpStrings.size ());

		for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
			aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

		for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
		{
			if (aryIps.at (uiIdx) == ipIp)
				return (swsSocket->SendInt (arySockets.at (uiIdx), iInt));
		}

		return (false);
	#endif
}

bool s13TcpipServer::SendInt (unsigned int uiId, int iInt)
{
	return (swsSocket->SendInt (swsSocket->GetClientSocket (uiId), iInt));
}

bool s13TcpipServer::SendUIntToAll (unsigned int uiInt, std::vector<unsigned int> aryExcept)
{
	return (swsSocket->SendUIntToAll (uiInt, aryExcept));
}

bool s13TcpipServer::SendUInt (s13Ipv4 ipIp, unsigned int uiInt)
{
	#ifdef defSYS_WIN32
		#ifndef defAPI_WXWIDGETS
			std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
			std::vector<s13Ipv4> aryIps;
			std::vector<SOCKET> arySockets = swsSocket->GetClientSockets ();

			aryIps.resize (aryIpStrings.size ());

			for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
				aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

			for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
			{
				if (aryIps.at (uiIdx) == ipIp)
					return (swsSocket->SendUInt (arySockets.at (uiIdx), uiInt));
			}

			return (false);
		#endif
	#endif

	#ifdef defAPI_WXWIDGETS
		std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
		std::vector<s13Ipv4> aryIps;
		std::vector<wxSocketBase *> arySockets = swsSocket->GetClientSockets ();

		aryIps.resize (aryIpStrings.size ());

		for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
			aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

		for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
		{
			if (aryIps.at (uiIdx) == ipIp)
				return (swsSocket->SendUInt (arySockets.at (uiIdx), uiInt));
		}

		return (false);
	#endif
}

bool s13TcpipServer::SendUInt (unsigned int uiId, unsigned int uiInt)
{
	return (swsSocket->SendUInt (swsSocket->GetClientSocket (uiId), uiInt));
}

bool s13TcpipServer::SendLongToAll (long lLong, std::vector<unsigned int> aryExcept)
{
	return (swsSocket->SendLongToAll (lLong, aryExcept));
}

bool s13TcpipServer::SendLong (s13Ipv4 ipIp, long lLong)
{
	#ifdef defSYS_WIN32
		#ifndef defAPI_WXWIDGETS
			std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
			std::vector<s13Ipv4> aryIps;
			std::vector<SOCKET> arySockets = swsSocket->GetClientSockets ();

			aryIps.resize (aryIpStrings.size ());

			for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
				aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

			for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
			{
				if (aryIps.at (uiIdx) == ipIp)
					return (swsSocket->SendLong (arySockets.at (uiIdx), lLong));
			}

			return (false);
		#endif
	#endif

	#ifdef defAPI_WXWIDGETS
		std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
		std::vector<s13Ipv4> aryIps;
		std::vector<wxSocketBase *> arySockets = swsSocket->GetClientSockets ();

		aryIps.resize (aryIpStrings.size ());

		for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
			aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

		for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
		{
			if (aryIps.at (uiIdx) == ipIp)
				return (swsSocket->SendLong (arySockets.at (uiIdx), lLong));
		}

		return (false);
	#endif
}

bool s13TcpipServer::SendLong (unsigned int uiId, long lLong)
{
	return (swsSocket->SendLong (swsSocket->GetClientSocket (uiId), lLong));
}

bool s13TcpipServer::SendULongToAll (unsigned long ulLong, std::vector<unsigned int> aryExcept)
{
	return (swsSocket->SendULongToAll (ulLong, aryExcept));
}

bool s13TcpipServer::SendULong (s13Ipv4 ipIp, unsigned long ulLong)
{
	#ifdef defSYS_WIN32
		#ifndef defAPI_WXWIDGETS
			std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
			std::vector<s13Ipv4> aryIps;
			std::vector<SOCKET> arySockets = swsSocket->GetClientSockets ();

			aryIps.resize (aryIpStrings.size ());

			for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
				aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

			for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
			{
				if (aryIps.at (uiIdx) == ipIp)
					return (swsSocket->SendULong (arySockets.at (uiIdx), ulLong));
			}

			return (false);
		#endif
	#endif

	#ifdef defAPI_WXWIDGETS
		std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
		std::vector<s13Ipv4> aryIps;
		std::vector<wxSocketBase *> arySockets = swsSocket->GetClientSockets ();

		aryIps.resize (aryIpStrings.size ());

		for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
			aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

		for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
		{
			if (aryIps.at (uiIdx) == ipIp)
				return (swsSocket->SendULong (arySockets.at (uiIdx), ulLong));
		}

		return (false);
	#endif
}

bool s13TcpipServer::SendULong (unsigned int uiId, unsigned long ulLong)
{
	return (swsSocket->SendULong (swsSocket->GetClientSocket (uiId), ulLong));
}

bool s13TcpipServer::SendShortToAll (short sShort, std::vector<unsigned int> aryExcept)
{
	return (swsSocket->SendShortToAll (sShort, aryExcept));
}

bool s13TcpipServer::SendShort (s13Ipv4 ipIp, short sShort)
{
	#ifdef defSYS_WIN32
		#ifndef defAPI_WXWIDGETS
			std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
			std::vector<s13Ipv4> aryIps;
			std::vector<SOCKET> arySockets = swsSocket->GetClientSockets ();

			aryIps.resize (aryIpStrings.size ());

			for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
				aryIps.at (uiIdx) = aryIpStrings[uiIdx];

			for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
			{
				if (aryIps.at (uiIdx) == ipIp)
					return (swsSocket->SendShort (arySockets.at (uiIdx), sShort));
			}

			return (false);
		#endif
	#endif

	#ifdef defAPI_WXWIDGETS
		std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
		std::vector<s13Ipv4> aryIps;
		std::vector<wxSocketBase *> arySockets = swsSocket->GetClientSockets ();

		aryIps.resize (aryIpStrings.size ());

		for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
			aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

		for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
		{
			if (aryIps.at (uiIdx) == ipIp)
				return (swsSocket->SendShort (arySockets.at (uiIdx), sShort));
		}

		return (false);
	#endif
}

bool s13TcpipServer::SendShort (unsigned int uiId, short sShort)
{
	return (swsSocket->SendShort (swsSocket->GetClientSocket (uiId), sShort));
}

bool s13TcpipServer::SendUShortToAll (unsigned short usShort, std::vector<unsigned int> aryExcept)
{
	return (swsSocket->SendUShortToAll (usShort, aryExcept));
}

bool s13TcpipServer::SendUShort (s13Ipv4 ipIp, unsigned short usShort)
{
	#ifdef defSYS_WIN32
		#ifndef defAPI_WXWIDGETS
			std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
			std::vector<s13Ipv4> aryIps;
			std::vector<SOCKET> arySockets = swsSocket->GetClientSockets ();

			aryIps.resize (aryIpStrings.size ());

			for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
				aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

			for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
			{
				if (aryIps.at (uiIdx) == ipIp)
					return (swsSocket->SendUShort (arySockets.at (uiIdx), usShort));
			}

			return (false);
		#endif
	#endif

	#ifdef defAPI_WXWIDGETS
		std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
		std::vector<s13Ipv4> aryIps;
		std::vector<wxSocketBase *> arySockets = swsSocket->GetClientSockets ();

		aryIps.resize (aryIpStrings.size ());

		for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
			aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

		for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
		{
			if (aryIps.at (uiIdx) == ipIp)
				return (swsSocket->SendUShort (arySockets.at (uiIdx), usShort));
		}

		return (false);
	#endif
}

bool s13TcpipServer::SendUShort (unsigned int uiId, unsigned short usShort)
{
	return (swsSocket->SendUShort (swsSocket->GetClientSocket (uiId), usShort));
}

bool s13TcpipServer::SendCharToAll (char cChar, std::vector<unsigned int> aryExcept)
{
	return (swsSocket->SendCharToAll (cChar, aryExcept));
}

bool s13TcpipServer::SendChar (s13Ipv4 ipIp, char cChar)
{
	#ifdef defSYS_WIN32
		#ifndef defAPI_WXWIDGETS
			std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
			std::vector<s13Ipv4> aryIps;
			std::vector<SOCKET> arySockets = swsSocket->GetClientSockets ();

			aryIps.resize (aryIpStrings.size ());

			for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
				aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

			for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
			{
				if (aryIps.at (uiIdx) == ipIp)
					return (swsSocket->SendChar (arySockets.at (uiIdx), cChar));
			}

			return (false);
		#endif
	#endif

	#ifdef defAPI_WXWIDGETS
		std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
		std::vector<s13Ipv4> aryIps;
		std::vector<wxSocketBase *> arySockets = swsSocket->GetClientSockets ();

		aryIps.resize (aryIpStrings.size ());

		for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
			aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

		for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
		{
			if (aryIps.at (uiIdx) == ipIp)
				return (swsSocket->SendChar (arySockets.at (uiIdx), cChar));
		}

		return (false);
	#endif
}

bool s13TcpipServer::SendChar (unsigned int uiId, char cChar)
{
	return (swsSocket->SendChar (swsSocket->GetClientSocket (uiId), cChar));
}

bool s13TcpipServer::SendUCharToAll (unsigned char ucChar, std::vector<unsigned int> aryExcept)
{
	return (swsSocket->SendUCharToAll (ucChar, aryExcept));
}

bool s13TcpipServer::SendUChar (s13Ipv4 ipIp, unsigned char ucChar)
{
	#ifdef defSYS_WIN32
		#ifndef defAPI_WXWIDGETS
			std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
			std::vector<s13Ipv4> aryIps;
			std::vector<SOCKET> arySockets = swsSocket->GetClientSockets ();

			aryIps.resize (aryIpStrings.size ());

			for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
				aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

			for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
			{
				if (aryIps.at (uiIdx) == ipIp)
					return (swsSocket->SendUChar (arySockets.at (uiIdx), ucChar));
			}

			return (false);
		#endif
	#endif

	#ifdef defAPI_WXWIDGETS
		std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
		std::vector<s13Ipv4> aryIps;
		std::vector<wxSocketBase *> arySockets = swsSocket->GetClientSockets ();

		aryIps.resize (aryIpStrings.size ());

		for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
			aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

		for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
		{
			if (aryIps.at (uiIdx) == ipIp)
				return (swsSocket->SendUChar (arySockets.at (uiIdx), ucChar));
		}

		return (false);
	#endif
}

bool s13TcpipServer::SendUChar (unsigned int uiId, unsigned char ucChar)
{
	return (swsSocket->SendUChar (swsSocket->GetClientSocket (uiId), ucChar));
}

bool s13TcpipServer::SendStringToAll (std::string strLine, std::vector<unsigned int> aryExcept)
{
	return (swsSocket->SendStringToAll (strLine, aryExcept));
}

// FIXME: This doesn't work properly...
bool s13TcpipServer::SendString (s13Ipv4 ipIp, std::string strLine)
{
	#ifdef defSYS_WIN32
		#ifndef defAPI_WXWIDGETS
			std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
			std::vector<s13Ipv4> aryIps;
			std::vector<SOCKET> arySockets = swsSocket->GetClientSockets ();

			aryIps.resize (aryIpStrings.size ());

			for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
				aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

			for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
			{
				if (aryIps.at (uiIdx) == ipIp)
					return (swsSocket->SendString (arySockets.at (uiIdx), strLine));
			}

			return (false);
		#endif
	#endif

	#ifdef defAPI_WXWIDGETS
		std::vector<std::string> aryIpStrings = swsSocket->GetConnectedIPs ();
		std::vector<s13Ipv4> aryIps;
		std::vector<wxSocketBase *> arySockets = swsSocket->GetClientSockets ();

		aryIps.resize (aryIpStrings.size ());

		for (unsigned int uiIdx = 0; uiIdx < aryIps.size (); uiIdx++)
			aryIps.at (uiIdx) = aryIpStrings.at (uiIdx);

		for (unsigned int uiIdx = 0; uiIdx < arySockets.size (); uiIdx++)
		{
			if (aryIps.at (uiIdx) == ipIp)
				return (swsSocket->SendString (arySockets.at (uiIdx), strLine));
		}

		return (false);
	#endif
}

bool s13TcpipServer::SendString (unsigned int uiId, std::string strLine)
{
	return (swsSocket->SendString (swsSocket->GetClientSocket (uiId), strLine));
}

std::vector<float> s13TcpipServer::RecvFloat ()
{
	return (swsSocket->RecvFloat ());
}

std::vector<double> s13TcpipServer::RecvDouble ()
{
	return (swsSocket->RecvDouble ());
}

std::vector<int> s13TcpipServer::RecvInt ()
{
	return (swsSocket->RecvInt ());
}

std::vector<unsigned int> s13TcpipServer::RecvUInt ()
{
	return (swsSocket->RecvUInt ());
}

std::vector<long> s13TcpipServer::RecvLong ()
{
	return (swsSocket->RecvLong ());
}

std::vector<unsigned long> s13TcpipServer::RecvULong ()
{
	return (swsSocket->RecvULong ());
}

std::vector<short> s13TcpipServer::RecvShort ()
{
	return (swsSocket->RecvShort ());
}

std::vector<unsigned short> s13TcpipServer::RecvUShort ()
{
	return (swsSocket->RecvUShort ());
}

std::vector<char> s13TcpipServer::RecvChar ()
{
	return (swsSocket->RecvChar ());
}

std::vector<unsigned char> s13TcpipServer::RecvUChar ()
{
	return (swsSocket->RecvUChar ());
}

std::vector<char *> s13TcpipServer::RecvString (int iLength)
{
	return (swsSocket->RecvString (iLength));
}

std::vector<std::string> s13TcpipServer::RecvString ()
{
	return (swsSocket->RecvString ());
}

#if (defNET_IPVERS == 1)
	std::vector<s13Ipv4> s13TcpipServer::GetConnectedIPs ()
#elif (defNET_IPVERS == 2)
	std::vector<s13Ipv6> s13TcpipServer::GetConnectedIPs ()
#endif
{
	return (aryIps);
}

#if (defNET_IPVERS == 1)
	s13Ipv4 s13TcpipServer::GetConnectedIP (unsigned int uiId)
#elif (defNET_IPVERS == 2)
	s13Ipv6 s13TcpipServer::GetConnectedIP (unsigned int uiId)
#endif
{
	return (aryIps.at (uiId));
}

#if (defNET_IPVERS == 1)
	s13Ipv4 s13TcpipServer::GetLastConnectedIp ()
#elif (defNET_IPVERS == 2)
	s13Ipv6 s13TcpipServer::GetLastConnectedIp ()
#endif
{
	return (aryIps.at ((aryIps.size () - 1)));
}

std::string s13TcpipServer::GetConnectedIpString (unsigned int uiId)
{
	return (swsSocket->GetConnectedIp (uiId));
}

std::string s13TcpipServer::GetLastConnectedIpString ()
{
	return (swsSocket->GetLastConnectedIp ());
}

unsigned int s13TcpipServer::GetNumConnected ()
{
	return (aryIps.size ());
}

unsigned short s13TcpipServer::GetPort ()
{
	return (swsSocket->GetPort ());
}

std::string s13TcpipServer::GetLastError ()
{
	return (swsSocket->GetLastError ());
}

std::vector<unsigned int> s13TcpipServer::GetLastReceivedFromList ()
{
	return (swsSocket->GetLastReceivedFromList ());
}

bool s13TcpipServer::IsSocketOpen (unsigned int uiId)
{
	return (swsSocket->IsSocketOpen (swsSocket->GetClientSocket (uiId)));
}

bool s13TcpipServer::CloseClient (unsigned int uiId)
{
	return (swsSocket->CloseClientSocket (swsSocket->GetClientSocket (uiId)));
}

bool s13TcpipServer::Close ()
{
	return (swsSocket->CloseAllSockets ());
}

s13NetworkingClass *s13TcpipServer::GetNetworkingClass ()
{
	return (swsSocket);
}

// s13TcpipClient
s13TcpipClient::s13TcpipClient ()
{
	swcSocket = 0;
	strHTTPserverAddress = "";
}

s13TcpipClient::~s13TcpipClient ()
{
	if (swcSocket != 0)
	{
		delete swcSocket;
		swcSocket = 0;
	}
}

bool s13TcpipClient::Init (unsigned short usPort)
{
	#ifdef defSYS_WIN32
		#ifndef defAPI_WXWIDGETS
			swcSocket = new s13WinSockClient ();
		#endif
	#endif

	#ifdef defAPI_WXWIDGETS
		swcSocket = new s13WXWClient ();
		swcSocket->setParent (this);
	#endif

	return (swcSocket->Init (usPort));
}

bool s13TcpipClient::ConnectWithBlocking (std::string strConnectTo)
{
	bool bReturn = swcSocket->ConnectWithBlocking (strConnectTo);

	if (bReturn == false)
		return (bReturn);

	ipServerIp = swcSocket->GetServerIP ();

	if (bIsStringIPAddressOnly (strConnectTo.c_str (), strConnectTo.size ()) == false)
		strHTTPserverAddress = strConnectTo;

	return (bReturn);
}

bool s13TcpipClient::ConnectWithoutBlocking (std::string strConnectTo)
{
	bool bReturn = swcSocket->ConnectWithoutBlocking (strConnectTo);

	if (bReturn == false)
		return (bReturn);

	ipServerIp = swcSocket->GetServerIP ();

	if (bIsStringIPAddressOnly (strConnectTo.c_str (), strConnectTo.size ()) == false)
		strHTTPserverAddress = strConnectTo;

	return (bReturn);
}

bool s13TcpipClient::SendFloat (float fFloat)
{
	return (swcSocket->SendFloat (fFloat));
}

bool s13TcpipClient::SendDouble (double dDouble)
{
	return (swcSocket->SendDouble (dDouble));
}

bool s13TcpipClient::SendInt (int iInt)
{
	return (swcSocket->SendInt (iInt));
}

bool s13TcpipClient::SendUInt (unsigned int uiInt)
{
	return (swcSocket->SendUInt (uiInt));
}

bool s13TcpipClient::SendLong (long lLong)
{
	return (swcSocket->SendLong (lLong));
}

bool s13TcpipClient::SendULong (unsigned long ulLong)
{
	return (swcSocket->SendULong (ulLong));
}

bool s13TcpipClient::SendShort (short sShort)
{
	return (swcSocket->SendShort (sShort));
}

bool s13TcpipClient::SendUShort (unsigned short usShort)
{
	return (swcSocket->SendUShort (usShort));
}

bool s13TcpipClient::SendChar (char cChar)
{
	return (swcSocket->SendChar (cChar));
}

bool s13TcpipClient::SendUChar (unsigned char ucChar)
{
	return (swcSocket->SendUChar (ucChar));
}

bool s13TcpipClient::SendString (std::string strLine)
{
	return (swcSocket->SendString (strLine));
}

bool s13TcpipClient::HTTP_SendGET (std::string strRetrieve, std::string strHeaders)
{
	bool bReturn = false;

	std::string strSendString = "GET " + strRetrieve;

	if (strHeaders != "")
		strSendString += "?" + strHeaders;

	strSendString += " HTTP/1.1\n";

	if (strHTTPserverAddress == "")
		strSendString += "Host: " + ipServerIp.GetString () + ":" + (std::string)RadXmlCommon::s13IntToString (GetPort ()) + "\n";
	else
		strSendString += "Host: " + strHTTPserverAddress + ":" + (std::string)RadXmlCommon::s13IntToString (GetPort ()) + "\n";

	strSendString += "User-Agent: " + (std::string)defNET_USERAGENT + "\n";
	strSendString += "\n";

	bReturn = swcSocket->SendString (strSendString);

	return (bReturn);
}

bool s13TcpipClient::HTTP_SendPOST (std::string strRetrieve, std::string strLine, std::string strHeaders, std::string strAuth)
{
	return (HTTP_SendPOST ("application/x-www-form-urlencoded", strRetrieve, strLine, strHeaders, strAuth));
}

bool s13TcpipClient::HTTP_SendPOST (std::string strContentType, std::string strRetrieve, std::string strLine, std::string strHeaders, std::string strAuth)
{
	bool bReturn = false;

	std::string strSendString = ("POST " + strRetrieve);

	if (strHeaders != "")
		strSendString += "?" + strHeaders;

	strSendString += " HTTP/1.1\n";

	if (strAuth != "")
		strSendString += "Authorization: " + strAuth;

	if (strHTTPserverAddress == "")
		strSendString += "Host: " + ipServerIp.GetString () + ":" + (std::string)RadXmlCommon::s13IntToString (GetPort ()) + "\n";
	else
		strSendString += "Host: " + strHTTPserverAddress + ":" + (std::string)RadXmlCommon::s13IntToString (GetPort ()) + "\n";

	strSendString += "User-Agent: " + (std::string)defNET_USERAGENT + "\n";
	strSendString += "Content-Type: " + strContentType + "\n";
	strSendString += "Content-Length: " + (std::string)RadXmlCommon::s13IntToString ((unsigned int)strLine.size ()) + "\n";
	strSendString += "\n";
	strSendString += strLine;
	strSendString += "\n";

	bReturn = swcSocket->SendString (strSendString);

	return (bReturn);
}

std::string s13TcpipClient::HTTP_ParseRequest (std::string strContent, std::string strNewLine)
{
	std::string strReturn = strContent;
	std::string strTemp = strReturn;

	for (int iIdx = 0; iIdx < (int)strTemp.size (); iIdx++)
		strTemp.at (iIdx) = tolower (strTemp.at (iIdx));

	int iStartPos = strTemp.find ((strNewLine + strNewLine));
	iStartPos += (strNewLine.size () * 2);
	int iEndPos = strTemp.find (strNewLine, (iStartPos + 1));
	strTemp = strTemp.substr (iStartPos, (iEndPos - iStartPos));
	iEndPos += strNewLine.size ();
	int iVeryEndPos = strReturn.find ("0" + strNewLine + strNewLine);
	int iCount = RadXmlCommon::s13StringToInt (strTemp);

	strReturn = strReturn.substr (iEndPos, (iVeryEndPos - (iEndPos + strNewLine.size ())));

	return (strReturn);
}

float s13TcpipClient::RecvFloat ()
{
	return (swcSocket->RecvFloat ());
}

double s13TcpipClient::RecvDouble ()
{
	return (swcSocket->RecvDouble ());
}

int s13TcpipClient::RecvInt ()
{
	return (swcSocket->RecvInt ());
}

unsigned int s13TcpipClient::RecvUInt ()
{
	return (swcSocket->RecvUInt ());
}

long s13TcpipClient::RecvLong ()
{
	return (swcSocket->RecvLong ());
}

unsigned long s13TcpipClient::RecvULong ()
{
	return (swcSocket->RecvULong ());
}

short s13TcpipClient::RecvShort ()
{
	return (swcSocket->RecvShort ());
}

unsigned short s13TcpipClient::RecvUShort ()
{
	return (swcSocket->RecvUShort ());
}

char s13TcpipClient::RecvChar ()
{
	return (swcSocket->RecvChar ());
}

unsigned char s13TcpipClient::RecvUChar ()
{
	return (swcSocket->RecvUChar ());
}

char *s13TcpipClient::RecvString (int iLength)
{
	return (swcSocket->RecvString (iLength));
}

std::string s13TcpipClient::RecvString ()
{
	return (swcSocket->RecvString ());
}

#if (defNET_IPVERS == 1)
	s13Ipv4 s13TcpipClient::GetServerIP ()
#elif (defNET_IPVERS == 2)
	s13Ipv6 s13TcpipClient::GetServerIP ()
#endif
{
	return (ipServerIp);
}

unsigned short s13TcpipClient::GetPort ()
{
	return (swcSocket->GetPort ());
}

std::string s13TcpipClient::GetLastError ()
{
	return (swcSocket->GetLastError ());
}

bool s13TcpipClient::Close ()
{
	return (swcSocket->Close ());
}

s13NetworkingClass *s13TcpipClient::GetNetworkingClass ()
{
	return (swcSocket);
}
#endif

