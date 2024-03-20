/*
G-Unicode - A LabVIEW unicode library.

Author - Dataflow_G
GitHub - https://github.com/dataflowg/g-unicode
Twitter - https://twitter.com/Dataflow_G

Some of the functions in this library should only be called from LabVIEW for Windows.
The functions are still defined for all platforms to avoid library loading issues on
non-Windows platforms, but when called will return the warning GE_W_WIN32_ONLY.
*/

#ifndef _G_UNICODE_H_
#define _G_UNICODE_H_

#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#if defined(_WIN32)
#include <io.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

#include "utf8.h"
#include "tinydir.h"
#define PFD_NO_ASYNC
#include "portable-file-dialogs.h"
#include "inputbox.h"

#if defined(_WIN32)
#define LV_DLL_IMPORT  __declspec(dllimport)
#define LV_DLL_EXPORT  __declspec(dllexport)
#define LV_DLL_PRIVATE static
#else
#if defined(__GNUC__) && __GNUC__ >= 4
#define LV_DLL_IMPORT  __attribute__((visibility("default")))
#define LV_DLL_EXPORT  __attribute__((visibility("default")))
#define LV_DLL_PRIVATE __attribute__((visibility("hidden")))
#else
#define LV_DLL_IMPORT
#define LV_DLL_EXPORT
#define LV_DLL_PRIVATE static
#endif
#endif

typedef int32_t gu_result;
typedef struct
{
	char* string;
	int32_t size;
} gu_string;

#define GU_SUCCESS				0
// WARNINGS
#define GU_W_WIN32_ONLY         1       // Result only valid on Windows
// ERRORS
#define GU_E_GENERIC            -1      // Generic error
#define GU_E_MEMORY             -2      // Memory allocation error
#define GU_E_INVALID_PARAMS     -3      // Invalid parameters
#define GU_E_INVALID_PATH       -4      // Invalid path / <Not A Path>
#define GU_E_RELATIVE_PATH      -5      // Path is relative, requires absolute
#define GU_E_FILE_IO            -6      // General file I/O error
#define GU_E_ALREADY_EXISTS     -7      // File/folder already exists

#define REALLOC_SIZE    50

////////////////////////////
// LabVIEW CLFN Callbacks //
////////////////////////////
extern "C" LV_DLL_EXPORT int32_t clfn_abort(void* data);

///////////////////////
// Memory Management //
///////////////////////
extern "C" LV_DLL_EXPORT void gu_free(intptr_t pointer);
void gu_free(void* pointer);
//void gu_free_string(gu_string* string_list, int32_t list_length);

/////////////////
// Unicode API //
/////////////////
extern "C" LV_DLL_EXPORT gu_result gu_strlen(const char* str, int32_t* length);
extern "C" LV_DLL_EXPORT gu_result gu_string_subset(const char* str, int32_t offset, int32_t length, intptr_t* substring_pointer, int32_t* substring_size);
extern "C" LV_DLL_EXPORT gu_result gu_to_upper_case(char* str);
extern "C" LV_DLL_EXPORT gu_result gu_to_lower_case(char* str);
extern "C" LV_DLL_EXPORT gu_result gu_reverse_string(char* str);
extern "C" LV_DLL_EXPORT gu_result gu_rotate_string(char* str);
extern "C" LV_DLL_EXPORT gu_result gu_search_split_string(const char* str, const char* search_str, int32_t offset, int32_t reverse, intptr_t* string_a_pointer, int32_t* string_a_length, intptr_t* string_b_pointer, int32_t* string_b_length);
extern "C" LV_DLL_EXPORT gu_result gu_string_to_codepoint_array(const char* str, intptr_t* codepoint_array, int32_t* codepoint_array_size);
extern "C" LV_DLL_EXPORT gu_result gu_codepoint_array_to_string(int32_t * codepoint_array, int32_t codepoint_array_size, intptr_t* str_pointer, int32_t* str_size);
extern "C" LV_DLL_EXPORT gu_result gu_normalize_string(const char* str, uint16_t form, intptr_t* normalized_pointer, int32_t* normalized_size);
//extern "C" LV_DLL_EXPORT gu_result gu_is_normalized_string(const char* str, uint16_t form, int32_t* is_normalized);

extern "C" LV_DLL_EXPORT gu_result gu_is_text_utf16(const char* buf, int32_t size, int32_t* is_utf16);
extern "C" LV_DLL_EXPORT gu_result gu_is_text_utf8(const char* str, int32_t* is_utf8);

////////////////
// Dialog API //
////////////////
extern "C" LV_DLL_EXPORT gu_result gu_open_file_dialog(const char* title, const char* default_path, int32_t num_filter_patterns, const char* filter_patterns, const char* filter_description, int32_t allow_multi_select, int32_t* cancelled, intptr_t* path_pointer, int32_t* path_length, intptr_t* paths_pointer, int32_t* num_paths);
extern "C" LV_DLL_EXPORT gu_result gu_select_folder_dialog(const char* title, const char* default_path, int32_t* cancelled, intptr_t* path_pointer, int32_t* path_length);
extern "C" LV_DLL_EXPORT gu_result gu_message_box(const char* title, const char* message, int32_t choice, int32_t icon, int32_t* user_selection);
extern "C" LV_DLL_EXPORT gu_result gu_input_box(const char* title, const char* message, const char* default_input, intptr_t* input_pointer, int32_t* input_size);

