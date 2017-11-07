#include "VideoWall/p_videowall.h"
#include <QWidget>
/**
     * Generic method to override for updating the presention.
     **/

P_VideoWall::P_VideoWall(Control *ctrl, QWidget *zone) : Presentation(ctrl) {
    this->zone = zone;
    QSize screenSize = Resources::instance().getScreenSize();
    workSpace = new QWidget(zone);
    rightBar = new QWidget(zone);
    rightBar->setFixedWidth(appSize.rightWidth);

    // init work space
    layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setAlignment(Qt::AlignLeft);
    zone->setLayout(layout);
    layout->addWidget(workSpace);
    layout->addWidget(rightBar);

    //  this->zone->setMouseTracking(true);
    //  this->zone->setAttribute(Qt::WA_Hover);
    //  this->zone->installEventFilter(this);

    qDebug() << "P_VideoWall Success";
}

void P_VideoWall::show(QWidget *zone) { Q_UNUSED(zone) }

void P_VideoWall::update() {}

QWidget *P_VideoWall::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return workSpace;
    case 2:
        return rightBar;
    default:
        return Q_NULLPTR;
    }
}

void P_VideoWall::showRightBar() {}
void P_VideoWall::siteChanged() {}

void P_VideoWall::enterFullscreenMode() { rightBar->setFixedWidth(0); }

void P_VideoWall::exitFullscreenMode() {
    rightBar->setFixedWidth(appSize.rightWidth);
}

bool P_VideoWall::eventFilter(QObject *watched, QEvent *event) {
    //  QWidget *widget = qobject_cast<QWidget *>(watched);
    //  if (widget == this->zone) {
    //    switch (event->type()) {
    //      case QEvent::MouseButtonPress: {
    //      } break;
    //      case QEvent::HoverLeave: {
    //        QPoint point = this->zone->mapFromGlobal(QCursor::pos());
    //        qDebug() << "width"
    //                 << Resources::instance().getScreenSize().width() -
    //                        appSize.rightWidth
    //                 << point.x();
    //        if (point.x() < (Resources::instance().getScreenSize().width() -
    //                         appSize.rightWidth)) {
    //          control()->getParent()->newAction(Message.ENTER_FULLSCREEN_MODE,
    //                                            Q_NULLPTR);
    //        }
    //      } break;
    //      case QEvent::HoverMove: {
    //        QPoint point = this->zone->mapFromGlobal(QCursor::pos());
    //        if (point.x() > Resources::instance().getScreenSize().width() - 3) {
    //          control()->getParent()->newAction(Message.EXIT_FULLSCREEN_MODE,
    //                                            Q_NULLPTR);
    //        }
    //      } break;
    //      default:
    //        break;
    //    }
    //  }

    //  if (widget == rightBar) {
    //    switch (event->type()) {
    //      case QEvent::MouseButtonPress: {
    //        rightBar->setFixedWidth(3);
    //        control()->getParent()->newAction(Message.ENTER_FULLSCREEN_MODE,
    //                                          Q_NULLPTR);
    //      } break;
    //      case QEvent::HoverLeave:
    //        return true;
    //        break;
    //      case QEvent::HoverMove: {
    //        QPoint point = workSpace->mapFromGlobal(QCursor::pos());
    //        if (point.x() > Resources::instance().getScreenSize().width() - 3) {
    //          rightBar->setFixedWidth(appSize.rightWidth);
    //          control()->getParent()->newAction(Message.EXIT_FULLSCREEN_MODE,
    //                                            Q_NULLPTR);
    //        }
    //      } break;
    //      default:
    //        break;
    //    }
    //  }
}
