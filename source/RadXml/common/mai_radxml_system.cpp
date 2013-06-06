#include "common/mai_radxml_system.h"
#include "common/com_radxml_strconv.h"

#ifdef defAPI_OGRE3D
	#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
		#include <OGRE/OgreLogManager.h>
	#else
		#include <OgreLogManager.h>
	#endif
#endif

#ifdef defSYS_WIN32
	#ifndef defAPI_WXWIDGETS
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
		#include <shellapi.h>
		#include <commdlg.h>
		#include <Shlobj.h>
	#endif
#endif

#ifdef defSYS_LINUX
	#ifndef defAPI_WXWIDGETS
		#include <iostream>
		#include <stdio.h>
		#include <dirent.h>
		#include <unistd.h>
		#include <sys/stat.h>
		#include <stdlib.h>
	#endif
#endif

#ifdef defAPI_WXWIDGETS
	#include <wx/stdpaths.h>
	#include <wx/msgdlg.h>
	#include <wx/filefn.h>

	#if (!defined (defAPI_WXWIDGETS_USE_UNICODE) && !defined(defAPI_WXWIDGETS_USE_ASCII))
		#define defAPI_WXWIDGETS_USE_ASCII
	#endif
#endif

namespace RadXmlCommon
{
	#ifdef defSYS_WIN32
	#ifndef defAPI_WXWIDGETS
		void s13MsgBox (std::string strLine, std::string strTitle)
		{
			MessageBox (0, strLine.c_str (), strTitle.c_str (), MB_OK);
		}

		void s13ErrorBox (std::string strLine)
		{
			MessageBox (0, strLine.c_str (), "Error", (MB_OK | MB_ICONEXCLAMATION));
		}

		void s13LogMessage (std::string strLine)
		{
			#ifdef defAPI_OGRE3D
				Ogre::LogManager::getSingletonPtr ()->logMessage (strLine);
			#endif
		}

		std::string s13OpenFile (std::string strTitle, std::string strFilter, std::string strDefExt)
		{
			std::string strReturn = "";
			OPENFILENAME ofnFile;
			char cReturn[MAX_PATH];
			char cCurrentDir[MAX_PATH];

			ShowCursor (true);
			GetCurrentDirectory (MAX_PATH, cCurrentDir);

			ZeroMemory (&ofnFile, sizeof (ofnFile));
			cReturn[0] = '\0';

			std::vector<std::string> aryStrings = s13SplitString (strFilter, "|");
			strFilter = "";

			for (int iIdx = 0; iIdx < (int)aryStrings.size (); iIdx++)
			{
				strFilter += aryStrings.at (iIdx);
				strFilter += '\0';
			}

			ofnFile.hwndOwner = 0;
			ofnFile.lpstrFilter = strFilter.c_str ();
			ofnFile.lpstrCustomFilter = 0;
			ofnFile.nMaxCustFilter = 0L;
			ofnFile.nFilterIndex = 1L;
			ofnFile.lpstrFile = cReturn;
			ofnFile.nMaxFile = 255;
			ofnFile.lpstrFileTitle = 0;
			ofnFile.nMaxFileTitle = 50;
			ofnFile.lpstrInitialDir = 0;
			ofnFile.lpstrTitle = strTitle.c_str ();
			ofnFile.nFileOffset = 0;
			ofnFile.nFileExtension = 2;
			ofnFile.lpstrDefExt = strDefExt.c_str ();
			ofnFile.lpfnHook = NULL;
			ofnFile.lCustData = 0;
			ofnFile.Flags = (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT);
			ofnFile.lStructSize = sizeof (OPENFILENAME);

			GetOpenFileName (&ofnFile);

			strReturn = ofnFile.lpstrFile;

			SetCurrentDirectory (cCurrentDir);
			ShowCursor (false);

			return (strReturn);
		}

		std::string s13SaveFile (std::string strTitle, std::string strFilter, std::string strDefExt)
		{
			std::string strReturn = "";
			OPENFILENAME ofnFile;
			char cReturn[MAX_PATH];
			char cCurrentDir[MAX_PATH];

			ShowCursor (true);
			GetCurrentDirectory (MAX_PATH, cCurrentDir);

			ZeroMemory (&ofnFile, sizeof (ofnFile));
			cReturn[0] = '\0';

			std::vector<std::string> aryStrings = s13SplitString (strFilter, "|");
			strFilter = "";

			for (int iIdx = 0; iIdx < (int)aryStrings.size (); iIdx++)
			{
				strFilter += aryStrings.at (iIdx);
				strFilter += '\0';
			}

			ofnFile.hwndOwner = 0;
			ofnFile.lpstrFilter = strFilter.c_str ();
			ofnFile.lpstrCustomFilter = 0;
			ofnFile.nMaxCustFilter = 0L;
			ofnFile.nFilterIndex = 1L;
			ofnFile.lpstrFile = cReturn;
			ofnFile.nMaxFile = 255;
			ofnFile.lpstrFileTitle = 0;
			ofnFile.nMaxFileTitle = 50;
			ofnFile.lpstrInitialDir = 0;
			ofnFile.lpstrTitle = strTitle.c_str ();
			ofnFile.nFileOffset = 0;
			ofnFile.nFileExtension = 2;
			ofnFile.lpstrDefExt = strDefExt.c_str ();
			ofnFile.lpfnHook = NULL;
			ofnFile.lCustData = 0;
			ofnFile.Flags = (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT);
			ofnFile.lStructSize = sizeof (OPENFILENAME);

			GetSaveFileName (&ofnFile);

			strReturn = ofnFile.lpstrFile;

			SetCurrentDirectory (cCurrentDir);
			ShowCursor (false);

			return (strReturn);
		}

		void s13CopyTextToClipboard (std::string strData)
		{
			if (OpenClipboard (NULL))
			{
				EmptyClipboard ();
				HGLOBAL hgMem = GlobalAlloc ((GMEM_SHARE | GMEM_MOVEABLE | GMEM_ZEROINIT), 
														(strData.size () * strData.at (0)));
				char *cText = (char *)GlobalLock (hgMem);
				memcpy (cText, strData.c_str (), strData.size ());
				GlobalUnlock (hgMem);

				SetClipboardData (CF_TEXT, hgMem);
			}

			CloseClipboard ();
		}

