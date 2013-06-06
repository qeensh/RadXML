#ifndef _RADXML_SERIALIO_H_
	#define _RADXML_SERIALIO_H_

	#include <string>

	#ifdef defSYS_WIN32
		#include <windows.h>
	#endif

	namespace RadXml
	{
		class Variable;

		class NativeSerialIO
		{
			public:
				NativeSerialIO ();
				~NativeSerialIO ();

				static const int NSI_ONESTOPBIT;
				static const int NSI_ONE5STOPBITS;
				static const int NSI_TWOSTOPBITS;

				static const int NSI_NOPARITY;
				static const int NSI_ODDPARITY;
				static const int NSI_EVENPARITY;
				static const int NSI_MARKPARITY;
				static const int NSI_SPACEPARITY;

				inline void setPortName (std::string portName)
				{
					strPortName = portName;
				}

				inline std::string getPortName ()
				{
					return (strPortName);
				}

				inline void setBaud (int baud)
				{
					iBaud = baud;
				}

				inline int getBaud ()
				{
					return (iBaud);
				}

				inline void setByteSize (int byteSize)
				{
					iByteSize = byteSize;
				}

				inline int getByteSize ()
				{
					return (iByteSize);
				}

				inline void setStopBits (int stopBits)
				{
					iStopBits = stopBits;
				}

				inline int getStopBits ()
				{
					return (iStopBits);
				}

				inline void setParity (int parity)
				{
					iParity = parity;
				}

				inline int getParity ()
				{
					return (iParity);
				}

				bool open (std::string portName, int baud = 9600, int byteSize = 8, 
							int stopBits = NSI_ONESTOPBIT, int parity = NSI_NOPARITY);

				inline bool isOpen ()
				{
					return (bIsConnected);
				}

				std::string read (unsigned int bufferSize);
				int read (char *buffer, unsigned int bufferSize);
				bool write (std::string buffer);
				bool write (char *buffer, unsigned int bufferSize);

				void close ();

			protected:
				std::string strPortName;
				int iBaud;
				int iByteSize;
				int iStopBits;
				int iParity;

				bool bIsConnected;

				#ifdef defSYS_WIN32
					HANDLE hSerialIO;
					COMSTAT cStatus;
				#endif
		};

		class SerialIOClass
		{
			public:
				static void setupSerialIOClass ();

				static Variable *open (Variable *thisObject, Variable *arguments);
				static Variable *getPortName (Variable *thisObject, Variable *arguments);
				static Variable *getBaud (Variable *thisObject, Variable *arguments);
				static Variable *getByteSize (Variable *thisObject, Variable *arguments);
				static Variable *getStopBits (Variable *thisObject, Variable *arguments);
				static Variable *getParity (Variable *thisObject, Variable *arguments);
				static Variable *isOpen (Variable *thisObject, Variable *arguments);
				static Variable *read (Variable *thisObject, Variable *arguments);
				static Variable *write (Variable *thisObject, Variable *arguments);
				static Variable *close (Variable *thisObject, Variable *arguments);
		};
	}
#endif

