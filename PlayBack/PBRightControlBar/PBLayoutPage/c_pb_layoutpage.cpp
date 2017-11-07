#include "c_pb_layoutpage.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_PBLayoutPage::C_PBLayoutPage(Control* ctrl, QWidget* zone) : Control(ctrl) {
    QVariant* dataStruct = new QVariant();
    getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
    this->appContext = dataStruct->value<AppContext*>();

    this->pres = new P_PBLayoutPage(this, zone);
    this->zone = zone;

    cLayoutSelector = new C_PBLayoutSelector(
                this,
                presentation()->getZone(Message.APP_PLAY_BACK_ZONE_LAYOUT_SELECTOR));

    cPageSelector = new C_PBPageSelector(
                this, presentation()->getZone(Message.APP_PLAY_BACK_ZONE_PAGE_SELECTOR));

    cPlayListOfDay = new C_PBPlayListOfDay(
                this,
                presentation()->getZone(
                    Message.APP_PLAY_BACK_ZONE_DATA_MAP_PLAYLIST_OF_DAY));
    cCalendar = new C_PBCalendar(
                this,
                presentation()->getZone(Message.APP_PLAY_BACK_ZONE_DATA_MAP_CALENDAR));
}

void C_PBLayoutPage::show(QVariant* attachment) { Q_UNUSED(attachment) }

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_PBLayoutPage::newUserAction(int message, QVariant* attachment) {
    Q_UNUSED(attachment)
    switch (message) {
    case Message.SHOW:
        break;

    case Message.APP_SHOW_SETTINGS: {
        getParent()->newAction(message, attachment);
    } break;

    default:
        qDebug() << "ERROR : General User action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}

/**
     * Method to receive a message from the Astraction Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_PBLayoutPage::newSystemAction(int message, QVariant* attachment) {
    Q_UNUSED(attachment)
    switch (message) {
    case 1:
        break;

    default:
        qDebug() << "ERROR : General System action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}

/**
     * Method to receive a message from an other agent.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_PBLayoutPage::newAction(int message, QVariant* attachment) {
    switch (message) {

    case Message.APP_PLAY_BACK_CALENDAR_MAX_HISTORYDAY:{
        cCalendar->newAction(message, attachment);
    }break;

    case Message.APP_PLAY_BACK_UPDATE_DATAMAP_HOURS:{
       cPlayListOfDay->newAction(message,attachment);
    }break;

    case Message.APP_SHOW:
    case Message.APP_CHANGED: {
        cCalendar->newAction(message,attachment);
    }break;

    case Message.APP_PLAY_BACK_UPDATE_DATAMAP_CALENDAR:{
        cCalendar->newAction(message,attachment);
    }break;

    case Message.APP_PLAY_BACK_SEEK_TO_NEW_POSITION:
        getParent()->newAction(message, attachment);
        break;

    case Message.APP_PLAY_BACK_ZONE_LAYOUT_SELECTED:
        this->cPageSelector->newAction(message, attachment);
        break;

    case Message.APP_PLAY_BACK_PAGE_DEFAULT_SET: {
        this->cPageSelector->newAction(message, attachment);
    } break;

    case Message.APP_CONTEXT_GET:
        attachment->setValue(this->appContext);
        break;

    case Message.APP_PLAY_BACK_ZONE_PAGE_SELECTED: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_LAYOUT_DEFAULT_SET:
        cLayoutSelector->newAction(message, attachment);
        break;

    case Message.SITE_CHANGED: {
        cCalendar->newAction(message,attachment);
        cLayoutSelector->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_CALENDAR_NEW_DAY_SELECTED: {
        QString selectedDateString =
                attachment->value<QDate>().toString("dd-MM-yyyy");

        QSettings settings;
        settings.beginGroup(QString::number(this->appContext->getWorkingUser()->getUserId()));
        settings.beginGroup("playback");
        settings.setValue("seleted_date", selectedDateString);

        getParent()->newAction(message, attachment);
        cPlayListOfDay->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_PlAYER_SHOW_FULL_SCREEN: {
        cLayoutSelector->newAction(message, attachment);
        cPageSelector->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_PlAYER_HIDE_FULL_SCREEN: {
        cLayoutSelector->newAction(message, attachment);
        cPageSelector->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_UPDATE_MAX_DATE_CALENDAR: {
        cCalendar->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_UPDATE_TIME_PLAYLISTOFDAY: {
        cPlayListOfDay->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_SELECTED_TIME_SLOT: {
        int seletedTimeSlot = attachment->value<int>();
        QSettings settings;
        settings.beginGroup(QString::number(this->appContext->getWorkingUser()->getUserId()));
        settings.beginGroup("playback");
        settings.setValue("selected_time_slot", seletedTimeSlot);
    } break;

    case Message.APP_PLAY_BACK_UPDATE_PAGE_DEFAULT: {
        getParent()->newAction(message, attachment);
    } break;

    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}
