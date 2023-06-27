/*
G-Unicode - A LabVIEW unicode library.

See g_unicode.h for details.
*/

#include "g_unicode.h"




////////////////////////////
// LabVIEW CLFN Callbacks //
////////////////////////////

extern "C" LV_DLL_EXPORT int32_t clfn_abort(void* data)
{
	return 0;
}




///////////////////////
// Memory Management //
///////////////////////

extern "C" LV_DLL_EXPORT void gu_free(intptr_t pointer)
{
	gu_free((void*)pointer);
}

void gu_free(void* pointer)
{
	if (pointer != NULL)
	{
		free(pointer);
		pointer = NULL;
	}
}




///////////////
// UTF-8 API //
///////////////

extern "C" LV_DLL_EXPORT gu_result gu_strlen(const char* str, int32_t* length)
{
	if (str == NULL)
	{
		return GU_E_INVALID_PARAMS;
	}

	*length = (int32_t)utf8len(str);

	return GU_SUCCESS;
}

extern "C" LV_DLL_EXPORT gu_result gu_string_subset(const char* str, int32_t offset, int32_t length, intptr_t* substring_pointer, int32_t* substring_size)
{
	*substring_pointer = 0;
	*substring_size = 0;

	if (str == NULL)
	{
		return GU_E_INVALID_PARAMS;
	}

	int32_t string_length = (int32_t)utf8len(str);
	if (offset >= string_length || length <= 0)
	{
		return GU_SUCCESS;
	}

	if (offset < 0)
	{
		offset = 0;
	}

	if (offset + (int64_t)length > string_length)
	{
		length = string_length - offset;
	}

	int32_t buffer_size = length * sizeof(utf8_int32_t);
	char* substring = (char*)malloc(buffer_size);
	if (substring == NULL)
	{
		return GU_E_MEMORY;
	}

	utf8_int32_t cp = 0;
	utf8_int8_t* pn = (utf8_int8_t*)str;
	for (int32_t i = 0; i < offset; i++)
	{
		pn = utf8codepoint(pn, &cp);
	}

	int32_t sz = buffer_size;
	utf8_int8_t* substr_pn = substring;
	for (int32_t i = 0; i < length; i++)
	{
		pn = utf8codepoint(pn, &cp);
		substr_pn = utf8catcodepoint(substr_pn, cp, sz);
		sz = sz - utf8codepointsize(cp);
	}

	*substring_size = buffer_size - sz;
	*substring_pointer = (intptr_t)substring;

	return GU_SUCCESS;
}

extern "C" LV_DLL_EXPORT gu_result gu_to_upper_case(char* str)
{
	if (str == NULL)
	{
		return GU_E_INVALID_PARAMS;
	}

	utf8upr(str);

	return GU_SUCCESS;
}

extern "C" LV_DLL_EXPORT gu_result gu_to_lower_case(char* str)
{
	if (str == NULL)
	{
		return GU_E_INVALID_PARAMS;
	}

	utf8lwr(str);

	return GU_SUCCESS;
}

extern "C" LV_DLL_EXPORT gu_result gu_reverse_string(char* str)
{
	if (str == NULL)
	{
		return GU_E_INVALID_PARAMS;
	}

	int32_t string_length = (int32_t)utf8len(str);

	if (string_length <= 1)
	{
		return GU_SUCCESS;
	}

	int32_t buffer_size = strlen(str) * sizeof(char);
	char* temp_string = (char*)calloc(buffer_size, sizeof(char));

	if (temp_string == NULL)
	{
		return GU_E_MEMORY;
	}

	int32_t sz = buffer_size;
	utf8_int8_t* temp_string_pn = temp_string;

	utf8_int32_t cp;
	// Get the null terminating (last) codepoint
	utf8_int8_t* pn = utf8rchr(str, '\0');
	pn = utf8rcodepoint(pn, &cp);

	for (int i = 0; i < string_length; i++)
	{
		pn = utf8rcodepoint(pn, &cp);
		temp_string_pn = utf8catcodepoint(temp_string_pn, cp, sz);
		sz = sz - utf8codepointsize(cp);
	}

	memcpy(str, temp_string, buffer_size);
	free(temp_string);

	return GU_SUCCESS;
}

