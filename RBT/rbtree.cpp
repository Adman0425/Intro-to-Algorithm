#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>

using namespace std;
ofstream out;

class RBT;
class RBT_node {/*node constructor*/ 
private:
	int key;
	int color;/*red=0, black=1*/ 
	RBT_node* leftchild;
	RBT_node* rightchild;
	RBT_node* parent;
	friend class RBT;

public:
	RBT_node() :leftchild(0), rightchild(0), parent(0), key(0), color(0) {}
	RBT_node(int key) :leftchild(0), rightchild(0), parent(0), key(key), color(0) {}

	//int GetKey()const { return key; };
};

class RBT {
private:
	RBT_node* root;
	RBT_node* N;/*NULL*/ 

	void LeftRotation(RBT_node* current);/*左旋*/
	void RightRotation(RBT_node* current);/*右旋*/ 
	void Insert_Fix(RBT_node* current);/*調整顏色*/ 
	void Delete_Fix(RBT_node* current);/*調整顏色*/ 

	RBT_node* successor(RBT_node* current);/*找successor*/ 
	RBT_node* predecessor(RBT_node* current);/*找predecessor*/

public:
	RBT() {/*construct 紅黑樹*/
		N = new RBT_node;/*NULL is a RBT node*/
		N->color = 1;/*顏色為黑*/ 
		root = N;/*設作root*/ 
		root->parent = N;/*root的parent為NULL*/ 
	};

	void InsertRBT(int key);
	void DeleteRBT(int key);
	void InOrder(RBT_node* current);
	void Output_RBT(RBT_node* current);

	RBT_node* search(int key);
	RBT_node* getRoot();
};

void RBT::Delete_Fix(RBT_node* current) {
	while (current != root && current->color == 1) {/*If current isn't root and color is black*/
		
		if (current->parent->leftchild == current) {/*左邊，current=current.p.left*/
			RBT_node* current_u = current->parent->rightchild;
			RBT_node* current_p = current->parent;

			if (current_u->color == 0) {/*case1: uncle is red, it can change it to other cases*/ 
			
				current_u->color = 1;/*change uncle color to black*/ 
				current_p->color = 0;/*change parent color to red*/ 
				LeftRotation(current_p);/*左旋*/
				current_u = current_p->rightchild;/*指向uncle*/ /*current = current_p->rightchild*/ 
			}
			
			if (current_u->leftchild->color == 1 && current_u->rightchild->color == 1) {/*case2: if uncle is black and its two children are black*/ 
				current_u->color = 0;/*change color to red*/ 
				current = current_p;/*current指向parent*/
			}
			
			else {
				if (current_u->leftchild->color == 0) {/*case3: if uncle is black and its leftchild is red, rightchild is black*/ 
					
					current_u->leftchild->color = 1;/*change leftchild's color to black*/
					current_u->color = 0;/*change uncle color to red;*/
					RightRotation(current_u);/*右旋*/
					current_u = current->parent->rightchild;/*指向新的parent*/ 
				}
				
				/*case4: uncle is black, and its rightchild is red*/ 
				current_u->color = current_p->color;/*let uncle become parent's color*/ 
				current_p->color = 1;/*parent become black*/
				current_u->rightchild->color = 1;/*uncle rightchild color be black*/
				LeftRotation(current_p);/*左旋*/
				current = root;/*指向root以脫離迴圈*/
			}
		}

		else {/*右邊，current=current.p.right*/
			RBT_node* current_u = current->parent->leftchild;
			RBT_node* current_p = current->parent;
			
			if (current_u->color == 0) {/*case1: uncle is red, it can change it to other cases*/ 

				current_u->color = 1;/*change uncle color to black*/
				current_p->color = 0;/*change parent color to red*/
				RightRotation(current_p);/*右旋*/
				current_u = current_p->leftchild;/*指向uncle*/
			}

			if (current_u->leftchild->color == 1 && current_u->rightchild->color == 1) {/*case2: if uncle is black and its two children are black*/
				current_u->color = 0;/*change color to red*/
				current = current_p;/*current指向parent*/
			}

			else {
				if (current_u->rightchild->color == 0) {/*case3: if uncle is black and its rightchild is red, leftchild is black*/ 

					current_u->rightchild->color = 1;/*change rightchild's color to black*/
					current_u->color = 0;/*change uncle color to red;*/
					LeftRotation(current_u);/*左旋*/
					current_u = current->parent->leftchild;/*指向新的parent*/ 
				}

				/*case4: uncle is black, and its leftchild is red*/ 
				current_u->color = current_p->color;/*let uncle become parent's color*/
				current_p->color = 1;
				current_u->leftchild->color = 1;
				RightRotation(current_p);/*右旋*/
				current = root;/*指向root以脫離迴圈*/
			}
		}
	}
	current->color = 1;/*額外狀況，如果current單純為red或者是root，則變黑*/
}

