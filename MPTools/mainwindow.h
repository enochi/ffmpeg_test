#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <set>
#include <string>
#include <QGridLayout>
#include <QLabel>
#include "../common/nalu_receiver.h"
#include "../common/nalu_sender.h"
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>
#include <unordered_map>
#include <QListWidgetItem>
#include "relation_hand.h"
#include <QAbstractButton>
namespace Ui {
class MainWindow;
}
class YUV_Widget;
class Dispart_Label;
class Input_Form;
class Device;
class Relation;
class My_Ace_Client;
class Dialog_Encode_Param;
class MainWindow : public QMainWindow
{
    Q_OBJECT
friend class Dialog_Encode_Param;
public:
    explicit MainWindow(QWidget *parent = 0);
    int init(int argc ,char *argv[]);
    ~MainWindow();

private slots:
    int slot_set_encode_param(T_encode_param *param);
    void on_tabwidget_currentChanged(int index);

    void slot_save_file();
    void on_frame_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_cmd_clicked();

    void slot_stop_save_file();

    void on_pushButton_select_file_clicked();

    void on_pushButton_begin_display_clicked();

    void on_pushButton_stop_display_clicked();

    void on_pushButton_start_send_file_clicked();

    void on_pushButton_stop_send_file_clicked();

    void on_pushButton_forward_clicked();

    void on_pushButton_forward_stop_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_add_clicked();

    void on_buttonGroup_button_clicked(QAbstractButton * button);
    void time_out();
private:
    int func_notice(int type,const std::string &msg);
    NALU_Receiver _nalu_receiver;
    Nalu_Sender _nalu_sender;

    QMenu *_main_menu;
    QMenu *_video_menu;
    QLabel *_remote_label;
    QGridLayout *_video_layout;
    Dialog_Encode_Param *_dialog_encode_param;
    YUV_Widget *_yuv_widget_display;
    YUV_Widget *_yuv_widget_card;
    Ui::MainWindow *ui;
    QString _send_file;
    QString _save_file;
    bool _start_save;
    FUNC_ON_YUV on_yuv_display;
    FUNC_ON_YUV on_yuv_card;


    /**/
    QGridLayout *_dispart_layout;
    //std::unordered_map<int, Dispart_Label*> _dispart_label_map;

    Input_Form * _input_send_info;
    int _mp_ip;
    //std::unordered_map<std::string, Device*> _item_string_device_map;
    Relation_Hand hand;
    ffmpeg_open_type _open_device;

    int _file_fd;

private slots:
    void save_send_device_info();

    void on_pushButton_del_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_cmd_mp_clicked();

    void on_checkBox_reencode_stateChanged(int arg1);
    void on_pushButton_forward_start_clicked();

    //void on_pushButton_set_reencode_param_clicked();
    void on_checkBox_reencode_2_toggled(bool checked);
    void on_pushButton_cmd_stopmp_clicked();
    void on_comboBox_video_type_currentIndexChanged(const QString &arg1);
};

#endif // MAINWINDOW_H
