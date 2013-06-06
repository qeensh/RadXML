#include "RadXmlXmlFile.h"
#include "RadXml.h"

#include "RadXmlApp.h"

#include "RadXmlGame.h"
#include "RadXmlVector3.h"
#include "RadXmlGlobal.h"

#include "languages/language.h"

#include "xml/xml.h"

#include "common/mai_radxml_system.h"

#include "common/com_radxml_strconv.h"

namespace RadXml
{
	XmlFile::XmlFile ()
		: Game (""), App ("")
	{
		init ();
	}

	XmlFile::XmlFile (std::string file)
		: Game (""), App ("")
	{
		init ();
		loadFile (file);
	}

	XmlFile::~XmlFile ()
	{
	}

	void XmlFile::init ()
	{
		dRadXmlVersion = 0;

		filename = "";
		dirPath = "";

		RadXML *ogrGame = RadXML::getSingletonPtr ();

		if (ogrGame->getCurrentXMLFile () == 0)
			ogrGame->setCurrentXMLFile (this);
	}

	bool XmlFile::loadFile (std::string file)
	{
		RadXmlCommon::s13LogMessage (MSG1 + file);

		{
			unsigned int iPos = 0;

			#ifdef defSYS_WIN32
				iPos = file.rfind ("\\");
			#else
				iPos = file.rfind ("/");
			#endif

			filename = file.substr (iPos + 1);
			dirPath = file.substr (0, iPos);
		}

		XMLParser *xmlFile = new XMLParser ();

		if (xmlFile->parseFile (file) == false)
			return (false);

		parseXMLFile (xmlFile);

		delete xmlFile;
		xmlFile = 0;

		return (true);
	}

	void XmlFile::parseXMLFile (XMLParser *xmlFile)
	{
		std::vector<XMLElement *> aryMetas = xmlFile->findAllTags ("meta");

		for (unsigned int iIdx = 0; iIdx < aryMetas.size (); iIdx++)
		{
			XMLElement *xmlElement = aryMetas.at (iIdx);
			std::string strName = xmlElement->getAttributeValue ("name");
			strName = RadXmlCommon::s13ASCII_StringToLower (strName);
			std::string strContent = xmlElement->getAttributeValue ("content");
			strContent = RadXmlCommon::s13ASCII_StringToLower (strContent);

			if (strName == "version")
				dRadXmlVersion = RadXmlCommon::s13StringToDouble (strContent);
		}

		if (dRadXmlVersion == 0.0)
		{
			RadXmlCommon::s13LogMessage (MSG2 + xmlFile->getFilename ());

			return;
		}

		if (dRadXmlVersion < defRADXML_MIN_VERSION)
		{
			RadXmlCommon::s13LogMessage (MSG3 + 
				RadXmlCommon::s13DoubleToString (defRADXML_MIN_VERSION) + MSG4 + xmlFile->getFilename ());

			return;
		}

		if (dRadXmlVersion > defRADXML_VERSION)
		{
			RadXmlCommon::s13LogMessage (MSG5 + 
				RadXmlCommon::s13DoubleToString (defRADXML_VERSION) + MSG4 + xmlFile->getFilename ());

			return;
		}

		std::vector<XMLElement *> aryIncludes = xmlFile->findAllTags ("include");

		for (unsigned int iIdx = 0; iIdx < aryIncludes.size (); iIdx++)
		{
			XMLElement *xmlElement = aryIncludes.at (iIdx);
			std::string strSrc = xmlElement->getAttributeValue ("src");
			std::string strType = xmlElement->getAttributeValue ("type");
			strType = RadXmlCommon::s13ASCII_StringToLower (strType);

			if (strType == "")
				strType = "text/xml";

			if ((strType == "text/xml") || (strType == "xml"))
			{
				strSrc = RadXmlCommon::System_NormalizeDirectoryPath (dirPath + "/" + strSrc, 0, false);
				RadXmlCommon::s13LogMessage (MSG7 + strSrc);

				XMLParser *xmlFile2 = new XMLParser ();

				if (xmlFile2->parseFile (strSrc) == false)
					continue;

				parseXMLFile (xmlFile2);

				delete xmlFile2;
				xmlFile2 = 0;
			}

			if ((strType == "text/javascript") || (strType == "javascript"))
			{
				RadXmlCommon::s13LogMessage (MSG8 + strSrc);

				RadXML::getSingletonPtr ()->compileAndRunJSScript (strSrc);
			}
		}

		std::vector<XMLElement *> aryExternals = xmlFile->findAllTags ("external");

		for (unsigned int iIdx = 0; iIdx < aryExternals.size (); iIdx++)
		{
			XMLElement *xmlElement = aryExternals.at (iIdx);
			std::string strName = xmlElement->getAttributeValue ("name");
			std::string strSrc = xmlElement->getAttributeValue ("src");
			std::string strType = xmlElement->getAttributeValue ("type");
			std::string strOS = xmlElement->getAttributeValue ("os");
			strType = RadXmlCommon::s13ASCII_StringToLower (strType);
			strOS = RadXmlCommon::s13ASCII_StringToLower (strOS);

			#ifdef defSYS_WIN32
				if (strType == "")
					strType = "binary/x86";

				if (strOS == "")
					strOS = "windows";

				if ((strOS == "windows"))
				{
					strSrc = RadXmlCommon::System_NormalizeDirectoryPath (dirPath + "/" + strSrc, 0, false);

					if (loadExternal (strName, strSrc) == true)
						RadXmlCommon::s13LogMessage (MSG9 + strSrc);
					else
						RadXmlCommon::s13LogMessage (MSG10 + strSrc);
				}
			#endif
		}

		std::vector<XMLElement *> aryApp = xmlFile->findAllTags ("app");

		for (unsigned int iIdx = 0; iIdx < aryApp.size (); iIdx++)
		{
			XMLElement *xmlElement = aryApp.at (iIdx);
			std::string strName = xmlElement->getAttributeValue ("name");
			std::string strLoad = xmlElement->getAttributeValue ("load");
			std::string strType = xmlElement->getAttributeValue ("type");
			strType = RadXmlCommon::s13ASCII_StringToLower (strType);
			bool bLoad = true;
			int iType = AT_RENDER;

			if (strLoad != "")
				bLoad = RadXmlCommon::s13StringToBool (strLoad);

			if (strType == "native")
				iType = AT_NATIVE;

			if (strType == "render")
				iType = AT_RENDER;

			if (iIdx == 0)
				parseApp (this, iType, xmlElement);
			else
			{
				App *aApp = new App (strName);

				if (bLoad == true)
					aApp->parseApp (this, iType, xmlElement);
			}
		}

		std::vector<XMLElement *> aryGames = xmlFile->findAllTags ("game");

		for (unsigned int iIdx = 0; iIdx < aryGames.size (); iIdx++)
		{
			XMLElement *xmlElement = aryGames.at (iIdx);
			std::string strName = xmlElement->getAttributeValue ("name");
			std::string strLoad = xmlElement->getAttributeValue ("load");
			bool bLoad = true;

			if (strLoad != "")
				bLoad = RadXmlCommon::s13StringToBool (strLoad);

			if (iIdx == 0)
				parse (this, xmlElement);
			else
			{
				Game *gGame = new Game (strName);

				if (bLoad == true)
					gGame->parse (this, xmlElement);
			}
		}

		std::vector<XMLElement *> aryScripts = xmlFile->findAllTags ("script");
		std::vector<XMLElement *> aryScripts2 = xmlFile->findAllTags ("rxscript");

		for (unsigned int iIdx = 0; iIdx < aryScripts2.size (); iIdx++)
		{
			XMLElement *xmlElement = aryScripts2.at (iIdx);
			aryScripts.push_back (xmlElement);
		}

		for (unsigned int iIdx = 0; iIdx < aryScripts.size (); iIdx++)
		{
			XMLElement *xmlElement = aryScripts.at (iIdx);
			std::string strType = xmlElement->getAttributeValue ("type");
			std::string strSrc = xmlElement->getAttributeValue ("src");
			strType = RadXmlCommon::s13ASCII_StringToLower (strType);

			if (strType == "")
				strType = "text/javascript";

			if (strSrc != "")
			{
				if ((strType == "javascript") || (strType == "text/javascript"))
				{
					strSrc = RadXmlCommon::System_NormalizeDirectoryPath (dirPath + "/" + strSrc, 0, false);
					RadXmlCommon::s13LogMessage (MSG8 + strSrc);
					RadXML::getSingletonPtr ()->compileAndRunJSScript (strSrc);
				}
			}

			if (xmlElement->content != "")
			{
				if ((strType == "javascript") || (strType == "text/javascript"))
					RadXML::getSingletonPtr ()->compileAndRunJSString (xmlElement->content);
			}
		}

		executeJavascriptFromEvent ("onload");
	}

