TypeSwitch:

This library allows one to write code like:

   Switch (
      arg,
      Case<WType1>(
         [&](const WType1& arg)
         {
         }),
      Case<WType2>(
         [&](const WType2& arg)
         {
            Switch (
               arg,
               Case<WDerivedFromType2>(
                  [&](const WDerivedFromType2& arg)
                  {
                  }),
               IgnoreOthers ());
         }),
      Case<WType3>(
         [&](const WType3& arg)
         {
         }),
      OneOf<WType4, WType5, WType6>(
         [&](const Arg& arg)
         {
         }),         
      Otherwise (
         [&](const Arg& arg)
         {
         }));

where arg is of some base type and WType1, WType2, etc. are derived types. This avoids having have a bunch of if's and dynamic_casts spalyed all over your code. 