//////////////////
// File I/O API //
//////////////////
extern "C" LV_DLL_EXPORT gu_result gu_create_file(const char* path);
extern "C" LV_DLL_EXPORT gu_result gu_create_folder(const char* path);
extern "C" LV_DLL_EXPORT gu_result gu_list_folder(const char* path, const char* match, intptr_t* files, int32_t* num_files, intptr_t* folders, int32_t* num_folders);
extern "C" LV_DLL_EXPORT gu_result gu_move(const char* src, const char* dest);
extern "C" LV_DLL_EXPORT gu_result gu_copy(const char* src, const char* dest);
extern "C" LV_DLL_EXPORT gu_result gu_delete(const char* src, int32_t delete_hierarchy);
gu_result gu_file_operation(const char* src, const char* dest, uint32_t func);
extern "C" LV_DLL_EXPORT gu_result gu_short_path(const char* path, intptr_t* short_path_pointer, int32_t* short_path_length);
extern "C" LV_DLL_EXPORT gu_result gu_long_path(const char* path, intptr_t* long_path_pointer, int32_t* long_path_length);

////////////////////////
// Win32 API Wrappers //
////////////////////////
extern "C" LV_DLL_EXPORT int32_t gu_utf8_to_utf16_length(const char* utf8);
extern "C" LV_DLL_EXPORT int32_t gu_utf8_to_utf16(const char* utf8_str, uint8_t* utf16_str, int32_t utf16_str_size);
extern "C" LV_DLL_EXPORT int32_t gu_utf16_to_utf8_length(const wchar_t* utf16_str);
extern "C" LV_DLL_EXPORT int32_t gu_utf16_to_utf8(const wchar_t* utf16_str, char* utf8_str, int32_t utf8_str_size);

//////////////////////////
// Ancilliary Functions //
//////////////////////////

//////////////////////////////////////////////////////////////////////////
//    WildcardMatch
//        str      - Input string to match
//        match    - Match mask that may contain wildcards like ? and *
//    
//        A ? sign matches any character, except an empty string.
//        A * sign matches any string inclusive an empty string.
//        Characters are compared caseless.
//        https://www.codeproject.com/articles/188256/a-simple-wildcard-matching-function
//
//    Modified to match UTF-8 codepoints
bool wildcard_match(const char* str, const char* match)
{
    utf8_int32_t str_cp;
    utf8_int32_t match_cp;

    // We have a special case where string is empty ("") and the mask is "*".
    // We need to handle this too. So we can't test on !*pszString here.
    // The loop breaks when the match string is exhausted.
    while (*match)
    {
        utf8codepoint(match, &match_cp);

        // Single wildcard character
        if (match_cp == '?')
        {
            // Matches any character except empty string
            if (!*str)
                return false;

            // OK next
            str = utf8codepoint(str, &str_cp);
            match = utf8codepoint(match, &match_cp);
        }
        else if (match_cp == '*')
        {
            // Need to do some tricks.

            // 1. The wildcard * is ignored. 
            //    So just an empty string matches. This is done by recursion.
            //      Because we eat one character from the match string, the
            //      recursion will stop.
            if (wildcard_match(str, utf8codepoint(match, &match_cp)))
                // we have a match and the * replaces no other character
                return true;

            // 2. Chance we eat the next character and try it again, with a
            //    wildcard * match. This is done by recursion. Because we eat
            //      one character from the string, the recursion will stop.
            if (*str && wildcard_match(utf8codepoint(str, &str_cp), match))
                return true;

            // Nothing worked with this wildcard.
            return false;
        }
        else
        {
            // Standard compare of 2 chars. Note that *str might be 0
            // here, but then we never get a match on *match that has always
            // a value while inside this loop.
            str = utf8codepoint(str, &str_cp);
            match = utf8codepoint(match, &match_cp);
            if (utf8uprcodepoint(str_cp) != utf8uprcodepoint(match_cp))
                return false;
        }
    }

    // Have a match? Only if both are at the end...
    return !*str && !*match;
}

#if defined(_WIN32)
// NOTE: Caller must free returned wchar_t*
wchar_t* widen(const char* utf8_string, bool double_null = false)
{
	if (utf8_string == NULL)
	{
		return NULL;
	}

	int32_t utf8_length = strlen(utf8_string);

	UINT codepage = CP_UTF8; // GetACP();
	int32_t wide_length = MultiByteToWideChar(codepage, 0, utf8_string, utf8_length, 0, 0);

	if (wide_length <= 0)
	{
		return NULL;
	}

	wchar_t* wide_string = (wchar_t*)malloc((wide_length * sizeof(wchar_t)) + (sizeof(wchar_t) * (double_null ? 2 : 1)));

	if (wide_string != NULL)
	{
		MultiByteToWideChar(codepage, 0, utf8_string, utf8_length, wide_string, wide_length);
		wide_string[wide_length] = L'\0';

        if (double_null)
        {
            wide_string[wide_length + 1] = L'\0';
        }
	}

	return wide_string;
}

// NOTE: Caller must free returned char*
char* narrow(const wchar_t* wide_string)
{
	if (wide_string == NULL)
	{
		return NULL;
	}

	int32_t wide_length = wcslen(wide_string);

	UINT codepage = CP_UTF8; // GetACP();
	int32_t utf8_length = WideCharToMultiByte(codepage, 0, wide_string, wide_length, 0, 0, NULL, NULL);

	if (utf8_length <= 0)
	{
		return NULL;
	}

	char* utf8_string = (char*)malloc(utf8_length + 1);

	if (utf8_string != NULL)
	{
		WideCharToMultiByte(codepage, 0, wide_string, wide_length, utf8_string, utf8_length, NULL, NULL);
		utf8_string[utf8_length] = '\0';
	}

	return utf8_string;
}
#endif

#endif