void RBT::DeleteRBT(int key) {
	RBT_node* remove = search(key);/*要delete的node*/ 

	if (remove == NULL) {/*key不存在*/ 
		cout << "Didn't found the data" << endl;
		return;
	}

	RBT_node* delete_node = 0;/*紀錄要delete的node*/
	RBT_node* delete_node_child = 0;/*要delete的node的child*/ 

	if (remove->leftchild == N || remove->rightchild == N) {/*if it doesn't have two subtrees, then it's itself successor or predecessor, so no change*/
		delete_node = remove;
	}else{
		delete_node = successor(remove);
	}

	if (delete_node->leftchild != N) {/*有一邊的subtree*/ 
		delete_node_child = delete_node->leftchild;
	}else{
		delete_node_child = delete_node->rightchild;
	}

	delete_node_child->parent = delete_node->parent;/*連上delete的node的parent*/ 

	if (delete_node->parent == N) {/*要delete的是root*/ 
		this->root = delete_node_child;/*child become new root*/
	}else if (delete_node->parent->leftchild == delete_node) {/*if delete node is leftchild*/ 
		delete_node->parent->leftchild = delete_node_child;
	}else {/*if delete node is rightchild*/ 
		delete_node->parent->rightchild = delete_node_child;
	}
	
	if (delete_node != remove) {/*if delete_node is successor of remove, then let successor key replace remove_node key*/ 
		remove->key = delete_node->key;
	}

	if (delete_node->color == 1) {
		Delete_Fix(delete_node_child);
	}
}

void RBT::InOrder(RBT_node* current) {
	if (current) {/*不為NULL*/ 
		InOrder(current->leftchild); 
		Output_RBT(current);
		InOrder(current->rightchild);
	}
}

void RBT::Output_RBT(RBT_node* current) {/*輸出用*/ 
	if (current == N) {/*NULL*/ 
		return;
	}
	
	cout << "key: " << current->key << " parent: ";
	out << "key: " << current->key << " parent: ";

	if (current->parent == N) {
		cout << " ";
		out << " ";
	}else {
		cout << current->parent->key;
		out << current->parent->key;
	}

	cout << " color: ";
	out << " color: ";

	if (current->color == 0) {
		cout << "red" << endl;
		out << "red" << endl;
	}else {
		cout << "black" << endl;
		out << "black" << endl;
	}
}

RBT_node* RBT::predecessor(RBT_node* current) {
	while (current->leftchild != N) {
		current = current->leftchild;
	}
	return current;
}

RBT_node* RBT::successor(RBT_node* current) {
	if (current->rightchild != N) {
		return predecessor(current->rightchild);
	}
}

RBT_node* RBT::search(int key) {
	RBT_node* current = root;
	while (current != NULL && key != current->key) {
		if (key < current->key) {/*往左邊找*/ 
			current = current->leftchild;
		}else {/*往右邊找*/ 
			current = current->rightchild;
		}
	}
	return current;
}

void RBT::LeftRotation(RBT_node* current) {
	RBT_node* current_right = current->rightchild;/*紀錄current's rightchild*/ 
	current->rightchild = current_right->leftchild;/*把current_right的left subtree接到current的右邊*/

	if (current_right->leftchild != N) {/*若current_right有left subtree，就把parent設成current*/ 
		current_right->leftchild->parent = current;
	}

	current_right->parent = current->parent;/*把current_right往上提，連接current的parent*/ 

	if (current->parent == N) {/*if current is root*/ 
		root = current_right;/*current_right變成root*/ 
	}else if (current == current->parent->leftchild) {/*current在左邊*/ 
		current->parent->leftchild = current_right;
	}else {/*current在右邊*/ 
		current->parent->rightchild = current_right;
	}
	current_right->leftchild = current;/*current變成current_right的left subtree*/ 
	current->parent = current_right;/*current_right變成current的parent*/ 
}

void RBT::RightRotation(RBT_node* current) {/*跟左旋全部相反*/ 
	RBT_node* current_left = current->leftchild;
	current->leftchild = current_left->rightchild;

	if (current_left->rightchild != N) {
		current_left->rightchild->parent = current;
	}

	current_left->parent = current->parent;

	if (current->parent == N) {
		root = current_left;
	}else if (current->parent->leftchild == current) {
		current->parent->leftchild = current_left;
	}else {
		current->parent->rightchild = current_left;
	}
	current_left->rightchild = current;
	current->parent = current_left;
}