		std::string s13GetTextFromClipboard ()
		{
			std::string strReturn = "";

			if (OpenClipboard (NULL))
			{
				HGLOBAL hgMem = GetClipboardData (CF_TEXT);

				if (hgMem != 0)
				{
					LPCTSTR lpText = (LPCTSTR)GlobalLock (hgMem);

					strReturn = lpText;
				}
			}

			CloseClipboard ();

			return (strReturn);
		}

		void s13OpenDefaultWebBrowser (std::string strURL)
		{
			ShellExecute (0, "open", strURL.c_str (), 0, 0, SW_SHOWNORMAL);
		}

		std::vector<std::string> s13SearchForFiles (std::string strSearchFor)
		{
			std::vector<std::string> aryReturn;

			WIN32_FIND_DATA wfdFile;
			HANDLE hSearch = FindFirstFile (strSearchFor.c_str (), &wfdFile);

			while (hSearch != INVALID_HANDLE_VALUE)
			{
				if (GetFileAttributes (wfdFile.cFileName) & FILE_ATTRIBUTE_NORMAL)
				{
					std::string strRet = (char *)wfdFile.cFileName;
					aryReturn.push_back (strRet);
				}

				if (FindNextFile (hSearch, &wfdFile) == 0)
					break;
			}

			return (aryReturn);
		}

		std::vector<std::string> s13SearchForDirectories (std::string strSearchFor)
		{
			std::vector<std::string> aryReturn;

			WIN32_FIND_DATA wfdFile;
			HANDLE hSearch = FindFirstFile (strSearchFor.c_str (), &wfdFile);

			while (hSearch != INVALID_HANDLE_VALUE)
			{
				if (GetFileAttributes (wfdFile.cFileName) & FILE_ATTRIBUTE_DIRECTORY)
				{
					bool bSkip = false;
					std::string strRet = (char *)wfdFile.cFileName;

					if (strRet == ".")
						bSkip = true;
					else if (strRet == "..")
						bSkip = true;

					if (bSkip == false)
						aryReturn.push_back (strRet);
				}

				if (FindNextFile (hSearch, &wfdFile) == 0)
					break;
			}

			return (aryReturn);
		}

		#pragma warning (disable: 4800)

		bool s13Mkdir (std::string strString)
		{
			return (CreateDirectory (strString.c_str (), 0));
		}

		bool s13MoveFile (std::string strExisting, std::string strNew)
		{
			return (MoveFile (strExisting.c_str (), strNew.c_str ()));
		}

		bool s13Rm (std::string strString)
		{
			return (DeleteFile (strString.c_str ()));
		}

		bool s13Rmdir (std::string strString)
		{
			return (RemoveDirectory (strString.c_str ()));
		}

		bool s13CopyFile (std::string strExisting, std::string strNew, bool bFailIfExists)
		{
			return (CopyFile (strExisting.c_str (), strNew.c_str (), bFailIfExists));
		}

		bool s13IsFile (std::string strString)
		{
			bool bReturn = true;

			if (GetFileAttributes (strString.c_str ()) == INVALID_FILE_ATTRIBUTES)
				bReturn = false;

			return (bReturn);
		}

		bool s13IsDirectory (std::string strString)
		{
			bool bReturn = false;

			if (GetFileAttributes (strString.c_str ()) == FILE_ATTRIBUTE_DIRECTORY)
				bReturn = true;

			return (bReturn);
		}

		std::string s13GetHomeDirectory ()
		{
			std::string strHome = "";
			char *cHome = new char [MAX_PATH];

			SHGetSpecialFolderPath (0, cHome, CSIDL_PERSONAL, false);
			strHome = cHome;

			delete cHome;
			cHome = 0;

			return (strHome);
		}

		std::string s13GetSettingsDirectory ()
		{
			std::string strHome = "";
			char *cHome = new char [MAX_PATH];

			SHGetSpecialFolderPath (0, cHome, CSIDL_PERSONAL, false);
			strHome = cHome;

			delete cHome;
			cHome = 0;

			std::string strReturn = strHome;
			strReturn += "\\Higher Edge Software\\Games\\Shattered Puzzle";

			return (strReturn);
		}
	#endif
	#endif

	#ifdef defSYS_LINUX
	#ifndef defAPI_WXWIDGETS
		void s13MsgBox (std::string strLine, std::string strTitle)
		{
			std::cout << strTitle << strLine;
		}

		void s13ErrorBox (std::string strLine)
		{
			std::cout << "Error: " << strLine;
		}

		void s13LogMessage (std::string strLine)
		{
			#ifdef defAPI_OGRE3D
				Ogre::LogManager::getSingletonPtr ()->logMessage (strLine);
			#endif
		}

		std::string s13OpenFile (std::string strTitle, std::string strFilter, std::string strDefExt)
		{
			std::string strReturn = "";

			return (strReturn);
		}

		std::string s13SaveFile (std::string strTitle, std::string strFilter, std::string strDefExt)
		{
			std::string strReturn = "";

			return (strReturn);
		}

		void s13CopyTextToClipboard (std::string strData)
		{
		}

		std::string s13GetTextFromClipboard ()
		{
			std::string strReturn = "";

			return (strReturn);
		}

		void s13OpenDefaultWebBrowser (std::string strURL)
		{
		}

		std::vector<std::string> s13SearchForFiles (std::string strSearchFor)
		{
			std::vector<std::string> aryReturn;

			DIR *dDir = opendir (strSearchFor.c_str ());

			if (dDir == 0)
				return (aryReturn);

			struct dirent *deEntry = readdir (dDir);

			while (deEntry != 0)
			{
				if (deEntry->d_type != DT_DIR)
					aryReturn.push_back (deEntry->d_name);

				deEntry = readdir (dDir);
			}

			closedir (dDir);

			return (aryReturn);
		}

