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
struct CaseHandler
{
   const Handler_t& m_handler;

   CaseHandler (const Handler_t& h):
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
CaseHandler<Arg_t, Handler_t> Case (const Handler_t& handler)
{
   return CaseHandler<Arg_t, Handler_t>(handler);
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
struct OneOfHandler
{
   const Handler_t& m_handler;

   OneOfHandler (const Handler_t& h):
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
OneOfHandler<Handler_t, Args_t...> OneOf (const Handler_t& handler)
{
   return OneOfHandler<Handler_t, Args_t...>(handler);
}

template <typename Handler_t>
struct OtherwiseHandler
{
   const Handler_t& m_handler;

   OtherwiseHandler (const Handler_t& h):
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
OtherwiseHandler<Handler_t> Otherwise (const Handler_t& handler)
{
   return OtherwiseHandler<Handler_t>(handler);
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
void Switch (const Arg_t& arg, const OtherwiseHandler<Handler_t>& handler)
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
