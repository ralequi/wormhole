/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class es_hpcn_wormhole_Worm */

#ifndef _Included_es_hpcn_wormhole_Worm
#define _Included_es_hpcn_wormhole_Worm
#ifdef __cplusplus
extern "C" {
#endif
	/*
	 * Class:     es_hpcn_wormhole_Worm
	 * Method:    init
	 * Signature: ()I
	 */
	JNIEXPORT jint JNICALL Java_es_hpcn_wormhole_Worm_init
	(JNIEnv *, jobject);

	/*
	 * Class:     es_hpcn_wormhole_Worm
	 * Method:    halt
	 * Signature: ()I
	 */
	JNIEXPORT jint JNICALL Java_es_hpcn_wormhole_Worm_halt
	(JNIEnv *, jobject);

	/*
	 * Class:     es_hpcn_wormhole_Worm
	 * Method:    recv
	 * Signature: ([B)I
	 */
	JNIEXPORT jint JNICALL Java_es_hpcn_wormhole_Worm_recv
	(JNIEnv *, jobject, jbyteArray);

	/*
	 * Class:     es_hpcn_wormhole_Worm
	 * Method:    send
	 * Signature: ([B)I
	 */
	JNIEXPORT jint JNICALL Java_es_hpcn_wormhole_Worm_send
	(JNIEnv *, jobject, jbyteArray);

	/*
	 * Class:     es_hpcn_wormhole_Worm
	 * Method:    flushIO
	 * Signature: ()V
	 */
	JNIEXPORT void JNICALL Java_es_hpcn_wormhole_Worm_flushIO
	(JNIEnv *, jobject);

	/*
	 * Class:     es_hpcn_wormhole_Worm
	 * Method:    getId
	 * Signature: ()I
	 */
	JNIEXPORT jint JNICALL Java_es_hpcn_wormhole_Worm_getId
	(JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
