#ifndef _COM_RADXML_SOUND_H_
	#define _COM_RADXML_SOUND_H_

	#include <string>
	#include <vector>

	using namespace std;

	#ifdef defAPI_VORBIS_FILE
		#include <vorbis/vorbisfile.h>
	#endif

	#ifdef __OBJC__
		#if (defined (defSYS_IPHONE) || defined (defSYS_MACOSX))
			#include <Foundation/Foundation.h>
			#include <AudioToolbox/AudioToolbox.h>
		#endif
	#endif

	namespace RadXmlCommon
	{
		class s13Sound
		{
			public:
				s13Sound ();
				s13Sound (string strFile, string strNamePass = "", bool bLoadNowpass = false);
				~s13Sound ();

				string LoadFile (string strFile);
				string LoadPath ();
				string LoadWAV (string strFile);
				string LoadOGG (string strFile);
				#ifdef __OBJC__
					#if (defined (defSYS_IPHONE) || defined (defSYS_MACOSX))
						string Apple_LoadFile (string strFile);
					#endif
				#endif

				string Play ();
				void Stop ();

				void SetPitch (double dNewPitch);
				void SetGain (double dNewGain);
				void SetMinGain (double dNewMinGain);
				void SetMaxGain (double dNewMaxGain);
				void SetVelocity (double dNewVelocityX, double dNewVelocityY, double dNewVelocityZ);

				void Unload ();

				void Volume (float fValue);
				void FadeOut (float fStep);
				void FadeIn (float fStep);

				bool IsPlaying ();
				bool IsLoaded ();

				string GetLastError ();

				int GetNumBuffersProcessed ();
				string GetName ();
				string GetPath ();

				void SetPos (double dX, double dY, double dZ);
				void SetOrientation (double dW, double dX, double dY, double dZ);

				string strName;
				bool bEnabled;
				string strPath;

				unsigned int uiBufferId[2];
				unsigned int uiSourceId;

				string strLastError;
				bool bLoaded;
				bool bLoadNow;
				bool bStreaming;
				unsigned long ulStreamingNextPos;
				bool bLoop;
				int iFormat;
				int iFrequency;
				int iBitStream;

				double dPos[3];
				double dOrientation[4];
				double dRot_Yaw;
				double dRot_Pitch;
				double dRot_Roll;
				double dGain;
				double dMinGain;
				double dMaxGain;
				double dMaxDist;
				double dPitch;
				double dVelocity[3];
				double dRolloffFactor;
				double dDir[3];
				double dCone[3];

				#ifdef defAPI_VORBIS_FILE
					OggVorbis_File ovfFile;
					vorbis_info *viInfo;
				#endif

				#ifdef __OBJC__
					#if (defined (defSYS_IPHONE) || defined (defSYS_MACOSX))
						AudioFileID afiId;
					#endif
				#endif
		};
	}
#endif

