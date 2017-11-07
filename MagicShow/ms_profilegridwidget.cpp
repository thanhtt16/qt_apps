#include "MagicShow/ms_profilegridwidget.h"
#include "Common/appprofile.h"

ProfileGridWidget::ProfileGridWidget(QWidget *parent) : QWidget(parent) {
  setAttribute(Qt::WA_DeleteOnClose, true);
  //    setStyleSheet(utils.simpleStyleSheet(colors.CLOUDS,
  //    colors.MIDNIGHT_BLUE));
  mMaxLayout = 25;
  if (mMaxLayout < 16) {
    mDefaultLayout = 4;
    if (mMaxLayout < 4) {
      mMaxLayout = 4;
    }
  } else {
    mDefaultLayout = 16;
  }
  mNumberOfLayout = sqrt(mMaxLayout) - 1;
  initUI();
  mSearchTimer = new QTimer(this);
}

void ProfileGridWidget::startSearch() {
  if (!mSearchTimer->isActive()) {
    connect(mSearchTimer, &QTimer::timeout, this, &ProfileGridWidget::onSearch);
    mSearchTimer->start(300);
  }
}

void ProfileGridWidget::stopSearch() {
  if (mSearchTimer->isActive()) {
    disconnect(mSearchTimer, &QTimer::timeout, this,
               &ProfileGridWidget::onSearch);
    mSearchTimer->stop();
  }
}

void ProfileGridWidget::onSelect() {
  hideSelectionWidget();
  QModelIndexList selectedRows =
      mCamPositionListView->selectionModel()->selectedRows();
  if (selectedRows.size() > 0) {
    int row =
        mCamPositionListView->selectionModel()->selectedRows().first().row();
    int key = mFilteredOriginalHash.keys().at(row);
    QString value = mFilteredOriginalHash.values().at(row);
    mSelectedOriginalHash.insert(key, value);
    mSelectedMinimalHash.insert(key, mMinimalHash.value(key));

    mFilteredOriginalHash.remove(key);
    mFilteredMinimalHash.remove(key);

    mCurrentOriginalHash.remove(key);
    mCurrentMinimalHash.remove(key);

    mCamPositions->setStringList(mFilteredOriginalHash.values());

    QList<int> ids = mProfilePage->getCamIds();
    ids.replace(mCurrentProfileCellIndex, key);
    mProfilePage->setIds(ids);
    ProfileCellWidget *profileCellWidget =
        mProfileCellWidgets.at(mCurrentProfileCellIndex);
    profileCellWidget->setText(value);
    profileCellWidget->setAdded(true);
    mSearchLineEdit->setText("");
  }
}

void ProfileGridWidget::onCancel() { hideSelectionWidget(); }

void ProfileGridWidget::onDeleteAt(int index) {
  ProfilePage *profilePage =
      mProfileBook->getProfilePages().at(mCurrentPage - 1);
  QList<int> ids = profilePage->getCamIds();
  int key = ids.at(index);
  QString value = mOriginalHash.value(key);
  ids.replace(index, 0);
  profilePage->setIds(ids);
  mCurrentOriginalHash.insert(key, value);
  mCurrentMinimalHash.insert(key, mMinimalHash.value(key));
  mSelectedOriginalHash.remove(key);
  mSelectedMinimalHash.remove(key);
  qDebug() << "\n after_delete";
  qDebug() << "\n current" << mCurrentMinimalHash;
  qDebug() << "\\n selected" << mSelectedMinimalHash;
  ProfileCellWidget *profileCellWidget = mProfileCellWidgets.at(index);
  //    profileCellWidget->setText(strings.noData);
  profileCellWidget->setAdded(false);
}

void ProfileGridWidget::onSearch() {
  if (!mIsSearching) {
    mLastKeyword = mCurrentKeyword;
    mCurrentKeyword = mSearchLineEdit->text();
    mCurrentKeyword = mCurrentKeyword.trimmed();
    if (mLastKeyword.toLower() != mCurrentKeyword.toLower() &&
        mMaxLengthKeyword >= mCurrentKeyword.length()) {
      mIsSearching = true;
      QStringList tokens = mCurrentKeyword.trimmed().split(" ");
      mFilteredMinimalHash.clear();
      mFilteredOriginalHash.clear();
      QList<int> keys;
      keys = mCurrentMinimalHash.keys();
      for (int i = 0; i < keys.size(); ++i) {
        int key = keys.at(i);
        QString title = mCurrentOriginalHash.value(key);
        QString compareTitle = mCurrentMinimalHash.value(key);
        bool found = true;
        for (int index = 0; index < tokens.size(); ++index) {
          QString compareToken = tokens.at(index);
          //                    QString compareToken =
          //                    strings.removeVietnamese(token.toLower());
          if (!compareTitle.contains(compareToken)) {
            found = false;
            break;
          }
        }
        if (found) {
          mFilteredOriginalHash.insert(key, title);
          mFilteredMinimalHash.insert(key, compareTitle);
        } else {
        }
      }
      qDebug() << "\n found";
      qDebug() << "\n mFilteredOriginalHash" << mFilteredOriginalHash.size()
               << mFilteredOriginalHash;
      //            qDebug() << "\n mFilteredMinimalHash" <<
      //            mFilteredMinimalHash;
      mCamPositions->setStringList(mFilteredOriginalHash.values());
      mIsSearching = false;
    }
  }
}