		std::vector<std::string> s13SearchForDirectories (std::string strSearchFor)
		{
			std::vector<std::string> aryReturn;

			/*struct ffblk fFinder;
			unsigned int uiResult = findfirst (strSearchFor.c_str (), &fFinder, (FA_DIREC | FA_ARCH));

			while (uiResult != 0)
			{
				if (fFinder.ff_attrib & FA_DIREC)
				{
					std::string strRet = (char *)fFinder.ff_name;
					aryReturn.push_back (strRet);
				}

				uiResult = findnext (fFinder);
			}*/

			DIR *dDir = opendir (strSearchFor.c_str ());
			struct dirent *deEntry = readdir (dDir);

			while (deEntry != 0)
			{
				if (deEntry->d_type == DT_DIR)
					aryReturn.push_back (deEntry->d_name);

				deEntry = readdir (dDir);
			}

			closedir (dDir);

			return (aryReturn);
		}

		bool s13Mkdir (std::string strString)
		{
			bool bResult = true;

			if (mkdir (strString.c_str (), 0777) != 0)
				bResult = false;

			return (bResult);
		}

		bool s13MoveFile (std::string strExisting, std::string strNew)
		{
			return (true);
		}

		bool s13Rm(std::string strString)
		{
			bool bResult = true;

			if (remove (strString.c_str ()) != 0)
				bResult = false;

			return (bResult);
		}

		bool s13Rmdir (std::string strString)
		{
			bool bResult = true;

			if (remove (strString.c_str ()) != 0)
				bResult = false;

			return (bResult);
		}

		bool s13CopyFile (std::string strExisting, std::string strNew, bool bFailIfExists)
		{
			return (true);
		}

		bool s13IsFile (std::string strString)
		{
			bool bReturn = false;

			return (bReturn);
		}

		bool s13IsDirectory (std::string strString)
		{
			struct stat st;

			if (stat (strString.c_str (), &st) == 0)
				return (true);

			return (false);
		}

		std::string s13GetHomeDirectory ()
		{
			std::string strHome = getenv ("HOME");
			strHome += "/.";

			return (strHome);
		}

		std::string s13GetSettingsDirectory ()
		{
			std::string strReturn = getenv ("HOME");
			strReturn += "/.Higher Edge Software/Games/Shattered Puzzle";

			return (strReturn);
		}
	#endif
	#endif

	#ifdef defSYS_MACOSX
	#ifndef defAPI_WXWIDGETS
		void s13MsgBox (std::string strLine, std::string strTitle)
		{
			#ifdef __OBJC__
				NSRunAlertPanel ([NSString stringWithUTF8String: strTitle.c_str ()], [NSString stringWithUTF8String: strLine.c_str ()], 
									NSLocalizedString(@"Ok", nil), nil, nil);
			#else
				CFStringRef cfTitle = CFStringCreateWithCString (kCFAllocatorDefault, strTitle.c_str (), GetApplicationTextEncoding ());
				CFStringRef cfLine = CFStringCreateWithCString (kCFAllocatorDefault, strLine.c_str (), GetApplicationTextEncoding ());

				DialogRef drAlert;
				CreateStandardAlert(kAlertPlainAlert, cfTitle, cfLine, 0, &drAlert);
				RunStandardAlert(drAlert, 0, 0);
			#endif
		}

		void s13ErrorBox (std::string strLine)
		{
			#ifdef __OBJC__
				NSRunAlertPanel (@"Error", [NSString stringWithUTF8String: strLine.c_str ()], NSLocalizedString(@"Ok", nil), nil, nil);
			#else
				CFStringRef cfLine = CFStringCreateWithCString (kCFAllocatorDefault, strLine.c_str (), GetApplicationTextEncoding ());

				DialogRef drAlert;
				CreateStandardAlert(kAlertCautionAlert, CFSTR ("Error"), cfLine, 0, &drAlert);
				RunStandardAlert(drAlert, 0, 0);
			#endif
		}

		void s13LogMessage (std::string strLine)
		{
			#ifdef defAPI_OGRE3D
				Ogre::LogManager::getSingletonPtr ()->logMessage (strLine);
			#endif
		}

		std::string s13OpenFile (std::string strTitle, std::string strFilter, std::string strDefExt)
		{
			std::string strReturn = "";

			return (strReturn);
		}

		std::string s13SaveFile (std::string strTitle, std::string strFilter, std::string strDefExt)
		{
			std::string strReturn = "";

			return (strReturn);
		}

		void s13CopyTextToClipboard (std::string strData)
		{
		}

		std::string s13GetTextFromClipboard ()
		{
			std::string strReturn = "";

			return (strReturn);
		}

		void s13OpenDefaultWebBrowser (std::string strURL)
		{
		}

		// http://developer.apple.com/mac/library/documentation/Carbon/Reference/File_Manager/Reference/reference.html

		std::vector<std::string> s13SearchForFiles (std::string strSearchFor)
		{
			std::vector<std::string> aryReturn;

			return (aryReturn);
		}

		std::vector<std::string> s13SearchForDirectories (std::string strSearchFor)
		{
			std::vector<std::string> aryReturn;

			return (aryReturn);
		}

		bool s13Mkdir (std::string strString)
		{
			NSString *strNSString = [NSString stringWithUTF8String: strString.c_str ()];
			bool bReturn = [[NSFileManager defaultManager] createDirectoryAtPath: strNSString attributes: nil];

			return (bReturn);
		}

		bool s13MoveFile (std::string strExisting, std::string strNew)
		{
			NSString *strSRC = [NSString stringWithUTF8String: strExisting.c_str ()];
			NSString *strDEST = [NSString stringWithUTF8String: strNew.c_str ()];

			bool bReturn = [[NSFileManager defaultManager] moveItemAtPath: strSRC toPath: strDEST error: nil];

			return (bReturn);
		}

		bool s13Rm(std::string strString)
		{
			NSString *strNSString = [NSString stringWithUTF8String: strString.c_str ()];
			bool bReturn = [[NSFileManager defaultManager] removeItemAtPath: strNSString error: nil];

			return (bReturn);
		}

