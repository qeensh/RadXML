#include "common/com_radxml_sound.h"

#ifdef defAPI_OPENAL
	#ifdef defAPI_OPENAL_NOT_IN_DIRECTORY
		#if ((!defined (defSYS_IPHONE)) && (!defined (defSYS_MACOSX)))
			#include <AL/al.h>
			#include <AL/alc.h>
		#else
			#include <OpenAL/al.h>
			#include <OpenAL/alc.h>
		#endif
	#else
		#include <al.h>
		#include <alc.h>
	#endif
#endif

#ifdef defAPI_OGRE3D
	#ifdef defAPI_OGRE3D_NOT_IN_DIRECTORY
		#include <OGRE/OgreLogManager.h>
		#include <OGRE/OgreStringConverter.h>
	#else
		#include <OgreLogManager.h>
		#include <OgreStringConverter.h>
	#endif
#endif

#ifdef defAPI_OPENMP
	#include <omp.h>
#endif

#ifndef defAUDIO_BUFFER_SIZE
	#define defAUDIO_BUFFER_SIZE			32768
#endif

#ifndef defAUDIO_STREAMING_BUFFER_SIZE
	#define defAUDIO_STREAMING_BUFFER_SIZE	32768
#endif

namespace RadXmlCommon
{
	// s13Sound
	s13Sound::s13Sound ()
	{
		strName = "";
		bEnabled = true;
		bLoop = false;
		strPath = "";
		uiBufferId[0] = 0;
		uiBufferId[1] = 0;
		uiSourceId = 0;
		bLoaded = false;
		bLoadNow = false;
		bStreaming = false;
		ulStreamingNextPos = 0;
		iFormat = -1;
		iFrequency = -1;
		iBitStream = -1;

		dPos[0] = 0.0;
		dPos[1] = 0.0;
		dPos[2] = 0.0;

		dOrientation[0] = 0.0;
		dOrientation[1] = 0.0;
		dOrientation[2] = 0.0;
		dOrientation[3] = 0.0;

		dGain = 1.0;
		dMinGain = 0.0;
		dMaxGain = 1.0;
		dMaxDist = 100000.0;
		dPitch = 0.0;

		dVelocity[0] = 0.0;
		dVelocity[1] = 0.0;
		dVelocity[2] = 0.0;

		dRolloffFactor = 0.0;
		dRot_Yaw = 0.0;
		dRot_Pitch = 0.0;
		dRot_Roll = 0.0;

		dDir[0] = 0.0;
		dDir[1] = 0.0;
		dDir[2] = 0.0;

		dCone[0] = 0.0;
		dCone[1] = 0.0;
		dCone[2] = 0.0;

		#ifdef defAPI_VORBIS_FILE
			viInfo = 0;
		#endif
	}

	s13Sound::s13Sound (string strFile, string strNamePass, bool bLoadNowpass)
	{
		strName = strNamePass;
		bEnabled = true;
		bLoop = false;
		strPath = strFile;
		uiBufferId[0] = 0;
		uiBufferId[1] = 0;
		uiSourceId = 0;
		bLoaded = false;
		bLoadNow = bLoadNowpass;
		bStreaming = false;
		ulStreamingNextPos = 0;
		iFormat = -1;
		iFrequency = -1;
		iBitStream = -1;

		dPos[0] = 0.0;
		dPos[1] = 0.0;
		dPos[2] = 0.0;

		dOrientation[0] = 0.0;
		dOrientation[1] = 0.0;
		dOrientation[2] = 0.0;
		dOrientation[3] = 0.0;

		dGain = 1.0;
		dMinGain = 0.0;
		dMaxGain = 1.0;
		dMaxDist = 100000.0;
		dPitch = 0.0;

		dVelocity[0] = 0.0;
		dVelocity[1] = 0.0;
		dVelocity[2] = 0.0;

		dRolloffFactor = 0.0;
		dRot_Yaw = 0.0;
		dRot_Pitch = 0.0;
		dRot_Roll = 0.0;

		dDir[0] = 0.0;
		dDir[1] = 0.0;
		dDir[2] = 0.0;

		dCone[0] = 0.0;
		dCone[1] = 0.0;
		dCone[2] = 0.0;

		#ifdef defAPI_VORBIS_FILE
			viInfo = 0;
		#endif

		if (bLoadNow == true)
			LoadFile (strFile);
	}

