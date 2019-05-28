#include "ListNode.h"
#include "ListHead.h"

ListHead::ListHead(bool areHeadings)
{
	this->nextHead = NULL;
	this->nextNode = NULL;
	if (areHeadings)
		pos = -1;
	else
		pos = 0;
}

ListHead::~ListHead()
{
	delete nextNode;
	delete nextHead;
	nextNode = NULL;
	nextHead = NULL;
}

ListHead::ListHead(ListHead* prevHead)
{
	prevHead->nextHead = this;
	this->nextHead = NULL;
	this->nextNode = NULL;
	pos = prevHead->GetPos() + 1;
}

int ListHead::GetPos()
{
	return pos;
}

void ListHead::SetPos(int inVal)
{
	pos = inVal;
}