		bool s13Rmdir (std::string strString)
		{
			NSString *strNSString = [NSString stringWithUTF8String: strString.c_str ()];
			bool bReturn = [[NSFileManager defaultManager] removeItemAtPath: strNSString error: nil];

			return (bReturn);
		}

		bool s13CopyFile (std::string strExisting, std::string strNew, bool bFailIfExists)
		{
			NSString *strSRC = [NSString stringWithUTF8String: strExisting.c_str ()];
			NSString *strDEST = [NSString stringWithUTF8String: strNew.c_str ()];

			if (bFailIfExists == true)
			{
				if ([[NSFileManager defaultManager] fileExistsAtPath: strDEST] == true)
					return (false);
			}

			bool bReturn = [[NSFileManager defaultManager] copyItemAtPath: strSRC toPath: strDEST error: nil];

			return (bReturn);
		}

		bool s13IsFile (std::string strString)
		{
			bool bReturn = false;
			BOOL bTemp = false;
			NSString *strNSString = [NSString stringWithUTF8String: strString.c_str ()];

			[[NSFileManager defaultManager] fileExistsAtPath: strNSString isDirectory: &bTemp];
			bReturn = bTemp;

			return (!bReturn);
		}

		bool s13IsDirectory (std::string strString)
		{
			bool bReturn = false;
			BOOL bTemp = false;
			NSString *strNSString = [NSString stringWithUTF8String: strString.c_str ()];

			[[NSFileManager defaultManager] fileExistsAtPath: strNSString isDirectory: &bTemp];
			bReturn = bTemp;

			return (bReturn);
		}

		std::string s13GetHomeDirectory ()
		{
			NSString *strHomeDir = NSHomeDirectory ();
			std::string strReturn = [strHomeDir UTF8String];

			return (strReturn);
		}

		std::string s13GetSettingsDirectory ()
		{
			NSString *strHomeDir = NSHomeDirectory ();
			std::string strReturn = [strHomeDir UTF8String];
			strReturn += "/Library/Higher Edge Software/Games/Shattered Puzzle";

			return (strReturn);
		}

		std::string convertNSStringToString (NSString *string)
		{
			return ([string UTF8String]);
		}
	#endif
	#endif

	#ifdef defSYS_IPHONE
	void s13MsgBox (std::string strLine, std::string strTitle)
	{
	}

	void s13ErrorBox (std::string strLine)
	{
	}

	void s13LogMessage (std::string strLine)
	{
		#ifdef defAPI_OGRE3D
			Ogre::LogManager::getSingletonPtr ()->logMessage (strLine);
		#endif
	}

	std::string s13OpenFile (std::string strTitle, std::string strFilter, std::string strDefExt)
	{
		std::string strReturn = "";
		
		return (strReturn);
	}

	std::string s13SaveFile (std::string strTitle, std::string strFilter, std::string strDefExt)
	{
		std::string strReturn = "";
		
		return (strReturn);
	}

	void s13CopyTextToClipboard (std::string strData)
	{
	}

	std::string s13GetTextFromClipboard ()
	{
		std::string strReturn = "";
		
		return (strReturn);
	}

	void s13OpenDefaultWebBrowser (std::string strURL)
	{
		NSString *nstrURL = [NSString stringWithUTF8String:strURL.c_str ()];

		[[UIApplication sharedApplication] openURL:[NSURL URLWithString:nstrURL]];
	}

	std::vector<std::string> s13SearchForFiles (std::string strSearchFor)
	{
		std::vector<std::string> aryReturn;
		
		return (aryReturn);
	}

	std::vector<std::string> s13SearchForDirectories (std::string strSearchFor)
	{
		std::vector<std::string> aryReturn;
		
		return (aryReturn);
	}

	bool s13Mkdir (std::string strString)
	{
		NSString *strNSString = [NSString stringWithUTF8String: strString.c_str ()];
		bool bReturn = [[NSFileManager defaultManager] createDirectoryAtPath: strNSString attributes: nil];

		return (bReturn);
	}

	bool s13MoveFile (std::string strExisting, std::string strNew)
	{
		NSString *strSRC = [NSString stringWithUTF8String: strExisting.c_str ()];
		NSString *strDEST = [NSString stringWithUTF8String: strNew.c_str ()];

		bool bReturn = [[NSFileManager defaultManager] moveItemAtPath: strSRC toPath: strDEST error: nil];

		return (bReturn);
	}

	bool s13Rm(std::string strString)
	{
		NSString *strNSString = [NSString stringWithUTF8String: strString.c_str ()];
		bool bReturn = [[NSFileManager defaultManager] removeItemAtPath: strNSString error: nil];

		return (bReturn);
	}

	bool s13Rmdir (std::string strString)
	{
		NSString *strNSString = [NSString stringWithUTF8String: strString.c_str ()];
		bool bReturn = [[NSFileManager defaultManager] removeItemAtPath: strNSString error: nil];

		return (bReturn);
	}

	bool s13CopyFile (std::string strExisting, std::string strNew, bool bFailIfExists)
	{
		NSString *strSRC = [NSString stringWithUTF8String: strExisting.c_str ()];
		NSString *strDEST = [NSString stringWithUTF8String: strNew.c_str ()];

		if (bFailIfExists == true)
		{
			if ([[NSFileManager defaultManager] fileExistsAtPath: strDEST] == true)
				return (false);
		}

		bool bReturn = [[NSFileManager defaultManager] copyItemAtPath: strSRC toPath: strDEST error: nil];

		return (bReturn);
	}

	bool s13IsFile (std::string strString)
	{
		bool bReturn = false;
		BOOL bTemp = false;
		NSString *strNSString = [NSString stringWithUTF8String: strString.c_str ()];

		[[NSFileManager defaultManager] fileExistsAtPath: strNSString isDirectory: &bTemp];
		bReturn = bTemp;

		return (!bReturn);
	}

