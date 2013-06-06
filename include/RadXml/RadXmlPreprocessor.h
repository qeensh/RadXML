#ifndef _RADXML_PREPROCESSOR_H_
	#define _RADXML_PREPROCESSOR_H_

	#ifdef defLIB_STATIC
		#define defRADXML_EXPORT
	#else
		#ifdef defLIB_DLL
			#define defRADXML_EXPORT	__declspec(dllexport)
		#else
			#define defRADXML_EXPORT	__declspec(dllimport)
		#endif
	#endif

	#define defRADXML_PLUGIN			__declspec(dllexport)
#endif

