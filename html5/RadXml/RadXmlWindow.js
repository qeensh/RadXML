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

function Window (name)
{
	this._name = name;
	this._type = AppObjectTypes.AOT_WINDOW;
	this._blend = false;

	this._events = new Events ();
	this._events._addEvent (new Event ("onclosed", ""));

	this._parseXML = function (level, element, parent)
	{
		this._parseAppObjectXML (level, element, parent);

		var strOnClosed = RadXML._getAttribute (element, "onclosed").value;
		this._events._setJavascriptToEvent ("onclosed", strOnClosed);

		var strBlend = RadXML._getAttribute (element, "blend").value;

		if (strBlend != "")
			this._blend = parseBoolean (strBlend);
	}

	this._create = function (creationType, parent)
	{
		this._setParent (parent);
		this._appObject = RadXML._appEngine.createWindow (this, parent);

		this.setVisible (this._visible);
		this.setPosition (this._position.x, this._position.y);
		this.setSize (this._size.x, this._size.y);
		this.setText (this._text);

		this._appObject._object = this;
		this._appObject._eventFunction = this._eventHandler;
	}

	this._eventHandler = function (eventType)
	{
		var obj = this._object;

		if (eventType == HTMLAppRenderSystemEvents.RSE_CLOSED)
			obj._onClose ();
	}

	this._onClose = function ()
	{
		this._events._executeJavascriptFromEvent ("onclosed");
		this.setVisible (false);
	}

	this.clone = function (newName, parent)
	{
	}
}

Window.prototype = new AppObject ();

