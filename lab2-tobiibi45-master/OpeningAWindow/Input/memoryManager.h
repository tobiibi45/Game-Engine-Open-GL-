#pragma once

#include <stdlib.h>
#include <string.h>

class DictionaryNode // Size of 64 bytes
{
public:
	char key[50];
	unsigned char * data;
	DictionaryNode * leftChild;
	DictionaryNode * rightChild;
};

class MemoryManager
{
private:
	unsigned char * m_memoryRoot; //!< Root address of the allocated memory
	unsigned char * m_nextAddress; //!< Next availible aligned address
	unsigned char * m_off; //!< First address past the allocated memory

	unsigned char * m_dictionaryRoot; //!< Root node of the dictionary
	unsigned long m_dictionaryCapacity; //!< Number entries allowed in the dictionary
	unsigned long m_dictionarySize; //!< Number of dictionary entries already allocated

	unsigned long m_alignmentSize; //!< Size to align memory too

	unsigned char * getNextAlignedAddress(unsigned char *); //!< Get the next address at the correct alignment
	unsigned char * getPrevoiusAlignedAddress(unsigned char *); //!< Get the prevoius address at the correct alignment

	unsigned long m_size; //!< Amount of memory allocated
	unsigned long m_allocatedFromBegin; //!< Amount of memory allocate from memory root
	unsigned long m_allocatedFromEnd; //!< Amount of memory allocated from off

	void insertDictionaryEntry(char key[50], unsigned char * ptr); //!< Insert entry into the dictionary
public:
	MemoryManager(long memoryRequired, short alignmentSize); //!< Constructor
	~MemoryManager(); //!< Destructor
	unsigned char * allocateBlock(char key[50], long dataSize); //!< Allocate a block of data
	void insertBlock(char key[50], unsigned char * data, long dataSize); //!< Insert a block of data
	unsigned char * getBlock(char key[50]); //!< Retreive a block of data from it's key
};
