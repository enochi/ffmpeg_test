#ifndef DISPART_LABEL_H
#define DISPART_LABEL_H

#include <QLabel>
#include "input_form.h"
class QMenu;

class Dispart_Label : public QLabel
{
    Q_OBJECT
public:
    Dispart_Label(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);
    ~Dispart_Label();
    void contextMenuEvent(QContextMenuEvent *event);
    Input_Form *get_form()
    {
        return _input_form;
    }

    void set_num(int num)
    {
        _num = num;
    }

public slots:
    void set_device_slot();
    void save_device_info();
    void clear_device_slot();
private:
    QMenu *_menu;
    QAction *_set_device;
    QAction *_clear_device;
    Input_Form *_input_form;

    int _num;
};

#endif // DISPART_LABEL_H
