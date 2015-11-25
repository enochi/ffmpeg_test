#include "relation_hand.h"
#include "dispart_label.h"
#include <QMessageBox>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include "../object/device.h"
#include "../object/operator_relation.h"
#include "common/protocol_head.h"
#include "common/my_ace_client.h"

Relation_Hand::Relation_Hand(): _list_widget(NULL), _layout(NULL)
{
}

Relation_Hand::~Relation_Hand()
{
    for(std::unordered_map<int, Dispart_Label*>::iterator iter = _dispart_label_map.begin();iter!=_dispart_label_map.end();++iter)
    {
        delete iter->second;
    }
    _dispart_label_map.clear();

    std::unordered_map<std::string, Device*>::const_iterator del_iter = _item_string_device_map.begin();
    for( ; del_iter!=_item_string_device_map.end(); ++del_iter)
    {
        delete del_iter->second;
    }
    _item_string_device_map.clear();
}

void Relation_Hand::init(QListWidget * list_widget, QGridLayout *layout, Input_Form * input_form, QComboBox *combobox, QLineEdit *lineedit)
{
    _list_widget = list_widget;
    _layout = layout;
    _input_form = input_form;
    _combobox = combobox;
    _lineedit = lineedit;

    _layout->setSpacing(2);
    _layout->setMargin(2);

    int n = 0;
    for(int i=0; i<4; ++i)
    {
        for(int j=0; j<4; ++j)
        {
            char tmp[10] = "";
            sprintf(tmp, "%d", n);
            Dispart_Label * label = new Dispart_Label(tmp);
            label->set_num(n);
//            _layout->addWidget(label, i, j);
            _dispart_label_map[n++] = label;
        }
    }

    if(0 !=init_from_file())
    {
        set_dispart_label(1, 2);
    }
    printf("%d==============\n", _layout->count());
}

int Relation_Hand::set_dispart_label(int row, int column)
{
//    for(auto iter = _dispart_label_map.begin();iter!=_dispart_label_map.end();++iter)
//    {
//        delete iter->second;
//    }
//    _dispart_label_map.clear();
    while(_layout->count() > 0)
    {
        QWidget* widget = _layout->itemAt(0)->widget();
        _layout->removeItem(_layout->itemAt(0));
        widget->hide();
    }

    int n = 0;
    for(int i=0; i<row; ++i)
    {
        for(int j=0; j<column; ++j)
        {
            _layout->addWidget(_dispart_label_map[n], i, j);
            Device device;
            device = *(_dispart_label_map[n]->get_form()->get_device());
            if(device.from_ip != 0x0100007f)
            {
                char xxx[128] = "";
                inet_ntop(AF_INET, &(device.from_ip), xxx, sizeof(xxx));
                _dispart_label_map[n]->setText(QString(xxx));
            }
            _dispart_label_map[n]->show();
            n++;
        }
    }
    return 0;
}


int Relation_Hand::set_layout(int index)
{
    switch (index)
    {
    case 0://2
//        printf("case 0 index %d\n", index);
        set_dispart_label(1, 2);
        break;
    case 1://4
//        printf("case 1 index %d\n", index);
        set_dispart_label(2, 2);
        break;
    case 2://6
//        printf("case 2 index %d\n", index);
        set_dispart_label(2, 3);
        break;
    case 3://8
//        printf("case 3 index %d\n", index);
        set_dispart_label(2, 4);
        break;
    case 4://9
//        printf("case 4 index %d\n", index);
        set_dispart_label(3, 3);
        break;
    case 5://16
//        printf("case 5 index %d\n", index);
        set_dispart_label(4, 4);
        break;
    default:
        break;
    }
    return 0;
}

int Relation_Hand::get_device_layout(Relation &video_relation, int mp_ip, int rows, int columns)
{
    Device_Location location;
    for(int i=0; i<_layout->count(); ++i)
    {
        Device device;
        device = *(_dispart_label_map[i]->get_form()->get_device());
        device.mp_id.mp_id = mp_ip;
        if(device.from_ip == 0x0100007f)
        {
            continue;
        }
        video_relation.from_to.from.push_back(device);
        location.device = device;
        int row = i/columns;
        int column = i%columns;
        location.vsyn_layout.left = column * (100/columns);
        location.vsyn_layout.top = row * (100/rows);
        location.vsyn_layout.right = location.vsyn_layout.left +100/columns;
        location.vsyn_layout.bottom = location.vsyn_layout.top + 100/rows;
        printf("%s left %d, top %d, right %d, botton %d\n",
               inet_ntoa(( *( struct in_addr * )(&device.from_ip))),
               location.vsyn_layout.left,
               location.vsyn_layout.top,
               location.vsyn_layout.right,
               location.vsyn_layout.bottom);
        video_relation.vsyn_layouts.push_back(location);
    }
    return 0;
}

