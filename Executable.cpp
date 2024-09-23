#include "Executable.h"

std::wstring Executable::create_executable_path(const std::string& path) {

	return std::wstring(path.begin(), path.end());
	
};

void Executable::set_path(const std::string& path) {

	this->path = create_executable_path(path);

};

void Executable::launch() {

	HWND main_window = HWND();

	if (!this->path.empty()) {

		LPCTSTR exe_path = this->path.c_str();

		LPTSTR command_line = NULL;

		LPSECURITY_ATTRIBUTES process_attributes = NULL;

		LPSECURITY_ATTRIBUTES thread_attributes = NULL;

		BOOL inherit_handles = FALSE;

		DWORD creation_flags = 0;

		LPVOID environment = NULL;

		LPCTSTR current_directory = NULL;

		PROCESS_INFORMATION process_information;

		STARTUPINFO startup_information;

		ZeroMemory(&process_information, sizeof(process_information));

		ZeroMemory(&startup_information, sizeof(startup_information));
		startup_information.cb = sizeof(startup_information);

		bool success = CreateProcess(

			exe_path,

			command_line,

			process_attributes,

			thread_attributes,

			inherit_handles,

			creation_flags,

			environment,

			current_directory,

			&startup_information,

			&process_information

		);

		assert(success);

		CloseHandle(process_information.hThread);
		//WaitForSingleObject(process_information.hProcess, INFINITE);
		CloseHandle(process_information.hProcess);

		SetForegroundWindow(main_window);

	}
	else {

		std::cout << "ERROR: couldnt find executable\n";

	};//game_path check bracket

};

Executable::Executable(const std::string& path) {

	if (!path.empty()) {

		this->path = create_executable_path(path);

	}
	else {

		std::cout << "couldnt find executable path => dead Executable object created" << std::endl;

	};

};
