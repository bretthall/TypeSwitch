#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "TypeSwitch.h"

#include <string>
#include <sstream>

struct S1
{
   int value;

   S1 (const int x) :
      value {x}
   {}
};

struct S2
{
   int value;

   S2 (const int x) :
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
   int value;
   
   S3 (const int x) :
      value {x}
   {}

   virtual ~S3 () {}
};

struct S3Derived : public S3
{
   S3Derived (const int x) :
      S3 {x}
   {}
};

struct S4
{
   int value;

   S4 (const int x) :
      value {x}
   {}
};

struct S5
{
   int value;

   S5 (const int x) :
      value {x}
   {}
};

std::string GoodResult (const int expected, const char* prefix)
{
   std::stringstream out;
   out << "good " << prefix << "(" << expected << ")";
   return out.str ();
}

template <typename S_t>
std::string CompareValues (const S_t& s, const int expected, const char* prefix)
{
   if (s.value == expected)
   {
      return GoodResult (expected, prefix);
   }
   else
   {
      std::stringstream out;
      out << "bad " << prefix << "(" << s.value << " != " << expected << ")";
      return out.str ();      
   }
}

BOOST_AUTO_TEST_SUITE(TypeSwitchTests)

std::string TestAny (const boost::any& a, const int expectedValue)
{
   auto result = std::string ();
   using namespace  TypeSwitch;
   Switch (
      a,
      Case<S1>(
         [&](const auto& s)
         {
            result = CompareValues (s, expectedValue, "S1");
         }),
      Case<S2>(
         [&](const auto& s)
         {
            result = CompareValues (s, expectedValue, "S2");
         }),
      Case<S2Derived>(
         [&](const auto& s)
         {
            result = CompareValues (s, expectedValue, "S2Derived");
         }),
      OneOf<S3, S4>(
         [&](const boost::any& a)
         {
            return "Got S3 or S4";
         }),
      Otherwise (
         [&](const boost::any& a)
         {
            return "Default";
         }));
   return result;
}

BOOST_AUTO_TEST_CASE (any_tests)
{
   BOOST_CHECK_EQUAL (TestAny (S1{462999}, 462999), GoodResult (462999, "S1"));
}


BOOST_AUTO_TEST_SUITE_END(/*TypeSwitchTests*/)

