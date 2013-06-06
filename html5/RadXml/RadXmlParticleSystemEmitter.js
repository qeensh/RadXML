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

function ParticleSystemEmitter (name)
{
	this._name = name;
	this._angle = 0;
	this._colour = ColourTypes.White ();
	this._colourRangeStart = ColourTypes.White ();
	this._colourRangeEnd = ColourTypes.White ();
	this._emissionRate = 10;
	this._position = new Vector3 ();
	this._velocity = 1;
	this._velocityMin = 1;
	this._velocityMax = 1;
	this._timeToLive = 5;
	this._timeToLiveMin = 5;
	this._timeToLiveMax = 5;
	this._duration = 0;
	this._durationMin = 0;
	this._durationMax = 0;
	this._repeatDelay = 0;
	this._repeatDelayMin = 0;
	this._repeatDelayMax = 0;

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

		this._angle = RadXML._parseFloat (RadXML._getAttribute (element, "angle").value);
		this._colour = RadXML._parseColourValue (RadXML._getAttribute (element, "colour").value);
		this._colourRangeStart = RadXML._parseColourValue (
			RadXML._getAttribute (element, "colourrangestart").value);
		this._colourRangeEnd = RadXML._parseColourValue (
			RadXML._getAttribute (element, "colourrangeend").value);
		this._direction = parseVector3 (RadXML._getAttribute (element, "direction").value);
		this._emissionRate = RadXML._parseFloat (RadXML._getAttribute (element, "emissionrate").value);
		this._position = parseVector3 (RadXML._getAttribute (element, "position").value);
		this._velocity = RadXML._parseFloat (RadXML._getAttribute (element, "velocity").value);
		this._velocityMin = RadXML._parseFloat (RadXML._getAttribute (element, "velocitymin").value);
		this._velocityMax = RadXML._parseFloat (RadXML._getAttribute (element, "velocitymax").value);
		this._timeToLive = RadXML._parseFloat (RadXML._getAttribute (element, "timetolive").value);
		this._timeToLiveMin = RadXML._parseFloat (RadXML._getAttribute (element, "timetolivemin").value);
		this._timeToLiveMax = RadXML._parseFloat (RadXML._getAttribute (element, "timetolivemax").value);
		this._duration = RadXML._parseFloat (RadXML._getAttribute (element, "duration").value);
		this._durationMin = RadXML._parseFloat (RadXML._getAttribute (element, "durationmin").value);
		this._durationMax = RadXML._parseFloat (RadXML._getAttribute (element, "durationmax").value);
		this._repeatDelay = RadXML._parseFloat (RadXML._getAttribute (element, "repeatdelay").value);
		this._repeatDelayMin = RadXML._parseFloat (RadXML._getAttribute (element, "repeatdelaymin").value);
		this._repeatDelayMax = RadXML._parseFloat (RadXML._getAttribute (element, "repeatdelaymax").value);
	}

	this._create = function (parent)
	{
		parent._addEmitter (this);
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

