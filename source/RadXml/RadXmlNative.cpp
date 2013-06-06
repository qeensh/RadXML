#include "RadXmlNative.h"

#include "RadXml.h"
#include "RadXmlGlobal.h"

#include "common/com_radxml_strconv.h"

namespace RadXml
{
	JavascriptManager * Singleton<JavascriptManager>::rxSingleton = 0;
	PluginManager *Singleton<PluginManager>::rxSingleton = 0;

	// Variable
	Variable::~Variable ()
	{
		if (destroyHiddenData == true)
		{
			HASHMAP<std::string, void *>::iterator itBegin = hshHidden.begin ();
			HASHMAP<std::string, void *>::iterator itEnd = hshHidden.end ();

			while (itBegin != itEnd)
			{
				delete (*itBegin).second;
				(*itBegin).second = 0;

				itBegin++;
			}
		}
	}

	void Variable::addHiddenData (std::string nameOfData, void *data)
	{
		void *vData = getHiddenData (nameOfData);

		if (vData == 0)
			hshHidden.insert (std::pair<std::string, void *> (nameOfData, data));
	}

	void *Variable::getHiddenDataByIndex (unsigned int index)
	{
		HASHMAP<std::string, void *>::iterator itBegin = hshHidden.begin ();

		for (unsigned int iIdx = 0; iIdx < index; iIdx++)
			itBegin++;

		return ((*itBegin).second);
	}

	std::string Variable::getHiddenDataNameByIndex (unsigned int index)
	{
		HASHMAP<std::string, void *>::iterator itBegin = hshHidden.begin ();

		for (unsigned int iIdx = 0; iIdx < index; iIdx++)
			itBegin++;

		return ((*itBegin).first);
	}

	void *Variable::getHiddenData (std::string nameOfData)
	{
		HASHMAP<std::string, void *>::iterator itBegin = hshHidden.find (nameOfData);
		HASHMAP<std::string, void *>::iterator itEnd = hshHidden.end ();

		if (itBegin != itEnd)
			return ((*itBegin).second);

		return (0);
	}

	bool Variable::hasHiddenData (std::string nameOfData)
	{
		HASHMAP<std::string, void *>::iterator itBegin = hshHidden.find (nameOfData);
		HASHMAP<std::string, void *>::iterator itEnd = hshHidden.end ();

		if (itBegin != itEnd)
			return (true);

		return (false);
	}

	void Variable::removeHiddenData (std::string nameOfData)
	{
		HASHMAP<std::string, void *>::iterator itBegin = hshHidden.find (nameOfData);
		HASHMAP<std::string, void *>::iterator itEnd = hshHidden.end ();

		if (itBegin != itEnd)
			hshHidden.erase (itBegin);
	}

	// Class
	Class::~Class ()
	{
		if (destroyData == true)
		{
			HASHMAP<unsigned long, Variable *>::iterator itBegin = hshVariables.begin ();
			HASHMAP<unsigned long, Variable *>::iterator itEnd = hshVariables.end ();

			while (itBegin != itEnd)
			{
				delete (*itBegin).second;
				(*itBegin).second = 0;

				itBegin++;
			}
		}
	}

	void Class::set (std::string variableName, Variable *value)
	{
		unsigned long ulHash = RadXmlCommon::s13StringToHash (variableName);
		hshVariables.insert (std::pair<unsigned long, Variable *> (ulHash, value));
	}

	Variable *Class::get (std::string variableName)
	{
		unsigned long ulHash = RadXmlCommon::s13StringToHash (variableName);
		HASHMAP<unsigned long, Variable *>::iterator itBegin = hshVariables.find (ulHash);
		HASHMAP<unsigned long, Variable *>::iterator itEnd = hshVariables.end ();

		if (itBegin != itEnd)
			return ((*itBegin).second);

		return (0);
	}

	Variable *Class::get (unsigned int index)
	{
		HASHMAP<unsigned long, Variable *>::iterator itBegin = hshVariables.begin ();

		for (unsigned int iIdx = 0; iIdx < hshVariables.size (); iIdx++)
		{
			if (iIdx == index)
				break;

			itBegin++;
		}

		return ((*itBegin).second);
	}

	// Function
	Function::~Function ()
	{
		if (destroyData == true)
		{
			HASHMAP<unsigned long, Variable *>::iterator itBegin = hshVariables.begin ();
			HASHMAP<unsigned long, Variable *>::iterator itEnd = hshVariables.end ();

			while (itBegin != itEnd)
			{
				delete (*itBegin).second;
				(*itBegin).second = 0;

				itBegin++;
			}
		}
	}

