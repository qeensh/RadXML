#ifndef _RADXML_DATATYPES_H_
	#define _RADXML_DATATYPES_H_

	#ifdef defAPI_OGRE3D
		#ifdef RXVECTOR2
			#undef RXVECTOR2
		#endif

		#define RXVECTOR2			Ogre::Vector2

		#ifdef RXVECTOR3
			#undef RXVECTOR3
		#endif

		#define RXVECTOR3			Ogre::Vector3

		#ifdef RXVECTOR4
			#undef RXVECTOR4
		#endif

		#define RXVECTOR4			Ogre::Vector4

		#ifdef RXQUATERNION
			#undef RXQUATERNION
		#endif

		#define RXQUATERNION		Ogre::Quaternion
	#endif

	#if (!(defined (RXVECTOR2)) && !(defined (RXVECTOR3)) && !(defined (RXVECTOR4)))
		#include "common/mat_vectors.h"

		#define RXVECTOR2			s13Vector2
		#define RXVECTOR3			s13Vector3
		#define RXVECTOR4			s13Vector4
	#endif
#endif

