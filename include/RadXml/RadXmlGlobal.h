#ifndef _RADXML_GLOBAL_H_
	#define _RADXML_GLOBAL_H_

	#ifdef defAPI_OGRE3D
		#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
			#include <OGRE/OgreVector2.h>
			#include <OGRE/OgreVector3.h>
			#include <OGRE/OgreVector4.h>
		#else
			#include <OgreVector2.h>
			#include <OgreVector3.h>
			#include <OgreVector4.h>
		#endif
	#endif

	#include "RadXmlPreprocessor.h"
	#include "RadXmlNative.h"

	#include <string>

	namespace RadXml
	{
		class XmlFile;
		class AppObject;
		class Movable;

		class GlobalClass
		{
			public:
				static void setupGlobalClass ();

				static Variable *alert (Variable *thisObject, Variable *arguments);
				static Variable *include (Variable *thisObject, Variable *arguments);
				static Variable *logMessage (Variable *thisObject, Variable *arguments);
				static Variable *openFileDialog (Variable *thisObject, Variable *arguments);
				static Variable *saveFileDialog (Variable *thisObject, Variable *arguments);
				static Variable *applyVideoSettings (Variable *thisObject, Variable *arguments);
				static Variable *copyTextToClipboard (Variable *thisObject, Variable *arguments);
				static Variable *getTextFromClipboard (Variable *thisObject, Variable *arguments);
				static Variable *openBrowser (Variable *thisObject, Variable *arguments);
				static Variable *mkdir (Variable *thisObject, Variable *arguments);
				static Variable *moveFile (Variable *thisObject, Variable *arguments);
				static Variable *deleteFile (Variable *thisObject, Variable *arguments);
				static Variable *deleteDir (Variable *thisObject, Variable *arguments);
				static Variable *copyFile (Variable *thisObject, Variable *arguments);
				static Variable *isFile (Variable *thisObject, Variable *arguments);
				static Variable *isDir (Variable *thisObject, Variable *arguments);
				static Variable *quit (Variable *thisObject, Variable *arguments);
				static Variable *setTimeout (Variable *thisObject, Variable *arguments);
		};
	}
#endif