extern "C" LV_DLL_EXPORT gu_result gu_rotate_string(char* str)
{
	if (str == NULL)
	{
		return GU_E_INVALID_PARAMS;
	}

	int32_t string_length = (int32_t)utf8len(str);
	int32_t buffer_size = strlen(str) * sizeof(char);

	if (string_length <= 1)
	{
		return GU_SUCCESS;
	}

	utf8_int32_t cp;
	// Get the first codepoint
	utf8codepoint(str, &cp);
	int32_t cp_size = utf8codepointsize(cp);
	if (memmove(str, str + cp_size, buffer_size - cp_size) == NULL)
	{
		return GU_E_MEMORY;
	}
	utf8catcodepoint(str + buffer_size - cp_size, cp, cp_size);

	return GU_SUCCESS;
}



extern "C" LV_DLL_EXPORT gu_result gu_search_split_string(const char* str, const char* search_str, int32_t offset, int32_t reverse, intptr_t* string_a_pointer, int32_t* string_a_size, intptr_t* string_b_pointer, int32_t* string_b_size)
{
	*string_a_pointer = 0;
	*string_b_pointer = 0;
	*string_a_size = 0;
	*string_b_size = 0;

	if (str == NULL || search_str == NULL)
	{
		return GU_E_INVALID_PARAMS;
	}

	utf8_int8_t* match_ptr = NULL;
	int32_t string_length = (int32_t)utf8len(str);
	int32_t string_size = (int32_t)strlen(str);
	if (offset < string_length)
	{
		if (offset < 0)
		{
			offset = 0;
		}

		int32_t search_str_length = utf8len(search_str);
		utf8_int32_t cp = 0;
		utf8_int8_t* str_offset = (utf8_int8_t*)str;
		for (int32_t i = 0; i < offset; i++)
		{
			str_offset = utf8codepoint(str_offset, &cp);
		}

		// Codepoint match only, allows forward or reverse searching
		if (search_str_length == 1)
		{
			utf8codepoint(search_str, &cp);
			match_ptr = (reverse == 0) ? utf8chr(str_offset, cp) : utf8rchr(str_offset, cp);
		}
		else
		{
			match_ptr = utf8str(str_offset, search_str);
		}
	}

	if (match_ptr == 0 || match_ptr == NULL)
	{
		match_ptr = (utf8_int8_t*)str + string_size;
	}

	int32_t a_size = match_ptr - str;
	int32_t b_size = string_size - a_size;

	char* string_a = (char*)malloc(a_size);
	char* string_b = (char*)malloc(b_size);

	if (string_a == NULL || string_b == NULL)
	{
		return GU_E_MEMORY;
	}

	memcpy(string_a, str, a_size);
	memcpy(string_b, match_ptr, b_size);

	*string_a_pointer = (intptr_t)string_a;
	*string_b_pointer = (intptr_t)string_b;
	*string_a_size = a_size;
	*string_b_size = b_size;

	return GU_SUCCESS;
}

extern "C" LV_DLL_EXPORT gu_result gu_is_text_utf16(const char* buf, int32_t size, int32_t* is_utf16)
{
	*is_utf16 = 0;
#if defined(_WIN32)
	int32_t params = IS_TEXT_UNICODE_STATISTICS | IS_TEXT_UNICODE_REVERSE_STATISTICS;
	
	if (IsTextUnicode(buf, size, &params) != 0)
	{
		*is_utf16 = (params & IS_TEXT_UNICODE_STATISTICS) ? 1 : 2;
	}

	return GU_SUCCESS;
#else
	return GU_W_WIN32_ONLY;
#endif
}

