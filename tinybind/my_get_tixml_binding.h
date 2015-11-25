#ifndef MY_GET_TIXML_BINDING_IMPL_H
#define MY_GET_TIXML_BINDING_IMPL_H

template<class T>
TiXml_Binding<T> const * get_tixml_binding( T const &, Identity_Base  );


//TiXml_Binding<float> const * get_tixml_binding( float const &, Identity_Base  );
//TiXml_Binding<double> const * get_tixml_binding( double const &, Identity_Base  );
//TiXml_Binding<int> const *  get_tixml_binding( int const &, Identity_Base  );
//TiXml_Binding<char const *> const * get_tixml_binding( char const * const &, Identity_Base  );
//TiXml_Binding<std::string> const * get_tixml_binding( std::string const &, Identity_Base  );

template<class T>
TiXml_Binding<T> const *  get_tixml_binding( T const &, Identity_Base  )
{
    static Generic_TiXml_Binding<T> binding;
    return &binding;
}

template<int array_size, class T>
TiXml_Binding< T[array_size] > const* get_tixml_binding( const T( &m)[array_size]  , Identity<T[array_size]> )
{
    static Array_TiXml_Binding<array_size, T> binding(false);
    return &binding;
}


template<int array_size, class T>
TiXml_Binding< T [array_size] > const* get_str_tiXml_binding( const T( &m)[array_size]  , Identity<T[array_size]> )
{
    static Array_Str_TiXml_Binding<array_size, T> binding;
    return &binding;
}

template<class T, class VecT>
TiXml_Binding<VecT> const * get_tixml_binding( std::vector<T> const &, Identity<VecT>  )
{
    static Stl_Container_TiXml_Binding<T, VecT> binding(false);
    return &binding;
}

template<class T, class VecT>
TiXml_Binding<VecT> const * get_tixml_binding( std::list<T> const &, Identity<VecT>  )
{
    static Stl_Container_TiXml_Binding<T, VecT> binding(false);
    return &binding;
}

template<typename Key_T, typename Value_T>
TiXml_Binding< std::pair<Key_T, Value_T> > const *
get_tixml_binding( std::pair<Key_T, Value_T> const &, Identity< std::pair<Key_T, Value_T> >  )
{
  static Member_TiXml_Binding< std::pair<Key_T, Value_T> > binding;
  if( binding.empty() )
  {
      binding.add_member( "key", member< std::pair<Key_T, Value_T>, Key_T>(&std::pair<Key_T, Value_T>::first) );
      binding.add_member( "value", member< std::pair<Key_T, Value_T>, Value_T>(&std::pair<Key_T, Value_T>::second) );
  }

  return &binding;
}

template<typename T_Key, typename T_Value, class Map_T>
TiXml_Binding<Map_T> const *
    get_tixml_binding( std::unordered_map<T_Key, T_Value> const &, Identity<Map_T>  )
{
    static Stl_Container_Map_TiXml_Binding<T_Key, T_Value, Map_T> binding(false);
    return &binding;
}

template<>
TiXml_Binding<float> const *
get_tixml_binding( float const &, Identity_Base  )
{
  static Generic_TiXml_Binding<float> binding;
  return &binding;
}

template<>
TiXml_Binding<double> const *
get_tixml_binding( double const &, Identity_Base  )
{
  static Generic_TiXml_Binding<double> binding;
  return &binding;
}

template<>
TiXml_Binding<int> const *
get_tixml_binding( int const &, Identity_Base  )
{
  static Generic_TiXml_Binding<int> binding;
  return &binding;
}

template<>
TiXml_Binding<char const *> const *
get_tixml_binding( char const * const &, Identity_Base  )
{
  static Generic_TiXml_Binding<char const *> binding;
  return &binding;
}

template<>
TiXml_Binding<std::string> const *
get_tixml_binding( std::string const &, Identity_Base  )
{
  static Generic_TiXml_Binding<std::string> binding;
  return &binding;
}




#endif // MY_GET_TIXML_BINDING_IMPL_H
