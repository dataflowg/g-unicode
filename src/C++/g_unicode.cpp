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

extern "C" LV_DLL_EXPORT int32_t gu_strlen(const char* str)
{
	if (str == NULL)
	{
		return 0;
	}

	return (int32_t)utf8len(str);
}

extern "C" LV_DLL_EXPORT void gu_string_subset(const char* str, int32_t offset, int32_t length, intptr_t* substring_pointer, int32_t* substring_size)
{
	if (str == NULL)
	{
		return;
	}

	int32_t string_length = (int32_t)utf8len(str);
	if (offset >= string_length || length <= 0)
	{
		return;
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
		return;
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
}

extern "C" LV_DLL_EXPORT void gu_to_upper_case(char* str)
{
	if (str == NULL)
	{
		return;
	}

	return utf8upr(str);
}

extern "C" LV_DLL_EXPORT void gu_to_lower_case(char* str)
{
	if (str == NULL)
	{
		return;
	}

	return utf8lwr(str);
}

extern "C" LV_DLL_EXPORT void gu_reverse_string(char* str)
{
	if (str == NULL)
	{
		return;
	}

	int32_t string_length = (int32_t)utf8len(str);

	if (string_length <= 1)
	{
		return;
	}

	int32_t buffer_size = strlen(str) * sizeof(char);
	char* temp_string = (char*)calloc(buffer_size, sizeof(char));

	if (temp_string == NULL)
	{
		return;
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
}

extern "C" LV_DLL_EXPORT void gu_rotate_string(char* str)
{
	if (str == NULL)
	{
		return;
	}

	int32_t string_length = (int32_t)utf8len(str);
	int32_t buffer_size = strlen(str) * sizeof(char);

	if (string_length <= 1)
	{
		return;
	}

	utf8_int32_t cp;
	// Get the first codepoint
	utf8codepoint(str, &cp);
	int32_t cp_size = utf8codepointsize(cp);
	if (memmove(str, str + cp_size, buffer_size - cp_size) == NULL)
	{
		return;
	}
	utf8catcodepoint(str + buffer_size - cp_size, cp, cp_size);
}



extern "C" LV_DLL_EXPORT void gu_search_split_string(const char* str, const char* search_str, int32_t offset, int32_t direction, intptr_t* string_a_pointer, int32_t* string_a_size, intptr_t* string_b_pointer, int32_t* string_b_size)
{
	if (str == NULL || search_str == NULL)
	{
		*string_a_size = 0;
		*string_b_size = 0;
		return;
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

			if (direction == 0)
			{
				match_ptr = utf8chr(str_offset, cp);
			}
			else
			{
				match_ptr = utf8rchr(str_offset, cp);
			}
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

	memcpy(string_a, str, a_size);
	memcpy(string_b, match_ptr, b_size);

	*string_a_pointer = (intptr_t)string_a;
	*string_b_pointer = (intptr_t)string_b;
	*string_a_size = a_size;
	*string_b_size = b_size;
}




////////////////
// Dialog API //
////////////////

extern "C" LV_DLL_EXPORT int32_t gu_open_file_dialog(const char* title, const char* default_path, int32_t num_filter_patterns, const char** filter_patterns, const char* filter_description, int32_t allow_multi_select, intptr_t* path_pointer, int32_t* path_length)
{
	char* path = tinyfd_openFileDialog(title, default_path, num_filter_patterns, filter_patterns, filter_description, allow_multi_select);

	if (path == NULL)
	{
		*path_length = 0;
		*path_pointer = 0;
		return 1;
	}

	*path_length = strlen(path);
	*path_pointer = (intptr_t)path;
	return 0;
}

extern "C" LV_DLL_EXPORT int32_t gu_message_box(const char* title, const char* message, const char* dialog_type, const char* icon_type, int32_t default_button)
{
	return tinyfd_messageBox(title, message, dialog_type, icon_type, default_button);
}

extern "C" LV_DLL_EXPORT int32_t gu_input_box(const char* title, const char* message, const char* default_input, intptr_t* input_pointer, int32_t* input_length)
{
	char* input = tinyfd_inputBox(title, message, default_input);

	if (input == NULL)
	{
		*input_length = 0;
		*input_pointer = 0;
		return 1;
	}

	*input_length = strlen(input);
	*input_pointer = (intptr_t)input;
	return 0;
}




//////////////////////////
// LabVIEW File I/O API //
//////////////////////////

extern "C" LV_DLL_EXPORT int32_t gu_create_file(const char* path)
{
#if defined(_WIN32)
	int32_t fd;
	errno_t err;
	wchar_t* wide_path = widen(path);

	if (wide_path == NULL)
		return 1;

	err = _wsopen_s(&fd, wide_path, O_CREAT | O_WRONLY | O_EXCL, _SH_DENYNO, _S_IREAD | _S_IWRITE);
	gu_free((intptr_t)wide_path);

	if (err != 0)
	{
		switch (err)
		{
			case EACCES: break;
			case EEXIST: break;
			case EINVAL: break;
			case EMFILE: break;
			case ENOENT: break;
			default:
				break;
		}

		return err;
	}
	_close(fd);
#endif

	return 0;
}

extern "C" LV_DLL_EXPORT void gu_list_folder(const char* path, const char* match, intptr_t* files, int32_t* num_files, intptr_t* folders, int32_t* num_folders)
{
	tinydir_dir dir;
#if defined _WIN32
	wchar_t* wide_path = widen(path);
	if (wide_path == NULL)
	{
		return;
	}
	tinydir_open(&dir, wide_path);
	free(wide_path);
#else
	tinydir_open(&dir, path);
#endif

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
		tinydir_readfile(&dir, &file);

		if (_tinydir_strcmp(file.name, TINYDIR_STRING(".")) == 0 ||
			_tinydir_strcmp(file.name, TINYDIR_STRING("..")) == 0)
		{
			tinydir_next(&dir);
			continue;
		}

#if defined _WIN32
		file_name = narrow(file.name);
#else
		file_name = file.name;
#endif
		if (do_match && !wildcard_match(file_name, match))
		{
#if defined _WIN32
			// free memory allocated in narrow();
			free(file_name);
#endif
			tinydir_next(&dir);
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
					*num_folders = 0;
					*num_files = 0;
					return;
				}
				folder_list = (gu_string*)temp_ptr;
				temp_ptr = NULL;
			}

			folder_list[folder_list_length].size = strlen(file_name);
#if defined _WIN32
			// The memory allocated in narrow() will be freed in LabVIEW
			folder_list[folder_list_length].string = file_name;
#else
			// The memory allocated by malloc() will be freed in LabVIEW
			folder_list[folder_list_length].string = (char*)malloc(folder_list[folder_list_length].size);
			memcpy(folder_list[folder_list_length].string, file_name, folder_list[folder_list_length].size);
#endif
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
					*num_folders = 0;
					*num_files = 0;
					return;
				}
				file_list = (gu_string*)temp_ptr;
				temp_ptr = NULL;
			}

			file_list[file_list_length].size = strlen(file_name);
#if defined _WIN32
			// The memory allocated by narrow() will be freed in LabVIEW
			file_list[file_list_length].string = file_name;
#else
			// The memory allocated by malloc() will be freed in LabVIEW
			file_list[file_list_length].string = (char*)malloc(file_list[file_list_length].size);
			memcpy(file_list[file_list_length].string, file.name, file_list[file_list_length].size);
#endif
			file_list_length++;
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);

	*files = (intptr_t)file_list;
	*folders = (intptr_t)folder_list;
	*num_files = file_list_length;
	*num_folders = folder_list_length;
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
