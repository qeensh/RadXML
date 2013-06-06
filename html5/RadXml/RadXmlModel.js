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

function Model (name)
{
	this._name = name;
	this._type = GameObjectTypes.GOT_MODEL;
	this._loadNow = true;

	this._src = "";

	this._parseXML = function (level, element)
	{
		var subName = RadXML._getAttribute (element, "name").value;
		var strSrc = RadXML._getAttribute (element, "src").value;
		var strLoad = RadXML._getAttribute (element, "load").value;

		if (subName == "")
		{
			var iRandName = RadXML._parseInt (Math.random () * 1000000000);
			var objFound = level.getMovable (iRandName, true);

			while (objFound != 0)
			{
				iRandName = RadXML._parseInt (Math.random () * 1000000000);
				objFound = level.getMovable (iRandName, true);
			}

			subName = "obj" + iRandName;
		}

		if (strLoad != "")
			this._loadNow = parseBoolean (strLoad);

		this._setName (subName);

		if (strSrc != "")
		{
			var strTemp = level._dirPath + "/" + strSrc;
			strSrc = strTemp.normalizeDirectoryPath (0, false);
			this._src = strSrc;
		}
	}

	this._create = function ()
	{
		RadXML._gameEngine.loadModel (this);
	}

	this._destroy = function ()
	{
	}

	this._setName = function (name)
	{
		this._name = name;
	}

	this.getName = function ()
	{
		return (this._name);
	}
}

