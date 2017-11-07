#include "MainFrame/p_mainframe.h"
#include <QEvent>
#include <QStackedLayout>
#include <QTimer>
#include <QVBoxLayout>
#include "Login/loginwidget.h"
/**
     * Generic method to override for updating the presention.
     **/

P_MainFrame::P_MainFrame(Control *ctrl, QWidget *zone) : Presentation(ctrl) {
    zone->setStyleSheet("background-color: black; color: white;");
    zone->showFullScreen();
    // init top bar
    topBar = new QWidget(zone);
    topBar->setFixedHeight(appSize.topHeight);

    // init top bar
    topBarOverlay = new QWidget(zone);
    topBarOverlay->setFixedSize(0, 0);
    topBarOverlay->show();
    topBarOverlay->raise();
    topBarOverlay->move(0, 0);

    // init work space
    workSpaceForVideoWall = new QWidget(zone);
    workSpaceForPlayBack = new QWidget(zone);
    workSpaceForMagicShow = new QWidget(zone);

    // init main window
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    layout->setSpacing(0);
    layout->setMargin(0);
    zone->setLayout(layout);
    layout->addWidget(topBar);
    layout->addWidget(workSpaceForVideoWall);
    layout->addWidget(workSpaceForPlayBack);
    layout->addWidget(workSpaceForMagicShow);


    workSpaceForVideoWall->setMouseTracking(true);
    workSpaceForVideoWall->setAttribute(Qt::WA_Hover);
    workSpaceForVideoWall->installEventFilter(this);

    workSpaceForPlayBack->setMouseTracking(true);
    workSpaceForPlayBack->setAttribute(Qt::WA_Hover);
    workSpaceForPlayBack->installEventFilter(this);

    topBarOverlay->setMouseTracking(true);
    topBarOverlay->setAttribute(Qt::WA_Hover);
    topBarOverlay->installEventFilter(this);

    settingZone = new QWidget(zone);

    aboutApp = new QWidget(zone);

    loadingIndicator = new QWidget(zone);
    showApp(Message.HIDE);
    QSize screenSize = Resources::instance().getScreenSize();
    this->loadingIndicator->resize(screenSize.width() * 0.6f, screenSize.height() * 0.6f);
    this->loadingIndicator->move((screenSize.width() - this->loadingIndicator->width()) / 2,
                     (screenSize.height() - this->loadingIndicator->height()) / 2);

}

void P_MainFrame::update() {}

QWidget *P_MainFrame::getZone(int zoneId) {
    switch (zoneId) {
    case -1:
        return topBarOverlay;
    case 0:
        return topBar;
    case 1:
        return workSpaceForVideoWall;
    case 2:
        return workSpaceForPlayBack;
    case 3:
        return workSpaceForMagicShow;
    case 4:
        return siteTop;
    case 5:
        return settingZone;
    case 6:
        return loadingIndicator;
    case 7 :
        return aboutApp;
    default:
        return Q_NULLPTR;
    }
}

void P_MainFrame::showApp(int message) {
    switch (message) {
    case Message.HIDE:
    {
        workSpaceForMagicShow->hide();
        workSpaceForPlayBack->hide();
        workSpaceForVideoWall->hide();
        showApp(Message.HIDE_INDICATOR);
    }
        break;

    case Message.APP_VIDEO_WALL_SHOW:
    {
        workSpaceForMagicShow->hide();
        workSpaceForPlayBack->hide();
        workSpaceForVideoWall->show();
        showApp(Message.HIDE_INDICATOR);

    }
        break;

    case Message.APP_PLAY_BACK_SHOW:
    {      workSpaceForMagicShow->hide();
        workSpaceForVideoWall->hide();
        workSpaceForPlayBack->show();
        showApp(Message.HIDE_INDICATOR);

    }

        break;

    case Message.APP_MAGIC_SHOW_SHOW:
    {        workSpaceForPlayBack->hide();
        workSpaceForVideoWall->hide();
        workSpaceForMagicShow->show();
        showApp(Message.HIDE_INDICATOR);

    }
        break;

    case Message.SHOW_INDICATOR:
    {
        loadingIndicator->show();
        loadingIndicator->raise();
    }
        break;

    case Message.HIDE_INDICATOR:
    {
        loadingIndicator->hide();
        loadingIndicator->lower();

    }
        break;

    default:break;
    }
}

void P_MainFrame::enterFullscreenMode() {
    topBar->setFixedHeight(0);
    control()->newUserAction(Message.ENTER_FULLSCREEN_MODE, Q_NULLPTR);
}

void P_MainFrame::exitFullscreenMode() {
    topBar->setFixedHeight(appSize.topHeight);
    control()->newUserAction(Message.EXIT_FULLSCREEN_MODE, Q_NULLPTR);
}

bool P_MainFrame::eventFilter(QObject *watched, QEvent *event) {
    QWidget *widget = qobject_cast<QWidget *>(watched);
    if (widget == workSpaceForVideoWall) {
        switch (event->type()) {
        case QEvent::HoverEnter:
            //          hoverEnter(static_cast<QHoverEvent *>(event));
            return true;
            break;
        case QEvent::HoverLeave:
            //          hoverLeave(static_cast<QHoverEvent *>(event));
            return true;
            break;
        case QEvent::HoverMove: {
            QPoint point = workSpaceForVideoWall->mapFromGlobal(QCursor::pos());
            if (point.y() < 3) {
                //          exitFullscreenMode();
                topBarOverlay->setFixedSize(
                            Resources::instance().getScreenSize().width(), appSize.topHeight);
                topBarOverlay->show();
                topBarOverlay->raise();
            }
        } break;
        default:
            break;
        }
    }
    if (widget == workSpaceForPlayBack) {
        switch (event->type()) {
        case QEvent::HoverEnter:
            //          hoverEnter(static_cast<QHoverEvent *>(event));
            return true;
            break;
        case QEvent::HoverLeave:
            //          hoverLeave(static_cast<QHoverEvent *>(event));
            return true;
            break;
        case QEvent::HoverMove: {
            QPoint point = workSpaceForPlayBack->mapFromGlobal(QCursor::pos());
            if (point.y() < 3) {
                topBarOverlay->setFixedSize(
                            Resources::instance().getScreenSize().width(), appSize.topHeight);
                topBarOverlay->show();
                topBarOverlay->raise();
                // exitFullscreenMode();
            }
        } break;
        default:
            break;
        }
    }

    if (widget == topBarOverlay) {
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            //        topBarOverlay->setFixedSize(0, 0);
            //        exitFullscreenMode();
            break;
        case QEvent::HoverEnter:
            //          hoverEnter(static_cast<QHoverEvent *>(event));
            return true;
            break;
        case QEvent::HoverLeave: {
            if (isTopBarOverlayCanHide == true) {
                topBarOverlay->setFixedSize(0, 0);
            }
            return true;
        } break;
        case QEvent::HoverMove: {
        } break;
        default:
            break;
        }
    }
}

void P_MainFrame::topBarOverlayCanHide() {
    isTopBarOverlayCanHide = true;
    topBarOverlay->setFixedSize(0, 0);
}
void P_MainFrame::topBarOverlayCanNotHide() { isTopBarOverlayCanHide = false; }
