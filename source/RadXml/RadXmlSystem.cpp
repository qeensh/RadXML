#include "RadXmlSystem.h"

#include "RadXmlGlobal.h"

#include "RadXml.h"
#include "RadXmlXmlFile.h"

#include <string>

namespace RadXml
{
	// SystemClass
	void SystemClass::setupSystemClass ()
	{
		Class *cClass = new Class ("System", "System");
		cClass->set ("loadExternal", new Function ("loadExternal", 0, SystemClass::loadExternal));
		cClass->set ("executeExternal", new Function ("executeExternal", 0, SystemClass::executeExternal));
		cClass->set ("unloadExternal", new Function ("unloadExternal", 0, SystemClass::unloadExternal));

		JavascriptManager::getSingletonPtr ()->set ("System", cClass);
	}

	Variable *SystemClass::loadExternal (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		std::string strName = aryArguments->getElement (0)->toString ()->getValue ();
		std::string strFilename = aryArguments->getElement (1)->toString ()->getValue ();
		bool bReturn = ogrGame->getCurrentXMLFile ()->loadExternal (strName, strFilename);

		return (Boolean::New (bReturn));
	}

	Variable *SystemClass::executeExternal (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		std::string strName = aryArguments->getElement (0)->toString ()->getValue ();
		std::string strFunction = aryArguments->getElement (1)->toString ()->getValue ();

		ogrGame->getCurrentXMLFile ()->executeExternalFunction (strName, strFunction);

		return (0);
	}

	Variable *SystemClass::unloadExternal (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		std::string strName = aryArguments->getElement (0)->toString ()->getValue ();
		bool bReturn = ogrGame->getCurrentXMLFile ()->unloadExternal (strName);

		return (Boolean::New (bReturn));
	}
}

