#include "RadXmlQuaternion.h"
#include "RadXmlGlobal.h"
#include "RadXml.h"

#include "RadXmlDatatypes.h"

namespace RadXml
{
	// QuaternionClass
	void QuaternionClass::setupQuaternionClass ()
	{
		Class *cClass = new Class ("Quaternion", "Quaternion");
		cClass->set ("w", new Number ("w", 1.0));
		cClass->set ("x", new Number ("x", 0.0));
		cClass->set ("y", new Number ("y", 0.0));
		cClass->set ("z", new Number ("z", 0.0));

		JavascriptManager::getSingletonPtr ()->set ("Quaternion", cClass);
	}
}

