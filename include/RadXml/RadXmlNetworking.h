#ifndef _RADXML_NETWORKING_H_
	#define _RADXML_NETWORKING_H_

	namespace RadXml
	{
		class Variable;

		#ifdef defNET_ON
		class NetworkServerClass
		{
			public:
				static void setupNetworkServerClass ();

				static Variable *listen (Variable *thisObject, Variable *arguments);
				static Variable *send (Variable *thisObject, Variable *arguments);
				static Variable *sendToAll (Variable *thisObject, Variable *arguments);
				static Variable *getIPs (Variable *thisObject, Variable *arguments);
				static Variable *getNumIPs (Variable *thisObject, Variable *arguments);
				static Variable *disconnectClient (Variable *thisObject, Variable *arguments);
				static Variable *close (Variable *thisObject, Variable *arguments);
		};

		class NetworkClientClass
		{
			public:
				static void setupNetworkClientClass ();

				static Variable *connect (Variable *thisObject, Variable *arguments);
				static Variable *send (Variable *thisObject, Variable *arguments);
				static Variable *getServerIP (Variable *thisObject, Variable *arguments);
				static Variable *close (Variable *thisObject, Variable *arguments);
		};
		#endif
	}
#endif

