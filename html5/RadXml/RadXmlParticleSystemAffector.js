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

var ParticleSystemAffectorForceApplicationType = 
{
	PSAFAT_AVERAGE		: 1, 
	PSAFAT_ADD			: 2
};

function ParticleSystemAffector (name)
{
	this._name = name;
	this._type = "";

	this._forceVector = new Vector3 (0, -100, 0);
	this._forceApplication = ParticleSystemAffectorForceApplicationType.PSAFAT_ADD;

	this._colourFader = ColourTypes.Black;

	this._colourFader1 = ColourTypes.Black;
	this._colourFader2 = ColourTypes.Black;
	this._stateChange = 1;

	this._rate = 1;

	this._rotationSpeedRangeStart = 0;
	this._rotationSpeedRangeEnd = 0;
	this._rotationRangeStart = 0;
	this._rotationRangeEnd = 0;

	this._times = new Array ();
	this._colours = new Array ();

	this._image = "";

	this._planePoint = new Vector3 (0, 0, 0);
	this._planeNormal = new Vector3 (0, 1, 0);
	this._bounce = 1;

	this._randomness = 1;
	this._scope = 1;
	this._keepVelocity = false;

	this._parent = null;

	this._parseXML = function (level, element)
	{
		var subName = RadXML._getAttribute (element, "name").value;

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

		this._type = RadXML._getAttribute (element, "type").value;
		this._forceVector = parseVector3 (RadXML._getAttribute (element, "forcevector").value);
		var strForceApplication = RadXML._getAttribute (element, "forceapplication").value;

		if (strForceApplication != "")
		{
			strForceApplication = strForceApplication.toLowerCase ();

			if (strForceApplication == "average")
				this._forceApplication = ParticleSystemAffectorForceApplicationType.PSAFAT_AVERAGE;

			if (strForceApplication == "add")
				this._forceApplication = ParticleSystemAffectorForceApplicationType.PSAFAT_ADD;
		}

		this._colourFader = RadXML._parseColourValue (RadXML._getAttribute (element, "colourfader").value);

		this._colourFader1 = RadXML._parseColourValue (RadXML._getAttribute (element, "colourfader1").value);
		this._colourFader2 = RadXML._parseColourValue (RadXML._getAttribute (element, "colourfader2").value);
		this._stateChange = RadXML._parseFloat (RadXML._getAttribute (element, "statechange").value);

		this._rate = RadXML._parseFloat (RadXML._getAttribute (element, "rate").value);

		this._rotationSpeedRangeStart = RadXML._parseFloat (
			RadXML._getAttribute (element, "rotationspeedrangestart").value);
		this._rotationSpeedRangeEnd = RadXML._parseFloat (
			RadXML._getAttribute (element, "rotationspeedrangeend").value);
		this._rotationRangeStart = RadXML._parseFloat (
			RadXML._getAttribute (element, "rotationrangestart").value);
		this._rotationRangeEnd = RadXML._parseFloat (
			RadXML._getAttribute (element, "rotationrangeend").value);

		for (var iIdx = 1; iIdx < 7; iIdx++)
		{
			var strTime = RadXML._getAttribute (element, "time" + iIdx).value;
			var strColour = RadXML._getAttribute (element, "colour" + iIdx).value;

			if ((strTime != "") && (strColour != ""))
			{
				var dTime = RadXML._parseFloat (strTime);
				var cColour = RadXML._parseColourValue (strColour);

				this._times.push (dTime);
				this._colours.push (cColour);
			}
		}

		this._image = RadXML._getAttribute (element, "rotationrangeend").value;

		this._planePoint = parseVector3 (RadXML._getAttribute (element, "planepoint").value);
		this._planeNormal = parseVector3 (RadXML._getAttribute (element, "planenormal").value);
		this._bounce = RadXML._parseFloat (RadXML._getAttribute (element, "bounce").value);

		this._randomness = RadXML._parseFloat (RadXML._getAttribute (element, "randomness").value);
		this._scope = RadXML._parseFloat (RadXML._getAttribute (element, "scope").value);
		this._keepVelocity = parseBoolean (RadXML._getAttribute (element, "keepvelocity").value);
	}

	this._create = function (parent)
	{
		parent._addAffector (this);
	}

	this._setParent = function (parent)
	{
		this._parent = parent;
	}

	this.getParent = function ()
	{
		return (this._parent);
	}

	this.getName = function ()
	{
		return (this._name);
	}
}

