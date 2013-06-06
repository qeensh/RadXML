#include "RadXmlGlobal.h"

#include "RadXml.h"
#include "RadXmlApp.h"
#include "RadXmlAppObject.h"
#include "RadXmlGame.h"

#include "RadXmlMovable.h"

#include "common/mai_radxml_system.h"
#include "common/com_radxml_strconv.h"

#include "languages/language.h"

namespace RadXml
{
	// GlobalClass
	void GlobalClass::setupGlobalClass ()
	{
		JavascriptManager::getSingletonPtr ()->set ("alert", new Function ("alert", 0, GlobalClass::alert));
		JavascriptManager::getSingletonPtr ()->set ("include", new Function ("include", 0, GlobalClass::include));
		JavascriptManager::getSingletonPtr ()->set ("logMessage", new Function ("logMessage", 0, GlobalClass::logMessage));
		JavascriptManager::getSingletonPtr ()->set ("openFileDialog", new Function ("openFileDialog", 0, GlobalClass::openFileDialog));
		JavascriptManager::getSingletonPtr ()->set ("saveFileDialog", new Function ("saveFileDialog", 0, GlobalClass::saveFileDialog));
		JavascriptManager::getSingletonPtr ()->set ("copyTextToClipboard", new Function ("copyTextToClipboard", 0, GlobalClass::copyTextToClipboard));
		JavascriptManager::getSingletonPtr ()->set ("getTextFromClipboard", new Function ("getTextFromClipboard", 0, GlobalClass::getTextFromClipboard));
		JavascriptManager::getSingletonPtr ()->set ("openBrowser", new Function ("openBrowser", 0, GlobalClass::openBrowser));
		JavascriptManager::getSingletonPtr ()->set ("mkdir", new Function ("mkdir", 0, GlobalClass::mkdir));
		JavascriptManager::getSingletonPtr ()->set ("moveFile", new Function ("moveFile", 0, GlobalClass::moveFile));
		JavascriptManager::getSingletonPtr ()->set ("deleteFile", new Function ("deleteFile", 0, GlobalClass::deleteFile));
		JavascriptManager::getSingletonPtr ()->set ("deleteDir", new Function ("deleteDir", 0, GlobalClass::deleteDir));
		JavascriptManager::getSingletonPtr ()->set ("copyFile", new Function ("copyFile", 0, GlobalClass::copyFile));
		JavascriptManager::getSingletonPtr ()->set ("isFile", new Function ("isFile", 0, GlobalClass::isFile));
		JavascriptManager::getSingletonPtr ()->set ("isDir", new Function ("isDir", 0, GlobalClass::isDir));
		JavascriptManager::getSingletonPtr ()->set ("quit", new Function ("quit", 0, GlobalClass::quit));
		JavascriptManager::getSingletonPtr ()->set ("setTimeout", new Function ("setTimeout", 0, GlobalClass::setTimeout));
	}

	Variable *GlobalClass::alert (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		std::string strMessage = aryArguments->getElement (0)->toString ()->getValue ();

		RadXmlCommon::s13MsgBox (strMessage, MSG18);

		return (0);
	}

	Variable *GlobalClass::include (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		RadXML *ogrGame = RadXML::getSingletonPtr ();
		std::string strDir = ogrGame->getCurrentScriptDirectory ();
		std::string strFile = aryArguments->getElement (0)->toString ()->getValue ();
		std::string strType = "";

		if (aryArguments->length () > 1)
		{
			strType = aryArguments->getElement (1)->toString ()->getValue ();
			strType = RadXmlCommon::s13ASCII_StringToLower (strType);
		}

		strFile = RadXmlCommon::System_NormalizeDirectoryPath ((strDir + strFile), 0, false);

		if (strType == "")
			strType = "text/xml";

		if ((strType == "text/xml") || (strType == "xml"))
			ogrGame->loadXml (strFile);

		if ((strType == "text/javascript") || (strType == "javascript"))
			ogrGame->compileAndRunJSScript (strFile);

		return (0);
	}

