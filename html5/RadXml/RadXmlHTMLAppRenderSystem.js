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

var HTMLAppRenderSystemEvents = 
{
	RSE_MOUSE_DOWN:							1, 
	RSE_MOUSE_CLICK:						2,
	RSE_CLOSED:								3,
	RSE_SELECT_LIST_GRID_ITEM_CLICKED:		4, 
	RSE_COMBO_BOX_ITEM_SELECTED:			5
};

function getHTMLText (text)
{
	var strText = text.replace (/ /g, "&nbsp;");
	strText = strText.replace (/\\n/g, "<br />");
	strText = strText.replace (/(\n)/g, "<br />");

	return (strText);
}

function _getObjectSize (strObjectId)
{
	var objObject = document.getElementById (strObjectId);
	var iWidth = 0;
	var iHeight = 0;

	if (objObject.clientWidth)
		iWidth = objObject.clientWidth;

	if (objObject.clientWidth)
		iHeight = objObject.clientHeight;

	if (objObject.offsetWidth)
		iWidth = objObject.offsetWidth;

	if (objObject.offsetHeight)
		iHeight = objObject.offsetHeight;

	return ([iWidth, iHeight]);
}

function HTMLAppRenderSystem ()
{
	this._aryWindows = new Array ();
	this._iTopWindow = 0;
	this._wPrevWindow = null;

	this._mouseButtonDown = 0;

	this._aryMousePos = [0, 0];
	this._aryPrevMousePos = [0, 0];

	this._parentWidth = window.innerWidth;
	this._parentHeight = window.innerHeight;

	function HTMLAppObject ()
	{
		this._name = "";
		this._parent = null;
		this._text = "";
		this._x = 0;
		this._y = 0;
		this._width = 500;
		this._height = 500;
		this._prevWidth = 500;
		this._prevHeight = 400;
		this._fontSize = 12;
		this._fontColour = "#000000";
		this._underline = "none";
		this._cursor = "default";
		this._visible = true;
		this._eventFunction = null;

		this._content = null;
		this._renderSystem = null;

		this.setSize = function (newWidth, newHeight)
		{
			this._width = newWidth;
			this._height = newHeight;

			this._divWindow.style.width = this._width + "px";
			this._divWindow.style.height = this._height + "px";
		}

		this.getSize = function ()
		{
			var arySize = [this._width, this._height];

			return (arySize);
		}

		this.setPosition = function (x, y)
		{
			this._x = x;
			this._y = y;

			this._divWindow.style.left = x + "px";
			this._divWindow.style.top = y + "px";
		}

		this.getPosition = function ()
		{
			var aryPos = [this._divWindow.offsetLeft, this._divWindow.offsetTop];

			return (aryPos);
		}

		this.setVisible = function (visible)
		{
			this._visible = visible;

			if (this._visible == true)
				this._divWindow.style.display = "block";
			else
				this._divWindow.style.display = "none";
		}

		this.setText = function (text)
		{
			this._text = text;
		}

		this.getText = function ()
		{
			return (this._text);
		}

		this.setFontSize = function (fontSize)
		{
			this._fontSize = fontSize;
			this._divWindow.style.fontSize = this._fontSize + "px";
		}

		this.setFontColour = function (fontColour)
		{
			this._fontColour = fontColour;
			this._divWindow.style.color = this._fontColour;
		}

		this.setUnderline = function (isUnderlined)
		{
			if (isUnderlined == true)
				this._underline = "underline";
			else
				this._underline = "none";

			this._divWindow.style.textDecoration = this._underline;
		}

		this.setCursor = function (type)
		{
			if (type == 1)
				this._cursor = "default";
			else
				this._cursor = "pointer";

			this._divWindow.style.cursor = this._cursor;
		}
	}

	function HTMLWindow (renderSystem)
	{
		this._divWindow = null;
		this._tblWindow = null;
		this._spnTitle = null;
		this._prevWindowPos = [0, 0];
		this._strURL = "";
		this._strTitle = "";
		this._maximized = false;
		this._callback = null;
		this._callbackArray = null;
		this._javascriptCode = "";
		this._blend = false;

		this._renderSystem = renderSystem;

		this.create = function (newId, newText)
		{
			this._name = newId;
			this._text = newText;

			if (this._blend == false)
			{
				this._divWindow = document.createElement ("div");
				this._divWindow.id = "divWindow-" + this._name;
				this._divWindow.setAttribute ("class", "divWindow");
				this._divWindow.setAttribute ("className", "divWindow");
				this._divWindow.setAttribute ("onmousedown", "this.wWindow.setFocus ();");
				this._divWindow.setAttribute ("onselectstart", "return (false);");
				this._divWindow.setAttribute ("onclick", "this.wWindow.setFocus ();");
				this._divWindow.wWindow = this;
				this._divWindow.style.position = "absolute";
				this._divWindow.style.top = "0px";
				this._divWindow.style.left = "0px";
				this._divWindow.style.zIndex = this._renderSystem._iTopWindow++;
				this._divWindow.style.display = "block";
				document.body.appendChild (this._divWindow);
	
				this._tblWindow = document.createElement ("table");
				this._tblWindow.setAttribute ("class", "tblWindow");
				this._tblWindow.setAttribute ("className", "tblWindow");
				this._tblWindow.style.width = this._width + "px";
				this._tblWindow.style.height = this._height + "px";
				this._divWindow.appendChild (this._tblWindow);
	
				var trTr = document.createElement ("tr");
				this._tblWindow.appendChild (trTr);
	
				var tdTd = document.createElement ("td");
				tdTd.id = "tdTitlebar" + this._name;
				tdTd.setAttribute ("class", "tdTitlebar");
				tdTd.setAttribute ("className", "tdTitlebar");
				tdTd.setAttribute ("ondblclick", "RadXML._appEngine.maxWindow (event, \"" + this._name + "\");");
				tdTd.setAttribute ("onmousemove", "RadXML._appEngine.moveWindow (event, \"" + this._name + "\")");
				tdTd.setAttribute ("onselectstart", "return (false);");
				trTr.appendChild (tdTd);
	
				this._spnTitle = document.createElement ("span");
				this._spnTitle.innerHTML = getHTMLText (this._text);
				this._spnTitle.setAttribute ("onselectstart", "return (false);");
				this._spnTitle.setAttribute ("class", "tblWindowTitle");
				this._spnTitle.setAttribute ("className", "tblWindowTitle");
				tdTd.appendChild (this._spnTitle);
	
				var imgImage = document.createElement ("img");
				imgImage.setAttribute ("class", "imgMinimize");
				imgImage.setAttribute ("className", "imgMinimize");
				imgImage.setAttribute ("onselectstart", "return (false);");
				imgImage.setAttribute ("onclick", "RadXML._appEngine.minWindow (event, \"" + this._name + "\");");
				tdTd.appendChild (imgImage);
	
				imgImage = document.createElement ("img");
				imgImage.setAttribute ("class", "imgMaximize");
				imgImage.setAttribute ("className", "imgMaximize");
				imgImage.setAttribute ("onselectstart", "return (false);");
				imgImage.setAttribute ("onclick", "RadXML._appEngine.maxWindow (event, \"" + this._name + "\");");
				tdTd.appendChild (imgImage);
	
				imgImage = document.createElement ("img");
				imgImage.setAttribute ("class", "imgClose");
				imgImage.setAttribute ("className", "imgClose");
				imgImage.setAttribute ("onclick", "RadXML._appEngine.closeWindow (event, \"" + this._name + "\");");
				imgImage.setAttribute ("onselectstart", "return (false);");
				tdTd.appendChild (imgImage);
	
				trTr = document.createElement ("tr");
				this._tblWindow.appendChild (trTr);
	
				tdTd = document.createElement ("td");
				tdTd.setAttribute ("class", "tdFrame");
				tdTd.setAttribute ("className", "tdFrame");
				trTr.appendChild (tdTd);
	
				this._content = document.createElement ("div");
				this._content.setAttribute ("class", "tdiFrame");
				this._content.setAttribute ("className", "tdiFrame");
				this._content.style.width = this._width;
				this._content.style.height = this._height;
				this._content.wWindow = this;
				this._content.id = "content-" + this._name;
				this._content.name = this._name;
				tdTd.appendChild (this._content);
			}
			else
			{
				this._divWindow = document.createElement ("div");
				this._divWindow.id = "divWindow-" + this._name;
				this._divWindow.setAttribute ("class", "divWindow");
				this._divWindow.setAttribute ("className", "divWindow");
				this._divWindow.setAttribute ("onmousedown", "this.wWindow.setFocus ();");
				this._divWindow.setAttribute ("onselectstart", "return (false);");
				this._divWindow.setAttribute ("onclick", "this.wWindow.setFocus ();");
				this._divWindow.wWindow = this;
				this._divWindow.style.position = "absolute";
				this._divWindow.style.top = "0px";
				this._divWindow.style.left = "0px";
				this._divWindow.style.zIndex = this._renderSystem._iTopWindow++;
				this._divWindow.style.display = "block";
				document.body.appendChild (this._divWindow);
	
				this._content = document.createElement ("div");
					this._content.setAttribute ("class", "tdiFrame");
				this._content.setAttribute ("className", "tdiFrame");
				this._content.style.width = this._width;
				this._content.style.height = this._height;
				this._content.wWindow = this;
				this._content.id = "content-" + this._name;
				this._content.name = this._name;
				this._divWindow.appendChild (this._content);
			}

			var v2dTemp = this.getPosition ();
			this._x = v2dTemp[0];
			this._y = v2dTemp[1];
			this._renderSystem._wPrevWindow = this;
		}

		this.setSize = function (newWidth, newHeight)
		{
			this._width = newWidth;
			this._height = newHeight;

			if (this._blend == false)
			{
				this._tblWindow.style.width = this._width;
				this._tblWindow.style.height = this._height;
	
				var arySize = _getObjectSize ("tdTitlebar" + this._name);
				var aryNewSize = [this._width, (this._height - (arySize[1] + 5))];
	
				this._content.style.width = aryNewSize[0];
				this._content.style.height = aryNewSize[1];
			}
			else
			{
				this._divWindow.style.width = this._width;
				this._divWindow.style.height = this._height;
			}
		}

		this.getSize = function ()
		{
			var arySize = [this._width, this._height];

			return (arySize);
		}

		this.setPosition = function (x, y)
		{
			this._prevWindowPos[0] = this._x;
			this._prevWindowPos[1] = this._y;

			this._divWindow.style.left = x + "px";
			this._divWindow.style.top = y + "px";

			this._x = x;
			this._y = y;
		}

		this.getPosition = function ()
		{
			var aryPos = [this._divWindow.offsetLeft, this._divWindow.offsetTop];

			return (aryPos);
		}

		this.setText = function (text)
		{
			this._text = getHTMLText (text);

			if (this._spnTitle != null)
				this._spnTitle.innerHTML = this._text;
		}

		this.setVisible = function (visible)
		{
			this._visible = visible;

			if (this._visible == true)
				this._divWindow.style.display = "block";
			else
				this._divWindow.style.display = "none";
		}

		this.close = function ()
		{
			for (var iIdx = 0; iIdx < this._renderSystem._aryWindows.length; iIdx++)
			{
				if (this._name == this._renderSystem._aryWindows[iIdx]._name)
				{
					this._renderSystem._aryWindows.splice (iIdx, 1);

					break;
				}
			}

			document.body.removeChild (this._divWindow);
			this._renderSystem._iTopWindow--;
		}

		this.setFocus = function ()
		{
			this._renderSystem._wPrevWindow._divWindow.style.zIndex = (this._renderSystem._iTopWindow - 1);
			this._divWindow.style.zIndex = this._renderSystem._iTopWindow;
			this._renderSystem._wPrevWindow = this;
		}
	}

	HTMLWindow.prototype = new HTMLAppObject ();

	function HTMLLabel (renderSystem)
	{
		this._divRelativeWindow = null;
		this._divWindow = null;

		this._renderSystem = renderSystem;

		this._x = 0;
		this._y = 0;
		this._width = 500;
		this._height = 500;

		this.create = function (newId, newText)
		{
			this._name = newId;
			this._text = newText;

			if (this._parent != null)
			{
				/*this._divRelativeWindow = document.createElement ("div");
				this._divRelativeWindow.id = "divLabelRelative-" + this._name;
				this._divRelativeWindow.wWindow = this;
				this._divRelativeWindow.style.position = "relative";
				this._divRelativeWindow.style.width = this._width + "px";
				this._divRelativeWindow.style.height = this._height + "px";
				this._divRelativeWindow.style.display = "block";*/

				this._divWindow = document.createElement ("div");
				this._divWindow.id = "divLabel-" + this._name;
				this._divWindow.name = "divLabel-" + this._name;
				this._divWindow.setAttribute ("class", "divLabel");
				this._divWindow.setAttribute ("className", "divLabel");
				this._divWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_DOWN);");
				this._divWindow.setAttribute ("onselectstart", "return (false);");
				this._divWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK);");
				this._divWindow.innerHTML = getHTMLText (this._text);
				this._divWindow.wWindow = this;
				this._divWindow.style.position = "absolute";
				this._divWindow.style.left = this._x + "px";
				this._divWindow.style.top = this._y + "px";
				this._divWindow.style.width = this._width + "px";
				this._divWindow.style.height = this._height + "px";
				this._divWindow.style.fontSize = this._fontSize + "px";
				//this._divWindow.style.zIndex = this._renderSystem._iTopWindow++;
				this._divWindow.style.display = "block";

				//this._divRelativeWindow.appendChild (this._divWindow);
				this._parent._content.appendChild (this._divWindow);
			}
		}

		this.receivedEvent = function (event, type)
		{
			if (this._eventFunction != null)
				this._eventFunction (type);
		}

		this.setText = function (text)
		{
			this._text = getHTMLText (text);

			if (this._divWindow != null)
				this._divWindow.innerHTML = this._text;
		}

		this.getText = function ()
		{
			return (this._divWindow.innerHTML);
		}
	}

	HTMLLabel.prototype = new HTMLAppObject ();

	function HTMLButton (renderSystem)
	{
		this._divRelativeWindow = null;
		this._divWindow = null;

		this._renderSystem = renderSystem;

		this.create = function (newId, newText)
		{
			this._name = newId;
			this._text = newText;

			if (this._parent != null)
			{
				/*this._divRelativeWindow = document.createElement ("div");
				this._divRelativeWindow.id = "divButtonRelative-" + this._name;
				this._divRelativeWindow.wWindow = this;
				this._divRelativeWindow.style.position = "relative";
				this._divRelativeWindow.style.width = this._width + "px";
				this._divRelativeWindow.style.height = this._height + "px";
				this._divRelativeWindow.style.display = "block";*/

				this._divWindow = document.createElement ("input");
				this._divWindow.type = "button";
				this._divWindow.id = "divButton-" + this._name;
				this._divWindow.name = "divButton-" + this._name;
				this._divWindow.setAttribute ("class", "divButton");
				this._divWindow.setAttribute ("className", "divButton");
				this._divWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_DOWN);");
				this._divWindow.setAttribute ("onselectstart", "return (false);");
				this._divWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK);");
				this._divWindow.value = this._text;
				this._divWindow.wWindow = this;
				this._divWindow.style.position = "absolute";
				this._divWindow.style.left = this._x + "px";
				this._divWindow.style.top = this._y + "px";
				this._divWindow.style.width = this._width + "px";
				this._divWindow.style.height = this._height + "px";
				//this._divWindow.style.zIndex = this._renderSystem._iTopWindow++;
				this._divWindow.style.display = "block";

				//this._divRelativeWindow.appendChild (this._divWindow);
				this._parent._content.appendChild (this._divWindow);
			}
		}

		this.receivedEvent = function (event, type)
		{
			if (this._eventFunction != null)
				this._eventFunction (type);
		}

		this.setText = function (text)
		{
			this._text = text;

			if (this._divWindow != null)
				this._divWindow.value = this._text;
		}

		this.getText = function ()
		{
			return (this._divWindow.value);
		}

		this.setReadOnly = function (isReadOnly)
		{
			this._isReadOnly = isReadOnly;

			this._divWindow.disabled = this._isReadOnly;
		}
	}

	HTMLButton.prototype = new HTMLAppObject ();

	function HTMLTextBox (renderSystem)
	{
		this._divRelativeWindow = null;
		this._divWindow = null;

		this._isMultiLine = false;
		this._isReadOnly = false;

		this._renderSystem = renderSystem;

		this.create = function (newId, newText, isMultiLine)
		{
			if (isMultiLine == null)
				isMultiLine = false;

			this._name = newId;
			this._text = newText;
			this._isMultiLine = isMultiLine;

			if (this._parent != null)
			{
				/*this._divRelativeWindow = document.createElement ("div");
				this._divRelativeWindow.id = "divTextBoxRelative-" + this._name;
				this._divRelativeWindow.wWindow = this;
				this._divRelativeWindow.style.position = "relative";
				this._divRelativeWindow.style.width = this._width + "px";
				this._divRelativeWindow.style.height = this._height + "px";
				this._divRelativeWindow.style.display = "block";*/

				if (isMultiLine == true)
					this._divWindow = document.createElement ("textarea");
				else
				{
					this._divWindow = document.createElement ("input");
					this._divWindow.type = "text";
				}

				this._divWindow.id = "divTextBox-" + this._name;
				this._divWindow.name = "divTextBox-" + this._name;
				this._divWindow.setAttribute ("class", "divTextBox");
				this._divWindow.setAttribute ("className", "divTextBox");
				this._divWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_DOWN);");
				this._divWindow.setAttribute ("onselectstart", "return (false);");
				this._divWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK);");
				this._divWindow.value = this._text;
				this._divWindow.wWindow = this;
				this._divWindow.style.position = "absolute";
				this._divWindow.style.left = this._x + "px";
				this._divWindow.style.top = this._y + "px";
				this._divWindow.style.width = this._width + "px";
				this._divWindow.style.height = this._height + "px";
				//this._divWindow.style.zIndex = this._renderSystem._iTopWindow++;
				this._divWindow.style.display = "block";

				//this._divRelativeWindow.appendChild (this._divWindow);
				this._parent._content.appendChild (this._divWindow);
			}
		}

		this.receivedEvent = function (event, type)
		{
		}

		this.setText = function (text)
		{
			this._text = text;
			this._divWindow.value = this._text;
		}

		this.getText = function ()
		{
			return (this._divWindow.value);
		}

		this.setReadOnly = function (isReadOnly)
		{
			this._isReadOnly = isReadOnly;

			this._divWindow.readOnly = this._isReadOnly;
		}

		this.getReadOnly = function ()
		{
			return (this._isReadOnly);
		}

		this.setFocus = function ()
		{
			this._divWindow.focus ();
		}

		this.scrollToBottom = function ()
		{
			this._divWindow.scrollTop = 999999;
		}

		this.setCaretPosition = function (index)
		{
			if (this._divWindow.createTextRange)
			{
				var cRange = this._divWindow.createTextRange ();

				cRange.collapse (true);
				cRange.moveStart ("character", index);
				cRange.moveEnd ("character", index);
			}

			if (this._divWindow.setSelectionRange)
				this._divWindow.setSelectionRange (index, index);
		}

		this.getCaretPosition = function ()
		{
			var iCaretPos = null;

			if (this._divWindow.selectionStart)
				iCartPos = this._divWindow.selectionStart;

			return (iCaretPos);
		}
	}

	HTMLTextBox.prototype = new HTMLAppObject ();

	function HTMLImageBox (renderSystem)
	{
		this._divRelativeWindow = null;
		this._divWindow = null;

		this._src = "";

		this._renderSystem = renderSystem;

		this.create = function (newId, src)
		{
			this._name = newId;
			this._src = src;

			if (this._parent != null)
			{
				/*this._divRelativeWindow = document.createElement ("div");
				this._divRelativeWindow.id = "divImageBoxRelative-" + this._name;
				this._divRelativeWindow.wWindow = this;
				this._divRelativeWindow.style.position = "relative";
				this._divRelativeWindow.style.width = this._width + "px";
				this._divRelativeWindow.style.height = this._height + "px";
				this._divRelativeWindow.style.display = "block";*/

				this._divWindow = document.createElement ("img");
				this._divWindow.id = "divImageBox-" + this._name;
				this._divWindow.name = "divImageBox-" + this._name;
				this._divWindow.setAttribute ("class", "divImageBox");
				this._divWindow.setAttribute ("className", "divImageBox");
				this._divWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_DOWN);");
				this._divWindow.setAttribute ("onselectstart", "return (false);");
				this._divWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK);");
				this._divWindow.src = this._src;
				this._divWindow.wWindow = this;
				this._divWindow.style.position = "absolute";
				this._divWindow.style.left = this._x + "px";
				this._divWindow.style.top = this._y + "px";
				this._divWindow.style.width = this._width + "px";
				this._divWindow.style.height = this._height + "px";
				//this._divWindow.style.zIndex = this._renderSystem._iTopWindow++;
				this._divWindow.style.display = "block";

				//this._divRelativeWindow.appendChild (this._divWindow);
				this._parent._content.appendChild (this._divWindow);
			}
		}

		this.receivedEvent = function (event, type)
		{
		}

		this.getSrc = function ()
		{
			return (this._divWindow.src);
		}
	}

	HTMLImageBox.prototype = new HTMLAppObject ();

	function HTMLCheckBox (renderSystem)
	{
		this._divRelativeWindow = null;
		this._divWindow = null;
		this._divLabel = null;

		this._renderSystem = renderSystem;

		this.create = function (newId, newText)
		{
			this._name = newId;
			this._text = newText;

			if (this._parent != null)
			{
				/*this._divRelativeWindow = document.createElement ("div");
				this._divRelativeWindow.id = "divButtonRelative-" + this._name;
				this._divRelativeWindow.wWindow = this;
				this._divRelativeWindow.style.position = "relative";
				this._divRelativeWindow.style.width = this._width + "px";
				this._divRelativeWindow.style.height = this._height + "px";
				this._divRelativeWindow.style.display = "block";*/

				this._divWindow = document.createElement ("input");
				this._divWindow.type = "checkbox";
				this._divWindow.id = "divCheckBox-" + this._name;
				this._divWindow.name = "divCheckBox-" + this._name;
				this._divWindow.setAttribute ("class", "divCheckBox");
				this._divWindow.setAttribute ("className", "divCheckBox");
				this._divWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_DOWN);");
				this._divWindow.setAttribute ("onselectstart", "return (false);");
				this._divWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK);");
				this._divWindow.value = "1";
				this._divWindow.wWindow = this;
				this._divWindow.style.position = "absolute";
				this._divWindow.style.left = this._x + "px";
				this._divWindow.style.top = this._y + "px";
				this._divWindow.style.width = "20px";
				this._divWindow.style.height = "20px";
				//this._divWindow.style.zIndex = this._renderSystem._iTopWindow++;
				this._divWindow.style.display = "block";

				this._divLabel = document.createElement ("label");
				this._divLabel.id = "divCheckBoxLabel-" + this._name;
				this._divLabel.name = "divCheckBoxLabel-" + this._name;
				this._divLabel.setAttribute ("class", "divCheckBoxLabel");
				this._divLabel.setAttribute ("className", "divCheckBoxLabel");
				this._divLabel.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_DOWN);");
				this._divLabel.setAttribute ("onselectstart", "return (false);");
				this._divLabel.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK);");
				this._divLabel.wWindow = this;
				this._divLabel.style.position = "absolute";
				this._divLabel.style.left = (this._x + (this._width * 0.55)) + "px";
				this._divLabel.style.top = this._y + "px";
				this._divLabel.style.width = this._width + "px";
				this._divLabel.style.height = this._height + "px";
				//this._divLabel.style.zIndex = this._renderSystem._iTopWindow++;
				this._divLabel.style.display = "block";
				this._divLabel.innerHTML = getHTMLText (this._text);
				this._divLabel.htmlFor = this._divWindow.id;

				//this._divRelativeWindow.appendChild (this._divWindow);
				this._parent._content.appendChild (this._divWindow);
				this._parent._content.appendChild (this._divLabel);
			}
		}

		this.receivedEvent = function (event, type)
		{
			if (this._eventFunction != null)
				this._eventFunction (type);
		}

		this.setPosition = function (x, y)
		{
			this._x = x;
			this._y = y;

			this._divWindow.style.left = x + "px";
			this._divWindow.style.top = y + "px";

			this._divLabel.style.left = (x + 25) + "px";
			this._divLabel.style.top = (y + 2) + "px";
		}

		this.setSize = function (newWidth, newHeight)
		{
			this._width = newWidth;
			this._height = newHeight;

			this._divWindow.style.width = "20px";
			this._divWindow.style.height = "20px";

			this._divLabel.style.width = this._width + "px";
			this._divLabel.style.height = this._height + "px";
		}

		this.setChecked = function (isChecked)
		{
			this._divWindow.checked = isChecked;
		}

		this.getChecked = function ()
		{
			return (this._divWindow.checked);
		}
	}

	HTMLCheckBox.prototype = new HTMLAppObject ();

	function HTMLComboBox (renderSystem)
	{
		this._divRelativeWindow = null;
		this._divWindow = null;

		this._renderSystem = renderSystem;

		this.create = function (newId, newText)
		{
			this._name = newId;
			this._text = newText;

			if (this._parent != null)
			{
				/*this._divRelativeWindow = document.createElement ("div");
				this._divRelativeWindow.id = "divButtonRelative-" + this._name;
				this._divRelativeWindow.wWindow = this;
				this._divRelativeWindow.style.position = "relative";
				this._divRelativeWindow.style.width = this._width + "px";
				this._divRelativeWindow.style.height = this._height + "px";
				this._divRelativeWindow.style.display = "block";*/

				this._divWindow = document.createElement ("select");
				this._divWindow.id = "divComboBox-" + this._name;
				this._divWindow.name = "divComboBox-" + this._name;
				this._divWindow.setAttribute ("class", "divComboBox");
				this._divWindow.setAttribute ("className", "divComboBox");
				this._divWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_DOWN);");
				this._divWindow.setAttribute ("onselectstart", "return (false);");
				this._divWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK);");
				this._divWindow.setAttribute ("onchange", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_COMBO_BOX_ITEM_SELECTED);");
				this._divWindow.wWindow = this;
				this._divWindow.style.position = "absolute";
				this._divWindow.style.left = this._x + "px";
				this._divWindow.style.top = this._y + "px";
				this._divWindow.style.width = "20px";
				this._divWindow.style.height = "20px";
				//this._divWindow.style.zIndex = this._renderSystem._iTopWindow++;
				this._divWindow.style.display = "block";

				//this._divRelativeWindow.appendChild (this._divWindow);
				this._parent._content.appendChild (this._divWindow);
			}
		}

		this.receivedEvent = function (event, type)
		{
			if (this._eventFunction != null)
				this._eventFunction (type);
		}

		this.addItem = function (text)
		{
			var obj = document.createElement ("option");
			obj.id = "optComboBoxItem-" + this._name;
			obj.name = "optComboBoxItem-" + this._name;
			obj.text = text;

			this._divWindow.add (obj);
		}

		this.setItem = function (index, text)
		{
			var obj = this._divWindow.options[index];
			obj.text = text;
		}

		this.getItem = function (index)
		{
			var obj = this._divWindow.options[index];
			var strText = obj.text;

			return (strText);
		}

		this.removeItem = function (index)
		{
			this._divWindow.remove (index);
		}

		this.getNumItems = function ()
		{
			return (this._divWindow.options.length);
		}

		this.setText = function (text)
		{
			this._text = text;
		}

		this.getText = function ()
		{
			return (this.getSelectedText ());
		}

		this.setReadOnly = function (isReadOnly)
		{
			this._isReadOnly = isReadOnly;

			this._divWindow.disabled = this._isReadOnly;
		}

		this.getReadOnly = function ()
		{
			return (this._isReadOnly);
		}

		this.getSelectedIndex = function (subIndex)
		{
			if (this._divWindow.selectedIndex < 0)
				throw new Error (RadXmlLanguage.MSG42);

			if (this._divWindow.selectedIndex >= this._divWindow.options.length)
				throw new Error (RadXmlLanguage.MSG43);

			return (this._divWindow.selectedIndex);
		}

		this.selectItem = function (index)
		{
			this._divWindow.selectedIndex = index;
		}

		this.getSelectedText = function (subIndex)
		{
			if (this._divWindow.selectedIndex < 0)
				throw new Error (RadXmlLanguage.MSG42);

			if (this._divWindow.selectedIndex >= this._divWindow.options.length)
				throw new Error (RadXmlLanguage.MSG43);

			return (this._divWindow.options[this._divWindow.selectedIndex].innerHTML);
		}

		this.clearItems = function ()
		{
			for (var iIdx = this._divWindow.options.length; iIdx >= 0; iIdx--)
				this._divWindow.options.remove (iIdx);
		}

		this.displayContent = function ()
		{
		}
	}

	HTMLComboBox.prototype = new HTMLAppObject ();

	function HTMLGroupBox (renderSystem)
	{
		this._divRelativeWindow = null;
		this._divWindow = null;

		this._renderSystem = renderSystem;

		this.create = function (newId, text)
		{
			this._name = newId;
			this._text = text;

			if (this._parent != null)
			{
				/*this._divRelativeWindow = document.createElement ("div");
				this._divRelativeWindow.id = "divImageBoxRelative-" + this._name;
				this._divRelativeWindow.wWindow = this;
				this._divRelativeWindow.style.position = "relative";
				this._divRelativeWindow.style.width = this._width + "px";
				this._divRelativeWindow.style.height = this._height + "px";
				this._divRelativeWindow.style.display = "block";*/

				this._divWindow = document.createElement ("div");
				this._divWindow.id = "divGroupBox-" + this._name;
				this._divWindow.name = "divGroupBox-" + this._name;
				this._divWindow.setAttribute ("class", "divGroupBox");
				this._divWindow.setAttribute ("className", "divGroupBox");
				this._divWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_DOWN);");
				this._divWindow.setAttribute ("onselectstart", "return (false);");
				this._divWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK);");
				this._divWindow.wWindow = this;
				this._divWindow.style.position = "absolute";
				this._divWindow.style.left = this._x + "px";
				this._divWindow.style.top = this._y + "px";
				this._divWindow.style.width = this._width + "px";
				this._divWindow.style.height = this._height + "px";
				//this._divWindow.style.zIndex = this._renderSystem._iTopWindow++;
				this._divWindow.style.display = "block";
				this._divWindow.innerHTML = getHTMLText (this._text);
				this._content = this._divWindow;

				//this._divRelativeWindow.appendChild (this._divWindow);
				this._parent._content.appendChild (this._divWindow);
			}
		}

		this.receivedEvent = function (event, type)
		{
		}
	}

	HTMLGroupBox.prototype = new HTMLAppObject ();

	function HTMLSelectList (renderSystem)
	{
		this._divRelativeWindow = null;
		this._divWindow = null;

		this._renderSystem = renderSystem;

		this.create = function (newId, newText)
		{
			this._name = newId;
			this._text = newText;

			if (this._parent != null)
			{
				/*this._divRelativeWindow = document.createElement ("div");
				this._divRelativeWindow.id = "divButtonRelative-" + this._name;
				this._divRelativeWindow.wWindow = this;
				this._divRelativeWindow.style.position = "relative";
				this._divRelativeWindow.style.width = this._width + "px";
				this._divRelativeWindow.style.height = this._height + "px";
				this._divRelativeWindow.style.display = "block";*/

				this._divWindow = document.createElement ("select");
				this._divWindow.id = "divSelectList-" + this._name;
				this._divWindow.name = "divSelectList-" + this._name;
				this._divWindow.setAttribute ("class", "divSelectList");
				this._divWindow.setAttribute ("className", "divSelectList");
				this._divWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_DOWN);");
				this._divWindow.setAttribute ("onselectstart", "return (false);");
				this._divWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK);");
				this._divWindow.wWindow = this;
				this._divWindow.multiple = true;
				this._divWindow.style.position = "absolute";
				this._divWindow.style.left = this._x + "px";
				this._divWindow.style.top = this._y + "px";
				this._divWindow.style.width = "20px";
				this._divWindow.style.height = "20px";
				//this._divWindow.style.zIndex = this._renderSystem._iTopWindow++;
				this._divWindow.style.display = "block";

				//this._divRelativeWindow.appendChild (this._divWindow);
				this._parent._content.appendChild (this._divWindow);
			}
		}

		this.receivedEvent = function (event, type)
		{
			if (this._eventFunction != null)
				this._eventFunction (type);
		}
	}

	HTMLSelectList.prototype = new HTMLAppObject ();

	function HTMLRadioButton (renderSystem)
	{
		this._divRelativeWindow = null;
		this._divWindow = null;
		this._divLabel = null;
		this._isReadOnly = false;
		this._isSelected = false;
		this._radioGroup = "";

		this._renderSystem = renderSystem;

		this.create = function (newId, newText, newGroup, isSelected)
		{
			this._name = newId;
			this._text = newText;
			this._radioGroup = newGroup;
			this._isSelected = isSelected;

			if (this._parent != null)
			{
				/*this._divRelativeWindow = document.createElement ("div");
				this._divRelativeWindow.id = "divButtonRelative-" + this._name;
				this._divRelativeWindow.wWindow = this;
				this._divRelativeWindow.style.position = "relative";
				this._divRelativeWindow.style.width = this._width + "px";
				this._divRelativeWindow.style.height = this._height + "px";
				this._divRelativeWindow.style.display = "block";*/

				this._divWindow = document.createElement ("input");
				this._divWindow.type = "radio";
				this._divWindow.id = "divRadioButton-" + this._name;
				this._divWindow.name = this._radioGroup;
				this._divWindow.setAttribute ("class", "divRadioButton");
				this._divWindow.setAttribute ("className", "divRadioButton");
				this._divWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_DOWN);");
				this._divWindow.setAttribute ("onselectstart", "return (false);");
				this._divWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK);");
				this._divWindow.value = this._text;
				this._divWindow.wWindow = this;
				this._divWindow.style.position = "absolute";
				this._divWindow.style.left = this._x + "px";
				this._divWindow.style.top = this._y + "px";
				this._divWindow.style.width = "20px";
				this._divWindow.style.height = "20px";
				//this._divWindow.style.zIndex = this._renderSystem._iTopWindow++;
				this._divWindow.style.display = "block";
				this._divWindow.checked = this._isSelected;

				this._divLabel = document.createElement ("label");
				this._divLabel.id = "divRadioButtonLabel-" + this._name;
				this._divLabel.name = "divRadioButtonLabel-" + this._name;
				this._divLabel.setAttribute ("class", "divRadioButtonLabel");
				this._divLabel.setAttribute ("className", "divRadioButtonLabel");
				this._divLabel.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_DOWN);");
				this._divLabel.setAttribute ("onselectstart", "return (false);");
				this._divLabel.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK);");
				this._divLabel.wWindow = this;
				this._divLabel.style.position = "absolute";
				this._divLabel.style.left = (this._x + (this._width * 0.55)) + "px";
				this._divLabel.style.top = this._y + "px";
				this._divLabel.style.width = this._width + "px";
				this._divLabel.style.height = this._height + "px";
				//this._divLabel.style.zIndex = this._renderSystem._iTopWindow++;
				this._divLabel.style.display = "block";
				this._divLabel.innerHTML = getHTMLText (this._text);
				this._divLabel.htmlFor = this._divWindow.id;

				//this._divRelativeWindow.appendChild (this._divWindow);
				this._parent._content.appendChild (this._divWindow);
				this._parent._content.appendChild (this._divLabel);
			}
		}

		this.receivedEvent = function (event, type)
		{
			if (this._eventFunction != null)
				this._eventFunction (type);
		}

		this.setPosition = function (x, y)
		{
			this._x = x;
			this._y = y;

			this._divWindow.style.left = x + "px";
			this._divWindow.style.top = y + "px";

			this._divLabel.style.left = (x + 25) + "px";
			this._divLabel.style.top = (y + 2) + "px";
		}

		this.setSize = function (newWidth, newHeight)
		{
			this._width = newWidth;
			this._height = newHeight;

			this._divWindow.style.width = "20px";
			this._divWindow.style.height = "20px";

			this._divLabel.style.width = this._width + "px";
			this._divLabel.style.height = this._height + "px";
		}

		this.setReadOnly = function (isReadOnly)
		{
			this._isReadOnly = isReadOnly;

			this._divWindow.disabled = this._isReadOnly;
		}

		this.getReadOnly = function ()
		{
			return (this._isReadOnly);
		}

		this.setSelected = function (isSelected)
		{
			this._isSelected = isSelected;

			this._divWindow.checked = this._isSelected;
		}

		this.setChecked = function (isChecked)
		{
			this._isSelected = isChecked;
			this._divWindow.checked = isChecked;
		}

		this.getChecked = function ()
		{
			return (this._divWindow.checked);
		}
	}

	HTMLRadioButton.prototype = new HTMLAppObject ();

	function HTMLSelectListGrid (renderSystem)
	{
		this._divRelativeWindow = null;
		this._divWindow = null;
		this._divLabel = null;
		this._isReadOnly = false;
		this._headers = new Array ();
		this._items = new Array ();
		this._headerContent = "";
		this._innerContent = "";
		this._selectedIndex = -1;
		this._selectedSubIndex = -1;

		this._renderSystem = renderSystem;

		this.create = function (newId, newText, newHeaders, newItems)
		{
			this._name = newId;
			this._text = newText;
			this._headers = newHeaders;
			this._items = newItems;

			if (this._parent != null)
			{
				/*this._divRelativeWindow = document.createElement ("div");
				this._divRelativeWindow.id = "divButtonRelative-" + this._name;
				this._divRelativeWindow.wWindow = this;
				this._divRelativeWindow.style.position = "relative";
				this._divRelativeWindow.style.width = this._width + "px";
				this._divRelativeWindow.style.height = this._height + "px";
				this._divRelativeWindow.style.display = "block";*/

				/*this._divLabel = document.createElement ("div");
				this._divLabel.id = "divSelectListGridHeader-" + this._name;
				this._divLabel.name = "divSelectListGridHeader-" + this._name;
				this._divLabel.setAttribute ("class", "divSelectListGridHeader");
				this._divLabel.setAttribute ("className", "divSelectListGridHeader");
				this._divLabel.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_DOWN);");
				this._divLabel.setAttribute ("onselectstart", "return (false);");
				this._divLabel.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK);");
				this._divLabel.wWindow = this;
				this._divLabel.style.position = "absolute";
				this._divLabel.style.left = this._x + "px";
				this._divLabel.style.top = this._y + "px";
				this._divLabel.style.width = this._width + "px";
				this._divLabel.style.height = "30px";
				//this._divLabel.style.zIndex = this._renderSystem._iTopWindow++;
				this._divLabel.style.display = "block";

				this._parent._content.appendChild (this._divLabel);
				var objSize = _getObjectSize (this._divLabel.id);

				this._divWindow = document.createElement ("iframe");
				this._divWindow.id = "divSelectListGrid-" + this._name;
				this._divWindow.name = "divSelectListGrid-" + this._name;
				this._divWindow.setAttribute ("class", "divSelectListGrid");
				this._divWindow.setAttribute ("className", "divSelectListGrid");
				this._divWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_DOWN);");
				this._divWindow.setAttribute ("onselectstart", "return (false);");
				this._divWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK);");
				this._divWindow.wWindow = this;
				this._divWindow.style.position = "absolute";
				this._divWindow.style.left = this._x + "px";
				this._divWindow.style.top = ((this._y + objSize[1]) + "px");
				this._divWindow.style.width = this._width + "px";
				this._divWindow.style.height = ((this._height - objSize[1]) + "px");
				//this._divWindow.style.zIndex = this._renderSystem._iTopWindow++;
				this._divWindow.style.display = "block";

				this.displayContent ();

				//this._divRelativeWindow.appendChild (this._divWindow);
				this._parent._content.appendChild (this._divWindow);*/

				this._divWindow = document.createElement ("div");
				this._divWindow.id = "divSelectListGrid-" + this._name;
				this._divWindow.name = "divSelectListGrid-" + this._name;
				this._divWindow.setAttribute ("class", "divSelectListGrid");
				this._divWindow.setAttribute ("className", "divSelectListGrid");
				this._divWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_DOWN);");
				this._divWindow.setAttribute ("onselectstart", "return (false);");
				this._divWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLAppRenderSystemEvents.RSE_MOUSE_CLICK);");
				this._divWindow.wWindow = this;
				this._divWindow.style.position = "absolute";
				this._divWindow.style.overflow = "auto";
				this._divWindow.style.left = this._x + "px";
				this._divWindow.style.top = (this._y + "px");
				this._divWindow.style.width = this._width + "px";
				this._divWindow.style.height = (this._height + "px");
				//this._divWindow.style.zIndex = this._renderSystem._iTopWindow++;
				this._divWindow.style.display = "block";

				this._parent._content.appendChild (this._divWindow);
			}
		}

		this.setPosition = function (x, y)
		{
			this._x = x;
			this._y = y;

			/*var objSize = _getObjectSize (this._divLabel.id);

			this._divLabel.style.left = this._x + "px";
			this._divLabel.style.top = this._y + "px";

			this._divWindow.style.left = this._x + "px";
			this._divWindow.style.top = ((this._y + objSize[1]) + "px");
			this._divWindow.style.width = this._width + "px";
			this._divWindow.style.height = ((this._height - objSize[1]) + "px");*/

			this._divWindow.style.left = this._x + "px";
			this._divWindow.style.top = this._y + "px";
		}

		this.setSize = function (newWidth, newHeight)
		{
			this._width = newWidth;
			this._height = newHeight;

			/*var objSize = _getObjectSize (this._divLabel.id);

			this._divWindow.style.width = this._width + "px";
			this._divWindow.style.height = ((this._height - objSize[1]) + "px");*/
			this._divWindow.style.width = this._width + "px";
			this._divWindow.style.height = this._height + "px";
		}

		this.displayContent = function ()
		{
			this._innerContent = "";
			/*this._innerContent += "<html>\n";
			this._innerContent += "<head>\n";
			this._innerContent += "\t<script type = \"text/javascript\" src = \"./RadXml/RadXmlHTMLAppRenderSystemSelectListGridItem.js\"></script>\n";
			this._innerContent += "\t<link rel = \"stylesheet\" type = \"text/css\" href = \"./main.css\">\n";
			this._innerContent += "</head>\n";
			this._innerContent += "<body class = \"bodySelectListGrid\">\n";*/
			this._innerContent += "<table border = \"0\" class = \"tblSelectListGrid\">\n";
			this._innerContent += "<tr class = \"tblSelectListGridHeaderRow\">\n";

			for (var iIdx = 0; iIdx < this._headers.length; iIdx++)
			{
				var strItem = this._headers[iIdx];

				this._innerContent += "<td class = \"tblSelectListGridHeaderCell\">\n";
				this._innerContent += strItem + "\n";
				this._innerContent += "</td>\n";
			}

			this._innerContent += "</tr>\n";

			for (var iIdx = 0; iIdx < this._items.length; iIdx++)
			{
				var objItem = this._items[iIdx];

				this._innerContent += "<tr class = \"tblSelectListGridItemRow\">\n";
				var aryArray = null;

				if (objItem instanceof Array)
					aryArray = objItem;

				if (objItem instanceof SelectListGridItem)
					aryArray = objItem._items;

				if (aryArray != null)
				{
					var iCount = 1;

					if (aryArray.length != this._headers.length)
						iCount = (this._headers.length - aryArray.length);

					for (var iJdx = 0; iJdx < aryArray.length; iJdx++)
					{
						var strItem = aryArray[iJdx];

						//this._innerContent += "<td id = \"tblSelectListGridItemCell-" + this._name + "-" + iIdx + "-" + iJdx + "\" class = \"tblSelectListGridItemCell\" onclick = \"onClick (this.id);\" colspan = \"" + iCount + "\">\n";
						this._innerContent += "<td id = \"tblSelectListGridItemCell-" + this._name + "-" + iIdx + "-" + iJdx + "\" class = \"tblSelectListGridItemCell\" onclick = \"RadXML._appEngine._renderSystem.onSelectListGridCellClick (this.id);\" colspan = \"" + iCount + "\">\n";
						this._innerContent += strItem + "\n";
						this._innerContent += "</td>\n";
					}
				}
				else
				{
					//this._innerContent += "<td id = \"tblSelectListGridItemCell-" + this._name + "-" + iIdx + "-0\" class = \"tblSelectListGridItemCell\" onclick = \"onClick (this.id);\" colspan = \"" + this._headers.length + "\">\n";
					this._innerContent += "<td id = \"tblSelectListGridItemCell-" + this._name + "-" + iIdx + "-0\" class = \"tblSelectListGridItemCell\" onclick = \"RadXML._appEngine._renderSystem.onSelectListGridCellClick (this.id);\" colspan = \"" + this._headers.length + "\">\n";
					this._innerContent += objItem + "\n";
					this._innerContent += "</td>\n";
				}

				this._innerContent += "</tr>\n";
			}

			this._innerContent += "</table>\n";
			/*this._innerContent += "</body>\n";
			this._innerContent += "</html>\n";*/

			/*this._headerContent = "<table border = \"0\" class = \"tblSelectListGrid\">\n";
			this._headerContent += "<tr class = \"tblSelectListGridHeaderRow\">\n";

			for (var iIdx = 0; iIdx < this._headers.length; iIdx++)
			{
				var strItem = this._headers[iIdx];

				this._headerContent += "<td class = \"tblSelectListGridHeaderCell\">\n";
				this._headerContent += strItem + "\n";
				this._headerContent += "</td>\n";
			}

			this._headerContent += "</tr>\n";
			this._headerContent += "</table>\n";*/
			this._divWindow.innerHTML = this._innerContent;
			/*this._divLabel.innerHTML = this._headerContent;

			if (this._divWindow.contentWindow == null)
				this._divWindow.onload = RadXML._keepContext (this.iFrameReady, this);
			else
			{
				this._divWindow.contentWindow.document.open ();
				this._divWindow.contentWindow.document.write (this._innerContent);
				this._divWindow.contentWindow.document.close ();
			}*/
		}

		this.iFrameReady = function ()
		{
			this._divWindow.contentWindow.document.open ();
			this._divWindow.contentWindow.document.write (this._innerContent);
			this._divWindow.contentWindow.document.close ();
		}

		this.receivedEvent = function (event, type, selectedIndex, selectedSubIndex)
		{
			if (this._eventFunction != null)
				this._eventFunction (type, selectedIndex, selectedSubIndex);
		}

		this.setHeaders = function (newArray)
		{
			this._headers = newArray;

			this.displayContent ();
		}

		this.setItems = function (newArray)
		{
			this._items = newArray;

			this.displayContent ();
		}

		this.addHeaderItem = function (item)
		{
			this._headers.push (item);

			this.displayContent ();
		}

		this.addItem = function (item)
		{
			this._items.push (item);

			this.displayContent ();
		}

		this.setReadOnly = function (isReadOnly)
		{
			this._isReadOnly = isReadOnly;
		}

		this.getReadOnly = function ()
		{
			return (this._isReadOnly);
		}

		this.onClick = function (itemIndex, subItemIndex)
		{
			if (this._isReadOnly == true)
				return;

			var objItem = this._items[itemIndex];

			if (objItem instanceof Array)
				aryArray = objItem;

			if (objItem instanceof SelectListGridItem)
				aryArray = objItem._items;

			this.resetCells ();

			if (aryArray != null)
			{
				for (var iJdx = 0; iJdx < aryArray.length; iJdx++)
				{
					/*var obj = this._divWindow.contentWindow.
						document.getElementById ("tblSelectListGridItemCell-" + 
							this._name + "-" + itemIndex + "-" + iJdx);*/
					var obj = document.getElementById ("tblSelectListGridItemCell-" + 
							this._name + "-" + itemIndex + "-" + iJdx);

					obj.setAttribute ("class", "tblSelectListGridItemCellSelected");
					obj.setAttribute ("className", "tblSelectListGridItemCellSelected");
				}
			}
			else
			{
				/*var obj = this._divWindow.contentWindow.
					document.getElementById ("tblSelectListGridItemCell-" + 
						this._name + "-" + itemIndex + "-0");*/
				var obj = document.getElementById ("tblSelectListGridItemCell-" + 
						this._name + "-" + itemIndex + "-0");

				obj.setAttribute ("class", "tblSelectListGridItemCellSelected");
				obj.setAttribute ("className", "tblSelectListGridItemCellSelected");
			}

			this._selectedIndex = itemIndex;
			this._selectedSubIndex = subItemIndex;
			this.receivedEvent (null, HTMLAppRenderSystemEvents.RSE_SELECT_LIST_GRID_ITEM_CLICKED, 
									itemIndex, subItemIndex);
		}

		this.resetCells = function (itemIndex)
		{
			for (var iIdx = 0; iIdx < this._items.length; iIdx++)
			{
				var objItem = this._items[iIdx];

				if (objItem instanceof Array)
					aryArray = objItem;

				if (objItem instanceof SelectListGridItem)
					aryArray = objItem._items;

				if (aryArray != null)
				{
					for (var iJdx = 0; iJdx < aryArray.length; iJdx++)
					{
						/*var obj = this._divWindow.contentWindow.
							document.getElementById ("tblSelectListGridItemCell-" + 
								this._name + "-" + iIdx + "-" + iJdx);*/
						var obj = document.getElementById ("tblSelectListGridItemCell-" + 
								this._name + "-" + iIdx + "-" + iJdx);

						obj.setAttribute ("class", "tblSelectListGridItemCell");
						obj.setAttribute ("className", "tblSelectListGridItemCell");
					}
				}
				else
				{
					/*var obj = this._divWindow.contentWindow.
						document.getElementById ("tblSelectListGridItemCell-" + 
							this._name + "-" + iIdx + "-0");*/
					var obj = document.getElementById ("tblSelectListGridItemCell-" + 
							this._name + "-" + iIdx + "-0");

					obj.setAttribute ("class", "tblSelectListGridItemCell");
					obj.setAttribute ("className", "tblSelectListGridItemCell");
				}
			}
		}

		this.getSelectedIndex = function ()
		{
			return (this._selectedIndex);
		}

		this.getSelectedText = function (subIndex)
		{
			if (subIndex == null)
				subIndex = this._selectedSubIndex;

			var objItem = this._items[this._selectedIndex];
			var aryArray = null;
			var strReturn = "";

			if (objItem instanceof Array)
				aryArray = objItem;

			if (objItem instanceof SelectListGridItem)
				aryArray = objItem._items;

			if (aryArray != null)
				strReturn = aryArray[subIndex];
			else
				strReturn = objItem;

			return (strReturn);
		}
	}

	this.onSelectListGridCellClick = function (cellId)
	{
		var arySplit = cellId.split ("-");
		var strName = arySplit[1];
		var iIndex = parseInt (arySplit[2]);
		var iSubIndex = parseInt (arySplit[3]);
		var obj = document.getElementById ("divSelectListGrid-" + strName);
		obj.wWindow.onClick (iIndex, iSubIndex);
	}

	HTMLSelectListGrid.prototype = new HTMLAppObject ();

	this._getRelativeMousePos = function ()
	{
		var aryRelative = [(RadXML._appEngine._renderSystem._aryMousePos[0] - 
							RadXML._appEngine._renderSystem._aryPrevMousePos[0]), 
						(RadXML._appEngine._renderSystem._aryMousePos[1] - 
							RadXML._appEngine._renderSystem._aryPrevMousePos[1])];

		return (aryRelative);
	}

	this._getMousePosEvent = function (event)
	{
		RadXML._appEngine._renderSystem._aryPrevMousePos = RadXML._appEngine._renderSystem._aryMousePos;
		RadXML._appEngine._renderSystem._aryMousePos = [event.clientX, event.clientY];
	}

	this._onMouseDown = function (event)
	{
		RadXML._appEngine._renderSystem._mouseButtonDown = 1;
	}

	this._onMouseUp = function (event)
	{
		RadXML._appEngine._renderSystem._mouseButtonDown = 0;
	}

	this.minWindow = function (event, name)
	{
	}

	this.maxWindow = function (event, name)
	{
	}

	this.moveWindow = function (event, name)
	{
		if (event.preventDefault)
			event.preventDefault ();

		if (event.defaultPrevented)
			event.defaultPrevented = true;

		if (RadXML._appEngine._renderSystem._mouseButtonDown == 1)
		{
			var mPos = RadXML._appEngine._renderSystem._getRelativeMousePos ();
			var objWindow = document.getElementById ("divWindow-" + name);
			var wWindow = objWindow.wWindow;

			if (wWindow._maximized == false)
			{
				wWindow._x += mPos[0];
				wWindow._y += mPos[1];

				wWindow.setPosition (wWindow._x, wWindow._y);
			}
		}
	}

	this.closeWindow = function (event, name)
	{
		var obj = event.target.parentNode.parentNode.parentNode.parentNode;
		var objHTMLObject = obj.wWindow;

		objHTMLObject._eventFunction (HTMLAppRenderSystemEvents.RSE_CLOSED);
	}

	this.getRandomId = function (checkFunction)
	{
		var iRandomId = parseInt (Math.random () * 10000);

		while (checkFunction (iRandomId) != null)
			iRandomId = parseInt (Math.random () * 10000);

		return (iRandomId);
	}

	this.getWindow = function (strId)
	{
		for (var iIdx = 0; iIdx < this.aryWindows.length; iIdx++)
		{
			if (strId == this.aryWindows[iIdx].strId)
				return (this.aryWindows[iIdx]);
		}

		return (null);
	}

	this._isRelativeSize = function (size)
	{
		if (((size[0] != 0) && (size[0] < 1)) || ((size[0] != 0) && (size[1] < 1)))
			return (true);

		return (false);
	}

	this._getPixelSizeFromSize = function (parentSize, size)
	{
		var v2dReturn = [0, 0];

		if (size == null)
		{
			if (this._isRelativeSize (parentSize) == true)
				v2dReturn = this._getPixelSizeFromRelativeSize (parentSize, size);
			else
				v2dReturn = parentSize;
		}
		else
		{
			if (this._isRelativeSize (size) == true)
				v2dReturn = this._getPixelSizeFromRelativeSize (parentSize, size);
			else
				v2dReturn = size;
		}

		return (v2dReturn);
	}

	this._getPixelSizeFromRelativeSize = function (parentSize, size)
	{
		var v2dReturn = [0, 0];

		if (size == null)
		{
			var v2dMonitorSize = [this._parentWidth, this._parentHeight];

			v2dReturn[0] = v2dMonitorSize[0] * parentSize[0];
			v2dReturn[1] = v2dMonitorSize[1] * parentSize[1];
		}
		else
		{
			v2dReturn[0] = parentSize[0] * size[0];
			v2dReturn[1] = parentSize[1] * size[1];
		}

		return (v2dReturn);
	}

	this.setupInputs = function (parentObject)
	{
		if (document.addEventListener)
			document.addEventListener ("mousedown", parentObject._onMouseDown, false);
		else
			document.attachEvent ("mousedown", parentObject._onMouseDown);

		if (document.addEventListener)
			document.addEventListener ("mouseup", parentObject._onMouseUp, false);
		else
			document.attachEvent ("mouseup", parentObject._onMouseUp);

		if (document.addEventListener)
			document.addEventListener ("mousemove", parentObject._getMousePosEvent, false);
		else
			document.attachEvent ("mousemove", parentObject._getMousePosEvent);
	}

	this.getAbsoluteSize = function (width, height, parent)
	{
		var size = [0, 0];

		if (parent != null)
			size = this._getPixelSizeFromSize ([parent._width, parent._height], [width, height]);
		else
			size = this._getPixelSizeFromSize ([width, height]);

		return (size);
	}

	this.createWindow = function (name, text, width, height, parent, blend)
	{
		var wWindow = new HTMLWindow (this);
		var size = this.getAbsoluteSize (width, height, parent);

		wWindow._width = size[0];
		wWindow._height = size[1];
		wWindow._blend = blend;
		wWindow.create (name, text);

		this._aryWindows.push (wWindow);

		return (wWindow);
	}

	this.createLabel = function (name, text, width, height, parent)
	{
		var wWindow = new HTMLLabel (this);
		var size = this.getAbsoluteSize (width, height, parent);

		wWindow._width = size[0];
		wWindow._height = size[1];
		wWindow._parent = parent;
		wWindow.create (name, text);

		this._aryWindows.push (wWindow);

		return (wWindow);
	}

	this.createButton = function (name, text, width, height, parent, eventFunction)
	{
		var wWindow = new HTMLButton (this);
		var size = this.getAbsoluteSize (width, height, parent);

		wWindow._width = size[0];
		wWindow._height = size[1];
		wWindow._parent = parent;
		wWindow.create (name, text);

		this._aryWindows.push (wWindow);

		return (wWindow);
	}

	this.createTextBox = function (name, text, width, height, parent, isMultiLine)
	{
		if (isMultiLine == null)
			isMultiLine = false;

		var wWindow = new HTMLTextBox (this);
		var size = this.getAbsoluteSize (width, height, parent);

		wWindow._width = size[0];
		wWindow._height = size[1];
		wWindow._parent = parent;
		wWindow.create (name, text, isMultiLine);

		this._aryWindows.push (wWindow);

		return (wWindow);
	}

	this.createImageBox = function (name, src, width, height, parent)
	{
		var wWindow = new HTMLImageBox (this);
		var size = this.getAbsoluteSize (width, height, parent);

		wWindow._width = size[0];
		wWindow._height = size[1];
		wWindow._parent = parent;
		wWindow.create (name, src);

		this._aryWindows.push (wWindow);

		return (wWindow);
	}

	this.createCheckBox = function (name, text, width, height, parent)
	{
		var wWindow = new HTMLCheckBox (this);
		var size = this.getAbsoluteSize (width, height, parent);

		wWindow._width = size[0];
		wWindow._height = size[1];
		wWindow._parent = parent;
		wWindow.create (name, text);

		this._aryWindows.push (wWindow);

		return (wWindow);
	}

	this.createComboBox = function (name, text, width, height, parent)
	{
		var wWindow = new HTMLComboBox (this);
		var size = this.getAbsoluteSize (width, height, parent);

		wWindow._width = size[0];
		wWindow._height = size[1];
		wWindow._parent = parent;
		wWindow.create (name, text);

		this._aryWindows.push (wWindow);

		return (wWindow);
	}

	this.createGroupBox = function (name, text, width, height, parent)
	{
		var wWindow = new HTMLGroupBox (this);
		var size = this.getAbsoluteSize (width, height, parent);

		wWindow._width = size[0];
		wWindow._height = size[1];
		wWindow._parent = parent;
		wWindow.create (name, text);

		this._aryWindows.push (wWindow);

		return (wWindow);
	}

	this.createSelectList = function (name, text, width, height, parent)
	{
		var wWindow = new HTMLSelectList (this);
		var size = this.getAbsoluteSize (width, height, parent);

		wWindow._width = size[0];
		wWindow._height = size[1];
		wWindow._parent = parent;
		wWindow.create (name, text);

		this._aryWindows.push (wWindow);

		return (wWindow);
	}

	this.createRadioButton = function (name, text, width, height, parent, radioGroup, isSelected)
	{
		var wWindow = new HTMLRadioButton (this);
		var size = this.getAbsoluteSize (width, height, parent);

		wWindow._width = size[0];
		wWindow._height = size[1];
		wWindow._parent = parent;
		wWindow.create (name, text, radioGroup, isSelected);

		this._aryWindows.push (wWindow);

		return (wWindow);
	}

	this.createSelectListGrid = function (name, text, width, height, parent, headers, items)
	{
		var wWindow = new HTMLSelectListGrid (this);
		var size = this.getAbsoluteSize (width, height, parent);

		wWindow._width = size[0];
		wWindow._height = size[1];
		wWindow._parent = parent;
		wWindow.create (name, text, headers, items);

		this._aryWindows.push (wWindow);

		return (wWindow);
	}

	this.recreateSelectListGrid = function (parent, item)
	{
		parent.displayContent ();
	}
}

