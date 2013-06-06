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

var GameRenderSystemTypes = 
{
	WEBGL: 1, 
	CONTEXT2D: 2, 
	ANYAVAILABLE: 3
};

function GameEngine ()
{
	this._renderSystem = null;
	this._renderSystemType = GameRenderSystemTypes.ANYAVAILABLE;
	this._renderCanvas = undefined;
	this._aspectRatio = parseFloat (RadXML._getActualWidth ()) / 
							parseFloat (RadXML._getActualHeight ());

	this._camMain = null;
	this._mSceneMgr = null;
	this._projector = null;

	this._models = new Object ();
	this._materials = new Object ();
	this._animateMaterials = new Array ();
	this._numModelsToLoad = 0;

	this._clock = null;

	this.initialize = function ()
	{
		if (this._renderSystemType == GameRenderSystemTypes.WEBGL)
		{
			this._renderSystem = new THREE.WebGLRenderer({
				canvas: this._renderCanvas, 
				alpha: false
			});
		}

		if (this._renderSystemType == GameRenderSystemTypes.CONTEXT2D)
		{
			this._renderSystem = new THREE.CanvasRenderer({
				canvas: this._renderCanvas, 
				alpha: false
			});
		}

		if (this._renderSystemType == GameRenderSystemTypes.ANYAVAILABLE)
		{
			try
			{
				this._renderSystem = new THREE.WebGLRenderer({
					canvas: this._renderCanvas, 
					alpha: false
				});
			}
			catch (ex)
			{
				logMessage (RadXmlLanguage.MSG34);
			}

			if (this._renderSystem == null)
			{
				this._renderSystem = new THREE.CanvasRenderer({
					canvas: this._renderCanvas, 
					alpha: false
				});
			}
		}

		if (this._renderSystem == null)
			throw RadXmlLanguage.MSG33;

		this._renderSystem.setClearColor (0x000000);
		this._setupDefaultCameras ();
		this._setupDefaultSceneManager ();
		this._renderSystem.setSize (RadXML._screenWidth, RadXML._screenHeight);

		if (this._renderCanvas == null)
		{
			document.body.style.margin = "0px";
			document.body.style.padding = "0px";
		}

		document.body.appendChild (this._renderSystem.domElement);
	}

	this.setCanvas = function (canvas)
	{
		this._renderCanvas = canvas;
	}

	this.addResourceLocation = function (dir, arch, type, loadNow)
	{
		
	}

	this.loadModel = function (model)
	{
		this._numModelsToLoad++;

		var jsonLoader = new THREE.JSONLoader ();
		jsonLoader.load (model._src, RadXML._keepContext (this._addModel, this, model));
	}

	this._addModel = function (geometry, materials, model)
	{
		this._numModelsToLoad--;

		var mMaterial = new THREE.MeshFaceMaterial (materials);
		var mMesh = new THREE.Mesh (geometry, mMaterial);
		mMesh.radXmlModel = model;
		mMesh.name = model._name;
		this._models[model._name] = mMesh;

		if (this._numModelsToLoad == 0)
			Game._continueParsing ();
	}

	this.loadMaterial = function (material)
	{
		var strName = material._name;
		var bAnimates = false;

		if (strName == "")
			return;

		if (material._cubicTextures != "")
		{
			var tTexture = THREE.ImageUtils.loadTextureCube (material._cubicTextures);
			material._addCubicTexture (tTexture);
		}
		else
		{
			if (material._animationImages.length == 0)
			{
				var tTexture = THREE.ImageUtils.loadTexture (material._image);
				material._addTexture (tTexture);
			}
			else
			{
				var dFrameTime = (material._animationTime / material._animationImages.length);

				for (var iIdx = 0; iIdx < material._animationImages.length; iIdx++)
				{
					var tTexture = THREE.ImageUtils.loadTexture (material._animationImages[iIdx]);
					material._addTexture (tTexture, dFrameTime);
				}

				bAnimates = true;
			}
		}

		material._loaded = true;
		this.createMaterial (material, bAnimates);
	}

	this.createMaterial = function (material, animates)
	{
		var strName = material._name;
		var aryTextures = null;
		var bAlphas = material._alphas;
		var bIsCubicTextures = false;

		if (material._cubicTextures.length > 0)
		{
			aryTextures = material._cubicTextures;
			bIsCubicTextures = true;
		}
		else
			aryTextures = material._textures;

		for (var iIdx = 0; iIdx < aryTextures.length; iIdx++)
		{
			var tTexture = aryTextures[iIdx];
			var mMaterial = null;

			if (bIsCubicTextures == true)
			{
				var sShader = THREE.ShaderUtils.lib["cube"];
				sShader.uniforms["tCube"].texture = tTexture;
				mMaterial = new THREE.MeshShaderMaterial ({
						uniforms: sShader.uniforms, 
						fragmentShader: sShader.fragmentShader, 
						vertexShader: sShader.vertexShader
					});
			}
			else
			{
				mMaterial = new THREE.MeshBasicMaterial ({
						color: 0xFFFFFF, 
						map: tTexture, 
						transparent: bAlphas, 
						depthWrite: !bAlphas
					});
			}

			mMaterial.radXmlMaterial = material;
			this._addMaterial (strName, mMaterial, material, animates);
		}

		if (animates == true)
			this._animateMaterials.push (material);
	}

	this.getImageFromMaterial = function (material, frameIndex)
	{
		if (frameIndex == null)
			frameIndex = 0;

		return (material._textures[frameIndex]);
	}

	this._addMaterial = function (name, material, radXmlMaterial)
	{
		if (this._materials[name] == null)
		{
			this._materials[name] = new Array ();
			this._materials[name]._current = material;
		}

		this._materials[name].push (material);
		radXmlMaterial._addMaterial (material);
	}

	this.getModel = function (name, giveWarning)
	{
		var obj = this._models[name];

		if (obj == null)
		{
			if (giveWarning == null)
				giveWarning = true;

			if (giveWarning == true)
				logMessage (RadXmlLanguage.MSG44 + name);

			return (null);
		}

		return (obj);
	}

	this._getMaterial = function (name, index)
	{
		var obj = this._materials[name];
		var objReturn = null;

		if (obj == null)
		{
			logMessage (RadXmlLanguage.MSG36 + name);

			return (null);
		}

		if (index == null)
			objReturn = obj._current;
		else
			objReturn = obj[index];

		return (objReturn);
	}

	this._setupDefaultCameras = function ()
	{
		this._camMain = new THREE.PerspectiveCamera ((90 / this._aspectRatio), 
			this._aspectRatio, 1.0, 10000000000.0);
		this._projector = new THREE.Projector ();
	}

	this.setCamera = function (movableObject)
	{
		this._camMain = new THREE.PerspectiveCamera (
			(90 / this._aspectRatio), this._aspectRatio, 
			movableObject._nearClipDistance, movableObject._farClipDistance);
		this._projector = new THREE.Projector ();

		return (this._camMain);
	}

	this._setupDefaultSceneManager = function ()
	{
		this._mSceneMgr = new THREE.Scene ();
		this._mSceneMgr.add (new THREE.AmbientLight (0xFFFFFF));
	}

	this.setAmbientLightColour = function (colour)
	{
		this._mSceneMgr.add (new THREE.AmbientLight (colour.toHexInt ()));
	}

	this.setShadowColour = function (colour)
	{
		logMessage (RadXmlLanguage.MSG45);
	}

	this.setSkyBoxMaterial = function (material)
	{
		var mSkyBox = this.createCube ("skybox", material._name, 1000000, 1000000, 1000000);
		this._mSceneMgr.add (mSkyBox);
	}

	this.createLight = function (light)
	{
		var objLight = null;

		if (light._type == LightTypes.LT_POINT)
		{
			objLight = new THREE.PointLight (light._diffuse.toHexInt (), 
									light._intensity, light._attenuation);
		}

		if (light._type == LightTypes.LT_DIRECTIONAL)
		{
			objLight = new THREE.DirectionalLight (light._diffuse.toHexInt (), 
									light._intensity, light._attenuation);
		}

		if (light._type == LightTypes.LT_SPOTLIGHT)
		{
			objLight = new THREE.SpotLight (light._diffuse.toHexInt (), 
									light._intensity, light._attenuation);
		}

		return (objLight);
	}

	this.createCube = function (entityName, materialName, 
		width, height, depth, widthSegs, heightSegs, depthSegs)
	{
		if (width == null)
			width = 200;

		if (height == null)
			height = 200;

		if (depth == null)
			depth = 200;

		if (widthSegs == null)
			widthSegs = 1;

		if (heightSegs == null)
			heightSegs = 1;

		if (depthSegs == null)
			depthSegs = 1;

		var cgGeom = new THREE.CubeGeometry (width, height, depth, widthSegs, heightSegs, depthSegs);
		var mMat = null;

		if (materialName == "")
		{
			mMat = new THREE.MeshBasicMaterial ({
				color: 0xFFFFFF
			});
		}
		else
			mMat = this._getMaterial (materialName);

		var mMesh = new THREE.Mesh (cgGeom, mMat);
		mMesh.name = entityName;

		return (mMesh);
	}

	this.createSphere = function (entityName, materialName, radius)
	{
		if (radius == null)
			radius = 100;

		var cgGeom = new THREE.SphereGeometry (radius);
		var mMat = null;

		if (materialName == "")
		{
			mMat = new THREE.MeshBasicMaterial ({
				color: 0xFFFFFF
			});
		}
		else
			mMat = this._getMaterial (materialName);

		var mMesh = new THREE.Mesh (cgGeom, mMat);
		mMesh.name = entityName;

		return (mMesh);
	}

	this.createPlane = function (entityName, materialName, width, height, segx, segy, normal, d, upDir)
	{
		if (segx == null)
			segx = 1;

		if (segy == null)
			segy = 1;

		var cgGeom = new THREE.PlaneGeometry (width, height, segx, segy);
		var mMat = null;

		if (materialName == "")
		{
			mMat = new THREE.MeshBasicMaterial ({
				color: 0xFFFFFF
			});
		}
		else
			mMat = this._getMaterial (materialName);

		var mMesh = new THREE.Mesh (cgGeom, mMat);
		mMesh.name = entityName;

		return (mMesh);
	}

	this.createModel = function (entityName, modelName)
	{
		var obj = this._models[modelName]

		if (obj == null)
		{
			logMessage (RadXmlLanguage.MSG44 + modelName);

			return (null);
		}

		var mMesh = this._models[modelName].clone ();
		mMesh.name = entityName;

		return (mMesh);
	}

	this.createAnimationTrack = function (animationTrack)
	{
		
	}

	this.createParticleSystem = function (particleSystem)
	{
		
	}

	this.setMaterial = function (entity, materialName)
	{
		if (entity.material._name == materialName)
			return;

		var material = this._getMaterial (materialName);
		entity.material = material;
	}

	this.setPosition = function (movableObject, vector3Value)
	{
		movableObject._snNode.position.x = vector3Value.x;
		movableObject._snNode.position.y = vector3Value.y;
		movableObject._snNode.position.z = vector3Value.z;
	}

	this.getPosition = function (movableObject)
	{
		var v3dReturn = new Vector3 ();

		v3dReturn.x = movableObject._snNode.position.x;
		v3dReturn.y = movableObject._snNode.position.y;
		v3dReturn.z = movableObject._snNode.position.z;

		return (v3dReturn);
	}

	this.setScale = function (movableObject, vector3Value)
	{
		movableObject._snNode.scale.x = vector3Value.x;
		movableObject._snNode.scale.y = vector3Value.y;
		movableObject._snNode.scale.z = vector3Value.z;
	}

	this.getScale = function (movableObject)
	{
		var v3dVector = new Vector3 ();

		v3dVector.x = movableObject._snNode.scale.x;
		v3dVector.y = movableObject._snNode.scale.y;
		v3dVector.z = movableObject._snNode.scale.z;

		return (v3dVector);
	}

	this.setVisible = function (movableObject, bValue)
	{
		movableObject._snNode.visible = bValue;
	}

	this.createSceneNode = function (movableObject, object, parent)
	{
		if (parent == null)
			this._mSceneMgr.add (object);
		else
			parent._snNode.add (object);

		movableObject._snNode = object;
	}

	this.destroySceneNode = function (sceneNode)
	{
		this._mSceneMgr.remove (sceneNode);
	}

	this._moveEntities = function (timeSinceLastFrame)
	{
		for (var key in RadXML._xmlFile.game.Movables)
		{
			var obj = RadXML._xmlFile.game.Movables[key];

			if ((obj._type == GameObjectTypes.GOT_OVERLAY) || 
				(obj._type == GameObjectTypes.GOT_OVERLAY_ELEMENT))
				continue;

			obj._moveEntity (timeSinceLastFrame);
		}
	}

	this._mouseClick = function (x, y)
	{
		//var v3dMouse = new THREE.Vector3( ( event.clientX / window.innerWidth ) * 2 - 1, - ( event.clientY / window.innerHeight ) * 2 + 1, 0.5 );
		var v3dMouse = new THREE.Vector3 (((x / RadXML._getActualWidth ()) * 2 - 1), 
			(- (y / RadXML._getActualHeight ()) * 2 + 1), 0.5);
		/*var v3dMouse = new THREE.Vector3 (((x / window.innerWidth) * 2 - 1), 
			(-1 * (y / window.innerHeight) * 2 + 1), 0.5);*/
		this._projector.unprojectVector (v3dMouse, this._camMain);
		var v3dDir = v3dMouse.sub (this._camMain.position).normalize ();
		var ray = new THREE.Raycaster (this._camMain.position, v3dDir);
		var intersection = ray.intersectObjects (Game._gameObjects);

		for (var iIdx = 0; iIdx < intersection.length; iIdx++)
		{
			var obj = intersection[iIdx];

			if (obj._movable != null)
				obj._movable._executeJavascriptFromEvent ("onclick");
		}
	}

	this._checkSphereCollision = function (cube1, cube2)
	{
		if (cube1 == null)
			return (false);

		if (cube1.position == null)
			return (false);

		if (cube1.geometry == null)
			return (false);

		if (cube2 == null)
			return (false);

		if (cube2.position == null)
			return (false);

		if (cube2.geometry == null)
			return (false);

		var sSphere1 = cube1.geometry.boundingSphere.clone ();
		sSphere1.center = cube1.position;
		sSphere1.radius *= RadXML._getLargestScalarFromVector3 (cube1.scale);
		var sSphere2 = cube2.geometry.boundingSphere.clone ();
		sSphere2.center = cube2.position;
		sSphere2.radius *= RadXML._getLargestScalarFromVector3 (cube2.scale);
		var bResult = sSphere1.intersectsSphere (sSphere2);

		/*var vOriginPoint = cube1.position.clone ();

		for (var iIdx = 0; iIdx < cube1.geometry.vertices.length; iIdx++)
		{
			var vVertLocal = cube1.geometry.vertices[iIdx].clone ();
			var vVertGlobal = vVertLocal.applyMatrix4 (cube1.matrix);
			var vVertDir = vVertGlobal.sub (cube1.position);
			var vVertDirNormalize = vVertDir.clone ().normalize ();

			var rRay = new THREE.Ray (vOriginPoint, vVertDirNormalize);
			var aryCollisionResults = new Array ();//rRay.intersectObjects (cube2);

			if (aryCollisionResults.length > 0)
			{
				if (aryCollisionResults[0].distance < vVertDir.length ())
					return (true);
			}
		}*/

		return (bResult);
	}

	this._processCollisions = function (timeSinceLastFrame)
	{
		for (var key1 in RadXML._xmlFile.game.Movables)
		{
			var obj1 = RadXML._xmlFile.game.Movables[key1];

			if (obj1._type != GameObjectTypes.GOT_ENTITY)
				continue;

			if (obj1._collides == false)
				continue;

			if (obj1._events._hshEvents["oncollision"] == null)
				continue;

			for (var key2 in RadXML._xmlFile.game.Movables)
			{
				var obj2 = RadXML._xmlFile.game.Movables[key2];

				if (key1 == key2)
					continue;

				if (obj2._type != GameObjectTypes.GOT_ENTITY)
					continue;

				if (obj2._collides == false)
					continue;

				var dDistance = obj1._snNode.position.distanceTo (obj2._snNode.position);
				var dSize = obj1._snNode.geometry.boundingSphere.radius * 
					RadXML._getLargestScalarFromVector3 (obj1._snNode.scale);

				if (dDistance > dSize)
					continue;

				if (RadXML._gameEngine._checkSphereCollision (obj1._snNode, obj2._snNode) == true)
				{
					var evtEvent1 = obj1._events._hshEvents["oncollision"];

					if (obj1.onCollision != null)
						obj1.onCollision (obj1, obj2);

					if (evtEvent1 != null)
					{
						if (evtEvent1._javascript != "")
						{
							RadXML.compileAndRunJSString (evtEvent1._javascript + 
									"(Game.getMovableByName (\"" + obj1._name + 
									"\"), Game.getMovableByName (\"" + obj2._name + "\"));");
						}
					}

					var evtEvent2 = obj2._events._hshEvents["oncollision"];

					if (obj2.onCollision != null)
						obj2.onCollision (obj2, obj1);

					if (evtEvent2 != null)
					{
						if (evtEvent2._javascript != "")
						{
							RadXML.compileAndRunJSString (evtEvent2._javascript + 
									"(Game.getMovableByName (\"" + obj2._name + 
									"\"), Game.getMovableByName (\"" + obj1._name + "\"));");
						}
					}
				}
			}
		}
	}

	this._frameStarted = function (timeSinceLastFrame)
	{
		if (RadXML._gameEngine._clock == null)
			RadXML._gameEngine._clock = new THREE.Clock (true);
		else
			timeSinceLastFrame = RadXML._gameEngine._clock.getDelta ();

		window.requestAnimationFrame (RadXML._gameEngine._frameStarted);

		if (Game.onFrameStarted != null)
			Game.onFrameStarted (timeSinceLastFrame);

		RadXML._gameEngine._moveEntities (timeSinceLastFrame);

		if (Game._bHasCollisionDetection == true)
			RadXML._gameEngine._processCollisions (timeSinceLastFrame);

		for (var iIdx = 0; iIdx < RadXML._gameEngine._animateMaterials.length; iIdx++)
		{
			var objMaterialAnim = RadXML._gameEngine._animateMaterials[iIdx];

			if (objMaterialAnim._loaded == true)
			{
				objMaterialAnim._addTime (timeSinceLastFrame);
				var objMaterial = objMaterialAnim._getCurrentFrame ();
				RadXML._gameEngine._materials[objMaterialAnim._name]._current = objMaterial;
			}
		}

		RadXML._gameEngine._renderSystem.render (
			RadXML._gameEngine._mSceneMgr, RadXML._gameEngine._camMain);

		if (Game.onFrameEnded != null)
			Game.onFrameEnded (timeSinceLastFrame);
	}

	this._frameEnded = function (timeSinceLastFrame)
	{
	}

	this.getSceneManager = function ()
	{
		return (this._mSceneMgr);
	}
}