	s13Sound::~s13Sound ()
	{
		Unload ();
	}

	string s13Sound::GetName ()
	{
		return (strName);
	}

	string s13Sound::GetPath ()
	{
		return (strPath);
	}

	void s13Sound::SetPos (double dX, double dY, double dZ)
	{
		dPos[0] = dX;
		dPos[1] = dY;
		dPos[2] = dZ;

		#ifdef defAPI_OPENAL
			alSourcefv (uiSourceId, AL_POSITION, (ALfloat *)dPos);
		#endif
	}

	void s13Sound::SetOrientation (double dW, double dX, double dY, double dZ)
	{
		dOrientation[0] = dW;
		dOrientation[1] = dX;
		dOrientation[2] = dY;
		dOrientation[3] = dZ;

		// FIXME: Add the ability to mess with these values in their seperate methods later...
		/*alSourcef (uiSourceId, AL_PITCH, dPitch);
		alSourcef (uiSourceId, AL_GAIN, dGain);
		alSourcef (uiSourceId, AL_MIN_GAIN, dMinGain);
		alSourcef (uiSourceId, AL_MAX_GAIN, dMaxGain);
		alSourcefv (uiSourceId, AL_VELOCITY, (ALfloat *)dVelocity);*/

		#ifdef defAPI_OPENAL
			alSourcefv (uiSourceId, AL_ORIENTATION, (ALfloat *)dOrientation);
		#endif
	}

	string s13Sound::LoadPath ()
	{
		return (LoadFile (strPath));
	}

	string s13Sound::LoadFile (string strFile)
	{
		strPath = strFile;

		int iIndex = strFile.rfind (".ogg");

		if (iIndex != -1)
			return (LoadOGG (strFile));

		/*iIndex = strFile.rfind (".wav");

		if (iIndex != -1)
			return (LoadWAV (strFile));*/

		#ifdef __OBJC__
			#if (defined (defSYS_IPHONE) || defined (defSYS_MACOSX))
				if ((strFile.rfind (".aif") != string::npos) || 
					(strFile.rfind (".caf") != string::npos) || 
					(strFile.rfind (".wav") != string::npos) || 
					(strFile.rfind (".mp3") != string::npos) || 
					(strFile.rfind (".m4a") != string::npos) || 
					(strFile.rfind (".aac") != string::npos))
					return (Apple_LoadFile (strFile));
			#endif
		#endif

		strLastError = "s13Sound: Unable to determine file type.\nFile: " + strFile;

		return (strLastError);
	}

