#ifndef MY_MEMBER_H
#define MY_MEMBER_H

#include "my_member_value.h"
#include "my_member_xml.h"

template<class T, class MT, class FromXmlPolicy, class MemberValuePolicy>
class MemberHolder
{
public:
    FromXmlPolicy xmlPolicy_;
    MemberValuePolicy mvPolicy_;

    MemberHolder()
        : xmlPolicy_((I_Member_Value_Policy<T, MT> *)&mvPolicy_)
    {
    }
};

template<class T, class MT, class FromXmlPolicy, class MemberValuePolicy>
class ArrayPtrMemberHolder
{
public:
    FromXmlPolicy xmlPolicy_;
    MemberValuePolicy mvPolicy_;

    ArrayPtrMemberHolder(int _size)
        : mvPolicy_(_size),
          xmlPolicy_((I_Member_Value_Policy<T, MT> *)&mvPolicy_, _size)
    {
    }
};

template<class T, class MT>
I_Member_Bind_Xml<T> * member(  MT T::*mp )
{
    typedef Member_Bind_Xml_Child_Element<T, MT> XmlPolicy;
    typedef Member_Value_Ptr<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberPtr_ = mp;
    return &mph->xmlPolicy_;
}

template<int array_size, class T, class MT>
I_Member_Bind_Xml<T> * member(MT  (T::*mp) [array_size] )
{
    typedef Member_Bind_Xml_Child_Element<T, MT[array_size] > XmlPolicy;
    typedef Member_Value_Array<T, MT[array_size]> MemberValuePolicy;
    typedef MemberHolder<T, MT[array_size], XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberPtr_ = mp;
    return &mph->xmlPolicy_;
}

template<int array_size, class T, class MT>
I_Member_Bind_Xml<T> * member(MT  T::*mp,  int size)
{
    typedef Member_Array_Ptr_Bind_Xml_Child_Element<T, MT> XmlPolicy;
    typedef Member_Value_Array_Ptr<T, MT> MemberValuePolicy;
    typedef ArrayPtrMemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type(size);
    mph->mvPolicy_.memberPtr_ = mp;
    return &mph->xmlPolicy_;
}

template<int array_size, class T, class MT>
I_Member_Bind_Xml<T> * member_str(MT  (T::*mp) [array_size] )
{
    typedef Member_Str_Bind_Xml_Child_Element<array_size, T, MT> XmlPolicy;
    typedef Member_Value_Array<T, MT[array_size]> MemberValuePolicy;
    typedef MemberHolder<T, MT[array_size], XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberPtr_ = mp;
    return &mph->xmlPolicy_;
}

template<class T, class MT>
I_Member_Bind_Xml<T> * member(MT & (T::*mp)() )
{
    typedef Member_Bind_Xml_Child_Element<T, MT> XmlPolicy;
    typedef Member_Value_Ref_Func<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberRefFunc_ = mp;
    return &mph->xmlPolicy_;
}

template<class T, class MT>
I_Member_Bind_Xml<T> * member(  MT (T::*getter)(), void (T::*setter)(MT) )
{
    typedef Member_Bind_Xml_Child_Element<T, MT> XmlPolicy;
    typedef Member_Value_Func<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
}

template<class T, class MT>
I_Member_Bind_Xml<T> * member(  MT (T::*getter)()const, void (T::*setter)(MT) )
{
    typedef Member_Bind_Xml_Child_Element<T, MT> XmlPolicy;
    typedef Member_Value_Func<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = (MT (T::*)())getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
}

template<class T, class MT>
I_Member_Bind_Xml<T> * member(
    MT const & (T::*getter)(),
    void (T::*setter)(MT const &))
{
    typedef Member_Bind_Xml_Child_Element<T, MT> XmlPolicy;
    typedef Member_Value_Func_Const_Ref<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
}

// BEGIN ATTRIBUTE MAKERS
template<class T, class MT>
I_Member_Bind_Xml<T> * member_attribute(  MT T::*mp )
{
    typedef Member_Bind_Xml_Attribute<T, MT> XmlPolicy;
    typedef Member_Value_Ptr<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberPtr_ = mp;
    return &mph->xmlPolicy_;
}

template<class T, class MT>
I_Member_Bind_Xml<T> * member_attribute(  MT (T::*getter)(), void (T::*setter)(MT) )
{
    typedef Member_Bind_Xml_Attribute<T, MT> XmlPolicy;
    typedef Member_Value_Func<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
}

template<class T, class MT>
I_Member_Bind_Xml<T> * member_attribute(  MT (T::*getter)() const, void (T::*setter)(MT) )
{
    typedef Member_Bind_Xml_Attribute<T, MT> XmlPolicy;
    typedef Member_Value_Func<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = (MT (T::*)())getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
}

template<class T, class MT>
I_Member_Bind_Xml<T> * member_attribute(
    MT const & (T::*getter)(),
    void (T::*setter)(MT const &))
{
    typedef Member_Bind_Xml_Attribute<T, MT> XmlPolicy;
    typedef Member_Value_Func_Const_Ref<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
}
// END ATTRIBUTE MAKERS

// BEGIN PEER MAKERS
template<class T, class MT>
I_Member_Bind_Xml<T> * member_peer(  MT T::*mp )
{
    typedef Member_Bind_Xml_Element<T, MT> XmlPolicy;
    typedef Member_Value_Ptr<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberPtr_ = mp;
    return &mph->xmlPolicy_;
}

template<class T, class MT>
I_Member_Bind_Xml<T> * member_peer(  MT (T::*getter)(), void (T::*setter)(MT) )
{
    typedef Member_Bind_Xml_Element<T, MT> XmlPolicy;
    typedef Member_Value_Func<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
}

template<class T, class MT>
I_Member_Bind_Xml<T> * member_peer(
    MT const & (T::*getter)(),
    void (T::*setter)(MT const &))
{
    typedef Member_Bind_Xml_Element<T, MT> XmlPolicy;
    typedef Member_Value_Func_Const_Ref<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
}

template<class T, class MT>
I_Member_Bind_Xml<T> * member_peer(MT & (T::*mp)() )
{
    typedef Member_Bind_Xml_Element<T, MT> XmlPolicy;
    typedef Member_Value_Ref_Func<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberRefFunc_ = mp;
    return &mph->xmlPolicy_;
}
#endif // MY_MEMBER_H