// Taken from https://stackoverflow.com/questions/1031645/how-to-detect-utf-8-in-plain-c/1031773#1031773
// Based on https://www.w3.org/International/questions/qa-forms-utf-8
extern "C" LV_DLL_EXPORT gu_result gu_is_text_utf8(const char* str, int32_t* is_utf8)
{
	*is_utf8 = 0;

	if (str == NULL)
	{
		return GU_E_INVALID_PARAMS;
	}

	const unsigned char * bytes = (const unsigned char *)str;
	while (*bytes)
	{
		if ((// ASCII
			 // use bytes[0] <= 0x7F to allow ASCII control characters
			bytes[0] == 0x09 ||
			bytes[0] == 0x0A ||
			bytes[0] == 0x0D ||
			(0x20 <= bytes[0] && bytes[0] <= 0x7E)
			)
			) {
			bytes += 1;
			continue;
		}

		if ((// non-overlong 2-byte
			(0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
			(0x80 <= bytes[1] && bytes[1] <= 0xBF)
			)
			) {
			bytes += 2;
			continue;
		}

		if ((// excluding overlongs
			bytes[0] == 0xE0 &&
			(0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF)
			) ||
			(// straight 3-byte
				((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
					bytes[0] == 0xEE ||
					bytes[0] == 0xEF) &&
				(0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
				(0x80 <= bytes[2] && bytes[2] <= 0xBF)
				) ||
			(// excluding surrogates
				bytes[0] == 0xED &&
				(0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
				(0x80 <= bytes[2] && bytes[2] <= 0xBF)
				)
			) {
			bytes += 3;
			continue;
		}

		if ((// planes 1-3
			bytes[0] == 0xF0 &&
			(0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
			(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
			(0x80 <= bytes[3] && bytes[3] <= 0xBF)
			) ||
			(// planes 4-15
				(0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
				(0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
				(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
				(0x80 <= bytes[3] && bytes[3] <= 0xBF)
				) ||
			(// plane 16
				bytes[0] == 0xF4 &&
				(0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
				(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
				(0x80 <= bytes[3] && bytes[3] <= 0xBF)
				)
			) {
			bytes += 4;
			continue;
		}

		*is_utf8 = 0;
		return GU_SUCCESS;
	}

	*is_utf8 = 1;
	return GU_SUCCESS;
}




////////////////
// Dialog API //
////////////////

extern "C" LV_DLL_EXPORT gu_result gu_open_file_dialog(const char* title, const char* default_path, int32_t num_filter_patterns, const char** filter_patterns, const char* filter_description, int32_t allow_multi_select, int32_t* cancelled, intptr_t* path_pointer, int32_t* path_length)
{
	*path_length = 0;
	*path_pointer = 0;
	*cancelled = 0;

	std::string _title = (strlen(title) > 0) ? title : "Choose or Enter Path of Folder";
	std::string _default_path = (strlen(default_path) > 0) ? default_path : pfd::path::home();

	auto selection = pfd::open_file(_title, _default_path, { "All Files (*.*)", "*" }, pfd::opt::none).result();

	// User cancelled the dialog
	if (selection.size() == 0)
	{
		*cancelled = 1;
		return GU_SUCCESS;
	}

	char* path = _strdup(selection[0].c_str());
	if (path == NULL)
	{
		return GU_E_MEMORY;
	}

	*path_length = strlen(path);
	*path_pointer = (intptr_t)path;

	return GU_SUCCESS;
}

extern "C" LV_DLL_EXPORT gu_result gu_select_folder_dialog(const char* title, const char* default_path, int32_t* cancelled, intptr_t* path_pointer, int32_t* path_length)
{
	*path_length = 0;
	*path_pointer = 0;
	*cancelled = 0;

	std::string _title = (strlen(title) > 0) ? title : "Choose or Enter Path of Folder";
	std::string _default_path = (strlen(default_path) > 0) ? default_path : pfd::path::home();

	std::string selection = pfd::select_folder(_title, _default_path).result();

	// User cancelled the dialog
	if (selection.empty())
	{
		*cancelled = 1;
		return GU_SUCCESS;
	}

	char* path = _strdup(selection.c_str());
	if (path == NULL)
	{
		return GU_E_MEMORY;
	}

	*path_length = strlen(path);
	*path_pointer = (intptr_t)path;

	return GU_SUCCESS;
}

extern "C" LV_DLL_EXPORT gu_result gu_message_box(const char* title, const char* message, int32_t choice, int32_t icon, int32_t* user_selection)
{
	*user_selection = (int32_t)pfd::message(title, message, (pfd::choice)choice, (pfd::icon)icon).result();

	return GU_SUCCESS;
}

extern "C" LV_DLL_EXPORT gu_result gu_input_box(const char* title, const char* message, const char* default_input, intptr_t* input_pointer, int32_t* input_size)
{
	*input_pointer = (intptr_t)InputBox((char*)message, (char*)title, (char*)default_input);
	*input_size = strlen((char*)*input_pointer);

	return GU_SUCCESS;
}




//////////////////////////
// LabVIEW File I/O API //
//////////////////////////

extern "C" LV_DLL_EXPORT gu_result gu_create_file(const char* path)
{
#if defined(_WIN32)
	int32_t fd;
	errno_t err;

	if (path == NULL)
	{
		return GU_E_INVALID_PARAMS;
	}

	wchar_t* wide_path = widen(path);
	if (wide_path == NULL)
	{
		return GU_E_MEMORY;
	}

	err = _wsopen_s(&fd, wide_path, O_CREAT | O_WRONLY | O_EXCL, _SH_DENYNO, _S_IREAD | _S_IWRITE);
	free(wide_path);

	if (err != 0)
	{
		switch (err)
		{
			case EACCES: break;
			case EEXIST: return GU_E_ALREADY_EXISTS; break;
			case EINVAL: return GU_E_INVALID_PARAMS; break;
			case EMFILE: break;
			case ENOENT: break;
			default:     break;
		}

		return GU_E_FILE_IO;
	}
	_close(fd);
	return GU_SUCCESS;
#else
	return GU_W_WIN32_ONLY;
#endif
}

extern "C" LV_DLL_EXPORT gu_result gu_create_folder(const char* path)
{
#if defined _WIN32
	int err;

	if (path == NULL)
	{
		return GU_E_INVALID_PARAMS;
	}

	wchar_t* wide_path = widen(path);
	if (wide_path == NULL)
	{
		return GU_E_MEMORY;
	}

	err = SHCreateDirectory(NULL, wide_path);
	free(wide_path);

	if (err != ERROR_SUCCESS)
	{
		switch (err)
		{
			case ERROR_BAD_PATHNAME: return GU_E_RELATIVE_PATH; break;
			case ERROR_FILENAME_EXCED_RANGE: return GU_E_INVALID_PATH; break;
			case ERROR_FILE_EXISTS:
			case ERROR_ALREADY_EXISTS: return GU_E_ALREADY_EXISTS; break;
			default: break;
		}

		return GU_E_FILE_IO;
	}

	return GU_SUCCESS;
#else
	return GU_W_WIN32_ONLY;
#endif
}

extern "C" LV_DLL_EXPORT gu_result gu_list_folder(const char* path, const char* match, intptr_t* files, int32_t* num_files, intptr_t* folders, int32_t* num_folders)
{
	*files = 0;
	*folders = 0;
	*num_files = 0;
	*num_folders = 0;

#if defined _WIN32
	tinydir_dir dir;
	wchar_t* wide_path = widen(path);
	if (wide_path == NULL)
	{
		return GU_E_MEMORY;
	}

	if (tinydir_open(&dir, wide_path) != 0)
	{
		free(wide_path);
		return GU_E_FILE_IO;
	}
	free(wide_path);

	int32_t file_list_length = 0;
	int32_t folder_list_length = 0;
	void* temp_ptr = NULL;
	gu_string* file_list = NULL;
	gu_string* folder_list = NULL;
	char* file_name = NULL;
	int32_t file_name_size = 0;
	bool do_match = strlen(match) > 0;

	while (dir.has_next)
	{
		tinydir_file file;
		
		if (tinydir_readfile(&dir, &file) != 0)
		{
			tinydir_close(&dir);
			return GU_E_FILE_IO;
		}

		if (_tinydir_strcmp(file.name, TINYDIR_STRING(".")) == 0 ||
			_tinydir_strcmp(file.name, TINYDIR_STRING("..")) == 0)
		{
			if (tinydir_next(&dir) != 0)
			{
				tinydir_close(&dir);
				return GU_E_FILE_IO;
			}
			continue;
		}

		file_name = narrow(file.name);
		if (file_name == NULL)
		{
			return GU_E_MEMORY;
		}

		if (do_match && !wildcard_match(file_name, match))
		{
			// free memory allocated in narrow();
			free(file_name);
			if (tinydir_next(&dir) != 0)
			{
				tinydir_close(&dir);
				return GU_E_FILE_IO;
			}
			continue;
		}

		if (file.is_dir)
		{
			if (folder_list_length % REALLOC_SIZE == 0)
			{
				temp_ptr = realloc(folder_list, sizeof(gu_string) * (folder_list_length + REALLOC_SIZE));
				if (temp_ptr == NULL)
				{
					//free_string_list(folder_list, folder_list_length);
					//free_string_list(file_list, file_list_length);
					return GU_E_MEMORY;
				}
				folder_list = (gu_string*)temp_ptr;
				temp_ptr = NULL;
			}

			folder_list[folder_list_length].size = strlen(file_name);
			// The memory allocated in previous call to narrow() will be freed in LabVIEW
			folder_list[folder_list_length].string = file_name;
			folder_list_length++;
		}
		else
		{
			if (file_list_length % REALLOC_SIZE == 0)
			{
				temp_ptr = realloc(file_list, sizeof(gu_string) * (file_list_length + REALLOC_SIZE));
				if (temp_ptr == NULL)
				{
					//free_string_list(folder_list, folder_list_length);
					//free_string_list(file_list, file_list_length);
					return GU_E_MEMORY;
				}
				file_list = (gu_string*)temp_ptr;
				temp_ptr = NULL;
			}

			file_list[file_list_length].size = strlen(file_name);
			// The memory allocated in previous call to narrow() will be freed in LabVIEW
			file_list[file_list_length].string = file_name;
			file_list_length++;
		}

		if (tinydir_next(&dir) != 0)
		{
			tinydir_close(&dir);
			return GU_E_FILE_IO;
		}
	}

	tinydir_close(&dir);

	*files = (intptr_t)file_list;
	*folders = (intptr_t)folder_list;
	*num_files = file_list_length;
	*num_folders = folder_list_length;

	return GU_SUCCESS;
#else
	return GU_W_WIN32_ONLY;
#endif
}

gu_result gu_file_operation(const char* src, const char* dest, UINT func)
{
#if defined _WIN32
	SHFILEOPSTRUCT fileOps;
	fileOps.hwnd = NULL;
	fileOps.wFunc = func;
	fileOps.pFrom = widen(src, true);
	fileOps.pTo = widen(dest, true);
	fileOps.fFlags = FOF_NO_UI;
	fileOps.fAnyOperationsAborted = FALSE;
	fileOps.hNameMappings = NULL;
	fileOps.lpszProgressTitle = NULL;

	if ((src != NULL && fileOps.pFrom == NULL) ||
		(dest != NULL && fileOps.pTo == NULL))
	{
		free((void*)fileOps.pFrom);
		free((void*)fileOps.pTo);
		return GU_E_MEMORY;
	}

	if ((fileOps.pFrom != NULL && PathIsRelative(fileOps.pFrom)) ||
		(fileOps.pTo != NULL && PathIsRelative(fileOps.pTo)))
	{
		free((void*)fileOps.pFrom);
		free((void*)fileOps.pTo);
		return GU_E_RELATIVE_PATH;
	}

	int32_t result = SHFileOperation(&fileOps);
	free((void*)fileOps.pFrom);
	free((void*)fileOps.pTo);

	return result == 0 ? GU_SUCCESS : GU_E_FILE_IO;
#else
	return GU_W_WIN32_ONLY;
#endif
}

extern "C" LV_DLL_EXPORT gu_result gu_move(const char* src, const char* dest)
{
#if defined _WIN32
	return gu_file_operation(src, dest, FO_MOVE);
#else
	return GU_W_WIN32_ONLY;
#endif
}

extern "C" LV_DLL_EXPORT gu_result gu_copy(const char* src, const char* dest)
{
#if defined _WIN32
	return gu_file_operation(src, dest, FO_COPY);
#else
	return GU_W_WIN32_ONLY;
#endif
}

extern "C" LV_DLL_EXPORT gu_result gu_delete(const char* src, int32_t delete_hierarchy)
{
#if defined _WIN32
	wchar_t* wide_src = widen(src);
	if (widen == NULL)
	{
		return GU_E_MEMORY;
	}

	int32_t result = GetFileAttributes(wide_src);

	if (result == INVALID_FILE_ATTRIBUTES)
	{
		free(wide_src);
		return GU_E_FILE_IO;
	}

	if (result & FILE_ATTRIBUTE_DIRECTORY)
	{
		if (delete_hierarchy == 0)
		{
			result = RemoveDirectory(wide_src);
			free(wide_src);

			if (result == 0)
			{
				return GU_E_FILE_IO;
			}
			return GU_SUCCESS;
		}
		else
		{
			free(wide_src);
			return gu_file_operation(src, NULL, FO_DELETE);
		}
	}
	else
	{
		result = DeleteFile(wide_src);
		free(wide_src);
		return result != 0 ? GU_SUCCESS : GU_E_FILE_IO;
	}
#else
	return GU_W_WIN32_ONLY;
#endif
}




////////////////////////
// Win32 API Wrappers //
////////////////////////

extern "C" LV_DLL_EXPORT int32_t gu_utf8_to_utf16_length(const char* utf8_str)
{
#if defined(_WIN32)
	if (utf8_str == NULL)
	{
		return 0;
	}

	int32_t utf8_length = strlen(utf8_str);

	return MultiByteToWideChar(CP_UTF8, 0, utf8_str, utf8_length, 0, 0);
#else
	return 0;
#endif
}

// Convenience function to be called from LabVIEW. The utf16_string is defined as uint8_t*, but is treated internally as a wchar_t*, so it must have
// at least twice the length of the utf8_string allocated to it. The output utf16le_size is the number of bytes in the utf16_string (not characters).
// LabVIEW *can* directly call MultiByteToWideChar, but this creates a dependency on kernel32.dll in the calling VI.
// When this VI is loaded under macOS or Linux, LabVIEW will search for kernel32.dll (even if the CLFN is inside a TARGET_TYPE conditional disable structure).
// Moving the dependency to g_unicode_*.* avoids this search.
extern "C" LV_DLL_EXPORT int32_t gu_utf8_to_utf16(const char* utf8_str, uint8_t * utf16_str, int32_t utf16_str_size)
{
#if defined(_WIN32)
	if (utf8_str == NULL || utf16_str_size == 0)
	{
		return 0;
	}

	int utf8_length = strlen(utf8_str);

	return MultiByteToWideChar(CP_UTF8, 0, utf8_str, utf8_length, (wchar_t*)utf16_str, utf16_str_size);
#else
	return 0;
#endif
}
