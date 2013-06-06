#ifndef _RADXML_TIMER_H_
	#define _RADXML_TIMER_H_

	#include <string>

	#include "RadXmlPreprocessor.h"

	namespace RadXml
	{
		class defRADXML_EXPORT Timer
		{
			public:
				inline Timer (std::string js, unsigned int milliseconds)
				{
					javascript = js;
					current = 0;
					end = milliseconds;
				}

				inline void setJavascript (std::string js)
				{
					javascript = js;
				}

				inline std::string getJavascript ()
				{
					return (javascript);
				}

				inline void addTime (unsigned int time)
				{
					current += time;
				}

				inline unsigned int getTime ()
				{
					return (current);
				}

				inline void setEnd (unsigned int time)
				{
					end = time;
				}

				inline unsigned int getEnd ()
				{
					return (end);
				}

				bool update (unsigned int time);

			protected:
				unsigned int current;
				unsigned int end;
				std::string javascript;
		};
	}
#endif

