#include "RadXmlVector3.h"
#include "RadXmlGlobal.h"
#include "RadXml.h"

#include "RadXmlDatatypes.h"

namespace RadXml
{
	// Vector3Class
	void Vector3Class::setupVector3Class ()
	{
		Class *cClass = new Class ("Vector3", "Vector3");
		cClass->set ("x", new Number ("x", 0.0));
		cClass->set ("y", new Number ("y", 0.0));
		cClass->set ("z", new Number ("z", 0.0));
		cClass->set ("add", new Function ("add", 0, Vector3Class::add));
		cClass->set ("subtract", new Function ("subtract", 0, Vector3Class::subtract));
		cClass->set ("multiply", new Function ("multiply", 0, Vector3Class::multiply));
		cClass->set ("divide", new Function ("divide", 0, Vector3Class::divide));
		cClass->set ("dot", new Function ("dot", 0, Vector3Class::dot));
		cClass->set ("cross", new Function ("cross", 0, Vector3Class::cross));
		cClass->set ("length", new Function ("length", 0, Vector3Class::length));
		cClass->set ("normalise", new Function ("normalise", 0, Vector3Class::normalise));
		cClass->set ("squaredLength", new Function ("squaredLength", 0, Vector3Class::squaredLength));
		cClass->set ("absDotProduct", new Function ("absDotProduct", 0, Vector3Class::absDotProduct));
		cClass->set ("angleBetween", new Function ("angleBetween", 0, Vector3Class::angleBetween));
		cClass->set ("distance", new Function ("distance", 0, Vector3Class::distance));
		cClass->set ("getRotationTo", new Function ("getRotationTo", 0, Vector3Class::getRotationTo));
		cClass->set ("makeCeil", new Function ("makeCeil", 0, Vector3Class::makeCeil));
		cClass->set ("makeFloor", new Function ("makeFloor", 0, Vector3Class::makeFloor));
		cClass->set ("midPoint", new Function ("midPoint", 0, Vector3Class::midPoint));
		cClass->set ("perpendicular", new Function ("perpendicular", 0, Vector3Class::perpendicular));
		cClass->set ("reflect", new Function ("reflect", 0, Vector3Class::reflect));

		JavascriptManager::getSingletonPtr ()->set ("Vector3", cClass);
	}

	Variable *Vector3Class::add (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		RXVECTOR3 v3dResult = RXVECTOR3::ZERO;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR3 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = (v3dLeft + v3dRight);
		}
		else
		{
			RXVECTOR3 v3dRight = RXVECTOR3::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();

			v3dResult = (v3dLeft + v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3JSClass (v3dResult.x, v3dResult.y, v3dResult.z);

		return (objReturn);
	}

	Variable *Vector3Class::subtract (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		RXVECTOR3 v3dResult = RXVECTOR3::ZERO;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR3 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = (v3dLeft - v3dRight);
		}
		else
		{
			RXVECTOR3 v3dRight = RXVECTOR3::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();

			v3dResult = (v3dLeft - v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3JSClass (v3dResult.x, v3dResult.y, v3dResult.z);

		return (objReturn);
	}

	Variable *Vector3Class::multiply (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		RXVECTOR3 v3dResult = RXVECTOR3::ZERO;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR3 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = (v3dLeft * v3dRight);
		}
		else
		{
			RXVECTOR3 v3dRight = RXVECTOR3::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();

			v3dResult = (v3dLeft * v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3JSClass (v3dResult.x, v3dResult.y, v3dResult.z);

		return (objReturn);
	}

	Variable *Vector3Class::divide (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		RXVECTOR3 v3dResult = RXVECTOR3::ZERO;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR3 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = (v3dLeft / v3dRight);
		}
		else
		{
			RXVECTOR3 v3dRight = RXVECTOR3::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();

			v3dResult = (v3dLeft / v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
						getVector3JSClass (v3dResult.x, v3dResult.y, v3dResult.z);

		return (objReturn);
	}

	Variable *Vector3Class::dot (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		double dResult = 0;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR3 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3FromJSObject (aryArguments->getElement (0)->toClass ());

			dResult = v3dLeft.dotProduct (v3dRight);
		}
		else
		{
			RXVECTOR3 v3dRight = RXVECTOR3::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();

			dResult = v3dLeft.dotProduct (v3dRight);
		}

		return (new Number ("", dResult));
	}

	Variable *Vector3Class::cross (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		RXVECTOR3 v3dResult = RXVECTOR3::ZERO;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR3 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = v3dLeft.crossProduct (v3dRight);
		}
		else
		{
			RXVECTOR3 v3dRight = RXVECTOR3::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();

			v3dResult = v3dLeft.crossProduct (v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3JSClass (v3dResult.x, v3dResult.y, v3dResult.z);

		return (objReturn);
	}

	Variable *Vector3Class::length (Variable *thisObject, Variable *arguments)
	{
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		double dResult = 0;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		dResult = v3dLeft.length ();

		return (new Number ("", dResult));
	}

	Variable *Vector3Class::normalise (Variable *thisObject, Variable *arguments)
	{
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		double dResult = 0;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		dResult = v3dLeft.normalise ();

		return (Number::New (dResult));
	}

	Variable *Vector3Class::squaredLength (Variable *thisObject, Variable *arguments)
	{
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		double dResult = 0;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		dResult = v3dLeft.squaredLength ();

		return (Number::New (dResult));
	}

	Variable *Vector3Class::absDotProduct (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		double dResult = 0;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR3 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3FromJSObject (aryArguments->getElement (0)->toClass ());

			dResult = v3dLeft.absDotProduct (v3dRight);
		}
		else
		{
			RXVECTOR3 v3dRight = RXVECTOR3::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();

			dResult = v3dLeft.absDotProduct (v3dRight);
		}

		return (new Number ("", dResult));
	}

	Variable *Vector3Class::angleBetween (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		Ogre::Radian dResult (0);

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR3 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3FromJSObject (aryArguments->getElement (0)->toClass ());

			dResult = v3dLeft.angleBetween (v3dRight);
		}
		else
		{
			RXVECTOR3 v3dRight = RXVECTOR3::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();

			dResult = v3dLeft.angleBetween (v3dRight);
		}

		return (new Number ("", dResult.valueDegrees ()));
	}

	Variable *Vector3Class::distance (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		double dResult = 0;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR3 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3FromJSObject (aryArguments->getElement (0)->toClass ());

			dResult = v3dLeft.distance (v3dRight);
		}
		else
		{
			RXVECTOR3 v3dRight = RXVECTOR3::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();

			dResult = v3dLeft.distance (v3dRight);
		}

		return (Number::New (dResult));
	}

	Variable *Vector3Class::getRotationTo (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		Ogre::Quaternion qResult = Ogre::Quaternion::IDENTITY;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR3 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3FromJSObject (aryArguments->getElement (0)->toClass ());

			qResult = v3dLeft.getRotationTo (v3dRight);
		}
		else
		{
			RXVECTOR3 v3dRight = RXVECTOR3::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();

			qResult = v3dLeft.getRotationTo (v3dRight);
		}

		Class *objResult = RadXML::getSingletonPtr ()->getJavascriptEngine ()->getQuaternionJSClass (qResult);

		return (objResult);
	}

