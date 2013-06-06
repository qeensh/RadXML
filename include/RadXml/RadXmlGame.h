#ifndef _RADXML_GAME_H_
	#define _RADXML_GAME_H_

	#ifndef HASHMAP
		#ifdef __GNUC__
			#include <ext/hash_map>

			namespace std
			{
				using namespace __gnu_cxx;
			}

			#define HASHMAP	std::hash_map

			/// @note You may have to use this for upcoming GCC compilers for hash maps
			/// in the mean time, you may want to compile with -Wno-deprecated on compilers
			/// that complain about a deprecated file.
			/// #include <unordered_map>
			/// #define HASHMAP	std::tr1::unordered_map
		#else
			#include <hash_map>

			#define HASHMAP	stdext::hash_map
		#endif
	#endif

	#include <string>
	#include <vector>

	class XMLElement;

	#include "RadXmlNative.h"
	#include "RadXmlPreprocessor.h"
	#include "RadXmlLevel.h"

	namespace RadXml
	{
		class XmlFile;
		class Movable;
		class Level;

		enum GameObjectTypes
		{
			GOT_ENTITY			= 1,
			GOT_CAMERA			= 2,
			GOT_PARTICLE_SYSTEM	= 3,
			GOT_LIGHT			= 4,
			GOT_ANIMATION_TRACK	= 5,
			GOT_AUDIO_LISTENER	= 6,
			GOT_SOUND			= 7,
			GOT_OVERLAY			= 8,
			GOT_OVERLAY_ELEMENT	= 9
		};

		class defRADXML_EXPORT Game: public Level
		{
			public:
				Game::Game (std::string newName);
				~Game ();

				void destroyAllGameObjects ();

				void setName (std::string newName);
				inline std::string getName ()
				{
					return (gameName);
				}

				inline unsigned long getHash ()
				{
					return (gameHash);
				}

				void parse (XmlFile *file, XMLElement *element);

				void addMovableForDeletion (Movable *object);

			protected:
				std::string gameName;
				unsigned long gameHash;

				std::vector<Movable *> aryMovablesToDelete;

				HASHMAP<unsigned long, Level *> hshLevels;
		};

		class GameClass
		{
			public:
				static void setupGameClass ();

				static Variable *getMovableByName (Variable *thisObject, Variable *arguments);
				static Variable *createObject (Variable *thisObject, Variable *arguments);
				static Variable *deleteObject (Variable *thisObject, Variable *arguments);
				static Variable *getObjectsFromRayCast (Variable *thisObject, Variable *arguments);
		};
	}
#endif

