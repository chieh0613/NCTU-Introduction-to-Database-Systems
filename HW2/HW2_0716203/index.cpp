#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<climits>
#include<vector>
#include<utility>
#include<stdlib.h>
#include"index.h"

using namespace std;
int MAX; //size of each node
BPTree bpt;

Node::Node()
{
	//dynamic memory allocation
	MAX = 200;
	IS_LEAF = false;
	size = 0;
	key = new pair<int, int>[MAX]();
	ptr = new Node*[MAX + 1]();
}

Index::Index(int num_rows, vector<int> key, vector<int> value)
{

	for (int i = 0; i < num_rows; i++)
	{
		pair<int, int> tmp;
		tmp.first = key[i];//key
		tmp.second = value[i];//value
		bpt.insert(tmp);
	}
}

void Index::key_query(vector<int> query_keys)
{
	//ofstream file("key_query_out.txt");
	FILE* fp;
	fp=fopen("key_query_out.txt","w");
	for (int i = 0; i < query_keys.size(); i++)
	{
		int val = bpt.search(query_keys[i]);
		/*if (file.is_open())
		{
			file << val << endl;
		}*/
		if(fp!=NULL)
		{
		  fprintf(fp,"%d%s",val,"\n");
		}
	}
	fclose(fp);
	//file.close();
}

void Index::range_query(vector<pair<int, int>> query_pairs)
{
	//ofstream file("range_query_out.txt");

	FILE* fp;
	fp=fopen("range_query_out.txt","w");

	for (int i = 0; i < query_pairs.size(); i++)
	{

		int compare = -1;
		Node* cursor = bpt.root;
		while (cursor->IS_LEAF == false)
		{
			for (int k = 0; k < cursor->size; k++)
			{
				if (query_pairs[i].first < cursor->key[k].first)
				{
					cursor = cursor->ptr[k];
					break;
				}
				if (k == cursor->size - 1)
				{
					cursor = cursor->ptr[k + 1];
					break;
				}
			}
		}
		int t = 0;
		//in the following for loop, we search for the key if it exists
		for (int k = 0; k < cursor->size; k++)
		{
			if (cursor->key[k].first >= query_pairs[i].first && cursor->key[k].first <= query_pairs[i].second)
			{
				compare = cursor->key[k].second;
				t = k;
				break;
			}


			if (k == cursor->size - 1)
			{
				cursor = cursor->ptr[k + 1];
				if (cursor == NULL)
					break;
				k = -1;

			}

		}


		if (cursor != NULL) {
			while (cursor->key[t].first <= query_pairs[i].second)
			{

				if (cursor->key[t].second > compare)
					compare = cursor->key[t].second;
				if (t == cursor->size - 1)
				{
					cursor = cursor->ptr[t + 1];
					if (cursor == NULL)
						break;
					t = 0;
					continue;
				}
				t += 1;


			}
		}

		if(fp!=NULL)
		{
		  fprintf(fp,"%d%s",compare,"\n");
		}


		/*if (file.is_open())
		{
			file << compare << endl;;
		}*/
	}
	//file.close();
	fclose(fp);
}

void Index::clear_index()
{
	bpt.cleanUp(bpt.root);
}

BPTree::BPTree()
{
	bpt.root = NULL;
}

int BPTree::search(int x)
{
	//search logic
	Node* cursor = root;
	//in the following while loop, cursor will travel to the leaf node possibly consisting the key
	while (cursor->IS_LEAF == false)
	{
		for (int i = 0; i < cursor->size; i++)
		{
			if (x < cursor->key[i].first)
			{
				cursor = cursor->ptr[i];
				break;
			}
			if (i == cursor->size - 1)
			{
				cursor = cursor->ptr[i + 1];
				break;
			}
		}
	}
	//in the following for loop, we search for the key if it exists
	for (int i = 0; i < cursor->size; i++)
	{
		if (cursor->key[i].first == x)
		{
			return cursor->key[i].second;
		}
	}
	return -1;

}

