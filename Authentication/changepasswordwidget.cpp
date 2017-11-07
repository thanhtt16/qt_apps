#include "Authentication/changepasswordwidget.h"
#include <Authentication/user.h>
#include <Camera/camsite.h>
#include <Common/networkutils.h>
#include <qdebug.h>
#include <QMessageBox>
#include <QVBoxLayout>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <mutex>
#include "QWidget"

ChangePassWordWidget::ChangePassWordWidget(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *mBoxLayout = new QVBoxLayout;
  mRootWidget.setLayout(mBoxLayout);
  lb_currentPassWord = new QLabel(tr("Current password"));
  lb_newPassWord = new QLabel(tr("New password"));
  lb_confirmPassWord = new QLabel(tr("Confirm password"));

  edit_currentPassWord = new QLineEdit();
  edit_currentPassWord->setEchoMode(QLineEdit::Password);
  edit_currentPassWord->setFixedWidth(300);

  edit_newPassWord = new QLineEdit();
  edit_newPassWord->setEchoMode(QLineEdit::Password);
  edit_newPassWord->setFixedWidth(300);

  edit_confirmPassWord = new QLineEdit();
  edit_confirmPassWord->setEchoMode(QLineEdit::Password);
  edit_confirmPassWord->setFixedWidth(300);

  btn_save = new QPushButton(tr("Save"));
  btn_save->setFixedSize(100, 30);

  mBoxLayout->setAlignment(Qt::AlignCenter);
  mBoxLayout->addWidget(lb_currentPassWord);
  mBoxLayout->addWidget(edit_currentPassWord);
  mBoxLayout->addWidget(lb_newPassWord);
  mBoxLayout->addWidget(edit_newPassWord);
  mBoxLayout->addWidget(lb_confirmPassWord);
  mBoxLayout->addWidget(edit_confirmPassWord);
  mBoxLayout->addWidget(btn_save);
  btn_save->setStyleSheet("margin-top: 4px;min-height: 30px;");
  mRootWidget.showMaximized();
  QObject::connect(btn_save, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
}

void ChangePassWordWidget::onSaveClicked() {
  if (edit_currentPassWord->text().length() == 0) {
    //    qDebug() << "currentpassword khong duoc de trong";
    return;
  }

  if (edit_newPassWord->text().length() == 0) {
    //    qDebug() << "newpassword khong duoc de trong";
    return;
  }

  if (edit_confirmPassWord->text().length() == 0) {
    //    qDebug() << "confirmpassword khong duoc de trong";
    return;
  }

  if (edit_confirmPassWord->text() != edit_newPassWord->text()) {
    //    qDebug() << "newpassword va confirmpassword khong giong nhau";
    return;
  }

  if (edit_currentPassWord->text() == edit_newPassWord->text()) {
    //    qDebug() << "currentpassword va newpassword trung nhau";
    return;
  }

  function<void(QString)> onFailure = [this](QString message) {
    // showStatus(WSError, message);
    showMessage("Message", message);
  };

  function<void(QJsonObject)> onSuccess = [this](QJsonObject data) {
    Q_UNUSED(data)
    showMessage("Message", "Success");
  };

  std::function<void(QJsonObject)> onFetchSuccess =
      [onSuccess, this](QJsonObject jsonObject) {
        QJsonValue jsonValue;
        jsonValue = jsonObject.take("code");
        if (!jsonValue.isUndefined()) {
          jsonValue = jsonObject.take("status");
          if (!jsonValue.isNull()) {
            showMessage("Message", jsonValue.toString());
          }
          return;
        }
        onSuccess(jsonObject);
        // this->mRootWidget.hide();
        // new VideoWallWidget(NULL);
      };

  std::function<void(QString)> onFetchFailure = [onFailure](QString message) {
    onFailure(message);
  };

  QMap<QString, QString> params;
  //    params["token"] = User::instance().getToken();
  //    params["id"] = QString::number(User::instance().getUserId());
  //    params["newpassword"] =
  //    NetworkUtils::instance().encodeSHA265(edit_newPassWord->text());
  //    params["oldpassword"] =
  //    NetworkUtils::instance().encodeSHA265(edit_currentPassWord->text());

  NetworkUtils::instance().getRequest(
      AppProfile::getAppProfile()->getAppConfig()->getChangePasswordApiUri(),
      params, onFetchSuccess, onFetchFailure);
}

void ChangePassWordWidget::showMessage(QString title, QString message) {
  QMessageBox::information(this, title, message);
}
