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

function OverlayTemplate (name)
{
	this._name = "";
	this._panels = new Object ();
	this._children = new Object ();

	this._parseXML = function (level, element)
	{
		this._name = RadXML._getAttribute (element, "name").value;

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
	}

	this._addOverlayPanel = function (overlayPanel)
	{
		this._children[overlayPanel._name] = overlayPanel;
	}

	this._removeOverlayPanel = function (name)
	{
		delete this._panels[name];
	}

	this.getParent = function ()
	{
		return (null);
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

