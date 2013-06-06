#include "RadXmlVector2.h"
#include "RadXmlGlobal.h"
#include "RadXml.h"

#include "RadXmlDatatypes.h"

namespace RadXml
{
	// Vector2Class
	void Vector2Class::setupVector2Class ()
	{
		Class *cClass = new Class ("Vector2", "Vector2");
		cClass->set ("x", new Number ("x", 0.0));
		cClass->set ("y", new Number ("y", 0.0));
		cClass->set ("add", new Function ("add", 0, Vector2Class::add));
		cClass->set ("subtract", new Function ("subtract", 0, Vector2Class::subtract));
		cClass->set ("multiply", new Function ("multiply", 0, Vector2Class::multiply));
		cClass->set ("divide", new Function ("divide", 0, Vector2Class::divide));
		cClass->set ("dot", new Function ("dot", 0, Vector2Class::dot));

		JavascriptManager::getSingletonPtr ()->set ("Vector2", cClass);
	}

	Variable *Vector2Class::add (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR2 v3dLeft = RXVECTOR2::ZERO;
		RXVECTOR2 v3dResult = RXVECTOR2::ZERO;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR2 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector2FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = (v3dLeft + v3dRight);
		}
		else
		{
			RXVECTOR2 v3dRight = RXVECTOR2::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();

			v3dResult = (v3dLeft + v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->getVector2JSClass (v3dResult.x, v3dResult.y);

		return (objReturn);
	}

	Variable *Vector2Class::subtract (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR2 v3dLeft = RXVECTOR2::ZERO;
		RXVECTOR2 v3dResult = RXVECTOR2::ZERO;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR2 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector2FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = (v3dLeft - v3dRight);
		}
		else
		{
			RXVECTOR2 v3dRight = RXVECTOR2::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();

			v3dResult = (v3dLeft - v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->getVector2JSClass (v3dResult.x, v3dResult.y);

		return (objReturn);
	}

	Variable *Vector2Class::multiply (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR2 v3dLeft = RXVECTOR2::ZERO;
		RXVECTOR2 v3dResult = RXVECTOR2::ZERO;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR2 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector2FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = (v3dLeft * v3dRight);
		}
		else
		{
			RXVECTOR2 v3dRight = RXVECTOR2::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();

			v3dResult = (v3dLeft * v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->getVector2JSClass (v3dResult.x, v3dResult.y);

		return (objReturn);
	}

	Variable *Vector2Class::divide (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR2 v3dLeft = RXVECTOR2::ZERO;
		RXVECTOR2 v3dResult = RXVECTOR2::ZERO;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR2 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector2FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = (v3dLeft / v3dRight);
		}
		else
		{
			RXVECTOR2 v3dRight = RXVECTOR2::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();

			v3dResult = (v3dLeft / v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->getVector2JSClass (v3dResult.x, v3dResult.y);

		return (objReturn);
	}

	Variable *Vector2Class::dot (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR2 v3dLeft = RXVECTOR2::ZERO;
		double dResult = 0;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR2 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector2FromJSObject (aryArguments->getElement (0)->toClass ());

			dResult = v3dLeft.dotProduct (v3dRight);
		}
		else
		{
			RXVECTOR2 v3dRight = RXVECTOR2::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();

			dResult = v3dLeft.dotProduct (v3dRight);
		}

		return (new Number ("", dResult));
	}
}

