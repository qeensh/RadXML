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
var Networking = 
{
	ActiveServers: new Array (), 
	ActiveClients: new Array ()
};

function NetworkClient ()
{
	this._port = "";
	this._serverIP = "";
	this._clientIndex = 0;

	this.onConnected = null;
	this.onReceived = null;
	this.onConnectionLost = null;

	this.connect = function (port, serverIP)
	{
		this._port = port;
		this._serverIP = serverIP;
		this._clientIndex = Networking.ActiveClients.length;

		RadXML._flashObject._NetworkClientCreateSocket (this._clientIndex);
		RadXML._flashObject._NetworkClientConnect (this._clientIndex, port, serverIP);
		Networking.ActiveClients.push (this);
	}

	this.send = function (data)
	{
		RadXML._flashObject._NetworkClientSend (this._clientIndex, data);
	}

	this.isConnected = function ()
	{
		try
		{
			return (RadXML._flashObject._NetworkClientIsConnected (this._clientIndex));
		}
		catch (ex)
		{
		}

		return (false);
	}

	this.getServerIP = function ()
	{
		return (this._serverIP);
	}

	this.close = function ()
	{
		RadXML._flashObject._NetworkClientClose (this._clientIndex);
	}
}

