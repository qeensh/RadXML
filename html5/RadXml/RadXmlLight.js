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

var LightTypes = 
{
	LT_POINT: 1, 
	LT_DIRECTIONAL: 2, 
	LT_SPOTLIGHT: 3
};

function Light (name)
{
	this._type = LightTypes.LT_POINT;
	this._diffuse = ColourTypes.White;
	this._specular = ColourTypes.White;
	this._intensity = 1;
	this._attenuation = 100;

	this._eObj = null;

	this._parseXML = function (level, element, parent)
	{
		this._parseMovableXML (level, element, parent);

		var strType = RadXML._getAttribute (element, "type").value;
		var strDiffuse = RadXML._getAttribute (element, "diffuse").value;
		var strSpecular = RadXML._getAttribute (element, "specular").value;
		var strIntensity = RadXML._getAttribute (element, "intensity").value;
		var strAttenuation = RadXML._getAttribute (element, "attenuation").value;

		if (strType != "")
		{
			strType = strType.toLowerCase ();

			if (strType == "point")
				this._type = LightTypes.LT_POINT;

			if (strType == "directional")
				this._type = LightTypes.LT_DIRECTIONAL;

			if (strType == "spotlight")
				this._type = LightTypes.LT_SPOTLIGHT;
		}

		if (strDiffuse != "")
			this._diffuse = RadXML._parseColourValue (strDiffuse);

		if (strSpecular != "")
			this._specular = RadXML._parseColourValue (strSpecular);

		if (strIntensity != "")
			this._intensity = RadXML._parseFloat (strIntensity);

		if (strAttenuation != "")
			this._attenuation = RadXML._parseFloat (strAttenuation);
	}

	this._create = function (parent)
	{
		this._eObj = RadXML._gameEngine.createLight (this);

		this._setParent (parent);
		this._createSceneNode (this._eObj, parent);
	}
}

Light.prototype = new Movable ();