void ProfileGridWidget::initMainUI() {
  mMainLayout = new QHBoxLayout();
  mMainLayout->setSpacing(0);
  mMainLayout->setMargin(0);
  mMainLayout->setAlignment(Qt::AlignCenter);
  setLayout(mMainLayout);
  initLeftSide();
  initRightSide();
}

void ProfileGridWidget::initLeftSide() {
  mLeftWidget = new QWidget(this);
  mLeftWidget->setFixedSize(mLeftWidth,
                            Resources::instance().getScreenSize().height());
  //    mLeftWidget->setStyleSheet(utils.simpleStyleSheet(colors.CLOUDS,
  //    colors.MIDNIGHT_BLUE));
  mMainLayout->addWidget(mLeftWidget);

  mLeftLayout = new QVBoxLayout();
  mLeftLayout->setAlignment(Qt::AlignCenter);
  mLeftLayout->setSpacing(10);
  mLeftLayout->setContentsMargins(5, 10, 5, 10);
  mLeftWidget->setLayout(mLeftLayout);

  mAddPageButton = new QPushButton(mLeftWidget);
  mAddPageButton->setText("Thêm");
  mAddPageButton->setFont(Resources::instance().getMediumBoldButtonFont());
  mAddPageButton->setIcon(
      Resources::instance()
          .getAwesome()
          ->icon(fa::edit, Resources::instance().getIconOptions())
          .pixmap(32, 32));
  mAddPageButton->setFixedSize(mLeftWidth - 10, 30);
  //  mAddPageButton->setStyleSheet(
  //      utils.borderStyleSheet(colors.PETER_RIVER, colors.WHITE, 0, 15));
  connect(mAddPageButton, &QPushButton::clicked, this,
          &ProfileGridWidget::onAddPageButtonClicked);
  mLeftLayout->addWidget(mAddPageButton);

  mPageLabel = new QLabel(mLeftWidget);
  mPageLabel->setAlignment(Qt::AlignCenter);
  mPageLabel->setFixedSize(mLeftWidth - 10, 30);
  mPageLabel->setFont(Resources::instance().getMediumBoldButtonFont());
  //    mPageLabel->setText(strings.page);
  //  mPageLabel->setStyleSheet(
  //      utils.simpleStyleSheet(colors.TRANSPARENT, colors.MIDNIGHT_BLUE));
  mLeftLayout->addWidget(mPageLabel);

  mPageScrollArea = new QScrollArea(mLeftWidget);
  mPageScrollArea->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  //  mPageScrollArea->setStyleSheet(
  //      utils.simpleStyleSheet(colors.TRANSPARENT, colors.MIDNIGHT_BLUE));
  mLeftLayout->addWidget(mPageScrollArea);

  mPageWidget = new QWidget(mPageScrollArea);
  mPageLayout = new QVBoxLayout();
  mPageLayout->setSpacing(20);
  mPageLayout->setMargin(0);
  mPageLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  mPageWidget->setLayout(mPageLayout);
  mPageScrollArea->setWidget(mPageWidget);
}

void ProfileGridWidget::updatePageWidgetSize() {
  mPageWidget->resize(mLeftWidth - 30, mPageButtons.size() * 70);
}

void ProfileGridWidget::updatePageButtons() {
  for (int index = 0; index < mPageButtons.size(); ++index) {
    int page = index + 1;
    QPushButton *pageButton = mPageButtons.at(index);
    if (page == mCurrentPage) {
      pageButton->setEnabled(false);
      //            pageButton->setStyleSheet(utils.borderStyleSheet(colors.EMERALD,
      //            colors.WHITE, 0, 25));
    } else {
      pageButton->setEnabled(true);
      //            pageButton->setStyleSheet(utils.borderStyleSheet(colors.MIDNIGHT_BLUE,
      //            colors.WHITE, 0, 25));
    }
  }
}

void ProfileGridWidget::onAddPageButtonClicked() {
  if (mProfileBook->getProfilePages().size() < mMaxPage) {
    addPage();
  } else {
    QMessageBox msgBox;
    msgBox.setText(QString("Bạn không thể tạo quá %1 trang!").arg(mMaxPage));
    msgBox.exec();
  }
}

