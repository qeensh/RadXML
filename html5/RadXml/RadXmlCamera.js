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

function Camera (name)
{
	this._name = name;
	this._type = GameObjectTypes.GOT_CAMERA;

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

	this._numMipMaps = 3;
	this._focalLength = 1;
	this._nearClipDistance = 1.0;
	this._farClipDistance = 10000000000.0;
	this._enabled = true;

	this._parseXML = function (level, element, parent)
	{
		this._parseMovableXML (level, element, parent);

		var strBgColor = RadXML._getAttribute (element, "bgcolor").value;
		var strFocallength = RadXML._getAttribute (element, "focallength").value;
		var strNearClip = RadXML._getAttribute (element, "nearclip").value;
		var strFarClip = RadXML._getAttribute (element, "farclip").value;
		var srtNumMipMaps = RadXML._getAttribute (element, "nummipmaps").value;
		var strEnabled = RadXML._getAttribute (element, "enabled").value;
		var cvBgColor = RadXML._parseColourValue (strBgColor);

		if (strEnabled != "")
			this._enabled = parseBoolean (strEnabled);

		bgColor = cvBgColor;

		if (strFocallength != "")
			this._focalLength = RadXML._parseFloat (strFocallength);

		if (strNearClip != "")
			this._nearClipDistance = RadXML._parseFloat (strNearClip);

		if (strFarClip != "")
			this._farClipDistance = RadXML._parseFloat (strFarClip);

		if (srtNumMipMaps != "")
			this._numMipMaps = RadXML._parseInt (srtNumMipMaps);
	}

	this._create = function (iWindow, parent)
	{
		if (this._enabled == true)
			this._eObj = RadXML._gameEngine.setCamera (this);

		this._setParent (parent);
		this._createSceneNode (this._eObj, parent);

		/*if (parent != null)
			this._createSceneNode (this._eObj, parent);
		else
		{
			this._snNode = this._eObj;

			this.setPosition (this._position);
			this.setScale (this._scale);
			this.setRotation (this._rotation);
			this.setVisible (this._bVisible);
		}*/
	}

	this.clone = function (newName, parent)
	{
		var objClone = RadXML._clone (this);

		objClone._setName (newName);
		objClone._create (0, parent);
		objClone._removeAllChildren ();

		for (var iIdx = 0; iIdx < this._getNumChildren (); iIdx++)
		{
			var mChild = this._getChild (iIdx);
			var mSubClone = mChild.clone (objClone.getName () + "_sub", objClone);
		}

		RadXML._addMovableJSObject (objClone);

		return (objClone);
	}
}

Camera.prototype = new Movable ();

