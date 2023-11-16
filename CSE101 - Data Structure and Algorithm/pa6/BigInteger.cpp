//-----------------------------------------------------------------------------
// Brian Camilo
// 2023 Winter CSE101 pa6
// BigInteger.cpp
// Implementation file for the BigInteger ADT
//-----------------------------------------------------------------------------

#include<iostream>
#include<string>
#include<stdexcept>
#include"List.h"
#include"BigInteger.h"

int power = 9;	
long base = 1000000000;

// Class Constructors & Destructors ----------------------------------------

// BigInteger()
// Constructor that creates a new BigInteger in the zero state: 
// signum=0, digits=().
BigInteger::BigInteger(){
	signum = 0;
	digits.clear();
}

// BigInteger()
// Constructor that creates a new BigInteger from the string s.
// Pre: s is a non-empty string consisting of (at least one) base 10 digit
// {0,1,2,3,4,5,6,7,8,9}, and an optional sign {+,-} prefix.
BigInteger::BigInteger(std::string s){
	if(s.length() == 0){
		throw std::invalid_argument("BigInteger: Constructor: empty string\n");
	}

	if(s[0] == '-'){
		s = s.substr(1, s.length());
		signum = -1;
	} else if(s[0] == '+'){
		s = s.substr(1, s.length());
		signum = 1;
	} else {
		signum = 1;
	}

	for(int i = s.length() - power; i > 0; i -= power){
		if(i == 0){
			digits.insertAfter(std::stol(s.substr(i, power)));
			break;
		} else if(i < 0){
			digits.insertAfter(std::stol(s.substr(0, i + power)));
			break;
		} else {
			digits.insertAfter(std::stol(s.substr(i, power)));
		}
	} 

	digits.moveFront();
    while(digits.peekNext() == 0 && digits.length() != 0){
        digits.eraseAfter();
    }
}

// BigInteger()
// Constructor that creates a copy of N.
BigInteger::BigInteger(const BigInteger& N){
	this->signum = N.signum;
	this->digits = N.digits;
}

// Optional Destuctor
// ~BigInteger()
// ~BigInteger();

// Access functions --------------------------------------------------------

// sign()
// Returns -1, 1 or 0 according to whether this BigInteger is negative, 
// positive or 0, respectively.
int BigInteger::sign() const{
	return signum;
}

// compare()
// Returns -1, 1 or 0 according to whether this BigInteger is less than N,
// greater than N or equal to N, respectively.
int BigInteger::compare(const BigInteger& N) const{
	if(this->signum < N.signum){
		return -1;
	} else if(this->signum > N.signum){
		return 1;
	} else if(this->signum == 0 && N.signum == 0){
		return 0;
	}

	if(this->signum == 1 && N.signum == 1){
		return 1;
	} else{
		return -1;}

	if(this->digits.length() != N.digits.length()){
		return 1;
	} else{
		return -1;}

	if(this->digits.length() > N.digits.length()){
		return 1;
	} else{
		return -1;}
	
	List list_this = this->digits;
	List list_n = N.digits;
	list_this.moveFront();
	list_n.moveFront();
	for (; list_this.position() < list_this.length(); list_this.moveNext(), list_n.moveNext()){
		if (list_this.peekNext() < list_n.peekNext()){
			return -1;
		}  else if (list_this.peekNext() > list_n.peekNext()){
			return 1;
		}
	}
	return 0;
}

// Manipulation procedures -------------------------------------------------

// makeZero()
// Re-sets this BigInteger to the zero state.
void BigInteger::makeZero(){
	signum = 0;
	digits.clear();
}

// negate()
// If this BigInteger is zero, does nothing, otherwise reverses the sign of 
// this BigInteger positive <--> negative. 
void BigInteger::negate(){
	if(signum != 0){
		 signum *= -1;
	}
}

// Helper Functions -------------------------------------------------------

// negateList()
// Changes the sign of each integer in List L. Used by sub().
void negateList(List& L){
	L.moveFront();
    while(L.position() < L.length()){
        L.setAfter(L.peekNext() * -1);
        L.moveNext();
	}
}

// sumList()
// Overwrites the state of S with A + sgn*B (considered as vectors).
// Used by both sum() and sub().
void sumList(List& S, List A, List B, int sgn){
	S.clear();
	S.moveBack();
	A.moveBack();
	B.moveBack();

	while(A.position() != 0 && B.position() != 0){
		S.insertAfter(A.peekPrev() + (B.peekPrev() * sgn));
		A.movePrev();
		B.movePrev();
	}

	while(A.position() != 0){
		S.insertAfter(A.peekPrev());
		A.movePrev();
	}

	while(B.position() != 0){
		S.insertAfter(B.peekPrev());
		B.movePrev();
	}

	S.front();
	while (S.front() == 0 && S.length() > 1) {
        S.eraseAfter();
    }
}

// normalizeList()
// Performs carries from right to left (least to most significant
// digits), then returns the sign of the resulting integer. Used
// by add(), sub() and mult().
int normalizeList(List& L){
 	if(L.length() == 0){
    	return 0;
   	}

   	int sign = 1;

    if(L.front() < 0){
        sign = -1;
		negateList(L);
    }

    L.moveBack();
    int carry = 0;
	ListElement num = 0;
    while(L.position() > 0){
        num = L.peekPrev();
        if(num >= 0){
			num += carry;
            carry = 0;
            if(num >= base){
                carry = num / base;
                num %= base;
            }
            L.setBefore(num);
        } else {
            num += (base + carry);
            L.setBefore(num);
            carry = -1;
        }
        L.movePrev();
    }

    if(carry != 0){
        L.moveFront();
        L.insertAfter(carry);
    }

    return sign;
}

