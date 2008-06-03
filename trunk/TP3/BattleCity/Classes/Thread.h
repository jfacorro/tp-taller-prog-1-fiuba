#include <windows.h>

#ifndef Thread_h

#define Thread_h

class Thread
{
	private:
		DWORD (WINAPI * method) (LPVOID);
		void * parameter;
		void * thread;

	public:
		Thread(DWORD (WINAPI * method) (LPVOID))
		{
			this->method = method;
			this->parameter = NULL;
		};

		bool Start(void * paramater);

		bool Terminate();
};

#endif