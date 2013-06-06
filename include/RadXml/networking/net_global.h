#ifndef _RADXML_NET_GLOBAL_H_
	#define _RADXML_NET_GLOBAL_H_

	#ifdef defNET_ON
		#include <vector>
		#include <string>

		#ifndef defNET_IPVERS
			#define defNET_IPVERS			1
		#endif

		#ifndef defNET_USERAGENT
			#define defNET_USERAGENT		"RADXML/0.1"
		#endif

		enum
		{
			defNET_HANDLE_TYPE_READ			= 1, 
			defNET_HANDLE_TYPE_WRITE		= 2, 
			defNET_HANDLE_TYPE_ACCEPT		= 3, 
			defNET_HANDLE_TYPE_CONNECT		= 4, 
			defNET_HANDLE_TYPE_CLOSE		= 5,
			defNET_HANDLE_TYPE_ERROR		= 6
		};

		#ifdef defSYS_WIN32
			#ifndef defAPI_WXWIDGETS
				#include "../win32/networking/tcpip/tcp_global.h"
			#endif
		#endif

		#ifdef defSYS_LINUX
			#ifndef defAPI_WXWIDGETS
				#include "../linux/networking/tcpip/tcp_global.h"
			#endif
		#endif

		#ifdef defSYS_MACOSX
			#ifndef defAPI_WXWIDGETS
				#include "../macosx/networking/tcpip/tcp_global.h"
			#endif
		#endif

		#ifdef defAPI_WXWIDGETS
			#ifdef USE_RADXML_DIR
				#include "RadXml/wxwidgets/RadXmlWxWidgetsNetworking.h"
			#else
				#include "wxwidgets/RadXmlWxWidgetsNetworking.h"
			#endif
		#endif

		class s13Ipv4
		{
			public:
				s13Ipv4 ();

				s13Ipv4 (std::string strIp);

				void GetIPfromString (std::string strLine);
				std::string GetString ();
				static std::string GetString (s13Ipv4 ipIPpass);
				static s13Ipv4 GetIP (std::string strLine);

				s13Ipv4 &operator= (std::string strIP);
				bool operator== (s13Ipv4 ipIp);
				bool operator== (std::string strIp);

				unsigned short usOctets[4];
		};

		class s13Ipv6
		{
			public:
				s13Ipv6 ();

				unsigned char ucOctets[8][4];
		};

		inline std::vector<std::string> Net_GetComputerIPs ()
		{
			#ifdef defSYS_WIN32
				#ifndef defAPI_WXWIDGETS
					return (WinSock_GetComputerIPs ());
				#endif
			#endif

			#ifdef defAPI_WXWIDGETS
				std::vector<std::string> aryReturn;
				return (aryReturn);
			#endif
		}

		inline static std::vector< std::vector<unsigned long> > Net_GetMACAddresses ()
		{
			#ifdef defSYS_WIN32
				#ifndef defAPI_WXWIDGETS
					return (WinSock_GetMACAddresses ());
				#endif
			#endif

			#ifdef defAPI_WXWIDGETS
				std::vector< std::vector<unsigned long> > aryReturn;
				return (aryReturn);
			#endif
		}

		class s13TcpipServer:public s13NetworkingClass
		{
			public:
				s13TcpipServer ();
				~s13TcpipServer ();

				bool Init (unsigned short usPort);

				bool AcceptWithBlocking ();
				bool AcceptWithoutBlocking ();

                bool SendFloatToAll (float fFloat, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
                bool SendFloat (s13Ipv4 ipIp, float fFloat);
                bool SendFloat (unsigned int uiId, float fFloat);
                bool SendDoubleToAll (double dDouble, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
                bool SendDouble (s13Ipv4 ipIp, double dDouble);
                bool SendDouble (unsigned int uiId, double dDouble);
                bool SendIntToAll (int iInt, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
                bool SendInt (s13Ipv4 ipIp, int iInt);
                bool SendInt (unsigned int uiId, int iInt);
                bool SendUIntToAll (unsigned int uiInt, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
                bool SendUInt (s13Ipv4 ipIp, unsigned int uiInt);
                bool SendUInt (unsigned int uiId, unsigned int uiInt);
                bool SendLongToAll (long lLong, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
                bool SendLong (s13Ipv4 ipIp, long lLong);
                bool SendLong (unsigned int uiId, long lLong);
                bool SendULongToAll (unsigned long ulLong, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
                bool SendULong (s13Ipv4 ipIp, unsigned long ulLong);
                bool SendULong (unsigned int uiId, unsigned long ulLong);
                bool SendShortToAll (short sShort, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
                bool SendShort (s13Ipv4 ipIp, short sShort);
                bool SendShort (unsigned int uiId, short sShort);
                bool SendUShortToAll (unsigned short usShort, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
                bool SendUShort (s13Ipv4 ipIp, unsigned short usShort);
                bool SendUShort (unsigned int uiId, unsigned short usShort);
                bool SendCharToAll (char cChar, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
                bool SendChar (s13Ipv4 ipIp, char cChar);
                bool SendChar (unsigned int uiId, char cChar);
                bool SendUCharToAll (unsigned char ucChar, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
                bool SendUChar (s13Ipv4 ipIp, unsigned char ucChar);
                bool SendUChar (unsigned int uiId, unsigned char ucChar);
				bool SendStringToAll (std::string strLine, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
				bool SendString (s13Ipv4 ipIp, std::string strLine);
				bool SendString (unsigned int uiId, std::string strLine);

                std::vector<float> RecvFloat ();
                std::vector<double> RecvDouble ();
                std::vector<int> RecvInt ();
                std::vector<unsigned int> RecvUInt ();
                std::vector<long> RecvLong ();
                std::vector<unsigned long> RecvULong ();
                std::vector<short> RecvShort ();
                std::vector<unsigned short> RecvUShort ();
                std::vector<char> RecvChar ();
                std::vector<unsigned char> RecvUChar ();
				std::vector<char *> RecvString (int iLength);
				std::vector<std::string> RecvString ();

				#if (defNET_IPVERS == 1)
					std::vector<s13Ipv4> GetConnectedIPs ();
					s13Ipv4 GetConnectedIP (unsigned int uiId);
					s13Ipv4 GetLastConnectedIp ();
				#elif (defNET_IPVERS == 2)
					std::vector<s13Ipv6> GetConnectedIPs ();
					s13Ipv6 GetConnectedIP (unsigned int uiId);
					s13Ipv6 GetLastConnectedIp ();
				#endif
				std::string GetConnectedIpString (unsigned int uiId);
				std::string GetLastConnectedIpString ();
				unsigned int GetNumConnected ();

				unsigned short GetPort ();

				std::string GetLastError ();

				std::vector<unsigned int> GetLastReceivedFromList ();

				bool IsSocketOpen (unsigned int uiId);

				bool CloseClient (unsigned int uiId);
				bool Close ();

				s13NetworkingClass *GetNetworkingClass ();

				#ifdef defSPUZZLE
					s13CallBack<s13PuzzleTCPIP, void> *scbCallback;
				#endif

			private:
				#if (defNET_IPVERS == 1)
					std::vector<s13Ipv4> aryIps;
				#elif (defNET_IPVERS == 2)
					std::vector<s13Ipv6> aryIps;
				#else
					#error "You must specify a valid IP-Protocol version setting with definition: defNET_IPVERS."
				#endif

				std::vector<unsigned int> aryReceivedFrom;

				#ifdef defSYS_WIN32
					#ifndef defAPI_WXWIDGETS
						s13WinSockServer *swsSocket;
					#endif
				#endif

				#ifdef defAPI_WXWIDGETS
					s13WXWServer *swsSocket;
				#endif
		};

		class s13TcpipClient:public s13NetworkingClass
		{
			public:
				s13TcpipClient ();
				~s13TcpipClient ();

				bool Init (unsigned short usPort);

				bool ConnectWithBlocking (std::string strConnectTo);
				bool ConnectWithoutBlocking (std::string strConnectTo);

                bool SendFloat (float fFloat);
                bool SendDouble (double dDouble);
                bool SendInt (int iInt);
                bool SendUInt (unsigned int uiInt);
                bool SendLong (long lLong);
                bool SendULong (unsigned long ulLong);
                bool SendShort (short sShort);
                bool SendUShort (unsigned short usShort);
                bool SendChar (char cChar);
                bool SendUChar (unsigned char ucChar);
				bool SendString (std::string strLine);

				bool HTTP_SendGET (std::string strRetrieve, std::string strHeaders = "");
				bool HTTP_SendPOST (std::string strRetrieve, std::string strLine, std::string strHeaders, std::string strAuth = "");
				bool HTTP_SendPOST (std::string strContentType, std::string strRetrieve, std::string strLine, std::string strHeaders, std::string strAuth = "");
				std::string HTTP_ParseRequest (std::string strContent, std::string strNewLine);

                float RecvFloat ();
                double RecvDouble ();
                int RecvInt ();
                unsigned int RecvUInt ();
                long RecvLong ();
                unsigned long RecvULong ();
                short RecvShort ();
                unsigned short RecvUShort ();
                char RecvChar ();
                unsigned char RecvUChar ();
				char *RecvString (int iLength);
				std::string RecvString ();

				#if (defNET_IPVERS == 1)
					s13Ipv4 GetServerIP ();
				#elif (defNET_IPVERS == 2)
					s13Ipv6 GetServerIP ();
				#endif

				unsigned short GetPort ();

				std::string GetLastError ();

				bool Close ();

				s13NetworkingClass *GetNetworkingClass ();

				#ifdef defSPUZZLE
					s13CallBack<s13PuzzleTCPIP, void> *scbCallback;
				#endif

			private:
				#if (defNET_IPVERS == 1)
					s13Ipv4 ipServerIp;
					std::string strHTTPserverAddress;
				#elif (defNET_IPVERS == 2)
					s13Ipv6 ipServerIp;
				#else
					#error "You must specify a valid IP-Protocol version setting in sco_settings.h with definition: defNET_IPVERS."
				#endif

				#ifdef defSYS_WIN32
					#ifndef defAPI_WXWIDGETS
						s13WinSockClient *swcSocket;
					#endif
				#endif

				#ifdef defAPI_WXWIDGETS
					s13WXWClient *swcSocket;
				#endif
		};
	#endif
#endif

