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

function OverlayElement (name)
{
	this._name = name;
	this._type = GameObjectTypes.GOT_OVERLAY_ELEMENT;
	this._disableInputs = false;

	this._events = new Events ();
	this._events._addEvent (new Event ("onclick", ""));
	this._events._addEvent (new Event ("onmousedown", ""));
	this._events._addEvent (new Event ("onmouseup", ""));

	this._overlayElement = "";

	this._overlayElementObj = null;

	this._parseXML = function (level, element, parent)
	{
		this._parseMovableXML (level, element, parent);
		this._overlayElement = RadXML._getAttribute (element, "element").value;
		var strDisableInputs = RadXML._getAttribute (element, "disableinputs").value;

		if (strDisableInputs != "")
			this._disableInputs = parseBoolean (strDisableInputs);
	}

	this._create = function (parent)
	{
		this._overlayElementObj = RadXML._overlayEngine.getOverlayElement (this._overlayElement);

		//this.setVisible (this._bVisible);

		this._overlayElementObj._object = this;
		this._overlayElementObj._eventFunction = this._eventHandler;
		this._overlayElementObj.setEventCaptureStatus (!this._disableInputs);
	}

	this._eventHandler = function (eventType)
	{
		var obj = this._object;

		if (eventType == HTMLOverlayRenderSystemEvents.ORSE_MOUSE_DOWN)
			obj._events._executeJavascriptFromEvent ("onmousedown");

		if (eventType == HTMLOverlayRenderSystemEvents.ORSE_MOUSE_CLICK)
			obj._events._executeJavascriptFromEvent ("onclick");

		if (eventType == HTMLOverlayRenderSystemEvents.ORSE_MOUSE_UP)
			obj._events._executeJavascriptFromEvent ("onmouseup");
	}

	this.setText = function (text)
	{
		if (this._overlayElementObj != null)
			this._overlayElementObj.setText (text);
	}

	this.getText = function ()
	{
		var strReturn = "";

		if (this._overlayElementObj != null)
			strReturn = this._overlayElementObj.getText ();

		return (strReturn);
	}

	this.setCaption = function (text)
	{
		if (this._overlayElementObj != null)
			this._overlayElementObj.setText (text);
	}

	this.getCaption = function ()
	{
		var strReturn = "";

		if (this._overlayElementObj != null)
			strReturn = this._overlayElementObj.getText ();

		return (strReturn);
	}

	this.setFontColor = function (color)
	{
		if (this._overlayElementObj != null)
			this._overlayElementObj.setFontColor (color.toHTMLColourString ());
	}

	this.setVisible = function (obj, visible)
	{
		if (this._overlayElementObj != null)
			this._overlayElementObj.setVisible (visible);
	}
}

OverlayElement.prototype = new Movable ();

