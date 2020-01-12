#include "HashTable.h"
#include <iostream>  
#include <functional> // String Hash

HashTable::Bucket::Bucket(string s, int o) // Bucket Constructor
	:m_subSeq(s), m_offset(o), next(nullptr) {}

HashTable::HashTable() // HashTable Constructor
{
	for (int i = 0; i < NUM_BUCKETS; i++)
		m_buckets[i] = nullptr;
}

HashTable::~HashTable() // HashTable Destructor
{
	for (int i = 0; i < NUM_BUCKETS; i++)
		deleteElement(m_buckets[i]);
}

void HashTable::deleteElement(Bucket* b) // Destruct Linked List, Helper Function for HashTable Destructor
{
	if (b == nullptr)
		return;
	Bucket* p = b->next;
	delete b;
	deleteElement(p);
}

void HashTable::insert(string s, int o) // Insert Bucket
{
	int i = hashFunction(s);            // Put bucket number into i.
	if (m_buckets[i] == nullptr) {      // If empty list, simply create a new bucket at index.
		m_buckets[i] = new Bucket(s, o);
		return;
	}
	Bucket* b = m_buckets[i];           // Point last and b at start of list.
	Bucket* last = b;
	while (b != nullptr)                // Move b to where new Bucket will be allocated.
		b = b->next;
	while (last->next != nullptr)       // Move last to where the last Bucket in list is located.
		last = last->next;
	b = new Bucket(s, o);               // Allocate new bucket just past the end of the list.
	last->next = b;                     // Point last bucket's next pointer at new bucket.
}

int HashTable::hashFunction(string hashMe) const // Hash Function for HashTable
{
	unsigned int hashValue = hash<string>()(hashMe);
	int bucketN = hashValue % NUM_BUCKETS;
	return bucketN;
}


bool HashTable::lookUpSubseq(string sub, int& offset) const // Check if subsequence is in table
{
	int bucketNum = hashFunction(sub);
	if (searchList(m_buckets[bucketNum], sub, offset))      // Search in Linked List at Hash Table index.
		return true;                                        // Subseq found.
	return false;                                           // Subseq not found.
}

bool HashTable::searchList(Bucket* b, string sub, int& o) const // Search Linked List for subseq, 
{                                                               // Helper Function for HashTable::lookUpSubseq()
	if (b == nullptr)
		return false;
	if (b->m_subSeq == sub) {
		o = b->m_offset;
		return true;
	}
	return searchList(b->next, sub, o);
}

void HashTable::printTable() const // Print Table List by List
{
	for (int i = 0; i < NUM_BUCKETS; i++)
		printList(m_buckets[i], i);
}

void HashTable::printList(Bucket* b, int i) const // Print the Linked List at specified Table index
{
	if (b == nullptr)
		return;
	cout << "Index: " << i << ", Subseq: " << b->m_subSeq << ", Offset: " << b->m_offset << endl;
	printList(b->next, i);
}