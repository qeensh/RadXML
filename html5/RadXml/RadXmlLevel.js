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

function Level ()
{
	this._name = "";
	this._ambientColour = new Colour (1, 1, 1, 1);
	this._shadowColour = new Colour (0, 0, 0, 1);

	this._aryAnimations = new Array ();
	this._aryMusic = new Array ();
	this._aryOverlays = new Array ();
	this._aryOverlayElements = new Array ();

	this._hshMovables = new Array ();

	this._addEvent (new Event ("onload", ""));

	this.setName = function (newName)
	{
		this._name = newName;
	}

	this.getName = function ()
	{
		return (this._name);
	}

	this._parseLevel = function (file, xmlElement)
	{
		RadXML._gameEngine._setupDefaultSceneManager ();

		this._name = RadXML._getAttribute (xmlElement, "name").value;
		this._setJavascriptToEvent ("onload", RadXML._getAttribute (xmlElement, "onload").value);

		var strSkyBox = RadXML._getAttribute (xmlElement, "skybox").value;
		var strAmbientColor = RadXML._getAttribute (xmlElement, "ambientcolor").value;
		var strShadowColor = RadXML._getAttribute (xmlElement, "shadowcolor").value;

		if (this._name == "")
		{
			var iRandName = RadXML._parseInt (Math.random () * 1000000000);
			this.setName (iRandName);
		}

		if (strSkyBox != "")
			RadXML._gameEngine.setSkyBoxMaterial (strSkyBox);

		if (strAmbientColor != "")
		{
			this._ambientColour = RadXML._parseColourValue (strAmbientColor);
			RadXML._gameEngine.setAmbientLight (this._ambientColour);
		}

		if (strShadowColor != "")
		{
			this._shadowColour = RadXML._parseColourValue (strShadowColor);
			RadXML._gameEngine.setShadowColour (this._ambientColour);
		}

		if (xmlElement.childNodes.length > 0)
			this._parseXMLMovables (file, xmlElement.childNodes[0]);

		this._executeJavascriptFromEvent ("onload");
	}

	this._parseXMLMovables = function (file, element, parent)
	{
		for (var iIdx = 0; iIdx < 9; iIdx++)
		{
			var strType = "";
			var iType = (iIdx + 1);

			switch (iIdx)
			{
				case 0:
					strType = "entity";
					break;
				case 1:
					strType = "camera";
					break;
				case 2:
					strType = "particlesystem";
					break;
				case 3:
					strType = "light";
					break;
				case 4:
					strType = "animationtrack";
					break;
				case 5:
					strType = "audiolistener";
					break;
				case 6:
					strType = "sound";
					break;
				case 7:
					strType = "overlay";
					break;
				case 8:
					strType = "overlayelement";
					break;
			}

			var aryTags = RadXML._getElementsByTagName (element, strType, true);

			for (var iJdx = 0; iJdx < aryTags.length; iJdx++)
			{
				var xmlElement = aryTags[iJdx];

				this._parseXMLMovable (file, iType, xmlElement, parent);
			}
		}
	}

	this._parseXMLMovable = function (file, type, element, parent)
	{
		var objObject = this._createMovable (file, type, "", parent, element);

		if (element.childNodes.length > 0)
			this._parseXMLMovables (file, element.childNodes[0], objObject);
	}

	this._createMovable = function (file, type, name, parent, element, createNow)
	{
		if (createNow == null)
			createNow = true;

		var objObject = null;

		if (type == GameObjectTypes.GOT_ENTITY)
		{
			var eEntity = new Entity (name);

			if (element != null)
				eEntity._parseXML (file, element, parent);

			if (createNow == true)
				eEntity._create (parent);

			objObject = eEntity;
		}

		if (type == GameObjectTypes.GOT_CAMERA)
		{
			var cCamera = new Camera (name);

			if (element != null)
				cCamera._parseXML (file, element, parent);

			if (createNow == true)
				cCamera._create (0, parent);

			objObject = cCamera;
		}

		if (type == GameObjectTypes.GOT_PARTICLE_SYSTEM)
		{
			var psSystem = new ParticleSystem (name);

			if (element != null)
				psSystem._parseXML (file, element, parent);

			if (createNow == true)
				psSystem._create (parent);

			objObject = psSystem;
		}

		if (type == GameObjectTypes.GOT_LIGHT)
		{
			var lLight = new Light (name);

			if (element != null)
				lLight._parseXML (file, element, parent);

			if (createNow == true)
				lLight._create (parent);

			objObject = lLight;
		}

		if (type == GameObjectTypes.GOT_ANIMATION_TRACK)
		{
			var atoObject = new AnimationTrackObject (name);

			if (element != null)
				atoObject._parseXML (file, element, parent);

			if (createNow == true)
				atoObject._create (parent);

			objObject = atoObject;
			this._aryAnimations.push (atoObject);
		}

		if (type == GameObjectTypes.GOT_AUDIO_LISTENER)
		{
			var alListener = new AudioListener (name);

			if (element != null)
				alListener._parseXML (file, element, parent);

			if (createNow == true)
				alListener._create (parent);

			objObject = alListener;
		}

		if (type == GameObjectTypes.GOT_SOUND)
		{
			var sSound = new Sound (name);

			if (element != null)
				sSound._parseXML (file, element, parent);

			if (createNow == true)
				sSound._create (parent);

			objObject = sSound;
		}

		if (type == GameObjectTypes.GOT_OVERLAY)
		{
			var oOverlay = new Overlay (name);

			if (element != null)
				oOverlay._parseXML (file, element, parent);

			if (createNow == true)
				oOverlay._create (parent);

			this._aryOverlays.push (oOverlay);
			objObject = oOverlay;
		}

		if (type == GameObjectTypes.GOT_OVERLAY_ELEMENT)
		{
			var oOverlayElement = new OverlayElement (name);

			if (element != null)
				oOverlayElement._parseXML (file, element, parent);

			if (createNow == true)
				oOverlayElement._create (parent);

			this._aryOverlayElements.push (oOverlayElement);
			objObject = oOverlayElement;
		}

		if (objObject != null)
			this._addMovable (objObject);

		return (objObject);
	}

	this._addMovable = function (newMovable)
	{
		this._hshMovables[newMovable.getName ()] = newMovable;
		RadXML.getSingletonPtr ()._addMovableJSObject (newMovable);
	}
}

Level.prototype = new Events ();

