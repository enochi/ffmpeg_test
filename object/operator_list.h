#ifndef OPERATOR_LIST_H
#define OPERATOR_LIST_H
#include <list>

template <typename T>
bool operator == (std::list<T> const &l1, std::list<T> const &l2)
{
    unsigned int target = 0;

    if(l1.size() != l2.size())
    {
        return false;
    }
    else
    {
        typename std::list<T>::const_iterator l1_iter = l1.begin();

        for( ; l1_iter != l1.end(); l1_iter++ )
        {
            typename std::list<T>::const_iterator l2_iter = l2.begin();
            for( ; l2_iter != l2.end(); l2_iter++ )
            {
                T const& t1 = *l1_iter;
                T const& t2 = *l2_iter;
                //if( *l1_iter == *l2_iter )
                if(t1 == t2)
                {
                    target++;
                    break;
                }
            }
        }

    }

    if( target == l1.size() )
        return true;

    return false;
}
#endif // OPERATOR_LIST_H
