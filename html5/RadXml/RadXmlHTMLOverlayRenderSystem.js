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
var HTMLOverlayRenderSystemEvents = 
{
	ORSE_MOUSE_DOWN:						1, 
	ORSE_MOUSE_CLICK:						2, 
	ORSE_MOUSE_UP:							3
};

function getHTMLText (text)
{
	var strText = text.replace (/ /g, "&nbsp;");
	strText = strText.replace (/\\n/g, "<br />");
	strText = strText.replace (/(\n)/g, "<br />");

	return (strText);
}

function HTMLOverlayRenderSystem ()
{
	this._mouseButtonDown = 0;

	this._aryMousePos = [0, 0];
	this._aryPrevMousePos = [0, 0];

	this._parentWidth = RadXML._screenWidth;
	this._parentHeight = RadXML._screenHeight;

	this._overlays = new Object ();

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

	this._getObjectSize = function (strObjectId)
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

	this.getRandomId = function (checkFunction)
	{
		var iRandomId = parseInt (Math.random () * 10000);

		while (checkFunction (iRandomId) != null)
			iRandomId = parseInt (Math.random () * 10000);

		return (iRandomId);
	}

	this._isRelativeSize = function (size)
	{
		if ((size[0] < 1) || (size[1] < 1))
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

	this.getAbsoluteSize = function (width, height, parent)
	{
		var size = [0, 0];

		if (parent != null)
			size = this._getPixelSizeFromSize ([parent._width, parent._height], [width, height]);
		else
			size = this._getPixelSizeFromSize ([width, height]);

		return (size);
	}

	this.createOverlay = function (name, children)
	{
		var obj = new HTMLOverlay (this);

		obj.create (name, children);
		this._overlays[name] = obj;

		return (obj);
	}
}

function HTMLOverlayObject ()
{
	this._name = "";
	this._parent = null;
	this._children = new Object ();
	this._text = "";
	this._left = 0;
	this._top = 0;
	this._leftPx = 0;
	this._topPx = 0;
	this._width = 500;
	this._height = 500;
	this._prevWidth = 500;
	this._prevHeight = 400;
	this._visible = true;
	this._eventFunction = null;

	this._font = "Arial";
	this._fontSize = "14px";
	this._fontColor = "#FFFFFF";

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

	this.setPosition = function (left, top)
	{
		this._left = left;
		this._top = top;

		this._divWindow.style.left = left + "px";
		this._divWindow.style.top = top + "px";
	}

	this.getPosition = function ()
	{
		var aryPos = [this._divWindow.offsetLeft, this._divWindow.offsetTop];

		return (aryPos);
	}

	this.getText = function ()
	{
		return (this._text);
	}
}

function HTMLOverlay (renderSystem)
{
	this._name = "";
	this._children = new Object ();

	this._renderSystem = renderSystem;

	this._divWindow = null;

	this.create = function (name, children)
	{
		this._name = name;
		this._children = children;

		var v2dPos = RadXML.getCanvasPosition ();
		var v2dSize = RadXML.getCanvasSize ();

		this._leftPx = v2dPos.x;
		this._topPx = v2dPos.y;
		this._width = v2dSize.x;
		this._height = v2dSize.y;

		this._divWindow = document.createElement ("div");
		this._divWindow.id = this._name;
		this._divWindow.wWindow = this;
		this._divWindow.style.cssText = "pointer-events: none;";
		this._divWindow.style.position = "absolute";
		this._divWindow.style.left = v2dPos.x + "px";
		this._divWindow.style.top = v2dPos.y + "px";
		this._divWindow.style.width = v2dSize.x + "px";
		this._divWindow.style.height = v2dSize.y + "px";
		this._divWindow.style.display = "block";

		document.body.appendChild (this._divWindow);

		for (var key in this._children)
		{
			var obj = this._children[key];
			obj.create (this._renderSystem, this, obj._name, obj._children);
		}
	}

	this.setVisible = function (visible)
	{
		this._visible = visible;

		if (this._visible == true)
			this._divWindow.style.display = "block";
		else
			this._divWindow.style.display = "none";

		for (var key in this._children)
		{
			var obj = this._children[key];
			obj.setVisible (visible);
		}
	}
}

function HTMLOverlayElement ()
{
	this._name = "";
	this._parent = null;
	this._children = new Object ();
	this._captureEvents = false;

	this._renderSystem = null;

	this._divWindow = null;
	this._spnDivWindow = null;
	this._spnWindow = null;
	this._imgDivWindow = null;
	this._imgWindow = null;

	this.create = function (renderSystem, parent, name, children)
	{
		this._renderSystem = renderSystem;
		this._name = name;
		this._parent = parent;
		this._children = children;
		//var pos = this._renderSystem._getPixelSizeFromRelativeSize ([this._left, this._top]);
		//var size = this._renderSystem._getPixelSizeFromRelativeSize ([this._width, this._height]);
		//var pos = [this._left * 100, this._top * 100];
		//var size = [this._width * 100, this._height * 100];
		var pos = [(this._parent._width * this._left), 
					(this._parent._height * this._top)];
		var size = [renderSystem._parentWidth * this._width, renderSystem._parentHeight * this._height];
		var checkPos = [this._parent._leftPx + pos[0], this._parent._topPx + pos[1]];

		if (checkPos[0] > renderSystem._parentWidth)
			checkPos[0] = renderSystem._parentWidth;

		if (checkPos[1] > renderSystem._parentHeight)
			checkPos[1] = renderSystem._parentHeight;

		if ((checkPos[0] + size[0]) > renderSystem._parentWidth)
			size[0] -= (renderSystem._parentWidth - (checkPos[0] + size[0]));

		if ((checkPos[1] + size[1]) > renderSystem._parentHeight)
			size[1] -= (renderSystem._parentHeight - (checkPos[1] + size[1]));

		this._leftPx = pos[0];
		this._topPx = pos[1];
		this._width = size[0];
		this._height = size[1];

		this._divWindow = document.createElement ("div");
		this._divWindow.id = this._name;
		this._divWindow.wWindow = this;
		this._divWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLOverlayRenderSystemEvents.ORSE_MOUSE_DOWN);");
		this._divWindow.setAttribute ("onmouseup", "this.wWindow.receivedEvent (event, HTMLOverlayRenderSystemEvents.ORSE_MOUSE_UP);");
		this._divWindow.setAttribute ("onselectstart", "return (false);");
		this._divWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLOverlayRenderSystemEvents.ORSE_MOUSE_CLICK);");
		this._divWindow.style.position = "absolute";
		/*this._divWindow.style.left = pos[0] + "px";
		this._divWindow.style.top = pos[1] + "px";
		this._divWindow.style.width = this._width + "px";
		this._divWindow.style.height = this._height + "px";*/
		this._divWindow.style.left = pos[0] + "px";
		this._divWindow.style.top = pos[1] + "px";
		this._divWindow.style.width = size[0] + "px";
		this._divWindow.style.height = size[1] + "px";
		this._divWindow.style.display = "block";

		if (this._text != "")
		{
			this._spnDivWindow = document.createElement ("div");
			this._spnDivWindow.id = "subDiv" + this._name;
			this._spnDivWindow.wWindow = this;
			this._spnDivWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLOverlayRenderSystemEvents.ORSE_MOUSE_DOWN);");
			this._spnDivWindow.setAttribute ("onmouseup", "this.wWindow.receivedEvent (event, HTMLOverlayRenderSystemEvents.ORSE_MOUSE_UP);");
			this._spnDivWindow.setAttribute ("onselectstart", "return (false);");
			this._spnDivWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLOverlayRenderSystemEvents.ORSE_MOUSE_CLICK);");
			this._spnDivWindow.style.position = "absolute";

			this._spnWindow = document.createElement ("span");
			this._spnWindow.id = "spn" + this._name;
			this._spnWindow.wWindow = this;
			this._spnWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLOverlayRenderSystemEvents.ORSE_MOUSE_DOWN);");
			this._spnWindow.setAttribute ("onmouseup", "this.wWindow.receivedEvent (event, HTMLOverlayRenderSystemEvents.ORSE_MOUSE_UP);");
			this._spnWindow.setAttribute ("onselectstart", "return (false);");
			this._spnWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLOverlayRenderSystemEvents.ORSE_MOUSE_CLICK);");
			this._spnWindow.style.fontFamily = this._font;
			this._spnWindow.style.fontSize = this._fontSize;
			this._spnWindow.style.color = this._fontColor;
			this._spnWindow.innerHTML = this._text;

			this._spnDivWindow.appendChild (this._spnWindow);
			this._divWindow.appendChild (this._spnDivWindow);
		}

		if (this._image != null)
		{
			this._imgDivWindow = document.createElement ("img");
			this._imgDivWindow.id = "img" + this._name;
			this._imgDivWindow.wWindow = this;
			this._imgDivWindow.setAttribute ("onmousedown", "this.wWindow.receivedEvent (event, HTMLOverlayRenderSystemEvents.ORSE_MOUSE_DOWN);");
			this._imgDivWindow.setAttribute ("onmouseup", "this.wWindow.receivedEvent (event, HTMLOverlayRenderSystemEvents.ORSE_MOUSE_UP);");
			this._imgDivWindow.setAttribute ("onselectstart", "return (false);");
			this._imgDivWindow.setAttribute ("onclick", "this.wWindow.receivedEvent (event, HTMLOverlayRenderSystemEvents.ORSE_MOUSE_CLICK);");
			this._imgDivWindow.style.border = "none";
			//this._imgDivWindow.style.position = "absolute";
			this._imgDivWindow.style.width = "100%";
			this._imgDivWindow.style.height = "100%";
			//this._imgDivWindow.style.width = size[0] + "px";
			//this._imgDivWindow.style.height = size[1] + "px";
			this._imgDivWindow.style.backgroundImage = "url('" + this._image.image.src + "')";
			this._imgDivWindow.style.backgroundSize = "100% 100%";

			this._divWindow.appendChild (this._imgDivWindow);
		}

		this._parent._divWindow.appendChild (this._divWindow);
		//document.body.appendChild (this._divWindow);
		this.setEventCaptureStatus (this._captureEvents);

		for (var key in this._children)
		{
			var obj = this._children[key];
			obj.create (renderSystem, this, obj._name, obj._children);
		}
	}

	this.setVisible = function (visible)
	{
		this._visible = visible;

		if (this._visible == true)
			this._divWindow.style.display = "block";
		else
			this._divWindow.style.display = "none";

		for (var key in this._children)
		{
			var obj = this._children[key];
			obj.setVisible (visible);
		}
	}

	this.setFontColor = function (color)
	{
		this._fontColor = color;

		if (this._spnWindow != null)
			this._spnWindow.style.color = this._fontColor;
	}

	this.setText = function (text)
	{
		this._text = text;

		this._spnWindow.innerHTML = getHTMLText (this._text);
	}

	this.getText = function ()
	{
		return (this._text);
	}

	this.receivedEvent = function (event, type)
	{
		if (this._eventFunction != null)
			this._eventFunction (type);
	}

	this.setEventCaptureStatus = function (isCapturing)
	{
		this._captureEvents = isCapturing;

		if (this._captureEvents == true)
		{
			this._divWindow.style.cssText = this._divWindow.style.cssText + " pointer-events: auto;";

			if (this._spnWindow != null)
			{
				this._spnDivWindow.style.cssText = 
					this._spnDivWindow.style.cssText + " pointer-events: auto;";
				this._spnWindow.style.cssText = 
					this._spnWindow.style.cssText + " pointer-events: auto;";
			}

			if (this._imgDivWindow != null)
			{
				this._imgDivWindow.style.cssText = 
					this._imgDivWindow.style.cssText + " pointer-events: auto;";
				this._imgDivWindow.style.backgroundSize = "100% 100%";
			}
		}
		else
		{
			this._divWindow.style.cssText = this._divWindow.style.cssText + " pointer-events: none;";

			if (this._spnWindow != null)
			{
				this._spnDivWindow.style.cssText = 
					this._spnDivWindow.style.cssText + " pointer-events: none;";
				this._spnWindow.style.cssText = 
					this._spnWindow.style.cssText + " pointer-events: none;";
			}

			if (this._imgDivWindow != null)
			{
				this._imgDivWindow.style.cssText = 
					this._imgDivWindow.style.cssText + " pointer-events: none;";
				this._imgDivWindow.style.backgroundSize = "100% 100%";
			}
		}
	}
}

HTMLOverlayElement.prototype = new HTMLOverlayObject ();

