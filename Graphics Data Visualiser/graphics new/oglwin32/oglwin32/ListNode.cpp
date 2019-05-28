#include "ListNode.h"

ListNode::ListNode()
{
	this->nextNode = NULL;
	pos = 0;
	floatVal = -1;
}

ListNode::ListNode(ListNode* node)
{
	node->nextNode = this;
	this->nextNode = NULL;
	pos = node->pos + 1;
	floatVal = -1;
}

ListNode::~ListNode()
{
	delete nextNode;
	nextNode = NULL;
}

std::string ListNode::GetString()
{
	return strVal;
}

void ListNode::SetString(std::string inStr)
{
	strVal = inStr;
}

int ListNode::GetPos()
{
	return pos;
}

void ListNode::FloatConv()
{
	try
	{
		floatVal = std::stof(strVal, NULL);
	}
	catch (const std::invalid_argument& e)
	{
		floatVal = -1;
	}
}