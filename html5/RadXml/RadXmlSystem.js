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

var System = 
{
	_makeObjectsIntoVariables: false, 

	DESKTOP: 1, 
	MOBILE: 2, 
	HTML5: 3, 

	setAllMajorObjectsIntoVariables: function (booleanValue)
	{
		this._makeObjectsIntoVariables = booleanValue;
	}, 

	makeAllMajorObjectsIntoVariables: function ()
	{
		for (var strName in App.AppObjects)
		{
			var obj = App.AppObjects[strName];
			eval ("window." + strName + " = App.AppObjects[\"" + strName + "\"];");
		}
	}, 

	getAllMajorObjectsIntoVariablesValue: function ()
	{
		return (booleanValue);
	}, 

	getOSType: function ()
	{
		return (System.HTML5);
	}, 

	isOSMobile: function ()
	{
		var strBrowser = navigator.appName;
		strBrowser = strBrowser.toLowerCase ();

		if ((strBrowser == "iphone") || (strBrowser == "ipod"))
			return (true);

		if (strBrowser == "ipad")
			return (true);

		if (strBrowser == "android")
			return (true);

		if (strBrowser == "opera mini")
			return (true);

		if (strBrowser == "blackberry")
			return (true);

		if (strBrowser == "palm")
			return (true);

		if (strBrowser == "windows ce")
			return (true);

		return (false);
	}
};