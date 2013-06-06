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

var AppTypes = 
{
	AT_NATIVE: 1,
	AT_RENDER: 2
};

var AppObjectTypes = 
{
	AOT_WINDOW: 1,
	AOT_MENU: 2,
	AOT_MENU_ITEM: 3,
	AOT_LABEL: 4,
	AOT_TEXT_BOX: 5,
	AOT_COMBO_BOX: 6,
	AOT_BUTTON: 7,
	AOT_IMAGE_BOX: 8,
	AOT_CHECK_BOX: 9,
	AOT_GROUP_BOX: 10,
	AOT_SELECT_LIST: 11, 
	AOT_RADIO_BUTTON: 12, 
	AOT_SELECT_LIST_GRID: 13, 
	AOT_SELECT_LIST_GRID_HEADER_ITEM: 14, 
	AOT_SELECT_LIST_GRID_ITEM: 15, 
	AOT_COMBO_BOX_ITEM: 16
};

var App = 
{
	WINDOW: AppObjectTypes.AOT_WINDOW, 
	MENU: AppObjectTypes.AOT_MENU, 
	MENU_ITEM: AppObjectTypes.AOT_MENU_ITEM, 
	LABEL: AppObjectTypes.AOT_LABEL, 
	TEXT_BOX: AppObjectTypes.AOT_TEXT_BOX, 
	COMBO_BOX: AppObjectTypes.AOT_COMBO_BOX, 
	BUTTON: AppObjectTypes.AOT_BUTTON, 
	IMAGE_BOX: AppObjectTypes.AOT_IMAGE_BOX, 
	CHECK_BOX: AppObjectTypes.AOT_CHECK_BOX, 
	GROUP_BOX: AppObjectTypes.AOT_GROUP_BOX, 
	SELECT_LIST: AppObjectTypes.AOT_SELECT_LIST, 
	RADIO_BUTTON: AppObjectTypes.AOT_RADIO_BUTTON, 
	SELECT_LIST_GRID: AppObjectTypes.AOT_SELECT_LIST_GRID, 
	SELECT_LIST_GRID_HEADER_ITEM: AppObjectTypes.AOT_SELECT_LIST_GRID_HEADER_ITEM, 
	SELECT_LIST_GRID_ITEM: AppObjectTypes.AOT_SELECT_LIST_GRID_ITEM, 
	COMBO_BOX_ITEM: AppObjectTypes.AOT_COMBO_BOX_ITEM, 

	Locations: new Array (), 
	AppObjects: new Array (),

	_locationTimerObject: null, 

	_parseApp: function (xmlFile, type, xmlElement)
	{
		if (xmlElement.childNodes.length > 0)
			this._parseXMLAppObjects (type, xmlElement.childNodes[0]);
	}, 

	_parseXMLAppObjects: function (creationType, element, parent)
	{
		for (var iIdx = 0; iIdx < 16; iIdx++)
		{
			var strType = "";
			var iType = (iIdx + 1);

			switch (iIdx)
			{
				case 0:
					strType = "window";
					break;
				case 1:
					strType = "menu";
					break;
				case 2:
					strType = "menuitem";
					break;
				case 3:
					strType = "label";
					break;
				case 4:
					strType = "textbox";
					break;
				case 5:
					strType = "combobox";
					break;
				case 6:
					strType = "button";
					break;
				case 7:
					strType = "imagebox";
					break;
				case 8:
					strType = "checkbox";
					break;
				case 9:
					strType = "groupbox";
					break;
				case 10:
					strType = "selectlist";
					break;
				case 11:
					strType = "radiobutton";
					break;
				case 12:
					strType = "selectlistgrid";
					break;
				case 13:
					strType = "selectlistgridheaderitem";
					break;
				case 14:
					strType = "selectlistgriditem";
					break;
				case 15:
					strType = "comboboxitem";
					break;
			}

			var aryTags = RadXML._getElementsByTagName (element, strType, true);

			for (var iJdx = 0; iJdx < aryTags.length; iJdx++)
			{
				var xmlElement = aryTags[iJdx];

				this._parseXMLAppObject (creationType, iType, xmlElement, parent);
			}
		}
	}, 

	_parseXMLAppObject: function (creationType, type, element, parent)
	{
		var objObject = this._createAppObject (this, creationType, type, "", parent, element);

		if (element.childNodes.length > 0)
			this._parseXMLAppObjects (creationType, element.childNodes[0], objObject);
	}, 

	_createAppObject: function (file, creationType, type, name, parent, element)
	{
		var objObject = null;

		if (type == AppObjectTypes.AOT_WINDOW)
		{
			var wWindow = new Window (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_MENU)
		{
			var wWindow = new Menu (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_MENU_ITEM)
		{
			var wWindow = new MenuItem (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_LABEL)
		{
			var wWindow = new Label (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_TEXT_BOX)
		{
			var wWindow = new TextBox (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_COMBO_BOX)
		{
			var wWindow = new ComboBox (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_BUTTON)
		{
			var wWindow = new Button (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_IMAGE_BOX)
		{
			var wWindow = new ImageBox (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_CHECK_BOX)
		{
			var wWindow = new CheckBox (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_GROUP_BOX)
		{
			var wWindow = new GroupBox (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_SELECT_LIST)
		{
			var wWindow = new SelectList (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_RADIO_BUTTON)
		{
			var wWindow = new RadioButton (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_SELECT_LIST_GRID)
		{
			var wWindow = new SelectListGrid (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_SELECT_LIST_GRID_HEADER_ITEM)
		{
			var wWindow = new SelectListGridHeaderItem (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_SELECT_LIST_GRID_ITEM)
		{
			var wWindow = new SelectListGridItem (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (type == AppObjectTypes.AOT_COMBO_BOX_ITEM)
		{
			var wWindow = new ComboBoxItem (name);

			if (element != null)
				wWindow._parseXML (file, element, parent);

			wWindow._create (creationType, parent);
			objObject = wWindow;
		}

		if (objObject != null)
			this._addAppObject (objObject);

		return (objObject);
	}, 

	_addLocation: function (newAppObject)
	{
		var iIndex = this.Locations.length;
		this.Locations.push (newAppObject);

		if (this._locationTimerObject == null)
		{
			var timer = setTimeout ("RadXML._xmlFile.app._updateLocations ();", 10);
			this._locationTimerObject = timer;
		}

		return (iIndex);
	}, 

	_removeLocation: function (index)
	{
		this.Locations.splice (index, 1);

		if (this.Locations.length < 1)
		{
			clearTimeout (this._locationTimerObject);
			this._locationTimerObject = null;
		}
	}, 

	_getLocation: function (index)
	{
		return (this.Locations[index]);
	}, 

	_updateLocations: function ()
	{
		for (var iIdx = 0; iIdx < this.Locations.length; iIdx++)
			this.Locations[iIdx]._update ();

		if (this._locationTimerObject != null)
			timer = setTimeout ("RadXML._xmlFile.app._updateLocations ();", 10);
	}, 

	_addAppObject: function (newAppObject)
	{
		this.AppObjects[newAppObject.getName ()] = newAppObject;
	}, 

	_getAppObject: function (findName, skipErrorMessage)
	{
		if (skipErrorMessage == null)
			skipErrorMessage = false;

		var obj = this.AppObjects[name];

		if (obj == null)
		{
			if (skipErrorMessage == false)
				logMessage (RadXmlLanguage.MSG35 + findName);
		}

		return (obj);
	}, 

	getAppObjectByName: function (name)
	{
		return (this.AppObjects[name]);
	},

	createObject: function (renderType, name, parent)
	{
	},

	deleteObject: function (name)
	{
	}
};