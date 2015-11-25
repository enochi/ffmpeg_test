#include "input_form.h"
#include "ui_input_form.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <QMessageBox>
#include "../object/device.h"

Input_Form::Input_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Input_Form)
{
    ui->setupUi(this);
    _device = new Device;
    this->setFixedSize( this->width (),this->height ());

    ui->lineEdit_port->setText("4602");
}

Input_Form::~Input_Form()
{
    delete ui;
    delete _device;
}

void Input_Form::on_pushButton_okay_clicked()
{
    if(ui->lineEdit_ip->text().isEmpty())
    {
        QMessageBox msg_box( QMessageBox::Warning,
                             QString::fromLocal8Bit( "" ),
                             QString::fromLocal8Bit( "ip为空" )
                             );
        msg_box.exec();
        return;
    }
    if(ui->lineEdit_port->text().isEmpty())
    {
        QMessageBox msg_box( QMessageBox::Warning,
                             QString::fromLocal8Bit( "" ),
                             QString::fromLocal8Bit( "port为空" )
                             );
        msg_box.exec();
        return;
    }
    QString ip_str = ui->lineEdit_ip->text();
    _device->from_ip = _device->to_ip =  inet_addr(ip_str.toStdString().c_str());
//    printf("get %s\t%d\n", qPrintable(ip_str), _ip);

    QString port_str = ui->lineEdit_port->text();
    _device->video_port = port_str.toInt();
    _device->audio_port = 4600;
//    printf("get %s\t%d\n", qPrintable(port_str), _device->video_port);

    snprintf(_device->uuid.uuid,
             sizeof(_device->uuid.uuid),
             "uuid_from_%s_%d====================",
             ip_str.toStdString().c_str(),_device->video_port);
    switch(ui->comboBox_resolution->currentIndex())
    {
    case 0:
        _device->from_video_stream_desc.stream_type =
                _device->to_video_stream_desc.stream_type = VIDEO_STREAM_TYPE_CIF;
        break;
    case 1:
        _device->from_video_stream_desc.stream_type =
                _device->to_video_stream_desc.stream_type = VIDEO_STREAM_TYPE_800X600;
        break;
    case 2:
        _device->from_video_stream_desc.stream_type =
                _device->to_video_stream_desc.stream_type = VIDEO_STREAM_TYPE_D1;
        break;
    case 3:
        _device->from_video_stream_desc.stream_type =
                _device->to_video_stream_desc.stream_type = VIDEO_STREAM_TYPE_720p;
        break;
    case 4:
        _device->from_video_stream_desc.stream_type =
                _device->to_video_stream_desc.stream_type = VIDEO_STREAM_TYPE_HD;
        break;
    case 5:
        _device->from_video_stream_desc.stream_type =
                _device->to_video_stream_desc.stream_type = VIDEO_STREAM_TYPE_DCIF;
        break;
    case 6:
        _device->from_video_stream_desc.stream_type =
                _device->to_video_stream_desc.stream_type = VIDEO_STREAM_TYPE_VGA;
        break;
    default:
        break;
    }

    switch (ui->comboBox_devicetype->currentIndex())
    {
    case 0:
        _device->device_type = DEVICE_TYPE_REAL_TERMINAL;
        break;
    case 1:
        _device->device_type = DEVICE_TYPE_MULTIDEV_TERMINAL;
        break;
    case 2:
        _device->device_type = DEVICE_TYPE_VIRTUAL_TERMINAL;
        break;
    default:
        break;
    }

    emit get_device_info();
    this->hide();
}

void Input_Form::on_pushButton_cancel_clicked()
{
    this->hide();
}

Device *Input_Form::get_device()
{
    return _device;
}

bool Input_Form::set_device(Device device)
{
    (*_device) = device;
}
