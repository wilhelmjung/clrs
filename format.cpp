#include <cstdarg>
#include <cstdio>
#include <vector>

#include "format.h"

#if 0
std::string
vformat(const char *fmt, va_list ap);

std::string
format_string(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	std::string buf = vformat(fmt, ap);
	va_end(ap);
	return buf;
}

std::string
vformat(const char *fmt, va_list ap)
{
	// Allocate a buffer on the stack that's big enough for us almost
	// all the time.
	size_t size = 1024;
	char* buf = new char[size];

	// Try to vsnprintf into our buffer.
	va_list apcopy;
	va_copy(apcopy, ap);
	int needed = vsnprintf(&buf[0], size, fmt, ap);
	// NB. On Windows, vsnprintf returns -1 if the string didn't fit the
	// buffer.  On Linux & OSX, it returns the length it would have needed.

	if (needed <= size && needed >= 0) {
		// It fit fine the first time, we're done.
		return std::string(&buf[0]);
	}
	else {
		// vsnprintf reported that it wanted to write more characters
		// than we allotted.  So do a malloc of the right size and try again.
		// This doesn't happen very often if we chose our initial size
		// well.
		std::vector <char> buf;
		size = needed;
		buf.resize(size);
		needed = vsnprintf(&buf[0], size, fmt, apcopy);
		return std::string(&buf[0]);
	}
}

#else

// format print for std::string
std::string format(const std::string fmt, ...)
{
	int size = 100;
	std::string str;
	va_list ap;
	while (1) {
		str.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *) str.c_str(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size) {
			str.resize(n);
			return str;
		}
		if (n > -1)
			size = n + 1;
		else
			size *= 2;
	}
	return str;
}

#endif