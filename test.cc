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
      Case<S2Derived>(
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

void PrintS2 (const S2& arg)
{
   using namespace TypeSwitch;

   Switch (
      arg,
      Case<S2>(
         [](const S2& a)
         {
            std::cout << "S2(" << a.value << ")"<< std::endl;
         }),
      Case<S2Derived>(
         [](const S2Derived& a)
         {
            std::cout << "S2D(" << a.value << ")"<< std::endl;
         }),
      Otherwise (
         [](const S2& a)
         {
            std::cout << "Shouldn't get here!" << std::endl;
         })
      );
}

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
            std::cout << "S3(" << a.value << ")"<< std::endl;
         }),
      Otherwise (
         [](const auto& a)
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
      std::cout << "Got base ptr" << std::endl;
   }
   //boost::any_cast does not take inheritance into account!!!!
   
   std::cout << "S1 = ";
   PrintAny (S1 {299298});

   std::cout << "S2 = ";
   PrintAny (S2 {984707});

   std::cout << "S2Derived = ";
   PrintAny (S2Derived {779639});

   std::cout << "S3 = ";
   PrintAny (S3 {});

   std::cout << "S3Derived = ";
   PrintAny (S3Derived {376581});

   std::cout << "S4 = ";
   PrintAny (S4 {});

   std::cout << "S5 = ";
   PrintAny (S5 {});

   std::cout << "S2: S2 = ";
   PrintS2 (S2 {238263});
   
   std::cout << "S2: S2Derived = ";
   PrintS2 (S2Derived {238263});

   std::cout << "Variant: S1 = ";
   PrintVariant (S1 {238263});

   std::cout << "Variant: S2 = ";
   PrintVariant (S2 {238263});

   std::cout << "Variant: S3 = ";
   PrintVariant (S3 {168066});
   
   std::cout << "Variant: S2Derived = ";
   PrintVariant (S2Derived {238263});

   return 0;
}
