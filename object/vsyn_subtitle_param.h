#ifndef VSYN_SUBTITLE_PARAM_H
#define VSYN_SUBTITLE_PARAM_H
#include <string.h>
#pragma pack(1)

class Vsyn_Subtitle_Param
{
public:
    unsigned char font;
    unsigned char pt;
    unsigned char dpi;
    unsigned char color;
    unsigned char bgcolor;
    unsigned char transparence;
    unsigned char scrollspeed;
    unsigned short subtitle_x;
    unsigned short subtitle_y;
    unsigned short subtitle_width;
    unsigned short subtitle_height;
    char subtitle[256];

    Vsyn_Subtitle_Param& operator = (Vsyn_Subtitle_Param const& param);

    bool operator == (Vsyn_Subtitle_Param const& param) const;
    bool operator != (Vsyn_Subtitle_Param const& param) const;
};


inline Vsyn_Subtitle_Param& Vsyn_Subtitle_Param::operator = (Vsyn_Subtitle_Param const& param)
{
    font = param.font;
    pt = param.pt;
    dpi = param.dpi;

    color = param.color;
    bgcolor = param.bgcolor;
    transparence = param.transparence;
    scrollspeed = param.scrollspeed;

    subtitle_x = param.subtitle_x;
    subtitle_y = param.subtitle_y;

    subtitle_width = param.subtitle_width;
    subtitle_height = param.subtitle_height;

    memcpy(subtitle, param.subtitle, sizeof(subtitle));

    return *this;
}

inline bool Vsyn_Subtitle_Param::operator == (Vsyn_Subtitle_Param const& param) const
{
    return
            font == param.font &&
            pt == param.pt &&
            dpi == param.dpi &&
            color == param.color &&
            bgcolor == param.bgcolor &&
            transparence == param.transparence  &&
            scrollspeed == param.scrollspeed    &&
            subtitle_x == param.subtitle_x &&
            subtitle_y == param.subtitle_y &&
            subtitle_width == param.subtitle_width &&
            subtitle_height == param.subtitle_height &&
            0 == memcmp(subtitle, param.subtitle, sizeof(subtitle))
            ;
}
inline bool Vsyn_Subtitle_Param::operator != (Vsyn_Subtitle_Param const& param) const
{
    return
            font != param.font ||
            pt != param.pt ||
            dpi != param.dpi ||
            color != param.color ||
            bgcolor != param.bgcolor ||
            transparence != param.transparence ||
            scrollspeed != param.scrollspeed ||
            subtitle_x != param.subtitle_x||
            subtitle_y != param.subtitle_y ||
            subtitle_width != param.subtitle_width ||
            subtitle_height != param.subtitle_height ||
            0 != memcmp(subtitle, param.subtitle, sizeof(subtitle))
            ;
}


enum Subtitle_Scroll_Way
{
    To_None = 0x00,
    To_Left = 0x01,
    To_Top = 0x02,
    To_Right = 0x04,
    To_Bottom = 0x08
};

enum Subtitle_Type
{
    Notice,
    //Mark,
    Conference_Subtitle,
    Mark,
    Subtitle_Type_Max
};

class Vsyn_OOB_Subtitle
{
public:
    Subtitle_Type       type;
    int                 subtitle_x;
    int                 subtitle_y;
    int                 subtitle_width;
    int                 subtitle_height;
    int                 scroll_speed;
    Subtitle_Scroll_Way scroll_way;
    int                 png_width;
    int                 png_height;
    int                 png_size;
    char                png[0];

    Vsyn_OOB_Subtitle() : subtitle_x(0),
        subtitle_y(0),
        subtitle_width(0),
        subtitle_height(0),
        scroll_speed(0),
        scroll_way(To_None),
        png_width(0),
        png_height(0),
        png_size(0)
    {
    }
};

#pragma pack()


#endif