	Variable *Vector3Class::makeCeil (Variable *thisObject, Variable *arguments)
	{
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		v3dLeft.makeCeil (v3dLeft);

		cClass->set ("x", Number::New (v3dLeft.x));
		cClass->set ("y", Number::New (v3dLeft.y));
		cClass->set ("z", Number::New (v3dLeft.z));

		return (0);
	}

	Variable *Vector3Class::makeFloor (Variable *thisObject, Variable *arguments)
	{
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		v3dLeft.makeFloor (v3dLeft);

		cClass->set ("x", Number::New (v3dLeft.x));
		cClass->set ("y", Number::New (v3dLeft.y));
		cClass->set ("z", Number::New (v3dLeft.z));

		return (0);
	}

	Variable *Vector3Class::midPoint (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		RXVECTOR3 v3dResult = RXVECTOR3::ZERO;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR3 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = v3dLeft.midPoint (v3dRight);
		}
		else
		{
			RXVECTOR3 v3dRight = RXVECTOR3::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();

			v3dResult = v3dLeft.midPoint (v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
						getVector3JSClass (v3dResult.x, v3dResult.y, v3dResult.z);

		return (objReturn);
	}

	Variable *Vector3Class::perpendicular (Variable *thisObject, Variable *arguments)
	{
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		RXVECTOR3 v3dResult = RXVECTOR3::ZERO;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		v3dResult = v3dLeft.perpendicular ();

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
						getVector3JSClass (v3dResult.x, v3dResult.y, v3dResult.z);

		return (objReturn);
	}

	Variable *Vector3Class::reflect (Variable *thisObject, Variable *arguments)
	{
		Array *aryArguments = static_cast <Array *> (arguments);
		Class *cClass = thisObject->toClass ();
		RXVECTOR3 v3dLeft = RXVECTOR3::ZERO;
		RXVECTOR3 v3dResult = RXVECTOR3::ZERO;

		v3dLeft.x = cClass->get ("x")->toNumber ()->getValue ();
		v3dLeft.y = cClass->get ("y")->toNumber ()->getValue ();
		v3dLeft.z = cClass->get ("z")->toNumber ()->getValue ();

		if (aryArguments->length () == 1)
		{
			RXVECTOR3 v3dRight = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3FromJSObject (aryArguments->getElement (0)->toClass ());

			v3dResult = v3dLeft.reflect (v3dRight);
		}
		else
		{
			RXVECTOR3 v3dRight = RXVECTOR3::ZERO;

			v3dRight.x = aryArguments->getElement (0)->toNumber ()->getValue ();
			v3dRight.y = aryArguments->getElement (1)->toNumber ()->getValue ();
			v3dRight.z = aryArguments->getElement (2)->toNumber ()->getValue ();

			v3dResult = v3dLeft.reflect (v3dRight);
		}

		Class *objReturn = RadXML::getSingletonPtr ()->getJavascriptEngine ()->\
					getVector3JSClass (v3dResult.x, v3dResult.y, v3dResult.z);

		return (objReturn);
	}
}

