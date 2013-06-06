/*
	MIT-LICENSE
	Copyright (c) 2013 Higher Edge Software, LLC

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
	and associated documentation files (the "Software"), to deal in the Software without restriction, 
	including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
	and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
	subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial 
	portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
	LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

function Movable (name)
{
	this._name = name;
	this._type = 0;

	this._events = new Events ();
	this._events._addEvent (new Event ("onclick", ""));
	this._events._addEvent (new Event ("onmousedown", ""));
	this._events._addEvent (new Event ("onmouseup", ""));

	this._aryChildren = new Array ();

	this._position = new Vector3 ();
	this._scale = newVector3 (1, 1, 1);
	this._rotation = new Vector3 ();

	this._bVisible = true;
	this._bImmovable = false;
	this._collides = true;

	this._collidedWithObject = null;

	this._snNode = null;
	this._mParent = null;
	this._rAngle = 0;
	this._v3dMaxVel = newVector3 (150, 150, 150);
	this._v3dSpeed = new Vector3 ();
	this._v3dAccel = new Vector3 ();
	this._v3dMoveScale = new Vector3 ();
	this._v3dVelocity = new Vector3 ();
	this._v3dFriction = newVector3 (15, 15, 15);

	this._v3dOriginalPos = new Vector3 ();
	this._bSetOriginalPos = true;
	this._v3dOriginalScale = new Vector3 ();
	this._bSetOriginalScale = true;
	this._v3dOriginalRotation = new Vector3 ();
	this._bSetOriginalRotation = true;

	this.onCollision = null;

	this._destroy = function ()
	{
	}

	this._setName = function (newName)
	{
		this._name = newName;
	}

	this.getName = function ()
	{
		return (this._name);
	}

	this._getType = function ()
	{
		return (this._type);
	}

	this._createSceneNode = function (object, parent)
	{
		var geEngine = RadXML._gameEngine;

		geEngine.createSceneNode (this, object, parent);

		this.setPosition (this._position);
		this.setScale (this._scale);
		this.setRotation (this._rotation);
		this.setVisible (this._bVisible);
	}

	this._destroySceneNode = function ()
	{
		RadXML._gameEngine.destroySceneNode (this._snNode);
	}

	this.clone = function (newName, parent)
	{
		var objClone = RadXML._clone (this);

		objClone._setName (newName);
		objClone._create (parent);
		objClone._removeAllChildren ();

		for (var iIdx = 0; iIdx < this._getNumChildren (); iIdx++)
		{
			var mChild = this._getChild (iIdx);
			var mSubClone = mChild.clone (objClone.getName () + "_sub", objClone);
		}

		RadXML._addMovableJSObject (objClone);

		return (objClone);
	}

	this.setPosition = function (x, y, z)
	{
		if (x instanceof Vector3)
			this._position = x;
		else
		{
			this._position.x = x;
			this._position.y = y;
			this._position.z = z;
		}

		if (this._snNode != null)
			RadXML._gameEngine.setPosition (this, this._position);

		if (this._bSetOriginalPos == true)
		{
			this._v3dOriginalPos = this._position;
			this._bSetOriginalPos = false;
		}
	}

	this.getPosition = function ()
	{
		var v3dReturn = this._position;

		if (this._snNode != null)
			v3dReturn = RadXML._gameEngine.getPosition (this);

		return (v3dReturn);
	}

	this.getWorldPosition = function ()
	{
		var v3dReturn = this._position;

		if (this._snNode != null)
		{
		}

		return (v3dReturn);
	}

	this.translate = function (x, y, z)
	{
		if (x instanceof Vector3)
		{
			this._position.x += x.x;
			this._position.y += x.y;
			this._position.z += x.z;
		}
		else
		{
			this._position.x += x;
			this._position.y += y;
			this._position.z += z;
		}

		this.setPosition (this._position);
	}

	this.setScale = function (x, y, z)
	{
		if (x instanceof Vector3)
			this._scale = x;
		else
			this._scale = newVector3 (x, y, z);

		if (this._snNode != null)
			RadXML._gameEngine.setScale (this, this._scale);

		if (this._bSetOriginalScale == true)
		{
			this._v3dOriginalScale = this._scale;
			this._bSetOriginalScale = false;
		}
	}

	this.getScale = function ()
	{
		var v3dReturn = this._scale;

		if (this._snNode != null)
			v3dReturn = RadXML._gameEngine.getScale (this);

		return (v3dReturn);
	}

	this.setRotation = function (yaw, pitch, roll)
	{
		if (yaw instanceof Vector3)
			this._rotation = newVector3 (yaw.x, yaw.y, yaw.z);
		else
			this._rotation = newVector3 (yaw, pitch, roll);

		if (this._snNode != null)
		{
			/// @todo Yaw, pitch, and roll the scene node
		}

		if (this._bSetOriginalRotation == true)
		{
			this._v3dOriginalRotation = this._rotation;
			this._bSetOriginalRotation = false;
		}
	}

	this.getRotation = function ()
	{
		var v3dRotation = new Vector3 ();

		/// @todo Get the scene nodes rotation

		return (v3dRotation);
	}

	this.setYaw = function (yaw)
	{
		if (this._snNode != null)
		{
			/// @todo set the yaw
		}

		this._rotation.x = yaw;
	}

	this.setPitch = function (pitch)
	{
		if (this._snNode != null)
		{
			/// @todo set the pitch
		}

		this._rotation.y = pitch;
	}

	this.setRoll = function (roll)
	{
		if (this._snNode != null)
		{
			/// @todo set the roll
		}

		this._rotation.z = roll;
	}

	this.setVelocity = function (x, y, z)
	{
		this._v3dVelocity.x = x;
		this._v3dVelocity.y = y;
		this._v3dVelocity.z = z;
	}

	this.setAcceleration = function (x, y, z)
	{
		this._v3dAccel.x = x;
		this._v3dAccel.y = y;
		this._v3dAccel.z = z;
	}

	this.getAcceleration = function ()
	{
		return (this._v3dAccel);
	}

	this.setMaxVelocity = function (x, y, z)
	{
		this._v3dMaxVel.x = x;
		this._v3dMaxVel.y = y;
		this._v3dMaxVel.z = z;
	}

	this.setFriction = function (x, y, z)
	{
		this._v3dFriction.x = x;
		this._v3dFriction.y = y;
		this._v3dFriction.z = z;
	}

	this.getOrientation = function ()
	{
		var qRot = new Quaternion;

		if (this._snNode != null)
		{
			/// @todo Set the quaternion
		}

		return (qRot);
	}

	this.setVisible = function (isVisible)
	{
		this._bVisible = isVisible;

		if (this._snNode != null)
			RadXML._gameEngine.setVisible (this, isVisible);
	}

	this.getVisible = function ()
	{
		return (this._bVisible);
	}

	this.setImmovable = function (isImmovable)
	{
		this._bImmovable = isImmovable;
	}

	this.getImmovable = function ()
	{
		return (this._bImmovable);
	}

	this.setCollides = function (value)
	{
		this._collides = value;
	}

	this.getCollides = function ()
	{
		return (this._collides);
	}

	this._moveEntity = function (timeSinceLastFrame)
	{
		if (this._snNode == null)
			return;

		if (this._bImmovable == true)
			return;

		this._v3dSpeed.x += this._v3dAccel.x;
		this._v3dSpeed.y += this._v3dAccel.y;
		this._v3dSpeed.z += this._v3dAccel.z;

		if (this._v3dSpeed.x <= -this._v3dMaxVel.x)
			this._v3dSpeed.x = -this._v3dMaxVel.x;

		if (this._v3dSpeed.x >= this._v3dMaxVel.x)
			this._v3dSpeed.x = this._v3dMaxVel.x;

		if (this._v3dSpeed.y <= -this._v3dMaxVel.y)
			this._v3dSpeed.y = -this._v3dMaxVel.y;

		if (this._v3dSpeed.y >= this._v3dMaxVel.y)
			this._v3dSpeed.y = this._v3dMaxVel.y;

		if (this._v3dSpeed.z <= -this._v3dMaxVel.z)
			this._v3dSpeed.z = -this._v3dMaxVel.z;

		if (this._v3dSpeed.z >= this._v3dMaxVel.z)
			this._v3dSpeed.z = this._v3dMaxVel.z;

		this._v3dVelocity.x = this._v3dSpeed.x;
		this._v3dVelocity.y = this._v3dSpeed.y;
		this._v3dVelocity.z = this._v3dSpeed.z;

		if (this._v3dSpeed.x > 0)
			this._v3dSpeed.x -= this._v3dFriction.x;

		if (this._v3dSpeed.x < 0)
			this._v3dSpeed.x += this._v3dFriction.x;

		if (this._v3dSpeed.y > 0)
			this._v3dSpeed.y -= this._v3dFriction.y;

		if (this._v3dSpeed.y < 0)
			this._v3dSpeed.y += this._v3dFriction.y;

		if (this._v3dSpeed.z > 0)
			this._v3dSpeed.z -= this._v3dFriction.z;

		if (this._v3dSpeed.z < 0)
			this._v3dSpeed.z += this._v3dFriction.z;

		var v3dPos = this.getPosition ();
		v3dPos.x += (this._v3dVelocity.x * timeSinceLastFrame);
		v3dPos.y += (this._v3dVelocity.y * timeSinceLastFrame);
		v3dPos.z += (this._v3dVelocity.z * timeSinceLastFrame);

		this.setPosition (v3dPos.x, v3dPos.y, v3dPos.z);
	}

	this.setBoundingBoxVisible = function (value)
	{
		if (this._snNode == null)
		{
			/// @todo Show the bounding box
		}
	}

	this.getBoundingBoxVisible = function ()
	{
		if (this._snNode == null)
		{
			/// @todo Get whether or not the bounding box is visible
		}

		return (false);
	}

	this.setLookAt = function (target, transformSpace, localDirVector)
	{
		if (this._snNode == null)
		{
			/// @todo Look at a target
		}
	}

	this._parseMovableXML = function (level, element, parent)
	{
		var subName = RadXML._getAttribute (element, "name").value;
		var strPosition = RadXML._getAttribute (element, "position").value;
		var strScale = RadXML._getAttribute (element, "scale").value;
		var strRotation = RadXML._getAttribute (element, "rotation").value;
		var x = RadXML._getAttribute (element, "x").value;
		var y = RadXML._getAttribute (element, "y").value;
		var z = RadXML._getAttribute (element, "z").value;
		var sx = RadXML._getAttribute (element, "scalex").value;
		var sy = RadXML._getAttribute (element, "scaley").value;
		var sz = RadXML._getAttribute (element, "scalez").value;
		var yaw = RadXML._getAttribute (element, "yaw").value;
		var pitch = RadXML._getAttribute (element, "pitch").value;
		var roll = RadXML._getAttribute (element, "roll").value;
		var visible = RadXML._getAttribute (element, "visible").value;
		var immovable = RadXML._getAttribute (element, "immovable").value;
		var strCollides = RadXML._getAttribute (element, "collides").value;
		var onclick = RadXML._getAttribute (element, "onclick").value;
		var onmousedown = RadXML._getAttribute (element, "onmousedown").value;
		var onmouseup = RadXML._getAttribute (element, "onmouseup").value;
		var dX = RadXML._parseFloat (x);
		var dY = RadXML._parseFloat (y);
		var dZ = RadXML._parseFloat (z);
		var dSX = 1;
		var dSY = 1;
		var dSZ = 1;
		var dYaw = RadXML._parseFloat (yaw);
		var dPitch = RadXML._parseFloat (pitch);
		var dRoll = RadXML._parseFloat (roll);
		var bImmovable = false;
		var bVisible = true;
		var bCollides = true;
		var objObject = null;

		if (strCollides != "")
			bCollides = parseBoolean (strCollides);

		if (immovable != "")
			bImmovable = parseBoolean (immovable);

		if (visible != "")
			bVisible = parseBoolean (visible);

		if (strPosition != "")
		{
			strPosition = strPosition.removeWhitespaces ();
			var aryString = strPosition.split (",");
			dX = RadXML._parseFloat (aryString[0]);
			dY = RadXML._parseFloat (aryString[1]);
			dZ = RadXML._parseFloat (aryString[2]);
		}

		if (strScale != "")
		{
			strScale = strScale.removeWhitespaces ();
			var aryString = strScale.split (",");
			dSX = RadXML._parseFloat (aryString[0]);
			dSY = RadXML._parseFloat (aryString[1]);
			dSZ = RadXML._parseFloat (aryString[2]);
		}

		if (strRotation != "")
		{
			strRotation = strRotation.removeWhitespaces ();
			var aryString = strRotation.split (",");
			dYaw = RadXML._parseFloat (aryString[0]);
			dPitch = RadXML._parseFloat (aryString[1]);
			dRoll = RadXML._parseFloat (aryString[2]);
		}

		if (sx != "")
			dSX = RadXML._parseFloat (sx);

		if (sy != "")
			dSY = RadXML._parseFloat (sy);

		if (sz != "")
			dSZ = RadXML._parseFloat (sz);

		if (subName == "")
		{
			var iRandName = RadXML._parseInt (Math.random () * 1000000000);
			var objFound = level.getMovable (iRandName, true);

			while (objFound != null)
			{
				iRandName = RadXML._parseInt (Math.random () * 1000000000);
				objFound = level.getMovable (iRandName, true);
			}

			subName = "obj" + iRandName;
		}

		this._scale.x = dSX;
		this._scale.y = dSY;
		this._scale.z = dSZ;
		this._rotation.x = dYaw;
		this._rotation.y = dPitch;
		this._rotation.z = dRoll;

		this._events._setJavascriptToEvent ("onclick", onclick);
		this._events._setJavascriptToEvent ("onmousedown", onmousedown);
		this._events._setJavascriptToEvent ("onmouseup", onmouseup);
		this.setPosition (dX, dY, dZ);
		this.setImmovable (bImmovable);
		this.setVisible (bVisible);
		this.setCollides (bCollides);
		this._setName (subName);
	}

	this._getSceneNode = function ()
	{
		return (this._snNode);
	}

	this._setParent = function (parent)
	{
		this._mParent = parent;

		if (this._mParent != null)
			this._mParent._addChild (this);
	}

	this._getParent = function ()
	{
		return (this._mParent);
	}

	this._addChild = function (child)
	{
		this._aryChildren.push (child);
	}

	this._removeChild = function (child)
	{
		for (var iIdx = 0; iIdx < this._aryChildren.length; iIdx++)
		{
			if (this._aryChildren[iIdx]._name == child._name)
			{
				this._aryChildren.splice (iIdx, 1);

				break;
			}
		}
	}

	this._removeAllChildren = function ()
	{
		this._aryChildren = new Array ();
	}

	this._getChild = function (index)
	{
		return (this._aryChildren[index]);
	}

	this._getNumChildren = function ()
	{
		return (this._aryChildren.length);
	}
}

