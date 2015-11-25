#ifndef MY_TINYBIND_H
#define MY_TINYBIND_H


class TiXmlElement;
class Serialize_Params;

template<class T>
class TiXml_Binding
{
public:
    virtual bool from_xml( TiXmlElement const & elem, T * data, Serialize_Params const & ) const = 0;
    virtual bool into_xml( TiXmlElement * elem, T const & data, Serialize_Params const & ) const = 0;
};


template<class T>
bool bind_to_xml( TiXmlElement * elemOut, T const & dataIn );

template<class T>
bool bind_from_xml( TiXmlElement const & elemIn, T * dataOut );

#include "my_tinybind_impl.h"

#endif // MY_TINYBIND_H
