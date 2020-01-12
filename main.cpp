#if defined(_MSC_VER)  &&  !defined(_DEBUG)
#include <iostream>
#include <windows.h>
#include <conio.h>

struct KeepWindowOpenUntilDismissed
{
	~KeepWindowOpenUntilDismissed()
	{
		DWORD pids[1];
		if (GetConsoleProcessList(pids, 1) == 1)
		{
			std::cout << "Press any key to continue . . . ";
			_getch();
		}
	}
} keepWindowOpenUntilDismissed;
#endif

#include "HashTable.h"
#include <iostream>
#include <fstream> // File Input/Output
#include <sstream> // String in/output?
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

// ================================================================
// ======================= DECLARATION ============================
// ================================================================

void createDiff(istream& fold, istream& fnew, ostream& fdiff);
bool applyDiff(istream& fold, istream& fdiff, ostream& fnew);

string createAdd(int length, string text);
string createCopy(int length, int offset);
void readFromText(istream& ffile, string& s1);
void writeToText(ostream& fdiff, string diff);
bool createSubseq(int o, string s, string& sub);
bool compareFiles(istream& file1, istream& file2);

bool getInt(istream& inf, int& n);
bool getCommand(istream& inf, char& cmd, int& length, int& offset);
bool runtest(string oldName, string newName, string diffName, string newName2);
bool runtestBS(string newName, string newName2);
void runtest(string oldtext, string newtext);

// =================================================================
// ===================== IMPLEMENTATION ============================
// =================================================================

//int main()
//{
//	// Test:
//	ifstream infile1("test1.txt", ios::binary);  // Old
//	ifstream infile2("test1.txt", ios::binary);  // Old
//	ifstream infile3("test3.txt", ios::binary);  // Diff1
//	ifstream infile4("test4.txt", ios::binary);  // Diff2
//
//	ofstream outfile1("test5.txt", ios::binary); // New1
//	ofstream outfile2("test6.txt", ios::binary); // New2
//	applyDiff(infile1, infile3, outfile1);
//	applyDiff(infile2, infile4, outfile2);
//
//	ifstream comp1("test5.txt", ios::binary);    // New1
//	ifstream comp2("test6.txt", ios::binary);    // New2
//	assert(compareFiles(comp1, comp2));
//
//	cout << "Test passed" << endl;
//
//
//	// Small-Mart:
//	ifstream infile5("smallmart1.txt", ios::binary);   // Old
//	ifstream infile6("smallmart2.txt", ios::binary);   // New
//	ofstream outfile3("smallmart3.txt", ios::binary); // Diff
//	createDiff(infile5, infile6, outfile3);
//
//	ifstream infile7("smallmart1.txt", ios::binary);   // Old
//	ifstream infile8("smallmart3.txt", ios::binary);  // Diff
//	ofstream outfile4("smallmart4.txt", ios::binary); // New2
//	applyDiff(infile7, infile8, outfile4);
//
//	ifstream comp3("smallmart2.txt", ios::binary);     // New
//	ifstream comp4("smallmart4.txt", ios::binary);    // New2
//	assert(compareFiles(comp3, comp4));
//
//	cout << "Small-Mart passed" << endl;
//
//
//	// War and Peace:
//	ifstream infile9("warandpeace1.txt", ios::binary);  // Old
//	ifstream infile10("warandpeace2.txt", ios::binary); // New
//	ofstream outfile5("warandpeace3.txt", ios::binary); // Diff
//	createDiff(infile9, infile10, outfile5);
//
//	ifstream infile11("warandpeace1.txt", ios::binary); // Old
//	ifstream infile12("warandpeace3.txt", ios::binary); // Diff
//	ofstream outfile6("warandpeace4.txt", ios::binary); // New2
//	applyDiff(infile11, infile12, outfile6);
//
//	ifstream comp5("warandpeace2.txt", ios::binary);    // New
//	ifstream comp6("warandpeace4.txt", ios::binary);    // New2
//	assert(compareFiles(comp5, comp6));
//
//	cout << "War and Peace passed" << endl;
//
//
//	// Strange:
//	ifstream infile13("strange1.txt", ios::binary); // Old
//	ifstream infile14("strange2.txt", ios::binary); // New
//	ofstream outfile7("strange3.txt", ios::binary); // Diff
//	createDiff(infile13, infile14, outfile7);
//
//	ifstream infile15("strange1.txt", ios::binary); // Old
//	ifstream infile16("strange3.txt", ios::binary); // Diff
//	ofstream outfile8("strange4.txt", ios::binary); // New2
//	applyDiff(infile15, infile16, outfile8);
//
//	ifstream comp7("strange2.txt", ios::binary);    // New
//	ifstream comp8("strange4.txt", ios::binary);    // New2
//	assert(compareFiles(comp7, comp8));
//
//	cout << "Strange passed" << endl;
//
//	// Dr. Seuss:
//	ifstream infile17("greeneggs1.txt", ios::binary); // Old
//	ifstream infile18("greeneggs2.txt", ios::binary); // New
//	ofstream outfile9("greeneggs3.txt", ios::binary); // Diff
//	createDiff(infile17, infile18, outfile9);
//
//	ifstream infile19("greeneggs1.txt", ios::binary); // Old
//	ifstream infile20("greeneggs3.txt", ios::binary); // Diff
//	ofstream outfile10("greeneggs4.txt", ios::binary); // New2
//	applyDiff(infile19, infile20, outfile10);
//
//	ifstream comp9("greeneggs2.txt", ios::binary);    // New
//	ifstream comp10("greeneggs4.txt", ios::binary);    // New2
//	assert(compareFiles(comp9, comp10));
//
//	cout << "Dr. Seuss passed" << endl;
//}

