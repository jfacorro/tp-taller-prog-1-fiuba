#include "Stack.h"

#ifndef Stack_cpp

#define Stack_cpp

void Stack::Push(void * object)
{
	StackElement * stackElement = new StackElement(object, this->topElement);
	this->topElement = stackElement;
}

void * Stack::Pop()
{
	StackElement * stackElement = this->topElement;

	this->topElement = stackElement->GetNext();

	void * object = stackElement->GetObject();
	
	delete stackElement;

	return object;
}

bool Stack::IsEmpty()
{
	return (this->topElement == NULL);
}

#endif