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

function InputEngine ()
{
	this._aryMousePos = null;
	this._aryPrevMousePos = null;
	this._aryRelative = null;

	this.initialize = function ()
	{
		RadXML._addEventListener ("mousedown", RadXML._keepContext (this._onMouseDown, this));
		RadXML._addEventListener ("mouseclick", RadXML._keepContext (this._onMouseClick, this));
		RadXML._addEventListener ("mouseup", RadXML._keepContext (this._onMouseUp, this));
		RadXML._addEventListener ("mousemove", RadXML._keepContext (this._onMouseMove, this));

		RadXML._addEventListener ("keydown", RadXML._keepContext (this._onKeyDown, this));
		RadXML._addEventListener ("keypress", RadXML._keepContext (this._onKeyPressed, this));
		RadXML._addEventListener ("keyup", RadXML._keepContext (this._onKeyUp, this));
	}

	this._onKeyDown = function (event)
	{
		if (Input.onKeyDown != null)
		{
			var inInputEvent = this._getCurrentInputEvent (event);
			Input.onKeyDown (inInputEvent);
		}
	}

	this._onKeyPressed = function (event)
	{
		if (Input.onKeyPressed != null)
		{
			var inInputEvent = this._getCurrentInputEvent (event);
			Input.onKeyPressed (inInputEvent);
		}
	}

	this._onKeyUp = function (event)
	{
		if (Input.onKeyUp != null)
		{
			var inInputEvent = this._getCurrentInputEvent (event);
			Input.onKeyUp (inInputEvent);
		}
	}

	this._onMouseDown = function (event)
	{
		if (Input.onKeyUp != null)
		{
			var inInputEvent = this._getCurrentInputEvent (event);
			Input.onKeyUp (inInputEvent);
		}
	}

	this._onMouseClick = function (event)
	{
		if (Input.onMousePressed != null)
		{
			var inInputEvent = this._getCurrentInputEvent (event);
			Input.onMousePressed (inInputEvent);
		}

		if (RadXML._gameEngine != null)
			RadXML._gameEngine._mouseClick (event.clientX, event.clientY);
	}

	this._onMouseUp = function (event)
	{
		if (Input.onMouseUp != null)
		{
			var inInputEvent = this._getCurrentInputEvent (event);
			Input.onMouseUp (inInputEvent);
		}

		if (RadXML._gameEngine != null)
			RadXML._gameEngine._mouseClick (event.clientX, event.clientY);
	}

	this._onMouseMove = function (event)
	{
		this._aryPrevMousePos = this._aryMousePos;
		this._aryMousePos = [event.clientX, event.clientY];

		if (this._aryPrevMousePos == null)
			this._aryPrevMousePos = this._aryMousePos;

		this._aryRelative = [(this._aryMousePos[0] - this._aryPrevMousePos[0]), 
						(this._aryMousePos[1] - this._aryPrevMousePos[1])];

		if (Input.onMouseMoved != null)
		{
			var inInputEvent = this._getCurrentInputEvent (event);
			Input.onMouseMoved (inInputEvent);
		}
	}

	this._getCurrentInputEvent = function (event)
	{
		var inInputEvent = new InputEvent ();
		inInputEvent.key = this._translateKey (event);
		var iB1 = false;
		var iB2 = false;
		var iB3 = false;

		if (event.button == 0)
			iB1 = true;

		if (event.button == 1)
			iB2 = true;

		if (event.button == 2)
			iB3 = true;

		inInputEvent.buttons.push (iB1);
		inInputEvent.buttons.push (iB2);
		inInputEvent.buttons.push (iB3);

		inInputEvent.x = this._aryMousePos[0];
		inInputEvent.y = this._aryMousePos[1];

		return (inInputEvent);
	}

	this._translateKey = function (event)
	{
		switch (event.keyCode)
		{
			case 8:
				return (Key.BACK);
				break;
			case 9:
				return (Key.TAB);
				break;
			case 13:
				return (Key.RETURN);
				break;
			case 16:
				return (Key.LSHIFT);
				break;
			case 17:
				return (Key.LCONTROL);
				break;
			case 18:
				return (Key.LMENU);
				break;
			case 19:
				return (Key.PAUSE);
				break;
			case 20:
				return (Key.CAPITAL);
				break;
			case 27:
				return (Key.ESCAPE);
				break;
			case 32:
				return (Key.SPACE);
				break;
			case 33:
				return (Key.PGUP);
				break;
			case 34:
				return (Key.PGDOWN);
				break;
			case 35:
				return (Key.END);
				break;
			case 36:
				return (Key.HOME);
				break;
			case 37:
				return (Key.LEFT);
				break;
			case 38:
				return (Key.UP);
				break;
			case 39:
				return (Key.RIGHT);
				break;
			case 40:
				return (Key.DOWN);
				break;
			case 45:
				return (Key.INSERT);
				break;
			case 46:
				return (Key.DELETE);
				break;
			case 48:
				return (Key._0);
				break;
			case 49:
				return (Key._1);
				break;
			case 50:
				return (Key._2);
				break;
			case 51:
				return (Key._3);
				break;
			case 52:
				return (Key._4);
				break;
			case 53:
				return (Key._5);
				break;
			case 54:
				return (Key._6);
				break;
			case 55:
				return (Key._7);
				break;
			case 56:
				return (Key._8);
				break;
			case 57:
				return (Key._9);
				break;
			case 65:
				return (Key.A);
				break;
			case 66:
				return (Key.B);
				break;
			case 67:
				return (Key.C);
				break;
			case 68:
				return (Key.D);
				break;
			case 69:
				return (Key.E);
				break;
			case 70:
				return (Key.F);
				break;
			case 71:
				return (Key.G);
				break;
			case 72:
				return (Key.H);
				break;
			case 73:
				return (Key.I);
				break;
			case 74:
				return (Key.J);
				break;
			case 75:
				return (Key.K);
				break;
			case 76:
				return (Key.L);
				break;
			case 77:
				return (Key.M);
				break;
			case 78:
				return (Key.N);
				break;
			case 79:
				return (Key.O);
				break;
			case 80:
				return (Key.P);
				break;
			case 81:
				return (Key.Q);
				break;
			case 82:
				return (Key.R);
				break;
			case 83:
				return (Key.S);
				break;
			case 84:
				return (Key.T);
				break;
			case 85:
				return (Key.U);
				break;
			case 86:
				return (Key.V);
				break;
			case 87:
				return (Key.W);
				break;
			case 88:
				return (Key.X);
				break;
			case 89:
				return (Key.Y);
				break;
			case 90:
				return (Key.Z);
				break;
			case 91:
				return (Key.LWIN);
				break;
			case 92:
				return (Key.RWIN);
				break;
			case 93:
				return (Key.UNASSIGNED);	/// @note Select key
				break;
			case 96:
				return (Key.NUMPAD0);
				break;
			case 97:
				return (Key.NUMPAD1);
				break;
			case 98:
				return (Key.NUMPAD2);
				break;
			case 99:
				return (Key.NUMPAD3);
				break;
			case 100:
				return (Key.NUMPAD4);
				break;
			case 101:
				return (Key.NUMPAD5);
				break;
			case 102:
				return (Key.NUMPAD6);
				break;
			case 103:
				return (Key.NUMPAD7);
				break;
			case 104:
				return (Key.NUMPAD8);
				break;
			case 105:
				return (Key.NUMPAD9);
				break;
			case 106:
				return (Key.MULTIPLY);
				break;
			case 107:
				return (Key.ADD);
				break;
			case 109:
				return (Key.SUBTRACT);
				break;
			case 110:
				return (Key.DECIMAL);
				break;
			case 111:
				return (Key.DIVIDE);
				break;
			case 112:
				return (Key.F1);
				break;
			case 113:
				return (Key.F2);
				break;
			case 114:
				return (Key.F3);
				break;
			case 115:
				return (Key.F4);
				break;
			case 116:
				return (Key.F5);
				break;
			case 117:
				return (Key.F6);
				break;
			case 118:
				return (Key.F7);
				break;
			case 119:
				return (Key.F8);
				break;
			case 120:
				return (Key.F9);
				break;
			case 121:
				return (Key.F10);
				break;
			case 122:
				return (Key.F11);
				break;
			case 123:
				return (Key.F12);
				break;
			case 144:
				return (Key.NUMLOCK);
				break;
			case 145:
				return (Key.SCROLL);
				break;
			case 186:
				return (Key.SEMICOLON);
				break;
			case 187:
				return (Key.EQUALS);
				break;
			case 188:
				return (Key.COMMA);
				break;
			case 189:
				return (Key.MINUS);
				break;
			case 190:
				return (Key.PERIOD);
				break;
			case 191:
				return (Key.SLASH);
				break;
			case 192:
				return (Key.GRAVE);
				break;
			case 219:
				return (Key.LBRACKET);
				break;
			case 220:
				return (Key.BACKSLASH);
				break;
			case 221:
				return (Key.RBRACKET);
				break;
			case 222:
				return (Key.APOSTROPHE);
				break;
		}

		return (Key.UNASSIGNED);
	}
}

