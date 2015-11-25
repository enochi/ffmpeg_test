#ifndef MY_CONVERT_STRING_H
#define MY_CONVERT_STRING_H

template<class T>
char const *
convert_to_string( T const & t )
{
  std::stringstream str;
  static std::string strOut;
  str << t;
  strOut = str.str();
  return strOut.c_str();
}

template<class T>
void
convert_from_string( char const * strIn, T * dataOut )
{
  std::stringstream str;
  str << strIn;
  str >> *dataOut;
}

template<>
void
convert_from_string<std::string>( char const * strIn,  std::string * dataOut )
{
    (*dataOut) = strIn;
}


template<>
void
convert_from_string<char const *>( char const * strIn, const char * * dataOut )
{
  static std::string strHolder;
  strHolder = strIn;
  *dataOut = strHolder.c_str();
}

#endif // MY_CONVERT_STRING_H
