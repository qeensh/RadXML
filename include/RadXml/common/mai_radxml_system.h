#ifndef _MAI_RADXML_SYSTEM_H_
	#define _MAI_RADXML_SYSTEM_H_

	#include <string>
	#include <vector>

	#ifdef defSYS_MACOSX
		#ifndef defAPI_WXWIDGETS
			#include <Cocoa/Cocoa.h>
			#include <CoreFoundation/CoreFoundation.h>
		#endif
	#endif

	#ifdef defSYS_ANDROID
		#include <jni.h>
	#endif

	#ifdef defAPI_WXWIDGETS
		#include <wx/string.h>
		#include <wx/filesys.h>
		#include <wx/filedlg.h>
		#include <wx/clipbrd.h>

		#include <stdio.h>
	#endif

	#ifdef defSYS_NO_SPECIFIC_OS
		#ifndef defSYS_WIN32
			#ifndef defSYS_LINUX
				#ifndef defSYS_MACOSX
					#include <iostream>
				#endif
			#endif
		#endif
	#endif

	namespace RadXmlCommon
	{
		#ifdef defSYS_WIN32
			#ifndef defAPI_WXWIDGETS
				void s13MsgBox (std::string strLine, std::string strTitle);
				void s13ErrorBox (std::string strLine);
				void s13LogMessage (std::string strLine);

				std::string s13OpenFile (std::string strTitle, std::string strFilter, std::string strDefExt);
				std::string s13SaveFile (std::string strTitle, std::string strFilter, std::string strDefExt);

				void s13CopyTextToClipboard (std::string strData);
				std::string s13GetTextFromClipboard ();
				void s13OpenDefaultWebBrowser (std::string strURL);

				std::vector<std::string> s13SearchForFiles (std::string strSearchFor);
				std::vector<std::string> s13SearchForDirectories (std::string strSearchFor);

				bool s13Mkdir (std::string strString);
				bool s13MoveFile (std::string strExisting, std::string strNew);
				bool s13Rm (std::string strString);
				bool s13Rmdir (std::string strString);
				bool s13CopyFile (std::string strExisting, std::string strNew, bool bFailIfExists);
				bool s13IsFile (std::string strString);
				bool s13IsDirectory (std::string strString);

				std::string s13GetHomeDirectory ();
				std::string s13GetSettingsDirectory ();
			#endif
		#endif

		#ifdef defSYS_LINUX
			#ifndef defAPI_WXWIDGETS
				void s13MsgBox (std::string strLine, std::string strTitle);
				void s13ErrorBox (std::string strLine);
				void s13LogMessage (std::string strLine);

				std::string s13OpenFile (std::string strTitle, std::string strFilter, std::string strDefExt);
				std::string s13SaveFile (std::string strTitle, std::string strFilter, std::string strDefExt);

				void s13CopyTextToClipboard (std::string strData);
				std::string s13GetTextFromClipboard ();
				void s13OpenDefaultWebBrowser (std::string strURL);

				std::vector<std::string> s13SearchForFiles (std::string strSearchFor);
				std::vector<std::string> s13SearchForDirectories (std::string strSearchFor);

				bool s13Mkdir (std::string strString);
				bool s13MoveFile (std::string strExisting, std::string strNew);
				bool s13Rm(std::string strString);
				bool s13Rmdir (std::string strString);
				bool s13CopyFile (std::string strExisting, std::string strNew, bool bFailIfExists);
				bool s13IsFile (std::string strString);
				bool s13IsDirectory (std::string strString);

				std::string s13GetHomeDirectory ();
				std::string s13GetSettingsDirectory ();
			#endif
		#endif

		#ifdef defSYS_MACOSX
			#ifndef defAPI_WXWIDGETS
				void s13MsgBox (std::string strLine, std::string strTitle);
				void s13ErrorBox (std::string strLine);
				void s13LogMessage (std::string strLine);

				std::string s13OpenFile (std::string strTitle, std::string strFilter, std::string strDefExt);
				std::string s13SaveFile (std::string strTitle, std::string strFilter, std::string strDefExt);

				void s13CopyTextToClipboard (std::string strData);
				std::string s13GetTextFromClipboard ();
				void s13OpenDefaultWebBrowser (std::string strURL);

				std::vector<std::string> s13SearchForFiles (std::string strSearchFor);
				std::vector<std::string> s13SearchForDirectories (std::string strSearchFor);

				bool s13Mkdir (std::string strString);
				bool s13MoveFile (std::string strExisting, std::string strNew);
				bool s13Rm(std::string strString);
				bool s13Rmdir (std::string strString);
				bool s13CopyFile (std::string strExisting, std::string strNew, bool bFailIfExists);
				bool s13IsFile (std::string strString);
				bool s13IsDirectory (std::string strString);

				std::string s13GetHomeDirectory ();
				std::string s13GetSettingsDirectory ();

				std::string convertNSStringToString (NSString *string);
			#endif
		#endif

		#ifdef defSYS_IPHONE
			void s13MsgBox (std::string strLine, std::string strTitle);
			void s13ErrorBox (std::string strLine);
			void s13LogMessage (std::string strLine);

			std::string s13OpenFile (std::string strTitle, std::string strFilter, std::string strDefExt);
			std::string s13SaveFile (std::string strTitle, std::string strFilter, std::string strDefExt);

			void s13CopyTextToClipboard (std::string strData);
			std::string s13GetTextFromClipboard ();
			void s13OpenDefaultWebBrowser (std::string strURL);

			std::vector<std::string> s13SearchForFiles (std::string strSearchFor);
			std::vector<std::string> s13SearchForDirectories (std::string strSearchFor);

			bool s13Mkdir (std::string strString);
			bool s13MoveFile (std::string strExisting, std::string strNew);
			bool s13Rm(std::string strString);
			bool s13Rmdir (std::string strString);
			bool s13CopyFile (std::string strExisting, std::string strNew, bool bFailIfExists);
			bool s13IsFile (std::string strString);
			bool s13IsDirectory (std::string strString);

			std::string s13GetHomeDirectory ();
			std::string s13GetSettingsDirectory ();

			std::string convertNSStringToString (NSString *string);
		#endif

		#ifdef defSYS_ANDROID
			void s13MsgBox (std::string strLine, std::string strTitle);
			void s13ErrorBox (std::string strLine);

			void s13OpenDefaultWebBrowser (std::string strURL);

			bool s13Mkdir (std::string strString);
			bool s13MoveFile (std::string strExisting, std::string strNew);
			bool s13Rm(std::string strString);
			bool s13Rmdir (std::string strString);
			std::vector<std::string> s13ListAllFilesInDirectory (std::string directory);
			bool s13CopyFile (std::string strExisting, std::string strNew, bool bFailIfExists);
			bool s13CopyAllFilesToDirectory (std::string strExisting, std::string strNew, bool bFailIfExists);
			bool s13IsFile (std::string strString);
			bool s13IsDirectory (std::string strString);

			std::string s13GetHomeDirectory ();

			std::string convertJStringToString (jstring stringToConvert);

			class JavaJNI
			{
				public:
					inline JavaJNI ()
					{
						env = 0;
						javaClass = 0;
					}

					inline JavaJNI (JNIEnv *envpass, jclass classpass)
					{
						env = envpass;
						javaClass = classpass;
					}

					inline void setJNI (JNIEnv *envpass, jclass classpass)
					{
						env = envpass;
						javaClass = classpass;
					}

					inline void setEnvironment (JNIEnv *envpass)
					{
						env = envpass;
					}

					inline JNIEnv *getEnvironment ()
					{
						return (env);
					}

					inline void setClass (jclass classpass)
					{
						javaClass = classpass;
					}

					inline jclass getClass ()
					{
						return (javaClass);
					}

				protected:
					JNIEnv *env;
					jclass javaClass;
			};

			extern JavaJNI *jniMain;

			extern "C"
			{
				jint JNI_OnLoad(JavaVM *vm, void *reserved);
				void JNI_OnUnload(JavaVM *vm, void *reserved);

				JNIEXPORT void JNICALL Java_com_highersoftware_Common_createJNI (JNIEnv *env, jclass javaClass);
				JNIEXPORT void JNICALL Java_com_highersoftware_Common_destroyJNI (JNIEnv *env, jclass javaClass);
			}
		#endif

		#ifdef defAPI_WXWIDGETS
			void s13MsgBox (std::string strLine, std::string strTitle);
			void s13ErrorBox (std::string strLine);
			void s13LogMessage (std::string strLine);

			std::string s13OpenFile (std::string strTitle, std::string strFilter, std::string strDefExt);
			std::string s13SaveFile (std::string strTitle, std::string strFilter, std::string strDefExt);

			void s13CopyTextToClipboard (std::string strData);
			std::string s13GetTextFromClipboard ();
			void s13OpenDefaultWebBrowser (std::string strURL);

			std::vector<std::string> s13SearchForFiles (std::string strSearchFor);
			std::vector<std::string> s13SearchForDirectories (std::string strSearchFor);

			bool s13Mkdir (std::string strString);
			bool s13MoveFile (std::string strExisting, std::string strNew);
			bool s13Rm(std::string strString);
			bool s13Rmdir (std::string strString);
			bool s13CopyFile (std::string strExisting, std::string strNew, bool bFailIfExists);
			bool s13IsFile (std::string strString);
			bool s13IsDirectory (std::string strString);

			std::string s13GetHomeDirectory ();
			std::string s13GetSettingsDirectory ();

			std::string convertwxStringToString (wxString strString);
			wxString convertStringTowxString (std::string strString);
		#endif

		#ifdef defSYS_NO_SPECIFIC_OS
			#ifndef defSYS_WIN32
				#ifndef defSYS_LINUX
					#ifndef defSYS_MACOSX
						void s13MsgBox (std::string strLine, std::string strTitle);
						void s13ErrorBox (std::string strLine);
						s13LogMessage (std::string strLine);

						std::string s13OpenFile (std::string strTitle, std::string strFilter, std::string strDefExt);
						std::string s13SaveFile (std::string strTitle, std::string strFilter, std::string strDefExt);

						void s13CopyTextToClipboard (std::string strData);
						std::string s13GetTextFromClipboard ();
						void s13OpenDefaultWebBrowser (std::string strURL);

						std::vector<std::string> s13SearchForFiles (std::string strSearchFor);
						std::vector<std::string> s13SearchForDirectories (std::string strSearchFor);

						bool s13Mkdir (std::string strString);
						bool s13MoveFile (std::string strExisting, std::string strNew);
						bool s13Rm(std::string strString);
						bool s13Rmdir (std::string strString);
						bool s13CopyFile (std::string strExisting, std::string strNew, bool bFailIfExists);
						bool s13IsFile (std::string strString);
						bool s13IsDirectory (std::string strString);

						std::string s13GetHomeDirectory ();
						std::string s13GetSettingsDirectory ();
					#endif
				#endif
			#endif
		#endif

		void s13Error (std::string strLine);
		void s13CreateSettingsDirectory ();

		std::string System_GetParentDirectoryFromFilePath (std::string strDir);
		std::string System_NormalizeDirectoryPath (std::string strDir, int iExclude = 0, bool bIncludeLastSlash = true);
	}
#endif

