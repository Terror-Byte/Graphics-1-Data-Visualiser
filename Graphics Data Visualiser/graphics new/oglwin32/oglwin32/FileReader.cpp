#include "FileReader.h"

FileReader::FileReader(wchar_t fName[200], char delimIn)
{
	wcstombs(initialFileName, fName, 200);	
	fileName = initialFileName;
	delim = delimIn;
	initialHead = new ListHead(true);
	currentNode = new ListNode();
	initialHead->nextNode = currentNode;
}

FileReader::~FileReader()
{

}

bool FileReader::Read()
{
	fReader.open(fileName);

	if (fReader.good())
	{
		currentHead = initialHead;

		while (std::getline(fReader, line, '\n'))
		{
			sStream << line;
			while (std::getline(sStream, token, delim))
			{
				currentNode->SetString(token);
				prevNode = currentNode;
				currentNode = new ListNode(prevNode);
			}

			if (initialRead)
				initialHead = currentHead, initialRead = false;

			prevHead = currentHead;
			currentHead = new ListHead(prevHead);
			currentNode = new ListNode();
			currentHead->nextNode = currentNode;
			sStream.clear();
		}
	}
	else
	{
		return false;
	}
	return true;
}

ListHead* FileReader::GetInitialHead()
{
	return initialHead;
}

void FileReader::PrintEntry(int entryNo)
{
	ListHead* currentEntryHead = initialHead;
	ListNode* currentEntryNode = NULL;

	while (currentEntryHead != NULL)
	{
		if (currentEntryHead->GetPos() == entryNo)
		{
			currentEntryNode = currentEntryHead->nextNode;
			while (currentEntryNode != NULL)
			{
				std::cout << currentEntryNode->GetString() << ",";
				currentEntryNode = currentEntryNode->nextNode;
			}
			std::cout << std::endl;
			break;
		}
		currentEntryHead = currentEntryHead->nextHead;
	}
}