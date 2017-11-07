#include "p_about.h"

/**
     * Generic method to override for updating the presention.
     **/

P_About::P_About(Control *ctrl, QWidget *zone) : Presentation(ctrl) {
    QSize screenSize = Resources::instance().getScreenSize();
    // init gui object
    this->zone = zone;
    //    this->zone->setStyleSheet("background: #EEEAE6;color:black;");
    this->zone->setStyleSheet("background-color:#edf0f5;border:0px solid #8a8a92;border-radius :5px;");
//    this->zone->move((screenSize.width() - this->zone->width()) / 2,
//                     (screenSize.height() - this->zone->height()) / 2);

    this->zoneLayout = new QVBoxLayout();
    this->zoneLayout->setMargin(0);
    this->zoneLayout->setSpacing(0);
    this->zoneLayout->setAlignment(Qt::AlignTop);
    this->zone->setLayout(this->zoneLayout);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this->zone);
    effect->setBlurRadius(80);
    effect->setColor(QColor("#000000"));
    effect->setOffset(0,0);
    this->zone->setGraphicsEffect(effect);
    this->zone->setFixedSize(QSize(500,335));
    this->zone->resize(500,300);

    //init title
    this->topWidget = new QWidget(this->zone);
    this->topWidget->setFixedSize(this->zone->width(), 35);

    this->topWidget->setStyleSheet(
                "background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 "
                "#eff0f5, stop: 1 #dedede); border: 0px solid #8a8a92; "
                "border-top-left-radius: 5px; border-top-right-radius: 5px; "
                "border-bottom-left-radius: 0px; border-bottom-right-radius: 0px;");
    //  this->topWidget->move(0, 0);
    //  this->topWidget->resize(this->zone->width(), 100);

    this->topLayout = new QVBoxLayout();
    this->topLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->topLayout->setSpacing(5);
    this->topLayout->setContentsMargins(0, 0, 0, 0);

    this->topWidget->setLayout(this->topLayout);
    this->zoneLayout->addWidget(this->topWidget);
    initTitleZone();
    this->zone->setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    this->zone->size(),
                    qApp->desktop()->availableGeometry()
                    )
                );

    //containwidget chứa toàn bộ widget ở phần dưới
    QWidget *containWidget = new QWidget(this->zone);
    containWidget->setStyleSheet("background: #EEEAE6;color:black;");
    QVBoxLayout *containLayout = new QVBoxLayout();

    containWidget->setLayout(containLayout);
    containLayout->setSpacing(20);
    containLayout->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    this->zoneLayout->addWidget(containWidget); // add contain

    QWidget *topMainWidget = new QWidget(containWidget);
    QHBoxLayout *topMainLayout = new QHBoxLayout();
    topMainWidget->setLayout(topMainLayout);

    QWidget *bottomMainWidget = new QWidget(containWidget);
    QVBoxLayout *bottomMainLayout = new QVBoxLayout();
    bottomMainWidget->setLayout(bottomMainLayout);
    //addlayout
    containLayout->addWidget(topMainWidget);
    containLayout->addWidget(bottomMainWidget);


    QWidget *leftWidget = new QWidget(topMainWidget);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setAlignment(Qt::AlignTop);
    leftWidget->setLayout(leftLayout);

    QLabel *appLogo = new QLabel(leftWidget);
    appLogo->setFixedSize(100, 100);
    appLogo->setScaledContents(true);
    QPixmap pixMap(":/images/res/logo_vcam.png");
    appLogo->setPixmap(pixMap);
    leftLayout->addWidget(appLogo);

    //right
    QWidget *rightWidget = new QWidget(topMainWidget);
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(15);
    rightWidget->setLayout(rightLayout);
    rightLayout->setAlignment(Qt::AlignTop);

    QLabel *appNameLabel = new QLabel(rightWidget);
    appNameLabel->setText("Videowall");

    QFont font("Ubuntu", 23, QFont::Bold);
    //    appNameLabel->setFont(Resources::instance().getExtraLargeRegularButtonFont());
    appNameLabel->setFont(font);
    rightLayout->addWidget(appNameLabel);
    //    QString vesionApp = AppProfile::getAppProfile()->getAppConfig()->getAppVersion();
    QSettings settings;
    settings.beginGroup(QString::number(control()->appContext->getWorkingUser()->getUserId()));
    QString vesionApp =  settings.value("app_version").toString();
    QLabel *versionLabel = new QLabel(rightWidget);
    versionLabel->setText("Phiên bản : " + vesionApp);
    rightLayout->addWidget(versionLabel);

    QFont fontLabel("Ubuntu", 12, QFont::Normal);

    //Date build

    QLabel *labelDateBuild = new QLabel(rightWidget);
    labelDateBuild->setFont(fontLabel);
    //    QString dateBuildApp = AppProfile::getAppProfile()->getAppConfig()->getDateBuild();
    QString dateBuildApp =  settings.value("date_build").toString();
    labelDateBuild->setText("Ngày cập nhật : " + dateBuildApp);
    rightLayout->addWidget(labelDateBuild);


    QLabel *featureLabel = new QLabel(rightWidget);
    featureLabel->setFont(fontLabel);
    featureLabel->setWordWrap(true);
    featureLabel->setText("Chức năng : Xem trực tiếp, xem lại, trích xuất video, tua nhanh, chụp hình ... ");
    rightLayout->addWidget(featureLabel);

    //add widget
    topMainLayout->addWidget(leftWidget);
    topMainLayout->addWidget(rightWidget);

    //bottom

    QWidget *bottomWidget = new QWidget(containWidget);
    bottomWidget->setFixedHeight(50);
    QHBoxLayout *layoutBottom =  new QHBoxLayout();
    layoutBottom->setSpacing(10);
    bottomWidget->setLayout(layoutBottom);
    bottomMainLayout->addWidget(bottomWidget);

    QLabel *copyRightLabel = new QLabel(bottomWidget);
    QFont fontRight("Ubuntu", 12, QFont::Bold);
    copyRightLabel->setFont(fontRight);
    copyRightLabel->setText("2017 © bản quyền thuộc về Vcam.");

    //bottom right
    QWidget *bottomRightWidget = new QWidget(bottomWidget);
    bottomRightWidget->setFixedHeight(50);
    QHBoxLayout *bottomRightLayout = new QHBoxLayout();
    bottomRightLayout->setSpacing(10);
    bottomRightLayout->setAlignment(Qt::AlignRight);
    bottomRightWidget->setLayout(bottomRightLayout);

    QPushButton *closeAboutAppButton = new QPushButton(bottomRightWidget);
    closeAboutAppButton->setStyleSheet("background-color:#DEDEDE; border-radius:2px");
    closeAboutAppButton->setFixedSize(80,35);
    connect(closeAboutAppButton, &QPushButton::clicked, this , [this]{
        this->zone->hide();
    });

    closeAboutAppButton->setText("Đóng");
    bottomRightLayout->addWidget(closeAboutAppButton);
    layoutBottom->addWidget(copyRightLabel);
    layoutBottom->addWidget(bottomRightWidget);
    bottomMainLayout->addWidget(bottomWidget);
    this->zone->hide();
}

