#ifndef RELATION_HAND_H
#define RELATION_HAND_H

#include <QGridLayout>
#include <unordered_map>
#include <QListWidget>
#include <QComboBox>
#include <QLineEdit>
#include "../object/device.h"
#include "../object/operator_relation.h"
#include "xml_parse/xml_handle.h"
class Dispart_Label;
class Relation;
class Device;
class Input_Form;
class Vsyn_Layout;
class Relation_Hand
{
public:
    Relation_Hand();
    ~Relation_Hand();

    void init(QListWidget * list_widget, QGridLayout *layout, Input_Form *input_form, QComboBox *combobox, QLineEdit *lineedit);

    int set_dispart_label(int row, int column);

    int set_layout(int index);

    int get_device_layout(Relation &video_relation, int mp_ip, int rows, int columns);

    int save_send_device_info();

    void list_del_item(QList<QListWidgetItem *> select);

    void list_clear();

    void send_relation(QString ip_str, int index);

    int row_colum_from_layout(Vsyn_Layout layout, unsigned int &num);
    void stop_mp(QString ip_str);
    int init_from_file();
         Table table;
private:
    std::unordered_map<int, Dispart_Label*> _dispart_label_map;
    std::unordered_map<std::string, Device*> _item_string_device_map;
    QListWidget *_list_widget;
    QGridLayout *_layout;
    Input_Form *_input_form;
    QComboBox *_combobox;
    QLineEdit *_lineedit;

};

#endif // RELATION_HAND_H
