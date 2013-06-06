#include "RadXmlParticleSystem.h"
#include "RadXmlSingleton.h"

#include "RadXml.h"
#include "RadXmlXmlFile.h"

#include "xml/xml.h"

namespace RadXml
{
	// ParticleSystem
	ParticleSystem::ParticleSystem (std::string newName)
		: Movable (newName)
	{
		psSystem = 0;
		particleSystemName = "";

		iType = GOT_PARTICLE_SYSTEM;
	}

	ParticleSystem::~ParticleSystem ()
	{
		destroy ();
	}

	void ParticleSystem::parseXML (XmlFile *level, XMLElement *element, Movable *parent)
	{
		parseMovableXML (level, element, parent);

		std::string strSystem = element->getAttributeValue ("system");

		particleSystemName = strSystem;
	}

	void ParticleSystem::setVisible (bool visible)
	{
		bVisible = visible;

		if (psSystem != 0)
			psSystem->setVisible (visible);
	}

	bool ParticleSystem::getVisible ()
	{
		return (bVisible);
	}

	Movable *ParticleSystem::clone (std::string newName, Movable *parent)
	{
		RadXML *ogrGame = RadXML::getSingletonPtr ();
		ParticleSystem *clone = new ParticleSystem (*this);

		clone->setName (newName);
		clone->create (parent);
		clone->removeAllChildren ();

		for (unsigned int iIdx = 0; iIdx < getNumChildren (); iIdx++)
		{
			Movable *mChild = getChild (iIdx);
			Movable *mSubClone = mChild->clone (clone->getName () + "_sub", clone);
		}

		ogrGame->getCurrentXMLFile ()->addMovable (clone);

		return (clone);
	}

	void ParticleSystem::create (Movable *parent)
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();
		psSystem = mSceneMgr->createParticleSystem ("psParticle" + name, particleSystemName);

		psSystem->setVisible (bVisible);
		psSystem->setQueryFlags (defENTITY_OBJECT);
		psSystem->setUserAny (Ogre::Any (this));

		setParent (parent);
		createSceneNode (psSystem, parent);
	}

	void ParticleSystem::destroy ()
	{
		Ogre::SceneManager *mSceneMgr = RadXML::getSingletonPtr ()->getSceneManager ();

		destroySceneNode ();

		if (psSystem != 0)
		{
			mSceneMgr->destroyParticleSystem (psSystem);
			psSystem = 0;
		}
	}
}