int Relation_Hand::save_send_device_info()
{
    Device *device = new Device;
    *device = *(_input_form->get_device());
    printf("%d\n", device->from_ip);

    char xxx[128] = "";
    inet_ntop(AF_INET, &(device->from_ip), xxx, sizeof(xxx));
    std::string item(xxx);
    std::unordered_map<std::string, Device*>::const_iterator iter = _item_string_device_map.find(item);
    if(iter != _item_string_device_map.end())
    {
        return -1;
    }
    _item_string_device_map[item] = device;
    _list_widget->addItem(item.c_str());
    return 0;
}

void Relation_Hand::list_del_item(QList<QListWidgetItem *> select)
{

    QListWidgetItem* del=select[0];
//  printf("%s\n", del->text().toStdString().c_str());

    delete _item_string_device_map[del->text().toStdString()];
    _item_string_device_map.erase(del->text().toStdString());

    if(del)
    {
        int del_row = _list_widget->row(del);
        delete _list_widget->takeItem(del_row);
    }

//    printf("now , num is %d\n", _item_string_device_map.size());
}

void Relation_Hand::list_clear()
{
    std::unordered_map<std::string, Device*>::const_iterator del_iter = _item_string_device_map.begin();
    for( ; del_iter!=_item_string_device_map.end(); ++del_iter)
    {
        delete del_iter->second;
    }
    _item_string_device_map.clear();
    _list_widget->clear();
/*    int now_count = _list_widget->count();
//    printf("all is %d\n", now_count);
    while(now_count > 0)
    {
        delete _list_widget->takeItem(now_count-1);
//        printf("del row %d\n", now_count-1);
        --now_count;
    }*/
}

void Relation_Hand::stop_mp(QString ip_str)
{
    table.audio_relation.clear();
    table.video_relation.clear();
    My_Ace_Client my_ace_client;
     int mp_ip = inet_addr(ip_str.toStdString().c_str());
   if(my_ace_client.init(table, mp_ip))
   {
    my_ace_client();
   }
}
void Relation_Hand::send_relation(QString ip_str, int index)
{
    int mp_ip = inet_addr(ip_str.toStdString().c_str());
    table.audio_relation.clear();
    table.video_relation.clear();
    Relation video_relation;
    video_relation.mp_id.mp_id = mp_ip;
    video_relation.confid.id = mp_ip;
    snprintf(video_relation.confid.uuid.uuid,
             36,
             "%s========================================",
             ip_str.toStdString().c_str());
    video_relation.relation_id.relation_id = 1;
    std::unordered_map<std::string, Device*>::iterator iter_to = _item_string_device_map.begin();
    for( ; iter_to!=_item_string_device_map.end(); ++iter_to)
    {
        Device device;
        device = *(iter_to->second);
        device.mp_id.mp_id = mp_ip;
        video_relation.from_to.to.push_back(device);
    }

    switch(index)
    {
    case 0://2
        get_device_layout(video_relation, mp_ip, 1, 2);
        break;
    case 1://4
        get_device_layout(video_relation, mp_ip, 2, 2);
        break;
    case 2://6
        get_device_layout(video_relation, mp_ip, 2, 3);
        break;
    case 3://8
        get_device_layout(video_relation, mp_ip, 2, 4);
        break;
    case 4://9
        get_device_layout(video_relation, mp_ip, 3, 3);
        break;
    case 5://16
        get_device_layout(video_relation, mp_ip, 4, 4);
        break;
    default:
        break;
    }

    table.video_relation.push_back(video_relation);
    table.audio_relation.push_back(video_relation);

    My_Ace_Client my_ace_client;
   if(my_ace_client.init(table, mp_ip))
   {
    my_ace_client();
   }
}

int Relation_Hand::row_colum_from_layout(Vsyn_Layout layout, unsigned int &num)
{
    int row = 100 /  (layout.bottom - layout.top);
    int column = 100 / (layout.right - layout.left);

    int r = layout.bottom / (100 / row);
    int c = layout.right / (100 / column);

    num = r*c - 1;

    return row*column;
}