	bool s13IsDirectory (std::string strString)
	{
		bool bReturn = false;
		BOOL bTemp = false;
		NSString *strNSString = [NSString stringWithUTF8String: strString.c_str ()];

		[[NSFileManager defaultManager] fileExistsAtPath: strNSString isDirectory: &bTemp];
		bReturn = bTemp;

		return (bReturn);
	}

	std::string s13GetHomeDirectory ()
	{
		NSString *strHomeDir = NSHomeDirectory ();
		std::string strReturn = [strHomeDir UTF8String];

		return (strReturn);
	}

	std::string s13GetSettingsDirectory ()
	{
		NSString *strHomeDir = NSHomeDirectory ();
		std::string strReturn = [strHomeDir UTF8String];
		strReturn += "/Library/Higher Edge Software/Games/Shattered Puzzle";

		return (strReturn);
	}

	std::string convertNSStringToString (NSString *string)
	{
		return ([string UTF8String]);
	}
	#endif

	#ifdef defSYS_ANDROID

	JavaJNI *jniMain = 0;
	std::string strHomeDir = "";

	void s13MsgBox (std::string strLine, std::string strTitle)
	{
		JNIEnv *jEnv = jniMain->getEnvironment ();
		jclass jcCommon = jEnv->FindClass ("com/highersoftware/Common");

		if (jcCommon != 0)
		{
			jmethodID jmMethod = jEnv->GetStaticMethodID (jcCommon, "s13MsgBox", "(Ljava/lang/String;Ljava/lang/String;)V");

			if (jmMethod != 0)
			{
				jstring jstrLine = jEnv->NewStringUTF (strLine.c_str ());
				jstring jstrTitle = jEnv->NewStringUTF (strTitle.c_str ());

				jEnv->CallStaticVoidMethod (jcCommon, jmMethod, jstrLine, jstrTitle);

				jEnv->DeleteLocalRef(jstrLine);
				jEnv->DeleteLocalRef(jstrTitle);
			}
		}
	}

	void s13ErrorBox (std::string strLine)
	{
		s13MsgBox (strLine, "Error");
	}

	void s13OpenDefaultWebBrowser (std::string strURL)
	{
		JNIEnv *jEnv = jniMain->getEnvironment ();
		jclass jcCommon = jEnv->FindClass ("com/highersoftware/Common");

		if (jcCommon != 0)
		{
			jmethodID jmMethod = jEnv->GetStaticMethodID (jcCommon, "s13OpenDefaultWebBrowser", "(Ljava/lang/String;)V");

			if (jmMethod != 0)
			{
				jstring jstrURL = jEnv->NewStringUTF (strURL.c_str ());
				jEnv->CallStaticVoidMethod (jcCommon, jmMethod, jstrURL);
				jEnv->DeleteLocalRef(jstrURL);
			}
		}
	}

	bool s13Mkdir (std::string strString)
	{
		bool bReturn = false;
		JNIEnv *jEnv = jniMain->getEnvironment ();
		jclass jcCommon = jEnv->FindClass ("com/highersoftware/Common");

		if (jcCommon != 0)
		{
			jmethodID jmMethod = jEnv->GetStaticMethodID (jcCommon, "s13Mkdir", "(Ljava/lang/String;)z");

			if (jmMethod != 0)
			{
				jstring jstrString = jEnv->NewStringUTF (strString.c_str ());
				bReturn = jEnv->CallStaticBooleanMethod (jcCommon, jmMethod, jstrString);
				jEnv->DeleteLocalRef(jstrString);
			}
		}

		return (bReturn);
	}

	bool s13MoveFile (std::string strExisting, std::string strNew)
	{
		bool bReturn = false;
		JNIEnv *jEnv = jniMain->getEnvironment ();
		jclass jcCommon = jEnv->FindClass ("com/highersoftware/Common");

		if (jcCommon != 0)
		{
			jmethodID jmMethod = jEnv->GetStaticMethodID (jcCommon, "s13MoveFile", "(Ljava/lang/String;Ljava/lang/String;)z");

			if (jmMethod != 0)
			{
				jstring jstrString1 = jEnv->NewStringUTF (strExisting.c_str ());
				jstring jstrString2 = jEnv->NewStringUTF (strNew.c_str ());

				bReturn = jEnv->CallStaticBooleanMethod (jcCommon, jmMethod, jstrString1, jstrString2);

				jEnv->DeleteLocalRef(jstrString1);
				jEnv->DeleteLocalRef(jstrString2);
			}
		}

		return (bReturn);
	}

	bool s13Rm (std::string strString)
	{
		bool bReturn = false;
		JNIEnv *jEnv = jniMain->getEnvironment ();
		jclass jcCommon = jEnv->FindClass ("com/highersoftware/Common");

		if (jcCommon != 0)
		{
			jmethodID jmMethod = jEnv->GetStaticMethodID (jcCommon, "s13Rm", "(Ljava/lang/String;)z");

			if (jmMethod != 0)
			{
				jstring jstrString = jEnv->NewStringUTF (strString.c_str ());
				bReturn = jEnv->CallStaticBooleanMethod (jcCommon, jmMethod, jstrString);
				jEnv->DeleteLocalRef(jstrString);
			}
		}

		return (bReturn);
	}

	bool s13Rmdir (std::string strString)
	{
		bool bReturn = false;
		JNIEnv *jEnv = jniMain->getEnvironment ();
		jclass jcCommon = jEnv->FindClass ("com/highersoftware/Common");

		if (jcCommon != 0)
		{
			jmethodID jmMethod = jEnv->GetStaticMethodID (jcCommon, "s13Rmdir", "(Ljava/lang/String;)z");

			if (jmMethod != 0)
			{
				jstring jstrString = jEnv->NewStringUTF (strString.c_str ());
				bReturn = jEnv->CallStaticBooleanMethod (jcCommon, jmMethod, jstrString);
				jEnv->DeleteLocalRef(jstrString);
			}
		}

		return (bReturn);
	}

	std::vector<std::string> s13ListAllFilesInDirectory (std::string directory)
	{
		std::vector<std::string> aryFiles;

		return (aryFiles);
	}

