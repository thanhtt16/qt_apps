#include "ms_profilecellwidget.h"

ProfileCellWidget::ProfileCellWidget(QWidget *parent) : QStackedWidget(parent) {
  mThumbLabel = new QLabel(this);
  mThumbLabel->setFont(Resources::instance().getLargeRegularButtonFont());
  //    mThumbLabel->setStyleSheet(utils.simpleStyleSheet(colors.MIDNIGHT_BLUE,
  //    colors.WHITE));
  mThumbLabel->setAlignment(Qt::AlignCenter);
  addWidget(mThumbLabel);
  setCurrentWidget(mThumbLabel);

  mButtonLayout = new QVBoxLayout();
  mButtonLayout->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
  mButtonLayout->setMargin(10);
  mButtonLayout->setSpacing(10);
  mThumbLabel->setLayout(mButtonLayout);

  mAddDelButton = new QPushButton(mThumbLabel);
  mAddDelButton->setFixedSize(50, 50);
  connect(mAddDelButton, &QPushButton::clicked, this,
          &ProfileCellWidget::onAddDelButtonClicked);
  mButtonLayout->addWidget(mAddDelButton);
  setAdded(false);
}

void ProfileCellWidget::onAddDelButtonClicked() {
  if (!mIsAdded) {
    Q_EMIT showSelection(mIndex);
  } else {
    Q_EMIT deleteAt(mIndex);
  }
}

void ProfileCellWidget::setAdded(bool added) {
  mIsAdded = added;
  //    mAddDelButton->setStyleSheet(utils.borderStyleSheet(mIsAdded ?
  //    colors.ALIZARIN : colors.EMERALD, colors.WHITE, 0,
  //    mAddDelButton->height() / 2));
  mAddDelButton->setIcon(Resources::instance().getAwesome()->icon(
      mIsAdded ? fa::minus : fa::plus, Resources::instance().getIconOptions()));
}

void ProfileCellWidget::setText(QString text) { mThumbLabel->setText(text); }

void ProfileCellWidget::setEnabledButton(bool enabled) {
  mAddDelButton->setEnabled(enabled);
}

void ProfileCellWidget::setIndex(int index) { mIndex = index; }

int ProfileCellWidget::getIndex() { return mIndex; }

bool ProfileCellWidget::isAdded() { return mIsAdded; }

void ProfileCellWidget::resizeEvent(QResizeEvent *event) {
  Q_UNUSED(event)
  //    mThumbLabel->resize(event->size());
  //    mThumbLabel->move(0, 0);
}
