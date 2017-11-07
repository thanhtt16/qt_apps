#include "messagewidget.h"
#include <QGraphicsBlurEffect>
#include <QPropertyAnimation>
#include "resources.h"

MessageWidget::MessageWidget(QWidget *parent) : QWidget(parent) {}

void MessageWidget::initUI(QSize screenSize) {
  mScreenSize = screenSize;
  QSize windowSize(500, 300);

  mTopPos.setX((mScreenSize.width() - windowSize.width()) / 2);
  mTopPos.setY(-windowSize.height());
  move(mTopPos);

  mCenPos.setX((mScreenSize.width() - windowSize.width()) / 2);
  mCenPos.setY((mScreenSize.height() - windowSize.height()) / 2);

  mBotPos.setX((mScreenSize.width() - windowSize.width()) / 2);
  mBotPos.setY(mScreenSize.height() + windowSize.height());

  QWidget *mainWidget = new QWidget(this);
  mainWidget->resize(windowSize);
  //    mainWidget->setStyleSheet(utils.borderStyleSheet(colors.MIDNIGHT_BLUE,
  //    colors.WHITE, 0, 10));

  mMessageLayout = new QVBoxLayout();
  mMessageLayout->setSpacing(20);
  mMessageLayout->setMargin(10);
  mMessageLayout->setAlignment(Qt::AlignCenter);
  mainWidget->setLayout(mMessageLayout);

  mTitleLabel = new QLabel(mainWidget);
  mTitleLabel->setFont(Resources::instance().getAwesome()->font(20));
  mTitleLabel->setAlignment(Qt::AlignCenter);
  //  mTitleLabel->setStyleSheet(
  //      utils.simpleStyleSheet(colors.TRANSPARENT, colors.WHITE));
  mMessageLayout->addWidget(mTitleLabel);

  mMessageLabel = new QLabel(mainWidget);
  mMessageLabel->setFont(Resources::instance().getAwesome()->font(16));
  mMessageLabel->setAlignment(Qt::AlignCenter);
  //  mMessageLabel->setStyleSheet(
  //      utils.simpleStyleSheet(colors.TRANSPARENT, colors.WHITE));
  mMessageLayout->addWidget(mMessageLabel);
}

void MessageWidget::setTitle(QString title) { mTitleLabel->setText(title); }

void MessageWidget::setMessage(QString message) {
  mMessageLabel->setText(message);
}

void MessageWidget::message(QString title, QString message) {
  setTitle(title);
  setMessage(message);
  showAnimation();
}

void MessageWidget::showAnimation() {
  if (x() == mTopPos.x() && y() == mTopPos.y()) {
    QPropertyAnimation *sourceAnimation = new QPropertyAnimation(this, "pos");
    sourceAnimation->setDuration(250);
    sourceAnimation->setStartValue(mTopPos);
    sourceAnimation->setEndValue(mCenPos);
    sourceAnimation->start(QAbstractAnimation::DeleteWhenStopped);
  }
}

void MessageWidget::hideAnimation() {
  if (x() == mCenPos.x() && y() == mCenPos.y()) {
    QPropertyAnimation *sourceAnimation = new QPropertyAnimation(this, "pos");
    sourceAnimation->setDuration(250);
    sourceAnimation->setStartValue(mCenPos);
    sourceAnimation->setEndValue(mTopPos);
    sourceAnimation->start(QAbstractAnimation::DeleteWhenStopped);
  }
}
