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

package RadXML
{
	import flash.external.*;
	import flash.net.Socket;
	import flash.events.*;
	import flash.events.ProgressEvent;
	import flash.system.Security;

	public class NetworkingClient
	{
		protected var port: int;
		protected var address: String;
		protected var socket: Socket;
		protected var clientIndex: int;
		protected var radXml: RadXml;

		public function createSocket (newRadXml: RadXml): void
		{
			radXml = newRadXml;

			socket = new Socket ();
			socket.addEventListener (Event.CONNECT, onConnected);
			socket.addEventListener (ProgressEvent.SOCKET_DATA, onReceived);
			socket.addEventListener (Event.CLOSE, onConnectionLost);
			socket.addEventListener (IOErrorEvent.IO_ERROR, onIOError);
			socket.addEventListener (SecurityErrorEvent.SECURITY_ERROR, onSecurityError);
		}

		public function connect (newClientIndex: int, newPort: int, newAddress: String): Boolean
		{
			port = newPort;
			address = newAddress;
			clientIndex = newClientIndex;

			try
			{
				socket.connect (address, port);
			}
			catch (ex: Error)
			{
				radXml.logMessage (ex.message);
			}

			return (true);
		}

		public function onIOError (event: IOErrorEvent): void
		{
			radXml.logMessage (event.toString ());
		}

		public function onSecurityError (event: SecurityErrorEvent): void
		{
			radXml.logMessage (event.toString ());
		}

		public function send (text: String): void
		{
			try
			{
				socket.writeUTFBytes (text);
				socket.flush ();
			}
			catch (ex: Error)
			{
				radXml.logMessage (ex.message);
			}
		}

		public function isConnected (): Boolean
		{
			return (socket.connected);
		}

		public function close (): void
		{
			socket.close ();
		}

		public function onConnected (event: Event): void
		{
			ExternalInterface.call ("RadXML._networkClientOnConnected", clientIndex);
		}

		public function onReceived (event: ProgressEvent): void
		{
			var strText: String = "";

			try
			{
				while (socket.bytesAvailable)
					strText += socket.readUTFBytes (socket.bytesAvailable);
			}
			catch (ex: Error)
			{
				radXml.logMessage (ex.message);
			}

			ExternalInterface.call ("RadXML._networkClientOnReceived", clientIndex, strText);
		}

		public function onConnectionLost (): void
		{
		}
	}
}

