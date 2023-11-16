/************************************
*  List.cpp
*  Brian Camilo
*  2023 Winter CSE101 pa6
*  Implementation file for List ADT
************************************/
#include<iostream>
#include<string>
#include<stdexcept>

#include "List.h"
using namespace std;
// Private Constructor --------------------------------------------------------

// List constructor
List::Node::Node(ListElement x){
	data = x;
	next = nullptr;
	prev = nullptr;
}

// Class Constructors & Destructors -------------------------------------------

// Creates a new List in the empty state.
List::List(){
	// make this an empty Queue
	frontDummy = new Node(0);
	backDummy = new Node(0);
	frontDummy->next = backDummy;
	backDummy->prev = frontDummy;
	afterCursor = backDummy;
	beforeCursor = frontDummy;
	pos_cursor = 0;
	num_elements = 0;
}

List::List(const List& L){
	// make this an empty Queue
	frontDummy = new Node(0);
	backDummy = new Node(0);
	frontDummy->next = backDummy;
	backDummy->prev = frontDummy;
	afterCursor = backDummy;
	beforeCursor = frontDummy;
	pos_cursor = 0;
	num_elements = 0;
	
	this->moveFront();
	
	// load elements of L into this List
	Node* N = L.frontDummy->next;
	while(N != L.backDummy){
		this->insertBefore(N->data);
		N = N->next;
	}
}

// Destructor
List::~List(){
	Node* N = frontDummy;
	while( N != backDummy){
		Node* temp = N->next;
		delete N;
		N = temp;
	}
	delete backDummy;
}

// Access functions --------------------------------------------------------

// Returns the length of this List.
int List::length() const{
	return num_elements;	
}

// front() Returns the front element in this List.
// pre: length()>0
ListElement List::front() const{
	if(length() <= 0){
		throw std::length_error("List: front(): empty List");
	}
	return frontDummy->next->data;
}

// back() Returns the back element in this List.
// pre: length()>0
ListElement List::back() const{
	if(length() <= 0){
		throw std::length_error("List: back(): empty List");
	}
	return backDummy->prev->data;
}

// position() Returns the position of cursor in this 
// List: 0 <= position() <= length().
int List::position() const{
	return pos_cursor;
}

// peekNext() Returns the element after the cursor.
// pre: position()<length()
ListElement List::peekNext() const{
	if(position() >= length()){
		throw std::range_error("List: peekNext(): cursor at back");
	}
	return afterCursor->data;
}

// peekPrev() Returns the element before the cursor.
// pre: position()>0
ListElement List::peekPrev() const{
	if(position() <= 0){
		throw std::range_error("List: peekPrev(): cursor at front");
	}
	return beforeCursor->data;
}

// Manipulation procedures -------------------------------------------------

// Deletes all elements in this List, setting it to the empty state.
void List::clear(){
	moveFront();
	while(length() > 0){
		eraseAfter();
	}
}

// Moves cursor to position 0 in this List.
void List::moveFront(){
	pos_cursor = 0;
	beforeCursor = frontDummy;
	afterCursor = frontDummy->next;
}
  
// Moves cursor to position length() in this List.
void List::moveBack(){
	pos_cursor = num_elements;
	afterCursor = backDummy;
	beforeCursor = backDummy->prev;
}

// Advances cursor to next higher position. Returns the List element thatwas passed over. 
// pre: position()<length() 
ListElement List::moveNext(){
	if(position() >= length()){
		throw std::range_error("List: moveNext(): cursor at back");
	} else {
		pos_cursor++;
		beforeCursor = afterCursor;
		afterCursor = afterCursor->next;
		return beforeCursor->data;
	}
}

// Advances cursor to next lower position. Returns the List element that was passed over. 
// pre: position()>0
ListElement List::movePrev(){
	if (position() <= 0){
		throw std::range_error("List: movePrev(): cursor at front");
	} else {
		pos_cursor--;
		afterCursor = beforeCursor;
		beforeCursor = beforeCursor->prev;
		return afterCursor->data;
	}
}

// Inserts x after cursor.
void List::insertAfter(ListElement x){
	Node* node = new Node(x);
	afterCursor->prev = beforeCursor->next = node;
	node->prev = beforeCursor;
	node->next = afterCursor;
	afterCursor = node;
	num_elements++;
}

// Inserts x before cursor.
void List::insertBefore(ListElement x){
	Node* node = new Node(x);
	afterCursor->prev = beforeCursor->next = node;
	node->prev = beforeCursor;
	node->next = afterCursor;
	beforeCursor = node;
	num_elements++;
	pos_cursor++;
}

// Overwrites the List element after the cursor with x.
// pre: position()<length()
void List::setAfter(ListElement x){
	if(position() >= length()){
		throw std::range_error("List: setAfter(): cursor at back");
	}
	afterCursor->data = x;
}

