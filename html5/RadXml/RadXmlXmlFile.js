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

var ApplicationType = 
{
	AT_DESKTOP: 1, 
	AT_MOBILE: 2
};

function XmlFile ()
{
	this._dRadXmlVersion = 0;
	this._filename = "";
	this._dirPath = "";
	this._type = ApplicationType.AT_DESKTOP;
	this._tempXmlFile = null;
	this._numIncludesToLoad = 0;
	this._doingAsyncScriptLoading = false;

	this.loadFile = function (file)
	{
		var iPos = file.lastIndexOf ("/");

		this._filename = file.substr (iPos + 1);
		this._dirPath = file.substr (0, iPos);

		var strTemp = this._dirPath + "/" + this._filename;
		file = strTemp.normalizeDirectoryPath (0, false);

		logMessage (RadXmlLanguage.MSG20 + file);

		var strXML = RadXML._getSyncResponseFromHTTP (file);
		var xmlFile = this._openXMLFile (strXML, file);
		this.parseXMLFile (xmlFile);
	}

	this.loadString = function (xmlString)
	{
		var file = window.location.href;
		var iPos = file.lastIndexOf ("/");

		this._filename = file.substr (iPos + 1);
		this._dirPath = file.substr (0, iPos);

		var strTemp = this._dirPath + "/" + this._filename;
		file = strTemp.normalizeDirectoryPath (0, false);

		logMessage (RadXmlLanguage.MSG39);

		var xmlFile = this._openXMLFile (xmlString, file);
		this.parseXMLFile (xmlFile);
	}

	this._openXMLFile = function (xmlString, file)
	{
		var xmlFile = null;

		if (DOMParser)
		{
			var xmlParser = new DOMParser ();
			xmlFile = xmlParser.parseFromString (xmlString, "text/xml");
		}
		else
		{
			xmlFile = new ActiveXObject ("Microsoft.XMLDOM");
			xmlFile.async = false;
			xmlFile.loadXML (xmlString);
		}

		xmlFile.fileName = file;

		return (xmlFile);
	}

	this.parseXMLFile = function (xmlFile)
	{
		var iTempIncludes = 0;
		var aryMetas = xmlFile.getElementsByTagName ("meta");
		this._tempXmlFile = xmlFile;

		for (var iIdx = 0; iIdx < aryMetas.length; iIdx++)
		{
			var xmlElement = aryMetas[iIdx];
			var strName = RadXML._getAttribute (xmlElement, "name").value;
			strName = strName.toLowerCase ();
			var strContent = RadXML._getAttribute (xmlElement, "content").value;
			strContent = strContent.toLowerCase ();

			if (strName == "version")
				this._dRadXmlVersion = RadXML._parseFloat (strContent);

			if (strName == "collisiondetection")
				Game.setCollisionDetection (parseBoolean (strContent));

			if (strName == "type")
			{
				var iType = ApplicationType.AT_DESKTOP;

				if (strContent == "desktop")
					iType = ApplicationType.AT_DESKTOP;

				if (strContent == "mobile")
					iType = ApplicationType.AT_MOBILE;

				this._type = iType;
			}
		}

		if (this._dRadXmlVersion == 0.0)
		{
			logMessage (RadXmlLanguage.MSG21 + xmlFile.fileName);

			return;
		}

		if (this._dRadXmlVersion < RadXML.defRADXML_MIN_VERSION)
		{
			logMessage (RadXmlLanguage.MSG22 + defRADXML_MIN_VERSION + 
						RadXmlLanguage.MSG23 + xmlFile.fileName);

			return;
		}

		if (this._dRadXmlVersion > RadXML.defRADXML_VERSION)
		{
			logMessage (RadXmlLanguage.MSG24 + RadXML.defRADXML_VERSION + 
					RadXmlLanguage.MSG23 + xmlFile.fileName);

			return;
		}

		var aryIncludes = xmlFile.getElementsByTagName ("include");

		for (var iIdx = 0; iIdx < aryIncludes.length; iIdx++)
		{
			var xmlElement = aryIncludes[iIdx];
			var strSrc = RadXML._getAttribute (xmlElement, "src").value;
			var strType = RadXML._getAttribute (xmlElement, "type").value;
			strType = strType.toLowerCase ();

			if (strType == "")
				strType = "text/xml";

			if ((strType == "text/xml") || (strType == "xml"))
			{
				var strTemp = this._dirPath + "/" + strSrc;
				strSrc = strTemp.normalizeDirectoryPath (0, false);
				s13LogMessage (RadXmlLanguage.MSG25 + strSrc);

				var strXML = RadXML._getSyncResponseFromHTTP (strSrc);
				var xmlFile = this._openXMLFile (strXML, strSrc);
				this.parseXMLFile (xmlFile);

				delete xmlFile;
			}

			if ((strType == "text/javascript") || (strType == "javascript"))
			{
				logMessage (RadXmlLanguage.MSG26 + strSrc);

				RadXML.compileAndRunJSScript (strSrc);
				iTempIncludes++;
			}
		}

		var aryScripts = xmlFile.getElementsByTagName ("script");
		var aryScripts2 = xmlFile.getElementsByTagName ("rxscript");

		for (var iSdx = 0; iSdx < 2; iSdx++)
		{
			var aryS = null;

			if (iSdx == 0)
				aryS = aryScripts;

			if (iSdx == 1)
				aryS = aryScripts2;

			for (var iIdx = 0; iIdx < aryS.length; iIdx++)
			{
				var xmlElement = aryS[iIdx];
				var strType = RadXML._getAttribute (xmlElement, "type").value;
				var strSrc = RadXML._getAttribute (xmlElement, "src").value;
				strType = strType.toLowerCase ();

				if (strType == "")
					strType = "text/javascript";

				if (strSrc != "")
				{
					if ((strType == "javascript") || (strType == "text/javascript"))
					{
						var strTemp = this._dirPath + "/" + strSrc;
						strSrc = strTemp.normalizeDirectoryPath (0, false);
						logMessage (RadXmlLanguage.MSG26 + strSrc);
						RadXML.compileAndRunJSScript (strSrc);
						iTempIncludes++;
					}
				}

				if (xmlElement.textContent != "")
				{
					if ((strType == "javascript") || (strType == "text/javascript"))
					{
						include ("", "", xmlElement.textContent);
						iTempIncludes++;
					}
				}
			}
		}

		var aryExternals = xmlFile.getElementsByTagName ("external");

		for (var iIdx = 0; iIdx < aryExternals.length; iIdx++)
		{
			var xmlElement = aryExternals[iIdx];
			var strName = RadXML._getAttribute (xmlElement, "name").value;
			var strSrc = RadXML._getAttribute (xmlElement, "src").value;
			var strType = RadXML._getAttribute (xmlElement, "type").value;
			var strOS = RadXML._getAttribute (xmlElement, "os").value;
			strType = strType.toLowerCase ();
			strOS = strOS.toLowerCase ();

			if (strType == "")
				strType = "binary/x86";

			if (strOS == "")
				strOS = "windows";

			logMessage (MSG27);
		}

		this._numIncludesToLoad = iTempIncludes;

		if ((this._numIncludesToLoad == 0) || (this._doingAsyncScriptLoading == false))
		{
			this.continueLoadingXml ();
			this._tempXmlFile = null;
		}
	}

	this.checkIfScriptsAreLoaded = function ()
	{
		if (this._numIncludesToLoad <= 0)
		{
			this.continueLoadingXml ();
			this._tempXmlFile = null;
		}
	}

	this.continueLoadingXml = function ()
	{
		var xmlFile = this._tempXmlFile;
		var aryApp = xmlFile.getElementsByTagName ("app");

		for (var iIdx = 0; iIdx < aryApp.length; iIdx++)
		{
			var xmlElement = aryApp[iIdx];
			var strName = RadXML._getAttribute (xmlElement, "name").value;
			var strLoad = RadXML._getAttribute (xmlElement, "load").value;
			var strType = RadXML._getAttribute (xmlElement, "type").value;
			var strOnLoad = RadXML._getAttribute (xmlElement, "onload").value;
			strType = strType.toLowerCase ();
			var bLoad = true;
			var iType = AppTypes.AT_RENDER;

			if (strLoad != "")
				bLoad = parseBoolean (strLoad);

			if (strType == "native")
				iType = AppTypes.AT_NATIVE;

			if (strType == "render")
				iType = AppTypes.AT_RENDER;

			if (bLoad == true)
			{
				if (iIdx == 0)
					this.app._parseApp (this, iType, xmlElement);
				else
				{
					var aApp = new App (strName);
					aApp._parseApp (this, iType, xmlElement);
				}

				RadXML.compileAndRunJSString (strOnLoad);
			}
		}

		var aryGames = xmlFile.getElementsByTagName ("game");

		for (var iIdx = 0; iIdx < aryGames.length; iIdx++)
		{
			var xmlElement = aryGames[iIdx];
			var strName = RadXML._getAttribute (xmlElement, "name").value;
			var strLoad = RadXML._getAttribute (xmlElement, "load").value;
			var bLoad = true;

			if (strLoad != "")
				bLoad = parseBoolean (strLoad);

			if (bLoad == true)
			{
				if (iIdx == 0)
					this.game._parse (this, xmlElement);
				else
				{
					var gGame = new Game (strName);
					gGame._parse (this, xmlElement);
				}

				RadXML.compileAndRunJSString (strOnLoad);
			}
		}
	}

	this.getMovable = function (name)
	{
		return (this.game.getMovableByName (name));
	}
}

XmlFile.prototype.app = App;
XmlFile.prototype.game = Game;