void RBT::InsertRBT(int key) {
	RBT_node* x = root;
	RBT_node* y = N;
	RBT_node* insert_node = new RBT_node(key);/*a new node*/ 

	while (x != N) {/*存在root*/ 
		y = x;/*Record parent*/ 
		if (insert_node->key < x->key) {/*往左邊*/
			x = x->leftchild;
		}else {/*往右邊*/ 
			x = x->rightchild;
		}
	}
	insert_node->parent = y;/*連接parent*/ 

	if (y == N) {
		this->root = insert_node;
	}else if (insert_node->key < y->key) {/*比parent小為leftchild*/ 
		y->leftchild = insert_node;
	}else {/*比parent大為rightchild*/ 
		y->rightchild = insert_node;
	}

	insert_node->leftchild = N;
	insert_node->rightchild = N;
	insert_node->color = 0;/*設成紅色*/ 
	Insert_Fix(insert_node);/*換顏色*/ 
}

void RBT::Insert_Fix(RBT_node* current) {
	while (current->parent->color == 0) {/*if the parent of insert node is red*/

		if (current->parent->parent->leftchild == current->parent) {/*左邊*/
			RBT_node* record_u = current->parent->parent->rightchild;

			if (record_u->color == 0) {/*case 1: record_u is red*/
				current->parent->color = 1;/*change color to black*/
				record_u->color = 1;
				current->parent->parent->color = 0;
				current = current->parent->parent;
			}else {
				if (current == current->parent->leftchild) {/*case2: if current is left child of parent*/
					current->parent->color = 1;
					current->parent->parent->color = 0;
					RightRotation(current->parent->parent);
				}else {/*case3: if current is right child of record_p*/
					current = current->parent;
					LeftRotation(current); // become leftchild case
					current->parent->color = 1;
					current->parent->parent->color = 0;
					RightRotation(current->parent->parent);
				}
			}
		}else {/*右邊*/
			RBT_node* record_u = current->parent->parent->leftchild;

			if (record_u->color == 0) {/*case 1: record_u is red*/
				current->parent->color = 1;
				record_u->color = 1;
				current->parent->parent->color = 0;
				current = current->parent->parent;
			}else {
				if (current == current->parent->leftchild) {/*case2*/ 
					current = current->parent;
					RightRotation(current);
				} 
				current->parent->color = 1;/*case 3*/
				current->parent->parent->color = 0;
				LeftRotation(current->parent->parent);
			}
		}
	}
	root->color = 1;/*root's color must be black*/
}

RBT_node* RBT::getRoot()
{
	return this->root;
}

int main() {
	ifstream inFile;
	inFile.open("input.txt");/*Read the input file*/
	out.open("output.txt");

	if (!inFile)/*If file fails*/ 
	{
		cout << "Read input.txt error" << endl;
		exit(1);
	}

	int test;
	inFile >> test;/*Test number*/

	RBT rbt;
	while (test--) {
		int op;
		inFile >> op;

		if (op == 1) {/*Insert*/

			string s;
			getline(inFile, s);/*'enter'*/
			getline(inFile, s);/*'the string of number'*/
			istringstream ss(s);/*Make string cut space*/

			string str;
			vector<int>key;/*Create vector and empty it*/
			key.clear();

			while (ss >> str) {/*Give one char to str*/
				int num;
				num = atoi(str.c_str());/*Make char become int*/
				rbt.InsertRBT(num);/*insert into 紅黑樹*/
				key.push_back(num);
			}

			cout << "Insert: ";
			out << "Insert: ";
			for (int i = 0; i < key.size(); i++) {
				cout << key[i];
				out << key[i];
				int k = i;

				if (k < key.size() - 1) {/*Touch the end*/
					cout << ", ";
					out << ", ";
				}
			}
			cout << endl;
			out << endl;

			rbt.InOrder(rbt.getRoot());
		}
		else {/*Delete*/
			string s;
			getline(inFile, s); /*'enter'*/
			getline(inFile, s); /*'the string of number'*/
			istringstream ss(s); /*Make string cut space*/

			string str;
			vector<int>key;
			key.clear();
			
			while (ss >> str) {
				int num;
				num = atoi(str.c_str()); /*Make char become int*/
				rbt.DeleteRBT(num);
				key.push_back(num);
			}

			cout << "Delete: ";
			out << "Delete: ";
			for (int i = 0; i < key.size(); i++) {
				cout << key[i];
				out << key[i];
				int k = i;

				if (k < key.size() - 1) {/*Touch the end*/
					cout << ", ";
					out << ", ";
				}
			}
			cout << endl;
			out << endl;
			rbt.InOrder(rbt.getRoot());
		}
	}
	inFile.close();
	out.close();
}
