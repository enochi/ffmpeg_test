#ifndef MY_MEMBER_XML_H
#define MY_MEMBER_XML_H

template<class T>
class I_Member_Bind_Xml
{
public:
    Member_Serialize_Flags flags_;
    Tag tag_;
    Serialize_Params params_;

    void set_flags( Member_Serialize_Flags f )
    {
        flags_ = f;
    }

    Serialize_Params const & params()
    {
        params_.tag_ = tag_;
        return params_;
    }

    virtual char const * tag( int which = 0) { return tag_.tag_[which]; }

    virtual bool from_xml( TiXmlElement const &, T * ) = 0;
    virtual bool into_xml( TiXmlElement *, T const * ) = 0;
    virtual bool is_attribute_member() = 0;


};

template<class T, class MT>
class Member_Bind_Xml_Element : public I_Member_Bind_Xml<T>
{
    using I_Member_Bind_Xml<T>::params;
    using I_Member_Bind_Xml<T>::tag;
public:
    I_Member_Value_Policy<T, MT> * mvPolicy_;
    Member_Bind_Xml_Element( I_Member_Value_Policy<T, MT> * mvPolicy )
    {
        mvPolicy_ = mvPolicy;
    }

    virtual bool from_xml( TiXmlElement const & elem, T * thisPtr)
    {
        MT & mv = const_cast<MT &>(mvPolicy_->get_member_value(thisPtr));
        TiXml_Binding<MT> const * binding = get_tixml_binding( mv,  Identity<MT>()  );
        if( binding->from_xml(elem, &mv, params()) ) {
            mvPolicy_->set_member_value(thisPtr, mv);
            return true;
        } else {
            return false;
        }
    }

    virtual bool into_xml( TiXmlElement * elem, T const * thisPtr)
    {
        MT const & mv = mvPolicy_->get_member_value(thisPtr);
        TiXml_Binding<MT> const * binding = get_tixml_binding( mv,  Identity<MT>() );
        std::string oldValue = elem->Value();
        elem->SetValue( tag() );
        bool ret = binding->into_xml( elem, mv, params() );
        elem->SetValue( oldValue );
        return ret;
    }

    virtual bool is_attribute_member() { return true; }
};

template<class T, class MT>
class Member_Bind_Xml_Child_Element : public I_Member_Bind_Xml<T>
{
    using I_Member_Bind_Xml<T>::tag;
    using I_Member_Bind_Xml<T>::params;
public:
    I_Member_Value_Policy<T, MT> * mvPolicy_;
    Member_Bind_Xml_Child_Element( I_Member_Value_Policy<T, MT> * mvPolicy )
    {
        mvPolicy_ = mvPolicy;
    }

