#include "Thread.h"

#ifndef Thread_cpp

#define Thread_cpp

bool Thread::Start(void * parameter)
{
	this->parameter = parameter;

	this->thread = CreateThread(NULL, 0, this->method, this->parameter, 0, NULL );

	return (WaitForSingleObject(this->thread, INFINITE) != WAIT_FAILED);
}

bool Thread::Terminate()
{
	return (TerminateThread(this->thread, 0) == 1);
}

#endif