	string s13Sound::LoadWAV (string strFile)
	{
		/*ifstream ifsFile (strFile.c_str (), (ios::in | ios::binary));
		unsigned char cHeader[4];
		unsigned char cHeaderTemp[5];
		//unsigned long ulFormatLength = 0;

		if (ifsFile.is_open () == false)
		{
			ifsFile.close ();
			bLoaded = false;

			return (strLastError = "s13Sound: Unable to open file.\nFile: " + strFile);
		}

		ifsFile.read ((char *)cHeader, sizeof (cHeader));

		for (unsigned long ulIdx = 0; ulIdx < 4; ulIdx++)
			cHeaderTemp[ulIdx] = cHeader[ulIdx];

		cHeaderTemp[4] = '\0';

		string strTemp = (char *)cHeaderTemp;

		if (strTemp.compare ("RIFF") != 0)
		{
			ifsFile.close ();
			bLoaded = false;

			return (strLastError = "s13Sound: Unable to verify WAV header.\nFile: " + strFile);
		}

		unsigned long ulFormatSize = 0;
		ifsFile.read ((char *)&ulFormatSize, sizeof (unsigned long));
		ifsFile.read ((char *)cHeader, sizeof (cHeader));

		for (unsigned long ulIdx = 0; ulIdx < 4; ulIdx++)
			cHeaderTemp[ulIdx] = cHeader[ulIdx];

		cHeaderTemp[4] = '\0';
		strTemp = (char *)cHeaderTemp;*/

		/*if (strTemp.compare ("WAVE") == false)
		{
			ifsFile.close ();
			bLoaded = false;

			return (strLastError = "s13Sound: Unable to verify WAVE header.\nFile: " + strFile);
		}*/

		/*ifsFile.read ((char *)cHeader, sizeof (cHeader));

		for (unsigned long ulIdx = 0; ulIdx < 4; ulIdx++)
			cHeaderTemp[ulIdx] = cHeader[ulIdx];

		cHeaderTemp[4] = '\0';
		strTemp = (char *)cHeaderTemp;

		// FIXME: This is not always going to be true...
		if (strTemp.compare ("fmt ") == false)
		{
			ifsFile.close ();
			bLoaded = false;

			return (strLastError = "s13Sound: Unable to verify WAVE header.\nFile: " + strFile);
		}

		unsigned long ulPCMHeaderLength = 0;
		unsigned short usFormat = 0;
		unsigned short usNumOfChannels = 0;
		unsigned long ulSamplesPerSec = 0;
		unsigned long ulAverageNumOfBytesPerSec = 0;
		unsigned short usBlockAlignment = 0;
		unsigned short usNumOfBitsPerSample = 0;

		ifsFile.read ((char *)&ulPCMHeaderLength, sizeof (unsigned long));
		ifsFile.read ((char *)&usFormat, sizeof (unsigned short));
		ifsFile.read ((char *)&usNumOfChannels, sizeof (unsigned short));
		ifsFile.read ((char *)&ulSamplesPerSec, sizeof (unsigned long));
		ifsFile.read ((char *)&ulAverageNumOfBytesPerSec, sizeof (unsigned long));
		ifsFile.read ((char *)&usBlockAlignment, sizeof (unsigned short));
		ifsFile.read ((char *)&usNumOfBitsPerSample, sizeof (unsigned short));

		unsigned long ulSoundDataSize = 0;

		ifsFile.read ((char *)cHeader, sizeof (cHeader));
		ifsFile.read ((char *)&ulSoundDataSize, sizeof (unsigned long));

		unsigned char *ucSoundData = new unsigned char [ulSoundDataSize];

		for (unsigned int uiIdx = 0; uiIdx < ulSoundDataSize; uiIdx++)
			ucSoundData[uiIdx] = 0x00;

		ifsFile.read ((char *)ucSoundData, (sizeof (unsigned char) * ulSoundDataSize));
		ifsFile.close ();

		#ifdef defAPI_OPENAL
			if (usNumOfChannels == 1)
				iFormat = AL_FORMAT_MONO16;
			else
				iFormat = AL_FORMAT_STEREO16;
		#endif

		iFrequency = ulSamplesPerSec;
		iBitStream = usNumOfBitsPerSample;

		//for (unsigned int uiIdx = 0; uiIdx < ulSoundDataSize; uiIdx++)
		//	arySoundBuffer1.push_back (ucSoundData[uiIdx]);
		arySoundBuffer1.insert (arySoundBuffer1.end (), (char *)ucSoundData, ((char *)ucSoundData + ulSoundDataSize));

		#ifdef defAPI_OPENAL
			alGenBuffers (1, &uiBufferId);
			alGenSources (1, &uiSourceId);

			alBufferData(uiBufferId, iFormat, &arySoundBuffer1[0], (ALsizei)(arySoundBuffer1.size()), iFrequency);
			alSourcei (uiSourceId, AL_BUFFER, uiBufferId);

			arySoundBuffer1.clear ();
			arySoundBuffer2.clear ();
		#endif

		bLoaded = true;*/

		return (strLastError = "");
	}