void ProfileGridWidget::onDelPageButtonClicked() {
  if (mProfileBook->getProfilePages().size() > 1) {
  } else {
    if (mCurrentPage > 1) {
      QMessageBox msgBox;
      msgBox.setText("Xác nhận hủy");
      msgBox.setInformativeText(
          QString("Bạn có muốn xóa trang %1 không?").arg(mCurrentPage));
      msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
      msgBox.setDefaultButton(QMessageBox::Yes);
      msgBox.setStyleSheet("QLabel{min-width: 400px;}");
      int ret = msgBox.exec();
      if (ret == QMessageBox::Yes) {
        QList<ProfilePage *> currentProfilePages =
            mProfileBook->getProfilePages();
        currentProfilePages.removeAt(mCurrentPage - 1);
        mProfileBook->setProfilePages(currentProfilePages);

        // TODO: do next version
      }
    }
  }
}

void ProfileGridWidget::initRightSide() {
  mRightWidget = new QWidget(this);
  mRightWidget->setFixedSize(
      Resources::instance().getScreenSize().width() - mLeftWidth,
      Resources::instance().getScreenSize().height());
  //  mRightWidget->setStyleSheet(
  //      utils.simpleStyleSheet(colors.EMERALD, colors.MIDNIGHT_BLUE));
  mMainLayout->addWidget(mRightWidget);

  mRightLayout = new QVBoxLayout();
  mRightLayout->setSpacing(0);
  mRightLayout->setMargin(0);
  mRightLayout->setAlignment(Qt::AlignCenter);
  mRightWidget->setLayout(mRightLayout);

  initTopSide();
  initBotSide();
}

void ProfileGridWidget::initTopSide() {
  mTopWidget = new QWidget(mRightWidget);
  //    mTopWidget->setStyleSheet(utils.simpleStyleSheet(colors.CLOUDS,
  //    colors.MIDNIGHT_BLUE));
  mTopWidget->setFixedSize(
      Resources::instance().getScreenSize().width() - mLeftWidget->width(),
      Resources::instance().getScreenSize().height() - mBotHeight);
  mRightLayout->addWidget(mTopWidget);

  mGridLayout = new QGridLayout();
  mGridLayout->setSpacing(1);
  mGridLayout->setMargin(1);
  mTopWidget->setLayout(mGridLayout);
  for (int index = 0; index < mMaxLayout; ++index) {
    ProfileCellWidget *cell = new ProfileCellWidget(this);
    mPoolCellWidgets.append(cell);
    cell->hide();
    connect(cell, &ProfileCellWidget::showSelection, this,
            &ProfileGridWidget::showSelectionWidget);
    connect(cell, &ProfileCellWidget::deleteAt, this,
            &ProfileGridWidget::onDeleteAt);
  }
}

