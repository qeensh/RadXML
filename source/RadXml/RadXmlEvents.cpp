#include "RadXmlEvents.h"

#include "RadXml.h"

#include "common/com_radxml_strconv.h"

namespace RadXml
{
	// Event
	Event::Event (std::string newName, std::string newJavascript)
	{
		setName (newName);
		setJavascript (newJavascript);
	}

	void Event::setName (std::string newName)
	{
		name = newName;
		hash = RadXmlCommon::s13StringToHash (newName);
	}

	// Events
	Events::Events ()
	{
	}

	Events::Events (const Events &copy)
	{
		HASHMAP<unsigned long, Event *>::const_iterator itBegin = copy.hshEvents.begin ();
		HASHMAP<unsigned long, Event *>::const_iterator itEnd = copy.hshEvents.end ();

		while (itBegin != itEnd)
		{
			Event *evtEvent = (*itBegin).second;
			Event *evtClone = new Event (*evtEvent);

			hshEvents.insert (std::pair<unsigned long, Event *> (evtClone->getHash (), evtClone));

			itBegin++;
		}
	}

	Events::~Events ()
	{
		HASHMAP<unsigned long, Event *>::iterator itBegin = hshEvents.begin ();
		HASHMAP<unsigned long, Event *>::iterator itEnd = hshEvents.end ();

		while (itBegin != itEnd)
		{
			Event *evtEvent = (*itBegin).second;

			if (evtEvent != 0)
			{
				delete evtEvent;
				evtEvent = 0;
			}

			itBegin++;
		}
	}

	void Events::addEvent (Event *newEvent)
	{
		hshEvents.insert (std::pair<unsigned long, Event *> (newEvent->getHash (), newEvent));
	}

	Event *Events::getEvent (std::string findName)
	{
		unsigned long ulHash = RadXmlCommon::s13StringToHash (findName);
		HASHMAP<unsigned long, Event *>::iterator itBegin = hshEvents.find (ulHash);
		HASHMAP<unsigned long, Event *>::iterator itEnd = hshEvents.end ();

		if (itBegin != itEnd)
			return ((*itBegin).second);

		return (0);
	}

	Event *Events::getEvent (unsigned long findHash)
	{
		HASHMAP<unsigned long, Event *>::iterator itBegin = hshEvents.find (findHash);
		HASHMAP<unsigned long, Event *>::iterator itEnd = hshEvents.end ();

		if (itBegin != itEnd)
			return ((*itBegin).second);

		return (0);
	}

	void Events::setJavascriptToEvent (std::string findName, std::string javascript)
	{
		Event *evtEvent = getEvent (findName);
		evtEvent->setJavascript (javascript);
	}

	void Events::setJavascriptToEvent (unsigned long findHash, std::string javascript)
	{
		Event *evtEvent = getEvent (findHash);
		evtEvent->setJavascript (javascript);
	}

	std::string Events::getJavascriptFromEvent (std::string findName)
	{
		Event *evtEvent = getEvent (findName);

		if (evtEvent == 0)
			return ("");

		return (evtEvent->getJavascript ());
	}

	std::string Events::getJavascriptFromEvent (unsigned long findHash)
	{
		Event *evtEvent = getEvent (findHash);

		if (evtEvent == 0)
			return (0);

		return (evtEvent->getJavascript ());
	}

	bool Events::executeJavascriptFromEvent (std::string findName)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		std::string javascript = getJavascriptFromEvent (findName);

		if (javascript == "")
			return (true);

		return (ogrGame->compileAndRunJSString (javascript));
	}

	bool Events::executeJavascriptFromEvent (unsigned long findHash)
	{
		std::string javascript = getJavascriptFromEvent (findHash);

		if (javascript == "")
			return (true);

		RadXML *ogrGame = RadXML::getSingletonPtr ();

		return (ogrGame->compileAndRunJSString (javascript));
	}
}

