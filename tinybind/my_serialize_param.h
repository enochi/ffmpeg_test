#ifndef MY_SERIALIZE_PARAM_H
#define MY_SERIALIZE_PARAM_H

class Tag
{
public:
    // support up to 3 tags
    char const * tag_[3];


    Tag( char const * tagOne = NULL ) {
        tag_[0] = tagOne;
        tag_[1] = NULL;
        tag_[2] = NULL;
    }
    Tag( char const * tagOne, char const * tagTwo ) {
        tag_[0] = tagOne;
        tag_[1] = tagTwo;
        tag_[2] = NULL;
    }
    Tag( char const * tagOne, char const * tagTwo, char const * tagThree ) {
        tag_[0] = tagOne;
        tag_[1] = tagTwo;
        tag_[2] = tagThree;
    }
};

class Serialize_Params
{
public:
    Tag tag_;
};
#endif // MY_SERIALIZE_PARAM_H
