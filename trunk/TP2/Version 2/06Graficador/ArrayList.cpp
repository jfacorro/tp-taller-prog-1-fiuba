#include "ArrayList.h"

#ifndef ArrayList_cpp

#define ArrayList_cpp

void ArrayList::Add(void * object)
{
	ArrayElement * arrElement;

	if(this->first == NULL)
	{
		arrElement = new ArrayElement(NULL, NULL, object);
		this->first = arrElement;
		this->current = arrElement;
	}
	else
	{
		ArrayElement * arrNavigateElement = this->first;

		while(arrNavigateElement->GetNext() != NULL)
		{
			arrNavigateElement = arrNavigateElement->GetNext();
		}

		arrElement = new ArrayElement(NULL, arrNavigateElement, object);

		arrNavigateElement->SetNext(arrElement);
	}
}

void * ArrayList::GetCurrent()
{
	if(this->current != NULL)
	{
		return this->current->GetObject();
	}

	return NULL;
}

void * ArrayList::RemoveCurrent()
{
	if(this->current != NULL)
	{
		void * object = this->current->GetObject();
		
		ArrayElement * next = this->current->GetNext();
		ArrayElement * previous = this->current->GetPrevious();
		ArrayElement * exCurrent = this->current;

		if(next != NULL) next->SetPrevious(previous);
		if(previous != NULL)
		{
			previous->SetNext(previous);
		}
		else
		{
			this->first = next;
		}


		if(next != NULL)
			this->current = next;
		else if(previous != NULL)
			this->current = previous;
		else 
			this->current = NULL;			

		delete exCurrent;

		return object;
	}

	return NULL;
}

bool ArrayList::MoveNext()
{
	if(this->current->GetNext() != NULL)
	{
		this->current = this->current->GetNext();

		return true;
	}

	return false;
}

bool ArrayList::MoveLast()
{
	if(this->first != NULL)
	{
		ArrayElement * arrElement = this->first;

		while(arrElement->GetNext() != NULL)
		{
			arrElement = arrElement->GetNext();
		}

		this->current = arrElement;

		return true;
	}

	return false;
}

bool ArrayList::MovePrevious()
{
	if(this->current->GetPrevious() != this->first)
	{
		this->current = this->current->GetPrevious();

		return true;
	}

	return false;
}

bool ArrayList::MoveFirst()
{
	if(this->first != NULL)
	{
		this->current = this->first;

		return true;
	}

	return false;
}

bool ArrayList::IsEmpty() 
{
	return (this->first == NULL); 
}


#endif