int main()
{
	runtest("test1.txt", "test2.txt", "test3.txt", "test4.txt");
	cout << "passed Test" << endl;
	runtest("warandpeace1.txt", "warandpeace2.txt", "warandpeace3.txt", "warandpeace4.txt");
	cout << "passed War and Peace" << endl;
	runtest("greeneggs1.txt", "greeneggs2.txt", "greeneggs3.txt", "greeneggs4.txt");
	cout << "passed Green Eggs" << endl;
	runtest("smallmart1.txt", "smallmart2.txt", "smallmart3.txt", "smallmart4.txt");
	cout << "passed Small-Mart" << endl;
	runtest("strange1.txt", "strange2.txt", "strange3.txt", "strange4.txt");
	//assert(runtestBS("strange2.txt", "strange4.txt"));
	cout << "passed Strange" << endl;

	//// Small-Mart:
	//ifstream infile5("smallmart1.txt", ios::binary);  // Old
	//ifstream infile6("smallmart2.txt", ios::binary);  // New
	//ofstream outfile3("smallmart3.txt", ios::binary); // Diff
	//createDiff(infile5, infile6, outfile3);

	//cout << "created" << endl;

	//ifstream infile7("smallmart1.txt", ios::binary);  // Old
	//ifstream infile8("smallmart3.txt", ios::binary);  // Diff
	//ofstream outfile4("smallmart4.txt", ios::binary); // New2
	//applyDiff(infile7, infile8, outfile4);

	//cout << "applied" << endl;

	//ifstream comp3("smallmart2.txt", ios::binary);    // New
	//ifstream comp4("smallmart4.txt", ios::binary);    // New2
	//assert(compareFiles(comp3, comp4));

	//cout << "Small-Mart passed" << endl;

	//====================================================================

	//assert(runtestBS("alpha1.txt", "alpha0.txt")); // Control
	//assert(runtestBS("alpha1.txt", "alpha2.txt")); // Spaces Between -Fail!
	//assert(runtestBS("alpha1.txt", "alpha3.txt")); // Space at end -Fail!

	//====================================================================

	//string x;
	//ifstream infileA1("alpha1.txt", ios::binary);
	//ifstream infileA2("alpha2.txt", ios::binary);
	//ifstream infileA3("alpha3.txt", ios::binary);

	//cout << "1" << endl;
	//readFromText(infileA1, x);

	//cout << "2" << endl;
	//readFromText(infileA2, x);

	//cout << "3" << endl;
	//readFromText(infileA3, x);

	//===================================================================

	//runtest("ABCDEFGHIJBLAHPQRSTUVPQRSTUV", "XYABCDEFGHIJBLETCHPQRSTUVPQRSTQQELF"); // Test
	cout << "passed" << endl;
}

