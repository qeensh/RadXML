#ifndef _RADXML_LEVEL_H_
	#define _RADXML_LEVEL_H_

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

	#include "RadXmlPreprocessor.h"
	#include "RadXmlEvents.h"

	class XMLElement;

	namespace RadXml
	{
		class XmlFile;
		class Movable;
		class Sound;
		class AnimationTrackObject;
		class Overlay;
		class OverlayElement;

		class defRADXML_EXPORT Level: public Events
		{
			public:
				Level ();
				~Level ();

				void setName (std::string newName);
				inline std::string getName ()
				{
					return (name);
				}

				inline unsigned long getHash ()
				{
					return (hashName);
				}

				void addMusic (Sound *music);

				void parseLevel (XmlFile *file, XMLElement *element);
				void parseXMLMovables (XmlFile *file, XMLElement *element, Movable *parent = 0);
				void parseXMLMovable (XmlFile *file, int type, XMLElement *element, Movable *parent = 0);

				Movable *createMovable (XmlFile *file, int type, std::string name, Movable *parent, XMLElement *element = 0, bool createNow = true);
				void addMovable (Movable *newMovable);
				Movable *getMovable (std::string findName, bool skipErrorMessage = false);
				Movable *getMovable (unsigned long hash, bool skipErrorMessage = false);
				void removeMovable (std::string findName);
				void removeMovable (unsigned long hash);

				inline std::vector<AnimationTrackObject *> getAnimations ()
				{
					return (aryAnimations);
				}

				inline std::vector<Sound *> getAllMusic ()
				{
					return (aryMusic);
				}

				inline HASHMAP<unsigned long, Movable *> getAllMovables ()
				{
					return (hshMovables);
				}

				inline std::vector<Overlay *> getOverlays ()
				{
					return (aryOverlays);
				}

				inline std::vector<OverlayElement *> getOverlayElements ()
				{
					return (aryOverlayElements);
				}

			protected:
				std::string name;
				unsigned long hashName;

				std::vector<AnimationTrackObject *> aryAnimations;
				std::vector<Sound *> aryMusic;
				std::vector<Overlay *> aryOverlays;
				std::vector<OverlayElement *> aryOverlayElements;

				HASHMAP<unsigned long, Movable *> hshMovables;
		};
	}
#endif

