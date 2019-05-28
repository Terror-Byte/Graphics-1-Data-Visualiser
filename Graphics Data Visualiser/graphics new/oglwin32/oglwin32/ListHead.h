#ifndef LISTHEAD_H
#define LISTHEAD_H

#include "ListNode.h"

class ListHead
{
	public:
		// Constructors + Destructors
		ListHead(bool areHeadings);
		ListHead(ListHead* prevHead);
		~ListHead();

		// Values/Pointers
		ListHead*		nextHead;
		ListNode*		nextNode;

		// Methods
		void			SetPos(int inVal);
		int				GetPos();

	private:
		// Variables
		int				pos;
};
#endif