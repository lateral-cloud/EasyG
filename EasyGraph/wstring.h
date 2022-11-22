#pragma once

size_t __cdecl wstrlen(wchar_t const* _Str) {
	size_t len = 0;
	while (_Str[len] != '\0')
		++len;
	return len;
}