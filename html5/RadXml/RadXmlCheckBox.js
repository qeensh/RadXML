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

function CheckBox (name)
{
	this._name = name;
	this._type = AppObjectTypes.AOT_CHECK_BOX;
	this._isChecked = false;

	this._size = newVector2 (0.15, 0.06);

	this._events = new Events ();
	this._events._addEvent (new Event ("onclick", ""));

	this._parseXML = function (level, element, parent)
	{
		this._parseAppObjectXML (level, element, parent);

		var strChecked = RadXML._getAttribute (element, "checked").value;

		if (strChecked != "")
			this._isChecked = parseBoolean (strChecked);
	}

	this._create = function (creationType, parent)
	{
		this._setParent (parent);
		this._appObject = RadXML._appEngine.createCheckBox (this, parent);

		this.setVisible (this._visible);
		this.setPosition (this._position.x, this._position.y);
		this.setSize (this._size.x, this._size.y);
		this.setText (this._text);
		this.setFontSize (this._fontSize);

		if (this._appObject != null)
			this._appObject.setChecked (this._isChecked);
	}

	this.clone = function (newName, parent)
	{
		
	}

	this.isChecked = function ()
	{
		return (this.getSelected ());
	}

	this.getSelected = function ()
	{
		var bReturn = false;

		if (this._appObject != null)
			bReturn = RadXML._appEngine.getSelected (this._appObject);

		return (bReturn);
	}
}

CheckBox.prototype = new AppObject ();

