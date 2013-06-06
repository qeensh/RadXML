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

function Material (name)
{
	this._name = name;
	this._type = GameObjectTypes.GOT_MATERIAL;

	this._image = "";
	this._animationImages = new Array ();
	this._numFrames = 0;
	this._animationTime = 0.0;
	this._alphas = true;
	this._loadNow = true;
	this._cubicTexturePaths = new Array ();
	this._loaded = false;

	this._cubicTextures = new Array ();
	this._textures = new Array ();
	this._engineMaterials = new Array ();
	this._textureTimings = new Array ();

	this._currentFrame = 0;
	this._currentTime = 0;

	this._parseXML = function (level, element)
	{
		var subName = RadXML._getAttribute (element, "name").value;
		var strImage = RadXML._getAttribute (element, "image").value;
		var strAnimation = RadXML._getAttribute (element, "animation").value;
		var strTime = RadXML._getAttribute (element, "time").value;
		var strFrames = RadXML._getAttribute (element, "frames").value;
		var strAlphas = RadXML._getAttribute (element, "alphas").value;
		var strCubicTexture = RadXML._getAttribute (element, "cubictexture").value;
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

		if (strImage != "")
		{
			var strTemp = level._dirPath + "/" + strImage;
			strImage = strTemp.normalizeDirectoryPath (0, false);
			this._image = strImage;
		}

		if (strAlphas != "")
			this._alphas = parseBoolean (strAlphas);

		if (strCubicTexture != "")
		{
			var aryTemps = strCubicTexture.split ("|");

			for (var iIdx = 0; iIdx < aryTemps.length; iIdx++)
			{
				var strPath = aryTemps[iIdx];
				var strTemp = level._dirPath + "/" + strPath;
				strPath = strTemp.normalizeDirectoryPath (0, false);
				this._cubicTexturePaths.push (strPath);
			}
		}

		if (strLoad != "")
			this._loadNow = parseBoolean (strLoad);

		this._animationTime = RadXML._parseFloat (strTime);
		this._numFrames = RadXML._parseInt (strFrames);

		if (strAnimation != "")
		{
			var strTemp = level._dirPath + "/" + strAnimation;
			strAnimation = strTemp.normalizeDirectoryPath (0, false);
			var iPos = strAnimation.lastIndexOf (".");

			if (iPos > -1)
			{
				var strExt = strAnimation.substr (iPos);
				var strFilename = strAnimation.substr (0, iPos);

				for (var iIdx = 0; iIdx < this._numFrames; iIdx++)
				{
					var strFrameUrl = strFilename + "_" + iIdx + strExt;
					this._animationImages.push (strFrameUrl);
				}
			}
		}

		this._setName (subName);
	}

	this._create = function ()
	{
		RadXML._gameEngine.loadMaterial (this);
	}

	this._destroy = function ()
	{
	}

	this._addTexture = function (texture, textureTiming)
	{
		if (textureTiming == null)
			textureTiming = 0;

		this._textures.push (texture);
		this._textureTimings.push (textureTiming);
	}

	this._addCubicTexture = function (texture, textureTiming)
	{
		if (textureTiming == null)
			textureTiming = 0;

		this._cubicTextures.push (texture);
		this._textureTimings.push (textureTiming);
	}

	this._addMaterial = function (material)
	{
		this._engineMaterials.push (material);
	}

	this._loadedFunction = function ()
	{
		this._loaded = true;
		RadXML._gameEngine.createMaterial (this);
	}

	this._setName = function (name)
	{
		this._name = name;
	}

	this.getName = function ()
	{
		return (this._name);
	}

	this.load = function ()
	{
		if (this._loaded == true)
			this._destroy ();

		this._create ();
	}

	this.isLoaded = function ()
	{
		return (this._loaded);
	}

	this._getFrame = function (index)
	{
		return (this._engineMaterials[index]);
	}

	this._getCurrentFrame = function ()
	{
		return (this._engineMaterials[this._currentFrame]);
	}

	this._getFrameTiming = function (index)
	{
		return (this._textureTimings[index]);
	}

	this._getNumFrames = function ()
	{
		return (this._engineMaterials.length);
	}

	this._addTime = function (time)
	{
		var iFrameTime = this._textureTimings[this._currentFrame];

		this._currentTime += time;

		if (this._currentTime >= iFrameTime)
		{
			this._currentFrame++;

			if (this._currentFrame >= this._engineMaterials.length)
				this._currentFrame = 0;

			this._currentTime = 0;
		}
	}

	this._resetTime = function ()
	{
		this._currentTime = 0;
		this._currentFrame = 0;
	}
}

