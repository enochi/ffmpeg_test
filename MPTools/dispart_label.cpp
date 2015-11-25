#include "dispart_label.h"
#include <QMenu>
#include <QAction>
#include <cstdio>
#include <arpa/inet.h>
#include "../object/device.h"

Dispart_Label::Dispart_Label(const QString &text, QWidget *parent, Qt::WindowFlags f):QLabel(text), _num(-1)
{
    setFrameStyle(QFrame::Panel + QFrame::Sunken);
    setAlignment(Qt::AlignCenter);
    adjustSize();
    setGeometry(QRect(328, 240, 329, 27*4));  //四倍行距
    setWordWrap(true);
//    setAlignment(Qt::AlignTop);

    _menu = new QMenu(this);
    _set_device = new QAction(tr("&set device"), _menu);
    _menu->addAction(_set_device);
    _clear_device = new QAction(tr("&clear"), _menu);
    _menu->addAction(_clear_device);

    _input_form = new Input_Form;

    connect(_set_device, SIGNAL(triggered()), this, SLOT(set_device_slot()));
    connect(_clear_device, SIGNAL(triggered()), this, SLOT(clear_device_slot()));
    connect(_input_form, SIGNAL(get_device_info()), this, SLOT(save_device_info()));
}

Dispart_Label::~Dispart_Label()
{
    if(_menu)
    {
        delete _menu;
    }
//    if(_set_device) // error
//    {
//        delete _set_device;
//    }
    if(_input_form)
    {
        delete _input_form;
    }
}

void Dispart_Label::contextMenuEvent(QContextMenuEvent *event)
{
//    menu->addAction(new QAction(QIcon(":/images/mark.png"), tr("&Mark"), menu));
    _menu->move(cursor().pos()); //让菜单显示的位置在鼠标的坐标上
    _menu->show();
}

void Dispart_Label::set_device_slot()
{
    _input_form->setWindowModality(Qt::ApplicationModal);
    _input_form->show();
}

void Dispart_Label::save_device_info()
{
//    printf("save device info! \n");
    Device device;
    device = *(_input_form->get_device());
    char xxx[128] = "";
    inet_ntop(AF_INET, &device.from_ip, xxx, sizeof(xxx));
    this->setText(QString(xxx));

}

void Dispart_Label::clear_device_slot()
{
    Device device;
    _input_form->set_device(device);
    char tmp[10] = "";
    sprintf(tmp, "%d", _num);
    this->setText(tmp);
}
