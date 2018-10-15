#include "log.h"
#include <stdarg.h>
#include <atltime.h>
#include <thread>

Log::Log(FILE* file)
{
	m_file = file;
}

Log::~Log()
{
	if (m_file != NULL) {
		fclose(m_file);
	}
}

std::string timestring()
{
	CTime tm = CTime::GetCurrentTime();//»ñÈ¡ÏµÍ³ÈÕÆÚ
	CString date = tm.Format(_T("%Y-%m-%d %X"));
	CStringA stra(date.GetBuffer(0));
	date.ReleaseBuffer();
	std::string timestr = stra.GetBuffer(0);
	stra.ReleaseBuffer();
	return timestr;
}