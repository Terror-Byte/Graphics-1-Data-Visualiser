#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "ListHead.h"
#include "ListNode.h"

class FileReader
{
public:
						FileReader(wchar_t fileName[200], char delimIn);
						~FileReader();
	bool				Read();
	ListHead*			GetInitialHead();
	void				PrintEntry(int entryNo);

private:
	ListHead*			initialHead;
	ListHead*			currentHead;
	ListHead*			prevHead;

	ListNode*			currentNode;
	ListNode*			prevNode;

	std::fstream		fReader;
	std::stringstream	sStream;

	char				initialFileName[200];
	std::string			fileName;
	std::string			line;
	std::string			token;

	bool				initialRead = true;
	char				delim;
};