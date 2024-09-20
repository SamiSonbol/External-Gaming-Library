#pragma once
#include <Windows.h>
#include <assert.h>
#include <iostream>

class Executable
{
public:

	//path is the location of the .exe on computer but in std::wstring type. Since launching the game requires the .exe path to be of type LPCTSTR and LPCTSTR is a pointer; to preserve
	//memory safety, i opted to use type std::wstring which can be transformed to type LPCTSTR right before we search for the game and then deleted right after we launch the exe (type std::string cannot be transformed to type LPCTSTR).
	//Hence, by using create_executable_path(string s) we transform the found path from std::string to std::wstring and later in launch to LPCTSTR. 
	std::wstring path;

	std::wstring create_executable_path(const std::string& path);

	void set_path(const std::string& path);

	void launch();

	Executable(const std::string& path = "");

};

