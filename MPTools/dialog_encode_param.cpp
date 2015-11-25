#include "dialog_encode_param.h"
#include "ui_dialog_encode_param.h"
#include "mainwindow.h"
#include <QMessageBox>
Dialog_Encode_Param::Dialog_Encode_Param( MainWindow* mainwindow,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Encode_Param)
{
    ui->setupUi(this);
    _mainwindow = mainwindow;

}

Dialog_Encode_Param::~Dialog_Encode_Param()
{
    delete ui;
}
void Dialog_Encode_Param::encode_start(bool start)
{

}

void Dialog_Encode_Param::on_pushButton_get_encode_2_clicked()
{
    T_encode_param param;

    int ret = _mainwindow->_nalu_sender.get_reconfig_encode_param(param);
    if(ret ==0)
    {
    QString tmp = QString::number(param.bitrate);
    ui->lineEdit_bitrate_2->setText(tmp);
    tmp = QString::number(param.fps);
    ui->lineEdit_fps_2->setText(tmp);
    tmp = QString::number(param.method);
    ui->lineEdit_method_2->setText(tmp);
    tmp = QString::number(param.qop_constant);
    ui->lineEdit_qop_2->setText(tmp);
    tmp = QString::number(param.qop_max);
    ui->lineEdit_qop_max_2->setText(tmp);
    tmp = QString::number(param.qop_min);
    ui->lineEdit_qop_min_2->setText(tmp);
    tmp = QString::number(param.qop_step);
    ui->lineEdit_qop_step_2->setText(tmp);
    }
    else
    {
        QMessageBox msg;
        msg.setText("encoder not open\n");
        msg.exec();
        return ;
    }
}

void Dialog_Encode_Param::on_pushButton_set_encode_2_clicked()
{
    T_encode_param param;

    param.bitrate = ui->lineEdit_bitrate_2->text().toInt();
    param.fps = ui->lineEdit_fps_2->text().toInt();
    param.method =  ui->lineEdit_method_2->text().toInt();
    param.qop_constant =ui->lineEdit_qop_2->text().toInt() ;
    param.qop_max =  ui->lineEdit_qop_max_2->text().toInt();
    param.qop_min = ui->lineEdit_qop_min_2->text().toInt() ;
    param.qop_step =  ui->lineEdit_qop_step_2->text().toInt() ;

   int ret = _mainwindow->_nalu_sender.set_reconfig_encode_param(param);
   QMessageBox msg;

   if(ret ==0)
   {
       msg.setText("set encode ok\n");
   }
   else
   {
        msg.setText("set encode failed\n");
   }
   msg.exec();
   hide();
   return ;
}