	bool XmlFile::loadExternal (std::string name, std::string filename)
	{
		/// @note Make sure that this binary matches up with the type specified. IE: x86 for x86, ARM for ARM, x64 for x64
		int iLibraryIndex = RadXML::getSingletonPtr ()->loadLibrary (filename);

		if (iLibraryIndex < 0)
			return (false);

		hshDLLs.insert (std::pair<unsigned long, int> (RadXmlCommon::s13StringToHash (name), iLibraryIndex));

		return (true);
	}

	void XmlFile::executeExternalFunction (std::string name, std::string function)
	{
		HASHMAP<unsigned long, int>::iterator itBegin = hshDLLs.find (RadXmlCommon::s13StringToHash (name));
		HASHMAP<unsigned long, int>::iterator itEnd = hshDLLs.end ();

		if (itBegin != itEnd)
		{
			int index = (*itBegin).second;

			#ifndef defAPI_WXWIDGETS
			#ifdef defSYS_WIN32
				RadXML::getSingletonPtr ()->getDLLFunction (index, function) ();
			#endif
			#else
				((RadXmlFunction)RadXML::getSingletonPtr ()->getDLLFunction (index, function)) ();
			#endif
		}
	}

	bool XmlFile::unloadExternal (std::string name)
	{
		HASHMAP<unsigned long, int>::iterator itBegin = hshDLLs.find (RadXmlCommon::s13StringToHash (name));
		HASHMAP<unsigned long, int>::iterator itEnd = hshDLLs.end ();

		if (itBegin != itEnd)
		{
			int index = (*itBegin).second;
			bool bReturn = RadXML::getSingletonPtr ()->unloadLibrary (index);

			return (bReturn);
		}

		return (false);
	}

	void XmlFile::processDeleteObjects ()
	{
		processDeleteAppObjects ();
	}
}

