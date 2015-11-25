#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <functional>
#include <QWidget>

#include <QtGui>
#include <QMessageBox>
#include <QFileDialog>
#include "../qt_yuv_widget/yuv_widget.h"
#include "dispart_label.h"
#include "../object/device.h"
#include "../object/mp_logic_table.h"
#include "common/protocol_head.h"
#include "common/my_ace_client.h"
#include "xml_handle.h"
#include <QRadioButton>
#include <QCheckBox>
#include "dialog_encode_param.h"
#include "common/video_stream_description.h"
#include <QComboBox>
using namespace std::placeholders;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->frame->setContextMenuPolicy(Qt::CustomContextMenu);
    _main_menu = new QMenu("菜单",this);
    QAction *action_save = new QAction("&SaveFile",_main_menu);
    QAction *action_stop = new QAction("&Stop",_main_menu);
    QAction *action_quit = new QAction("&Quit",_main_menu);

    connect(action_save,&QAction::triggered,this,&MainWindow::slot_save_file);
    connect(action_stop,&QAction::triggered,this,&MainWindow::slot_stop_save_file);
    connect(action_quit,&QAction::triggered,this,&MainWindow::close);

    _main_menu->addAction(action_quit);

    _video_menu = new QMenu(this);
    _video_menu->addAction(action_save);
    _video_menu->addAction(action_stop);
    _yuv_widget_display = new YUV_Widget(/*ui->frame*/);
    //    _yuv_widget_display->s
    _yuv_widget_card = new YUV_Widget(/*ui->widget_card*/);

    on_yuv_display = std::bind(&YUV_Widget::on_yuv_data_arrive,_yuv_widget_display,_1,_2,_3,_4);
    on_yuv_card = std::bind(&YUV_Widget::on_yuv_data_arrive,_yuv_widget_card,_1,_2,_3,_4);

    _video_layout = new QGridLayout(ui->frame);
    _video_layout->addWidget(_yuv_widget_display);

    QGridLayout *card_video_layout = new QGridLayout(ui->widget_card);
    card_video_layout->addWidget(_yuv_widget_card);

    memset(&_open_device,0,sizeof(_open_device));
    _open_device.type = 2;

    Video_Stream_Description desc(VIDEO_STREAM_TYPE_X265_1920X1080);
    _open_device._encode_param.codec_id = desc.codec_id();
    _open_device._encode_param.w = desc.width();
    _open_device._encode_param.h = desc.height();
    _open_device._encode_param.bitrate = desc.bitrate();
    _open_device._encode_param.fps = desc.fps();
    _open_device._encode_param.gop = desc.gop_len();

    FUNC_NOTICE fun = std::bind(&MainWindow::func_notice,this,_1,_2);
    fprintf(stderr,"111\n");
    _nalu_sender.init(&on_yuv_card);
     fprintf(stderr,"112\n");
    _nalu_sender.enable_qos_handler(4603);
     fprintf(stderr,"113\n");
    _nalu_receiver.init(&on_yuv_display,fun);
     fprintf(stderr,"114\n");
    ui->pushButton_stop_display->setEnabled(false);
    ui->pushButton_forward_stop->setEnabled(false);
    _remote_label = new QLabel("0.0.0.0");
    statusBar()->addPermanentWidget(_remote_label);
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),  this, SLOT(time_out()));
    timer->start( 10*1000);

    connect(ui->buttonGroup , SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(on_buttonGroup_button_clicked(QAbstractButton*)));

    /**/
    _input_send_info = new Input_Form();
    _dispart_layout = new QGridLayout(ui->frame_dispart);
    hand.init(ui->listWidget_sendgroup, _dispart_layout, _input_send_info, ui->comboBox, ui->lineEdit_mp_ip);
    connect(_input_send_info, SIGNAL(get_device_info()), this, SLOT(save_send_device_info()));
    _dialog_encode_param = new Dialog_Encode_Param(this);
    _dialog_encode_param->hide();


}
 int MainWindow::slot_set_encode_param(T_encode_param *param)
 {
     printf("slot_set_encode_param method:%d\n ",param->method);
     return 0;
 }
