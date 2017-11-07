﻿#include "Login/loginwidget.h"
#include <Authentication/user.h>
#include <Camera/camsite.h>
#include <Common/networkutils.h>
#include <qdebug.h>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QScreen>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidget>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <mutex>
#include "Authentication/user.h"
#include "MainFrame/c_mainframe.h"
#include "QApplication"

QString LoginWidget::API_CHECK_TOKEN = "/cms_api/check";
LoginWidget::LoginWidget(QWidget *parent, QVariant *attachment)
    : QDialog(parent) {
    this->responseData = attachment;

    setModal(true);
    setFixedSize(400, 300);
    loginFaildCounter = 0;

    QVBoxLayout *mBoxLayout = new QVBoxLayout();
    mBoxLayout->setAlignment(Qt::AlignCenter);
    mBoxLayout->setSpacing(10);
    mBoxLayout->setMargin(20);
    setLayout(mBoxLayout);

    mUserLabel = new QLabel(this);
    mUserLabel->setText("Tài khoản:");
    mUserLabel->setFont(Resources::instance().getLargeBoldButtonFont());
    mBoxLayout->addWidget(mUserLabel);

    userLineEdit = new QLineEdit(this);
    userLineEdit->setFont(Resources::instance().getLargeRegularButtonFont());
    userLineEdit->setFixedSize(360, 30);
    userLineEdit->setText("");
    connect(userLineEdit, &QLineEdit::returnPressed, this, [this] {
        passWordLineEdit->setText("");
        passWordLineEdit->setFocus();
    });
    mBoxLayout->addWidget(userLineEdit);

    mPasswordLabel = new QLabel(this);
    mPasswordLabel->setFont(Resources::instance().getLargeBoldButtonFont());
    mPasswordLabel->setText("Mật khẩu:");
    mBoxLayout->addWidget(mPasswordLabel);

    passWordLineEdit = new QLineEdit(this);
    passWordLineEdit->setFont(Resources::instance().getLargeRegularButtonFont());
    passWordLineEdit->setFixedSize(360, 30);
    passWordLineEdit->setEchoMode(QLineEdit::Password);
    passWordLineEdit->setText("");
    connect(passWordLineEdit, &QLineEdit::returnPressed, this,
            &LoginWidget::onLoginClicked);
    mBoxLayout->addWidget(passWordLineEdit);

    mMessageLabel = new QLabel(this);
    mMessageLabel->setFont(Resources::instance().getLargeRegularButtonFont());
    mMessageLabel->setFixedSize(360, 30);
    mMessageLabel->setAlignment(Qt::AlignCenter);
    mMessageLabel->setText("");
    mBoxLayout->addWidget(mMessageLabel);

    mLoginButton = new QPushButton(this);
    mLoginButton->setFont(Resources::instance().getLargeBoldButtonFont());
    mLoginButton->setText("Đăng nhập");
    mLoginButton->setFixedSize(360, 30);
    connect(mLoginButton, &QPushButton::clicked, this,
            &LoginWidget::onLoginClicked);
    mBoxLayout->addWidget(mLoginButton);

    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(),
                                    qApp->desktop()->availableGeometry()));
    connect(&pingProcess, &QProcess::readyReadStandardOutput, this, [this]{
        this->loadLocalUserInfo();
        qDebug() << "NETWORK REACHABLE => START LOGIN";

    });
    connect(&pingProcess, &QProcess::readyReadStandardError, this,[this]{
        mMessageLabel->setText("Vui lòng kiểm tra kết nối mạng!");
    });

    timerCheckNetwork = new QTimer(this);
    timerCheckNetwork->start(500);
    connect(timerCheckNetwork, &QTimer::timeout, this, &LoginWidget::recheckNetWork);
    startCheckNetWork();
}
void LoginWidget::recheckNetWork(){
    pingProcess.close();
    startCheckNetWork();
}
void LoginWidget::onLoggedOut() {
    User::clearUserInfo();
    mMessageLabel->setText("");
}