void ProfileGridWidget::initBotSide() {
  mBotWidget = new QWidget(mRightWidget);
  //    mBotWidget->setStyleSheet(utils.simpleStyleSheet(colors.CLOUDS,
  //    colors.MIDNIGHT_BLUE));
  mBotWidget->setFixedSize(
      Resources::instance().getScreenSize().width() - mLeftWidget->width(),
      mBotHeight);
  mRightLayout->addWidget(mBotWidget);

  mBotLayout = new QHBoxLayout();
  mBotLayout->setAlignment(Qt::AlignCenter);
  mBotLayout->setSpacing(10);
  mBotLayout->setContentsMargins(10, 0, 10, 0);
  mBotWidget->setLayout(mBotLayout);

  QWidget *layoutWidget = new QWidget(mBotWidget);
  int layoutWidth = 40 * mNumberOfLayout + 20 * (mNumberOfLayout - 1);
  layoutWidget->setMaximumWidth(layoutWidth);
  layoutWidget->setMinimumWidth(layoutWidth);
  QHBoxLayout *layoutLayout = new QHBoxLayout();
  layoutLayout->setMargin(0);
  layoutLayout->setSpacing(20);
  layoutLayout->setAlignment(Qt::AlignCenter);
  layoutWidget->setLayout(layoutLayout);
  for (int index = 0; index < mNumberOfLayout; ++index) {
    int layout = pow(index + 2, 2);
    QPushButton *button = new QPushButton(mBotWidget);
    button->setFixedSize(40, 40);
    //    button->setStyleSheet(
    //        utils.borderStyleSheet(colors.EMERALD, colors.CLOUDS, 0, 20));
    button->setFont(Resources::instance().getLargeBoldButtonFont());
    button->setText(QString::number(layout));
    connect(button, &QPushButton::clicked, this,
            &ProfileGridWidget::onLayoutButtonClicked);
    layoutLayout->addWidget(button);
    mLayoutButtons.append(button);
  }
  mBotLayout->addWidget(layoutWidget);

  //    mDelPageButton = new QPushButton(mBotWidget);
  //    mDelPageButton->setFixedSize(120, 30);
  //    mDelPageButton->setStyleSheet(utils.borderStyleSheet(colors.ALIZARIN,
  //    colors.CLOUDS, 0, 5));
  //    mDelPageButton->setFont(Resources::instance().getLargeBoldButtonFont());
  //    mDelPageButton->setIcon(Resources::instance().getAwesome()->icon(fa::trash,
  //    Resources::instance().getIconOptions()).pixmap(32,32));
  //    mDelPageButton->setText("Xóa trang");
  //    connect(mDelPageButton, &QPushButton::clicked, this,
  //    &ProfileGridWidget::onDelPageButtonClicked);
  //    mBotLayout->addWidget(mDelPageButton);

  QWidget *editWidget = new QWidget(mBotWidget);
  editWidget->setMaximumWidth(mBotWidget->width() - layoutWidth - 20);
  editWidget->setMinimumWidth(mBotWidget->width() - layoutWidth - 20);
  mBotLayout->addWidget(editWidget);
  QHBoxLayout *editLayout = new QHBoxLayout();
  editLayout->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  editLayout->setContentsMargins(10, 0, 10, 0);
  editLayout->setSpacing(10);
  editWidget->setLayout(editLayout);

  //    QLabel *label = new QLabel(editWidget);
  //    label->setFont(Resources::instance().getLargeBoldButtonFont());
  //    label->setText("Chuyển trang khi:");
  //    label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  //    editLayout->addWidget(label);

  mTransitionComboBox = new QComboBox(editWidget);
  //  mTransitionComboBox->setStyleSheet(
  //      utils.borderStyleSheet(colors.WHITE, colors.MIDNIGHT_BLUE, 0, 5));
  mTransitionComboBox->setFont(Resources::instance().getLargeBoldButtonFont());
  mTransitionComboBox->setFixedSize(270, 30);
  mTransitionComboBox->addItem("Click chuột để chuyển trang", 0);
  mTransitionComboBox->addItem("Tự động chuyển trang sau", 1);
  connect(mTransitionComboBox, SIGNAL(activated(int)), this,
          SLOT(onTransitionComboBoxSelectedIndex(int)));
  editLayout->addWidget(mTransitionComboBox);

  mDurationSpinBox = new QSpinBox(editWidget);
  //  mDurationSpinBox->setStyleSheet(
  //      utils.borderStyleSheet(colors.WHITE, colors.MIDNIGHT_BLUE, 0, 5));
  mDurationSpinBox->setFont(Resources::instance().getLargeBoldButtonFont());
  mDurationSpinBox->setFixedSize(100, 30);
  mDurationSpinBox->setMaximum(86400);
  mDurationSpinBox->setMinimum(1);
  mDurationSpinBox->setSingleStep(1);
  mDurationSpinBox->setSuffix("  giây");
  mDurationSpinBox->setDisabled(true);
  connect(mDurationSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(onDurationSpinBoxValueChanged(int)));
  editLayout->addWidget(mDurationSpinBox);

  mNameLineEdit = new QLineEdit(editWidget);
  mNameLineEdit->setFont(Resources::instance().getMediumBoldButtonFont());
  mNameLineEdit->setMaximumSize(270, 30);
  mNameLineEdit->setMinimumSize(100, 30);
  mNameLineEdit->setPlaceholderText("Nhập tên của cấu hình...");
  //  mNameLineEdit->setStyleSheet(
  //      utils.borderStyleSheet(colors.WHITE, colors.MIDNIGHT_BLUE, 0, 5));
  editLayout->addWidget(mNameLineEdit);

  mApplyButton = new QPushButton(editWidget);
  mApplyButton->setFixedSize(150, 30);
  //  mApplyButton->setStyleSheet(
  //      utils.borderStyleSheet(colors.EMERALD, colors.CLOUDS, 0, 15));
  mApplyButton->setFont(Resources::instance().getLargeBoldButtonFont());
  mApplyButton->setIcon(
      Resources::instance()
          .getAwesome()
          ->icon(fa::check, Resources::instance().getIconOptions())
          .pixmap(32, 32));
  mApplyButton->setText("Áp dụng");
  editLayout->addWidget(mApplyButton);
  connect(mApplyButton, &QPushButton::clicked, this,
          &ProfileGridWidget::onApplyButtonClicked);

  mClearButton = new QPushButton(editWidget);
  mClearButton->setFixedSize(100, 30);
  //  mClearButton->setStyleSheet(
  //      utils.borderStyleSheet(colors.ALIZARIN, colors.CLOUDS, 0, 15));
  mClearButton->setFont(Resources::instance().getLargeBoldButtonFont());
  mClearButton->setIcon(
      Resources::instance()
          .getAwesome()
          ->icon(fa::close, Resources::instance().getIconOptions())
          .pixmap(32, 32));
  mClearButton->setText("Hủy bỏ");
  editLayout->addWidget(mClearButton);
  connect(mClearButton, &QPushButton::clicked, this,
          &ProfileGridWidget::onClearButtonClicked);
}

void ProfileGridWidget::onTransitionComboBoxSelectedIndex(int index) {
  mDurationSpinBox->setDisabled(index == 0);
  if (index == 0) {
    mProfilePage->setDuration(0);
    mDurationSpinBox->hide();
    mDurationSpinBox->setValue(0);
  } else {
    mProfilePage->setDuration(60);
    mDurationSpinBox->show();
    mDurationSpinBox->setValue(60);
  }
}

