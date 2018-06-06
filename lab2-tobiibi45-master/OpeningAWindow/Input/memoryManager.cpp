#include "memoryManager.h"
#include <memory>

MemoryManager::MemoryManager(long memoryRequired, short alignmentSize) //!< Constructor
{
	m_alignmentSize = alignmentSize;
	// Make sure memory required is multiple of alignmentSize
	if (memoryRequired % m_alignmentSize)
	{
		memoryRequired += memoryRequired % m_alignmentSize;
	}

	m_memoryRoot = (unsigned char *)_aligned_malloc(memoryRequired, m_alignmentSize);
	m_nextAddress = m_memoryRoot;
	m_off = m_memoryRoot + memoryRequired;

	m_size = memoryRequired;

	// TODO: add check check for capacity
	m_dictionaryCapacity = 100;
	m_dictionaryRoot = m_off - (64 * m_dictionaryCapacity);
	m_dictionarySize = 0;
}

MemoryManager::~MemoryManager()
{
	m_nextAddress = nullptr; 
	m_off = nullptr;
	m_dictionaryRoot = nullptr;
	_aligned_free(m_memoryRoot);
}
unsigned char * MemoryManager::getNextAlignedAddress(unsigned char * addr) //!< Get the next address at the correct alignment
{
	uintptr_t newAddr = (uintptr_t)addr;
	if (newAddr % m_alignmentSize)
	{
		// addr is not aligned
		newAddr = newAddr + m_alignmentSize - (newAddr % m_alignmentSize);
	}
	return (unsigned char *)newAddr;
}

unsigned char * MemoryManager::getPrevoiusAlignedAddress(unsigned char * addr) //!< Get the prevoius address at the correct alignment
{
	return getNextAlignedAddress(addr - m_alignmentSize);
}

unsigned char * MemoryManager::allocateBlock(char key[50], long dataSize) //!< Allocate a block of data
{
	unsigned char * result = m_nextAddress;
	unsigned long capacity = m_size - m_allocatedFromBegin - m_allocatedFromEnd;
	memset(m_nextAddress, 0, dataSize);
	insertDictionaryEntry(key, m_nextAddress);

	m_nextAddress = getNextAlignedAddress(m_nextAddress + dataSize);
	return result;
}

void MemoryManager::insertBlock(char key[50], unsigned char * data, long dataSize) //!< Insert a block of data
{
	unsigned long capacity = m_size - m_allocatedFromBegin - m_allocatedFromEnd;
	memcpy_s(m_nextAddress, capacity, data, dataSize);
	insertDictionaryEntry(key, m_nextAddress);

	m_nextAddress = getNextAlignedAddress(m_nextAddress + dataSize);
}

void MemoryManager::insertDictionaryEntry(char key[50], unsigned char * ptr) //!< Insert entry into the dictionary
{
	if (m_dictionarySize == 0)
	{
		// Add root node
		DictionaryNode * root = (DictionaryNode *)m_dictionaryRoot;
		strcpy_s(root->key, 50, key);
		root->data = ptr;
		root->leftChild = nullptr;
		root->rightChild = nullptr;
	}
	else
	{
		// Add node in spare capacity
		// TODO: Add capacity check
		DictionaryNode * m_NextDictionaryEntry = (DictionaryNode *)m_dictionaryRoot + m_dictionarySize;
		strcpy_s(m_NextDictionaryEntry->key, 50, key);
		m_NextDictionaryEntry->data = ptr;
		m_NextDictionaryEntry->leftChild = nullptr;
		m_NextDictionaryEntry->rightChild = nullptr;

		// Sort out child pointers
		DictionaryNode * walker = (DictionaryNode *)m_dictionaryRoot;
		bool bExit = false;
		while (!bExit)
		{
			if (key < walker->key)
			{
				if (walker->leftChild == nullptr) { bExit = true; }
				else { walker = walker->leftChild; }
			}
			else
			{
				if (walker->rightChild == nullptr) { bExit = true; }
				else { walker = walker->rightChild; }
			}
		}
		if (key < walker->key){walker->leftChild = m_NextDictionaryEntry;}
		else {walker->rightChild = m_NextDictionaryEntry;}
	}
	m_dictionarySize++;
}

unsigned char * MemoryManager::getBlock(char key[50])
{
	DictionaryNode * walker = (DictionaryNode *)m_dictionaryRoot;
	bool bExit = false;
	while (walker != nullptr)
	{
		int iCmp = strcmp(key, walker->key);
		if (iCmp == 0)
		{
			// Keys match
			return walker->data;
		}
		else if (iCmp < 0)
		{
			if (walker->leftChild == nullptr) { return nullptr; }
			else { walker = walker->leftChild; }
		}
		else
		{
			if (walker->rightChild == nullptr) { return nullptr; }
			else { walker = walker->rightChild; }
		}
	}
	return nullptr;

}