	string s13Sound::LoadOGG (string strFile)
	{
		FILE *fFile = 0;
		bool bClose = true;
		bool bFirstTime = false;

		if (bStreaming == true)
			bClose = false;

		if (ulStreamingNextPos == 0)
		{
			#ifdef defVCNET2K8
				fopen_s (&fFile, strFile.c_str (), "rb");
			#else
				fFile = fopen (strFile.c_str (), "rb");
			#endif

			if (fFile == 0)
			{
				bLoaded = false;
				strLastError = "s13Sound: Unable to open file.\nFile: " + strFile;

				return (strLastError);
			}

			bFirstTime = true;
		}

		int iBufferSize1 = 0;
		int iBufferSize2 = 0;
		char *aryTempBuffer1 = 0;
		char *aryTempBuffer2 = 0;
		unsigned long ulDataSize = 0;
		vector<char> arySoundBuffer;

		#ifdef defAPI_VORBIS_FILE
			if (ulStreamingNextPos == 0)
			{
				ov_open (fFile, &ovfFile, 0, 0);
				viInfo = ov_info (&ovfFile, -1);
			}

			if (viInfo == 0)
			{
				bLoaded = false;
				strLastError = "s13Sound: Unable to verify OGG header.\nFile: " + strFile;
				//Ogre::LogManager::getSingleton ().logMessage (strLastError);
				/// @todo Output error message to log file
				ov_clear (&ovfFile);

				return (strLastError);
			}

			#ifdef defAPI_OPENAL
				if (viInfo->channels == 1)
					iFormat = AL_FORMAT_MONO16;
				else
					iFormat = AL_FORMAT_STEREO16;
			#endif

			iFrequency = viInfo->rate;
			long lBytes = 1;
			int iBigEndian = 0;

			// NOTE: The 0 is little-endian, 1 is big-endian
			#if (defined (defARCH_PPC) || defined (defARCH_PPC_64))
				iBigEndian = 1;
			#endif

			if (bStreaming == true)
			{
				if (bFirstTime == true)
				{
					aryTempBuffer1 = new char[defAUDIO_STREAMING_BUFFER_SIZE];

					while (iBufferSize1 < defAUDIO_STREAMING_BUFFER_SIZE)
					{
						lBytes = ov_read (&ovfFile, (aryTempBuffer1 + iBufferSize1), (defAUDIO_STREAMING_BUFFER_SIZE - iBufferSize1), iBigEndian, 2, 1, &iBitStream);

						if (lBytes > 0)
						{
							iBufferSize1 += lBytes;
							ulStreamingNextPos += (lBytes + 1);
						}
						else
							break;
					}

					aryTempBuffer2 = new char[defAUDIO_STREAMING_BUFFER_SIZE];

					while (iBufferSize2 < defAUDIO_STREAMING_BUFFER_SIZE)
					{
						lBytes = ov_read (&ovfFile, (aryTempBuffer2 + iBufferSize2), (defAUDIO_STREAMING_BUFFER_SIZE - iBufferSize2), iBigEndian, 2, 1, &iBitStream);

						if (lBytes > 0)
						{
							iBufferSize2 += lBytes;
							ulStreamingNextPos += (lBytes + 1);
						}
						else
							break;
					}
				}
			}
			else
			{
				while (lBytes > 0)
				{
					aryTempBuffer1 = new char[defAUDIO_BUFFER_SIZE];
					lBytes = ov_read (&ovfFile, aryTempBuffer1, defAUDIO_BUFFER_SIZE, iBigEndian, 2, 1, &iBitStream);
					arySoundBuffer.insert (arySoundBuffer.end (), aryTempBuffer1, (aryTempBuffer1 + lBytes));

					delete []aryTempBuffer1;
					aryTempBuffer1 = 0;
				}
			}
		#endif

		#ifdef defAPI_OPENAL
			if (bStreaming == true)
			{
				if (bFirstTime == true)
				{
					alGenBuffers (2, uiBufferId);
					alGenSources (1, &uiSourceId);

					alBufferData (uiBufferId[0], iFormat, aryTempBuffer1, iBufferSize1, iFrequency);
					alBufferData (uiBufferId[1], iFormat, aryTempBuffer2, iBufferSize2, iFrequency);
					alSourceQueueBuffers (uiSourceId, 2, uiBufferId);

					delete []aryTempBuffer1;
					aryTempBuffer1 = 0;
					delete []aryTempBuffer2;
					aryTempBuffer2 = 0;
				}
				else
				{
					#ifdef defAPI_VORBIS_FILE
						int iProcessed = GetNumBuffersProcessed ();

						for (int iIdx = (iProcessed - 1); iIdx >= 0; iIdx--)
						{
							unsigned int uiBuff = 0;
							alSourceUnqueueBuffers (uiSourceId, 1, &uiBuff);

							iBufferSize1 = 0;
							aryTempBuffer1 = new char[defAUDIO_STREAMING_BUFFER_SIZE];

							while (iBufferSize1 < defAUDIO_STREAMING_BUFFER_SIZE)
							{
								lBytes = ov_read (&ovfFile, (aryTempBuffer1 + iBufferSize1), (defAUDIO_STREAMING_BUFFER_SIZE - iBufferSize1), iBigEndian, 2, 1, &iBitStream);

								if (lBytes > 0)
								{
									iBufferSize1 += lBytes;
									ulStreamingNextPos += (lBytes + 1);
								}
								else
								{
									if (bLoop == false)
									{
										bClose = true;

										break;
									}
									else
									{
										ov_raw_seek (&ovfFile, 0);
										iBufferSize1 = 0;
										ulStreamingNextPos = 1;
									}
								}
							}

							alBufferData (uiBuff, iFormat, aryTempBuffer1, iBufferSize1, iFrequency);
							alSourceQueueBuffers (uiSourceId, 1, &uiBuff);

							delete []aryTempBuffer1;
							aryTempBuffer1 = 0;

							if (bClose == true)
								break;
						}
					#endif
				}
			}
			else
			{
				alGenBuffers (1, &uiBufferId[0]);
				alGenSources (1, &uiSourceId);

				alBufferData (uiBufferId[0], iFormat, &arySoundBuffer[0], arySoundBuffer.size (), iFrequency);
				alSourcei (uiSourceId, AL_BUFFER, uiBufferId[0]);

				arySoundBuffer.clear ();
			}
		#endif

		#ifdef defAPI_VORBIS_FILE
			if (bClose == true)
				ov_clear (&ovfFile);
		#endif

		bLoaded = true;

		return (strLastError = "");
	}