void P_About::initTopZone() {

}


void P_About::initTitleZone() {
    titleWidget = new QWidget(this->topWidget);
    titleWidget->setFixedSize(this->topWidget->size());
    titleWidget->setStyleSheet("background-color: #00000000");
//    titleWidget->installEventFilter(this);
//    titleWidget->setMouseTracking(true);
//    titleWidget->setAttribute(Qt::WA_Hover);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setAlignment(Qt::AlignLeft);
    titleLayout->setSpacing(10);
    titleLayout->setContentsMargins(8, 10, 10, 8);
    titleWidget->setLayout(titleLayout);

    this->closeButton =
            createButton(titleWidget, QSize(16, 16), QRect(8, 8, 8, 8), 0,
                         "#aaff3b30", "#ff3b30", "#ff3b30", "");
    this->closeButton->setIcon(QIcon(":/images/res/icon_tab_close.png"));
    this->closeButton->setIconSize(QSize(10, 10));
    connect(this->closeButton, &QPushButton::clicked, this,
            &P_About::onCloseButtonClicked);

    this->minimizeButton =
            createButton(titleWidget, QSize(16, 16), QRect(8, 8, 8, 8), 0,
                         "#aaffcc00", "#ffcc00", "#ffcc00", "");
    this->minimizeButton->setIcon(QIcon(":/images/res/icon_tab_minimize.png"));
    this->minimizeButton->setIconSize(QSize(10, 10));
    connect(this->minimizeButton, &QPushButton::clicked, this,
            &P_About::onMinimizeButtonClicked);

    this->maximizeButton =
            createButton(titleWidget, QSize(16, 16), QRect(8, 8, 8, 8), 0,
                         "#aa4cd964", "#4cd964", "#4cd964", "");
    this->maximizeButton->setIcon(QIcon(":/images/res/icon_tab_maximize.png"));
    this->maximizeButton->setIconSize(QSize(10, 10));
    connect(this->maximizeButton, &QPushButton::clicked, this,
            &P_About::onMaximizeButtonClicked);

    this->titleLabel = new QLabel(titleWidget);
    this->titleLabel->setMinimumWidth(this->topWidget->width() - 110);
    this->titleLabel->setAlignment(Qt::AlignCenter);
    this->titleLabel->setFont(Resources::instance().getLargeBoldButtonFont());
    this->titleLabel->setStyleSheet(
                "background-color: #00000000; color: #1e1e1e");
    this->titleLabel->setText("Giới thiệu ứng dụng Videowall");

    titleLayout->addWidget(this->closeButton);
    titleLayout->addWidget(this->minimizeButton);
    titleLayout->addWidget(this->maximizeButton);
    titleLayout->addWidget(this->titleLabel);

    this->topLayout->addWidget(titleWidget);
}

