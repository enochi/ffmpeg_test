#ifndef DIALOG_ENCODE_PARAM_H
#define DIALOG_ENCODE_PARAM_H

#include <QDialog>

namespace Ui {
class Dialog_Encode_Param;
}
class MainWindow;
class Dialog_Encode_Param : public QDialog
{
    Q_OBJECT

public:
    bool _encode_start;
    explicit Dialog_Encode_Param(MainWindow * mainwindow,QWidget *parent = 0);
    ~Dialog_Encode_Param();
    void encode_start(bool start);
private slots:
    void on_pushButton_get_encode_2_clicked();

    void on_pushButton_set_encode_2_clicked();

private:
    Ui::Dialog_Encode_Param *ui;
    MainWindow *_mainwindow;
};

#endif // DIALOG_ENCODE_PARAM_H
