#ifndef VSYN_LAYOUT_H
#define VSYN_LAYOUT_H



#pragma pack(1)

class Vsyn_Layout
{
public:
    unsigned short left;
    unsigned short top;
    unsigned short right;
    unsigned short bottom;
};


inline bool operator == ( Vsyn_Layout const& a, Vsyn_Layout const& b )
{
    return (a.left == b.left&&
            a.top == b.top&&
            a.right == b.right&&
            a.bottom == b.bottom);
}
#pragma pack()

#endif
