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

function include (file, type, strString)
{
	var strProtocol = location.protocol;
	var obj = document.createElement ("script");
	obj.type = "text/javascript";

	if (strProtocol != "file:")
	{
		if (strString == null)
			strString = RadXML._getSyncResponseFromHTTP (file);

		if (strString == "")
			strString = RadXML._getSyncResponseFromHTTP (file);

		if (RadXML._isUsingInternetExplorerTheWorstWebBrowserEver () == true)
			obj.text = strString;
		else
		{
			var objTextNode = document.createTextNode (strString);
			obj.appendChild (objTextNode);
		}

		if (RadXML_skipDebuggerHelper == false) /// @note This is primarily used for debugging purposes on Chrome. Set RadXML_skipDebuggerHelper to true before running or compiling RadXML.
		{
			if (file != "")
			{
				obj.async = false;
				obj.defer = false;
				obj.src = file;
				obj.onreadystatechange = function ()
					{
						if (this.readyState == null)
							this.readyState = "complete";

						if (this.readyState == "complete")
						{
							if (RadXML._xmlFile != null)
							{
								RadXML._xmlFile._numIncludesToLoad--;
								RadXML._xmlFile.checkIfScriptsAreLoaded ();
							}
							else
							{
								RadXML._numIncludesToLoad--;
								RadXML._checkIfAsyncScriptLoadingIsCompleted ();
							}
						}
					};
				obj.onload = obj.onreadystatechange;

				if (RadXML._xmlFile != null)
					RadXML._xmlFile._doingAsyncScriptLoading = true;
				else
				{
					RadXML._numIncludesToLoad++;
					RadXML._doingAsyncScriptLoading = true;
				}
			}
		}
	}
	else
	{
		obj.async = false;
		obj.defer = false;

		if (RadXML._isUsingInternetExplorerTheWorstWebBrowserEver () == true)
			obj.text = strString;
		else
		{
			var objTextNode = document.createTextNode (strString);
			obj.appendChild (objTextNode);
		}

		if (file != "")
			obj.src = file;

		obj.onreadystatechange = function ()
			{
				if (this.readyState == null)
					this.readyState = "complete";

				if (this.readyState == "complete")
				{
					if (RadXML._xmlFile != null)
					{
						RadXML._xmlFile._numIncludesToLoad--;
						RadXML._xmlFile.checkIfScriptsAreLoaded ();
					}
					else
					{
						RadXML._numIncludesToLoad--;
						RadXML._checkIfAsyncScriptLoadingIsCompleted ();
					}
				}
			};
		obj.onload = obj.onreadystatechange;

		if (RadXML._xmlFile != null)
			RadXML._xmlFile._doingAsyncScriptLoading = true;
		else
		{
			RadXML._numIncludesToLoad++;
			RadXML._doingAsyncScriptLoading = true;
		}
	}

	document.documentElement.insertBefore (obj, document.documentElement.firstChild);
}

