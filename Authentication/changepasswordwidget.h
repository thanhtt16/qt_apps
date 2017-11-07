#ifndef CHANGEPASSWORDWIDGET_H
#define CHANGEPASSWORDWIDGET_H
#include <QWidget>
#include <QVBoxLayout>
#include <qlabel.h>
#include <QLineEdit>
#include <QPushButton>

class ChangePassWordWidget : public QWidget
{
    Q_OBJECT

private:

    QWidget mRootWidget;
    QLabel *lb_currentPassWord;
    QLabel *lb_newPassWord;
    QLabel *lb_confirmPassWord;
    QLineEdit * edit_currentPassWord;
    QLineEdit * edit_newPassWord;
    QLineEdit * edit_confirmPassWord;
    QPushButton * btn_save;
public:
    ChangePassWordWidget(QWidget *parent);
    void showMessage(QString title, QString message);
Q_SIGNALS:


public Q_SLOTS:
    void onSaveClicked();
};
#endif // CHANGEPASSWORDWIDGET_H
