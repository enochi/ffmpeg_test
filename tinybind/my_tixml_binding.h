#ifndef MY_TIXML_BINDING_H
#define MY_TIXML_BINDING_H

template<class T>
class Member_TiXml_Binding : public TiXml_Binding<T>
{
private:
    std::vector<I_Member_Bind_Xml<T> *> members_;

public:
    bool empty() const
    {
        return members_.empty();
    }

    I_Member_Bind_Xml<T> * add_member( Tag tag, I_Member_Bind_Xml<T> * mph )
    {
        mph->flags_ = Member_Serialize_Flags_None;
        mph->tag_ = tag;
        members_.push_back( mph );
        return mph;
    }

    virtual bool from_xml( TiXmlElement const & elem, T * data, Serialize_Params const & params ) const
    {
        TiXmlElement const * child = elem.FirstChildElement();
        for( unsigned int i = 0; i < members_.size(); i++ ) {
            I_Member_Bind_Xml<T> * mph = members_[i];
            bool error = false;

            bool ret;
            if( mph->is_attribute_member() ) {
                ret = mph->from_xml( elem, data );
            } else {
                if( !child ) {
                    return false;
                }
                ret = mph->from_xml( *child, data );
            }
            error = !ret;
            if( mph->is_attribute_member() ) {
            } else if( !error ) {
                child = child->NextSiblingElement();
            }

            if( error ) {
                if( mph->is_attribute_member() ){
                    // no problem
                    continue;
                } else {
                    // error
                    return false;
                }
            }
        }
        return true;
    }

    virtual bool into_xml( TiXmlElement * elem, T const & data, Serialize_Params const & ) const
    {
        for( unsigned int i = 0; i < members_.size(); i++ ) {
            I_Member_Bind_Xml<T> * mph = members_[i];
            mph->into_xml( elem, &data );
        }
        return true;
    }
};

template<class T>
class Generic_TiXml_Binding : public TiXml_Binding<T>
{
public:
    virtual bool from_xml( TiXmlElement const & elem, T * data, Serialize_Params const & ) const
    {
        char const *p = "";

        TiXmlNode * node = ((TiXmlElement&)elem).FirstChild();
        if(node != NULL)
        {
            TiXmlText * nodedata = node->ToText();
            if(nodedata != NULL)
                p = nodedata->Value();
        }
        convert_from_string(p, data );

        //convert_from_string( nodedata->Value(), data );
        return true;
    }

    virtual bool into_xml( TiXmlElement * elem, T const & data, Serialize_Params const & ) const
    {
        TiXmlText textData( convert_to_string( data ) );
        elem->InsertEndChild( textData );
        return true;
    }
};

///////////////////////////////////////////////////////////////////
template<int array_size, typename T>
class Array_TiXml_Binding : public TiXml_Binding< T[array_size] >
{
    typedef T Array[array_size] ;
public:

    char const * subTag_;
    bool useSubTag_;
    char const * sizeAttributeName_;
    Array_TiXml_Binding(bool useSubTag, char const * st = NULL, char const * sizeAttributeName = NULL)
        :subTag_(st), useSubTag_(useSubTag), sizeAttributeName_(sizeAttributeName)
    {
    }

    virtual bool from_xml( TiXmlElement const & elem, Array *data, Serialize_Params const & params ) const
    {
        TiXmlElement const * child;
        child = elem.FirstChildElement();
        if( sizeAttributeName_ )
        {
            int sz = 0;
            convert_from_string( elem.Attribute(sizeAttributeName_), &sz );
            if( sz )
            {
                //data->reserve(sz);
            }
        }

        int child_count = 0;
        while(child)
        {
            if(child_count < array_size)
            {
                T * value = &(*data) [child_count++];
                TiXml_Binding<T> const * binding = get_tixml_binding( *value,  Identity<T>()  );
                bool ret = binding->from_xml( *child, value, params );
                if( ! ret ) {
                    return false;
                }
                child = child->NextSiblingElement();
            }
        }
        return true;
    }

