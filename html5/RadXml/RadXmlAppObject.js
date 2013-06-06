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

var GUIWindowFunctionTypes = 
{
	defGUIFUNC_TYPE_KEY_UP					: 0, 
	defGUIFUNC_TYPE_KEY_DOWN				: 1, 
	defGUIFUNC_TYPE_MOUSE_CLICK				: 2, 
	defGUIFUNC_TYPE_MOUSE_DOWN				: 3, 
	defGUIFUNC_TYPE_MOUSE_UP				: 4, 
	defGUIFUNC_TYPE_MOUSE_ENTERS			: 5, 
	defGUIFUNC_TYPE_MOUSE_LEAVES			: 6, 
	defGUIFUNC_TYPE_SLIDER					: 7, 
	defGUIFUNC_TYPE_COMBOBOX				: 8, 
	defGUIFUNC_TYPE_ACTIVATED				: 9, 
	defGUIFUNC_TYPE_DEACTIVATED				: 10, 
	defGUIFUNC_TYPE_INPUT_GAINED			: 11, 
	defGUIFUNC_TYPE_INPUT_LOST				: 12, 
	defGUIFUNC_TYPE_INPUT_ENABLED			: 13, 
	defGUIFUNC_TYPE_INPUT_DISABLED			: 14, 
	defGUIFUNC_TYPE_INPUT_CLOSED			: 15, 
	defGUIFUNC_TYPE_LISTBOX_SELECTED		: 16, 
	defGUIFUNC_TYPE_WINDOW_SHOW				: 17, 
	defGUIFUNC_TYPE_WINDOW_HIDE				: 18
};

var AppCursorTypes = 
{
	NORMAL:			1, 
	POINTER:		2, 
};

function AppObject ()
{
	this._name = "";
	this._type = 0;

	this._position = new Vector2 ();
	this._size = new Vector2 ();
	this._visible = true;
	this._text = "";
	this._fontSize = "12";
	this._fontColor = ColourTypes.Black;
	this._underline = false;
	this._cursor = AppCursorTypes.NORMAL;

	this._parent = null;

	this._appObject = null;

	this._parseAppObjectXML = function (level, element, parent)
	{
		var strName = RadXML._getAttribute (element, "name").value;
		var strVisible = RadXML._getAttribute (element, "visible").value;
		var strPosition = RadXML._getAttribute (element, "position").value;
		var strX = RadXML._getAttribute (element, "x").value;
		var strY = RadXML._getAttribute (element, "y").value;
		var strSize = RadXML._getAttribute (element, "size").value;
		var strWidth = RadXML._getAttribute (element, "width").value;
		var strHeight = RadXML._getAttribute (element, "height").value;
		var strText = RadXML._getAttribute (element, "text").value;
		var strFontSize = RadXML._getAttribute (element, "fontsize").value;
		var strFontColor = RadXML._getAttribute (element, "fontcolor").value;
		var strUnderline = RadXML._getAttribute (element, "underline").value;
		var strCursor = RadXML._getAttribute (element, "cursor").value;
		var bVisible = true;

		if (strName == "")
		{
			var iRandName = RadXML._parseInt (Math.random () * 1000000000);
			var objFound = level._getAppObject (iRandName, true);

			while (objFound != null)
			{
				iRandName = RadXML._parseInt (Math.random () * 1000000000);
				objFound = level._getAppObject (iRandName, true);
			}

			strName = "obj" + RadXML._parseInt (iRandName);
		}

		if (strVisible != "")
			bVisible = parseBoolean (strVisible);

		if (strFontSize != "")
			this._fontSize = strFontSize;

		if (strFontColor != "")
			this._fontColor = RadXML._parseColourValue (strFontColor);

		if (strUnderline != "")
			this._underline = parseBoolean (strUnderline);

		if (strCursor != "")
		{
			strCursor = strCursor.toLowerCase ();

			if (strCursor == "normal")
				this._cursor = AppCursorTypes.NORMAL;

			if (strCursor == "pointer")
				this._cursor = AppCursorTypes.POINTER;
		}

		if (strPosition != "")
		{
			strPosition = strPosition.removeWhitespaces ();
			var aryString = strPosition.split (",");
			this._position.x = RadXML._parseFloat (aryString[0]);
			this._position.y = RadXML._parseFloat (aryString[1]);
		}

		if (strX != "")
			this._position.x = RadXML._parseFloat (strX);

		if (strY != "")
			this._position.y = RadXML._parseFloat (strY);

		if (strSize != "")
		{
			strSize = strSize.removeWhitespaces ();
			var aryString = strSize.split (",");
			this._size.x = RadXML._parseFloat (aryString[0]);
			this._size.y = RadXML._parseFloat (aryString[1]);
		}

		if (strWidth != "")
			this._size.x = RadXML._parseFloat (strWidth);

		if (strHeight != "")
			this._size.y = RadXML._parseFloat (strHeight);

		this._setName (strName);
		this.setVisible (bVisible);
		this.setPosition (this._position.x, this._position.y);
		this.setSize (this._size.x, this._size.y);
		this.setText (strText);
	}

	this._setParent = function (parent)
	{
		this._parent = parent;
	}

	this._getParent = function ()
	{
		return (this._parent);
	}

	this._setName = function (name)
	{
		this._name = name;
	}

	this.getName = function ()
	{
		return (this._name);
	}

	this.clone = function (newName)
	{
	}

	this.setPosition = function (x, y)
	{
		this._position.x = x;
		this._position.y = y;

		if (this._appObject != null)
			RadXML._appEngine.setPosition (this._appObject, x, y);
	}

	this.getPosition = function ()
	{
		return (this._position);
	}

	this.setSize = function (x, y)
	{
		this._size.x = x;
		this._size.y = y;

		if (this._appObject != null)
			RadXML._appEngine.setSize (this._appObject, x, y);
	}

	this.getSize = function ()
	{
		return (this._size);
	}

	this.setVisible = function (isVisible)
	{
		this._visible = isVisible;

		if (this._appObject != null)
			RadXML._appEngine.setVisible (this._appObject, isVisible);
	}

	this.getVisible = function ()
	{
		return (this._visible);
	}

	this.setText = function (text)
	{
		this._text = text;

		if (this._appObject != null)
			RadXML._appEngine.setText (this._appObject, text);
	}

	this.getText = function ()
	{
		var strText = RadXML._appEngine.getText (this._appObject);

		return (strText);
	}

	this.setFontSize = function (fontSize)
	{
		this._fontSize = fontSize;

		if (this._appObject != null)
			RadXML._appEngine.setFontSize (this._appObject, this._fontSize);
	}

	this.getFontSize = function ()
	{
		return (this._fontSize);
	}

	this.setFontColour = function (fontColour)
	{
		this._fontColor = fontColour;

		if (this._appObject != null)
			RadXML._appEngine.setFontColour (this._appObject, this._fontColor);
	}

	this.setUnderline = function (isUnderlined)
	{
		this._underline = isUnderlined;

		if (this._appObject != null)
			RadXML._appEngine.setUnderline (this._appObject, this._underline);
	}

	this.setCursor = function (type)
	{
		this._cursor = type;

		if (this._appObject != null)
			RadXML._appEngine.setCursor (this._appObject, this._cursor);
	}
}

