#ifndef LOGIN_H
#define LOGIN_H

#include <qlabel.h>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QShowEvent>
#include <QVBoxLayout>
#include <QVariant>
#include <QVariantList>
#include <QWidget>
#include "Authentication/user.h"
#include "message.h"
#include <QProcess>
class LoginWidget : public QDialog {
    Q_OBJECT

private:
    static QString API_CHECK_TOKEN;
    QVariant *responseData;
    QProcess pingProcess;

    QLabel *mLogoLabel;
    QLabel *mUserLabel;
    QLabel *mPasswordLabel;
    QLineEdit *userLineEdit;
    QLineEdit *passWordLineEdit;
    QPushButton *mLoginButton;
    QLabel *mMessageLabel;

    UserInfo userInfo;
    int loginFaildCounter;
    bool mIsRequesting = false;

    QString username;
    QString password;
    int stateNetwork;
public:
    AppMessage Message;
    LoginWidget(QWidget *parent, QVariant *attachment);
    bool isValidPhone(QString str);
    void startCheckNetWork();
    void loadLocalUserInfo();
    QTimer *timerCheckNetwork;

Q_SIGNALS:
    void windowIsShown();
public Q_SLOTS:
    void onLoginClicked();
    void onLoggedOut();
    void recheckNetWork();

protected:
    void show();
};
#endif  // LOGIN_H
