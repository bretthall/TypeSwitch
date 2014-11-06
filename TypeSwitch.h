#pragma once

#include <boost/any.hpp>

template <typename Src_t, typename Dest_t>
const Dest_t* SwitchCast (const Src_t& src)
{
   return dynamic_cast<const Dest_t*>(&src);
}

template <typename Dest_t>
const Dest_t* SwitchCast (const boost::any& src)
{
   return boost::any_cast<Dest_t>(&src);
}

template <typename Arg_t, typename Handler_t>
struct WCase
{
   const Handler_t& m_handler;

   WCase (const Handler_t& h):
      m_handler (h)
   {}

   template <typename BaseArg_t>
   bool operator()(const BaseArg_t& arg) const
   {
      if (const auto arg_p = SwitchCast<Arg_t>(arg))
      {
         m_handler (*arg_p);
         return true;
      }
      else
      {
         return false;
      }
   }
};

template <typename Arg_t, typename Handler_t>
WCase<Arg_t, Handler_t> Case (const Handler_t& handler)
{
   return WCase<Arg_t, Handler_t>(handler);
}

template <typename Base_t>
bool IsOneOf (const Base_t& base)
{
   return false;
}

template <typename Arg_t, typename... Args_t, typename Base_t>
bool IsOneOf (const Base_t& base)
{
   if (SwitchCast<Arg_t>(base))
   {
      return true;
   }
   else
   {
      return IsOneOf<Args_t...> (base);
   }
}


template <typename Handler_t, typename ... Args_t>
struct WOneOf
{
   const Handler_t& m_handler;

   WOneOf (const Handler_t& h):
      m_handler (h)
   {}

   template <typename BaseArg_t>
   bool operator()(const BaseArg_t& arg) const
   {
      if (IsOneOf<Args_t...>(arg))
      {
         m_handler (arg);
         return true;
      }
      else
      {
         return false;
      }
   }
};

template <typename ... Args_t, typename Handler_t>
WOneOf<Handler_t, Args_t...> OneOf (const Handler_t& handler)
{
   return WOneOf<Handler_t, Args_t...>(handler);
}

template <typename Handler_t>
struct WOtherwise
{
   const Handler_t& m_handler;

   WOtherwise (const Handler_t& h):
      m_handler (h)
   {}
         
   template <typename Arg_t>
   bool operator()(const Arg_t& arg) const
   {
      m_handler (arg);
      return true;
   }         
};

template <typename Handler_t>
WOtherwise<Handler_t> Otherwise (const Handler_t& handler)
{
   return WOtherwise<Handler_t>(handler);
}

struct IgnoreOthers
{
   template <typename Arg_t>
   bool operator()(const Arg_t&) const
   {
      return true;
   }         
};

template <typename Arg_t, typename Handler_t>
void Switch (const Arg_t& arg, const WOtherwise<Handler_t>& handler)
{
   handler (arg);
}

template <typename Arg_t>
void Switch (const Arg_t& arg, const IgnoreOthers&)
{}

template <typename Arg_t, typename Handler_t, typename... Handlers_t>
void Switch (const Arg_t& arg, const Handler_t& handler, const Handlers_t&... handlers)
{
   //NOTE: If you get an error about "no specialization for Switch (arg, handler) then your
   //Switch call is missing either Otherwise or IgnoreOthers at the end of the handlers.
         
   if (!handler (arg))
   {
      Switch (arg, handlers...);
   }
}

// void Test ()
// {
//    Switch (
//       arg,
//       Case<WType1>(
//          [&](const WType1& arg)
//          {
//          }),
//       Case<WType2>(
//          [&](const WType2& arg)
//          {
//             Switch (
//                arg,
//                Case<WDerivedFromType2>(
//                   [&](const WDerivedFromType2& arg)
//                   {
//                   }),
//                IgnoreOthers ());
//          }),
//       Case<WType3>(
//          [&](const WType3& arg)
//          {
//          }),
//       OneOf<WType4, WType5, WType6>(
//          [&](const Arg& arg)
//          {
//          }),         
//       Otherwise (
//          [&](const Arg& arg)
//          {
//          }));
// }
