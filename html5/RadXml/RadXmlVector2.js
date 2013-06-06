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

function Vector2 (x, y)
{
	if (x == null)
		x = 0;

	if (y == null)
		y = 0;

	this.x = x;
	this.y = y;

	this.toString = function ()
	{
		return (this.x + "," + this.y);
	}
}

function parseVector2 (string)
{
	var v2dReturn = new Vector2 ();

	if (string == "")
		return (v2dReturn);

	string = string.toLowerCase ();
	string = string.replace (/ /g, "");
	var aryStrings = string.split (",");

	v2dReturn.x = parseFloat (aryStrings[0]);
	v2dReturn.y = parseFloat (aryStrings[1]);

	return (v2dReturn);
}

function newVector2 (newX, newY)
{
	var vReturn = new Vector2 ();

	vReturn.x = newX;
	vReturn.y = newY;

	return (vReturn);
}