void MainWindow::time_out()
{
    char msg[1024]={0};
    T_nalu_count nalu_count_send;
    T_nalu_count nalu_count_reencode_send;
    T_nalu_count nalu_count_recv;

    _nalu_sender._sender.get_nalu_count(nalu_count_send);
//    _nalu_receiver._resend_handler.get_nalu_count(nalu_count_recv);
    snprintf(msg,sizeof(msg),"receive bitrate:%dkbps,fps:%d,encode bitrate:%dkbps,fps:%d"
             ,nalu_count_recv._bitrate,nalu_count_recv._fps
             ,nalu_count_send._bitrate,nalu_count_send._fps);

    if(_nalu_receiver._reencode)
    {
        _nalu_receiver._resend_send.get_nalu_count(nalu_count_reencode_send);
        snprintf(msg+strlen(msg),sizeof(msg)," reencode bitrate:%d,fps:%d"
                 ,nalu_count_reencode_send._bitrate,nalu_count_reencode_send._fps);

    }
    statusBar()->showMessage(msg);

}
MainWindow::~MainWindow()
{
    delete ui;
    if(_dispart_layout)
    {
        delete _dispart_layout;
    }
    if(_input_send_info)
    {
        delete _input_send_info;
    }

}
int MainWindow::init(int argc ,char *argv[])
{

    return 0;

}

void MainWindow::on_tabwidget_currentChanged(int index)
{
    printf("change to index:%d\n",index);
}

void MainWindow::on_frame_customContextMenuRequested(const QPoint &pos)
{
    _video_menu->exec(QCursor::pos());

}
void MainWindow::slot_save_file()
{
    QString savefile = QFileDialog::getSaveFileName(this,"save file",".");

    if(!savefile.isEmpty())
    {
        printf("get save file:%s\n",savefile.toStdString().c_str());
        _nalu_receiver.set_save_file_name(savefile.toStdString());
        QString tmp;
        tmp.sprintf("save video to :%s",savefile.toStdString().c_str());
        statusBar()->showMessage(tmp);
    }
}
void MainWindow::slot_stop_save_file()
{
    std::string empty;
    _nalu_receiver.set_save_file_name(empty);
    statusBar()->clearMessage();

}
void MainWindow::on_pushButton_cmd_clicked()
{

}

void MainWindow::on_pushButton_select_file_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,"open file",".");
    printf("get send file:%s\n",file.toStdString().c_str());
    if(!file.isEmpty())
    {
        ui->lineEdit_file->setText(file);
    }

}
int MainWindow::func_notice(int type,const std::string &msg)
{
    printf("recv notic type:%d,msg:%s\n",type,msg.c_str());
    if(0 == type)
    {
        _remote_label->setText(msg.c_str());
    }
}

void MainWindow::on_pushButton_begin_display_clicked()
{


    int recv_video_port = ui->lineEdit_port->text().toInt();
    bool x265  = ui->checkBox_x265->isChecked();
   // x265= _open_device._encode_param.codec_id==CODECID_H265?true:false;
    bool ret_codec=_nalu_receiver.init_codec(x265);
    int ret =_nalu_receiver.begin_recv(recv_video_port);
    if(!ret_codec || ret != 0)
    {
        QMessageBox msg;
        msg.setText("_nalu_receiver.init error\n");
        msg.exec();
        return ;
    }

    ui->pushButton_begin_display->setEnabled(false);
    ui->pushButton_stop_display->setEnabled(true);


}

void MainWindow::on_pushButton_stop_display_clicked()
{
    _nalu_receiver.stop();
    ui->pushButton_begin_display->setEnabled(true);
    ui->pushButton_stop_display->setEnabled(false);
}