// shiftList()
// Prepends p zero digits to L, multiplying L by base^p. Used by mult().
void shiftList(List& L, int p){
	L.moveBack();
   	for(int i = 0; i < p; i++) {
    	L.insertAfter(0);
  	}
}

// scalarMultList()
// Multiplies L (considered as a vector) by m. Used by mult().
void scalarMultList(List& L, ListElement m){
	L.moveFront();
	while(L.position() < L.length()){
    	L.setAfter(m * L.peekNext());
        L.moveNext();
    }
}

// BigInteger Arithmetic operations ----------------------------------------

// add()
// Returns a BigInteger representing the sum of this and N.
BigInteger BigInteger::add(const BigInteger& N) const{
	BigInteger sum;
    List list_sum;
    List list_this_digits = this->digits;
    List list_n_digits = N.digits;

    if(this->signum == -1 && N.signum== -1){
        negateList(list_n_digits);
		negateList(list_this_digits);
        sumList(list_sum, list_this_digits, list_n_digits, 1);
        sum.digits = list_sum;
		sum.signum = normalizeList(list_sum);
    } else if(this->signum == 1 && N.signum == 1){
		sumList(list_sum, list_this_digits, list_n_digits, 1);
        sum.digits = list_sum;
		sum.signum = normalizeList(list_sum);
    } else {
        if(this->signum < 0) {
            negateList(list_this_digits);
        }
        if (N.signum < 0) {
            negateList(list_n_digits);
        }
        sumList(list_sum, list_this_digits, list_n_digits, 1);
        sum.digits = list_sum;
		sum.signum = normalizeList(list_sum);
    }

    return sum;
}

// sub()
// Returns a BigInteger representing the difference of this and N.
BigInteger BigInteger::sub(const BigInteger& N) const{
	BigInteger difference;
    BigInteger n = N;
	negateList(n.digits);
    difference = this->add(n);
    return difference;
}

// mult()
// Returns a BigInteger representing the product of this and N. 
BigInteger BigInteger::mult(const BigInteger& N) const{
	BigInteger product;
    if(this->signum == 0 || N.signum == 0){
        return product;
    }

    List list_product;
    List list_this_digits;
    List list_n_digits = N.digits;

    list_n_digits.moveBack();
	list_product.insertAfter(0);
    int shift = 0;

    while(list_n_digits.position() > 0){
        list_this_digits = this->digits;
        scalarMultList(list_this_digits, list_n_digits.peekPrev());
        shiftList(list_this_digits, shift);
        
		List list_product_clone = list_product;
        sumList(list_product, list_product_clone, list_this_digits, 1);
        normalizeList(list_product);

        list_n_digits.movePrev();
        shift += 1;
    }

    product.digits = list_product;
    if(this->signum != N.signum){
        product.signum = -1;
    } else {
        product.signum = 1;
    }

    return product;
}

// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this BigInteger consisting of its
// base 10 digits. If this BigInteger is negative, the returned string 
// will begin with a negative sign '-'. If this BigInteger is zero, the
// returned string will consist of the character '0' only.
std::string BigInteger::to_string(){
	std::string str = "";
	if(this->signum == 0){
		str += "0";
		return str;
	}
	
	if(this->signum == -1){
		str += "-";
	}
	str += this->digits.to_string();
	return str;
}

// Overriden Operators -----------------------------------------------------

// operator<<()
// Inserts string representation of N into stream.
std::ostream& operator<<( std::ostream& stream, BigInteger N ) {
    return stream << N.BigInteger::to_string();
}

// operator==()
// Returns true if and only if A equals B. 
bool operator==( const BigInteger& A, const BigInteger& B ) {
    if (A.compare(B) == 0) {
        return true;
    }
    else {
        return false;
    }
}

// operator<()
// Returns true if and only if A is less than B. 
bool operator<( const BigInteger& A, const BigInteger& B ) {
    if (A.compare(B) == -1) {
        return true;
    }
    else {
        return false;
    }    
}

// operator<=()
// Returns true if and only if A is less than or equal to B. 
bool operator<=( const BigInteger& A, const BigInteger& B ) {
    if (A.compare(B) == -1 || A.compare(B) == 0) {
        return true;
    }
    else {
        return false;
    }
}

// operator>()
// Returns true if and only if A is greater than B. 
bool operator>( const BigInteger& A, const BigInteger& B ) {
    if (A.compare(B) == 1) {
        return true;
    }
    else {
        return false;
    }
}

// operator>=()
// Returns true if and only if A is greater than or equal to B. 
bool operator>=( const BigInteger& A, const BigInteger& B ) {
    if (A.compare(B) == 1 || A.compare(B) == 0) {
        return true;
    }
    else {
        return false;
    } 
}

// operator+()
// Returns the sum A+B. 
BigInteger operator+( const BigInteger& A, const BigInteger& B ) {
    return A.add(B);

}

// operator+=()
// Overwrites A with the sum A+B. 
BigInteger operator+=( BigInteger& A, const BigInteger& B ) {
    A = A.add(B);
    return A;
}

// operator-()
// Returns the difference A-B. 
BigInteger operator-( const BigInteger& A, const BigInteger& B ) {
    return A.sub(B);
}

// operator-=()
// Overwrites A with the difference A-B. 
BigInteger operator-=( BigInteger& A, const BigInteger& B ) {
    A = A.sub(B);
    return A;
}

// operator*()
// Returns the product A*B. 
BigInteger operator*( const BigInteger& A, const BigInteger& B ) {
    return A.mult(B);
}

// operator*=()
// Overwrites A with the product A*B. 
BigInteger operator*=( BigInteger& A, const BigInteger& B ) {
    A = A.mult(B);
    return A;
}


