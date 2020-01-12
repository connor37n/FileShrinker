#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
using namespace std;

const int NUM_BUCKETS = 997; // Size of HashTable using an array of pointers.
const int SUBSEQ_LENGTH = 24; // Size of subseq's placed in table.

class HashTable
{
public:
	HashTable();
	~HashTable();
	void insert(string s, int o);
	bool lookUpSubseq(string sub, int& offset) const;
	void printTable() const;
private:
	struct Bucket
	{
		Bucket(string s, int o);
		string m_subSeq;
		int m_offset;
		Bucket* next;
	};
	Bucket* m_buckets[NUM_BUCKETS];
	// Helper Functions:
	int hashFunction(string hashMe) const;
	void deleteElement(Bucket* b);
	bool searchList(Bucket* b, string sub, int& o) const;
	void printList(Bucket* b, int o) const;
};

#endif // HASHTABLE_H