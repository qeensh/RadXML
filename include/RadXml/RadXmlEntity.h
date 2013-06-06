#ifndef _RADXML_ENTITY_H_
	#define _RADXML_ENTITY_H_

	#ifdef defAPI_OGRE3D
		#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
			#include <OGRE/OgreEntity.h>
		#else
			#include <OgreEntity.h>
		#endif
	#endif

	#include <string>
	#include <vector>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlMovable.h"

	namespace RadXml
	{
		class XmlFile;
		class Level;

		class defRADXML_EXPORT Entity: public Movable
		{
			public:
				Entity (std::string newName);
				~Entity ();

				void create (Movable *parent = 0);
				void destroy ();

				void setVisible (bool visible);
				bool getVisible ();

				Movable *clone (std::string newName, Movable *parent = 0);

				void parseXML (XmlFile *level, XMLElement *element, Movable *parent = 0);

				void setModel (std::string newModel);
				inline std::string getModel ()
				{
					return (model);
				}

				inline Ogre::Entity *getEntity ()
				{
					return (eObj);
				}

				void setMaterial (std::string newMaterial);
				inline std::string getMaterial ()
				{
					return (material);
				}

			protected:
				Ogre::Entity *eObj;

				std::string model;
				std::string material;
		};

		class EntityClass
		{
			public:
				static void setupEntityClass ();

				static Variable *setModel (Variable *thisObject, Variable *arguments);
				static Variable *getModel (Variable *thisObject, Variable *arguments);
				static Variable *setMaterial (Variable *thisObject, Variable *arguments);
				static Variable *getMaterial (Variable *thisObject, Variable *arguments);
				static Variable *getCollidingObject (Variable *thisObject, Variable *arguments);
		};
	}
#endif

