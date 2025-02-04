#include <iostream>
#include <string>
#include "pch.h"
#include "framework.h"
#include <cstdlib>
extern "C"
{
	int __stdcall mystrlen(char* buffer, char* str)					// вычисление длины строки
	{
		if (str == nullptr)
			return 0;
		int len = 0;
		for (int i = 0; i < 256; i++)
			if (str[i] == '\0')
			{
				len = i; break;
			}
		return len;
	}
	
	void __stdcall strcopy(char* buffer, const char* str)				// копирование строк
	{
		strcpy_s(buffer, strlen(str), str);
	}

	int __stdcall outnum(int value)									// вывод в консоль целочисленного литерала
	{
		std::cout << value;
		return 0;
	}


	int __stdcall outstr(char* ptr)									// вывод в консоль строкового литерала
	{
		if (ptr == nullptr)
			std::cout << std::endl;
		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];
		return 0;
	}
}
