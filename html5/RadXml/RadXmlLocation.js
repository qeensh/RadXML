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

function Location ()
{
	this._index = 0;
	this._watchLocationObj = null;

	this.start = function ()
	{
		/*if (navigator.geolocation)
		{
			this._watchLocationObj = navigator.geolocation.watchPosition (
				RadXML._keepContext (this._onFound, this), 
				RadXML._keepContext (this._onNotFound, this));
		}
		else
			throw RadXmlLanguage.MSG37;*/

		this._index = RadXML._xmlFile.app._addLocation (this);
		this._update ();
	}

	this._update = function ()
	{
		if (navigator.geolocation)
		{
			navigator.geolocation.getCurrentPosition (
				RadXML._keepContext (this._onFound, this), 
				RadXML._keepContext (this._onNotFound, this));
		}
		else
			throw RadXmlLanguage.MSG37;
	}

	this.stop = function ()
	{
		RadXML._xmlFile.app._removeLocation (this._index);

		if (this._watchLocationObj != null)
		{
			navigator.geolocation.clearWatch (this._watchLocationObj);
			this._watchLocationObj = null;
		}
	}

	this._onFound = function (geoLoc)
	{
		var dLatitude = geoLoc.coords.latitude;
		var dLongitude = geoLoc.coords.longitude;

		if (this.onFound != null)
			this.onFound (dLatitude, dLongitude);
	}

	this._onNotFound = function (error)
	{
		if (this.onNotFound != null)
			this.onNotFound (error);
	}

	this.onFound = null;
	this.onNotFound = null;
}