	#ifdef __OBJC__
	#if (defined (defSYS_IPHONE) || defined (defSYS_MACOSX))
	string s13Sound::Apple_LoadFile (string strFile)
	{
		bool bClose = true;
		bool bFirstTime = false;
		OSStatus osError;

		if (bStreaming == true)
			bClose = false;

		if (ulStreamingNextPos == 0)
		{
			AudioFileTypeID idFileType = 0;

			if (strFile.rfind (".aif") != std::string::npos)
				idFileType = kAudioFileAIFFType;

			if (strFile.rfind (".mp3") != std::string::npos)
				idFileType = kAudioFileMP3Type;

			if (strFile.rfind (".wav") != std::string::npos)
				idFileType = kAudioFileWAVEType;

			if (strFile.rfind (".m4a") != std::string::npos)
				idFileType = kAudioFileM4AType;

			if (strFile.rfind (".caf") != std::string::npos)
				idFileType = kAudioFileCAFType;

			#ifndef defSYS_MACOSX_USING_10_4_SDK
				NSURL *nuUrl = [NSURL fileURLWithPath:[NSString stringWithUTF8String: strFile.c_str ()]];
			#endif

			#ifdef defSYS_IPHONE
				osError = AudioFileOpenURL ((CFURLRef)nuUrl, kAudioFileReadPermission, idFileType, &afiId);
			#else
				#ifdef defSYS_MACOSX_USING_10_4_SDK
					FSRef frFileRef;
					FSPathMakeRef ((const UInt8 *)[[NSString stringWithUTF8String: strFile.c_str ()] fileSystemRepresentation], 
								   &frFileRef, false);
					osError = AudioFileOpen (&frFileRef, fsRdPerm, idFileType, &afiId);
				#else
					osError = AudioFileOpenURL ((CFURLRef)nuUrl, fsRdPerm, idFileType, &afiId);
				#endif
			#endif

			if (osError != 0)
			{
				strLastError = "s13Sound: Unable to load sound file. Filename: " + strPath;

				return (strLastError);
			}
		}

		UInt32 u32DataSize = 0;
		
		if (ulStreamingNextPos == 0)
		{
			unsigned long long u64DataSize = 0;
			iFrequency = 0;
			UInt32 uiProperty_DataSize = sizeof (unsigned long long);
			AudioFileGetProperty (afiId, kAudioFilePropertyAudioDataByteCount, &uiProperty_DataSize, &u64DataSize);
			
			AudioStreamBasicDescription asbDesc;
			UInt32 ulProperty_Desc = sizeof (asbDesc);
			AudioFileGetProperty (afiId, kAudioFilePropertyDataFormat, &ulProperty_Desc, &asbDesc);

			u32DataSize = u64DataSize;
			iFrequency = asbDesc.mSampleRate;

			#ifdef defAPI_OPENAL
				if (asbDesc.mBitsPerChannel == 8)
				{
					if (asbDesc.mChannelsPerFrame == 1)
						iFormat = AL_FORMAT_MONO8;
					else if (asbDesc.mChannelsPerFrame == 2)
						iFormat = AL_FORMAT_STEREO8;
				}
				else if (asbDesc.mBitsPerChannel == 16)
				{
					if (asbDesc.mChannelsPerFrame == 1)
						iFormat = AL_FORMAT_MONO16;
					else if (asbDesc.mChannelsPerFrame == 2)
						iFormat = AL_FORMAT_STEREO16;
				}
			#endif
		}

		if (bStreaming == true)
		{
			char *aryTempBuffer1 = 0;
			//int iBufferSize1 = 0;
			char *aryTempBuffer2 = 0;
			//int iBufferSize2 = 0;
			u32DataSize = defAUDIO_STREAMING_BUFFER_SIZE;

			if (ulStreamingNextPos == 0)
			{
				aryTempBuffer1 = new char [u32DataSize];
				AudioFileReadBytes (afiId, false, 0, &u32DataSize, aryTempBuffer1);
				ulStreamingNextPos += u32DataSize;

				aryTempBuffer2 = new char [u32DataSize];
				AudioFileReadBytes (afiId, false, ulStreamingNextPos, &u32DataSize, aryTempBuffer2);
				ulStreamingNextPos += u32DataSize;

				#ifdef defAPI_OPENAL
					alGenBuffers (2, uiBufferId);
					alGenSources (1, &uiSourceId);
					
					alBufferData (uiBufferId[0], iFormat, aryTempBuffer1, u32DataSize, iFrequency);
					alBufferData (uiBufferId[1], iFormat, aryTempBuffer2, u32DataSize, iFrequency);
					alSourceQueueBuffers (uiSourceId, 2, uiBufferId);
				#endif
			}
			else
			{
				int iProcessed = GetNumBuffersProcessed ();

				for (int iIdx = (iProcessed - 1); iIdx >= 0; iIdx--)
				{
					#ifdef defAPI_OPENAL
						unsigned int uiBuff = 0;
						alSourceUnqueueBuffers (uiSourceId, 1, &uiBuff);
					#endif

					aryTempBuffer1 = new char [u32DataSize];

					while (1)
					{
						AudioFileReadBytes (afiId, false, ulStreamingNextPos, &u32DataSize, aryTempBuffer1);

						if (u32DataSize > 0)
						{
							ulStreamingNextPos += u32DataSize;

							break;
						}
						else
						{
							if (bLoop == false)
							{
								bClose = true;

								break;
							}
							else
							{
								ulStreamingNextPos = 0;
								u32DataSize = defAUDIO_STREAMING_BUFFER_SIZE;

								continue;
							}
						}
					}

					if (bClose == true)
						break;

					#ifdef defAPI_OPENAL
						alBufferData (uiBuff, iFormat, aryTempBuffer1, u32DataSize, iFrequency);
						alSourceQueueBuffers (uiSourceId, 1, &uiBuff);
					#endif

					delete []aryTempBuffer1;
					aryTempBuffer1 = 0;
				}
			}

			if (aryTempBuffer1 != 0)
			{
				delete []aryTempBuffer1;
				aryTempBuffer1 = 0;
			}

			if (aryTempBuffer2 != 0)
			{
				delete []aryTempBuffer2;
				aryTempBuffer2 = 0;
			}
		}
		else
		{
			char *aryTempBuffer = new char [u32DataSize];
			AudioFileReadBytes (afiId, false, 0, &u32DataSize, aryTempBuffer);

			#ifdef defAPI_OPENAL
				alGenBuffers (1, &uiBufferId[0]);
				alGenSources (1, &uiSourceId);

				// FIXME: (iFrequency / 32) is incorrect, seems to only work well with WAV files.
				alBufferData (uiBufferId[0], iFormat, aryTempBuffer, u32DataSize, iFrequency);
				alSourcei (uiSourceId, AL_BUFFER, uiBufferId[0]);
			#endif

			delete []aryTempBuffer;
			aryTempBuffer = 0;
		}

		if (bClose == true)
			AudioFileClose (afiId);

		bLoaded = true;

		return (strLastError = "");
	}
	#endif
	#endif

