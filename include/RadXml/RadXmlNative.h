#ifndef _RADXML_NATIVE_H_
	#define _RADXML_NATIVE_H_

	#include <string>
	#include <vector>

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

	#ifdef defAPI_JAVASCRIPT_V8
		#include <v8.h>
	#endif

	#include "RadXmlPreprocessor.h"

	#include "RadXmlSingleton.h"

	namespace RadXml
	{
		enum VariableTypes
		{
			VT_NONE			= 0,
			VT_BOOLEAN		= 1,
			VT_NUMBER		= 2,
			VT_STRING		= 3,
			VT_ARRAY		= 4,
			VT_CLASS		= 5,
			VT_CLASS_OBJECT	= 6,
			VT_OBJECT		= 7,
			VT_FUNCTION		= 8
		};

		class Boolean;
		class Number;
		class String;
		class Array;
		class Class;
		class Function;

		class defRADXML_EXPORT Variable
		{
			public:
				inline Variable (std::string newName)
				{
					type = VT_NONE;
					name = newName;
					destroyHiddenData = true;
				}

				virtual ~Variable ();

				inline int getType ()
				{
					return (type);
				}

				inline void setName (std::string newName)
				{
					name = newName;
				}

				inline std::string getName ()
				{
					return (name);
				}

				void addHiddenData (std::string nameOfData, void *data);
				void *getHiddenDataByIndex (unsigned int index);
				std::string getHiddenDataNameByIndex (unsigned int index);
				void *getHiddenData (std::string nameOfData);
				bool hasHiddenData (std::string nameOfData);
				void removeHiddenData (std::string nameOfData);

				inline void setDestroyHiddenData (bool destroyData)
				{
					destroyHiddenData = destroyData;
				}

				inline bool getDestroyHiddenData ()
				{
					return (destroyHiddenData);
				}

				inline unsigned int getNumHiddenData ()
				{
					return (hshHidden.size ());
				}

				inline Boolean *toBoolean ()
				{
					return ((Boolean *)this);
				}

				inline Number *toNumber ()
				{
					return ((Number *)this);
				}

				inline String *toString ()
				{
					return ((String *)this);
				}

				inline Array *toArray ()
				{
					return ((Array *)this);
				}

				inline Class *toClass ()
				{
					return ((Class *)this);
				}

				inline Function *toFunction ()
				{
					return ((Function *)this);
				}

			protected:
				int type;
				std::string name;
				bool destroyHiddenData;

				HASHMAP<std::string, void *> hshHidden;
		};

		class defRADXML_EXPORT Boolean: public Variable
		{
			public:
				inline Boolean (std::string newName)
					: Variable (newName)
				{
					value = false;
					type = VT_BOOLEAN;
				}

				inline Boolean (std::string newName, bool newValue)
					: Variable (newName)
				{
					value = false;
					type = VT_BOOLEAN;
					value = newValue;
				}

				inline void setValue (bool newValue)
				{
					value = newValue;
				}

				inline bool getValue ()
				{
					return (value);
				}

				static inline Boolean *New (std::string newName, bool newValue)
				{
					return (new Boolean (newName, newValue));
				}

				static inline Boolean *New (bool newValue)
				{
					return (new Boolean ("", newValue));
				}

			protected:
				bool value;
		};

		class defRADXML_EXPORT Number: public Variable
		{
			public:
				inline Number (std::string newName)
					: Variable (newName)
				{
					value = 0.0;
					type = VT_NUMBER;
				}

				inline Number (std::string newName, double newValue)
					: Variable (newName)
				{
					value = 0.0;
					type = VT_NUMBER;
					value = newValue;
				}

				inline void setValue (double newValue)
				{
					value = newValue;
				}

				inline double getValue ()
				{
					return (value);
				}

				static inline Number *New (std::string newName, double newValue)
				{
					return (new Number (newName, newValue));
				}

				static inline Number *New (double newValue)
				{
					return (new Number ("", newValue));
				}

			protected:
				double value;
		};

		class defRADXML_EXPORT String: public Variable
		{
			public:
				inline String (std::string newName)
					: Variable (newName)
				{
					value = "";
					type = VT_STRING;
				}

				inline String (std::string newName, std::string newValue)
					: Variable (newName)
				{
					value = "";
					type = VT_STRING;
					value = newValue;
				}

				inline void setValue (std::string newValue)
				{
					value = newValue;
				}

				inline std::string getValue ()
				{
					return (value);
				}

				static inline String *New (std::string newName, std::string newValue)
				{
					return (new String (newName, newValue));
				}

				static inline String *New (std::string newValue)
				{
					return (new String ("", newValue));
				}

			protected:
				std::string value;
		};

		class defRADXML_EXPORT Array: public Variable
		{
			public:
				inline Array (std::string newName)
					: Variable (newName)
				{
					type = VT_ARRAY;
					destroyData = true;
				}

				inline Array (std::string newName, Variable *element)
					: Variable (newName)
				{
					type = VT_ARRAY;
					destroyData = true;
					value.push_back (element);
				}

				inline ~Array ()
				{
					if (destroyData == true)
					{
						for (unsigned int iIdx = 0; iIdx < value.size (); iIdx++)
						{
							delete value.at (iIdx);
							value.at (iIdx) = 0;
						}
					}
				}

				inline void addElement (Variable *element)
				{
					value.push_back (element);
				}

				inline void setElement (unsigned int index, Variable *element)
				{
					value.at (index) = element;
				}

				inline void insertElement (unsigned int index, Variable *element)
				{
					std::vector<Variable *>::iterator itBegin = value.begin ();
					itBegin += index;
					value.insert (itBegin, element);
				}

				inline void removeElement (unsigned int index)
				{
					std::vector<Variable *>::iterator itBegin = value.begin ();
					itBegin += index;
					value.erase (itBegin);
				}

				inline Variable *getElement (unsigned int index)
				{
					if (index >= value.size ())
						return (0);

					return (value.at (index));
				}

				inline void setValue (std::vector<Variable *> newValue)
				{
					value = newValue;
				}

				inline std::vector<Variable *> getValue ()
				{
					return (value);
				}

				inline unsigned int size ()
				{
					return (value.size ());
				}

				inline unsigned int length ()
				{
					return (value.size ());
				}

				inline void setDestroyData (bool destroy)
				{
					destroyData = destroy;
				}

				inline bool getDestroyData ()
				{
					return (destroyData);
				}

				static inline Array *New (std::string newName, Variable *element)
				{
					return (new Array (newName, element));
				}

				static inline Array *New (Variable *element)
				{
					return (new Array ("", element));
				}

			protected:
				bool destroyData;

				std::vector<Variable *> value;
		};

		class defRADXML_EXPORT Class: public Variable
		{
			public:
				inline Class (std::string newName, std::string newTypeName)
					: Variable (newName)
				{
					type = VT_CLASS;
					typeName = newTypeName;
					destroyData = true;
				}

				~Class ();

				void set (std::string variableName, Variable *value);
				Variable *get (std::string variableName);
				Variable *get (unsigned int index);

				inline void setTypeName (std::string newTypeName)
				{
					typeName = newTypeName;
				}

				inline std::string getTypeName ()
				{
					return (typeName);
				}

				inline unsigned int size ()
				{
					return (hshVariables.size ());
				}

				inline unsigned int length ()
				{
					return (hshVariables.size ());
				}

				inline void setDestroyData (bool destroy)
				{
					destroyData = destroy;
				}

				inline bool getDestroyData ()
				{
					return (destroyData);
				}

				static inline Class *New (std::string newName, std::string newTypeName)
				{
					return (new Class (newName, newTypeName));
				}

				static inline Class *New ()
				{
					return (new Class ("", ""));
				}

			protected:
				std::string typeName;
				bool destroyData;

				HASHMAP<unsigned long, Variable *> hshVariables;
		};

		class defRADXML_EXPORT ClassObject: public Class
		{
			public:
				inline ClassObject (std::string newName, std::string newTypeName)
					: Class (newName, newTypeName)
				{
					type = VT_CLASS_OBJECT;
				}
		};

		typedef Variable *(*NativeCallback) (Variable *thisObject, Variable *arguments);

		class defRADXML_EXPORT Function: public Class
		{
			public:
				inline Function (std::string newName)
					: Class (newName, "")
				{
					type = VT_FUNCTION;
					destroyData = true;
				}

				inline Function (std::string newName, Variable *value, NativeCallback function = 0)
					: Class (newName, "")
				{
					type = VT_FUNCTION;
					destroyData = true;
					set (newName, value, function);
				}

				~Function ();

				void set (std::string variableName, Variable *value, NativeCallback function = 0);
				void executeCallback (std::string variableName, Variable *thisObject, Variable *arguments);
				void executeCallback (unsigned int index, Variable *thisObject, Variable *arguments);
				Variable *get (std::string variableName);
				Variable *get (unsigned int index);
				NativeCallback getCallback (std::string variableName);
				NativeCallback getCallback (unsigned int index);

				inline unsigned int size ()
				{
					return (hshVariables.size ());
				}

				inline unsigned int length ()
				{
					return (hshVariables.size ());
				}

				inline void setDestroyData (bool destroy)
				{
					destroyData = destroy;
				}

				inline bool getDestroyData ()
				{
					return (destroyData);
				}

				static inline Function *New (std::string newName, NativeCallback function)
				{
					return (new Function (newName, 0, function));
				}

				static inline Function *New (NativeCallback function)
				{
					return (new Function ("", 0, function));
				}

			protected:
				bool destroyData;

				HASHMAP<unsigned long, NativeCallback> hshFunctions;
		};

		class AppObject;
		class Movable;

		class defRADXML_EXPORT RadXmlObject: public Variable
		{
			public:
				inline RadXmlObject (std::string newName, AppObject *object)
					: Variable (newName)
				{
					type = VT_OBJECT;
					aoObject = object;
					mMovable = 0;
				}

				inline RadXmlObject (std::string newName, Movable *object)
					: Variable (newName)
				{
					type = VT_OBJECT;
					aoObject = 0;
					mMovable = object;
				}

				inline void setObject (AppObject *object)
				{
					aoObject = object;
				}

				inline void setObject (Movable *object)
				{
					mMovable = object;
				}

				inline AppObject *getAppObject ()
				{
					return (aoObject);
				}

				inline Movable *getMovableObject ()
				{
					return (mMovable);
				}

			protected:
				AppObject *aoObject;
				Movable *mMovable;
		};

		class defRADXML_EXPORT JavascriptManager: public Singleton<JavascriptManager>
		{
			public:
				static void initialise ();
				static void destroy ();

				void set (std::string variableName, Variable *value, NativeCallback function = 0);
				void executeCallback (std::string variableName, Variable *thisObject, Variable *arguments);
				void executeCallback (unsigned int index, Variable *thisObject, Variable *arguments);
				Variable *get (std::string variableName);
				Variable *get (unsigned int index);
				NativeCallback getFunction (std::string variableName);
				NativeCallback getFunction (unsigned int index);

				inline unsigned int size ()
				{
					return (hshVariables.size ());
				}

				inline unsigned int length ()
				{
					return (hshVariables.size ());
				}

				inline unsigned int numOfFunctions ()
				{
					return (hshFunctions.size ());
				}

			protected:
				JavascriptManager ();
				~JavascriptManager ();

				HASHMAP<unsigned long, Variable *> hshVariables;
				HASHMAP<unsigned long, NativeCallback> hshFunctions;
		};

		enum PluginTypes
		{
			PLT_TAG, 
			PLT_JAVASCRIPT_API
		};

		class defRADXML_EXPORT Plugin
		{
			public:
				inline Plugin (std::string pluginName, int pluginType, std::string pluginPath)
				{
					description = "";
					version = 0.0;
					type = pluginType;
					path = pluginPath;
					copyright = "";
					developer = "";

					setName (pluginName);
				}

				void setName (std::string newName);

				inline std::string getName ()
				{
					return (name);
				}

				inline unsigned long getHash ()
				{
					return (hash);
				}

				inline void setDescription (std::string desc)
				{
					description = desc;
				}

				inline std::string getDescription ()
				{
					return (description);
				}

				inline void setVersion (double newVersion)
				{
					version = newVersion;
				}

				inline double getVersion ()
				{
					return (version);
				}

				inline void setPluginType (int newType)
				{
					type = newType;
				}

				inline int getPluginType ()
				{
					return (type);
				}

				inline void setFilePath (std::string filePath)
				{
					path = filePath;
				}

				inline std::string getFilePath ()
				{
					return (path);
				}

				inline void setCopyright (std::string copy)
				{
					copyright = copy;
				}

				inline std::string getCopyright ()
				{
					return (copyright);
				}

				inline void setDeveloper (std::string dev)
				{
					developer = dev;
				}

				inline std::string getDeveloper ()
				{
					return (developer);
				}

			protected:
				std::string name;
				unsigned long hash;
				std::string description;
				std::string copyright;
				std::string developer;
				double version;
				int type;
				std::string path;
		};

		class defRADXML_EXPORT PluginManager: public Singleton<PluginManager>
		{
			public:
				static void initialise ();
				static void destroy ();

				void addPlugin (Plugin *newPlugin);
				Plugin *getPlugin (std::string findName);
				Plugin *getPlugin (unsigned int index);
				void removePlugin (std::string name);
				inline unsigned int getNumPlugins ()
				{
					return (hshPlugins.size ());
				}

			protected:
				PluginManager ();
				~PluginManager ();

				HASHMAP<unsigned long, Plugin *> hshPlugins;
		};
	}
#endif

