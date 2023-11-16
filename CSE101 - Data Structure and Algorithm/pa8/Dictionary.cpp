//-----------------------------------------------------------------------------
// Brian Camilo
// 2023 Winter CSE101 
// Dictionary.cpp
// Implementation file for Dictionary ADT based on a Binary Search Tree.
//-----------------------------------------------------------------------------
#include<iostream>
#include<string>
#include"Dictionary.h"

#define RED 1
#define BLACK 2


// Helper Functions (Optional) ---------------------------------------------
// pseudocodes from BST-RBT Algorithm

// inOrderString() -> pseudocode InOrderOrderTreeWalk
void Dictionary::inOrderString(std::string& s, Node* R) const{
	if (R != nil){
		inOrderString(s, R->left);
		s += R->key;
		s += " : ";
		s += std::to_string(R->val);
		s += "\n";
		inOrderString(s, R->right);
	}
}

// preOrderString() -> pseudocode PreOrderTreeWalk
void Dictionary::preOrderString(std::string& s, Node* R) const{
	if (R != nil){
		s += R->key;
		s += "\n";
		preOrderString(s, R->left);
		preOrderString(s, R->right);
	}
}

// preOrderCopy() -> pseudocode PreOrderTreeWalk
void Dictionary::preOrderCopy(Node* R, Node* N){
	if (N != R){
		setValue(R->key, R->val);
		preOrderCopy(R->left, N);
		preOrderCopy(R->right, N);
	}
}

// postOrderDelete() -> pseudocode PostOrderTreeWalk
void Dictionary::postOrderDelete(Node* R){
	if(R != nil) {
		postOrderDelete(R->left);
		postOrderDelete(R->right);
		delete R;
    }
}

// search() -> pseudocode TreeSearch
Dictionary::Node* Dictionary::search(Node* R, keyType k) const{
	if( R == nil || k == R->key){
		return R;
	} else if (k < R->key){
		return search(R->left, k);
	} else // k > R.key
		return search(R->right, k);
}

// findMin() -> pseudocode TreeMinimum
Dictionary::Node* Dictionary::findMin(Node* R){
	if (R != nil){
		while (R->left != nil){
		R = R->left;
		}
	}
	return R;
}

// findMax() ->  pseudocode TreeMaximum
Dictionary::Node* Dictionary::findMax(Node* R){
	if (R != nil){
		while (R->right != nil){
		R = R->right;
		}
	}
	return R;
}

// findNext() -> pseudocode TreeSuccessor
Dictionary::Node* Dictionary::findNext(Node* N){
	if (N->right != nil){
		return findMin(N->right);
	}
	Node* y = N->parent;
	while (y != nil && N == y->right){
		N = y;
		y = y->parent;
	}
	return y;
}

// findPrev() -> pseudocode TreeSuccessor/TreePredecessor
Dictionary::Node* Dictionary::findPrev(Node* N){
	if (N->left != nil){
		return findMax(N->left);
	}
	Node* y = N->parent;
	while (y != nil && N == y->left){
		N = y;
		y = y->parent;
	}
	return y;
}

// RBT Helper Functions (Optional) -----------------------------------------

// LeftRotate()	-> pseudocode LeftRotate
void Dictionary::LeftRotate(Node* N){
	// set y
	Node* y = N->right;

	// turn y's left subtree into N's right subtree
	N->right = y->left;
	if(y->left != nil){	// not necessary if using sentinal nil node
		y->left->parent = N;
	}	

	// link y's parent to N
	y->parent = N->parent;
	if (N->parent == nil){
		root = y;
	} else if(N == N->parent->left){
		N->parent->left = y;
	} else{
		N->parent->right = y;
	} 

	// put N on y's left
	y->left = N;
	N->parent = y;

}

// RightRotate() -> pseudocode RotateRight
void Dictionary::RightRotate(Node* N){
	// set y
	Node* y = N->left;

	// turn y's right subtree into N's left subtree
	N->left = y->right;
	if(y->right != nil){	// not necessary if using sentinal nil node
		y->right->parent = N;
	}  
		
	// link y's parent to x
	y->parent = N->parent; 
	if(N->parent == nil){
		root = y;
	} else if (N == N->parent->right){
			N->parent->right = y;
	} else{
			N->parent->left = y;
	} 
			
	// put x on y's right
	y->right = N;
	N->parent = y;
}