void ProfileGridWidget::onDurationSpinBoxValueChanged(int value) {
  if (mTransitionComboBox->currentIndex() > 0) {
    mProfilePage->setDuration(value);
  }
}

void ProfileGridWidget::onClearButtonClicked() {
  QMessageBox msgBox;
  msgBox.setText("Xác nhận hủy");
  QString name = mProfileBook->getName();
  if (name.length() > 0) {
    name += " ";
  }
  msgBox.setInformativeText(
      QString("Bạn có muốn hủy các thay đổi gần đây của cấu hình %1không?")
          .arg(name));
  msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
  msgBox.setDefaultButton(QMessageBox::Yes);
  msgBox.setStyleSheet("QLabel{min-width: 400px;}");
  int ret = msgBox.exec();
  if (ret == QMessageBox::Yes) {
    Q_EMIT cancelProfileBook();
  }
}

void ProfileGridWidget::onLayoutButtonClicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  int index = mLayoutButtons.indexOf(button);
  if (index >= 0 && index < mLayoutButtons.size()) {
    mCurrentLayout = pow(index + 2, 2);
    QList<int> currentIds = mProfilePage->getCamIds();
    mProfilePage->setLayout(mCurrentLayout);
    //        if (mSelectedData.keys().size() > 0) {
    //            int lastKey = mSelectedData.keys().last();
    //            while (currentIds.contains(lastKey)) {
    //                QString value = mSelectedData.value(lastKey);
    //                mSelectedData.remove(lastKey);
    //                mOriginalData.insert(lastKey, value);
    //                mFilteredData.insert(lastKey, value);
    //                mMinimalData.insert(lastKey,
    //                strings.removeVietnamese(value));
    //                currentIds.replace(currentIds.indexOf(lastKey), 0);
    //                qDebug() << lastKey << value;
    //                if (mSelectedData.keys().size() > 0) {
    //                    lastKey = mSelectedData.keys().last();
    //                } else {
    //                    break;
    //                }
    //            }
    //        }
    //        mProfilePage->setIds(currentIds);
    mProfilePage->setDuration(mDurationSpinBox->value());
  }
  updateCurrentPageLayoutButtons();
  updateCurrentPageLayout();
  displayCurrentPage();
}

void ProfileGridWidget::updateCurrentPageLayoutButtons() {
  for (int index = 0; index < mLayoutButtons.size(); ++index) {
    //    QPushButton *button = mLayoutButtons.at(index);
    int layout = pow(index + 2, 2);
    if (mCurrentLayout == layout) {
      //            button->setStyleSheet(utils.borderStyleSheet(colors.EMERALD,
      //            colors.CLOUDS, 0, 20));
    } else {
      //            button->setStyleSheet(utils.borderStyleSheet(colors.MIDNIGHT_BLUE,
      //            colors.CLOUDS, 0, 20));
    }
  }
}

void ProfileGridWidget::initUI() {
  initMainUI();
  initSelectionWidget();
}

