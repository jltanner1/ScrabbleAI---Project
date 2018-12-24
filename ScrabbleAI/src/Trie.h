#ifndef TRIE_H_
#define TRIE_H_

#include <iostream>
#include <string>
#include <set>
#include "Dictionary.h"

struct TrieNode {

	bool inSet;	//if this node is the end of a string in the dictionary
	TrieNode* parent;
	TrieNode* children[26]; //one child for each letter of the alphabet

	TrieNode(TrieNode* _p, bool _end);
	~TrieNode();
};


class TrieSet {

	public:
		TrieSet();
		~TrieSet();

		void insert (std::string input);	//add string to the set, do nothing if it already exists in set
		void remove (std::string remove);	//remove string from the set, do nothing if the string is not in set
		TrieNode* prefix (std::string px);	//return TrieNode matching provided stirng, or NULL if it is not the prefix of a word in the set
		int childrenCheck(TrieNode* current); //return how many children a TrieNode has
		void fill(Dictionary* d);			//fills trie with dictionary words
		void clear(TrieNode* current);		//recursively delete all memory allocated to trie

		TrieNode* root;						//the root node of the trie
};


#endif /*TRIE_H_*/