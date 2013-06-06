#ifndef _RADXML_TEXTFILE_H_
	#define _RADXML_TEXTFILE_H_

	namespace RadXml
	{
		class Variable;

		class TextFileClass
		{
			public:
				static void setupTextFileClass ();

				static Variable *open (Variable *thisObject, Variable *arguments);
				static Variable *isOpen (Variable *thisObject, Variable *arguments);
				static Variable *write (Variable *thisObject, Variable *arguments);
				static Variable *writeln (Variable *thisObject, Variable *arguments);
				static Variable *read (Variable *thisObject, Variable *arguments);
				static Variable *readln (Variable *thisObject, Variable *arguments);
				static Variable *readToEnd (Variable *thisObject, Variable *arguments);
				static Variable *getChar (Variable *thisObject, Variable *arguments);
				static Variable *seek (Variable *thisObject, Variable *arguments);
				static Variable *peek (Variable *thisObject, Variable *arguments);
				static Variable *getPosition (Variable *thisObject, Variable *arguments);
				static Variable *isEOF (Variable *thisObject, Variable *arguments);
				static Variable *close (Variable *thisObject, Variable *arguments);
		};
	}
#endif

