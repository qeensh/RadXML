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

var GameObjectTypes = 
{
	GOT_ENTITY						: 1,
	GOT_CAMERA						: 2,
	GOT_PARTICLE_SYSTEM				: 3,
	GOT_LIGHT						: 4,
	GOT_ANIMATION_TRACK				: 5,
	GOT_AUDIO_LISTENER				: 6,
	GOT_SOUND						: 7,
	GOT_OVERLAY						: 8,
	GOT_OVERLAY_ELEMENT				: 9,
	GOT_MATERIAL					: 10,
	GOT_MODEL						: 11,
	GOT_PARTICLE_SYSTEM_TEMPLATE	: 12
};

var Game = 
{
	_file: null, 
	_xmlFile: null, 

	ENTITY: 1, 
	CAMERA: 2, 
	PARTICLE_SYSTEM: 3, 
	LIGHT: 4, 
	ANIMATION_TRACK: 5, 
	AUDIO_LISTENER: 6, 
	SOUND: 7, 

	Models: new Object (),
	Materials: new Object (),
	ParticleSystems: new Object (),
	OverlayTemplates: new Object (),
	Movables: new Object (),

	_gameObjects: new Array (), 
	_bHasCollisionDetection: true, 

	onFrameStarted: null, 
	onFrameEnded: null, 

	_parse: function (file, xmlFile)
	{
		this._file = file;
		this._xmlFile = xmlFile;

		var aryResourceLocations = xmlFile.getElementsByTagName ("resourcelocation");

		for (var iIdx = 0; iIdx < aryResourceLocations.length; iIdx++)
		{
			var xmlElement = aryResourceLocations[iIdx];
			var strDir = RadXML._getAttribute (xmlElement, "dir").value;
			var strArch = RadXML._getAttribute (xmlElement, "arch").value;
			var strType = RadXML._getAttribute (xmlElement, "type").value;
			var strLoad = RadXML._getAttribute (xmlElement, "load").value;
			strType = strType.toLowerCase ();
			var bLoad = false;

			var strTemp = this._dirPath + "/" + strDir;
			strDir = strTemp.normalizeDirectoryPath (0, false);

			if (strLoad != "")
				bLoad = parseBoolean (strLoad);

			RadXML._gameEngine.
				addResourceLocation (strDir, strArch, strType, bLoad);
		}

		var aryModels = xmlFile.getElementsByTagName ("model");

		for (var iIdx = 0; iIdx < aryModels.length; iIdx++)
		{
			var xmlElement = aryModels[iIdx];
			var mModel = new Model ("");
			mModel._parseXML (file, xmlElement);

			if (mModel._loadNow == true)
				mModel._create ();

			Game._addModel (mModel);
		}

		if (RadXML._gameEngine._numModelsToLoad == 0)
			this._continueParsing ();
	}, 

	_continueParsing: function ()
	{
		var file = this._file;
		var xmlFile = this._xmlFile;
		var aryMaterials = xmlFile.getElementsByTagName ("material");

		for (var iIdx = 0; iIdx < aryMaterials.length; iIdx++)
		{
			var xmlElement = aryMaterials[iIdx];
			var mMaterial = new Material ("");
			mMaterial._parseXML (file, xmlElement);

			if (mMaterial._loadNow == true)
				mMaterial._create ();

			Game._addMaterial (mMaterial);
		}

		var aryPSTemplates = xmlFile.getElementsByTagName ("particlesystemtemplate");

		for (var iIdx = 0; iIdx < aryPSTemplates.length; iIdx++)
		{
			var xmlElement = aryPSTemplates[iIdx];
			var pstTemplate = new ParticleSystemTemplate ("");
			pstTemplate._parseXML (file, xmlElement);

			if (pstTemplate._loadNow == true)
				pstTemplate._create ();

			Game._addParticleSystem (pstTemplate);
		}

		var aryOverlayTemplates = xmlFile.getElementsByTagName ("overlaytemplate");

		for (var iIdx = 0; iIdx < aryOverlayTemplates.length; iIdx++)
		{
			var xmlElement = aryOverlayTemplates[iIdx];
			var otTemplate = new OverlayTemplate ("");
			otTemplate._parseXML (file, xmlElement);
			otTemplate._create ();

			Game._addOverlayTemplate (otTemplate);
		}

		var aryLevels = xmlFile.getElementsByTagName ("level");

		for (var iIdx = 0; iIdx < aryLevels.length; iIdx++)
		{
			var xmlElement = aryLevels[iIdx];

			if (iIdx == 0)
				this.level._parseLevel (file, xmlElement);
			else
			{
				var lvlLevel = new Level ();
				lvlLevel._parseLevel (file, xmlElement);
			}
		}
	}, 

	setCollisionDetection: function (value)
	{
		this._bHasCollisionDetection = value;
	}, 

	getCollisionDetection: function ()
	{
		return (this._bHasCollisionDetection);
	}, 

	_addModel: function (model)
	{
		this.Models[model._name] = model;
	},

	_addMaterial: function (material)
	{
		this.Materials[material._name] = material;
	},

	_getImageFromMaterial: function (material)
	{
		return (RadXML._gameEngine.getImageFromMaterial (this.Materials[material._name]));
	},

	_getImageFromMaterialName: function (name)
	{
		return (RadXML._gameEngine.getImageFromMaterial (this.Materials[name]));
	},

	_addOverlayTemplate: function (overlayTemplate)
	{
		this.OverlayTemplates[overlayTemplate._name] = overlayTemplate;
	},

	_addMovableJSObject: function (newMovable)
	{
		this.Movables[newMovable._name] = newMovable;

		if (newMovable._type == GameObjectTypes.GOT_ENTITY)
			this._gameObjects.push (newMovable._eObj);
	}, 

	getMovableByName: function (name)
	{
		return (this.Movables[name]);
	},

	createObject: function (movableType, name, parent)
	{
		var xmlCurrentFile = RadXML._xmlFile;

		xmlCurrentFile._createMovable (xmlCurrentFile, movableType, name, parent);
	},

	deleteObject: function (name)
	{
		var objMovable = this.Movables[name];

		if (objMovable == null)
			throw RadXmlLanguage.MSG32 + name;

		if (this.Movables[name]._snNode != null)
			RadXML._gameEngine.destroySceneNode (this.Movables[name]._snNode);

		delete this.Movables[name];
	},

	getObjectsFromRayCast: function (x, y, z, xdir, ydir, zdir)
	{
	}
};

Game.level = new Level ();