int Relation_Hand::init_from_file()
{
    int file_fd = open("table.txt", O_RDONLY);
    if(file_fd < 0)
    {
        perror("open table.txt\n");
        return -1;
    }

    struct stat buf;
    if(stat("table.txt", &buf)<0)
    {
        return -1;
    }
    unsigned char * str = new unsigned char[buf.st_size];
    read(file_fd, str, buf.st_size);

    Table table;
    int xml_len = buf.st_size;
    XML_Handle *xml_handle = XML_Handle::instance();
    xml_handle->table_from_xml(str, xml_len, table);

    int layout_num = 0;//table.video_relation.begin()->from_to.from.size();

    list<Device_Location>::iterator iter_layout = table.video_relation.begin()->vsyn_layouts.begin();
    unsigned int n = 0;
    for( ; iter_layout!=table.video_relation.begin()->vsyn_layouts.end(); ++iter_layout)
    {
        layout_num = row_colum_from_layout(iter_layout->vsyn_layout, n);
        _dispart_label_map[n]->get_form()->set_device((*iter_layout).device);
//        if(iter_layout_device->from_ip != 0x0100007f)
//        {
//            char xxx[128] = "";
//            inet_ntop(AF_INET, &(iter_layout_device->from_ip), xxx, sizeof(xxx));
//            _dispart_label_map[n]->setText(QString(xxx));
//            n++;
//        }
    }

    char xxx[128] = "";
    inet_ntop(AF_INET, &(table.video_relation.begin()->mp_id.mp_id), xxx, sizeof(xxx));
    _lineedit->setText(QString(xxx));

    switch(layout_num)
    {
    case 2://2
        _combobox->setCurrentIndex(0);
        set_layout(0);
        break;
    case 4://4
        _combobox->setCurrentIndex(1);
        set_layout(1);
        break;
    case 6://6
        _combobox->setCurrentIndex(2);
        set_layout(2);
        break;
    case 8://8
        _combobox->setCurrentIndex(3);
        set_layout(3);
        break;
    case 9://9
        _combobox->setCurrentIndex(4);
        set_layout(4);
        break;
    case 16://16
        _combobox->setCurrentIndex(5);
        set_layout(5);
        break;
    default:
        break;
    }

//    list<Device>::reverse_iterator iter_layout_device = table.video_relation.begin()->from_to.from.rbegin();
//    std::unordered_map<int, Dispart_Label*>::iterator iter_map = _dispart_label_map.begin();
//    for( ; iter_layout_device!=table.video_relation.begin()->from_to.from.rend()&&iter_map!=_dispart_label_map.end(); ++iter_layout_device, ++iter_map)
//    {
//        iter_map->second->get_form()->set_device(*iter_layout_device);
//        if(iter_layout_device->from_ip != 0x0100007f)
//        {
//            char xxx[128] = "";
//            inet_ntop(AF_INET, &(iter_layout_device->from_ip), xxx, sizeof(xxx));
//            iter_map->second->setText(QString(xxx));
//        }
//    }
//    list<Device>::iterator iter_layout_device = table.video_relation.begin()->from_to.from.begin();
//    unsigned int n = 0;
//    for( ; iter_layout_device!=table.video_relation.begin()->from_to.from.end()&&n<_dispart_label_map.size(); ++iter_layout_device)
//    {
//        _dispart_label_map[n]->get_form()->set_device(*iter_layout_device);
//        if(iter_layout_device->from_ip != 0x0100007f)
//        {
//            char xxx[128] = "";
//            inet_ntop(AF_INET, &(iter_layout_device->from_ip), xxx, sizeof(xxx));
//            _dispart_label_map[n]->setText(QString(xxx));
//            n++;
//        }
//    }

    list<Device>::iterator iter_list_device = table.video_relation.begin()->from_to.to.begin();
    for( ; iter_list_device!=table.video_relation.begin()->from_to.to.end(); ++iter_list_device)
    {
        Device *device = new Device;
        *device = *iter_list_device;
        char xxx[128] = "";
        inet_ntop(AF_INET, &(iter_list_device->from_ip), xxx, sizeof(xxx));
        std::string item(xxx);
        _item_string_device_map[item] = device;
        _list_widget->addItem(item.c_str());
    }

    if(str)
    {
        delete []str;
        str = NULL;
    }

    return 0;
}
