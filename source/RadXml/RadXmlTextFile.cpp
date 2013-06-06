#include "RadXmlTextFile.h"

#include "RadXmlNative.h"

#include <fstream>

namespace RadXml
{
	// TextFileClass
	void TextFileClass::setupTextFileClass ()
	{
		Class *cClass = new Class ("TextFile", "TextFile");
		cClass->set ("open", new Function ("open", 0, TextFileClass::open));
		cClass->set ("isOpen", new Function ("isOpen", 0, TextFileClass::isOpen));
		cClass->set ("write", new Function ("write", 0, TextFileClass::write));
		cClass->set ("writeln", new Function ("writeln", 0, TextFileClass::writeln));
		cClass->set ("read", new Function ("read", 0, TextFileClass::read));
		cClass->set ("readln", new Function ("readln", 0, TextFileClass::readln));
		cClass->set ("readToEnd", new Function ("readToEnd", 0, TextFileClass::readToEnd));
		cClass->set ("getChar", new Function ("getChar", 0, TextFileClass::getChar));
		cClass->set ("seek", new Function ("seek", 0, TextFileClass::seek));
		cClass->set ("peek", new Function ("peek", 0, TextFileClass::peek));
		cClass->set ("getPosition", new Function ("getPosition", 0, TextFileClass::getPosition));
		cClass->set ("isEOF", new Function ("isEOF", 0, TextFileClass::isEOF));
		cClass->set ("close", new Function ("close", 0, TextFileClass::close));

		JavascriptManager::getSingletonPtr ()->set ("TextFile", cClass);
	}

	Variable *TextFileClass::open (Variable *thisObject, Variable *arguments)
	{
		Boolean *bReturn = new Boolean ("");
		Array *aryArguments = static_cast <Array *> (arguments);
		std::string strFile = ((String *)aryArguments->getElement (0))->getValue ();
		std::fstream *oFile = 0;

		if (thisObject->hasHiddenData ("file") == true)
			oFile = (std::fstream *)thisObject->getHiddenData ("file");
		else
			oFile = new std::fstream ();

		thisObject->addHiddenData ("file", oFile);
		oFile->open (strFile.c_str (), (std::ios_base::in | std::ios_base::out | std::ios_base::trunc));

		if (oFile->is_open () == true)
			bReturn->setValue (true);
		else
			bReturn->setValue (false);

		return (bReturn);
	}

	Variable *TextFileClass::isOpen (Variable *thisObject, Variable *arguments)
	{
		Boolean *bReturn = new Boolean ("");
		Array *aryArguments = static_cast <Array *> (arguments);
		std::string strFile = ((String *)aryArguments->getElement (0))->getValue ();
		std::fstream *oFile = (std::fstream *)thisObject->getHiddenData ("file");

		if (oFile->is_open () == true)
			bReturn->setValue (true);
		else
			bReturn->setValue (false);

		return (bReturn);
	}

	Variable *TextFileClass::write (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		std::string strText = ((String *)aryArguments->getElement (0))->getValue ();
		std::fstream *oFile = (std::fstream *)thisObject->getHiddenData ("file");

		oFile->write (strText.c_str (), strText.size ());

		return (0);
	}

	Variable *TextFileClass::writeln (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		std::string strText = ((String *)aryArguments->getElement (0))->getValue ();
		std::fstream *oFile = (std::fstream *)thisObject->getHiddenData ("file");

		strText += "\n";
		oFile->write (strText.c_str (), strText.size ());

		return (0);
	}

	Variable *TextFileClass::read (Variable *thisObject, Variable *arguments)
	{
		String *strText = new String ("");
		Array *aryArguments = static_cast <Array *> (arguments);
		unsigned int iNumBytes = ((Number *)aryArguments->getElement (0))->getValue ();
		std::fstream *oFile = (std::fstream *)thisObject->getHiddenData ("file");
		char *cRead = new char [iNumBytes];

		oFile->read (cRead, iNumBytes);
		strText->setValue (cRead);

		delete []cRead;
		cRead = 0;

		return (strText);
	}

	Variable *TextFileClass::readln (Variable *thisObject, Variable *arguments)
	{
		String *strText = new String ("");
		Array *aryArguments = static_cast <Array *> (arguments);
		std::fstream *oFile = (std::fstream *)thisObject->getHiddenData ("file");

		std::string strReadLine = "";

		while (oFile->good () == true)
		{
			char cChar = oFile->get ();

			if (oFile->good () == false)
				break;

			if (cChar == '\n')
			{
				strReadLine += '\n';

				break;
			}

			strReadLine += cChar;
		}

		strText->setValue (strReadLine);

		return (strText);
	}

	Variable *TextFileClass::readToEnd (Variable *thisObject, Variable *arguments)
	{
		String *strText = new String ("");
		Array *aryArguments = static_cast <Array *> (arguments);
		std::fstream *oFile = (std::fstream *)thisObject->getHiddenData ("file");

		std::string strReadToEnd = "";

		while (oFile->good () == true)
		{
			char cChar = oFile->get ();

			if (oFile->good () == false)
				break;

			strReadToEnd += cChar;
		}

		strText->setValue (strReadToEnd);

		return (strText);
	}

	Variable *TextFileClass::getChar (Variable *thisObject, Variable *arguments)
	{
		String *strText = new String ("");
		Array *aryArguments = static_cast <Array *> (arguments);
		std::fstream *oFile = (std::fstream *)thisObject->getHiddenData ("file");

		char cChar = oFile->get ();
		std::string strChar = "";
		strChar += cChar;
		strText->setValue (strChar);

		return (strText);
	}

	Variable *TextFileClass::seek (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		unsigned int iPos = ((Number *)aryArguments->getElement (0))->getValue ();
		std::fstream *oFile = (std::fstream *)thisObject->getHiddenData ("file");

		oFile->seekg (iPos);

		return (0);
	}

	Variable *TextFileClass::peek (Variable *thisObject, Variable *arguments)
	{
		String *strText = new String ("");
		Array *aryArguments = static_cast <Array *> (arguments);
		std::fstream *oFile = (std::fstream *)thisObject->getHiddenData ("file");

		char cChar = oFile->peek ();
		std::string strChar = "";
		strChar += cChar;
		strText->setValue (strChar);

		return (strText);
	}

	Variable *TextFileClass::getPosition (Variable *thisObject, Variable *arguments)
	{
		Number *iPos = new Number ("");
		Array *aryArguments = static_cast <Array *> (arguments);
		std::fstream *oFile = (std::fstream *)thisObject->getHiddenData ("file");

		iPos->setValue (oFile->tellg ());

		return (iPos);
	}

	Variable *TextFileClass::isEOF (Variable *thisObject, Variable *arguments)
	{
		Boolean *bReturn = new Boolean ("");
		Array *aryArguments = static_cast <Array *> (arguments);
		std::fstream *oFile = (std::fstream *)thisObject->getHiddenData ("file");

		bReturn->setValue (oFile->eof ());

		return (bReturn);
	}

	Variable *TextFileClass::close (Variable *thisObject, Variable *arguments)
	{
		std::fstream *oFile = (std::fstream *)thisObject->getHiddenData ("file");
		oFile->close ();

		return (0);
	}
}

