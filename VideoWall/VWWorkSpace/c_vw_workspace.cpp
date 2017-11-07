#include "c_vw_workspace.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
QString C_VWWorkSpace::getAppName() const { return appName; }

void C_VWWorkSpace::setAppName(const QString& value) { appName = value; }

C_VWWorkSpace::C_VWWorkSpace(Control* ctrl, QWidget* zone) : Control(ctrl) {
    // update app context
    QVariant* dataStruct = new QVariant();
    getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
    this->appContext = dataStruct->value<AppContext*>();

    // init facets
    this->abst = new A_VWWorkSpace(this);
    this->pres = new P_VWWorkSpace(this, zone);
    this->zone = zone;

    //    // create player controls
    // create player controls
    for (int index = 0; index < Message.APP_VIDEO_WALL_MAX_NUMBER_OF_PLAYERS;
         ++index) {
        QWidget* newZone = presentation()->getZone(index);
        Cam9PlayerPool::instance().getCam9RTCPlayer(this, newZone, appName,
                                                    QString::number(index));
    }
}
/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_VWWorkSpace::newUserAction(int message, QVariant* attachment) {
    Q_UNUSED(attachment)
    switch (message) {
    case Message.SHOW:
        break;

    case Message.APP_VIDEO_WALL_GET_CAMERAS_OF_SITE_WITH_LAYOUT_PAGE:{
        getParent()->newAction(message,attachment);
    }break;

    default:
        qDebug() << "ERROR : General User action in" << getClass()
                 << "non-catched :" << Message.toString(message);
    }
}

/**
     * Method to receive a message from the Astraction Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_VWWorkSpace::newSystemAction(int message, QVariant* attachment) {
    switch (message) {
    case Message.APP_CONTEXT_GET:
        attachment->setValue(this->appContext);
        break;

    default:
        qDebug() << "ERROR : General System action in" << getClass()
                 << "non-catched :" << Message.toString(message);
    }
}

/**
     * Method to receive a message from an other agent.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_VWWorkSpace::newAction(int message, QVariant* attachment) {
    switch (message) {
    case Message.UPDATE_CDN_TYPE_SELECTED:{
        CamItemType type = attachment->value<CamItemType>();
        this->presentation()->updateNetworkTypeSelected(type);
    }break;

    case Message.APP_NETWORK_IS_REACHABLE:
    {
        qDebug() << QDateTime::currentDateTime() << "APP_NETWORK_IS_REACHABLE";
        this->presentation()->networkChangeStatus(message, attachment);
    }
        break;
    case Message.APP_NETWORK_IS_UNREACHABLE:
    {
        qDebug() << QDateTime::currentDateTime() << "APP_NETWORK_IS_UNREACHABLE";
        this->presentation()->networkChangeStatus(message, attachment);
    }
        break;

    case Message.APP_SHOW:{
        if(!firstShow){
            presentation()->refreshAppShow();
        }
        firstShow = false;
    }
        break;

    case Message.APP_CHANGED:
        presentation()->stopPlaying();
        break;

    case Message.APP_CONTEXT_GET: {
        attachment->setValue(this->appContext);
    } break;

    case Message.APP_VIDEO_WALL_ZONE_PAGE_SELECTED: {
        LayoutStruct layout = attachment->value<LayoutStruct>();
        QSettings settings;
        settings.beginGroup(QString::number(this->appContext->getWorkingUser()->getUserId()));
        settings.beginGroup("videowall");
        settings.setValue("selected_layout",layout.code);
        settings.setValue("selected_page",layout.selectedPage);
        presentation()->refresh(layout);
    } break;

    case Message.APP_VIDEO_WALL_SHOW_SITE_TOP: {
        siteSelector->newAction(message, attachment);
    } break;

    case Message.PLAYER_BEGIN_SHOW_FULLSCREEN: {
        int index = attachment->value<int>();
        presentation()->showFullCellAt(index);
    } break;

    case Message.PLAYER_BEGIN_HIDE_FULLSCREEN: {
        int index = attachment->value<int>();
        presentation()->hideFullCellAt(index);
    } break;

    case Message.PLAYER_END_SHOW_FULLSCREEN: {
        C_Cam9Player* player = attachment->value<C_Cam9Player*>();
        if (player != Q_NULLPTR) {
            player->newAction(message, Q_NULLPTR);
        }
    } break;

    case Message.PLAYER_END_HIDE_FULLSCREEN: {
        C_Cam9Player* player = attachment->value<C_Cam9Player*>();
        if (player != Q_NULLPTR) {
            player->newAction(message, Q_NULLPTR);
        }
    } break;

    case Message.APP_VIDEO_WALL_UPDATE_PAGE_DEFAULT: {
        LayoutStruct defaultLayout = attachment->value<LayoutStruct>();
        QSettings settings;
        settings.beginGroup(QString::number(this->appContext->getWorkingUser()->getUserId()));
        settings.beginGroup("videowall");
        settings.setValue("selected_layout",defaultLayout.code);
        settings.setValue("selected_page",defaultLayout.selectedPage);
        presentation()->updateDefaulLayout(defaultLayout);
    } break;

    case Message.APP_VIDEO_WALL_LOAD_CAMERAS_OF_SITE_WITH_LAYOUT_SUCCESS:{
        presentation()->refreshNewLayoutPage();
    }break;

    case Message.APP_VIDEO_WALL_LOAD_CAMERAS_OF_SITE_WITH_SITE_ID_SUCCESS:{
        presentation()->refreshNewLayoutPage();
    }break;

    case Message.ENTER_FULLSCREEN_MODE :{
        presentation()->resizePlayerWhenEnterFullScreen();
    }break;

     case Message.EXIT_FULLSCREEN_MODE :{
        presentation()->resizePlayerWhenExitFullScreen();
    }break;


    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" << Message.toString(message);
    }
}