	bool s13CopyFile (std::string strExisting, std::string strNew, bool bFailIfExists)
	{
		bool bReturn = false;
		JNIEnv *jEnv = jniMain->getEnvironment ();
		jclass jcCommon = jEnv->FindClass ("com/highersoftware/Common");

		if (jcCommon != 0)
		{
			jmethodID jmMethod = jEnv->GetStaticMethodID (jcCommon, "s13CopyFile", "(Ljava/lang/String;Ljava/lang/String;z)z");

			if (jmMethod != 0)
			{
				jstring jstrString1 = jEnv->NewStringUTF (strExisting.c_str ());
				jstring jstrString2 = jEnv->NewStringUTF (strNew.c_str ());
				jboolean jbBoolean = bFailIfExists;

				bReturn = jEnv->CallStaticBooleanMethod (jcCommon, jmMethod, jstrString1, jstrString2, jbBoolean);

				jEnv->DeleteLocalRef(jstrString1);
				jEnv->DeleteLocalRef(jstrString2);
			}
		}

		return (bReturn);
	}

	bool s13CopyAllFilesToDirectory (std::string strExisting, std::string strNew, bool bFailIfExists)
	{
		bool bReturn = false;
		JNIEnv *jEnv = jniMain->getEnvironment ();
		jclass jcCommon = jEnv->FindClass ("com/highersoftware/Common");

		if (jcCommon != 0)
		{
			jmethodID jmMethod = jEnv->GetStaticMethodID (jcCommon, "s13CopyAllFilesToDirectory",
														"(Ljava/lang/String;Ljava/lang/String;z)z");

			if (jmMethod != 0)
			{
				jstring jstrString1 = jEnv->NewStringUTF (strExisting.c_str ());
				jstring jstrString2 = jEnv->NewStringUTF (strNew.c_str ());
				jboolean jbBoolean = bFailIfExists;

				bReturn = jEnv->CallStaticBooleanMethod (jcCommon, jmMethod, jstrString1, jstrString2, jbBoolean);

				jEnv->DeleteLocalRef(jstrString1);
				jEnv->DeleteLocalRef(jstrString2);
			}
		}

		return (bReturn);
	}

	bool s13IsFile (std::string strString)
	{
		bool bReturn = false;
		JNIEnv *jEnv = jniMain->getEnvironment ();
		jclass jcCommon = jEnv->FindClass ("com/highersoftware/Common");

		if (jcCommon != 0)
		{
			jmethodID jmMethod = jEnv->GetStaticMethodID (jcCommon, "s13IsFile", "(Ljava/lang/String;)z");

			if (jmMethod != 0)
			{
				jstring jstrString = jEnv->NewStringUTF (strString.c_str ());
				bReturn = jEnv->CallStaticBooleanMethod (jcCommon, jmMethod, jstrString);
				jEnv->DeleteLocalRef(jstrString);
			}
		}

		return (bReturn);
	}

	bool s13IsDirectory (std::string strString)
	{
		bool bReturn = false;
		JNIEnv *jEnv = jniMain->getEnvironment ();
		jclass jcCommon = jEnv->FindClass ("com/highersoftware/Common");

		if (jcCommon != 0)
		{
			jmethodID jmMethod = jEnv->GetStaticMethodID (jcCommon, "s13IsDirectory", "(Ljava/lang/String;)z");

			if (jmMethod != 0)
			{
				jstring jstrString = jEnv->NewStringUTF (strString.c_str ());
				bReturn = jEnv->CallStaticBooleanMethod (jcCommon, jmMethod, jstrString);
				jEnv->DeleteLocalRef(jstrString);
			}
		}

		return (bReturn);
	}

	std::string s13GetHomeDirectory ()
	{
		std::string strReturn = "";

		if (strHomeDir == "")
		{
			JNIEnv *jEnv = jniMain->getEnvironment ();
			jclass jcCommon = jEnv->FindClass ("com/highersoftware/Common");

			if (jcCommon != 0)
			{
				jmethodID jmMethod = jEnv->GetStaticMethodID (jcCommon, "s13GetHomeDirectory", "()Ljava/lang/String;");

				if (jmMethod != 0)
				{
					jstring jstrReturn = (jstring)jEnv->CallStaticObjectMethod (jcCommon, jmMethod);
					const char *ccReturn = jEnv->GetStringUTFChars (jstrReturn, 0);
					strReturn = ccReturn;
					strHomeDir = strReturn;
				}
			}
		}
		else
			strReturn = strHomeDir;

		return (strReturn);
	}

	std::string convertJStringToString (jstring stringToConvert)
	{
		JNIEnv *jEnv = jniMain->getEnvironment ();
		const char *ccReturn = jEnv->GetStringUTFChars (stringToConvert, 0);
		std::string strReturn = ccReturn;

		return (strReturn);
	}

	jint JNI_OnLoad(JavaVM *vm, void *reserved)
	{
		return (JNI_VERSION_1_2);
	}

	void JNI_OnUnload(JavaVM *vm, void *reserved)
	{
	}

	JNIEXPORT void JNICALL Java_com_highersoftware_Common_createJNI (JNIEnv *env, jclass javaClass)
	{
		jniMain = new JavaJNI (env, javaClass);
	}

	JNIEXPORT void JNICALL Java_com_highersoftware_Common_destroyJNI (JNIEnv *env, jclass javaClass)
	{
		if (jniMain != 0)
		{
			delete jniMain;
			jniMain = 0;
		}
	}
	#endif

	#ifdef defAPI_WXWIDGETS
		void s13MsgBox (std::string strLine, std::string strTitle)
		{
			wxString strLineWX = wxT ("");
			wxString strTitleWX = wxT ("");

			#ifdef defAPI_WXWIDGETS_USE_UNICODE
				strLineWX = wxString::FromAscii (strLine.c_str());
				strTitleWX = wxString::FromAscii (strTitle.c_str());
			#endif

			#ifdef defAPI_WXWIDGETS_USE_ASCII
				strLineWX = wxString::From8BitData (strLine.c_str());
				strTitleWX = wxString::From8BitData (strTitle.c_str());
			#endif

			wxMessageBox (strLineWX, strTitleWX, (wxCENTRE | wxOK));
		}

