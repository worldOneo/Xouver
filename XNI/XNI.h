/*

	Xouve Native Integrit Header

	This header contains functions and types
	which are used by the Xouver Runtime.

	Also, this header gives the ability to create,
	execute and maintain processes of the runtime


*/

#ifndef XNI_H
#define XNI_H

#ifdef __cplusplus
extern "C" {
#endif

	/*

		Runtime value definitions

	*/

	typedef int xint;
	typedef float xfloat;
	typedef bool xbool;
	typedef char xchar;

	typedef struct _xobject {};
	typedef _xobject* xobject;

	typedef union _xvalues {
		xint i;
		xfloat f;
		xbool b;
		xchar c;
		xobject o;
	} xvalues;


	/*

		Definitions for creating and maintaining a process of the runtime


	*/

	extern enum XNI_Error {
		NO_XNI_ERROR = 0,
		FAILED_RT_CREATION = 178,
		ARGUMENT_NOT_RT = 122,
		FAILED_EXECUTION = 183,
		POINTER_TO_NO_RT = 104,
		UNEXPECTED_BYTE = 184
	};

	enum XRT_Error {
		NO_XRT_ERROR,
		MEMORY_INITIALIZATION_FAILED,
		MEMORY_ALLOCATION_FAILED,
		EXCEPTION_THROWN
	};

	struct xfunc_data {
	public:
		xvalues* params;
		xvalues returnValue;
	};

	extern void* createRuntime(XNI_Error* error, unsigned char* bytes, int bytesCount);
	XNI_Error createClass(void* _rt, unsigned char* bytes, int bytesCount);
	extern const char* resolveRTError(XRT_Error error);
	extern const char* resolveXNIError(XNI_Error error);
	extern bool xniVerify(char c);
	extern int getXMajorVersion();
	extern int getXMinorVersion();
	extern void registerFunction(void* _rt, const char* signature, void (*fn)(void*));
	extern void callFunction(void* _rt, int index);
	extern void runRuntime(void* _rt, const char* mainClass, const char* mainFuncSignature);
	extern void haltRuntime(void* _rt);
	extern void throwError(void* _rt, const char* msg);
	extern int getExpceptionMessageSize(void* _rt);
	extern void getExceptionMessage(void* _rt, char** outMsg);

#ifdef __cplusplus
}
#endif
#endif