#include "Common.h"

#ifndef Stack_h

#define Stack_h

class StackElement
{
	protected:
		void * object;
		StackElement * next;
	public:
		/// Constructor
		StackElement(void * object, StackElement * next)
		{
			this->object = object;
			this->next = next;
		};

		/// Accessors
		void * GetObject() { return this->object; };
		void SetObject(void * object) { this->object = object; };

		StackElement * GetNext() { return this->next; };
		void SetNext(StackElement * next) { this->next = next; };
};

class Stack
{
	public:
		Stack()
		{
			this->topElement = NULL;
		};
		void Push(void *);
		void * Pop();
		bool IsEmpty();
	protected:
		StackElement * topElement;
};

#endif