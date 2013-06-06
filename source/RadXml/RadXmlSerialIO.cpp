#include "RadXmlSerialIO.h"

#include "RadXmlNative.h"

namespace RadXml
{
	const int NativeSerialIO::NSI_ONESTOPBIT = 0;
	const int NativeSerialIO::NSI_ONE5STOPBITS = 0;
	const int NativeSerialIO::NSI_TWOSTOPBITS = 0;

	const int NativeSerialIO::NSI_NOPARITY = 0;
	const int NativeSerialIO::NSI_ODDPARITY = 1;
	const int NativeSerialIO::NSI_EVENPARITY = 2;
	const int NativeSerialIO::NSI_MARKPARITY = 2;
	const int NativeSerialIO::NSI_SPACEPARITY = 2;

	// NativeSerialIO
	NativeSerialIO::NativeSerialIO ()
	{
		strPortName = "";
		iBaud = 9600;
		iByteSize = 8;
		iStopBits = NSI_ONESTOPBIT;
		iParity = NSI_NOPARITY;
		bIsConnected = false;

		#ifdef defSYS_WIN32
			hSerialIO = 0;
		#endif
	}

	NativeSerialIO::~NativeSerialIO ()
	{
	}

	bool NativeSerialIO::open (std::string portName, int baud, int byteSize, int stopBits, int parity)
	{
		strPortName = portName;
		iBaud = baud;
		iByteSize = byteSize;
		iStopBits = stopBits;
		iParity = parity;

		#ifdef defSYS_WIN32
			hSerialIO = CreateFile (strPortName.c_str (), (GENERIC_READ | GENERIC_WRITE), 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

			if (hSerialIO == INVALID_HANDLE_VALUE)
			{
				close ();

				return (false);
			}

			DCB dcbSerialParams;

			if (GetCommState (hSerialIO, &dcbSerialParams) == false)
			{
				close ();

				return (false);
			}

			dcbSerialParams.BaudRate = iBaud;
			dcbSerialParams.ByteSize = iByteSize;
			dcbSerialParams.StopBits = iStopBits;
			dcbSerialParams.Parity = iParity;

			if (SetCommState (hSerialIO, &dcbSerialParams) == false)
			{
				close ();

				return (false);
			}

			bIsConnected = true;
		#endif

		return (true);
	}

	std::string NativeSerialIO::read (unsigned int bufferSize)
	{
		std::string strReturn = "";

		#ifdef defSYS_WIN32
			DWORD dErrors = 0;
			DWORD dNumBytesRead = 0;

			ClearCommError (hSerialIO, &dErrors, &cStatus);

			if (cStatus.cbInQue > 0)
			{
				unsigned int iSize = 0;

				if (cStatus.cbInQue > bufferSize)
					iSize = bufferSize;
				else
					iSize = cStatus.cbInQue;

				char *cBuffer = new char [iSize];
				if (ReadFile (hSerialIO, cBuffer, iSize, &dNumBytesRead, 0) == false)
				{
					delete []cBuffer;
					cBuffer = 0;

					return ("");
				}

				if (dNumBytesRead == 0)
				{
					delete []cBuffer;
					cBuffer = 0;

					return ("");
				}

				strReturn = cBuffer;

				delete []cBuffer;
				cBuffer = 0;
			}
		#endif

		return (strReturn);
	}

	int NativeSerialIO::read (char *buffer, unsigned int bufferSize)
	{
		std::string strReturn = "";

		#ifdef defSYS_WIN32
			DWORD dErrors = 0;
			DWORD dNumBytesRead = 0;

			ClearCommError (hSerialIO, &dErrors, &cStatus);

			if (cStatus.cbInQue > 0)
			{
				unsigned int iSize = 0;

				if (cStatus.cbInQue > bufferSize)
					iSize = bufferSize;
				else
					iSize = cStatus.cbInQue;

				if (ReadFile (hSerialIO, buffer, iSize, &dNumBytesRead, 0) == false)
					return (-1);

				if (dNumBytesRead == 0)
					return (dNumBytesRead);
			}
		#endif

		return (-1);
	}

	bool NativeSerialIO::write (std::string buffer)
	{
		bool bResult = false;
		char *cBuffer = const_cast<char *> (buffer.c_str ());

		#ifdef defSYS_WIN32
			DWORD iBytesToSend = 0;

			bResult = WriteFile (hSerialIO, (void *)cBuffer, buffer.length (), &iBytesToSend, 0);
		#endif

		return (bResult);
	}

	bool NativeSerialIO::write (char *buffer, unsigned int bufferSize)
	{
		bool bResult = false;

		#ifdef defSYS_WIN32
			DWORD iBytesToSend = 0;

			bResult = WriteFile (hSerialIO, (void *)buffer, bufferSize, &iBytesToSend, 0);
		#endif

		return (bResult);
	}

	void NativeSerialIO::close ()
	{
		bIsConnected = false;

		#ifdef defSYS_WIN32
			CloseHandle (hSerialIO);
		#endif
	}

	// SerialIOClass
	void SerialIOClass::setupSerialIOClass ()
	{
		Class *cClass = new Class ("SerialIO", "SerialIO");
		cClass->set ("open", new Function ("open", 0, SerialIOClass::open));
		cClass->set ("getPortName", new Function ("getPortName", 0, SerialIOClass::getPortName));
		cClass->set ("getBaud", new Function ("getBaud", 0, SerialIOClass::getBaud));
		cClass->set ("getByteSize", new Function ("getByteSize", 0, SerialIOClass::getByteSize));
		cClass->set ("getStopBits", new Function ("getStopBits", 0, SerialIOClass::getStopBits));
		cClass->set ("getParity", new Function ("getParity", 0, SerialIOClass::getParity));
		cClass->set ("isOpen", new Function ("isOpen", 0, SerialIOClass::isOpen));
		cClass->set ("read", new Function ("read", 0, SerialIOClass::read));
		cClass->set ("write", new Function ("write", 0, SerialIOClass::write));
		cClass->set ("close", new Function ("close", 0, SerialIOClass::close));

		JavascriptManager::getSingletonPtr ()->set ("SerialIO", cClass);
	}

	Variable *SerialIOClass::open (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		std::string strComPort = ((String *)aryArguments->getElement (0))->getValue ();
		Number *iValue = (Number *)aryArguments->getElement (1);
		NativeSerialIO *nsiSerialIO = 0;

		if (thisObject->hasHiddenData ("serialIO") == true)
			nsiSerialIO = (NativeSerialIO *)thisObject->getHiddenData ("serialio");
		else
			nsiSerialIO = new NativeSerialIO ();

		nsiSerialIO->setPortName (strComPort);

		if (iValue != 0)
			nsiSerialIO->setBaud (iValue->getValue ());

		iValue = (Number *)aryArguments->getElement (2);

		if (iValue != 0)
			nsiSerialIO->setByteSize (iValue->getValue ());

		iValue = (Number *)aryArguments->getElement (3);

		if (iValue != 0)
			nsiSerialIO->setStopBits (iValue->getValue ());

		iValue = (Number *)aryArguments->getElement (4);

		if (iValue != 0)
			nsiSerialIO->setParity (iValue->getValue ());

		nsiSerialIO->open (nsiSerialIO->getPortName (), nsiSerialIO->getBaud (), 
			nsiSerialIO->getByteSize (), nsiSerialIO->getStopBits (), nsiSerialIO->getParity ());

		thisObject->addHiddenData ("serialio", nsiSerialIO);

		return (0);
	}

	Variable *SerialIOClass::getPortName (Variable *thisObject, Variable *arguments)
	{
		NativeSerialIO *nsiSerialIO = (NativeSerialIO *)thisObject->getHiddenData ("serialio");
		String *strReturn = new String ("", nsiSerialIO->getPortName ());

		return (strReturn);
	}

	Variable *SerialIOClass::getBaud (Variable *thisObject, Variable *arguments)
	{
		NativeSerialIO *nsiSerialIO = (NativeSerialIO *)thisObject->getHiddenData ("serialio");
		Number *iReturn = new Number ("", nsiSerialIO->getBaud ());

		return (iReturn);
	}

	Variable *SerialIOClass::getByteSize (Variable *thisObject, Variable *arguments)
	{
		NativeSerialIO *nsiSerialIO = (NativeSerialIO *)thisObject->getHiddenData ("serialio");
		Number *iReturn = new Number ("", nsiSerialIO->getByteSize ());

		return (iReturn);
	}

	Variable *SerialIOClass::getStopBits (Variable *thisObject, Variable *arguments)
	{
		NativeSerialIO *nsiSerialIO = (NativeSerialIO *)thisObject->getHiddenData ("serialio");
		Number *iReturn = new Number ("", nsiSerialIO->getStopBits ());

		return (iReturn);
	}

	Variable *SerialIOClass::getParity (Variable *thisObject, Variable *arguments)
	{
		NativeSerialIO *nsiSerialIO = (NativeSerialIO *)thisObject->getHiddenData ("serialio");
		Number *iReturn = new Number ("", nsiSerialIO->getParity ());

		return (iReturn);
	}

	Variable *SerialIOClass::isOpen (Variable *thisObject, Variable *arguments)
	{
		NativeSerialIO *nsiSerialIO = (NativeSerialIO *)thisObject->getHiddenData ("serialio");
		Boolean *bReturn = new Boolean ("", nsiSerialIO->isOpen ());

		return (bReturn);
	}

	Variable *SerialIOClass::read (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Number *iValue = (Number *)aryArguments->getElement (0);
		NativeSerialIO *nsiSerialIO = (NativeSerialIO *)thisObject->getHiddenData ("serialio");
		std::string strData = nsiSerialIO->read (iValue->getValue ());
		String *strReturn = new String ("", strData);

		return (strReturn);
	}

	Variable *SerialIOClass::write (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		String *strValue = (String *)aryArguments->getElement (0);
		NativeSerialIO *nsiSerialIO = (NativeSerialIO *)thisObject->getHiddenData ("serialio");
		Boolean *bReturn = new Boolean ("", nsiSerialIO->write (strValue->getValue ()));

		return (bReturn);
	}

	Variable *SerialIOClass::close (Variable *thisObject, Variable *arguments)
	{
		NativeSerialIO *nsiSerialIO = (NativeSerialIO *)thisObject->getHiddenData ("serialio");
		nsiSerialIO->close ();

		return (0);
	}
}