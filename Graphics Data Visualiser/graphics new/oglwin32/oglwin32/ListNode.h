#ifndef LISTNODE_H
#define LISTNODE_H
#include <string>
#include <stdexcept>
class ListNode
{
	public:
		// Constructors + Destructor
		ListNode();
		ListNode(ListNode* node);
		~ListNode();

		// Variables/Pointers
		ListNode*		nextNode;
	
		std::string     GetString();
		void			SetString(std::string inStr);
		int				GetPos();
		void			FloatConv();

	private:
		// Variables
		float			floatVal;
		int				pos;
		std::string		strVal;
};
#endif
