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

function parseBoolean (string)
{
	string = string.toLowerCase ();

	if (string == "yes")
		return (true);
	else if (string == "no")
		return (false);
	else if (string == "true")
		return (true);
	else if (string == "false")
		return (false);
	else
	{
		var iValue = parseInt (string);

		if (iValue != 0)
			return (true);
	}

	return (false);
}

String.prototype.removeWhitespaces = function ()
{
	return (this.replace (/\s+/g, ''));
}

String.prototype.normalizeDirectoryPath = function (iExclude, bIncludeLastSlash)
{
	var strChar = "/";
	var aryDirs = new Array ();
	var strDir = this;
	var strReturn = "";
	var strProtocol = "";

	if (iExclude == null)
		iExclude = 0;

	if (bIncludeLastSlash == null)
		bIncludeLastSlash = false;

	if (strDir.indexOf ("://") < 0)
	{
		var strURL = window.location.protocol + "//" + window.location.host + window.location.pathname;
		var iPos = strURL.lastIndexOf ("/");

		if (iPos > -1)
			strURL = strURL.substr (0, (iPos + 1));

		strDir = strURL + strDir;
	}

	if (strDir.indexOf ("://") > 0)
	{
		var iPos = -1;
		var strSearchFor = "://";
		var iSearchForSize = 3;

		if (strDir.indexOf ("file:///") > -1)
		{
			strSearchFor = ":///";
			iSearchForSize = 4;
		}

		iPos = strDir.indexOf (strSearchFor);
		strProtocol = strDir.substr (0, iPos + iSearchForSize);
		strDir = strDir.substr (iPos + iSearchForSize);
		strReturn = strProtocol;
	}

	if (strDir.indexOf ("/") > 0)
		aryDirs = strDir.split ("/");

	for (var iIdx = 0; iIdx < (aryDirs.length - 1); iIdx++)
	{
		if (aryDirs[iIdx] == ".")
		{
			aryDirs.splice (iIdx, 1);
			iIdx--;

			continue;
		}

		if (aryDirs[iIdx] == "..")
		{
			if ((iIdx - 1) > 0)
			{
				aryDirs.splice (iIdx, 1);
				iIdx--;

				aryDirs.splice (iIdx, 1);
				iIdx--;

				continue;
			}
		}
	}

	for (var iIdx = 0; iIdx < (aryDirs.length - iExclude); iIdx++)
	{
		if ((iIdx + 1) == aryDirs.length)
		{
			if (bIncludeLastSlash == true)
				strReturn += (aryDirs[iIdx] + strChar);
			else
				strReturn += aryDirs[iIdx];
		}
		else
			strReturn += (aryDirs[iIdx] + strChar);
	}

	return (strReturn);
}

Math.map = function (value, start1, stop1, start2, stop2)
{
	return (start2 + (stop2 - start2) * (value - start1) / (stop1 - start1));
}

Math.cosh = function (arg)
{
	// http://kevin.vanzonneveld.net
	// +   original by: Onno Marsman
	return (Math.exp(arg) + Math.exp(-arg)) / 2;
}

Math.tanh = function (arg)
{
	// http://kevin.vanzonneveld.net
	// +   original by: Onno Marsman
	return (Math.exp(arg) - Math.exp(-arg)) / (Math.exp(arg) + Math.exp(-arg));
}

Math.log10 = function (arg)
{
	// http://kevin.vanzonneveld.net
	// +   original by: Philip Peterson
	// +   improved by: Onno Marsman
	// +   improved by: Tod Gentille
	// +   improved by: Brett Zamir (http://brett-zamir.me)
	return Math.log(arg) / 2.302585092994046; // Math.LN10
}

Math.clamp = function (x, a, b)
{
	var dReturn = 0;

	if (x < a)
		dReturn = a;
	else
	{
		if (x > b)
			dReturn = b;
		else
			dReturn = x;
	}

	return (dReturn);
}

