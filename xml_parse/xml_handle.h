#ifndef XML_HANDLE_H
#define XML_HANDLE_H

#include"object/mp_logic_table.h"
#include"object/mp_forward_relation.h"
class XML_Handle
{
public:
    XML_Handle();


    static XML_Handle* instance()
    {
        static XML_Handle object;
        return &object;
    }


    bool table_from_xml(unsigned char const *xml_str, int xml_str_len, Table &table);

    bool device_from_xml(unsigned char const *xml_str, int xml_str_len,  Device& device);

    bool table_to_xml(Table *table, unsigned char const *xml_str, int *xml_str_len);

    bool device_to_xml(Device* device,unsigned char const *xml_str,int *xml_str_len);

};

#endif

