#ifndef __DREAM_PROCESS_H__
#define __DREAM_PROCESS_H__

#include "pipe/pipe_operation.h"
#include <DreamSky/string/dstring.h>
#include <string>

DREAM_NAMESPACE_BEGIN

#if defined(_WIN32) || defined(_WINDLL)

// 这个进程创建是Windows下的API实现，Linux下有fork可以直接调用。
// 并且由于Linux和Windows的进程设计不同，处理对应的也有区别

class DREAMSKY_API ProcessInfos
{
public:
	ProcessInfos();
	~ProcessInfos() = default;

	unsigned long GetProcessId() const { return m_processID; }
	unsigned long GetThreadId() const { return m_threadID; }
	d_handle GetProcessHandle() const { return m_processHdl; }
	d_handle GetThreadHandle() const { return m_threadHdl; }

private:
	friend class DWinProcess;
	unsigned long m_processID;
	unsigned long m_threadID;
	d_handle      m_processHdl;
	d_handle      m_threadHdl;
};

class DREAMSKY_API StartupInfo
{
public:
	StartupInfo();
	~StartupInfo() = default;

	unsigned long GetX() const { return m_x; }
	unsigned long GetY() const { return m_y; }
	unsigned long GetXSize() const { return m_xSize; }
	unsigned long GetYSize() const { return m_ySize; }

private:
	friend class DWinProcess;
	unsigned long m_x;
	unsigned long m_y;
	unsigned long m_xSize;
	unsigned long m_ySize;
};

class DREAMSKY_API ProcessParams
{
public:
	ProcessParams();
	explicit ProcessParams(const char* app_name, const char* argv, bool is_inherit = false);
	explicit ProcessParams(const dstring& app_name, const dstring& argv, bool is_inherit = false);

private:
	friend class DWinProcess;
	bool    m_bInherit;
	dstring m_appName;
	dstring m_cmdLine;
};

class DREAMSKY_API DWinProcess
{
public:
	DWinProcess();
	~DWinProcess();

	bool IsCreated() const { return m_bCreated; }
	const ProcessInfos& GetProcessInfo() const { return m_infos; }

	bool CreateOneProcess(const ProcessParams& params, StartupInfo* p_startup);
	void WaitProcess(int timeout_ms);
	int GetExitCode();
	void CloseProcess();
	const dstring& GetErorrString() const { return m_errStr; }

private:
	bool          m_bCreated;
	dstring       m_errStr;
	ProcessParams m_params;
	StartupInfo   m_startupInfo;
	ProcessInfos  m_infos;
};

#endif

DREAM_NAMESPACE_END

#endif //!__DREAM_PROCESS_H__