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

var Key = 
{
	UNASSIGNED : 0x00,
	ESCAPE : 0x01,
	_1 : 0x02,
	_2 : 0x03,
	_3 : 0x04,
	_4 : 0x05,
	_5 : 0x06,
	_6 : 0x07,
	_7 : 0x08,
	_8 : 0x09,
	_9 : 0x0A,
	_0 : 0x0B,
	MINUS : 0x0C, // - on main keyboard
	EQUALS : 0x0D,
	BACK : 0x0E, // backspace
	TAB : 0x0F,
	Q : 0x10,
	W : 0x11,
	E : 0x12,
	R : 0x13,
	T : 0x14,
	Y : 0x15,
	U : 0x16,
	I : 0x17,
	O : 0x18,
	P : 0x19,
	LBRACKET : 0x1A,
	RBRACKET : 0x1B,
	RETURN : 0x1C, // Enter on main keyboard
	LCONTROL : 0x1D,
	A : 0x1E,
	S : 0x1F,
	D : 0x20,
	F : 0x21,
	G : 0x22,
	H : 0x23,
	J : 0x24,
	K : 0x25,
	L : 0x26,
	SEMICOLON : 0x27,
	APOSTROPHE : 0x28,
	GRAVE : 0x29, // accent
	LSHIFT : 0x2A,
	BACKSLASH : 0x2B,
	Z : 0x2C,
	X : 0x2D,
	C : 0x2E,
	V : 0x2F,
	B : 0x30,
	N : 0x31,
	M : 0x32,
	COMMA : 0x33,
	PERIOD : 0x34, // . on main keyboard
	SLASH : 0x35, // / on main keyboard
	RSHIFT : 0x36,
	MULTIPLY : 0x37, // * on numeric keypad
	LMENU : 0x38, // left Alt
	SPACE : 0x39,
	CAPITAL : 0x3A,
	F1 : 0x3B,
	F2 : 0x3C,
	F3 : 0x3D,
	F4 : 0x3E,
	F5 : 0x3F,
	F6 : 0x40,
	F7 : 0x41,
	F8 : 0x42,
	F9 : 0x43,
	F10 : 0x44,
	NUMLOCK : 0x45,
	SCROLL : 0x46, // Scroll Lock
	NUMPAD7 : 0x47,
	NUMPAD8 : 0x48,
	NUMPAD9 : 0x49,
	SUBTRACT : 0x4A, // - on numeric keypad
	NUMPAD4 : 0x4B,
	NUMPAD5 : 0x4C,
	NUMPAD6 : 0x4D,
	ADD : 0x4E, // + on numeric keypad
	NUMPAD1 : 0x4F,
	NUMPAD2 : 0x50,
	NUMPAD3 : 0x51,
	NUMPAD0 : 0x52,
	DECIMAL : 0x53, // . on numeric keypad
	OEM_102 : 0x56, // < > | on UK/Germany keyboards
	F11 : 0x57,
	F12 : 0x58,
	F13 : 0x64, // (NEC PC98)
	F14 : 0x65, // (NEC PC98)
	F15 : 0x66, // (NEC PC98)
	KANA : 0x70, // (Japanese keyboard)
	ABNT_C1 : 0x73, // / ? on Portugese (Brazilian) keyboards
	CONVERT : 0x79, // (Japanese keyboard)
	NOCONVERT : 0x7B, // (Japanese keyboard)
	YEN : 0x7D, // (Japanese keyboard)
	ABNT_C2 : 0x7E, // Numpad . on Portugese (Brazilian) keyboards
	NUMPADEQUALS: 0x8D, // : on numeric keypad (NEC PC98)
	PREVTRACK : 0x90, // Previous Track (CIRCUMFLEX on Japanese keyboard)
	AT : 0x91, // (NEC PC98)
	COLON : 0x92, // (NEC PC98)
	UNDERLINE : 0x93, // (NEC PC98)
	KANJI : 0x94, // (Japanese keyboard)
	STOP : 0x95, // (NEC PC98)
	AX : 0x96, // (Japan AX)
	UNLABELED : 0x97, // (J3100)
	NEXTTRACK : 0x99, // Next Track
	NUMPADENTER : 0x9C, // Enter on numeric keypad
	RCONTROL : 0x9D,
	MUTE : 0xA0, // Mute
	CALCULATOR : 0xA1, // Calculator
	PLAYPAUSE : 0xA2, // Play / Pause
	MEDIASTOP : 0xA4, // Media Stop
	VOLUMEDOWN : 0xAE, // Volume -
	VOLUMEUP : 0xB0, // Volume +
	WEBHOME : 0xB2, // Web home
	NUMPADCOMMA : 0xB3, // , on numeric keypad (NEC PC98)
	DIVIDE : 0xB5, // / on numeric keypad
	SYSRQ : 0xB7,
	RMENU : 0xB8, // right Alt
	PAUSE : 0xC5, // Pause
	HOME : 0xC7, // Home on arrow keypad
	UP : 0xC8, // UpArrow on arrow keypad
	PGUP : 0xC9, // PgUp on arrow keypad
	LEFT : 0xCB, // LeftArrow on arrow keypad
	RIGHT : 0xCD, // RightArrow on arrow keypad
	END : 0xCF, // End on arrow keypad
	DOWN : 0xD0, // DownArrow on arrow keypad
	PGDOWN : 0xD1, // PgDn on arrow keypad
	INSERT : 0xD2, // Insert on arrow keypad
	DELETE : 0xD3, // Delete on arrow keypad
	LWIN : 0xDB, // Left Windows key
	RWIN : 0xDC, // Right Windows key
	APPS : 0xDD, // AppMenu key
	POWER : 0xDE, // System Power
	SLEEP : 0xDF, // System Sleep
	WAKE : 0xE3, // System Wake
	WEBSEARCH : 0xE5, // Web Search
	WEBFAVORITES: 0xE6, // Web Favorites
	WEBREFRESH : 0xE7, // Web Refresh
	WEBSTOP : 0xE8, // Web Stop
	WEBFORWARD : 0xE9, // Web Forward
	WEBBACK : 0xEA, // Web Back
	MYCOMPUTER : 0xEB, // My Computer
	MAIL : 0xEC, // Mail
	MEDIASELECT : 0xED // Media Select
};