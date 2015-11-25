#ifndef FORWARD_RELATION_H
#define FORWARD_RELATION_H

#include <list>
#include<iostream>
using namespace std;

#include "device.h"


#pragma pack(1)

class MP_Forward_Relation
{
public:
    Device      device;
    list<Device> device_list;
};

typedef list<MP_Forward_Relation> Video_Forward_Table;
typedef list<MP_Forward_Relation> Audio_Forward_Table;

class Video_Forward
{
public:
    Video_Forward_Table video_forward;
    Video_Forward_Table qos_video_forward;

};


class Audio_Forward
{
public:
    Audio_Forward_Table audio_forward;
    Audio_Forward_Table qos_audio_forward;

};

#include "operator_forward_relation.h"

#pragma pack()


#endif