    virtual bool into_xml( TiXmlElement * elem, Array const &data, Serialize_Params const & params ) const
    {
        if( sizeAttributeName_ ) {
            elem->SetAttribute( sizeAttributeName_, convert_to_string( array_size ) );
        }
        for( int i = 0; i < array_size; i++ ) {
            T const & value = data[i];
            TiXml_Binding<T> const * binding = get_tixml_binding( value,  Identity<T>()  );
            char const * tag;
            if( useSubTag_ ) {
                tag = subTag_;
            } else {
                tag = elem->Value();
            }
            TiXmlElement child(tag);
            if( ! binding->into_xml( &child, value, params ) ) {
                return false;
            }
            elem->InsertEndChild(child);
        }
        return true;
    }
};
///////////////////////////////////////////////////////////////////
/*
template<typename T>
class Array_Ptr_TiXml_Binding : public TiXml_Binding<T>
{
    typedef T Array[array_size] ;
public:

    char const * subTag_;
    bool useSubTag_;
    char const * sizeAttributeName_;
    Array_TiXml_Binding(bool useSubTag, char const * st = NULL, char const * sizeAttributeName = NULL)
        :subTag_(st), useSubTag_(useSubTag), sizeAttributeName_(sizeAttributeName)
    {
    }

    virtual bool from_xml( TiXmlElement const & elem, Array *data, Serialize_Params const & params ) const
    {
        TiXmlElement const * child;
        child = elem.FirstChildElement();
        if( sizeAttributeName_ )
        {
            int sz = 0;
            convert_from_string( elem.Attribute(sizeAttributeName_), &sz );
            if( sz )
            {
                //data->reserve(sz);
            }
        }

        int child_count = 0;
        while(child)
        {
            if(child_count < array_size)
            {
                T * value = &(*data) [child_count++];
                TiXml_Binding<T> const * binding = get_tixml_binding( *value,  Identity<T>()  );
                bool ret = binding->from_xml( *child, value, params );
                if( ! ret ) {
                    return false;
                }
                child = child->NextSiblingElement();
            }
        }
        return true;
    }

    virtual bool into_xml( TiXmlElement * elem, Array const &data, Serialize_Params const & params ) const
    {
        if( sizeAttributeName_ ) {
            elem->SetAttribute( sizeAttributeName_, convert_to_string( array_size ) );
        }
        for( int i = 0; i < array_size; i++ ) {
            T const & value = data[i];
            TiXml_Binding<T> const * binding = get_tixml_binding( value,  Identity<T>()  );
            char const * tag;
            if( useSubTag_ ) {
                tag = subTag_;
            } else {
                tag = elem->Value();
            }
            TiXmlElement child(tag);
            if( ! binding->into_xml( &child, value, params ) ) {
                return false;
            }
            elem->InsertEndChild(child);
        }
        return true;
    }
};
*/
///////////////////////////////////////////////////////////////////
template<int array_size, typename T>
class Array_Str_TiXml_Binding : public TiXml_Binding< T[array_size] >
{
    typedef T Array[array_size] ;
public:

    virtual bool from_xml( TiXmlElement const & elem, Array *data, Serialize_Params const & params ) const
    {
        TiXmlNode * node = ((TiXmlElement&)elem).FirstChild();

        if(node != NULL)
        {
            TiXmlText * nodedata = node->ToText();
            char const *p = "";
            if(nodedata != NULL)
                p = nodedata->Value();
            int len = strlen(p);
            if(len > array_size) len = array_size;
            memcpy((*data), p, len);
        }
        //convert_from_string(p, data );

        //convert_from_string( nodedata->Value(), data );
        return true;


    }

    virtual bool into_xml( TiXmlElement * elem, Array const &data, Serialize_Params const & params ) const
    {
        char const *_data = (char const*)data;
        TiXmlText textData( _data );
        elem->InsertEndChild( textData );
        return true;
    }
};
////////////////////////////////////////////////////////////////////
template<class T, class VecT>
class Stl_Container_TiXml_Binding : public TiXml_Binding<VecT>
{
public:

    char const * subTag_;
    bool useSubTag_;
    char const * sizeAttributeName_;
    Stl_Container_TiXml_Binding(bool useSubTag, char const * st = NULL, char const * sizeAttributeName = NULL)
        :subTag_(st), useSubTag_(useSubTag), sizeAttributeName_(sizeAttributeName)
    {
    }

    virtual bool from_xml( TiXmlElement const & elem, VecT * data, Serialize_Params const & params ) const
    {
        data->clear();
        TiXmlElement const * child;
        child = elem.FirstChildElement();
        if( sizeAttributeName_ ) {
            int sz = 0;
            convert_from_string( elem.Attribute(sizeAttributeName_), &sz );
            if( sz ) {
                //data->reserve(sz);
            }
        }
        while(child) {
            T * value = new T();
            TiXml_Binding<T> const * binding = get_tixml_binding( *value,  Identity<T>()  );
            bool ret = binding->from_xml( *child, value, params );
            data->push_back(*value);
            if( ! ret ) {
                return false;
            }
            child = child->NextSiblingElement();
        }
        return true;
    }

    virtual bool into_xml( TiXmlElement * elem, VecT const & data, Serialize_Params const & params ) const
    {
        if( sizeAttributeName_ ) {
            elem->SetAttribute( sizeAttributeName_, convert_to_string( data.size() ) );
        }
        for( typename VecT::const_iterator i = data.begin(); i != data.end(); i++ ) {
            T const & value = *i;
            TiXml_Binding<T> const * binding = get_tixml_binding( value,  Identity<T>()  );
            char const * tag;
            if( useSubTag_ ) {
                tag = subTag_;
            } else {
                tag = elem->Value();
            }
            TiXmlElement child(tag);
            if( ! binding->into_xml( &child, value, params ) ) {
                return false;
            }
            elem->InsertEndChild(child);
        }
        return true;
    }
};

