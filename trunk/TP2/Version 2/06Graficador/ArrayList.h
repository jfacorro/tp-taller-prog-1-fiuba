#include "Common.h"

#ifndef ArrayList_h

#define ArrayList_h

class ArrayElement
{
	public:
		/// Constructor
		ArrayElement(ArrayElement * next, ArrayElement * previous, void * object)
		{
			this->next = next;
			this->previous = previous;
			this->object = object;
		};

		/// Accessors
		void * GetObject() { return this->object; };
		void SetObject(void * object) { this->object = object; };

		ArrayElement * GetNext() { return this->next; };
		void SetNext(ArrayElement * next) { this->next = next; };

		ArrayElement * GetPrevious() { return this->previous; };
		void SetPrevious(ArrayElement * previous) { this->previous = previous; };
	private:
		ArrayElement * next;
		ArrayElement * previous;
		void * object;
};

class ArrayList
{
	public:
		ArrayList()
		{
			this->first = NULL;
			this->current = NULL;
		};

		void Add(void * object);
		void * GetCurrent();
		void * RemoveCurrent();

		bool MoveNext();
		bool MovePrevious();
		bool MoveFirst();
		bool MoveLast();

		bool IsEmpty();
		
	private:
		ArrayElement * first;
		ArrayElement * current;
};

#endif