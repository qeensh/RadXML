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

function ComboBox (name)
{
	this._name = name;
	this._type = AppObjectTypes.AOT_COMBO_BOX;
	this._items = new Array ();
	this._selectedIndex = 0;

	this._size = newVector2 (0.15, 0.06);

	this._events = new Events ();
	this._events._addEvent (new Event ("onclick", ""));
	this._events._addEvent (new Event ("onselected", ""));

	this._parseXML = function (level, element, parent)
	{
		this._parseAppObjectXML (level, element, parent);

		var strSelectedIndex = RadXML._getAttribute (element, "selectedindex").value;

		if (strSelectedIndex != "")
			this._selectedIndex = parseInt (strSelectedIndex);

		var strOnClick = RadXML._getAttribute (element, "onclick").value;
		this._events._setJavascriptToEvent ("onclick", strOnClick);

		var strOnSelected = RadXML._getAttribute (element, "onselected").value;
		this._events._setJavascriptToEvent ("onselected", strOnSelected);
	}

	this._create = function (creationType, parent)
	{
		this._setParent (parent);
		this._appObject = RadXML._appEngine.createComboBox (this, parent);

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

		if (eventType == HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK)
			obj._events._executeJavascriptFromEvent ("onclick");

		if (eventType == HTMLAppRenderSystemEvents.RSE_COMBO_BOX_ITEM_SELECTED)
			obj._events._executeJavascriptFromEvent ("onselected");
	}

	this.clone = function (newName, parent)
	{
	}

	this._addItem = function (text)
	{
		this._items.push (text);

		if (this._appObject != null)
			this._appObject.addItem (text);

		if (this._items.length > this._selectedIndex)
			this.selectItem (this._selectedIndex);
	}

	this.addItem = function (text)
	{
		this._items.push (text);

		if (this._appObject != null)
			this._appObject.addItem (text);
	}

	this.setItem = function (index, text)
	{
		this._items[index] = text;

		if (this._appObject != null)
			this._appObject.setItem (index, text);
	}

	this.getItem = function (index)
	{
		var strItem = this._items[index];

		if (this._appObject != null)
			strItem = this._appObject.getItem (index);

		return (strItem);
	}

	this.clear = function ()
	{
		for (var iIdx = (this._items.length - 1); iIdx >= 0; iIdx--)
			this.removeItem (iIdx);
	}

	this.removeItem = function (index)
	{
		this._items.splice (index, 1);

		if (this._appObject != null)
			this._appObject.removeItem (index);
	}

	this.getNumItems = function ()
	{
		var iCount = this._items.length;

		if (this._appObject != null)
			iCount = this._appObject.getNumItems ();

		return (iCount);
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

	this.getText = function ()
	{
		return (this.getSelectedText ());
	}

	this.clearItems = function ()
	{
		this._items.splice (0, this._items.length);

		if (this._appObject != null)
			this._appObject.clearItems ();
	}

	this.getSelectedIndex = function ()
	{
		var iIndex = 0;

		if (this._appObject != null)
			iIndex = RadXML._appEngine.getSelectedIndex (this._appObject);

		return (iIndex);
	}

	this.getSelectedText = function ()
	{
		var strText = "";

		if (this._appObject != null)
			strText = RadXML._appEngine.getSelectedText (this._appObject);

		return (strText);
	}

	this.selectItem = function (index)
	{
		if (this._appObject != null)
			RadXML._appEngine.selectItem (this._appObject, index);
	}
}

ComboBox.prototype = new AppObject ();