void ProfileGridWidget::initSelectionWidget() {
  mSelectionWidget = new QWidget(this);
  mSelectionWidget->resize(500, 500);
  mSelectionWidget->setMaximumSize(500, 500);
  mSelectionWidget->setMinimumSize(0, 0);
  mSelectionLayout = new QVBoxLayout();
  mSelectionLayout->setMargin(10);
  mSelectionLayout->setSpacing(10);
  mSelectionLayout->setAlignment(Qt::AlignCenter);
  mSelectionWidget->setLayout(mSelectionLayout);
  //    mSelectionWidget->setStyleSheet(utils.borderStyleSheet(colors.CLOUDS,
  //    colors.MIDNIGHT_BLUE, 0, 5));

  mSelectionLabel = new QLabel(mSelectionWidget);
  mSelectionLabel->setFont(Resources::instance().getLargeBoldButtonFont());
  mSelectionLabel->setText("Chọn Camera");
  mSelectionLabel->setAlignment(Qt::AlignCenter);
  mSelectionLayout->addWidget(mSelectionLabel);

  mSearchLineEdit = new QLineEdit(mSelectionWidget);
  mSearchLineEdit->setMaximumHeight(30);
  mSearchLineEdit->setMinimumHeight(30);
  //  mSearchLineEdit->setStyleSheet(
  //      utils.borderStyleSheet(colors.WHITE, colors.MIDNIGHT_BLUE, 0, 5));
  mSearchLineEdit->setPlaceholderText("Nhập từ khóa cần tìm kiếm...");
  mSelectionLayout->addWidget(mSearchLineEdit);
  connect(mSearchLineEdit, &QLineEdit::textChanged, this,
          &ProfileGridWidget::filterWithKeyword);

  mCamPositionListView = new QListView(mSelectionWidget);
  mCamPositionListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  //  mCamPositionListView->setStyleSheet(
  //      utils.borderStyleSheet(colors.WHITE, colors.MIDNIGHT_BLUE, 0, 5));
  connect(mCamPositionListView, &QListView::doubleClicked, this,
          &ProfileGridWidget::onSelect);
  mCamPositions = new QStringListModel();
  mCamPositionListView->setModel(mCamPositions);
  mSelectionLayout->addWidget(mCamPositionListView);

  QWidget *controlWidget = new QWidget(mSelectionWidget);
  QHBoxLayout *controlLayout = new QHBoxLayout();
  controlWidget->setLayout(controlLayout);

  QSize buttonSize(150, 30);
  mCancelButton = new QPushButton(controlWidget);
  mCancelButton->setIcon(
      Resources::instance()
          .getAwesome()
          ->icon(fa::close, Resources::instance().getIconOptions())
          .pixmap(32, 32));
  mCancelButton->setFont(Resources::instance().getMediumBoldButtonFont());
  mCancelButton->setText("Hủy");
  mCancelButton->resize(buttonSize);
  mCancelButton->setMinimumSize(buttonSize);
  mCancelButton->setMaximumSize(buttonSize);
  //  mCancelButton->setStyleSheet(utils.borderStyleSheet(
  //      colors.ALIZARIN, colors.WHITE, 0, mCancelButton->height() / 2));
  controlLayout->addWidget(mCancelButton);
  connect(mCancelButton, &QPushButton::clicked, this,
          &ProfileGridWidget::onCancel);

  mSelectButton = new QPushButton(controlWidget);
  mSelectButton->setFont(Resources::instance().getMediumBoldButtonFont());
  mSelectButton->setIcon(
      Resources::instance()
          .getAwesome()
          ->icon(fa::check, Resources::instance().getIconOptions())
          .pixmap(32, 32));
  mSelectButton->setText("Chọn");
  mSelectButton->resize(buttonSize);
  mSelectButton->setMinimumSize(buttonSize);
  mSelectButton->setMaximumSize(buttonSize);
  //  mSelectButton->setStyleSheet(utils.borderStyleSheet(
  //      colors.EMERALD, colors.WHITE, 0, mSelectButton->height() / 2));
  controlLayout->addWidget(mSelectButton);
  connect(mSelectButton, &QPushButton::clicked, this,
          &ProfileGridWidget::onSelect);

  mSelectionLayout->addWidget(controlWidget);

  mSelectionLabel = new QLabel(mSelectionWidget);
  mSelectionWidget->hide();
}

void ProfileGridWidget::filterWithKeyword(QString keyword) { Q_UNUSED(keyword) }

void ProfileGridWidget::showSelectionWidget(int index) {
  if (!mIsAnimating) {
    mCurrentProfileCellIndex = index;
    for (int index = 0; index < mProfileCellWidgets.size(); ++index) {
      mProfileCellWidgets.at(index)->setEnabledButton(false);
    }
    mSelectionWidget->show();
    updateSelectionWidgetPosition();
    startAnimation(true);
  }
}

void ProfileGridWidget::startAnimation(bool open) {
  mIsAnimating = true;
  mIsOpen = open;
  if (mIsOpen) {
    mCamPositionListView->clearSelection();
    mSearchLineEdit->setFocus();
    startSearch();
  } else {
    stopSearch();
  }
  QPropertyAnimation *animation =
      new QPropertyAnimation(mSelectionWidget, "geometry");
  animation->setDuration(100);

  int sourceWidth = open ? mSelectionWidget->minimumWidth()
                         : mSelectionWidget->maximumWidth();
  int sourceHeight = open ? mSelectionWidget->minimumHeight()
                          : mSelectionWidget->maximumHeight();

  int sourceX = (width() - sourceWidth) / 2;
  int sourceY = (height() - sourceHeight) / 2;

  int targetWidth = open ? mSelectionWidget->maximumWidth()
                         : mSelectionWidget->minimumWidth();
  int targetHeight = open ? mSelectionWidget->maximumHeight()
                          : mSelectionWidget->minimumHeight();

  int targetX = (width() - targetWidth) / 2;
  int targetY = (height() - targetHeight) / 2;

  animation->setStartValue(QRect(sourceX, sourceY, sourceWidth, sourceHeight));
  animation->setEndValue(QRect(targetX, targetY, targetWidth, targetHeight));
  animation->start(QAbstractAnimation::DeleteWhenStopped);
  connect(animation, &QPropertyAnimation::finished, this,
          &ProfileGridWidget::onAnimationEnd);
}

void ProfileGridWidget::onAnimationEnd() {
  mIsAnimating = false;
  QPropertyAnimation *animation = qobject_cast<QPropertyAnimation *>(sender());
  disconnect(animation, &QPropertyAnimation::finished, this,
             &ProfileGridWidget::onAnimationEnd);
  if (!mIsOpen) {
    mSelectionWidget->hide();
  }
}