void BPTree::insert(pair<int, int> tmp)
{
	//insert logic
	if (root == NULL)
	{
		root = new Node;
		root->key[0] = tmp;
		root->IS_LEAF = true;
		root->size = 1;

	}
	else
	{
		Node* cursor = root;
		Node* parent = NULL;
		//in the following while loop, cursor will travel to the leaf node possibly consisting the key
		while (cursor->IS_LEAF == false)
		{
			parent = cursor;
			for (int i = 0; i < cursor->size; i++)
			{
				if (tmp.first < cursor->key[i].first)
				{
					cursor = cursor->ptr[i];
					break;
				}
				if (i == cursor->size - 1)
				{
					cursor = cursor->ptr[i + 1];
					break;
				}
			}
		}
		//now cursor is the leaf node in which we'll insert the new key
		if (cursor->size < MAX)
		{
			//if cursor is not full
			//find the correct position for new key
			int i = 0;
			while (tmp.first > cursor->key[i].first && i < cursor->size) i++;
			//make space for new key
			for (int j = cursor->size; j > i; j--)
			{
				cursor->key[j] = cursor->key[j - 1];
			}
			cursor->key[i] = tmp;
			cursor->size++;
			cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
			cursor->ptr[cursor->size - 1] = NULL;

		}
		else
		{

			//overflow condition
			//create new leaf node
			Node* newLeaf = new Node;
			//create a virtual node and insert x into it
			vector<pair<int, int>> virtualNode(MAX + 1);
			//int virtualNode[MAX + 1];
			for (int i = 0; i < MAX; i++)
			{
				virtualNode[i] = cursor->key[i];
			}
			int i = 0, j;
			while (tmp.first > virtualNode[i].first && i < MAX) i++;
			//make space for new key
			for (int j = MAX; j > i; j--)
			{
				virtualNode[j] = virtualNode[j - 1];
			}
			virtualNode[i] = tmp;
			newLeaf->IS_LEAF = true;
			//split the cursor into two leaf nodes
			cursor->size = (MAX + 1) / 2;
			newLeaf->size = MAX + 1 - (MAX + 1) / 2;
			//make cursor point to new leaf node
			cursor->ptr[cursor->size] = newLeaf;
			//make new leaf node point to the next leaf node
			newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
			cursor->ptr[MAX] = NULL;
			//now give elements to new leaf nodes
			for (i = 0; i < cursor->size; i++)
			{
				cursor->key[i] = virtualNode[i];
			}
			for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++)
			{
				newLeaf->key[i] = virtualNode[j];
			}
			//modify the parent
			if (cursor == root)
			{
				//if cursor is a root node, we create a new root
				Node* newRoot = new Node;
				newRoot->key[0] = newLeaf->key[0];
				newRoot->ptr[0] = cursor;
				newRoot->ptr[1] = newLeaf;
				newRoot->IS_LEAF = false;
				newRoot->size = 1;
				root = newRoot;

			}
			else
			{
				//insert new key in parent node
				insertInternal(newLeaf->key[0], parent, newLeaf);
			}
		}
	}
}
void BPTree::insertInternal(pair<int, int> tmp, Node* cursor, Node* child)
{
	if (cursor->size < MAX)
	{
		//if cursor is not full
		//find the correct position for new key
		int i = 0;
		while (tmp.first > cursor->key[i].first && i < cursor->size) i++;
		//make space for new key
		for (int j = cursor->size; j > i; j--)
		{
			cursor->key[j] = cursor->key[j - 1];
		}//make space for new pointer
		for (int j = cursor->size + 1; j > i + 1; j--)
		{
			cursor->ptr[j] = cursor->ptr[j - 1];
		}
		cursor->key[i] = tmp;
		cursor->size++;
		cursor->ptr[i + 1] = child;

	}
	else
	{

		//if overflow in internal node
		//create new internal node
		Node* newInternal = new Node;
		//create virtual Internal Node;
		vector <pair<int, int>> virtualKey(MAX + 1);

		vector<Node*>virtualPtr(MAX + 2);

		for (int i = 0; i < MAX; i++)
		{
			virtualKey[i] = cursor->key[i];
		}
		for (int i = 0; i < MAX + 1; i++)
		{
			virtualPtr[i] = cursor->ptr[i];
		}
		int i = 0, j;
		while (tmp.first > virtualKey[i].first && i < MAX) i++;
		//make space for new key
		for (int j = MAX; j > i; j--)
		{
			virtualKey[j] = virtualKey[j - 1];
		}
		virtualKey[i] = tmp;
		//make space for new ptr
		for (int j = MAX + 1; j > i + 1; j--)
		{
			virtualPtr[j] = virtualPtr[j - 1];
		}
		virtualPtr[i + 1] = child;
		newInternal->IS_LEAF = false;
		//split cursor into two nodes
		cursor->size = (MAX + 1) / 2;
		newInternal->size = MAX - (MAX + 1) / 2;
		//give elements and pointers to the new node

		for (i = 0; i < cursor->size + 1; i++)
		{
			cursor->key[i] = virtualKey[i];
		}
		for (i = 0; i < cursor->size + 1; i++)
		{
			cursor->ptr[i] = virtualPtr[i];
		}
		for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++)
		{
			newInternal->key[i] = virtualKey[j];
		}
		for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++)
		{
			newInternal->ptr[i] = virtualPtr[j];
		}
		// m = cursor->key[cursor->size]
		if (cursor == root)
		{
			//if cursor is a root node, we create a new root
			Node* newRoot = new Node;
			newRoot->key[0] = cursor->key[cursor->size];
			newRoot->ptr[0] = cursor;
			newRoot->ptr[1] = newInternal;
			newRoot->IS_LEAF = false;
			newRoot->size = 1;
			root = newRoot;

		}
		else
		{
			//recursion
			//find depth first search to find parent of cursor
			insertInternal(cursor->key[cursor->size], findParent(root, cursor), newInternal);
		}
	}
}
Node* BPTree::findParent(Node* cursor, Node* child)
{
	//finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
	//also ignores second last level because we will never find parent of a leaf node during insertion using this function
	Node* parent = NULL;
	if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF)
	{
		return NULL;
	}
	for (int i = 0; i < cursor->size + 1; i++)
	{
		if (cursor->ptr[i] == child)
		{
			parent = cursor;
			return parent;
		}
		else
		{
			parent = findParent(cursor->ptr[i], child);
			if (parent != NULL)return parent;
		}
	}
	return parent;
}

Node* BPTree::getRoot()
{
	return root;
}
void BPTree::cleanUp(Node* cursor)
{
	//clean up logic
	if (cursor != NULL)
	{
		if (cursor->IS_LEAF != true)
		{
			for (int i = 0; i < cursor->size + 1; i++)
			{
				cleanUp(cursor->ptr[i]);
			}
		}
		delete[] cursor->key;
		delete[] cursor->ptr;
		delete cursor;
	}
}