void MainWindow::on_pushButton_start_send_file_clicked()
{
     QString sendaddrs = ui->lineEdit_send_addrs->text();
    if(_open_device.type == 0)
    {
        QString file = ui->lineEdit_file->text();

        if(file.isEmpty() || sendaddrs.isEmpty())
        {
            QMessageBox msg;
            msg.setText("please set send file and send addrs");
            msg.exec();
            return ;
        }
        strncpy(_open_device.device_name,file.toStdString().c_str(),sizeof(_open_device.device_name));
    }
    QString cur = ui->comboBox_video_type->currentText();
   on_comboBox_video_type_currentIndexChanged(cur);
    int ret =  _nalu_sender.set_open_device(_open_device);
    if(ret != 0)
    {
        QMessageBox msg;
        msg.setText("open file or device failed");
        msg.exec();
        return ;
    }

    QStringList sendlist = sendaddrs.split(";");

    std::set<std::string> send_list_v;
    for (QStringList::const_iterator constIterator = sendlist.constBegin();
         constIterator != sendlist.constEnd();
         ++constIterator)
    {
        send_list_v.insert((*constIterator).toStdString());
    }
    _nalu_sender.set_qos_send_addrs(send_list_v);
    _nalu_sender.start();
    ui->pushButton_start_send_file->setEnabled(false);
    ui->pushButton_stop_send_file->setEnabled(true);
    _dialog_encode_param->encode_start(true);
}

void MainWindow::on_pushButton_stop_send_file_clicked()
{
    _nalu_sender.stop();
    ui->pushButton_start_send_file->setEnabled(true);
    ui->pushButton_stop_send_file->setEnabled(false);
     _dialog_encode_param->encode_start(false);
}

void MainWindow::on_pushButton_forward_clicked()
{
    QString sendaddrs = ui->lineEdit_forward_addrs->text();
    if(sendaddrs.isEmpty())
    {
        QMessageBox msg;
        msg.setText("please enter send addrs as:ip:port");
        msg.exec();
        return ;
    }
    QStringList sendlist = sendaddrs.split(";");

    std::set<std::string> send_list_v;
    for (QStringList::const_iterator constIterator = sendlist.constBegin();
         constIterator != sendlist.constEnd();
         ++constIterator)
    {
        send_list_v.insert((*constIterator).toStdString());
    }
    _nalu_receiver.set_forward_addrs(send_list_v);
    ui->pushButton_forward_stop->setEnabled(true);

}

void MainWindow::on_pushButton_forward_stop_clicked()
{
    std::set<std::string> send_list_v;
    _nalu_receiver.set_forward_addrs(send_list_v);
    ui->pushButton_forward_stop->setEnabled(false);
     ui->pushButton_forward_start->setEnabled(true);
}
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    hand.set_layout(index);
    printf("layout number %d\n", _dispart_layout->count());
}

void MainWindow::on_pushButton_add_clicked()
{
    _input_send_info->setWindowModality(Qt::ApplicationModal);
    _input_send_info->show();
}
void MainWindow::on_buttonGroup_button_clicked(QAbstractButton * button)
{
    printf("on_buttonGroup_buttonClicked button name:%s\n",button->objectName().toStdString().c_str());

    //判断QButtonGroup中哪个QRadioButton被选中，通过对象名称去判断

    if(QString::compare(button->objectName(), "radioButton_capture_screen") == 0)
    {
        _open_device.type = 1;
        printf("set capture screen\n");
    }
    else if(QString::compare(button->objectName(), "radioButton_capture_camera")==0)

    {
        _open_device.type = 2;
        printf("set capture camera\n");
    }
    else if(QString::compare(button->objectName(), "radioButton_capture_file")==0)

    {
        _open_device.type = 0;
        printf("set capture file\n");
    }

}
void MainWindow::save_send_device_info()
{
    if(-1 == hand.save_send_device_info())
    {
        QMessageBox msg_box( QMessageBox::Warning,
                             QString::fromLocal8Bit( "" ),
                             QString::fromLocal8Bit( "不可重复添加同一ip" )
                             );
        msg_box.exec();
        return;
    }
}

