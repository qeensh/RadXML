#include "RadXmlVector4.h"
#include "RadXmlGlobal.h"
#include "RadXml.h"

#include "RadXmlDatatypes.h"

namespace RadXml
{
	// Vector4Class
	void Vector4Class::setupVector4Class ()
	{
		Class *cClass = new Class ("Vector4", "Vector4");
		cClass->set ("x", new Number ("x", 0.0));
		cClass->set ("y", new Number ("y", 0.0));
		cClass->set ("z", new Number ("z", 0.0));
		cClass->set ("w", new Number ("w", 0.0));
		cClass->set ("add", new Function ("add", 0, Vector4Class::add));
		cClass->set ("subtract", new Function ("subtract", 0, Vector4Class::subtract));
		cClass->set ("multiply", new Function ("multiply", 0, Vector4Class::multiply));
		cClass->set ("divide", new Function ("divide", 0, Vector4Class::divide));
		cClass->set ("dot", new Function ("dot", 0, Vector4Class::dot));

		JavascriptManager::getSingletonPtr ()->set ("Vector4", cClass);
	}

	Variable *Vector4Class::add (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RXVECTOR4 v3dLeft = RXVECTOR4::ZERO;
		RXVECTOR4 v3dResult = RXVECTOR4::ZERO;

		v3dLeft.x = objThis->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = objThis->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = objThis->get ("z")->toNumber ()->getValue ();
		v3dLeft.w = objThis->get ("w")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR4 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector4FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = (v3dLeft + v3dRight);
		}
		else
		{
			RXVECTOR4 v3dRight = RXVECTOR4::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();
			v3dRight.w = aryArguments->getElement (3)->toNumber ()->getValue ();

			v3dResult = (v3dLeft + v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
			getVector4JSClass (v3dResult.x, v3dResult.y, v3dResult.z, v3dResult.w);

		return (objReturn);
	}

	Variable *Vector4Class::subtract (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RXVECTOR4 v3dLeft = RXVECTOR4::ZERO;
		RXVECTOR4 v3dResult = RXVECTOR4::ZERO;

		v3dLeft.x = objThis->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = objThis->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = objThis->get ("z")->toNumber ()->getValue ();
		v3dLeft.w = objThis->get ("w")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR4 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector4FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = (v3dLeft - v3dRight);
		}
		else
		{
			RXVECTOR4 v3dRight = RXVECTOR4::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();
			v3dRight.w = aryArguments->getElement (3)->toNumber ()->getValue ();

			v3dResult = (v3dLeft - v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
			getVector4JSClass (v3dResult.x, v3dResult.y, v3dResult.z, v3dResult.w);

		return (objReturn);
	}

	Variable *Vector4Class::multiply (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RXVECTOR4 v3dLeft = RXVECTOR4::ZERO;
		RXVECTOR4 v3dResult = RXVECTOR4::ZERO;

		v3dLeft.x = objThis->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = objThis->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = objThis->get ("z")->toNumber ()->getValue ();
		v3dLeft.w = objThis->get ("w")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR4 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector4FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = (v3dLeft * v3dRight);
		}
		else
		{
			RXVECTOR4 v3dRight = RXVECTOR4::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();
			v3dRight.w = aryArguments->getElement (3)->toNumber ()->getValue ();

			v3dResult = (v3dLeft * v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
			getVector4JSClass (v3dResult.x, v3dResult.y, v3dResult.z, v3dResult.w);

		return (objReturn);
	}

	Variable *Vector4Class::divide (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RXVECTOR4 v3dLeft = RXVECTOR4::ZERO;
		RXVECTOR4 v3dResult = RXVECTOR4::ZERO;

		v3dLeft.x = objThis->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = objThis->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = objThis->get ("z")->toNumber ()->getValue ();
		v3dLeft.w = objThis->get ("w")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR4 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector4FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = (v3dLeft / v3dRight);
		}
		else
		{
			RXVECTOR4 v3dRight = RXVECTOR4::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();
			v3dRight.w = aryArguments->getElement (3)->toNumber ()->getValue ();

			v3dResult = (v3dLeft / v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
			getVector4JSClass (v3dResult.x, v3dResult.y, v3dResult.z, v3dResult.w);

		return (objReturn);
	}

	Variable *Vector4Class::dot (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *objThis = thisObject->toClass ();
		RXVECTOR4 v3dLeft = RXVECTOR4::ZERO;
		double dResult = 0;

		v3dLeft.x = objThis->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = objThis->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = objThis->get ("z")->toNumber ()->getValue ();
		v3dLeft.w = objThis->get ("w")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR4 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector4FromJSObject (aryArguments->getElement (0)->toClass ());

			dResult = v3dLeft.dotProduct (v3dRight);
		}
		else
		{
			RXVECTOR4 v3dRight = RXVECTOR4::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();
			v3dRight.w = aryArguments->getElement (3)->toNumber ()->getValue ();

			dResult = v3dLeft.dotProduct (v3dRight);
		}

		return (Number::New (dResult));
	}
}

