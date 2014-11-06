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

struct S3
{};

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
   std::cout << "S1 = ";
   Print (S1 {299298});

   std::cout << "S2 = ";
   Print (S2 {984707});

   std::cout << "S3 = ";
   Print (S3 ());

   std::cout << "S4 = ";
   Print (S4 ());

   std::cout << "S5 = ";
   Print (S5 ());

   return 0;
}