void ProfileGridWidget::updateSelectionWidgetPosition() {
  int x = (width() - mSelectionWidget->width()) / 2;
  int y = (height() - mSelectionWidget->height()) / 2;
  mSelectionWidget->move(x, y);
}

void ProfileGridWidget::hideSelectionWidget() {
  if (!mIsAnimating) {
    for (int index = 0; index < mProfileCellWidgets.size(); ++index) {
      mProfileCellWidgets.at(index)->setEnabledButton(true);
    }
    startAnimation(false);
  }
}

void ProfileGridWidget::updateCurrentPageLayout() {
  mCurrentLayout = mProfilePage->getLayout();
  mCurrentSize = (int)sqrt(mCurrentLayout);
  int currentLayout = mProfileCellWidgets.size();
  int delta = mCurrentLayout - currentLayout;
  QList<int> currentIds = mProfilePage->getCamIds();
  if (delta > 0) {
    qDebug() << "last_selected_ids" << currentIds;
    for (int index = 0; index < delta; ++index) {
      ProfileCellWidget *cell = mPoolCellWidgets.last();
      cell->show();
      cell->setIndex(mProfileCellWidgets.size());
      mProfileCellWidgets.append(cell);
      mPoolCellWidgets.removeLast();
      mGridLayout->removeWidget(cell);
      if (currentIds.size() < mCurrentLayout) {
        currentIds.append(0);
      }
    }
  } else {
    for (int index = 0; index < abs(delta); ++index) {
      ProfileCellWidget *cell = mProfileCellWidgets.last();
      mPoolCellWidgets.append(cell);
      mProfileCellWidgets.removeLast();
      mGridLayout->removeWidget(cell);
      cell->hide();
      cell->setAdded(false);
      //      cell->setText(strings.noData);
      if (currentIds.size() > mCurrentLayout) {
        int key = currentIds.last();
        if (key > 0) {
          mCurrentOriginalHash.insert(key, mOriginalHash.value(key));
          mCurrentMinimalHash.insert(key, mMinimalHash.value(key));
          mSelectedOriginalHash.remove(key);
          mSelectedMinimalHash.remove(key);
        }
        currentIds.removeLast();
      }
    }
  }
  mProfilePage->setIds(currentIds);
}

void ProfileGridWidget::resizeEvent(QResizeEvent *event) {
  updateSelectionWidgetPosition();
  Q_UNUSED(event)
}

QPushButton *ProfileGridWidget::createPageButton() {
  QPushButton *newPageButton = new QPushButton(mPageWidget);
  newPageButton->setFixedSize(50, 50);
  newPageButton->setFont(Resources::instance().getLargeBoldButtonFont());
  newPageButton->setText(QString::number(++mTotalPage));
  connect(newPageButton, &QPushButton::clicked, this,
          &ProfileGridWidget::onPageButtonClicked);
  return newPageButton;
}

void ProfileGridWidget::setCamSite(CamSite *camSite) {
  mCamSite = camSite;
  mOriginalHash.clear();
  mMinimalHash.clear();
  //  QList<CamItem *> *camItems = camSite->getCamItems();
  //  for (int index = 0; index < camItems->size(); ++index) {
  //    CamItem *camItem = camItems->at(index);
  //    mOriginalHash.insert(camItem->getCameraId(), camItem->getPostion());
  //    mMinimalHash.insert(camItem->getCameraId(), camItem->getMinimal());
  //  }
  //  mCamPositionListView->clearSelection();
  //  if (mIsCreateNew) {
  //    addPage();
  //  } else {
  //    mTotalPage = 0;
  //    for (int index = 0; index < mProfileBook->getProfilePages().size();
  //         ++index) {
  //      QPushButton *newPageButton = createPageButton();
  //      mPageLayout->addWidget(newPageButton);
  //      mPageButtons.append(newPageButton);
  //    }

  //    mProfilePage = mProfileBook->getProfilePages().at(0);
  //    mCurrentPage = mProfilePage->getPage();
  //    mCurrentLayout = mProfilePage->getLayout();

  //    mNameLineEdit->setText(mProfileBook->getName());
  //    updatePageWidgetSize();
  //    updatePageButtons();
  //    updateCurrentPageLayoutButtons();
  //    updateCurrentPageData();
  //    updateCurrentPageLayout();
  //    displayCurrentPage();
  //  }
}

void ProfileGridWidget::updateCurrentPageData() {
  if (mCurrentPage >= 1 && mCurrentPage <= mTotalPage) {
    mCamPositionListView->clearSelection();
    mMaxLengthKeyword = 0;
    mCurrentOriginalHash.clear();
    mCurrentMinimalHash.clear();
    mSelectedOriginalHash.clear();
    mSelectedMinimalHash.clear();
    for (int index = 0; index < mOriginalHash.size(); ++index) {
      int key = mOriginalHash.keys().at(index);
      mCurrentOriginalHash.insert(key, mOriginalHash.value(key));
      QString minimal = mMinimalHash.value(key);
      mCurrentMinimalHash.insert(key, minimal);
      if (minimal.length() > mMaxLengthKeyword) {
        mMaxLengthKeyword = minimal.length();
      }
    }
    QList<int> ids = mProfilePage->getCamIds();
    for (int index = 0; index < ids.size(); ++index) {
      int key = ids.at(index);
      if (key > 0) {
        mSelectedOriginalHash.insert(key, mOriginalHash.value(key));
        mSelectedMinimalHash.insert(key, mMinimalHash.value(key));

        mCurrentOriginalHash.remove(key);
        mCurrentMinimalHash.remove(key);
      }
    }
    mCamPositions->setStringList(mCurrentOriginalHash.values());
  }
}