// RB_InsertFixUP() -> pseudocode RB_InsertFixUp
void Dictionary::RB_InsertFixUp(Node* N){
	Node* y;
	while (N->parent->color == RED){
		if (N->parent == N->parent->parent->left){
			y = N->parent->parent->right;
			if(y->color == RED){
				N->parent->color = BLACK;           // case 1
				y->color = BLACK;                   // case 1
				N->parent->parent->color = RED;     // case 1
				N = N->parent->parent;             	// case 1
			} else {
			if(N == N->parent->right){
				N = N->parent;                    	// case 2
				LeftRotate(N);                 		// case 2
			}
			N->parent->color = BLACK;              	// case 3
			N->parent->parent->color = RED;        	// case 3
			RightRotate(N->parent->parent);    		// case 3
			}
		}
		else{
			y = N->parent->parent->left;
			if (y->color == RED){
				N->parent->color = BLACK;           // case 4
				y->color = BLACK;                 	// case 4
				N->parent->parent->color = RED;     // case 4
				N = N->parent->parent;              // case 4
			} else {
			if (N == N->parent->left){
				N = N->parent;                     	// case 5
				RightRotate(N);               		// case 5
			}
			N->parent->color = BLACK;         		// case 6
			N->parent->parent->color = RED;        	// case 6
			LeftRotate(N->parent->parent);      	// case 6
			} 
		} 	
	}
	root->color = BLACK;
}

// RB_Transplant() -> pseudocode RB_Transplant
void Dictionary::RB_Transplant(Node* u, Node* v){
	if(u->parent == nil){
		root = v;
	}else if (u == u->parent->left){
		u->parent->left = v;
	}else{
 		u->parent->right = v;
	} 
		v->parent = u->parent;
}

// RB_DeleteFixUp() -> pseudocode RB_DeleteFixUp
void Dictionary::RB_DeleteFixUp(Node* N){
	Node* w;
	while (N != root and N->color == BLACK){
		if (N == N->parent->left){
			w = N->parent->right;
			if(w->color == RED){
				w->color = BLACK;                       // case 1
				N->parent->color = RED;                 // case 1
				LeftRotate(N->parent);                	// case 1
				w = N->parent->right;                   // case 1
			}
			if (w->left->color == BLACK and w->right->color == BLACK){
				w->color = RED;                         // case 2
				N = N->parent;                          // case 2
			}
			else{
				if (w->right->color == BLACK){
					w->left->color = BLACK;           	// case 3
					w->color = RED;                     // case 3
					RightRotate(w);                 	// case 3
					w = N->parent->right;               // case 3
				}
				w->color = N->parent->color;            // case 4
				N->parent->color = BLACK ;              // case 4
				w->right->color = BLACK;                // case 4
				LeftRotate(N->parent);                	// case 4
				N = root;                            	// case 4
			} 
		}		
		else {
			w = N->parent->left;
			if (w->color == RED){
				w->color = BLACK;                       // case 5
				N->parent->color = RED;               	// case 5
				RightRotate(N->parent);               	// case 5
				w = N->parent->left;                    // case 5
			}
			if (w->right->color == BLACK and w->left->color == BLACK){
				w->color = RED;                     	// case 6
				N = N->parent;                          // case 6
			}	
			else{
				if (w->left->color == BLACK){
					w->right->color = BLACK;            // case 7
					w->color = RED;                    	// case 7
					LeftRotate(w);                    	// case 7
					w = N->parent->left;                // case 7
				}
				w->color = N->parent->color;            // case 8
				N->parent->color = BLACK;              	// case 8
				w->left->color = BLACK;                 // case 8
				RightRotate(N->parent);              	// case 8
				N = root;                            	// case 8
			}
				
		}	
	}
	N->color = BLACK;
}

// RB_Delete() -> pseudocode RB_Delete
void Dictionary::RB_Delete(Node* N){
	Node*y = N;
	Node* x;
	y = N;

	int y_original_color = y->color;
	if (N->left == nil){
		x = N->right;
		RB_Transplant(N, N->right);
	}else if (N->right == nil){
		x = N->left;
		RB_Transplant(N, N->left);
	}else{
		y = findMin(N->right);
		y_original_color = y->color;
		x = y->right;
		if (y->parent == N){
			x->parent = y;
		}else{
			RB_Transplant(y, y->right);
			y->right = N->right;
			y->right->parent = y;
		} 
		RB_Transplant(N, y);
		y->left = N->left;
		y->left->parent = y;
		y->color = N->color;
	} 	
	if (y_original_color == BLACK){
		RB_DeleteFixUp(x);
	}	
}

Dictionary::Node::Node(keyType k, valType v) {
   key = k;
   val = v;
   parent = nullptr;
   left = nullptr;
   right = nullptr;
   color = BLACK;
}

// Class Constructors & Destructors ----------------------------------------

