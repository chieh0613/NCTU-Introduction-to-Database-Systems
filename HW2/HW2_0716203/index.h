#include<vector>
#include<utility>
using namespace std;

#ifndef INDEX_H
#define INDEX_H

class Index
{
	friend class Node;
public:
	Index(int,vector<int>,vector<int>);
	void key_query(vector<int>);
	void range_query(vector<pair<int, int>>);
	void clear_index();
};

class Node
{
	friend class BPTree;
public:
	bool IS_LEAF;
	pair<int, int> *key;
	int size;
	Node** ptr;
	Node();
};
class BPTree
{
	friend class Index;
public:
	Node *root;
	void insertInternal(pair<int,int>, Node*, Node*);
	Node* findParent(Node*, Node*);
	BPTree();
	int search(int);
	void insert(pair<int, int>);
	Node* getRoot();
	void cleanUp(Node*);
	
};
#endif // !INDEX_H
