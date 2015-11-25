#ifndef MY_MEMBER_VALUE_H
#define MY_MEMBER_VALUE_H

template<class T, class MT>
class I_Member_Value_Policy
{
public:
    virtual MT const & get_member_value( T const * thisPtr ) = 0;
    virtual void set_member_value( T * thisPtr, MT const & mv ) = 0;
};


template<class T, class MT>
class Member_Value_Func : public I_Member_Value_Policy<T, MT>
{
public:
    MT (T::*getter_)();
    void (T::*setter_)(MT);

    virtual MT const & get_member_value( T const * thisPtr ) {
        static MT mv;
        mv = (const_cast<T*>(thisPtr)->*getter_)();
        return mv;
    }

    virtual void set_member_value( T * thisPtr, MT const & mv ) {
        (thisPtr->*setter_)(mv);
    }

};

template<class T, class MT>
class Member_Value_Func_Const_Ref  : public I_Member_Value_Policy<T, MT>
{
public:
    MT const & (T::*getter_)();
    void (T::*setter_)(MT const &);

    virtual MT const & get_member_value( T const * thisPtr ) {
        return (thisPtr->*getter_)();
    }

    virtual void set_member_value( T * thisPtr, MT const & mv ) {
        (thisPtr->*setter_)(mv);
    }
};

template<class T, class MT>
class Member_Value_Ptr : public I_Member_Value_Policy<T, MT>
{
public:
    MT T::*memberPtr_;
    virtual MT const & get_member_value( T const * thisPtr ) { return thisPtr->*memberPtr_; }
    virtual void set_member_value( T * thisPtr, MT const & mv ) {
        // by casting away const here, we can support member pointers to arrays
        thisPtr->*memberPtr_ = const_cast<MT &>(mv);
    }
};

template <class T, class MT>
class Member_Value_Array : public I_Member_Value_Policy<T, MT>
{
public:
    MT T::*memberPtr_;
    virtual MT const & get_member_value( T const * thisPtr ) { return thisPtr->*memberPtr_; }
    virtual void set_member_value( T * thisPtr, MT const & mv ) {
        // by casting away const here, we can support member pointers to arrays
        //thisPtr->*memberPtr_ = const_cast<MT &>(mv);
        MT &a = thisPtr->*memberPtr_;
        //memcpy(&a, &mv, sizeof(MT));
		memset(&a, 0, sizeof(a));
        for(int i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
        {
            a[i] = mv[i];
        }
    }
};

template <class T, class MT>
class Member_Value_Array_Ptr : public I_Member_Value_Policy<T, MT>
{
public:
    Member_Value_Array_Ptr(int _size) : size(_size){}
    MT T::*memberPtr_;
    virtual MT const & get_member_value( T const * thisPtr ) { return thisPtr->*memberPtr_; }
    virtual void set_member_value( T * thisPtr, MT const & mv ) {
        // by casting away const here, we can support member pointers to arrays
        //thisPtr->*memberPtr_ = const_cast<MT &>(mv);
        MT &a = thisPtr->*memberPtr_;
        //memcpy(&a, &mv, sizeof(MT));
        memset(&a, 0, sizeof(*a) * size );
        for(int i = 0; i < size; ++i)
        {
            a[i] = mv[i];
        }
    }
    int size;
};

template<class T, class MT>
class Member_Value_Ref_Func : public I_Member_Value_Policy<T, MT>
{
public:
    MT & (T::*memberRefFunc_)();
    virtual MT const & get_member_value( T const * thisPtr ) { return (const_cast<T*>(thisPtr)->*memberRefFunc_)(); }
    virtual void set_member_value( T * thisPtr, MT const & mv ) {
        (thisPtr->*memberRefFunc_)() = mv;
    }
};
#endif // MY_MEMBER_VALUE_H
