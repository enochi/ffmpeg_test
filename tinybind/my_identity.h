#ifndef MY_IDENTITY_H
#define MY_IDENTITY_H

class Identity_Base
{
};

template< class T >
class Identity : public Identity_Base
{
public:
    typedef T type;

};

#endif // MY_IDENTITY_H
