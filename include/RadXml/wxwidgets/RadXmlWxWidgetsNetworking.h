#ifndef _RADXML_WX_NETWORKING_H_
	#define _RADXML_WX_NETWORKING_H_

	#ifdef defAPI_WXWIDGETS
		#ifdef defNET_ON
			#include <vector>
			#include <sstream>
			#include <string>

			#include <wx/wxprec.h>
			#include <wx/wx.h>
			#include <wx/socket.h>
			#include <wx/sckipc.h>

			#ifdef USE_RADXML_DIR
				#include "RadXml/networking/net_netclass.h"
			#else
				#include "networking/net_netclass.h"
			#endif

			enum
			{
				defSOCKET_SERVER			= 5000,
				defSOCKET_SOCKET			= 5001,
				defSOCKET_CLIENT			= 5002
			};

			std::vector<std::string> WXW_GetComputerIPs ();
			std::vector< std::vector<unsigned long> > WXW_GetMACAddresses ();

			class s13TcpipServer;
			class s13TcpipClient;

			class s13WXWServer:public s13NetworkingClass
			{
				public:
					s13WXWServer ();

					bool Init (unsigned short usPortPass);

					bool AcceptWithBlocking ();
					bool AcceptWithoutBlocking ();

					bool SendFloatToAll (float fFloat, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
					bool SendFloat (wxSocketBase *socSocket, float fFloat);
					bool SendDoubleToAll (double dDouble, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
					bool SendDouble (wxSocketBase *socSocket, double dDouble);
					bool SendIntToAll (int iInt, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
					bool SendInt (wxSocketBase *socSocket, int iInt);
					bool SendUIntToAll (unsigned int uiInt, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
					bool SendUInt (wxSocketBase *socSocket, unsigned int uiInt);
					bool SendLongToAll (long lLong, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
					bool SendLong (wxSocketBase *socSocket, long lLong);
					bool SendULongToAll (unsigned long ulLong, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
					bool SendULong (wxSocketBase *socSocket, unsigned long ulLong);
					bool SendShortToAll (short sShort, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
					bool SendShort (wxSocketBase *socSocket, short sShort);
					bool SendUShortToAll (unsigned short usShort, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
					bool SendUShort (wxSocketBase *socSocket, unsigned short usShort);
					bool SendCharToAll (char cChar, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
					bool SendChar (wxSocketBase *socSocket, char cChar);
					bool SendUCharToAll (unsigned char ucChar, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
					bool SendUChar (wxSocketBase *socSocket, unsigned char ucChar);
					bool SendStringToAll (std::string strLine, std::vector<unsigned int> aryExcept = std::vector<unsigned int> ());
					bool SendString (wxSocketBase *socSocket, std::string strLine);

					std::vector<float> RecvFloat ();
					float RecvFloat (wxSocketBase *socSocket);
					std::vector<double> RecvDouble ();
					double RecvDouble (wxSocketBase *socSocket);
					std::vector<int> RecvInt ();
					int RecvInt (wxSocketBase *socSocket);
					std::vector<unsigned int> RecvUInt ();
					unsigned int RecvUInt (wxSocketBase *socSocket);
					std::vector<long> RecvLong ();
					long RecvLong (wxSocketBase *socSocket);
					std::vector<unsigned long> RecvULong ();
					unsigned long RecvULong (wxSocketBase *socSocket);
					std::vector<short> RecvShort ();
					short RecvShort (wxSocketBase *socSocket);
					std::vector<unsigned short> RecvUShort ();
					unsigned short RecvUShort (wxSocketBase *socSocket);
					std::vector<char> RecvChar ();
					char RecvChar (wxSocketBase *socSocket);
					std::vector<unsigned char> RecvUChar ();
					unsigned char RecvUChar (wxSocketBase *socSocket);
					std::vector<char *> RecvString (int iLength);
					char *RecvString (wxSocketBase *socSocket, int iLength);
					std::vector<std::string> RecvString ();
					std::string RecvString (wxSocketBase *socSocket);

					void SetPort (unsigned short usPortPass);
					unsigned short GetPort ();
					void SetServerAddr (wxIPV4address adrServerPass);
					wxIPV4address GetServerAddr ();
					void SetServerSocket (wxSocketServer *arySocketsPass);
					wxSocketServer * GetServerSocket ();
					void SetClientSockets (std::vector<wxSocketBase *> arySocketsPass);
					std::vector<wxSocketBase *> GetClientSockets ();
					wxSocketBase *GetClientSocket (unsigned int uiId);
					std::vector<std::string> GetConnectedIPs ();
					std::string GetConnectedIp (unsigned int uiId);
					std::string GetLastConnectedIp ();
					std::vector<unsigned int> GetLastReceivedFromList ();

					std::string GetLastError ();

					bool IsSocketOpen (wxSocketBase *socSocket);

					bool CloseServerSocket ();
					bool CloseClientSocket (wxSocketBase *socSocket);
					bool CloseAllSockets ();

					inline void setParent (s13TcpipServer *parent)
					{
						tcpParent = parent;
					}

					inline s13TcpipServer *getParent ()
					{
						return (tcpParent);
					}

				private:
					wxSocketServer *wssServer;
					unsigned short usPort;
					wxIPV4address adrServer;

					std::vector<wxSocketBase *> aryClientSockets;
					std::vector<wxIPV4address> aryAddresses;
					std::vector<unsigned int> aryReceivedFrom;

					s13TcpipServer *tcpParent;
			};

			class s13WXWClient:public s13NetworkingClass
			{
				public:
					s13WXWClient ();
					
					bool Init (unsigned short usPortPass);

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

					void SetPort (unsigned short usPortPass);
					unsigned short GetPort ();
					void SetSocket (wxSocketClient *socSocketPass);
					wxSocketClient *GetSocket ();
					std::string GetServerIP ();

					std::string GetLastError ();

					bool Close ();

					inline void setParent (s13TcpipClient *parent)
					{
						tcpParent = parent;
					}

					inline s13TcpipClient *getParent ()
					{
						return (tcpParent);
					}

				private:
					unsigned short usPort;
					wxSocketClient *socSocket;

					std::string strServerIp;

					s13TcpipClient *tcpParent;
			};
		#endif
	#endif
#endif