	void Function::set (std::string variableName, Variable *value, NativeCallback function)
	{
		unsigned long ulHash = RadXmlCommon::s13StringToHash (variableName);
		hshVariables.insert (std::pair<unsigned long, Variable *> (ulHash, value));
		hshFunctions.insert (std::pair<unsigned long, NativeCallback> (ulHash, function));
	}

	void Function::executeCallback (std::string variableName, Variable *thisObject, Variable *arguments)
	{
		unsigned long ulHash = RadXmlCommon::s13StringToHash (variableName);
		HASHMAP<unsigned long, NativeCallback>::iterator itBegin = hshFunctions.find (ulHash);
		HASHMAP<unsigned long, NativeCallback>::iterator itEnd = hshFunctions.end ();

		if (itBegin != itEnd)
			((*itBegin).second) (thisObject, arguments);
	}

	void Function::executeCallback (unsigned int index, Variable *thisObject, Variable *arguments)
	{
		HASHMAP<unsigned long, NativeCallback>::iterator itBegin = hshFunctions.begin ();

		for (unsigned int iIdx = 0; iIdx < hshFunctions.size (); iIdx++)
		{
			if (iIdx == index)
				break;

			itBegin++;
		}

		((*itBegin).second) (thisObject, arguments);
	}

	Variable *Function::get (std::string variableName)
	{
		unsigned long ulHash = RadXmlCommon::s13StringToHash (variableName);
		HASHMAP<unsigned long, Variable *>::iterator itBegin = hshVariables.find (ulHash);
		HASHMAP<unsigned long, Variable *>::iterator itEnd = hshVariables.end ();

		if (itBegin != itEnd)
			return ((*itBegin).second);

		return (0);
	}

	Variable *Function::get (unsigned int index)
	{
		HASHMAP<unsigned long, Variable *>::iterator itBegin = hshVariables.begin ();

		for (unsigned int iIdx = 0; iIdx < hshVariables.size (); iIdx++)
		{
			if (iIdx == index)
				break;

			itBegin++;
		}

		return ((*itBegin).second);
	}

	NativeCallback Function::getCallback (std::string variableName)
	{
		unsigned long ulHash = RadXmlCommon::s13StringToHash (variableName);
		HASHMAP<unsigned long, NativeCallback>::iterator itBegin = hshFunctions.find (ulHash);
		HASHMAP<unsigned long, NativeCallback>::iterator itEnd = hshFunctions.end ();

		if (itBegin != itEnd)
			return ((*itBegin).second);

		return (0);
	}

	NativeCallback Function::getCallback (unsigned int index)
	{
		HASHMAP<unsigned long, NativeCallback>::iterator itBegin = hshFunctions.begin ();

		for (unsigned int iIdx = 0; iIdx < hshFunctions.size (); iIdx++)
		{
			if (iIdx == index)
				break;

			itBegin++;
		}

		return ((*itBegin).second);
	}

	// JavascriptManager
	JavascriptManager::JavascriptManager ()
	{
	}

	JavascriptManager::~JavascriptManager ()
	{
		HASHMAP<unsigned long, Variable *>::iterator itBegin = hshVariables.begin ();
		HASHMAP<unsigned long, Variable *>::iterator itEnd = hshVariables.end ();

		while (itBegin != itEnd)
		{
			delete (*itBegin).second;
			(*itBegin).second = 0;

			itBegin++;
		}
	}

	void JavascriptManager::initialise ()
	{
		if (rxSingleton == 0)
			rxSingleton = new JavascriptManager ();
	}

	void JavascriptManager::destroy ()
	{
		if (rxSingleton != 0)
		{
			delete rxSingleton;
			rxSingleton = 0;
		}
	}

	void JavascriptManager::set (std::string variableName, Variable *value, NativeCallback function)
	{
		unsigned long ulHash = RadXmlCommon::s13StringToHash (variableName);
		hshVariables.insert (std::pair<unsigned long, Variable *> (ulHash, value));
		hshFunctions.insert (std::pair<unsigned long, NativeCallback> (ulHash, function));
	}

	void JavascriptManager::executeCallback (std::string variableName, Variable *thisObject, Variable *arguments)
	{
		unsigned long ulHash = RadXmlCommon::s13StringToHash (variableName);
		HASHMAP<unsigned long, NativeCallback>::iterator itBegin = hshFunctions.find (ulHash);
		HASHMAP<unsigned long, NativeCallback>::iterator itEnd = hshFunctions.end ();

		if (itBegin != itEnd)
			((*itBegin).second) (thisObject, arguments);
	}

