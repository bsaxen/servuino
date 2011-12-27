void passTime();
void digitalWrite(int pin,int value);
int  digitalRead(int pin);
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;

#include <cstring> // strcpy and strcat prototypes
using std::strcmp;
using std::strcpy;
using std::strcat;

#include <cstdlib> // exit prototype
using std::exit;

#include <sstream>


class String
{
   friend ostream &operator<<( ostream &, const String & );
   friend istream &operator>>( istream &, String & );
public:
   String( const char * = "" ); // conversion/default constructor
   String( const String & ); // copy constructor
   ~String(); // destructor

   const String &operator=( const String & ); // assignment operator
   const String &operator=( string right );
   const String &operator=( const char *right );
   const String &operator+=( const String & ); // concatenation operator
   String operator+( const int number ); // concatenation operator
   String operator+( const char one );
   String operator+( unsigned long number );
   String operator+( String right );
   //String string::operator+( String right );

   bool operator!() const; // is String empty?
   bool operator==( const String & ) const; // test s1 == s2
   bool operator<( const String & ) const; // test s1 < s2

   // test s1 != s2
   bool operator!=( const String &right ) const
   { 
      return !( *this == right ); 
   } // end function operator!=

   // test s1 > s2
   bool operator>( const String &right ) const
   { 
      return right < *this; 
   } // end function operator>
 
   // test s1 <= s2
   bool operator<=( const String &right ) const
   { 
      return !( right < *this ); 
   } // end function operator <=

   // test s1 >= s2
   bool operator>=( const String &right ) const
   { 
      return !( *this < right ); 
   } // end function operator>=

   char &operator[]( int ); // subscript operator (modifiable lvalue)
   char operator[]( int ) const; // subscript operator (rvalue)
   String operator()( int, int = 0 ) const; // return a substring
   int length() const; // return string length
   //private:
   int slength; // string length (not counting null terminator)
   char *sPtr; // pointer to start of pointer-based string

   void setString( const char * ); // utility function

   String replace(string, string );
   void charAt(int pos); 
   void compareTo(); 
   void concat(); 
   void endsWith(); 
   void equals(); 
   void equalsIgnoreCase(); 
   void getBytes(); 
   void indexOf(); 
   void lastIndexOf(); 
   void length(); 
   String replace(char x,char y); 
   String replace(const char *x,const char *y); 
   void setCharAt(); 
   void startsWith(); 
   void substring(); 
   void toCharArray(); 
   void toLowerCase(); 
   void toUpperCase(); 
   void trim();
 
}; // end class String
