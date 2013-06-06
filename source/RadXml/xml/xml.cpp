// parser.cpp
// Modified by:
// Nathanael Coonrod
// ncoonrod@highersoftware.com
// Higher Edge Software, LLC
// www.highersoftware.com
// Modified: August 10, 2011

#include "xml/xml.h"

XMLElement::XMLElement ()
{
	parent = 0;
	parentParser = 0;
	children = new std::vector<XMLElement *> ();

	tag = "";
	content = "";
}

XMLElement::~XMLElement ()
{
	if (children != 0)
	{
		for (unsigned int iIdx = 0; iIdx < children->size (); iIdx++)
		{
			delete children->at (iIdx);
			children->at (iIdx) = 0;
		}

		delete children;
		children = 0;
	}
}

XMLElement *XMLElement::findTag (std::string tagToFind, XMLElement *inElement)
{
	XMLElement *xmlSearch = inElement;

	if (xmlSearch == 0)
		xmlSearch = this;

	if (xmlSearch->tag == tagToFind)
		return (xmlSearch);
	else
	{
		for (unsigned int iIdx = 0; iIdx < xmlSearch->children->size (); iIdx++)
		{
			XMLElement *xmlTemp = findTag (tagToFind, xmlSearch->children->at (iIdx));

			if (xmlTemp != 0)
				return (xmlTemp);
		}
	}

	return (0);
}

std::vector<XMLElement *> XMLElement::findAllTags (std::string tagToFind, XMLElement *inElement, bool traverseDown)
{
	std::vector<XMLElement *> aryFound;
	XMLElement *xmlSearch = inElement;

	if (xmlSearch == 0)
		xmlSearch = this;

	for (unsigned int iIdx = 0; iIdx < xmlSearch->children->size (); iIdx++)
	{
		XMLElement *xmlCurrent = xmlSearch->children->at (iIdx);

		if (traverseDown == true)
		{
			XMLElement *xmlElement = xmlCurrent->findTag (tagToFind, xmlCurrent);

			if (xmlElement != 0)
				aryFound.push_back (xmlElement);
		}
		else
		{
			if (xmlCurrent->tag == tagToFind)
				aryFound.push_back (xmlCurrent);
		}
	}

	return (aryFound);
}

std::string XMLElement::getAttributeValue (std::string attributeName)
{
	for (unsigned int iIdx = 0; iIdx < aryAttributeNames.size (); iIdx++)
	{
		std::string strAttributeName = aryAttributeNames.at (iIdx);
		std::string strAttributeValue = aryAttributeValues.at (iIdx);

		if (strAttributeName == attributeName)
			return (strAttributeValue);
	}

	return ("");
}

// XMLParser
XMLParser::XMLParser ()
{
	aryElements = new std::vector<XMLElement *> ();
	strFilename = "";
}
XMLParser::XMLParser (std::string file)
{
	aryElements = new std::vector<XMLElement *> ();
	parseFile (file);
}

XMLParser::~XMLParser ()
{
	if (aryElements != 0)
	{
		for (unsigned int iIdx = 0; iIdx < aryElements->size (); iIdx++)
		{
			delete aryElements->at (iIdx);
			aryElements->at (iIdx) = 0;
		}

		delete aryElements;
		aryElements = 0;
	}
}

XMLElement *XMLParser::findTag (std::string tagToFind)
{
	for (unsigned int iIdx = 0; iIdx < aryElements->size (); iIdx++)
	{
		XMLElement *xmlElement = aryElements->at (iIdx)->findTag (tagToFind);

		if (xmlElement != 0)
			return (xmlElement);
	}

	return (0);
}

std::vector<XMLElement *> XMLParser::findAllTags (std::string tagToFind)
{
	std::vector<XMLElement *> aryFound;

	for (unsigned int iIdx = 0; iIdx < aryElements->size (); iIdx++)
	{
		XMLElement *xmlCurrent = aryElements->at (iIdx);
		XMLElement *xmlElement = xmlCurrent->findTag (tagToFind);

		if (xmlElement != 0)
			aryFound.push_back (xmlElement);
	}

	return (aryFound);
}

bool XMLParser::parseFile (std::string file)
{
	strFilename = file;

	#ifdef defAPI_TINY_XML
		if (tdFile.LoadFile(file.c_str ()) == false)
			return (false);

		navigateChildren (0, 0, tdFile.RootElement ());
	#endif

	return (true);
}

void XMLParser::navigateChildren (int depth, XMLElement *parent, TiXmlElement *element)
{
	#ifdef defAPI_TINY_XML
		TiXmlElement *xeElm = element;
		xeElm = xeElm->FirstChildElement ();

		while (xeElm != 0)
		{
			XMLElement *xmlElment = new XMLElement ();
			const char *cParentValue = (const char *)element->Value ();
			const char *cValue = (const char *)xeElm->Value ();
			const char *cContent = 0;
			std::string strParentValue = "";
			std::string strValue = "";
			std::string strContent = "";

			if (xeElm->NoChildren () == false)
			{
				TiXmlNode *xeChild = xeElm->FirstChild ();

				if (xeChild != 0)
					cContent = xeChild->Value ();
			}

			if (cParentValue != 0)
				strParentValue = cParentValue;

			if (cValue != 0)
				strValue = cValue;

			if (cContent != 0)
				strContent = cContent;

			std::vector<std::string> aryAttributeNames;
			std::vector<std::string> aryAttributeValues;
			TiXmlAttribute *xaAtt = xeElm->FirstAttribute ();

			while (xaAtt != 0)
			{
				const char *cAttributeName = (const char *)xaAtt->Name ();
				std::string strAttributeName = "";
				const char *cAttributeValue = (const char *)xaAtt->Value ();
				std::string strAttributeValue = "";

				if (cAttributeName != 0)
					strAttributeName = cAttributeName;

				if (cAttributeValue != 0)
					strAttributeValue = cAttributeValue;

				aryAttributeNames.push_back (strAttributeName);
				aryAttributeValues.push_back (strAttributeValue);

				xaAtt = xaAtt->Next ();
			}

			xmlElment->parent = parent;
			xmlElment->tag = strValue;

			for (unsigned int iIdx = 0; iIdx < aryAttributeNames.size (); iIdx++)
				xmlElment->aryAttributeNames.push_back (aryAttributeNames.at (iIdx));

			for (unsigned int iIdx = 0; iIdx < aryAttributeValues.size (); iIdx++)
				xmlElment->aryAttributeValues.push_back (aryAttributeValues.at (iIdx));

			xmlElment->content = strContent;
			xmlElment->parentParser = this;

			if (parent == 0)
				aryElements->push_back (xmlElment);
			else
				parent->children->push_back (xmlElment);

			if (xeElm->NoChildren () == false)
				navigateChildren (depth++, xmlElment, xeElm);

			xeElm = xeElm->NextSiblingElement ();
		}
	#endif
}

