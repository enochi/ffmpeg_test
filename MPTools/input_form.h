#ifndef INPUT_FORM_H
#define INPUT_FORM_H

#include <QWidget>

namespace Ui {
class Input_Form;
}
class Device;
class Input_Form : public QWidget
{
    Q_OBJECT
    
public:
    explicit Input_Form(QWidget *parent = 0);
    ~Input_Form();
    Device * get_device();
    bool set_device(Device device);


signals:
    void get_device_info();
    
private slots:
    void on_pushButton_okay_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::Input_Form *ui;
    Device *_device;
};

#endif // INPUT_FORM_H