var RadXML = 
{
	_strHTTPResponseText: "", 
	_xmlFile: null, 
	_onLoadCallback: null, 
	_canvas: null, 
	_hasFlash: false, 
	_skipFlash: false, 
	_flashObject: null, 
	_loadingPercentage: 0, 
	_loadingDiv:  null, 
	_loadingHTML: "<div style = \"width: 100%; height: 100%; background-color: #FFFFFF;\">Loading...</div>", 
	_networkingHTML: null, 
	_numIncludesToLoad: 0, 
	_doingAsyncScriptLoading: false, 
	_aryOnAsyncCompleteLoadFunctions: new Array (), 
	_aryOnAsyncCompleteLoadFunctionsArgs: new Array (), 

	_javascriptEngine: null, 
	_gameEngine: null, 
	_overlayEngine: null, 
	_appEngine: null, 
	_inputEngine: null, 

	_screenWidth: window.innerWidth, 
	_screenHeight: window.innerHeight, 

	defRADXML_MIN_VERSION: 0.1, 
	defRADXML_VERSION: 0.2, 

	_checkIfAsyncScriptLoadingIsCompleted: function ()
	{
		if (this._doingAsyncScriptLoading == true)
		{
			if (this._numIncludesToLoad <= 0)
			{
				for (var iIdx = 0; iIdx < this._aryOnAsyncCompleteLoadFunctions.length; iIdx++)
				{
					var func = this._aryOnAsyncCompleteLoadFunctions[iIdx];
					var arg = this._aryOnAsyncCompleteLoadFunctionsArgs[iIdx];

					func (arg);
				}
			}
			else
				return (false);
		}

		return (true);
	}, 

	_addScriptLoadingFunction: function (func, argument)
	{
		this._aryOnAsyncCompleteLoadFunctions.push (func);
		this._aryOnAsyncCompleteLoadFunctionsArgs.push (argument);
	}, 

	initialize: function (canvas)
	{
		if (this._checkIfAsyncScriptLoadingIsCompleted () == false)
		{
			this._addScriptLoadingFunction (this.initialize, canvas);

			return;
		}

		this.initializeGameEngine (canvas);
		this.initializeOverlayEngine ();
		this.initializeAppEngine ();
		this.initializeInputEngine ();
	},

	initializeGameEngine: function (canvas)
	{
		if (this._checkIfAsyncScriptLoadingIsCompleted () == false)
		{
			this._addScriptLoadingFunction (this.initializeGameEngine, canvas);

			return;
		}

		if (canvas == null)
			canvas = "";

		if (RadXML._hasFlash == true)
			this._createLoading ();

		if (canvas != "")
		{
			this._canvas = document.getElementById (canvas);
			this._screenWidth = this._canvas.width;
			this._screenHeight = this._canvas.height;
		}

		if (this._gameEngine == null)
		{
			this._gameEngine = new GameEngine ();

			if (this._canvas != null)
				this._gameEngine._renderCanvas = this._canvas;

			this._gameEngine.initialize ();

			if (this._canvas == null)
				this._canvas = this._gameEngine._renderCanvas;
		}
	},

	initializeOverlayEngine: function (overlayEngine)
	{
		if (this._checkIfAsyncScriptLoadingIsCompleted () == false)
		{
			this._addScriptLoadingFunction (this.initializeOverlayEngine, overlayEngine);

			return;
		}

		if (overlayEngine != null)
			this._overlayEngine = overlayEngine;

		if (RadXML._hasFlash == true)
			this._createLoading ();

		if (this._overlayEngine == null)
			this._overlayEngine = new OverlayEngine ();

		this._overlayEngine.initialize ();
	},

	initializeAppEngine: function (appEngine)
	{
		if (this._checkIfAsyncScriptLoadingIsCompleted () == false)
		{
			this._addScriptLoadingFunction (this.initializeAppEngine, appEngine);

			return;
		}

		if (appEngine != null)
			this._appEngine = appEngine;

		if (RadXML._hasFlash == true)
			this._createLoading ();

		if (this._appEngine == null)
			this._appEngine = new AppEngine ();

		this._appEngine.initialize ();

		if (RadXML._hasFlash == true)
			this._createLoading ();
	},

	initializeInputEngine: function (inputEngine)
	{
		if (this._checkIfAsyncScriptLoadingIsCompleted () == false)
		{
			this._addScriptLoadingFunction (this.initializeInputEngine, inputEngine);

			return;
		}

		if (inputEngine != null)
			this._inputEngine = inputEngine;

		if (RadXML._hasFlash == true)
			this._createLoading ();

		if (this._inputEngine == null)
			this._inputEngine = new InputEngine ();

		this._inputEngine.initialize ();
	},

	destroy: function ()
	{
	},

	skipFlash: function (skipFlash)
	{
		this._skipFlash = skipFlash;
	}, 

	compileAndRunJSScript: function (file)
	{
		include (file);
	},

	_compileAndRunJSStringFunction: function (string)
	{
		(new Function (string))();
	},

	_compileAndRunJSStringEval: function (string)
	{
		eval (string);
	},

	loadXml: function (file)
	{
		this._xmlFile = new XmlFile ();
		this._xmlFile.loadFile (file);

		if (this._onLoadCallback != null)
			this._onLoadCallback ();

		if (this._gameEngine != null)
			this._gameEngine._frameStarted (0);
	},

	loadXmlFromId: function (id)
	{
		var obj = document.getElementById (id);
		var strText = obj.innerHTML;

		this._xmlFile = new XmlFile ();
		this._xmlFile.loadString (strText);

		if (this._onLoadCallback != null)
			this._onLoadCallback ();

		if (this._gameEngine != null)
			this._gameEngine._frameStarted (0);
	},

	setupJavascript: function ()
	{
	},

	getSingleton: function ()
	{
		return (this);
	},

	getSingletonPtr: function ()
	{
		return (this);
	},

	setGameEngine: function (gameEngine)
	{
		this._gameEngine = gameEngine;
	},

	getGameEngine: function ()
	{
		return (this._gameEngine);
	},

	setAppEngine: function (appEngine)
	{
		this._appEngine = appEngine;
	},

	getAppEngine: function ()
	{
		return (this._appEngine);
	},

	setSceneManager: function (sceneManager)
	{
		this._gameEngine._mSceneMgr = sceneManager;
	}, 

	getSceneManager: function ()
	{
		return (this._gameEngine._mSceneMgr);
	}, 

	getCanvasPosition: function ()
	{
		if (this._canvas != null)
		{
			var objObject = this._canvas;

			return (newVector2 (objObject.offsetLeft, objObject.offsetTop));
		}

		return (newVector2 (0, 0));
	}, 

	getCanvasSize: function ()
	{
		if (this._canvas != null)
		{
			var objObject = this._canvas;
			var iWidth = 0;
			var iHeight = 0;

			if (objObject.clientWidth)
				iWidth = objObject.clientWidth;

			if (objObject.clientWidth)
				iHeight = objObject.clientHeight;

			if (objObject.offsetWidth)
				iWidth = objObject.offsetWidth;

			if (objObject.offsetHeight)
				iHeight = objObject.offsetHeight;

			return (newVector2 (iWidth, iHeight));
		}

		return (newVector2 (0, 0));
	}, 

	_parseInt: function (integer)
	{
		var iReturn = parseInt (integer);

		if (isNaN (iReturn) == true)
			iReturn = 0;

		if (iReturn == null)
			iReturn = 0;

		return (iReturn);
	},

	_parseFloat: function (fFloat)
	{
		var dReturn = parseFloat (fFloat);

		if (isNaN (dReturn) == true)
			dReturn = 0;

		if (dReturn == null)
			dReturn = 0;

		return (dReturn);
	},

	_parseColourValue: function (color)
	{
		var cvValue = newColour (0, 0, 0, 1);

		if (color == "")
			return (cvValue);

		var iPos = 0;
		iPos = color.indexOf ("#");
		color = color.toLowerCase ();

		if (color == "black")
			cvValue = ColourTypes.Black;

		if (color == "white")
			cvValue = ColourTypes.White;

		if (color == "red")
			cvValue = ColourTypes.Red;

		if (color == "green")
			cvValue = ColourTypes.Green;

		if (color == "blue")
			cvValue = ColourTypes.Blue;

		if (iPos > -1)
			iPos++;

		var strR = color.substr ((iPos + 0), 2);
		var strG = color.substr ((iPos + 2), 2);
		var strB = color.substr ((iPos + 4), 2);
		var iR = parseInt (strR, 16);
		var iG = parseInt (strG, 16);
		var iB = parseInt (strB, 16);
		var dR = (iR / 255.0);
		var dG = (iG / 255.0);
		var dB = (iB / 255.0);

		cvValue.r = dR;
		cvValue.g = dG;
		cvValue.b = dB;

		return (cvValue);
	}, 

	getCurrentXMLFile: function ()
	{
		return (this._xmlFile);
	}, 

	_getActualWidth: function ()
	{
		/// @note THE - 16 IS A TEMPORARY HACK TO MATCH THE EMBEDDED VERSION OF RADXML
		return (RadXML._screenWidth - 16);
	}, 

	_getActualHeight: function ()
	{
		/// @note THE - 38 IS A TEMPORARY HACK TO MATCH THE EMBEDDED VERSION OF RADXML
		return (RadXML._screenHeight - 38);
	}, 

	_keepContext: function (func, context, args)
	{
		var objReturn = function ()
			{
				var aryArgs = Array.prototype.slice.call (arguments);
				aryArgs = aryArgs.concat (args);
				return (func.apply (context, aryArgs));
			};

		return (objReturn);
	}, 

	_clone: function (obj)
	{
		if (obj == null)
			return (null);

		if (! (obj instanceof Object))
			return (obj);

		var objReturn = Object.create (obj);

		for (var property in obj)
		{
			if (Object.prototype.hasOwnProperty.call (obj, property) == true)
			{
				var obj2 = obj[property];

				/// @note Certain objects cause infinite loops
				if (obj2 instanceof THREE.Scene)
				{
					objReturn[property] = obj2;

					continue;
				}

				if (obj2 instanceof THREE.PerspectiveCamera)
				{
					objReturn[property] = obj2;

					continue;
				}

				if (obj2 instanceof HTMLElement)
				{
					objReturn[property] = obj2;

					continue;
				}

				if (obj2 instanceof Function)
				{
					objReturn[property] = obj2;

					continue;
				}

				if (property == "_mParent")
				{
					objReturn[property] = null;

					continue;
				}

				if (property == "_aryChildren")
				{
					objReturn[property] = new Array ();

					continue;
				}

				if (property == "_eObj")
				{
					objReturn[property] = null;

					continue;
				}

				if (property == "_snNode")
				{
					objReturn[property] = null;

					continue;
				}

				objReturn[property] = RadXML._clone (obj2);
				//objReturn[property] = obj2;
			}
		}

		return (objReturn);
	},

	_isUsingInternetExplorerTheWorstWebBrowserEver: function ()
	{
		if (navigator.appName)
		{
			if (navigator.appName == "Microsoft Internet Explorer")
				return (true);
		}

		return (false);
	}, 

	_getElementsByTagName: function (xmlElement, tagName, skipChildNodes)
	{
		if (skipChildNodes == null)
			skipChildNodes = false;

		if (skipChildNodes == false)
			return (xmlElement.getElementsByTagName (tagName));

		var aryReturn = new Array ();

		if (xmlElement.tagName == tagName)
			aryReturn.push (xmlElement);

		while ((xmlElement = xmlElement.nextSibling) != null)
		{
			if (xmlElement.tagName == tagName)
				aryReturn.push (xmlElement);
		}

		return (aryReturn);
	}, 

	_addMovableJSObject: function (newMovable)
	{
		this._xmlFile.game._addMovableJSObject (newMovable);
	}, 

	_getResponseFromHTTP: function (fileAddress, doFunction)
	{
		var aFile = null;

		if ((typeof doFunction) == "undefined")
			doFunction = null;

		if (window.XMLHttpRequest)
			aFile = new XMLHttpRequest ();
		else
			aFile = new ActiveXObject ("Microsoft.XMLHTTP");

		aFile.parent = this;
		aFile.onreadystatechange = function ()
		{
			if ((aFile.readyState == 4) && (aFile.status == 200))
			{
				this.parent._strHTTPResponseText = aFile.responseText;

				doFunction (this.parent._strHTTPResponseText);
			}
		}

		aFile.open ("GET", fileAddress, true);
		aFile.send ();
	}, 

	_getSyncResponseFromHTTP: function (fileAddress)
	{
		var aFile = null;

		if (window.XMLHttpRequest)
			aFile = new XMLHttpRequest ();
		else
			aFile = new ActiveXObject ("Microsoft.XMLHTTP");

		aFile.parent = this;
		aFile.onreadystatechange = function ()
		{
			try
			{
				var httpStatus = aFile.status;

				if ((aFile.readyState == 4) && (aFile.status == 200))
				{
					this.parent._strHTTPResponseText = aFile.responseText;
				}
			}
			catch (ex)
			{
			}
		}

		aFile.open ("GET", fileAddress, false);
		aFile.send ();

		return (this._strHTTPResponseText);
	}, 

	_getLargestScalarFromVector3: function (vector)
	{
		var iLargest = vector.x;

		if (vector.y > iLargest)
			iLargest = vector.y;

		if (vector.z > iLargest)
			iLargest = vector.z;

		return (iLargest);
	}, 

	_getAttribute: function (xmlElement, attributeName)
	{
		var objAttribute = xmlElement.attributes[attributeName];

		if (objAttribute == null)
		{
			if (xmlElement.setAttribute)
			{
				xmlElement.setAttribute (attributeName, "");
				objAttribute = xmlElement.attributes[attributeName];
			}
			else
				objAttribute = document.createAttribute (attributeName);
		}

		return (objAttribute);
	}, 

	_addEventListenerToDocument: function (event, func)
	{
		if (document.addEventListener)
			document.addEventListener (event, func, false);
		else
			document.attachEvent (event, func);
	}, 

	_addEventListenerToObject: function (object, event, func)
	{
		if (object.addEventListener)
			object.addEventListener (event, func, false);
		else
			object.attachEvent (event, func);
	}, 

	_addEventListener: function (eventName, func, otherEventName)
	{
		if (otherEventName == null)
			otherEventName = eventName;

		if (document.addEventListener)
			document.addEventListener (eventName, func, false);
		else
			document.attachEvent (otherEventName, func);
	}, 

	_checkForFlash: function ()
	{
		try
		{
			var obj = null;

			if (typeof (ActiveXObject) != "undefined")
				obj = new ActiveXObject ("ShockwaveFlash.ShockwaveFlash");
			else
				obj = navigator.mimeTypes["application/x-shockwave-flash"];

			if (obj == null)
				return (false);
		}
		catch (ex)
		{
			return (false);
		}

		return (true);
	}, 

	_initializeFlash: function ()
	{
		RadXML._flashObject = document.createElement ("embed");

		RadXML._flashObject.width = 0;
		RadXML._flashObject.height = 0;
		RadXML._flashObject.src = "./RadXml/RadXML.swf";

		document.body.appendChild (RadXML._flashObject);
	}, 

	_flashReady: function (isReady)
	{
		if (isReady == true)
		{
			logMessage (RadXmlLanguage.MSG40);
			RadXML._loadingPercentage = 1;
			RadXML._displayLoading (false);
		}
	}, 

	setLoadingHTML: function (html)
	{
		RadXML._loadingHTML = html;
	}, 

	_createLoading: function ()
	{
		if (RadXML._loadingDiv == null)
		{
			var canvasPos = RadXML.getCanvasPosition ();
			var canvasSize = RadXML.getCanvasSize ();

			RadXML._loadingDiv = document.createElement ("div");
			RadXML._loadingDiv.style.position = "absolute";
			RadXML._loadingDiv.style.top = canvasPos.x + "px";
			RadXML._loadingDiv.style.left = canvasPos.y + "px";

			if (canvasSize.x != 0)
			{
				RadXML._loadingDiv.style.width = canvasSize.x + "px";
				RadXML._loadingDiv.style.height = canvasSize.y + "px";
			}
			else
			{
				RadXML._loadingDiv.style.width = "100%";
				RadXML._loadingDiv.style.height = "100%";
			}

			RadXML._loadingDiv.style.zIndex = 100;
			RadXML._loadingDiv.innerHTML = RadXML._loadingHTML;

			document.body.appendChild (RadXML._loadingDiv);
			RadXML._displayLoading (true);
		}
	}, 

	_displayLoading: function (isVisible)
	{
		if (isVisible == true)
			RadXML._loadingDiv.style.display = "block";
		else
			RadXML._loadingDiv.style.display = "none";
	}, 

	_createNetworkHTML: function ()
	{
		this._networkingHTML = document.createElement ("iframe");
		this._networkingHTML.style.display = "none";

		document.body.appendChild (this._networkingHTML);
	}, 

	_networkClientOnConnected: function (clientIndex)
	{
		Networking.ActiveClients[clientIndex].onConnected ();
	}, 

	_networkClientOnReceived: function (clientIndex, data)
	{
		Networking.ActiveClients[clientIndex].onReceived (data);
	}
};

if (typeof (RadXML_skipFlash) != "undefined")
	RadXML._skipFlash = RadXML_skipFlash;

if (typeof (RadXML_skipDebuggerHelper) == "undefined")
	RadXML_skipDebuggerHelper = false;

if (typeof (RadXML_isMinBuild) == "undefined")
	RadXML_isMinBuild = false;

if (typeof (RadXML_useEval) == "undefined")
	RadXML_useEval = false;

if (RadXML_useEval == true)
	RadXML.compileAndRunJSString = RadXML._compileAndRunJSStringEval;
else
	RadXML.compileAndRunJSString = RadXML._compileAndRunJSStringFunction;

if (RadXML._skipFlash == false)
{
	RadXML._hasFlash = RadXML._checkForFlash ();

	if (RadXML._hasFlash == true)
	{
		if (window.addEventListener)
			window.addEventListener ("load", RadXML._initializeFlash, false);
		else
			window.attachEvent ("onload", RadXML._initializeFlash);
	}
}

if (RadXML_isMinBuild == false)
{
	include ("./RadXml/RadXmlUtils.js");
	include ("./RadXml/RadXmlIncludeFiles.js");
}

