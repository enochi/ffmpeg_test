#ifndef MY_TINYBIND_IMPL_H
#define MY_TINYBIND_IMPL_H

#include <vector>
#include <list>
#include <unordered_map>

#include "my_tinybind_defines.h"

#include "my_tinybind_types.h"

#include "tinyxml.h"

#include "my_serialize_param.h"
#include "my_identity.h"

//#include "my_convert_string_declare.h"



//#include "my_get_tixml_binding_declare.h"



//#include "my_tixml_binding_declare.h"

#include "my_convert_string.h"

#include "my_member.h"

#include "my_tixml_binding.h"

#include "my_get_tixml_binding.h"




template<class T>
bool bind_to_xml( TiXmlElement * elemOut, T const & dataIn )
{
    TiXml_Binding<T> const * binding = get_tixml_binding( dataIn, Identity<T>() );
    Serialize_Params params;
    return binding->into_xml( elemOut, dataIn, params );
}

template<class T>
bool bind_from_xml( TiXmlElement const & elemIn, T * dataOut )
{
    TiXml_Binding<T> const * binding = get_tixml_binding( *dataOut, Identity<T>() );
    Serialize_Params params;
    return binding->from_xml( elemIn, dataOut, params );
}


#endif // MY_TINYBIND_IMPL_H
