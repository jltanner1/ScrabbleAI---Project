#include <iostream>
#include <string>
#include <set>
#include "Trie.h"
#include "Dictionary.h"

TrieNode::TrieNode(TrieNode* _parent, bool _end) {

	inSet = _end;
	parent = _parent;

	for (int i = 0; i < 26; i++) children[i] = NULL;
}

TrieNode::~TrieNode() {}

TrieSet::TrieSet() {

	root = new TrieNode(NULL, false);
}

TrieSet::~TrieSet() {

	clear(root);
	root = NULL;
}

void TrieSet::insert(std::string input) {

	TrieNode* temp = root;
    
	for (unsigned int i = 0; i < input.size(); ++i) {

		if (temp->children[std::tolower(input[i]) -'a'] == NULL) {

			if (i == input.size()-1) temp->children[std::tolower(input[i]) - 'a'] = new TrieNode(temp, true);
			else temp->children[std::tolower(input[i]) - 'a'] = new TrieNode(temp, false);
		}

		temp = temp->children[std::tolower(input[i]) - 'a'];
	}
}

void TrieSet::remove(std::string input) {

	TrieNode* temp = prefix(input);

	if (temp == NULL) return; //if word not in Trie

	unsigned int i = input.size()-1; //current of string we are checking
	int num_children = -1;
	temp->inSet = false; //mark input as not a word in trie
	TrieNode* Parent = NULL;

	while (temp != NULL) {

		num_children = childrenCheck(temp);
		Parent = temp->parent;

		if (Parent != NULL && temp->inSet == false && num_children == 0) { //if not the root, not a word node, and no children --> delete
			delete temp;
			Parent->children[std::tolower(input[i])-'a'] = NULL; //set children pointer of deleted node to NULL
			temp = Parent;
		}

		else temp = Parent;

		i--;
	}
}

TrieNode* TrieSet::prefix(std::string px) {

	TrieNode* temp = root;

	for (unsigned int i = 0; i < px.size(); ++i) {

		if (temp->children[std::tolower(px[i]) - 'a'] == NULL) return NULL;	//if a letter of px is not in the tree

		temp = temp->children[std::tolower(px[i]) - 'a'];
	}

	return temp;
}

int TrieSet::childrenCheck(TrieNode* current) {
	int count = 0;

	for (unsigned int i = 0; i < 26; ++i) {

		if (current->children[i] != NULL) ++count; //counter for # of children branches of current
	}

	return count;
}

void TrieSet::fill(Dictionary* d) {

	std::set<std::string>::iterator it;
	std::set<std::string> dictionary = d->getDict();

	for (it = dictionary.begin(); it != dictionary.end(); ++it) insert(*it);
}

void TrieSet::clear(TrieNode* current) {

	for(unsigned int i = 0; i < 26; i++) {

		if(current->children[i] != NULL) clear(current->children[i]);
	}
	delete current;
}
