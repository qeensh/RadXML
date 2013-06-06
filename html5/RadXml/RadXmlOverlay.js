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

function Overlay (name)
{
	this._name = name;
	this._type = GameObjectTypes.GOT_OVERLAY;

	this._overlay = "";
	this._ovOverlay = null;

	this._parseXML = function (level, element, parent)
	{
		this._parseMovableXML (level, element, parent);
		this._overlay = RadXML._getAttribute (element, "overlay").value;
	}

	this._create = function (parent)
	{
		this._ovOverlay = RadXML._overlayEngine.createOverlay (this);

		this.setVisible (this._bVisible);
	}

	this.setVisible = function (isVisible)
	{
		this._bVisible = isVisible;

		if (this._ovOverlay != null)
			RadXML._overlayEngine.setVisible (this, isVisible);
	}
}

Overlay.prototype = new Movable ();

