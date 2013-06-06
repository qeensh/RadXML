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

function AnimationTrack (name)
{
	this._attach = "";
	this._length = 0;
	this._loop = false;
	this._enabled = true;

	this._parseXML = function (level, element, parent)
	{
		this._parseMovableXML (level, element, parent);

		var strAttach = RadXML._getAttribute (element, "attach").value;
		var strLength = RadXML._getAttribute (element, "length").value;
		var strLoop = RadXML._getAttribute (element, "loop").value;
		var strEnabled = RadXML._getAttribute (element, "enabled").value;

		this._attach = strAttach;
		this._length = RadXML._parseFloat (strLength);

		if (strLoop != "")
			this._loop = parseBoolean (strLoop);

		if (strEnabled != "")
			this._enabled = parseBoolean (strEnabled);
	}

	this._create = function (parent)
	{
		RadXML._gameEngine.createAnimationTrack (this);
	}
}

AnimationTrack.prototype = new Movable ();