// Creates new Dictionary in the empty state. 
Dictionary::Dictionary(){
	nil = new Node(" ", 0);
	root = nil;
	current = nil;
	num_pairs = 0;
}

// Copy constructor.
Dictionary::Dictionary(const Dictionary& D){
	nil = new Node(" ", 0);
	root = nil;
	current = nil;
	num_pairs = 0;
	preOrderCopy(D.root, D.nil);
}

// Destructor
Dictionary::~Dictionary(){
	postOrderDelete(root);
	delete nil;
}


// Access functions --------------------------------------------------------

// size()
int Dictionary::size() const{
	return num_pairs;
}

// contains()
bool Dictionary::contains(keyType k) const{
	if (search(root, k) != nil){
		return true;
	}
	return false;
}

// getValue()
valType& Dictionary::getValue(keyType k) const{
	Node* s = search(root, k);
	if (s == nil){
		throw std::logic_error("Dictionary: getValue(): key \"" + k + "\" does not exist");
   	}
   return(s->val);
}

// hasCurrent()
bool Dictionary::hasCurrent() const{
	if (current != nil){
		return true;
	} else{
		return false;
	}
}

// currentKey()
keyType Dictionary::currentKey() const{
	if (!hasCurrent()) {
    	throw std::logic_error("Dictionary: currentKey(): current undefined");
	} 
	return current->key;
}

// currentVal()
// Returns a reference to the current value.
// Pre: hasCurrent()
valType& Dictionary::currentVal() const{
	if (!hasCurrent()) {
    	throw std::logic_error("Dictionary: currentVal(): current undefined");
	} 
	return current->val;
}


// Manipulation procedures -------------------------------------------------

// clear()
void Dictionary::clear(){
	postOrderDelete(root);
	root = nil;
	current = nil;
	num_pairs = 0;
}

// setValue() -> pseudocode TreeInsert
void Dictionary::setValue(keyType k, valType v){
	Node* z = new Node(k, v);
	z->left = nil;
	z->right = nil;

	Node* y = nil;
	Node* x = root;

	while (x != nil){
		y = x;
		if(z->key < x->key){
			x = x->left;
		} else if (x->key == k){
			x->val = v;
            delete z;
            return;
		} else{
			x = x->right;
		}
	}
	z->parent = y;
		if(y == nil){
			root = z;	// tree was empty
		} else if( z->key < y->key){
			y->left = z;
		} else {
			y->right = z;
	}
	z->left = nil;
   	z->right = nil;  
  	z->color = RED;
   	RB_InsertFixUp(z);
	num_pairs++;
}

// remove() -> pseudocode from delete
void Dictionary::remove(keyType k){
	Node* z = search(root, k);
	
	if(contains(k) == false){
		throw std::logic_error("Dictionary: remove(): key \"" + k + "\" does not exist");
	}
	if(current == z){
		current = nil;
	}
	RB_Delete(z);
	delete z;
	num_pairs--;
}

// begin()
void Dictionary::begin(){
	if(num_pairs > 0){
		current = findMin(root);
	}
}

// end()
void Dictionary::end(){
	if(num_pairs > 0){
		current = findMax(root);
	}
}

// next()
void Dictionary::next(){
	if(!hasCurrent()) {
		throw std::logic_error("Dictionary: next(): current undefined");
	}
	current = findNext(current);
}

// prev()
void Dictionary::prev(){
	if(!hasCurrent()) {
		throw std::logic_error("Dictionary: prev(): current undefined");
	}
	current = findPrev(current);
}


// Other Functions ---------------------------------------------------------

// to_string()
std::string Dictionary::to_string() const{
	std::string s = "";
	inOrderString(s, root);
	return s;
}

// pre_string()
std::string Dictionary::pre_string() const{
	std::string s = "";
	preOrderString(s, root);
	return s;
}

// equals()
bool Dictionary::equals(const Dictionary& D) const{
	std::string x,y;
	x = this->to_string();
	y = D.to_string();

	if (this->num_pairs != D.num_pairs) {
         return false;
    }
	if(x == y){
		return true;
	}
	return false;
}


// Overloaded Operators ----------------------------------------------------

// operator<<()
std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
	return stream << D.Dictionary::to_string();
}

// operator==()
bool operator==( const Dictionary& A, const Dictionary& B ){
	return A.Dictionary::equals(B);
}

// operator=()
Dictionary& Dictionary::operator=( const Dictionary& D ){
	if (this != &D){
		Dictionary temp = D;
		std::swap(this->nil, temp.nil);
		std::swap(this->root, temp.root);
		std::swap(this->current, temp.current);
		std::swap(this->num_pairs, temp.num_pairs);
	}
	return *this;
}