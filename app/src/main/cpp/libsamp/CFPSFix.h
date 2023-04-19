#pragma once

#include <vector>
#include <thread>
#include <mutex>

class CFPSFix
{
private:
	[[noreturn]] void Routine();

	std::mutex m_Mutex;
	std::vector<pid_t> m_Threads;
public:
	CFPSFix();
	~CFPSFix()= default;;

	void PushThread(pid_t tid);
};

