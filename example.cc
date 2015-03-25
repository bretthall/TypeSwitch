//          Copyright Brett Hall 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//Example program for the TypeSwitch libraray.
//Compile with
//   clang++ --std=c++1y test.cc  -o test
//or
//   g++ --std=c++1y test.cc  -o test

#include "TypeSwitch.h"

#include <iostream>

//Vairous test classes.
struct S1
{
   int value;
};

struct S2
{
   int value;

   S2 (const int x):
      value {x}
   {}
   
   virtual ~S2 () {}
};

struct S2Derived : public S2
{
   S2Derived (const int x) :
      S2 {x}
   {}
};

struct S3
{
   virtual ~S3 () {}
};

struct S3Derived : public S3
{};

struct S4
{};

struct S5
{};

//Uses TypeSwitch to print different messages based on the type stored in the argument.
void PrintAny (const boost::any& arg)
{
   using namespace TypeSwitch;

   Switch (
      arg,
      Case<S1>(
         [](const S1& a)
         {
            std::cout << "S1(" << a.value << ")"<< std::endl;
         }),
      Case<S2>(
         [](const S2& a)
         {
            std::cout << "S2(" << a.value << ")"<< std::endl;
         }),
      Case<S2Derived>( //Note that boost::any_cast ignores inheritance so a S2Derived will not be
                       //handled by the Case<S2> above in this case
         [](const S2Derived& a)
         {
            std::cout << "S2D(" << a.value << ")"<< std::endl;
         }),
      OneOf<S3, S4>(
         [](const boost::any& a)
         {
            std::cout << "S3 or 4" << "(";
            Switch (
               a,
               Case<S3>([](const S3& a) {std::cout << "S3";}),
               Case<S4>([](const S4& a) {std::cout << "S4";}),
               IgnoreOthers ());
            std::cout << ")" << std::endl;
         }),
      Otherwise (
         [](const boost::any& a)
         {
            std::cout << "Default" << std::endl;
         })
      );
}

//Used TypeSwitch to print different messages based on the type of arg. See notes below
void PrintS2 (const S2& arg)
{
   using namespace TypeSwitch;

   Switch (
      arg,
      Case<S2>(
         [](const S2& a)
         {
            std::cout << "S2(" << a.value << ")" << std::endl;
         }),
      Case<S2Derived>( //An S2Derived will be handled by the S2 case above since we are using
                       //references this time
         [](const S2Derived& a)
         {
            std::cout << "S2D(" << a.value << ") -- should never gt here !! " << std::endl;
         }),
      Otherwise (
         [](const S2& a)
         {
            std::cout << "Shouldn't get here!" << std::endl;
         })
      );
}

//Used TypeSwitch to print different messages based on the type of arg. This one handles inheritance properly. 
void PrintS2Corrected (const S2& arg)
{
   using namespace TypeSwitch;

   Switch (
      arg,
      Case<S2Derived>( //We put the S2Derived case first so that it can handle any S2Derived objects
                       //before the S2 case gets a hold of them
         [](const S2Derived& a)
         {
            std::cout << "S2D(" << a.value << ")" << std::endl;
         }),
      Case<S2>(
         [](const S2& a)
         {
            std::cout << "S2(" << a.value << ")" << std::endl;
         }),
      Otherwise (
         [](const S2& a)
         {
            std::cout << "Shouldn't get here!" << std::endl;
         })
      );
}

//Prints diferent messages based ont eh contents of a boost::variant
void PrintVariant (const boost::variant<S1, S2, S3>& v)
{
   using namespace TypeSwitch;

   Switch (
      v,
      Case<S1>(
         [](const S1& a)
         {
            std::cout << "S1(" << a.value << ")"<< std::endl;
         }),
      Case<S2>(
         [](const S2& a)
         {
            std::cout << "S2(" << a.value << ")"<< std::endl;
         }),
      Case<S3>(
         [](const S3& a)
         {
            std::cout << "S3()"<< std::endl;
         }),
      Otherwise (
         [](const auto& a) //c++14 lets unsigned short get away with not having to specify the
                           //variant type again
         {
            std::cout << "Shouldn't get here" << std::endl;
         })
      );
}

int main ()
{
   const auto s2d = boost::any {S2Derived {974496}};
   if (boost::any_cast<S2>(&s2d))
   {
      //we'll never get here because boost::any_cast does not take inheritance into account!!!!
      std::cout << "Got base ptr" << std::endl;
   }
   
   std::cout << "S1 = ";
   PrintAny (S1 {299298});

   std::cout << "S2 = ";
   PrintAny (S2 {984707});

   std::cout << "S2Derived = ";
   PrintAny (S2Derived {779639});

   std::cout << "S3 = ";
   PrintAny (S3 {});

   std::cout << "S3Derived = ";
   PrintAny (S3Derived {});

   std::cout << "S4 = ";
   PrintAny (S4 {});

   std::cout << "S5 = ";
   PrintAny (S5 {});

   std::cout << "S2: S2 = ";
   PrintS2 (S2 {238263});
   
   std::cout << "S2: S2Derived = ";
   PrintS2 (S2Derived {238263});

   std::cout << "S2 corrected: S2 = ";
   PrintS2Corrected (S2 {238263});
   
   std::cout << "S2 corrected: S2Derived = ";
   PrintS2Corrected (S2Derived {238263});

   std::cout << "Variant: S1 = ";
   PrintVariant (S1 {238263});

   std::cout << "Variant: S2 = ";
   PrintVariant (S2 {238263});

   std::cout << "Variant: S3 = ";
   PrintVariant (S3 {});
   
   std::cout << "Variant: S2Derived = ";
   PrintVariant (S2Derived {238263});

   return 0;
}
