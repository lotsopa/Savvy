#ifndef SAVVY_DATA_TYPES_H
#define SAVVY_DATA_TYPES_H

#define SAVVY_INLINE __forceinline

#ifndef NULL
#define NULL 0
#endif

namespace Savvy
{
	// Some typedefs
	typedef int int32;
	typedef unsigned int uint32;
	typedef short int16;
	typedef unsigned short uint16;
	typedef long long int64;
	typedef unsigned long long uint64;
	typedef float float32;
	typedef double float64;
	typedef char char8;
	typedef wchar_t wchar;

	/*
	Result codes returned by the various functions of the tool in order to detect if the last operation was successful or not.
	*/
	enum ResultCode
	{
		SAVVY_OK = 0,
		SAVVY_INVALID_ARG,
		SAVVY_INVALID_FILE_PATH,
		SAVVY_INVALID_ENTRY_POINT,
		SAVVY_SHADER_SYNTAX_ERROR,
		SAVVY_UNSUPPORTED_FUNCTION,
		SAVVY_UNSUPPORTED_SHADER_TYPE,
		SAVVY_LOGICAL_ERROR,
		SAVVY_SHADER_TYPE_ALREADY_EXISTS,
		SAVVY_SHADER_LANG_ALREADY_EXISTS,
		SAVVY_INVALID_SHADER_TYPE,
		SAVVY_INVALID_SHADER_LANG,
		SAVVY_UNRECOGNIZED_TOKEN,
		SAVVY_MAX_RECURSION_DEPTH_EXCEEDED,
		SAVVY_UNKNOWN_ERROR
	};
}

#endif // !SAVVY_DATA_TYPES_H