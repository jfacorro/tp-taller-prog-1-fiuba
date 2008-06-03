#include "Classes/Socket.h"
#include "conio.h"
#include "stdio.h"
#include "Classes/Exception.h"
#include "SDL.h"
#include "Classes/SDLHelper.h"
#include "Classes/Thread.h"


//#define DEBUG 1

DWORD WINAPI Chau(void * p)
{
	Socket server;

	server.Listen(2023);
	/*
	//while(true)
	for(int i = 0; i < 100 ; i++)
	{
		printf("Chau\n");
		Sleep(1000);
	}
	*/

	return 1;
}

DWORD WINAPI Hola(void * p)
{	
	Socket server;

	server.Listen(2022);

	/*
	Thread * t2 = new Thread(Chau);
	t2->Start(NULL);
	///while(true)
	for(int i = 0; i < 100 ; i++)
	{
		printf("Hola\n");
		Sleep(1000);
	}
	*/
	
	return 1;
}

int main(int argc, char* argv[] ) 
{
	Thread * t1 = new Thread(Hola);
	Thread * t2 = new Thread(Chau);
	t1->Start(NULL);
	t2->Start(NULL);


	Socket client;

	client.Connect("127.0.0.1", 2022);

	return 0;
}