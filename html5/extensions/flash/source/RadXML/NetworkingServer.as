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
	import flash.net.ServerSocket;
	import flash.net.Socket;
	import flash.events.ServerSocketConnectEvent;
	import flash.events.ProgressEvent;

	public class NetworkingServer
	{
		protected var port: int;
		protected var sSocket: ServerSocket = null;
		protected var aryClients: Array = new Array ();

		public function listen (newPort: int): void
		{
			port = newPort;

			if (ServerSocket.isSupported == false)
				throw new Error ("NetworkingServer is not supported in this version of RadXML!");

			sSocket = new ServerSocket ();
			sSocket.bind (port, "127.0.0.1");
			sSocket.addEventListener (ServerSocketConnectEvent.CONNECT, clientConnected);
			sSocket.listen ();
		}

		public function clientConnected (event: ServerSocketConnectEvent)
		{
			var sSocket: Socket = event.socket;
			sSocket.addEventListener (ProgressEvent.SOCKET_DATA, clientDataReceived);
			radXml.networkServerOnAccept ();
		}

		public function clientDataReceived (event: ProgressEvent)
		{
			for (var iIdx: int = 0; iIdx < aryClients.length; iIdx++)
			{
				var buffer: ByteArray = new ByteArray ();
				var sSocket: Socket = aryClients[iIdx];
				sSocket.readBytes (buffer, 0, sSocket.bytesAvailable);
			}
		}
	}
}

