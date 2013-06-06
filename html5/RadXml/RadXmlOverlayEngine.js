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

var OverlayRenderSystemTypes = 
{
	WEBGL: 1, 
	HTML: 2
};

function OverlayEngine ()
{
	this._renderSystem = null;
	this._renderSystemType = OverlayRenderSystemTypes.HTML;

	this._overlayElements = new Object ();

	this.initialize = function ()
	{
		if (this._renderSystemType == OverlayRenderSystemTypes.HTML)
		{
			this._renderSystem = new HTMLOverlayRenderSystem ();
			this._renderSystem.setupInputs (this._renderSystem);
		}
	}

	this.getSystemReadableChildren = function (overlayThingy)
	{
		var children = new Object ();

		for (var key in overlayThingy._children)
		{
			var obj = overlayThingy._children[key];
			var temp = new HTMLOverlayElement ();
			temp._name = obj._name;
			temp._text = obj._text;

			if (obj._material != "")
				temp._image = Game._getImageFromMaterialName (obj._material);

			temp._left = obj._left;
			temp._top = obj._top;
			temp._width = obj._width;
			temp._height = obj._height;
			temp._font = obj._font;
			temp._fontSize = "14px";
			temp._fontColor = obj._fontColour.toHTMLColourString ();

			this._overlayElements[key] = temp;
			temp._children = this.getSystemReadableChildren (obj);

			children[obj._name] = temp;
		}

		return (children);
	}

	this.createOverlay = function (overlay)
	{
		var ovTemplate = Game.OverlayTemplates[overlay._overlay];

		if (ovTemplate == null)
		{
			logMessage (RadXmlLanguage.MSG38 + overlay._overlay);

			return (null);
		}

		this._overlayElements[overlay._overlay] = obj;

		var children = this.getSystemReadableChildren (ovTemplate);
		var obj = this._renderSystem.createOverlay (ovTemplate._name, children);

		return (obj);
	}

	this.getOverlayElement = function (name)
	{
		return (this._overlayElements[name]);
	}

	this.setPosition = function (obj, x, y)
	{
	}

	this.setSize = function (obj, width, height)
	{
	}

	this.setVisible = function (obj, visible)
	{
		var obj2 = this._renderSystem._overlays[obj._overlay];

		if (obj2 != null)
			obj2.setVisible (visible);
	}

	this.setText = function (obj, text)
	{
		var obj2 = obj._overlayElement;

		if (obj2 != null)
			obj2.setText (text);
	}

	this.getText = function (obj)
	{
		return ("");
	}
}