void LoginWidget::onLoginClicked() {
    if (!mIsRequesting) {
        mLoginButton->setEnabled(false);
        mIsRequesting = true;
        mMessageLabel->setText("Đang đăng nhập...");

        this->username = userLineEdit->text().trimmed();
        this->password = NetworkUtils::instance().encodeSHA265(
                    passWordLineEdit->text().trimmed());

        userLineEdit->setText("");
        passWordLineEdit->setText("");
        userLineEdit->setDisabled(true);
        passWordLineEdit->setDisabled(true);

        function<void(QString)> onFailure = [this](QString message) {
            qDebug() << "Login failure!";
            userLineEdit->setEnabled(true);
            passWordLineEdit->setEnabled(true);
            mLoginButton->setEnabled(true);
            mIsRequesting = false;
            mMessageLabel->setText("Đăng nhập lỗi!");
            userLineEdit->setFocus();
            loginFaildCounter++;
            if (loginFaildCounter == 3) {
                this->responseData->setValue(Q_NULLPTR);
                this->close();
            }
        };

        function<void(User *)> onSuccess = [this](User *user) {
            qDebug() << "Login successfull!";
            qDebug() << "Tên người dùng:" << user->getFullName();
            qDebug() << "Địa chỉ email :" << user->getEmail();
            userLineEdit->setEnabled(true);
            passWordLineEdit->setEnabled(true);
            mLoginButton->setEnabled(true);
            mIsRequesting = false;
            mMessageLabel->setText("Đăng nhập thành công!");
            this->responseData->setValue(user);
            this->close();
        };

        std::function<void(QJsonObject)> onFetchSuccess =
                [this, onSuccess, onFailure](QJsonObject jsonObject) {
            // duongnt_25_09_2017
            QJsonValue jsonValue;
            jsonValue = jsonObject.take("code");
            if (!jsonValue.isUndefined()) {
                mMessageLabel->setText("Đăng nhập lỗi!");
                onFailure("Đăng nhập lỗi!");
                return;
            }
            // duongnt_25_09_2017 convert user json object to user json string
            QJsonDocument jsonDocument(jsonObject);
            QString jsonString(jsonDocument.toJson(QJsonDocument::Compact));
            userInfo.userData = jsonString;
            User::saveUserInfo(userInfo);
            User *user = User::parse(jsonObject);
            onSuccess(user);
        };

        std::function<void(QString)> onFetchFailure =
                [this, onFailure](QString message) { onFailure(message); };

        QMap<QString, QString> params;
        params["action"] = "login";
        params["password"] = this->password;
        if (isValidPhone(this->username)) {
            params["phone"] = this->username;
        } else {
            params["username"] = this->username;
        }

        NetworkUtils::instance().getRequest(
                    AppProfile::getAppProfile()->getAppConfig()->getLoginApiUri(), params,
                    onFetchSuccess, onFetchFailure);
    }
}

bool LoginWidget::isValidPhone(QString str) {
    QRegExp re("\\d*");
    return re.exactMatch(str);
}

void LoginWidget::startCheckNetWork(){
    QString hostPing = AppProfile::getAppProfile()->getAppConfig()->getBaseApiUrl();
    QUrl url = QUrl(hostPing);
    hostPing = url.host();
    // ping with interval 200 ms
    QString exec = "ping";
    QStringList params;
    params << "-c1" << hostPing ;

    // start pinging
    pingProcess.start(exec, params, QIODevice::ReadOnly);
    QCoreApplication::processEvents();

}


void LoginWidget::show() {}

void LoginWidget::loadLocalUserInfo() {
    timerCheckNetwork->stop();
    pingProcess.close();
    qDebug() << "LoginWidget -> loadLocalUserInfo";
    userInfo = User::loadUserInfo();
    if (!userInfo.userData.isEmpty()) {
        User *user = User::getUserFrom(userInfo);
        if (user != Q_NULLPTR && !user->getToken().isEmpty() &&
                user->getUserId() > 0) {
            QString tokenSaved = user->getToken();
            std::function<void(QJsonObject)> onFetchSuccess = [this, user](QJsonObject jsonObject){
                QJsonValue jsonValue = jsonObject.take("fullname");
                QString username;
                int activeState;
                if(!jsonValue.isNull()){
                    username =  jsonValue.toString();
                }
                jsonValue = jsonObject.take("active");
                if(!jsonValue.isNull()){
                    QString activeString = jsonValue.toString();
                    activeState = activeString.toInt();
                }
                qDebug() << "username" << username << activeState << user->getFullName();
                //token isvalid
                if(username == user->getFullName() && activeState == 1){
                    user->log();
                    mMessageLabel->setText("Đăng nhập thành công!");
                    this->responseData->setValue(user);
                    QTimer::singleShot(1000, this, [this] {
                        this->close();
                    });
                }else{
                    userLineEdit->setFocus();
                }
            };

            std::function<void(QString)> onFetchFailure = [this](QString message){
                userLineEdit->setFocus();
                mMessageLabel->setText("Đăng nhập lỗi!");
                qDebug() << "Token login failure";
            };
            QMap<QString, QString> params;
            params.insert("token", tokenSaved);
            NetworkUtils::instance().getRequest(API_CHECK_TOKEN, params, onFetchSuccess, onFetchFailure);

        }
    }
}

