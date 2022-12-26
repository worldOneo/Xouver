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

	enum XNI_Error {
		NO_XNI_ERROR,
		FAILED_CREATION,
		FAILED_EXECUTION,
		POINTER_TO_NO_RT,
		NO_RT_ERROR
	};

	enum XRT_Error {
		NO_XRT_ERROR,
		INTERNAL_XNI_FAILURE,
		FAILED_HALT,
		CLASS_NOT_FOUND,
		FUNCTON_NOT_FOUND,
		POOL_INDEX_OUT_OF_RANGE,
		FIELD_INDEX_OUT_OF_RANGE,
		APP_EXCPETION_THROWN
	};

	struct xfunc_data {
	public:
		xvalues* params;
		xvalues returnValue;
	};

//#ifdef XRT {
	int getXMajorVersion();
	int getXMinorVersion();

	bool xniVerify(char c);

	void* createRuntime();

	bool xniHasError();
	bool xrtHasError(void* _rt);

	XNI_Error getXniError();
	XRT_Error getXrtError(void* _rt);


	void callFunction(void* _rt, const char* signature, xvalues* args, unsigned int argCount);
	void registerFunction(void* _rt, const char* signature, void (*fn)(void*, xfunc_data* data));

	void registerClass(void* _rt, const char* signature);

	void runRuntime(void* _rt, const char* mainClass, const char* mainFuncSignature);
	void haltCurrentProcess(void* _rt);
	void haltRuntime(void* _rt);

	void throwError(void* _rt, const char* msg);

	int getExpceptionMessageSize(void* _rt);
	void getExceptionMessage(void* _rt, char** outMsg);

	const xvalues& getStackTop(void* _rt);
/*#else
	extern int getXMajorVersion();
	extern int getXMinorVersion();

	extern bool xniVerify(char c);

	extern void* createRuntime();

	extern bool xniHasError();
	extern bool xrtHasError(void* _rt);

	extern XNI_Error getXniError();
	extern XRT_Error getXrtError();


	extern xvalues callFunction(void* _rt, const char* signature, xvalues* args);
	extern void registerFunction(void* _rt, const char* signature, void (*fn)(void*, xfunc_data* data));

	extern void runRuntime(void* _rt, const char* mainClass, const char* mainFuncSignature);
	extern xfunc_signature resolveSignature(const char* signature);
	extern void haltCurrentProcess(void* _rt);
	extern void haltRuntime(void* _rt);
#endif*/

#ifdef __cplusplus
}
#endif
#endif