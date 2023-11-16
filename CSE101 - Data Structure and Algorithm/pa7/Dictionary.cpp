//-----------------------------------------------------------------------------
// Brian Camilo
// 2023 Winter CSE101 
// Dictionary.cpp
// Implementation file for Dictionary ADT based on a Binary Search Tree.
//-----------------------------------------------------------------------------
#include<iostream>
#include<string>
#include "Dictionary.h"

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

// added helper function -> pseudocode transplant
void Dictionary::transplant(Node* U, Node*V){
	if (U->parent == nil){
		root = V;
	} else if(U == U->parent->left) {
		U->parent->left = V;
	} else {
		U->parent->right = V;
	}
	if (V != nil){
		V->parent =  U->parent;
	}
}

Dictionary::Node::Node(keyType k, valType v) {
   key = k;
   val = v;
   parent = nullptr;
   left = nullptr;
   right = nullptr;
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
	num_pairs++;
}

// remove() -> pseudocode form delete
void Dictionary::remove(keyType k){
	Node* z = search(root, k);
	
	if(contains(k) == false){
		throw std::logic_error("Dictionary: remove(): key \"" + k + "\" does not exist");
	}

	if(z->left == nil){				// case 1 or case 2.1 (right only)
		transplant(z, z->right);
	} else if (z->right == nil){	// case 2.2 (left only)
		transplant(z, z->left); 
	} else{							// case 3
		Node* y = findMin(z->right);
		if (y->parent != z){
			transplant(y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		transplant(z, y);
		y->left = z->left;
		y->left->parent = y;
	}

	if(root == z){
		root = nil;
	}
	if(current == z){
		current = nil;
	}
	
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
// If the current iterator is not at the last pair, advances current 
// to the next pair (as defined by the order operator < on keys). If 
// the current iterator is at the last pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::next(){
	if(!hasCurrent()) {
		throw std::logic_error("Dictionary: next(): current not defined");
	}
	current = findNext(current);
}

// prev()
// If the current iterator is not at the first pair, moves current to  
// the previous pair (as defined by the order operator < on keys). If 
// the current iterator is at the first pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::prev(){
	if(!hasCurrent()) {
		throw std::logic_error("Dictionary: prev(): current not defined");
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
// Inserts string representation of Dictionary D into stream, as defined by
// member function to_string().
std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
	return stream << D.Dictionary::to_string();
}

// operator==()
// Returns true if and only if Dictionary A equals Dictionary B, as defined
// by member function equals(). 
bool operator==( const Dictionary& A, const Dictionary& B ){
	return A.Dictionary::equals(B);
}

// operator=()
// Overwrites the state of this Dictionary with state of D, and returns a
// reference to this Dictionary.
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