void MainWindow::on_pushButton_del_clicked()
{
    QList<QListWidgetItem *> select = ui->listWidget_sendgroup->selectedItems();
    if(select.size() == 0)
    {
        QMessageBox msg_box( QMessageBox::Warning,
                             QString::fromLocal8Bit( "" ),
                             QString::fromLocal8Bit( "请选择一个item删除" )
                             );
        msg_box.exec();
        return;
    }
    hand.list_del_item(select);
    //    printf("now , num is %d\n", _item_string_device_map.size());
}

void MainWindow::on_pushButton_clear_clicked()
{
    hand.list_clear();
}

void MainWindow::on_pushButton_cmd_mp_clicked()
{
    if(ui->listWidget_sendgroup->count() <= 0)
    {
        QMessageBox msg_box( QMessageBox::Warning,
                             QString::fromLocal8Bit( "" ),
                             QString::fromLocal8Bit( "请添加信息" )
                             );
        msg_box.exec();
        return;
    }
    if(ui->lineEdit_mp_ip->text().isEmpty())
    {
        QMessageBox msg_box( QMessageBox::Warning,
                             QString::fromLocal8Bit( "" ),
                             QString::fromLocal8Bit( "请输入mp的ip" )
                             );
        msg_box.exec();
        return;
    }
    QString ip_str = ui->lineEdit_mp_ip->text();
    _mp_ip = inet_addr(ip_str.toStdString().c_str());
    //    printf("mp ip %d \t %s\n", mp_ip, ui->lineEdit_mp_ip->text().toStdString().c_str());
    hand.table.encode_bitrate = ui->lineEdit_bitrate->text().toInt();
    hand.send_relation(ip_str, ui->comboBox->currentIndex());
}

void MainWindow::on_checkBox_reencode_stateChanged(int arg1)
{

    bool reencode = arg1==Qt::Checked?true:false;
    _nalu_receiver._reencode = reencode;
}

void MainWindow::on_pushButton_forward_start_clicked()
{
    QString sendaddrs = ui->lineEdit_forward_addrs->text();
    if(sendaddrs.isEmpty())
    {
        QMessageBox msg;
        msg.setText("please enter send addrs as:ip:port");
        msg.exec();
        return ;
    }
    QStringList sendlist = sendaddrs.split(";");

    std::set<std::string> send_list_v;
    for (QStringList::const_iterator constIterator = sendlist.constBegin();
         constIterator != sendlist.constEnd();
         ++constIterator)
    {
        send_list_v.insert((*constIterator).toStdString());
    }
    _nalu_receiver.set_forward_addrs(send_list_v);
     ui->pushButton_forward_start->setEnabled(false);
      ui->pushButton_forward_stop->setEnabled(true);
}


//void MainWindow::on_pushButton_set_reencode_param_clicked()
//{
//    _dialog_encode_param->show();
//}

void MainWindow::on_checkBox_reencode_2_toggled(bool checked)
{
   if(_open_device.type == 0 )
   {
       _open_device.type = 4;
   }
}

void MainWindow::on_pushButton_cmd_stopmp_clicked()
{
    QString ip_str = ui->lineEdit_mp_ip->text();
    hand.stop_mp(ip_str);
}
static Video_Stream_Type string2stream_type(const QString &str)
{
    Video_Stream_Type ret = VIDEO_STREAM_TYPE_HCIF_2012;
    if(str =="HCIF")
        ret=VIDEO_STREAM_TYPE_HCIF_2012;
    else if(str == "D1")
        ret = VIDEO_STREAM_TYPE_D1;
    else if(str == "720P")
        ret = VIDEO_STREAM_TYPE_720p;
    else if(str == "1080P")
        ret = VIDEO_STREAM_TYPE_HD;
    else if(str == "1080PX265")
        ret = VIDEO_STREAM_TYPE_X265_1920X1080;
    return ret;

}
void MainWindow::on_comboBox_video_type_currentIndexChanged(const QString &arg1)
{

    Video_Stream_Type stream_type = string2stream_type(arg1);
     T_encode_param param(stream_type);
    _open_device._encode_param = param;
}
