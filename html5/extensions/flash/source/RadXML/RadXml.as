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

	public class RadXml
	{
		protected var aryServers: Array = new Array ();
		protected var aryClients: Array = new Array ();

		public function initialize (): void
		{
			//ExternalInterface.addCallback ("_NetworkServerListen", networkServerListen);
			ExternalInterface.addCallback ("_NetworkClientCreateSocket", networkClientCreateSocket);
			ExternalInterface.addCallback ("_NetworkClientConnect", networkClientConnect);
			ExternalInterface.addCallback ("_NetworkClientIsConnected", networkClientIsConnected);
			ExternalInterface.addCallback ("_NetworkClientSend", networkClientSend);
			ExternalInterface.addCallback ("_NetworkClientClose", networkClientClose);

			ExternalInterface.call ("RadXML._flashReady", true);
		}

		public function alert (message: String): void
		{
			ExternalInterface.call ("alert", message);
		}

		public function logMessage (message: String): void
		{
			ExternalInterface.call ("logMessage", message);
		}

		public function networkClientCreateSocket (clientIndex: int): void
		{
			var netClient: NetworkingClient = new NetworkingClient ();
			netClient.createSocket (this);
			aryClients.push (netClient);
		}

		public function networkClientConnect (clientIndex: int, port: int, address: String): void
		{
			aryClients[clientIndex].connect (clientIndex, port, address);
		}

		public function networkClientIsConnected (clientIndex: int): Boolean
		{
			return (aryClients[clientIndex].isConnected ());
		}

		public function networkClientSend (clientIndex: int, text: String): void
		{
			aryClients[clientIndex].send (text);
		}

		public function networkClientClose (clientIndex: int): void
		{
			aryClients[clientIndex].close ();
		}

		/*public function networkServerListen (port: int): void
		{
			try
			{
				var netServer = new NetworkingServer ();
				netServer.listen (port);
				aryServers.push (netServer);
			}
			catch (ex: Error)
			{
				logMessage (ex.message);
			}
		}

		public function networkServerOnAccept ()
		{
		}*/
	}
}