void createDiff(istream& fold, istream& fnew, ostream& fdiff)
{
	string oldS, newS, diff, sub, rejects, tail = "";
	int offset, k = 0;
	readFromText(fold, oldS);                             // Put Old File into Old String.
	readFromText(fnew, newS);                             // Put New File into New String.

	if (newS.size() < SUBSEQ_LENGTH) {                    // If new string is smaller than subseq_length.      
		diff = createAdd(newS.length(), newS);            // Treat new string like a tail string.
		writeToText(fdiff, diff);                         // Print basic diff string consisting of a single add instruction.
		return;
	}

	HashTable h;
	while (createSubseq(k, oldS, sub)) {                  // Insert the sliding window of subsequences into the HashTable.
		h.insert(sub, k);
		k++;
	}
	k = 0;
	vector<string> newSub;
	while (createSubseq(k, newS, sub)) {                  // Create a vector of new string's sliding subsequences.
		newSub.push_back(sub);
		k++;
	}
	size_t i = 0;
	while (i < newSub.size()) {                           // For every N length subseq in the new string.
		if (h.lookUpSubseq(newSub[i], offset)) {          // If subseq is found in HashTable.
			int len = SUBSEQ_LENGTH;                      // If subseq is found, then match length is now subseq_length.
			if (!rejects.empty())                         // Finalize reject add instruction if new char's are found.
				diff += createAdd(rejects.size(), rejects);
			rejects = "";                                 // Empty reject string when char's have been applied.
			i += SUBSEQ_LENGTH;                           // Index moves to first char of new subseq.
			size_t j = offset + SUBSEQ_LENGTH;
			while (i < newS.size()
				&& j < oldS.size() && oldS[j] == newS[i]) // Check if next char in old string matches next char in new string.
				i++, j++, len++;                          // If char doesn't match, stop searching for contiguous matches.
			diff += createCopy(len, offset);              // Create Copy Instruction using subseq_length plus any additional matches.
		}
		else {                                            // If the subseq isn't found.
			rejects += newS[i];                           // Append the first char of the reject subseq- the reject char.
			i++;                                          // Iterate to next char.
		}
	}

	for (; i < newS.size(); i++)                          // Create the "tail" string.
		tail += newS[i];
	rejects += tail;                                      // Append tail string to reject string.
	diff += createAdd(rejects.size(), rejects);           // Create an add instruction for last subseq_length-1 char's.

	writeToText(fdiff, diff);                             // Print complete diff string to output file.
}

bool applyDiff(istream& fold, istream& fdiff, ostream& fnew)
{
	string oldS, newS, diff;
	int length, offset;
	char cmd;
	readFromText(fold, oldS);                                  // Copy files into strings.

	for (;;) {                                                 // Loop through entire diff string.
		if (getCommand(fdiff, cmd, length, offset)) {          // If valid character (C, A, or newline/carriage).
			if (cmd == 'x')                                    // Marks end of diff file.
				break;
			else if (cmd == 'C') {                             // Indicates Copy Instruction.
				string temp;
				if (offset + length > oldS.size())             // Can't access string out of bounds.
					return false;
				for (int j = offset; j < offset + length; j++) // Copy values from offset to offset + length - 1.
					newS += oldS[j];                           // Append char by char.
			}
			else if (cmd == 'A') {                             // Indicates Add Instruction.
				int j = 0;
				char cc;
				while (j < length && fdiff.get(cc)) {          // Get char's following the colon in the diff file.
					newS += cc;                                // Append these char's to the new string.
					j++;
				}
				if (j != length)                               // Return false if Add Instruction doesn't have enough char's.
					return false;
			}
		}
	}
	writeToText(fnew, newS);                                   // Print new string onto file.
	return true;
}

// Helper Functions:

bool createSubseq(int o, string s, string& sub) // Create a Subseq from a string and an offset
{
	sub = "";
	if (o > s.length() - SUBSEQ_LENGTH)         // If the main string doesn't have enough char's for a new subseq.
		return false;                           // Return false, subseq not formed.
	for (int i = o; i < o + SUBSEQ_LENGTH; i++) // Start at offset and end at offset + sub_length.
		sub += s[i];                            // Append char to subseq.
	return true;                                // Return true, subseq formed successfully.
}

void writeToText(ostream& fdiff, string diff) // Print string to file.
{
	for (int i = 0; i < diff.size(); i++) {
		fdiff << diff[i];
	}
}

void readFromText(istream& ffile, string& s1) // Put the contents of a text file into a string.
{
	s1 = "";
	char c;
	while (ffile.get(c)) {
		s1 += c;
	}
}

string createAdd(int length, string text) // Create an Add instruction.
{
	if (text.empty())
		return text;
	string len = to_string(length);       // Convert int's to string's.
	string x = "A" + len;
	string y = ":" + text;
	return x + y;                         // Append and return elements of Add Instruction.
}