    virtual bool from_xml( TiXmlElement const & elem, T * thisPtr)
    {
        if( !strcmp(elem.Value(), tag()) ) {
            MT mv;
            TiXml_Binding<MT> const * binding = get_tixml_binding( mv,  Identity<MT>()  );
            if( binding->from_xml(elem, &mv, params()) ) {
                mvPolicy_->set_member_value(thisPtr, mv);
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    virtual bool into_xml( TiXmlElement * elem, T const * thisPtr)
    {
        MT const & mv = mvPolicy_->get_member_value(thisPtr);
        TiXmlElement child(tag());
        TiXml_Binding<MT> const * binding = get_tixml_binding( mv, Identity<MT>()  );
        if( binding->into_xml( &child, mv, params() ) ) {
            elem->InsertEndChild(child);
            return true;
        } else {
            return false;
        }
    }

    virtual bool is_attribute_member() { return false; }
};

template<class T, class MT>
class Member_Array_Ptr_Bind_Xml_Child_Element : public I_Member_Bind_Xml<T>
{
    using I_Member_Bind_Xml<T>::tag;
    using I_Member_Bind_Xml<T>::params;
public:
    I_Member_Value_Policy<T, MT> * mvPolicy_;
    Member_Array_Ptr_Bind_Xml_Child_Element( I_Member_Value_Policy<T, MT> * mvPolicy, int _size )
    {
        mvPolicy_ = mvPolicy;
        size = _size;
    }

    virtual bool from_xml( TiXmlElement const & elem, T * thisPtr)
    {
        if( !strcmp(elem.Value(), tag()) ) {
            MT mv;
            TiXml_Binding<MT> const * binding = get_tixml_binding( mv,  Identity<MT>(), size  );
            if( binding->from_xml(elem, &mv, params()) ) {
                mvPolicy_->set_member_value(thisPtr, mv);
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    virtual bool into_xml( TiXmlElement * elem, T const * thisPtr)
    {
        MT const & mv = mvPolicy_->get_member_value(thisPtr);
        TiXmlElement child(tag());
        TiXml_Binding<MT> const * binding = get_tixml_binding( mv, Identity<MT>(), size );
        if( binding->into_xml( &child, mv, params() ) ) {
            elem->InsertEndChild(child);
            return true;
        } else {
            return false;
        }
    }

    virtual bool is_attribute_member() { return false; }

    int size;
};

template<int array_size, class T, class MT>
class Member_Str_Bind_Xml_Child_Element : public I_Member_Bind_Xml<T>
{
    using I_Member_Bind_Xml<T>::tag;
    using I_Member_Bind_Xml<T>::params;
public:
    I_Member_Value_Policy<T, MT[array_size]> * mvPolicy_;
    Member_Str_Bind_Xml_Child_Element( I_Member_Value_Policy<T, MT[array_size]> * mvPolicy )
    {
        mvPolicy_ = mvPolicy;
    }

    virtual bool from_xml( TiXmlElement const & elem, T * thisPtr)
    {
        if( !strcmp(elem.Value(), tag()) ) {
            MT mv [array_size] ;
			memset(mv, 0, sizeof(mv));
            TiXml_Binding<MT[array_size]> const * binding = get_str_tiXml_binding( mv,  Identity<MT[array_size]>()  );
            if( binding->from_xml(elem, &mv, params()) ) {
                mvPolicy_->set_member_value(thisPtr, mv);
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    virtual bool into_xml( TiXmlElement * elem, T const * thisPtr)
    {
        const MT( & mv)[array_size] = mvPolicy_->get_member_value(thisPtr);
        TiXmlElement child(tag());
        TiXml_Binding<MT[array_size]> const * binding = get_str_tiXml_binding( mv, Identity<MT[array_size]>()  );
        if( binding->into_xml( &child, mv, params() ) ) {
            elem->InsertEndChild(child);
            return true;
        } else {
            return false;
        }
    }

    virtual bool is_attribute_member() { return false; }
};
///////////////////////////////////////

template<class T, class MT>
class Member_Bind_Xml_Attribute  : public I_Member_Bind_Xml<T>
{
    using I_Member_Bind_Xml<T>::tag;
public:
    I_Member_Value_Policy<T, MT> * mvPolicy_;
    Member_Bind_Xml_Attribute( I_Member_Value_Policy<T, MT> * mvPolicy )
    {
        mvPolicy_ = mvPolicy;
    }

    virtual bool from_xml( TiXmlElement const & elem, T * thisPtr)
    {
        MT mv;
        const char * attributeValue = elem.Attribute( tag() );
        if( attributeValue && *attributeValue ) {
            convert_from_string( attributeValue, &mv );
            mvPolicy_->set_member_value(thisPtr, mv);
            return true;
        } else {
            return false;
        }
    }

    virtual bool into_xml( TiXmlElement * elem, T const * thisPtr)
    {
        MT const & mv = mvPolicy_->get_member_value(thisPtr);
        char const * attributeValue = convert_to_string( mv );
        elem->SetAttribute( tag(), attributeValue );
        return true;
    }

    virtual bool is_attribute_member() { return true; }
};
#endif // MY_MEMBER_XML_H