		void s13ErrorBox (std::string strLine)
		{
			wxString strLineWX = wxT ("");

			#ifdef defAPI_WXWIDGETS_USE_UNICODE
				strLineWX = wxString::FromAscii (strLine.c_str());
			#endif

			#ifdef defAPI_WXWIDGETS_USE_ASCII
				strLineWX = wxString::From8BitData (strLine.c_str());
			#endif

			wxMessageBox (strLineWX, wxT ("Error"), (wxCENTRE | wxOK | wxICON_EXCLAMATION));
		}

		void s13LogMessage (std::string strLine)
		{
			#ifdef defAPI_OGRE3D
				Ogre::LogManager::getSingletonPtr ()->logMessage (strLine);
			#endif
		}

		std::string s13OpenFile (std::string strTitle, std::string strFilter, std::string strDefExt)
		{
			std::string strReturn = "";
			wxString strTitleWX = wxT ("");
			wxString strFilterWX = wxT ("");

			#ifdef defAPI_WXWIDGETS_USE_UNICODE
				strTitleWX = wxString::FromAscii (strTitle.c_str());
				strFilterWX = wxString::FromAscii (strFilter.c_str());
			#endif

			#ifdef defAPI_WXWIDGETS_USE_ASCII
				strTitleWX = wxString::From8BitData (strTitle.c_str());
				strFilterWX = wxString::From8BitData (strFilter.c_str());
			#endif

			wxFileDialog *wfdOpen = new wxFileDialog (0, strTitleWX, wxT (""), wxT (""), 
														strFilterWX, wxOPEN);

			wfdOpen->ShowModal ();
			strReturn = RadXmlCommon::convertwxStringToString (wfdOpen->GetPath ());

			delete wfdOpen;
			wfdOpen = 0;

			return (strReturn);
		}

		std::string s13SaveFile (std::string strTitle, std::string strFilter, std::string strDefExt)
		{
			std::string strReturn = "";
			wxString strTitleWX = wxT ("");
			wxString strFilterWX = wxT ("");

			#ifdef defAPI_WXWIDGETS_USE_UNICODE
				strTitleWX = wxString::FromAscii (strTitle.c_str());
				strFilterWX = wxString::FromAscii (strFilter.c_str());
			#endif

			#ifdef defAPI_WXWIDGETS_USE_ASCII
				strTitleWX = wxString::From8BitData (strTitle.c_str());
				strFilterWX = wxString::From8BitData (strFilter.c_str());
			#endif

			wxFileDialog *wfdSave = new wxFileDialog (0, strTitleWX, wxT (""), wxT (""), 
														strFilterWX, wxOPEN);

			wfdSave->ShowModal ();
			strReturn = RadXmlCommon::convertwxStringToString (wfdSave->GetPath ());

			delete wfdSave;
			wfdSave = 0;

			return (strReturn);
		}

		void s13CopyTextToClipboard (std::string strData)
		{
			wxClipboard *wcClip = new wxClipboard ();

			if (wcClip->Open () == true)
			{
				wxString strDataWX = wxT ("");

				#ifdef defAPI_WXWIDGETS_USE_UNICODE
					strDataWX = wxString::FromAscii (strData.c_str ());
				#endif

				#ifdef defAPI_WXWIDGETS_USE_ASCII
					strDataWX = wxString::From8BitData (strData.c_str ());
				#endif

				wcClip->SetData (new wxTextDataObject (strDataWX));
				wcClip->Close ();
			}

			delete wcClip;
			wcClip = 0;
		}

		std::string s13GetTextFromClipboard ()
		{
			std::string strReturn = "";
			wxClipboard *wcClip = new wxClipboard ();

			if (wcClip->Open () == true)
			{
				wxTextDataObject objData;
				wcClip->GetData (objData);
				strReturn = RadXmlCommon::convertwxStringToString (objData.GetText ());
				wcClip->Close ();
			}

			delete wcClip;
			wcClip = 0;

			return (strReturn);
		}

		void s13OpenDefaultWebBrowser (std::string strURL)
		{
		}

		std::vector<std::string> s13SearchForFiles (std::string strSearchFor)
		{
			std::vector<std::string> aryReturn;

			return (aryReturn);
		}

		std::vector<std::string> s13SearchForDirectories (std::string strSearchFor)
		{
			std::vector<std::string> aryReturn;

			return (aryReturn);
		}

		bool s13Mkdir (std::string strString)
		{
			return (wxMkDir (strString.c_str ()));
		}

		bool s13MoveFile (std::string strExisting, std::string strNew)
		{
			return (wxRenameFile (RadXmlCommon::convertStringTowxString (strExisting), RadXmlCommon::convertStringTowxString (strNew)));
		}

		bool s13Rm (std::string strString)
		{
			return (wxRemoveFile (RadXmlCommon::convertStringTowxString (strString)));
		}

		bool s13Rmdir (std::string strString)
		{
			return (wxRmDir (strString.c_str ()));
		}

		bool s13CopyFile (std::string strExisting, std::string strNew, bool bFailIfExists)
		{
			return (wxCopyFile (RadXmlCommon::convertStringTowxString (strExisting), RadXmlCommon::convertStringTowxString (strNew)));
		}

		bool s13IsFile (std::string strString)
		{
			return (wxFileExists (RadXmlCommon::convertStringTowxString (strString)));
		}

		bool s13IsDirectory (std::string strString)
		{
			return (wxDirExists (RadXmlCommon::convertStringTowxString (strString)));
		}

		std::string s13GetHomeDirectory ()
		{
			wxStandardPaths spPaths;
			return (RadXmlCommon::convertwxStringToString (spPaths.GetDocumentsDir ()));
		}

		std::string s13GetSettingsDirectory ()
		{
			wxStandardPaths spPaths;
			std::string strReturn = RadXmlCommon::convertwxStringToString (spPaths.GetDocumentsDir ());

			#ifdef defSYS_WIN32
				strReturn += "/Higher Edge Software/Games/Shattered Puzzle";
			#endif
			#ifdef defSYS_LINUX
				strReturn += "/.Higher Edge Software/Games/Shattered Puzzle";
			#endif
			#ifdef defSYS_MACOSX
				strReturn += "/Library/Higher Edge Software/Games/Shattered Puzzle";
			#endif

			return (strReturn);
		}

