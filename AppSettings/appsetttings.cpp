#include "appsetttings.h"

void AppSetttings::createSettingsDefaults(int userId)
{

//    QString server = AppProfile::getAppProfile()->getAppConfig()->getBrandName();
//    if(server == "") {
//        server == "VCAM";
//    }
    QSettings settings;
    settings.beginGroup(QString::number(userId));
    settings.setValue("app_version", "3.10.5");
    settings.setValue("date_build", "11/10/2017");
    settings.setValue("id_working_site", -1);
    settings.setValue("site_name", "");
    settings.setValue("path_save_media", "");
    settings.setValue("page_transition_start_mode", -1);
    settings.setValue("page_transition_delay", -1);

    settings.beginGroup("playback");
    settings.setValue("app_name", "playback");
    settings.setValue("selected_page", -1);
    settings.setValue("selected_layout", -1);
    settings.setValue("seleted_date", "");
    settings.setValue("selected_time_slot", -1);
    settings.setValue("selected_position", -1);
    settings.endGroup();

    settings.beginGroup("videowall");
    settings.setValue("selected_page", -1);
    settings.setValue("selected_layout", -1);
    settings.endGroup();

    settings.endGroup();
}

AppSetttings::AppSetttings()
{

}
