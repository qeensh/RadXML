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

var ParticleSystemRenderType = 
{
	PSRT_BILLBOARD		: 1
};

var ParticleSystemBillboardType = 
{
	PSBT_POINT					: 1, 
	PSBT_ORIENTED_COMMON		: 2, 
	PSBT_ORIENTED_SELF			: 3, 
	PSBT_PERPENDICULAR_COMMON	: 4, 
	PSBT_PERPENDICULAR_SELF		: 5
};

var ParticleSystemBillboardOriginType = 
{
	PSBOT_TOP_LEFT			: 1, 
	PSBOT_TOP_CENTER		: 2, 
	PSBOT_TOP_RIGHT			: 3, 
	PSBOT_CENTER_LEFT		: 4, 
	PSBOT_CENTER			: 5, 
	PSBOT_CENTER_RIGHT		: 6, 
	PSBOT_BOTTOM_LEFT		: 7, 
	PSBOT_BOTTOM_CENTER		: 8, 
	PSBOT_BOTTOM_RIGHT		: 9
};

var ParticleSystemBillboardRotationType = 
{
	PSBRT_VERTEX		: 1, 
	PSBRT_TEX_COORD		: 2
};

function ParticleSystemTemplate (name)
{
	this._name = name;
	this._loadNow = true;

	this._quota = 10;
	this._material = "";
	this._particleWidth = 100;
	this._particleHeight = 100;
	this._cullEach = false;
	this._renderType = ParticleSystemRenderType.PSRT_BILLBOARD;
	this._sorted = false;
	this._localSpace = false;
	this._billboardType = ParticleSystemBillboardType.PSBT_POINT;
	this._billboardOrigin = ParticleSystemBillboardOriginType.PSBOT_CENTER;
	this._billboardRotationType = ParticleSystemBillboardRotationType.PSBRT_TEX_COORD;
	this._commonDirection = new Vector3 (0, 0, 1);
	this._commonUpVector = new Vector3 (0, 1, 0);
	this._pointRendering = false;
	this._accurateFacing = false;
	this._iterationInterval = 0;
	this._nonvisibleUpdateTimeout = 0;

	this._emitters = new Array ();
	this._affectors = new Array ();

	this._parseXML = function (level, element, parent)
	{
		var subName = RadXML._getAttribute (element, "name").value;
		var strLoad = RadXML._getAttribute (element, "load").value;
		var strQuota = RadXML._getAttribute (element, "quota").value;
		this._material = RadXML._getAttribute (element, "material").value;
		this._particleWidth = RadXML._parseFloat (RadXML._getAttribute (element, "width").value);
		this._particleHeight = RadXML._parseFloat (RadXML._getAttribute (element, "height").value);
		var strCullEach = RadXML._getAttribute (element, "culleach").value;
		var strRenderType = RadXML._getAttribute (element, "rendertype").value;
		var strSorted = RadXML._getAttribute (element, "sorted").value;
		var strLocalSpace = RadXML._getAttribute (element, "localspace").value;
		var strBillboardType = RadXML._getAttribute (element, "billboardtype").value;
		var strBillboardOrigin = RadXML._getAttribute (element, "billboardorigin").value;
		var strBillboardRotationType = RadXML._getAttribute (element, "billboardrotationtype").value;
		this._commonDirection = parseVector3 (RadXML._getAttribute (element, "commondirection").value);
		this._commonUpVector = parseVector3 (RadXML._getAttribute (element, "commonupvector").value);
		var strPointRendering = RadXML._getAttribute (element, "pointrendering").value;
		var strAccurateFacing = RadXML._getAttribute (element, "accuratefacing").value;
		this._iterationInterval = RadXML._parseFloat (
			RadXML._getAttribute (element, "iterationInterval").value);
		this._nonvisibleUpdateTimeout = RadXML._parseFloat (
			RadXML._getAttribute (element, "nonvisibleupdatetimeout").value);

		if (subName == "")
		{
			var iRandName = RadXML._parseInt (Math.random () * 1000000000);
			var objFound = level.getMovable (iRandName, true);

			while (objFound != 0)
			{
				iRandName = RadXML._parseInt (Math.random () * 1000000000);
				objFound = level.getMovable (iRandName, true);
			}

			subName = "obj" + iRandName;
		}

		if (strLoad != "")
			this._loadNow = parseBoolean (strLoad);

		if (strCullEach != "")
			this._cullEach = parseBoolean (strCullEach);

		if (strSorted != "")
			this._sorted = parseBoolean (strSorted);

		if (strLocalSpace != "")
			this._localSpace = parseBoolean (strLocalSpace);

		if (strRenderType != "")
		{
			strRenderType = strRenderType.toLowerCase ();

			if (strRenderType == "billboard")
				this._renderType = ParticleSystemRenderType.PSRT_BILLBOARD;
		}

		if (strBillboardType != "")
		{
			strBillboardType = strBillboardType.toLowerCase ();

			if (strBillboardType == "point")
				this._billboardType = ParticleSystemBillboardType.PSBT_POINT;

			if (strBillboardType == "oriented_common")
				this._billboardType = ParticleSystemBillboardType.PSBT_ORIENTED_COMMON;

			if (strBillboardType == "oriented_self")
				this._billboardType = ParticleSystemBillboardType.PSBT_ORIENTED_SELF;

			if (strBillboardType == "perpendicular_common")
				this._billboardType = ParticleSystemBillboardType.PSBT_PERPENDICULAR_COMMON;

			if (strBillboardType == "perpendicular_self")
				this._billboardType = ParticleSystemBillboardType.PSBT_PERPENDICULAR_SELF;
		}

		if (strBillboardOrigin != "")
		{
			strBillboardOrigin = strBillboardOrigin.toLowerCase ();

			if (strBillboardOrigin == "top_left")
				this._billboardOrigin = ParticleSystemBillboardType.PSBOT_TOP_LEFT;

			if (strBillboardOrigin == "top_center")
				this._billboardOrigin = ParticleSystemBillboardType.PSBOT_TOP_CENTER;

			if (strBillboardOrigin == "top_right")
				this._billboardOrigin = ParticleSystemBillboardType.PSBOT_TOP_RIGHT;

			if (strBillboardOrigin == "center_left")
				this._billboardOrigin = ParticleSystemBillboardType.PSBOT_CENTER_LEFT;

			if (strBillboardOrigin == "center")
				this._billboardOrigin = ParticleSystemBillboardType.PSBOT_CENTER;

			if (strBillboardOrigin == "center_right")
				this._billboardOrigin = ParticleSystemBillboardType.PSBOT_CENTER_RIGHT;

			if (strBillboardOrigin == "bottom_left")
				this._billboardOrigin = ParticleSystemBillboardType.PSBOT_BOTTOM_LEFT;

			if (strBillboardOrigin == "bottom_center")
				this._billboardOrigin = ParticleSystemBillboardType.PSBOT_BOTTOM_CENTER;

			if (strBillboardOrigin == "bottom_right")
				this._billboardOrigin = ParticleSystemBillboardType.PSBOT_BOTTOM_RIGHT;
		}

		if (strBillboardRotationType != "")
		{
			strBillboardRotationType = strBillboardRotationType.toLowerCase ();

			if (strBillboardRotationType == "vertex")
				this._billboardRotationType = ParticleSystemBillboardType.PSBRT_VERTEX;

			if (strBillboardRotationType == "texcoord")
				this._billboardRotationType = ParticleSystemBillboardType.PSBRT_TEX_COORD;
		}

		if (strPointRendering != "")
			this._pointRendering = parseBoolean (strPointRendering);

		if (strAccurateFacing != "")
			this._accurateFacing = parseBoolean (strAccurateFacing);
	}

	this._create = function (parent)
	{
	}

	this._addEmitter = function (emitter)
	{
	}

	this._addAffector = function (affector)
	{
	}
}