		std::string convertwxStringToString (wxString strString)
		{
			return ((const char *)strString.mb_str (wxConvUTF8));
		}

		wxString convertStringTowxString (std::string strString)
		{
			return (wxString::FromAscii (strString.c_str ()));
		}
	#endif

	#ifdef defSYS_NO_SPECIFIC_OS
		#ifndef defSYS_WIN32
			#ifndef defSYS_LINUX
				#ifndef defSYS_MACOSX
					void s13MsgBox (std::string strLine, std::string strTitle)
					{
						cout << strTitle << strLine;
					}

					void s13ErrorBox (std::string strLine)
					{
						cout << "Error: " << strLine;
					}

					void s13LogMessage (std::string strLine)
					{
						#ifdef defAPI_OGRE3D
							Ogre::LogManager::getSingletonPtr ()->logMessage (strLine);
						#endif
					}

					std::string s13OpenFile (std::string strTitle, std::string strFilter, std::string strDefExt)
					{
						std::string strReturn = "";

						return (strReturn);
					}

					std::string s13SaveFile (std::string strTitle, std::string strFilter, std::string strDefExt)
					{
						std::string strReturn = "";

						return (strReturn);
					}

					void s13CopyTextToClipboard (std::string strData)
					{
					}

					std::string s13GetTextFromClipboard ()
					{
						std::string strReturn = "";

						return (strReturn);
					}

					void s13OpenDefaultWebBrowser (std::string strURL)
					{
					}

					std::vector<std::string> s13SearchForFiles (std::string strSearchFor)
					{
						std::vector<std::string> aryReturn;

						return (aryReturn);
					}

					std::vector<std::string> s13SearchForDirectories (std::string strSearchFor)
					{
						std::vector<std::string> aryReturn;

						return (aryReturn);
					}

					bool s13Mkdir (std::string strString)
					{
						return (true);
					}

					bool s13MoveFile (std::string strExisting, std::string strNew)
					{
						return (true);
					}

					bool s13Rm(std::string strString)
					{
						return (true);
					}

					bool s13Rmdir (std::string strString)
					{
						return (true);
					}

					bool s13CopyFile (std::string strExisting, std::string strNew, bool bFailIfExists)
					{
						return (true);
					}

					bool s13IsFile (std::string strString)
					{
						bool bReturn = false;

						return (bReturn);
					}

					bool s13IsDirectory (std::string strString)
					{
						bool bReturn = false;

						return (bReturn);
					}

					std::string s13GetHomeDirectory ()
					{
						return ("");
					}
				#endif
			#endif
		#endif
	#endif

	void s13Error (std::string strLine)
	{
		#ifdef defAPI_OGRE3D
			Ogre::LogManager::getSingletonPtr ()->logMessage (strLine);
		#endif

		s13ErrorBox (strLine);
	}

	void s13CreateSettingsDirectory ()
	{
		std::string strDir = s13GetSettingsDirectory ();
		#ifdef defSYS_WIN32
			std::string strDelim = "\\";
			strDir += "\\mods";
		#else
			std::string strDelim = "/";
			strDir += "/mods";
		#endif
		std::vector<std::string> aryStrings = RadXmlCommon::s13StringSplit (strDir, strDelim);
		std::string strDirSoFar = "";

		for (unsigned int iIdx = 0; iIdx < aryStrings.size (); iIdx++)
		{
			strDirSoFar += (aryStrings.at (iIdx) + "/");

			if (strDirSoFar != "")
			{
				if (s13IsDirectory (strDirSoFar) == false)
					s13Mkdir (strDirSoFar);
			}
		}
	}

	std::string System_GetParentDirectoryFromFilePath (std::string strDir)
	{
		return (RadXmlCommon::System_NormalizeDirectoryPath (strDir, 1));
	}

	std::string System_NormalizeDirectoryPath (std::string strDir, int iExclude, bool bIncludeLastSlash)
	{
		#ifdef defSYS_WIN32
			std::string strChar = "\\";
		#else
			std::string strChar = "/";
		#endif

		std::vector<std::string> aryDirs;

		for (int iIdx = 0; iIdx < (int)strDir.size (); iIdx++)
		{
			if (strDir.at (iIdx) == '\\')
				strDir.at (iIdx) = '/';
		}

		if (strDir.find ("/") != std::string::npos)
			aryDirs = RadXmlCommon::s13SplitString (strDir, "/");

		for (int iIdx = 0; iIdx < ((int)aryDirs.size () - 1); iIdx++)
		{
			if (aryDirs.at (iIdx) == ".")
			{
				std::vector<std::string>::iterator itBegin = aryDirs.begin ();
				itBegin += iIdx;
				aryDirs.erase (itBegin);
				iIdx--;

				continue;
			}

			if (aryDirs.at (iIdx) == "..")
			{
				if ((iIdx - 1) > 0)
				{
					std::vector<std::string>::iterator itBegin = aryDirs.begin ();
					itBegin += (iIdx - 1);
					aryDirs.erase (itBegin);
					iIdx--;

					itBegin = aryDirs.begin ();
					itBegin += iIdx;
					aryDirs.erase (itBegin);
					iIdx--;

					continue;
				}
			}
		}

		std::string strReturn = "";

		#ifndef defSYS_WIN32
			//strReturn += "/";
		#endif

		for (int iIdx = 0; iIdx < ((int)aryDirs.size () - iExclude); iIdx++)
		{
			if ((iIdx + 1) == aryDirs.size ())
			{
				if (bIncludeLastSlash == true)
					strReturn += (aryDirs.at (iIdx) + strChar);
				else
					strReturn += aryDirs.at (iIdx);
			}
			else
				strReturn += (aryDirs.at (iIdx) + strChar);
		}

		return (strReturn);
	}
}