void P_About::onCloseButtonClicked(){
    this->zone->hide();
}

void P_About::onMinimizeButtonClicked(){

}

void P_About::onMaximizeButtonClicked(){

}

//bool P_About::eventFilter(QObject *watched, QEvent *event) {
//  QWidget *widget = (QWidget *)watched;
//  if (widget == titleWidget) {
//      qDebug() << "EVEntFilter";
//    QMouseEvent *mouseEvent = (QMouseEvent *)event;
//    if (mouseEvent) {
//      if (mouseEvent->type() == QMouseEvent::MouseButtonPress) {
//        oldPos = mouseEvent->globalPos();
//        isMoving = true;
//      }
//      if (mouseEvent->type() == QMouseEvent::MouseMove) {
//        if (isMoving) {
//          const QPoint delta = mouseEvent->globalPos() - oldPos;
//          this->zone->move(this->zone->x() + delta.x(),
//                           this->zone->y() + delta.y());
//          oldPos = mouseEvent->globalPos();
//        }
//      }
//      if (mouseEvent->type() == QMouseEvent::MouseButtonRelease) {
//        isMoving = false;
//      }
//    }
//  }
//  return true;
//}




void P_About::update() {}

QObject *P_About::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return Q_NULLPTR;
    default:
        return Q_NULLPTR;
    }
}
void P_About::showAboutApp(){
    this->zone->show();
    this->zone->raise();
}

/**
     * Method to create a new presentation exactly equals (field by field) to
  *this presentation.
     * @return the created clone
     **/
//    public Presentation getClone(){
//        try{
//            return (Presentation)clone();
//        } catch(Exception e) {System.out.println("ERROR: can't duplicate a
//        presentation.");}
//        return null;
//    }
