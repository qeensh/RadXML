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

function Vector3 (x, y, z)
{
	if (x == null)
		x = 0;

	if (y == null)
		y = 0;

	if (z == null)
		z = 0;

	this.x = x;
	this.y = y;
	this.z = z;

	this.toString = function ()
	{
		return (this.x + "," + this.y + "," + this.z);
	}

	this.add = function (vector3)
	{
		var v3dReturn = new Vector3 ();

		if (vector3 instanceof Vector3)
		{
			v3dReturn.x = this.x + vector3.x;
			v3dReturn.y = this.y + vector3.y;
			v3dReturn.z = this.z + vector3.z;
		}
		else
		{
			v3dReturn.x = this.x + vector3;
			v3dReturn.y = this.y + vector3;
			v3dReturn.z = this.z + vector3;
		}

		return (v3dReturn);
	}

	this.subtract = function (vector3)
	{
		var v3dReturn = new Vector3 ();

		if (vector3 instanceof Vector3)
		{
			v3dReturn.x = this.x - vector3.x;
			v3dReturn.y = this.y - vector3.y;
			v3dReturn.z = this.z - vector3.z;
		}
		else
		{
			v3dReturn.x = this.x - vector3;
			v3dReturn.y = this.y - vector3;
			v3dReturn.z = this.z - vector3;
		}

		return (v3dReturn);
	}

	this.multiply = function (vector3)
	{
		var v3dReturn = new Vector3 ();

		if (vector3 instanceof Vector3)
		{
			v3dReturn.x = this.x * vector3.x;
			v3dReturn.y = this.y * vector3.y;
			v3dReturn.z = this.z * vector3.z;
		}
		else
		{
			v3dReturn.x = this.x * vector3;
			v3dReturn.y = this.y * vector3;
			v3dReturn.z = this.z * vector3;
		}

		return (v3dReturn);
	}

	this.divide = function (vector3)
	{
		var v3dReturn = new Vector3 ();

		if (vector3 instanceof Vector3)
		{
			v3dReturn.x = this.x / vector3.x;
			v3dReturn.y = this.y / vector3.y;
			v3dReturn.z = this.z / vector3.z;
		}
		else
		{
			v3dReturn.x = this.x / vector3;
			v3dReturn.y = this.y / vector3;
			v3dReturn.z = this.z / vector3;
		}

		return (v3dReturn);
	}

	this.dot = function (vector3)
	{
		var dReturn = ((this.x * vector3.x) + (this.y * vector3.y) + (this.z * vector3.z));

		return (dReturn);
	}

	this.cross = function (vector3)
	{
		var v3dReturn = new Vector3 ();

		v3dReturn.x = (this.y * vector3.z - this.z * vector3.y);
		v3dReturn.y = (this.z * vector3.x - this.x * vector3.z);
		v3dReturn.z = (this.x * vector3.y - this.y * vector3.x);

		return (v3dReturn);
	}

	this.length = function ()
	{
		var dReturn = Math.sqrt ((this.x * this.x) + (this.y * this.y) + (this.z * this.z));

		return (dReturn);
	}

	this.normalize = function ()
	{
		var dReturn = this.divide (this.length ());

		return (dReturn);
	}

	this.squaredLength = function ()
	{
		var dReturn = ((this.x * this.x) + (this.y * this.y) + (this.z * this.z));

		return (dReturn);
	}

	this.absDotProduct = function (vector3)
	{
		var dReturn = Math.abs (this.dot (vector3));

		return (dReturn);
	}

	this.angleBetween = function (vector3)
	{
		var dTheta = this.dot (vector3) / (this.length () * vector3.length ());
		var dReturn = Math.acos (Math.clamp (dTheta, -1, 1));

		return (dReturn);
	}

	this.distance = function (vector3)
	{
		var dX = (this.x - vector3.x);
		var dY = (this.y - vector3.y);
		var dZ = (this.z - vector3.z);
		var dReturn = Math.sqrt ((dX * dX) + (dY * dY) + (dZ * dZ));

		return (dReturn);
	}

	this.getRotationTo = function (vector3)
	{
		return (null);
	}

	this.makeCeil = function ()
	{
		return (null);
	}

	this.makeFloor = function ()
	{
		return (null);
	}

	this.midPoint = function (vector3)
	{
		return (null);
	}

	this.perpendicular = function ()
	{
		return (null);
	}

	this.reflect = function ()
	{
		return (null);
	}
}

function parseVector3 (string)
{
	var v3dReturn = new Vector3 ();

	if (string == "")
		return (v3dReturn);

	string = string.toLowerCase ();
	string = string.replace (/ /g, "");
	var aryStrings = string.split (",");

	v3dReturn.x = parseFloat (aryStrings[0]);
	v3dReturn.y = parseFloat (aryStrings[1]);
	v3dReturn.z = parseFloat (aryStrings[2]);

	return (v3dReturn);
}

function newVector3 (newX, newY, newZ)
{
	var vReturn = new Vector3 ();

	vReturn.x = newX;
	vReturn.y = newY;
	vReturn.z = newZ;

	return (vReturn);
}

