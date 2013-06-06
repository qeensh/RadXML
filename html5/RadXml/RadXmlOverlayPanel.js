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

function OverlayPanel (name)
{
	this._name = name;
	this._top = 0;
	this._left = 0;
	this._width = 0;
	this._height = 0;
	this._material = "";
	this._font = "Arial";
	this._fontSize = 0.06;
	this._fontColour = ColourTypes.White;
	this._text = "";

	this._parent = null;
	this._children = new Object ();

	this._parseXML = function (level, element)
	{
		this._name = RadXML._getAttribute (element, "name").value;
		var strTop = RadXML._getAttribute (element, "top").value;
		var strLeft = RadXML._getAttribute (element, "left").value;
		var strPosition = RadXML._getAttribute (element, "position").value;
		var strWidth = RadXML._getAttribute (element, "width").value;
		var strHeight = RadXML._getAttribute (element, "height").value;
		var strSize = RadXML._getAttribute (element, "size").value;
		this._material = RadXML._getAttribute (element, "material").value;
		this._font = RadXML._getAttribute (element, "font").value;
		var strFontSize = RadXML._getAttribute (element, "fontsize").value;
		var strFontColor = RadXML._getAttribute (element, "fontcolor").value;
		this._text = RadXML._getAttribute (element, "text").value;

		this._top = RadXML._parseFloat (strTop);
		this._left = RadXML._parseFloat (strLeft);
		this._width = RadXML._parseFloat (strWidth);
		this._height = RadXML._parseFloat (strHeight);

		if (strPosition != "")
		{
			strPosition = strPosition.removeWhitespaces ();
			var aryString = strPosition.split (",");
			this._top = RadXML._parseFloat (aryString[0]);
			this._left = RadXML._parseFloat (aryString[1]);
		}

		if (strSize != "")
		{
			strSize = strSize.removeWhitespaces ();
			var aryString = strSize.split (",");
			this._width = RadXML._parseFloat (aryString[0]);
			this._height = RadXML._parseFloat (aryString[1]);
		}

		if (strFontSize != "")
			this._fontSize = RadXML._parseFloat (strFontSize);

		if (strFontColor != "")
			this._fontColour = RadXML._parseColourValue (strFontColor);

		if (this._name == "")
		{
			var iRandName = RadXML._parseInt (Math.random () * 1000000000);
			var objFound = level.getMovable (iRandName, true);

			while (objFound != null)
			{
				iRandName = RadXML._parseInt (Math.random () * 1000000000);
				objFound = level.getMovable (iRandName, true);
			}

			this._name = "obj" + iRandName;
		}

		if (element.childNodes.length > 0)
		{
			for (var iIdx = 0; iIdx < element.childNodes.length; iIdx++)
			{
				if (element.childNodes[iIdx].nodeName == "overlaypanel")
				{
					var oPanel = new OverlayPanel ("");
					oPanel._parseXML (level, element.childNodes[iIdx]);

					this._addOverlayPanel (oPanel);
				}
			}
		}
	}

	this._create = function (parent)
	{
		parent._addOverlayPanel (this);
	}

	this._addOverlayPanel = function (overlayPanel)
	{
		this._children[overlayPanel._name] = overlayPanel;
	}

	this._setParent = function (parent)
	{
		this._parent = parent;
	}

	this.getParent = function ()
	{
		return (this._parent);
	}

	this.getChildren = function ()
	{
		return (this._children);
	}

	this.getName = function ()
	{
		return (this._name);
	}
}

