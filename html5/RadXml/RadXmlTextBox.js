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

function TextBox (name)
{
	this._name = name;
	this._type = AppObjectTypes.AOT_TEXTBOX;

	this._size = newVector2 (0.2, 0.06);
	this._isMultiline = false;
	this._isReadOnly = false;

	this._events = new Events ();
	this._events._addEvent (new Event ("onclick", ""));
	this._events._addEvent (new Event ("ontextchanged", ""));
	this._events._addEvent (new Event ("onenter", ""));

	this._parseXML = function (level, element, parent)
	{
		this._parseAppObjectXML (level, element, parent);
		var strMultiline = RadXML._getAttribute (element, "multiline").value;
		var strReadOnly = RadXML._getAttribute (element, "readonly").value;
		var strOnClick = RadXML._getAttribute (element, "onclick").value;
		var strTextChanged = RadXML._getAttribute (element, "ontextchanged").value;
		var strEnter = RadXML._getAttribute (element, "onenter").value;

		if (strMultiline != "")
			this._isMultiline = parseBoolean (strMultiline);

		if (strReadOnly != "")
			this._isReadOnly = parseBoolean (strReadOnly);

		this._events._setJavascriptToEvent ("onclick", strOnClick);
		this._events._setJavascriptToEvent ("ontextchanged", strTextChanged);
		this._events._setJavascriptToEvent ("onenter", strEnter);
	}

	this._create = function (creationType, parent)
	{
		this._setParent (parent);
		this._appObject = RadXML._appEngine.createTextBox (this, parent);

		this.setVisible (this._visible);
		this.setPosition (this._position.x, this._position.y);
		this.setSize (this._size.x, this._size.y);
		this.setText (this._text);
		this.setReadOnly (this._isReadOnly);
		this.setFontSize (this._fontSize);
		this._setupEvents ();
	}

	this._setupEvents = function ()
	{
		if (this._appObject == null)
			return;

		if (this._events._hshEvents["onenter"]._javascript != "")
		{
			RadXML._addEventListenerToObject (this._appObject._divWindow, "keyup", 
				RadXML._keepContext (this._onEnter, this));
		}
	}

	this._onEnter = function (event)
	{
		if (event.keyCode == 13)
			RadXML.compileAndRunJSString (this._events._hshEvents["onenter"]._javascript);
	}

	this.clone = function (newName, parent)
	{
	}

	this.setEnabled = function (isEnabled)
	{
		this._isReadOnly = !isEnabled;

		if (this._appObject != null)
			RadXML._appEngine.setReadOnly (this._appObject, this._isReadOnly);
	}

	this.getEnabled = function ()
	{
		return (!this._isReadOnly);
	}

	this.setReadOnly = function (isReadOnly)
	{
		this._isReadOnly = isReadOnly;

		if (this._appObject != null)
			RadXML._appEngine.setReadOnly (this._appObject, this._isReadOnly);
	}

	this.getReadOnly = function ()
	{
		return (this._isReadOnly);
	}

	this.scrollToBottom = function ()
	{
		if (this._appObject != null)
			RadXML._appEngine.scrollToBottom (this._appObject);
	}

	this.setFocus = function ()
	{
		if (this._appObject != null)
			RadXML._appEngine.setFocus (this._appObject);
	}

	this.setCaretPosition = function (index)
	{
		if (this._appObject != null)
			RadXML._appEngine.setCaretPosition (this._appObject, index);
	}

	this.getCaretPosition = function ()
	{
		var iCaretPos = null;

		if (this._appObject != null)
			iCaretPos = RadXML._appEngine.getCaretPosition (this._appObject);

		return (iCaretPos);
	}
}

TextBox.prototype = new AppObject ();

