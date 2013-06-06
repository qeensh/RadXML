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

function Entity (name)
{
	this._name = name;
	this._type = GameObjectTypes.GOT_ENTITY;

	this._aryChildren = new Array ();

	this._position = new Vector3 ();
	this._scale = newVector3 (1, 1, 1);
	this._rotation = new Vector3 ();

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

	this._eObj = null;
	this._model = "";
	this._material = "";

	this._events._addEvent (new Event ("oncollision", ""));

	this._parseXML = function (level, element, parent)
	{
		this._parseMovableXML (level, element, parent);

		var model = RadXML._getAttribute (element, "model").value;
		var material = RadXML._getAttribute (element, "material").value;
		var oncollision = RadXML._getAttribute (element, "oncollision").value;
		var collides = RadXML._getAttribute (element, "collides").value;

		if (collides == "")
			collides = "true";

		if (model == "")
			model = "cube";

		this.setModel (model);
		this.setMaterial (material);
		this._events._setJavascriptToEvent ("oncollision", oncollision);

		this._collides = parseBoolean (collides);
	}

	this._create = function (parent)
	{
		var geEngine = RadXML._gameEngine;

		if (this._model == "cube")
			this._eObj = geEngine.createCube ("ent" + this._name, this._material);
		else if (this._model == "sphere")
			this._eObj = geEngine.createSphere ("ent" + this._name, this._material);
		else if (this._model.indexOf ("plane") > -1)
		{
			var entity = this._model;

			if (geEngine.getModel (entity, false) == null)
			{
				entity = entity.toLowerCase ();
				var iPos = entity.indexOf ("x");

				if (iPos < 0)
				{
					logMessage (RadXmlLanguage.MSG31);
					this._eObj = geEngine.createCube ("ent" + this._name);

					return;
				}

				var strWidth = entity.substr (5, (iPos - 5));
				var strHeight = entity.substr (iPos + 1);
				var dWidth = RadXML._parseInt (strWidth);
				var dHeight = RadXML._parseInt (strHeight);
				var dD = 0;
				var iSegX = 1;
				var iSegY = 1;
				var v3dNormal = newVector3 (0, 0, 1);
				var v3dUp = newVector3 (0, 1, 0);

				this._eObj = geEngine.createPlane ("ent" + this._name, this._material, 
									dWidth, dHeight, iSegX, iSegY, v3dNormal, dD, v3dUp);
			}
			else
				this._eObj = geEngine.createPlane ("ent" + this.name, entity);
		}
		else
			this._eObj = geEngine.createModel ("ent" + this.name, this._model);

		if (this._eObj == null)
			this._eObj = geEngine.createCube ("ent" + this._name);

		if (this._material != "")
			geEngine.setMaterial (this._eObj, this._material);

		this._eObj._movable = this;

		this._setParent (parent);
		this._createSceneNode (this._eObj, parent);
	}

	this.setModel = function (newModel)
	{
		this._model = newModel;

		if (this._eObj != null)
		{
			this._destroy ();
			this._create (this._mParent);
		}
	}

	this.getModel = function ()
	{
		return (this._model);
	}

	this.getEntity = function ()
	{
		return (this._eObj);
	}

	this.setMaterial = function (newMaterial)
	{
		var geEngine = RadXML._gameEngine;
		this._material = newMaterial;

		if (this._eObj != null)
			geEngine.setMaterial (this._eObj, this._material);
	}

	this.getMaterial = function ()
	{
		return (this._material);
	}

	this.getCollidingObject = function ()
	{
		return (this._collidedWithObject);
	}
}

Entity.prototype = new Movable ();