void ProfileGridWidget::initProfileBook(int userId, int siteId, int index) {
  mProfileBook = new ProfileBook(NULL);
  mIsCreateNew = true;
  mProfileBookIndex = index;
  mProfileBook->setUserId(userId);
  mProfileBook->setSiteId(siteId);
  mTotalPage = 0;
  mCurrentPage = 0;
  mCurrentLayout = 0;
}

void ProfileGridWidget::setProfileBook(ProfileBook *profileBook, int index) {
  mProfileBook = profileBook;
  mIsCreateNew = false;
  mProfileBookIndex = index;
  mTotalPage = 0;
  mCurrentPage = 0;
  mCurrentLayout = 0;
}

void ProfileGridWidget::addPage() {
  mCurrentPage = mTotalPage + 1;
  mCurrentLayout = mDefaultLayout;

  QList<ProfilePage *> currentProfilePages = mProfileBook->getProfilePages();
  mProfilePage = new ProfilePage(mProfileBook);
  mProfilePage->setPage(mCurrentPage);
  mProfilePage->setLayout(mCurrentLayout);
  currentProfilePages.append(mProfilePage);
  mProfileBook->setProfilePages(currentProfilePages);

  QPushButton *newPageButton = createPageButton();
  mPageLayout->addWidget(newPageButton);
  mPageButtons.append(newPageButton);
  updatePageWidgetSize();

  updatePageButtons();
  updateCurrentPageLayoutButtons();
  updateCurrentPageData();
  updateCurrentPageLayout();
  displayCurrentPage();
}

void ProfileGridWidget::onPageButtonClicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  int index = mPageButtons.indexOf(button);
  mCurrentPage = index + 1;
  mProfilePage = mProfileBook->getProfilePages().at(mCurrentPage - 1);
  mCurrentLayout = mProfilePage->getLayout();
  updatePageButtons();
  updateCurrentPageLayoutButtons();
  updateCurrentPageData();
  updateCurrentPageLayout();
  displayCurrentPage();
}

void ProfileGridWidget::displayCurrentPage() {
  for (int index = 0; index < mProfileCellWidgets.size(); ++index) {
    ProfileCellWidget *profileCellWidget = mProfileCellWidgets.at(index);
    mGridLayout->removeWidget(profileCellWidget);
    profileCellWidget->hide();
    profileCellWidget->setAdded(false);
    //        profileCellWidget->setText(strings.noData);
  }
  QList<int> ids = mProfilePage->getCamIds();
  for (int index = 0; index < mProfileCellWidgets.size(); ++index) {
    if (index < ids.size()) {
      ProfileCellWidget *profileCellWidget = mProfileCellWidgets.at(index);
      int row = index / mCurrentSize;
      int col = index - row * mCurrentSize;
      mGridLayout->addWidget(profileCellWidget, row, col);
      profileCellWidget->show();
      int key = ids.at(index);
      if (key > 0) {
        profileCellWidget->setAdded(true);
        profileCellWidget->setText(mOriginalHash.value(key));
      } else {
        profileCellWidget->setAdded(false);
        //                profileCellWidget->setText(strings.noData);
      }
    }
  }
  int duration = mProfilePage->getDuration();
  if (duration > 0) {
    mTransitionComboBox->setCurrentIndex(1);
    mDurationSpinBox->show();
    mDurationSpinBox->setValue(duration);
  } else {
    mTransitionComboBox->setCurrentIndex(0);
    mDurationSpinBox->hide();
    mDurationSpinBox->setValue(0);
  }
}

int ProfileGridWidget::getTotalPage() { return mTotalPage; }

int ProfileGridWidget::getCurrentPage() { return mCurrentPage; }

int ProfileGridWidget::getCurrentLayout() { return mCurrentLayout; }

ProfileBook *ProfileGridWidget::getCurrentProfileBook() { return mProfileBook; }

void ProfileGridWidget::onApplyButtonClicked() {
  QString name = mNameLineEdit->text();
  if (name.isEmpty()) {
    name = QString("Cấu hình %1").arg(mProfileBookIndex);
  }
  mProfileBook->setName(name);
  if (mIsCreateNew) {
    Q_EMIT createdProfileBook(mProfileBook);
  } else {
    Q_EMIT updatedProfileBook(mProfileBook);
  }
}