	string s13Sound::Play ()
	{
		if (bLoaded == false)
		{
			strLastError = "s13Sound: Sound not loaded. Filename: " + strPath;

			return (strLastError);
		}

		if (bEnabled == false)
			return ("");

		#ifdef defAPI_OPENAL
			alSourcePlay (uiSourceId);
		#endif

		return ("");
	}

	void s13Sound::Stop ()
	{
		if (bLoaded == false)
			return;

		#ifdef defAPI_OPENAL
			alSourceStop (uiSourceId);
		#endif
	}

	void s13Sound::SetPitch (double dNewPitch)
	{
		dPitch = dNewPitch;

		#ifdef defAPI_OPENAL
			alSourcef (uiSourceId, AL_PITCH, dPitch);
		#endif
	}

	void s13Sound::SetGain (double dNewGain)
	{
		dGain = dNewGain;

		#ifdef defAPI_OPENAL
			alSourcef (uiSourceId, AL_GAIN, dGain);
		#endif
	}

	void s13Sound::SetMinGain (double dNewMinGain)
	{
		dMinGain = dNewMinGain;

		#ifdef defAPI_OPENAL
			alSourcef (uiSourceId, AL_MIN_GAIN, dMinGain);
		#endif
	}

	void s13Sound::SetMaxGain (double dNewMaxGain)
	{
		dMaxGain = dNewMaxGain;

		#ifdef defAPI_OPENAL
			alSourcef (uiSourceId, AL_MAX_GAIN, dMaxGain);
		#endif
	}

