/* DO NOT EDIT THIS FILE - it is machine generated */
#include "es_hpcn_wormhole_Worm.h"
#include <worm.h>

//#define WH_JAVASTRING_UTF8 //Defines if the JavaString is converted to UTF-8 instead of UTF-16

#ifdef __cplusplus
extern "C" {
#endif
	/****************
	 * STATIC CONTENT *
	 ****************/

#define JWH_NUMTYPES 2
	static const ConnectionDataType _JWH_array8type  = {.type = ARRAY, .ext.arrayType = UINT8};
	static const ConnectionDataType _JWH_array16type = {.type = ARRAY, .ext.arrayType = UINT16};
	static int hasBeenInitialized = 1;

	/**************************
	 * STATIC-PARAMETRIC CONTENT *
	 **************************/

	/****************/
	/*
	 * Class:     es_hpcn_wormhole_Worm
	 * Method:    init
	 * Signature: ()I
	 */
	JNIEXPORT jint JNICALL Java_es_hpcn_wormhole_Worm_init
	(JNIEnv *env, jobject obj)
	{
		if (hasBeenInitialized) {
			ConnectionDataType type[2];

			type[0] = _JWH_array8type;
			type[1] = _JWH_array16type;

			WH_setup_types(JWH_NUMTYPES, type);

			hasBeenInitialized = WH_init();
		}

		return hasBeenInitialized;
	}

	/*
	 * Class:     es_hpcn_wormhole_Worm
	 * Method:    halt
	 * Signature: ()I
	 */
	JNIEXPORT jint JNICALL Java_es_hpcn_wormhole_Worm_halt
	(JNIEnv *env, jobject obj)
	{
		uint8_t ret = WH_halt();
		jint j = ret;

		return j;
	}

	/*
	 * Class:     es_hpcn_wormhole_Worm
	 * Method:    recv
	 * Signature: ([B)I
	 */
	JNIEXPORT jint JNICALL Java_es_hpcn_wormhole_Worm_recv___3B
	(JNIEnv *env, jobject obj, jbyteArray data)
	{
		MessageInfo mi;
		jboolean iscopy;
		mi.type = &_JWH_array8type;
		mi.size = (*env)->GetArrayLength(env, data);

		//int8_t* bytes = (*env)->GetByteArrayElements(env, data, &iscopy);
		int8_t *bytes = (*env)->GetPrimitiveArrayCritical(env, data, &iscopy);

		if (iscopy == JNI_TRUE) {
			fprintf(stderr, "msg has been copied!\n");
		}

		if (bytes == NULL) {
			fprintf(stderr, "JVM returned NULL!\n");
			return -1;
		}

		jint ret = WH_recv(bytes, &mi);
		//(*env)->ReleaseByteArrayElements(env,data,bytes,0);
		(*env)->ReleasePrimitiveArrayCritical(env, data, bytes, 0);

		return ret;
	}

	/*
	 * Class:     es_hpcn_wormhole_Worm
	 * Method:    send
	 * Signature: ([B)I
	 */
	JNIEXPORT jint JNICALL Java_es_hpcn_wormhole_Worm_send___3B
	(JNIEnv *env, jobject obj, jbyteArray data)
	{
		MessageInfo mi;
		jboolean iscopy;
		mi.type = &_JWH_array8type;
		mi.size = (*env)->GetArrayLength(env, data);

		//int8_t* bytes = (*env)->GetByteArrayElements(env, data, &iscopy);
		int8_t *bytes = (*env)->GetPrimitiveArrayCritical(env, data, &iscopy);

		if (iscopy == JNI_TRUE) {
			fprintf(stderr, "msg has been copied!\n");
		}

		if (bytes == NULL) {
			fprintf(stderr, "JVM returned NULL!\n");
			return -1;
		}

		jint ret = WH_send(bytes, &mi);
		//(*env)->ReleaseByteArrayElements(env,data,bytes,JNI_ABORT);
		(*env)->ReleasePrimitiveArrayCritical(env, data, bytes, JNI_ABORT);

		return ret;
	}

	/*
	 * Class:     es_hpcn_wormhole_Worm
	 * Method:    recv
	 * Signature: (Ljava/lang/String;)I
	 */
	JNIEXPORT jint JNICALL Java_es_hpcn_wormhole_Worm_recv__Ljava_lang_String_2
	(JNIEnv *env, jobject obj, jstring data)
	{
		MessageInfo mi;
		jboolean iscopy;

#ifdef WH_JAVASTRING_UTF8
		mi.type = &_JWH_array8type;
		mi.size = (*env)->GetStringUTFLength(env, data);
		uint8_t  *bytes = (uint8_t *)(*env)->GetStringUTFChars(env, data, &iscopy);
#else
		mi.type = &_JWH_array16type;
		mi.size = (*env)->GetStringLength(env, data);
		uint16_t *bytes = (uint16_t *)(*env)->GetStringCritical(env, data, &iscopy);
#endif

		if (iscopy == JNI_TRUE) {
			fprintf(stderr, "msg has been copied!\n");
		}

		if (bytes == NULL) {
			fprintf(stderr, "JVM returned NULL!\n");
			return -1;
		}

		jint ret = WH_recv(bytes, &mi);

#ifdef WH_JAVASTRING_UTF8
		(*env)->ReleaseStringUTFChars(env, data, (const char *)bytes);
#else
		(*env)->ReleaseStringCritical(env, data,              bytes);
#endif

		return ret;
	}

	/*
	 * Class:     es_hpcn_wormhole_Worm
	 * Method:    send
	 * Signature: (Ljava/lang/String;)I
	 */
	JNIEXPORT jint JNICALL Java_es_hpcn_wormhole_Worm_send__Ljava_lang_String_2
	(JNIEnv *env, jobject obj, jstring data)
	{
		MessageInfo mi;
		jboolean iscopy;
		mi.type = &_JWH_array16type;
		mi.size = (*env)->GetStringLength(env, data);

		uint16_t *bytes = (uint16_t *)(*env)->GetStringCritical(env, data, &iscopy);

		if (iscopy == JNI_TRUE) {
			fprintf(stderr, "msg has been copied!\n");
		}

		if (bytes == NULL) {
			fprintf(stderr, "JVM returned NULL!\n");
			return -1;
		}

		jint ret = WH_send(bytes, &mi);
		(*env)->ReleaseStringCritical(env, data, bytes);

		return ret;
	}

	/*
	 * Class:     es_hpcn_wormhole_Worm
	 * Method:    flushIO
	 * Signature: ()V
	 */
	JNIEXPORT void JNICALL Java_es_hpcn_wormhole_Worm_flushIO
	(JNIEnv *env, jobject obj)
	{
		WH_flushIO();
	}

	/*
	 * Class:     es_hpcn_wormhole_Worm
	 * Method:    getId
	 * Signature: ()I
	 */
	JNIEXPORT jint JNICALL Java_es_hpcn_wormhole_Worm_getId
	(JNIEnv *env, jobject obj)
	{
		uint8_t ret = WH_get_id();
		jint j = ret;

		return j;
	}

#ifdef __cplusplus
}
#endif
