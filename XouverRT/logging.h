#include <iostream>
#include <string_view>

#define LOGLEVEL_ERROR 4
#define LOGLEVEL_WARN 3
#define LOGLEVEL_INFO 2
#define LOGLEVEL_DEBUG 1
#define LOGLEVEL_FINE 0

#ifndef LOGLEVEL
#ifdef _DEBUG
#define LOGLEVEL LOGLEVEL_DEBUG
#else
#define LOGLEVEL LOGLEVEL_INFO
#endif
#endif

template <class T = void>
void logPrint(std::size_t level, std::string_view format) {
	if (level < LOGLEVEL)
		return;
	for (auto c : format)
		std::cout << c;
}

template <class T, class... Types>
void logPrint(std::size_t level, std::string_view format, T v, Types... args) {
	if (level < LOGLEVEL)
		return;
	for (std::size_t i = 0; i < format.size(); ++i) {
		if (format[i] == '#') {
			std::cout << v;
			logPrint(level, format.substr(i + 1, format.size()), args...);
			return;
		}
		std::cout << format[i];
	}
}

template <class... Types>
void debugPrint(const char* fmt, Types... args) {
	std::string fmtString = std::string(fmt);
	logPrint(LOGLEVEL_DEBUG, fmtString, args...);
}