string createCopy(int length, int offset) // Create a Copy instruction.
{
	string len = to_string(length);       // Convert int to string.
	string off = to_string(offset);
	string x = "C" + len;
	string y = "," + off;
	return x + y;                         // Append and return elements of Copy Instruction.
}

bool compareFiles(istream& file1, istream& file2) // Return true if the content of two files is the same.
{
	string s1, s2;
	readFromText(file1, s1);
	readFromText(file2, s2);
	return(s1 == s2);
}

// Given Functions:

bool getCommand(istream& inf, char& cmd, int& length, int& offset) // Given by Prof. Smallberg
{
	if (!inf.get(cmd))
	{
		cmd = 'x';  // signals end of file
		return true;
	}
	char ch;
	switch (cmd)
	{
	case 'A':
		return getInt(inf, length) && inf.get(ch) && ch == ':';
	case 'C':
		return getInt(inf, length) && inf.get(ch) && ch == ',' && getInt(inf, offset);
	case '\r':
	case '\n':
		return true;
	}
	return false;
}

bool getInt(istream& inf, int& n) // Given by Prof. Smallberg
{
	char ch;
	if (!inf.get(ch) || !isascii(ch) || !isdigit(ch))
		return false;
	inf.unget();
	inf >> n;
	return true;
}

bool runtest(string oldName, string newName, string diffName, string newName2) // Given by Prof. Smallberg
{
	if (diffName == oldName || diffName == newName ||
		newName2 == oldName || newName2 == diffName ||
		newName2 == newName)
	{
		cerr << "Files used for output must have names distinct from other files" << endl;
		return false;
	}
	ifstream oldFile(oldName, ios::binary);
	if (!oldFile)
	{
		cerr << "Cannot open " << oldName << endl;
		return false;
	}
	ifstream newFile(newName, ios::binary);
	if (!newFile)
	{
		cerr << "Cannot open " << newName << endl;
		return false;
	}
	ofstream diffFile(diffName, ios::binary);
	if (!diffFile)
	{
		cerr << "Cannot create " << diffName << endl;
		return false;
	}
	createDiff(oldFile, newFile, diffFile);
	diffFile.close();

	oldFile.clear();   // clear the end of file condition
	oldFile.seekg(0);  // reset back to beginning of the file
	ifstream diffFile2(diffName, ios::binary);
	if (!diffFile2)
	{
		cerr << "Cannot read the " << diffName << " that was just created!" << endl;
		return false;
	}
	ofstream newFile2(newName2, ios::binary);
	if (!newFile2)
	{
		cerr << "Cannot create " << newName2 << endl;
		return false;
	}
	applyDiff(oldFile, diffFile2, newFile2);
	newFile2.close();

	newFile.clear();
	newFile.seekg(0);
	ifstream newFile3(newName2, ios::binary);
	if (!newFile)
	{
		cerr << "Cannot open " << newName2 << endl;
		return false;
	}

	if (!equal(istreambuf_iterator<char>(newFile), istreambuf_iterator<char>(),
		istreambuf_iterator<char>(newFile3), istreambuf_iterator<char>()))
	{

		cerr << newName2 << " is not identical to " << newName
			<< "; test FAILED" << endl;
		return false;
	}
	return true;
}

void runtest(string oldtext, string newtext)
{
	istringstream oldFile(oldtext);
	istringstream newFile(newtext);
	ostringstream diffFile;
	createDiff(oldFile, newFile, diffFile);
	string result = diffFile.str();
	cout << "The diff file length is " << result.size()
		<< " and its text is " << endl;
	cout << result << endl;

	oldFile.clear();   // clear the end of file condition
	oldFile.seekg(0);  // reset back to beginning of the stream
	istringstream diffFile2(result);
	ostringstream newFile2;
	assert(applyDiff(oldFile, diffFile2, newFile2));
	assert(newtext == newFile2.str());
}

bool runtestBS(string newName, string newName2) // Trimmed the Fat.
{
	ifstream newFile(newName, ios::binary);
	ifstream newFile3(newName2, ios::binary);

	if (!equal(istreambuf_iterator<char>(newFile), istreambuf_iterator<char>(),
		istreambuf_iterator<char>(newFile3), istreambuf_iterator<char>()))
	{

		cerr << newName2 << " is not identical to " << newName
			<< "; test FAILED" << endl;
		return false;
	}
	return true;
}