	Variable *GlobalClass::logMessage (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		std::string strMessage = aryArguments->getElement (0)->toString ()->getValue ();

		RadXmlCommon::s13LogMessage (strMessage);

		return (0);
	}

	Variable *GlobalClass::openFileDialog (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strTitle = aryArguments->getElement (0)->toString ()->getValue ();
		std::string strFilter = aryArguments->getElement (1)->toString ()->getValue ();
		std::string strDefExt = aryArguments->getElement (2)->toString ()->getValue ();
		std::string strFile = RadXmlCommon::s13OpenFile (strTitle, strFilter, strDefExt);

		return (String::New (strFile));
	}

	Variable *GlobalClass::saveFileDialog (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strTitle = aryArguments->getElement (0)->toString ()->getValue ();
		std::string strFilter = aryArguments->getElement (1)->toString ()->getValue ();
		std::string strDefExt = aryArguments->getElement (2)->toString ()->getValue ();
		std::string strFile = RadXmlCommon::s13SaveFile (strTitle, strFilter, strDefExt);

		return (String::New (strFile));
	}

	Variable *GlobalClass::applyVideoSettings (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		

		return (0);
	}

	Variable *GlobalClass::copyTextToClipboard (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strText = aryArguments->getElement (0)->toString ()->getValue ();

		RadXmlCommon::s13CopyTextToClipboard (strText);

		return (0);
	}

	Variable *GlobalClass::getTextFromClipboard (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strText = RadXmlCommon::s13GetTextFromClipboard ();

		return (String::New (strText));
	}

	Variable *GlobalClass::openBrowser (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strText = aryArguments->getElement (0)->toString ()->getValue ();

		RadXmlCommon::s13OpenDefaultWebBrowser (strText);

		return (0);
	}

	Variable *GlobalClass::mkdir (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strText = aryArguments->getElement (0)->toString ()->getValue ();
		bool bValue = RadXmlCommon::s13Mkdir (strText);

		return (Boolean::New (bValue));
	}

	Variable *GlobalClass::moveFile (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strText = aryArguments->getElement (0)->toString ()->getValue ();
		std::string strText2 = aryArguments->getElement (1)->toString ()->getValue ();
		bool bValue = RadXmlCommon::s13MoveFile (strText, strText2);

		return (Boolean::New (bValue));
	}

	Variable *GlobalClass::deleteFile (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strText = aryArguments->getElement (0)->toString ()->getValue ();
		bool bValue = RadXmlCommon::s13Rm (strText);

		return (Boolean::New (bValue));
	}

	Variable *GlobalClass::deleteDir (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strText = aryArguments->getElement (0)->toString ()->getValue ();
		bool bValue = RadXmlCommon::s13Rmdir (strText);

		return (Boolean::New (bValue));
	}

	Variable *GlobalClass::copyFile (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strExisting = aryArguments->getElement (0)->toString ()->getValue ();
		std::string strNew = aryArguments->getElement (1)->toString ()->getValue ();
		bool bFailIfExists = aryArguments->getElement (2)->toBoolean ()->getValue ();
		bool bValue = RadXmlCommon::s13CopyFile (strExisting, strNew, bFailIfExists);

		return (Boolean::New (bValue));
	}

	Variable *GlobalClass::isFile (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strText = aryArguments->getElement (0)->toString ()->getValue ();
		bool bValue = RadXmlCommon::s13IsFile (strText);

		return (Boolean::New (bValue));
	}

	Variable *GlobalClass::isDir (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strText = aryArguments->getElement (0)->toString ()->getValue ();
		bool bValue = RadXmlCommon::s13IsDirectory (strText);

		return (Boolean::New (bValue));
	}

	Variable *GlobalClass::quit (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		RadXML::getSingletonPtr ()->callCallback (RadXml::JCE_QUIT, 0);

		return (0);
	}

	Variable *GlobalClass::setTimeout (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();

		std::string strText = aryArguments->getElement (0)->toString ()->getValue ();
		double dValue = aryArguments->getElement (1)->toNumber ()->getValue ();

		RadXML *ogrGame = RadXML::getSingletonPtr ();
		ogrGame->createTimer (strText, (unsigned int)dValue);

		return (0);
	}
}