// Overwrites the List element before the cursor with x.
// pre: position()>0
void List::setBefore(ListElement x){
	if(position() <= 0){
		throw std::range_error("List: setBefore(): cursor at front");
	}	
	beforeCursor->data = x;
}

// Deletes element after cursor.
// pre: position()<length()
void List::eraseAfter(){
	if(position() >= length()){
		throw std::range_error("List: eraseAfter(): cursor at back");
	}else{
		Node* node = afterCursor;
		beforeCursor->next = afterCursor->next;
        afterCursor->next->prev = beforeCursor;
        afterCursor = afterCursor->next;
		delete node;
		num_elements--;
	}
}

// Deletes element before cursor.
// pre: position()>0
void List::eraseBefore(){
	if(position() <= 0){
		throw std::range_error("List: setBefore(): cursor at front");
	} else{
		Node* node = beforeCursor;
		afterCursor->prev = beforeCursor->prev;
        beforeCursor->prev->next = afterCursor;
        beforeCursor = beforeCursor->prev;
		delete node;
		pos_cursor--;
		num_elements--;
	}
}

// find if x exist ->look at this later
int List::findNext(ListElement x){
	while(position() < length()){
		if(moveNext() == x){
			return position();
		}
	}
	return -1;
}

int List::findPrev(ListElement x){
	while (position() > 0){
		if(movePrev() == x){
			return position();
		}
	}
	return -1;
}

// Removes any repeated elements in this List, leaving only unique elements.
void List::cleanup(){
	Node *node_A, *node_B;
	int i = 0, j = 0;

	for(node_A = frontDummy->next; node_A != backDummy; node_A = node_A->next, i++){
		j = i + 1;
		for (node_B = node_A->next; node_B != backDummy; node_B = node_B->next){
			if(j >= position() && node_B->data == node_A->data && node_B != afterCursor){
				node_B->prev->next = node_B->next;
				node_B->next->prev = node_B->prev;
				delete node_B;
				node_B = node_B->prev;
				num_elements--;
            } else if(j < position() && node_B->data == node_A->data && node_B != beforeCursor){
				node_B->prev->next = node_B->next;
				node_B->next->prev = node_B->prev;
				delete node_B;
				node_B = node_B->prev;
				num_elements--;
				pos_cursor--;
			} else if (node_A->data == node_B->data && node_B == afterCursor) {
                node_B = node_B->prev;
                eraseAfter();
            } else if (node_A->data == node_B->data && node_B == beforeCursor) {
                node_B = node_B->prev;
                eraseBefore();
            } else {
				j++;
				continue;
			}
		} 
	}
}

// Returns a new List consisting of the elements of this List, followed by the elements of L. 
List List::concat(const List& L) const{
	List list;
	Node* node = this->frontDummy->next;
	Node* node_L = L.frontDummy->next;
	
	int counter = 0;
	while(counter < num_elements){
		list.insertBefore(node->data);
		node = node->next;
		counter++;
	}
	
	counter = 0;
	while(counter < L.length()){
		list.insertBefore(node_L->data);
		node_L = node_L->next;
		counter++;
	}
	
	list.moveFront();

	return list;
}

// Returns a string representation of this List consisting of a comma 
// separated sequence of elements, surrounded by parentheses.
std::string List::to_string() const{
	Node* N = nullptr;
	std::string s = "(";

   	for(N = frontDummy->next; N != backDummy->prev; N = N->next){
      s += std::to_string(N->data)+", ";
  	}
   	s += std::to_string(N->data)+")";
   
   	return s;
}

// Returns true if and only if this List is the same integer sequence as R.
bool List::equals(const List& R) const{
	bool eq = false;
	Node* N = nullptr;
	Node* M = nullptr;

	eq = ( this->num_elements == R.num_elements );
	N = this->frontDummy->next;
	M = R.frontDummy->next;
	while( eq && N != backDummy){
		eq = (N->data == M->data);
		N = N->next;
		M = M->next;
	}
	return eq;
}

// Overriden Operators -----------------------------------------------------

   
// Inserts string representation of L into stream.
std::ostream& operator<<( std::ostream& stream, const List& L ){
	return stream << L.List::to_string();
}

// Returns true if and only if A is the same integer sequence as B. The 
// cursors in both Lists are unchanged.
bool operator==( const List& A, const List& B ){
	return A.List::equals(B);
}

// operator=()
// Overwrites the state of this List with state of L.
List& List::operator=( const List& L ){
	 if( this != &L ){ // not self assignment
      // make a copy of Q
      List temp = L;

      // then swap the copy's fields with fields of this
      std::swap(frontDummy, temp.frontDummy);
      std::swap(backDummy, temp.backDummy);
      std::swap(beforeCursor, temp.beforeCursor);
	  std::swap(afterCursor, temp.afterCursor);
	  std::swap(pos_cursor, temp.pos_cursor);
	  std::swap(num_elements, temp.num_elements);
   }

   // return this with the new data installed
   return *this;

   // the copy, if there is one, is deleted upon return
}