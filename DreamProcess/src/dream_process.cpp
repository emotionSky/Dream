#include "DreamSky/dream_process.h"

#if defined(_WIN32) || defined(_WINDLL)
#include <Windows.h>
#else
#include <unistd.h>
#endif

DREAM_NAMESPACE_BEGIN

#if defined(_WIN32) || defined(_WINDLL)

#ifdef UNICODE
LPWSTR ConvertStringToWin(const char* str)
{
	if (!str || str[0] == 0)
		return nullptr;

	// 获取需要分配的宽字符字符串的长度
	int length = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);

	// 分配内存来存储宽字符字符串
	LPWSTR wideString = new wchar_t[length * sizeof(wchar_t)];

	// 将多字节字符串转换为宽字符字符串
	MultiByteToWideChar(CP_UTF8, 0, str, -1, wideString, length);

	// 使用LPCWSTR类型的字符串
	//wprintf(L"%ls\n", outputString);

	return wideString;
}

void FreeWinString(LPCWSTR str)
{
	if (str)
		delete[] str;
}
#endif

ProcessInfos::ProcessInfos() : 
	m_processID(0), m_threadID(0),
	m_processHdl(nullptr), m_threadHdl(nullptr)
{
}

StartupInfo::StartupInfo() : 
	m_x(0), m_y(0), m_xSize(0), m_ySize(0)
{

}

ProcessParams::ProcessParams() : m_bInherit(false)
{}

ProcessParams::ProcessParams(const char* app_name, const char* argv, bool is_inherit) : 
	m_bInherit(is_inherit), m_appName(app_name), m_cmdLine(argv)
{}

ProcessParams::ProcessParams(const dstring& app_name, const dstring& argv, bool is_inherit) :
	m_bInherit(is_inherit), m_appName(app_name), m_cmdLine(argv)
{}

DWinProcess::DWinProcess() : 
	m_bCreated(false), m_errStr("no error")
{
}

DWinProcess::~DWinProcess()
{
	CloseProcess();
}

bool DWinProcess::CreateOneProcess(const ProcessParams& params, StartupInfo* p_startup)
{
	if (m_bCreated)
		return true;

	m_params = params;
	if (p_startup)
		m_startupInfo = *p_startup;

	STARTUPINFO si = { sizeof(STARTUPINFO) };//在产生子进程时，子进程的窗口相关信息
	if (p_startup)
	{
		// TODO
	}
	PROCESS_INFORMATION pi;                  //子进程的ID/线程相关信息

#ifdef UNICODE
	LPWSTR app_name = ConvertStringToWin(m_params.m_appName.data());
	LPWSTR cmd_line = ConvertStringToWin(m_params.m_cmdLine.data());
#else
	char* app_name = m_params.m_appName.data();
	char* cmd_line = m_params.m_cmdLine.data();
#endif

	BOOL bRet = CreateProcess( //调用失败，返回0；调用成功返回非0；
		app_name,              //一般都是空；（另一种批处理情况：此参数指定"cmd.exe",下一个命令行参数 "/c otherBatFile")
		cmd_line,              //命令行参数         
		nullptr,               //_In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
		nullptr,               //_In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
		m_params.m_bInherit,   //_In_        BOOL                  bInheritHandles,
		CREATE_NEW_CONSOLE,    //新的进程使用新的窗口。
		nullptr,               //_In_opt_    LPVOID                lpEnvironment,
		nullptr,               //_In_opt_    LPCTSTR               lpCurrentDirectory,
		&si,                   //_In_        LPSTARTUPINFO         lpStartupInfo,
		&pi);                  //_Out_       LPPROCESS_INFORMATION lpProcessInformation

	if (bRet)
	{
		m_infos.m_processID = pi.dwProcessId;
		m_infos.m_threadID = pi.dwThreadId;
		m_infos.m_processHdl = pi.hProcess;
		m_infos.m_threadHdl = pi.hThread;
		m_bCreated = true;
	}
	else
	{
		int err = GetLastError();
		char* pStr = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pStr, 0, NULL);
		m_errStr = pStr;
		LocalFree(pStr);
	}

#ifdef UNICODE
	FreeWinString(app_name);
	FreeWinString(cmd_line);
#endif

	return m_bCreated;
}

void DWinProcess::WaitProcess(int timeout_ms)
{
	if (m_bCreated && m_infos.m_processHdl)
	{
		DWORD timeout = static_cast<int>(timeout_ms);
		WaitForSingleObject(m_infos.m_processHdl, timeout);
	}
}

int DWinProcess::GetExitCode()
{
	DWORD ret = 0;
	if (m_bCreated && m_infos.m_processHdl)
	{
		GetExitCodeProcess(m_infos.m_processHdl, &ret);
	}
	return static_cast<int>(ret);
}

void DWinProcess::CloseProcess()
{
	if (m_bCreated && m_infos.m_processHdl)
	{
		CloseHandle(m_infos.m_threadHdl);
		CloseHandle(m_infos.m_processHdl);

		m_infos.m_processID = 0;
		m_infos.m_threadID = 0;
		m_infos.m_processHdl = nullptr;
		m_infos.m_threadHdl = nullptr;
	}
	m_bCreated = false;
}

#endif

DREAM_NAMESPACE_END