template<class T, class VecT>
class Stl_Container_Ptr_Binding : public TiXml_Binding<VecT>
{
public:

    char const * subTag_;
    bool useSubTag_;
    char const * sizeAttributeName_;
    Stl_Container_Ptr_Binding(bool useSubTag, char const * st = NULL, char const * sizeAttributeName = NULL)
        :subTag_(st), useSubTag_(useSubTag), sizeAttributeName_(sizeAttributeName)
    {
    }

    virtual bool from_xml( TiXmlElement const & elem, VecT * data, Serialize_Params const & params ) const
    {
        data->clear();
        TiXmlElement const * child;
        child = elem.FirstChildElement();
        if( sizeAttributeName_ ) {
            int sz = 0;
            convert_from_string( elem.Attribute(sizeAttributeName_), &sz );
            if( sz ) {
                //data->reserve(sz);
            }
        }
        while(child) {
            T * value = new T();
            if( !value->create() ) {
                return false;
            }
            TiXml_Binding<T> const * binding = get_tixml_binding( *value,  Identity<T>()  );
            bool ret = binding->from_xml( *child, value, params );
            data->push_back(value);
            if( ! ret ) {
                return false;
            }
            child = child->NextSiblingElement();
        }
        return true;
    }

    virtual bool into_xml( TiXmlElement * elem, VecT const & data, Serialize_Params const & params ) const
    {
        if( sizeAttributeName_ ) {
            elem->SetAttribute( sizeAttributeName_, convert_to_string( data.size() ) );
        }
        for( typename VecT::const_iterator i = data.begin(); i != data.end(); i++ ) {
            T const * value = *i;
            if( ! value ) {
                continue;
            }
            TiXml_Binding<T> const * binding = get_tixml_binding( *value,  Identity<T>()  );
            char const * tag;
            if( useSubTag_ ) {
                tag = subTag_;
            } else {
                tag = elem->Value();
            }
            TiXmlElement child(tag);
            if( ! binding->into_xml( &child, *value, params ) ) {
                return false;
            }
            elem->InsertEndChild(child);
        }
        return true;
    }
};

/////////////////////////////////////////////////
template<typename T_Key, typename T_Value, class T_Map>
class Stl_Container_Map_TiXml_Binding : public TiXml_Binding<T_Map>
{
public:

    char const * subTag_;
    bool useSubTag_;
    char const * sizeAttributeName_;
    Stl_Container_Map_TiXml_Binding(bool useSubTag, char const * st = NULL, char const * sizeAttributeName = NULL)
        :subTag_(st), useSubTag_(useSubTag), sizeAttributeName_(sizeAttributeName)
    {
    }

    virtual bool from_xml( TiXmlElement const & elem, T_Map * data, Serialize_Params const & params ) const
    {
        data->clear();
        TiXmlElement const * child;
        child = elem.FirstChildElement();
        if( sizeAttributeName_ ) {
            int sz = 0;
            convert_from_string( elem.Attribute(sizeAttributeName_), &sz );
            if( sz ) {
                //data->reserve(sz);
            }
        }
        while(child) {
            typename std::pair<typename T_Map::key_type, typename T_Map::mapped_type> pair;
            TiXml_Binding<typename std::pair<typename T_Map::key_type, typename T_Map::mapped_type> > const * binding =
                    get_tixml_binding( pair,  Identity< typename std::pair<typename T_Map::key_type, typename T_Map::mapped_type> >()  );
            bool ret = binding->from_xml( *child, &pair, params );
            (*data)[pair.first] = pair.second;
            if( ! ret ) {
                return false;
            }
            child = child->NextSiblingElement();
        }
        return true;
    }

    virtual bool into_xml( TiXmlElement * elem, T_Map const & data, Serialize_Params const & params ) const
    {
        if( sizeAttributeName_ ) {
            elem->SetAttribute( sizeAttributeName_, convert_to_string( data.size() ) );
        }
        for( typename T_Map::const_iterator i = data.begin(); i != data.end(); i++ ) {
            typename std::pair<typename T_Map::key_type, typename T_Map::mapped_type> const & value = *i;
            TiXml_Binding<typename std::pair<typename T_Map::key_type, typename T_Map::mapped_type> > const * binding
                    = get_tixml_binding( value,  Identity< typename std::pair<typename T_Map::key_type, typename T_Map::mapped_type> >()  );
            char const * tag;
            if( useSubTag_ ) {
                tag = subTag_;
            } else {
                tag = elem->Value();
            }
            TiXmlElement child(tag);
            if( ! binding->into_xml( &child, value, params ) ) {
                return false;
            }
            elem->InsertEndChild(child);
        }
        return true;
    }
};

#endif // MY_TIXML_BINDING_H
