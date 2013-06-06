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

function Event (name, javascript)
{
	if (name == null)
		name = "";

	if (javascript == null)
		javascript = "";

	this._name = name;
	this._javascript = javascript;

	this._setName = function (newName)
	{
		this._name = newName;
	}

	this._getName = function ()
	{
		return (name);
	}

	this._setJavascript = function (newJavascript)
	{
		this._javascript = newJavascript;
	}

	this._getJavascript = function ()
	{
		return (this._javascript);
	}
}

function Events ()
{
	this._hshEvents = new Object ();

	this._addEvent = function (newEvent)
	{
		this._hshEvents[newEvent._getName ()] = newEvent;
	}

	this._getEvent = function (findName)
	{
		return (this._hshEvents[findName]);
	}

	this._setJavascriptToEvent = function (findName, code)
	{
		this._hshEvents[findName]._javascript = code;
	}

	this._getJavascriptFromEvent = function (findName)
	{
		return (this._hshEvents[findName]._javascript);
	}

	this._executeJavascriptFromEvent = function (findName)
	{
		var strJS = this._hshEvents[findName]._javascript;
		RadXML.compileAndRunJSString (strJS);
	}
}

