#include <XNI.h>

#include <iostream>

#ifdef WIN32
#include <Windows.h>
#endif

int main() {
	num = 4;
	void* rt = createRuntime();

	runRuntime(rt, "Xouver.Test.TestClass", "xouver:Main()#void");

	if (getXrtError(rt) == XRT_Error::APP_EXCPETION_THROWN) {
		int size = getExpceptionMessageSize(rt);
		char* msg = new char[size];
		getExceptionMessage(rt, &msg);

		std::string str = "";

		for (int i = 0; i < size; i++) {
			str += msg[i];
		}

#ifdef WIN32
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFO info;
		WORD oldAttr;
		DWORD bytesWritten = 0;
		if (!GetConsoleScreenBufferInfo(handle, &info)) {
			std::cout << msg;
		}
		else {
			oldAttr = info.wAttributes;
			SetConsoleTextAttribute(handle, FOREGROUND_RED);
			WriteFile(handle, msg, size, &bytesWritten, NULL);


			SetConsoleTextAttribute(handle, oldAttr);
		}
#else
		std::cout << str;
#endif

		delete[] msg;
	}

	delete rt;
	return 0;
}

int num = 4;