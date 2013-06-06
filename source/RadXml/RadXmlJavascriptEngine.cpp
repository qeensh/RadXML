#include "RadXmlJavascriptEngine.h"

namespace RadXml
{
	// JavascriptEngine
	Ogre::Vector2 JavascriptEngine::getVector2FromJSObject (Class *vector)
	{
		Ogre::Vector2 v2dReturn = Ogre::Vector2::ZERO;

		v2dReturn.x = vector->get ("x")->toNumber ()->getValue ();
		v2dReturn.y = vector->get ("y")->toNumber ()->getValue ();

		return (v2dReturn);
	}

	Ogre::Vector3 JavascriptEngine::getVector3FromJSObject (Class *vector)
	{
		Ogre::Vector3 v3dReturn = Ogre::Vector3::ZERO;

		v3dReturn.x = vector->get ("x")->toNumber ()->getValue ();
		v3dReturn.y = vector->get ("y")->toNumber ()->getValue ();
		v3dReturn.z = vector->get ("z")->toNumber ()->getValue ();

		return (v3dReturn);
	}

	Ogre::Vector4 JavascriptEngine::getVector4FromJSObject (Class *vector)
	{
		Ogre::Vector4 v4dReturn = Ogre::Vector4::ZERO;

		v4dReturn.x = vector->get ("x")->toNumber ()->getValue ();
		v4dReturn.y = vector->get ("y")->toNumber ()->getValue ();
		v4dReturn.z = vector->get ("z")->toNumber ()->getValue ();
		v4dReturn.w = vector->get ("w")->toNumber ()->getValue ();

		return (v4dReturn);
	}
}