	void JavascriptManager::executeCallback (unsigned int index, Variable *thisObject, Variable *arguments)
	{
		HASHMAP<unsigned long, NativeCallback>::iterator itBegin = hshFunctions.begin ();

		for (unsigned int iIdx = 0; iIdx < hshFunctions.size (); iIdx++)
		{
			if (iIdx == index)
				break;

			itBegin++;
		}

		((*itBegin).second) (thisObject, arguments);
	}

	Variable *JavascriptManager::get (std::string variableName)
	{
		unsigned long ulHash = RadXmlCommon::s13StringToHash (variableName);
		HASHMAP<unsigned long, Variable *>::iterator itBegin = hshVariables.find (ulHash);
		HASHMAP<unsigned long, Variable *>::iterator itEnd = hshVariables.end ();

		if (itBegin != itEnd)
			return ((*itBegin).second);

		return (0);
	}

	Variable *JavascriptManager::get (unsigned int index)
	{
		HASHMAP<unsigned long, Variable *>::iterator itBegin = hshVariables.begin ();

		for (unsigned int iIdx = 0; iIdx < hshVariables.size (); iIdx++)
		{
			if (iIdx == index)
				break;

			itBegin++;
		}

		return ((*itBegin).second);
	}

	NativeCallback JavascriptManager::getFunction (std::string variableName)
	{
		unsigned long ulHash = RadXmlCommon::s13StringToHash (variableName);
		HASHMAP<unsigned long, NativeCallback>::iterator itBegin = hshFunctions.find (ulHash);
		HASHMAP<unsigned long, NativeCallback>::iterator itEnd = hshFunctions.end ();

		if (itBegin != itEnd)
			return ((*itBegin).second);

		return (0);
	}

	NativeCallback JavascriptManager::getFunction (unsigned int index)
	{
		HASHMAP<unsigned long, NativeCallback>::iterator itBegin = hshFunctions.begin ();

		for (unsigned int iIdx = 0; iIdx < hshFunctions.size (); iIdx++)
		{
			if (iIdx == index)
				break;

			itBegin++;
		}

		return ((*itBegin).second);
	}

	// Plugin
	void Plugin::setName (std::string newName)
	{
		name = newName;
		hash = RadXmlCommon::s13StringToHash (newName);
	}

	// PluginManager
	void PluginManager::initialise ()
	{
		if (rxSingleton == 0)
			rxSingleton = new PluginManager ();
	}

	void PluginManager::destroy ()
	{
		if (rxSingleton != 0)
		{
			delete rxSingleton;
			rxSingleton = 0;
		}
	}

	PluginManager::PluginManager ()
	{
	}

	PluginManager::~PluginManager ()
	{
		HASHMAP<unsigned long, Plugin *>::iterator itBegin = hshPlugins.begin ();
		HASHMAP<unsigned long, Plugin *>::iterator itEnd = hshPlugins.end ();

		while (itBegin != itEnd)
		{
			Plugin *plgPlugin = (*itBegin).second;

			if (plgPlugin != 0)
			{
				delete plgPlugin;
				plgPlugin = 0;
			}

			itBegin++;
		}
	}

	void PluginManager::addPlugin (Plugin *newPlugin)
	{
		hshPlugins.insert (std::pair<unsigned long, Plugin *> (newPlugin->getHash (), newPlugin));
	}

	Plugin *PluginManager::getPlugin (std::string findName)
	{
		unsigned long ulHash = RadXmlCommon::s13StringToHash (findName);
		HASHMAP<unsigned long, Plugin *>::iterator itBegin = hshPlugins.find (ulHash);
		HASHMAP<unsigned long, Plugin *>::iterator itEnd = hshPlugins.end ();

		if (itBegin != itEnd)
			return ((*itBegin).second);

		return (0);
	}

	Plugin *PluginManager::getPlugin (unsigned int index)
	{
		unsigned int iCount = 0;
		HASHMAP<unsigned long, Plugin *>::iterator itBegin = hshPlugins.begin ();
		HASHMAP<unsigned long, Plugin *>::iterator itEnd = hshPlugins.end ();

		while (itBegin != itEnd)
		{
			if (iCount == index)
				return ((*itBegin).second);

			itBegin++;
			iCount++;
		}

		return (0);
	}

	void PluginManager::removePlugin (std::string name)
	{
		unsigned long ulHash = RadXmlCommon::s13StringToHash (name);
		HASHMAP<unsigned long, Plugin *>::iterator itBegin = hshPlugins.find (ulHash);
		HASHMAP<unsigned long, Plugin *>::iterator itEnd = hshPlugins.end ();

		if (itBegin != itEnd)
		{
			Plugin *plgPlugin = (*itBegin).second;

			if (plgPlugin != 0)
			{
				delete plgPlugin;
				plgPlugin = 0;
			}

			hshPlugins.erase (itBegin);
		}
	}
}

