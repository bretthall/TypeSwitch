//          Copyright Brett Hall 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/any.hpp>
#include <boost/variant.hpp>

namespace TypeSwitch
{

   /**
    * Casts the given object to the given type using dynamic_cast. If the cast fails then nullptr is
    * returned.
    */
   template <typename Src_t, typename Dest_t>
   const Dest_t* SwitchCast (const Src_t& src)
   {
      return dynamic_cast<const Dest_t*>(&src);
   }

   /**
    * Casts the contents of the given boost::any object to the given type. If the boost::any object
    * is empty or its contents are not of the correct type then nullptr is returned. Note that this
    * function uses boost::any_cast and as such it doesn't pay any attention to inheritance, a cast
    * type to Foo will only succeed if the boost::any object contains a Foo object and not a
    * sub-class of Foo.
    */
   template <typename Dest_t>
   const Dest_t* SwitchCast (const boost::any& src)
   {
      return boost::any_cast<Dest_t>(&src);
   }

   /**
    * Casts the contents of the given boost variant to the given type. If the variant doesn't
    * contain an object of the given type the nullptr si returned.
    */
   template <typename Dest_t, typename ... Types_t>
   const Dest_t* SwitchCast (const boost::variant<Types_t...>& src)
   {
      return boost::get<Dest_t>(&src);
   }

   namespace Detail
   {
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
   }
   
   /**
    * Creates a case clause to handle a specific type.
    *
    * @param Arg_t The type to handle. The argument of the enclosing Switch call will be cast to
    * this type using SwitchCast and if that cast is successful then the resulting object will be
    * passed to the handler.
    * @param handler The function that will be called if Switch's argument is of the proper
    * type. This function will be passed the object that SwitchCast returned (note: the *object*,
    * not the pointer returned by SwitchCast -- the pointer will be derefernced).
    */
   template <typename Arg_t, typename Handler_t>
   Detail::CaseHandler<Arg_t, Handler_t> Case (const Handler_t& handler)
   {
      return Detail::CaseHandler<Arg_t, Handler_t>(handler);
   }

   namespace Detail
   {
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
   }
   
   /**
    * Creates a case clause in a call to Switch that handles multiple types.
    *
    * @param Args_t The types that will be handled. If the Switch's argument can be cast to one of
    * these types then the handler will be called.
    * @params handler The function that will be called if the Switch's argument can be cast to one
    * of the types in Args_t using SwitchCast. The function will be called with the argument of
    * Switch since the types in Args_t may be unrelated.
    */
   template <typename ... Args_t, typename Handler_t>
   Detail::OneOfHandler<Handler_t, Args_t...> OneOf (const Handler_t& handler)
   {
      return Detail::OneOfHandler<Handler_t, Args_t...>(handler);
   }

   namespace Detail
   {
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
   }
   
   /**
    * Create a case clause that handles any type. This should be placed at the end of the case
    * clauses for a switch call, any case clauses that come after it will never be checked.
    *
    * @param handler The function that will be called if this clause is hit. It will be passed the
    * argument that Switch was called with.
    */
   template <typename Handler_t>
   Detail::OtherwiseHandler<Handler_t> Otherwise (const Handler_t& handler)
   {
      return Detail::OtherwiseHandler<Handler_t>(handler);
   }

   /**
    * Creates a case clause the does nothing. Any set of case clauses in a call to Switch must end
    * with either an Otherwise clause of IgnoreOthers. This forces the caller to be explicit about
    * how to handle unforseen cases.
    */
   struct IgnoreOthers
   {
      template <typename Arg_t>
      bool operator()(const Arg_t&) const
      {
         return true;
      }         
   };

   //!{
   /**
    * The "type switch" function.
    *
    * @param arg The argument for the switch.
    * @param handlers The "case clauses", each one must be either a Case, OneOf, Otherwise, or IgnoreOthers
    * with the final clause being one of the latter two. Each clause is checked in order and the
    * first that matches has its handler called.
    */
   template <typename Arg_t, typename Handler_t>
   void Switch (const Arg_t& arg, const Detail::OtherwiseHandler<Handler_t>& handler)
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
   //!}
}
