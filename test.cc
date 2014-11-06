#include "TypeSwitch.h"

#include <iostream>

struct S1
{
   int value;
};

struct S2
{
   int value;
};

struct S2Derived : public S2
{
   S2Derived (const int x) :
      S2 {x}
   {}
};

struct S3
{
   int value;
};

struct S3Derived : public S3
{
   S3Derived (const int x) :
      S3 {x}
   {}
};

struct S4
{};

struct S5
{};

void Print (const boost::any& arg)
{
   Switch (
      arg,
      Case<S1>([](const S1& a) {std::cout << "S1(" << a.value << ")"<< std::endl;}),
      Case<S2>([](const S2& a) {std::cout << "S2(" << a.value << ")"<< std::endl;}),
      Case<S2Derived>([](const S2Derived& a) {std::cout << "S2D(" << a.value << ")"<< std::endl;}),
      //Case<S3Derived>([](const S3Derived& a) {std::cout << "S3D(" << a.value << ")"<< std::endl;}),
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
      Otherwise ([](const boost::any& a) {std::cout << "Default" << std::endl;}));
}

int main ()
{
   const auto s2d = boost::any (S2Derived {974496});
   if (boost::any_cast<S2>(&s2d))
   {
      std::cout << "Got base ptr" << std::endl;
   }
   //boost::any_cast does not take inheritance into account!!!!
   
   std::cout << "S1 = ";
   Print (S1 {299298});

   std::cout << "S2 = ";
   Print (S2 {984707});

   std::cout << "S2Derived = ";
   Print (S2Derived {779639});

   std::cout << "S3 = ";
   Print (S3 ());

   std::cout << "S3Derived = ";
   Print (S3Derived {376581});

   std::cout << "S4 = ";
   Print (S4 ());

   std::cout << "S5 = ";
   Print (S5 ());

   return 0;
}
