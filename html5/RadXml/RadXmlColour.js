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

var ColourTypes = 
{
	Black: newColour (0, 0, 0, 1), 
	White: newColour (1, 1, 1, 1), 
	Red: newColour (1, 0, 0, 1), 
	Green: newColour (0, 1, 0, 1), 
	Blue: newColour (0, 0, 1, 1)
};

function Colour ()
{
	this.r = 0;
	this.g = 0;
	this.b = 0;
	this.a = 1;

	this.toHexString = function ()
	{
		var red = this.r;
		var green = this.g;
		var blue = this.b;

		red *= 255;
		green *= 255;
		blue *= 255;

		red = red.toString (16);
		green = green.toString (16);
		blue = blue.toString (16);

		if (parseInt (red) <= 9)
			red = "0" + red;

		if (parseInt (green) <= 9)
			green = "0" + green;

		if (parseInt (blue) <= 9)
			blue = "0" + blue;

		return ("0x" + red + green + blue);
	}

	this.toHTMLColourString = function ()
	{
		var hex = this.toHexString ();
		hex = hex.substring (2);
		return ("#" + hex);
	}

	this.toHexInt = function ()
	{
		var hex = this.toHexString ();
		return (parseInt (hex));
	}
}

function parseColour (string)
{
	return (RadXML._parseColourValue (string));
}

function newColour (r, g, b, a)
{
	var cColour = new Colour ();

	cColour.r = r;
	cColour.g = g;
	cColour.b = b;
	cColour.a = a;

	return (cColour);
}