	void s13Sound::SetVelocity (double dNewVelocityX, double dNewVelocityY, double dNewVelocityZ)
	{
		dVelocity[0] = dNewVelocityX;
		dVelocity[1] = dNewVelocityY;
		dVelocity[2] = dNewVelocityZ;

		#ifdef defAPI_OPENAL
			alSource3f (uiSourceId, AL_VELOCITY, dVelocity[0], dVelocity[1], dVelocity[2]);
		#endif
	}

	void s13Sound::Unload ()
	{
		#ifdef defAPI_OPENAL
			alSourceStop (uiSourceId);
			alDeleteSources (1, &uiSourceId);

			if (bStreaming == true)
			{
				int iQueued = 0;
				alGetSourcei (uiSourceId, AL_BUFFERS_QUEUED, &iQueued);

				for (int iIdx = iQueued; iIdx >= 0; iIdx--)
				{
					unsigned int uiBuff = 0;
					alSourceUnqueueBuffers (uiSourceId, 1, &uiBuff);
				}

				alDeleteBuffers (2, uiBufferId);
			}
			else
				alDeleteBuffers (1, &uiBufferId[0]);
		#endif

		bLoaded = false;
	}

	void s13Sound::Volume (float fValue)
	{
		#ifdef defAPI_OPENAL
			alSourcef (uiSourceId, AL_GAIN, fValue);
		#endif
	}

	void s13Sound::FadeOut (float fStep)
	{
		#ifdef defAPI_OPENAL
			float fPrevValue = 0.0f;
			float fResult = 0.0f;

			alGetSourcef (uiSourceId, AL_GAIN, &fPrevValue);

			if (fPrevValue <= 0.0f)
				return;

			if ((fPrevValue - fStep) < 0.0f)
				fResult = 0.0f;
			else
				fResult = (fPrevValue - fStep);

			alSourcef (uiSourceId, AL_GAIN, fResult);
		#endif
	}

	void s13Sound::FadeIn (float fStep)
	{
		#ifdef defAPI_OPENAL
			float fPrevValue = 0.0f;
			float fResult = 0.0f;

			alGetSourcef (uiSourceId, AL_GAIN, &fPrevValue);

			if (fPrevValue >= 1.0f)
				return;

			if ((fPrevValue + fStep) > 1.0f)
				fResult = 1.0f;
			else
				fResult = (fPrevValue + fStep);

			alSourcef (uiSourceId, AL_GAIN, fResult);
		#endif
	}

	int s13Sound::GetNumBuffersProcessed ()
	{
		int iReturn = 0;

		#ifdef defAPI_OPENAL
			alGetSourcei (uiSourceId, AL_BUFFERS_PROCESSED, &iReturn);
		#endif

		return (iReturn);
	}

	bool s13Sound::IsPlaying ()
	{
		int iState = 0;
		bool bIsPlaying = false;

		#ifdef defAPI_OPENAL
			alGetSourcei (uiSourceId, AL_SOURCE_STATE, &iState);

			if (iState == AL_PLAYING)
				bIsPlaying = true;
		#endif

		return (bIsPlaying);
	}

	bool s13Sound::IsLoaded ()
	{
		return (bLoaded);
	}

	string s13Sound::GetLastError ()
	{
		return (strLastError);
	}
}

