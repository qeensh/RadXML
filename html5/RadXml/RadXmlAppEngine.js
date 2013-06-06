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

var AppRenderSystemTypes = 
{
	WEBGL: 1, 
	HTML: 2
};

function AppEngine ()
{
	this._renderSystem = null;
	this._renderSystemType = AppRenderSystemTypes.HTML;

	this.initialize = function ()
	{
		if (this._renderSystemType == AppRenderSystemTypes.HTML)
		{
			this._renderSystem = new HTMLAppRenderSystem ();
			this._renderSystem.setupInputs (this._renderSystem);
		}
	}

	this.createWindow = function (appObject, parent)
	{
		var parentObj = null;

		if (parent != null)
			parentObj = parent._appObject;

		var obj = this._renderSystem.createWindow (appObject._name, appObject._text, 
				appObject._size.x, appObject._size.y, parentObj, appObject._blend);

		return (obj);
	}

	this.createLabel = function (appObject, parent)
	{
		var parentObj = null;

		if (parent != null)
			parentObj = parent._appObject;

		var obj = this._renderSystem.createLabel (appObject._name, appObject._text, 
									appObject._size.x, appObject._size.y, parentObj);

		return (obj);
	}

	this.createButton = function (appObject, parent)
	{
		var parentObj = null;

		if (parent != null)
			parentObj = parent._appObject;

		var obj = this._renderSystem.createButton (appObject._name, appObject._text, 
									appObject._size.x, appObject._size.y, parentObj);

		return (obj);
	}

	this.createTextBox = function (appObject, parent)
	{
		var parentObj = null;

		if (parent != null)
			parentObj = parent._appObject;

		var obj = this._renderSystem.createTextBox (appObject._name, appObject._text, 
									appObject._size.x, appObject._size.y, parentObj, 
									appObject._isMultiline);

		return (obj);
	}

	this.createImageBox = function (appObject, parent)
	{
		var parentObj = null;

		if (parent != null)
			parentObj = parent._appObject;

		var obj = this._renderSystem.createImageBox (appObject._name, appObject._src, 
									appObject._size.x, appObject._size.y, parentObj);

		return (obj);
	}

	this.createCheckBox = function (appObject, parent)
	{
		var parentObj = null;

		if (parent != null)
			parentObj = parent._appObject;

		var obj = this._renderSystem.createCheckBox (appObject._name, appObject._text, 
									appObject._size.x, appObject._size.y, parentObj);

		return (obj);
	}

	this.createComboBox = function (appObject, parent)
	{
		var parentObj = null;

		if (parent != null)
			parentObj = parent._appObject;

		var obj = this._renderSystem.createComboBox (appObject._name, appObject._text, 
									appObject._size.x, appObject._size.y, parentObj);

		return (obj);
	}

	this.createGroupBox = function (appObject, parent)
	{
		var parentObj = null;

		if (parent != null)
			parentObj = parent._appObject;

		var obj = this._renderSystem.createGroupBox (appObject._name, appObject._text, 
									appObject._size.x, appObject._size.y, parentObj);

		return (obj);
	}

	this.createSelectList = function (appObject, parent)
	{
		var parentObj = null;

		if (parent != null)
			parentObj = parent._appObject;

		var obj = this._renderSystem.createSelectList (appObject._name, appObject._text, 
									appObject._size.x, appObject._size.y, parentObj);

		return (obj);
	}

	this.createRadioButton = function (appObject, parent)
	{
		var parentObj = null;

		if (parent != null)
			parentObj = parent._appObject;

		var obj = this._renderSystem.createRadioButton (appObject._name, appObject._text, 
					appObject._size.x, appObject._size.y, parentObj, appObject._radioGroup, 
					appObject._isSelected);

		return (obj);
	}

	this.createSelectListGrid = function (appObject, parent)
	{
		var parentObj = null;

		if (parent != null)
			parentObj = parent._appObject;

		var obj = this._renderSystem.createSelectListGrid (appObject._name, appObject._text, 
			appObject._size.x, appObject._size.y, parentObj, appObject._headers, appObject._items);

		return (obj);
	}

	this.recreateSelectListGrid = function (appObject, parent)
	{
		var parentObj = null;

		if (parent != null)
			parentObj = parent._appObject;

		var obj = this._renderSystem.recreateSelectListGrid (parentObj);

		return (obj);
	}

	this.recreateComboBoxList = function (appObject, parent)
	{
		var parentObj = null;

		if (parent != null)
			parentObj = parent._appObject;

		var obj = this._renderSystem.recreateComboBoxList (parentObj);

		return (obj);
	}

	this.setPosition = function (appObject, x, y)
	{
		var pos = this._renderSystem.getAbsoluteSize (x, y, appObject._parent);

		appObject.setPosition (pos[0], pos[1]);
	}

	this.setSize = function (appObject, width, height)
	{
		var size = this._renderSystem.getAbsoluteSize (width, height, appObject._parent);

		appObject.setSize (size[0], size[1]);
	}

	this.setFocus = function (appObject)
	{
		appObject.setFocus ();
	}

	this.setVisible = function (appObject, visible)
	{
		appObject.setVisible (visible);
	}

	this.setText = function (appObject, text)
	{
		appObject.setText (text);
	}

	this.getText = function (appObject)
	{
		return (appObject.getText ());
	}

	this.setFontSize = function (appObject, fontSize)
	{
		appObject.setFontSize (fontSize);
	}

	this.setFontColour = function (appObject, fontColour)
	{
		appObject.setFontColour (fontColour);
	}

	this.setUnderline = function (appObject, isUnderlined)
	{
		appObject.setUnderline (isUnderlined);
	}

	this.setCursor = function (appObject, type)
	{
		appObject.setCursor (type);
	}

	this.setReadOnly = function (appObject, isReadOnly)
	{
		appObject.setReadOnly (isReadOnly);
	}

	this.setSelected = function (appObject, isSelected)
	{
		appObject.setSelected (isSelected);
	}

	this.selectItem = function (appObject, index)
	{
		appObject.selectItem (index);
	}

	this.getSelectedIndex = function (appObject)
	{
		return (appObject.getSelectedIndex ());
	}

	this.getSelectedText = function (appObject, subIndex)
	{
		return (appObject.getSelectedText (subIndex));
	}

	this.getSelected = function (appObject)
	{
		return (appObject.getChecked ());
	}

	this.getChecked = function (appObject)
	{
		return (appObject.getChecked ());
	}

	this.scrollToBottom = function (appObject)
	{
		appObject.scrollToBottom ();
	}

	this.setCaretPosition = function (appObject, index)
	{
		appObject.setCaretPosition (index);
	}

	this.getCaretPosition = function (appObject)
	{
		return (appObject.getCaretPosition ());
	}

	this.minWindow = function (event, name)
	{
		this._renderSystem.minWindow (event, name);
	}

	this.maxWindow = function (event, name)
	{
		this._renderSystem.maxWindow (event, name);
	}

	this.moveWindow = function (event, name)
	{
		this._renderSystem.moveWindow (event, name);
	}

	this.closeWindow = function (event, name)
	{
		this._renderSystem.closeWindow (event, name);
	}
}

