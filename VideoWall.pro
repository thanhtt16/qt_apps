#-------------------------------------------------
#
# Project created by QtCreator 2017-04-08T14:00:40
#
#-------------------------------------------------

QT += core gui network websockets avwidgets
LIBS += -lavutil -lavcodec -lavformat -lswscale
LIBS+=-lcrypto
LIBS+=-lssl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
TARGET = VideoWall
TEMPLATE = app
CONFIG += no_keywords
CONFIG += c++11
PRECOMPILED_HEADER = precompiled_header.h
CONFIG += precompile_header
QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig debug
PKGCONFIG += mpv
QMAKE_CXX = ccache g++

INCLUDEPATH += $$PWD/lib/websocketpp
INCLUDEPATH += $$PWD/lib/rapidjson/include
INCLUDEPATH += $$PWD/build/include

SOURCES += main.cpp \
    Authentication/appcontext.cpp \
    Authentication/changepasswordwidget.cpp \
    Authentication/connector.cpp \
    Authentication/sessionuser.cpp \
    Authentication/user.cpp \
    Camera/caminfo.cpp \
    Camera/camitem.cpp \
    Camera/camprofile.cpp \
    Camera/camsite.cpp \
    Camera/camstatistic.cpp \
    Camera/camstream.cpp \
    Camera/monitoringcamera.cpp \
    Common/appconfig.cpp \
    Common/AppListWidget.cpp \
    Common/appprofile.cpp \
    Common/appsettingwidget.cpp \
    Common/generaldefine.cpp \
    Common/LayoutSet.cpp \
    Common/logutils.cpp \
    Common/messagewidget.cpp \
    Common/networkutils.cpp \
    Common/resources.cpp \
    Common/touchwidget.cpp \
    MagicShow/ms_profilebook.cpp \
    MagicShow/ms_profilecellwidget.cpp \
    MagicShow/ms_profilegridwidget.cpp \
    MagicShow/ms_profilepage.cpp \
    MainFrame/Settings/GeneralSetting/a_setting_general.cpp \
    MainFrame/Settings/GeneralSetting/c_setting_general.cpp \
    MainFrame/Settings/GeneralSetting/p_setting_general.cpp \
    MainFrame/Settings/PlayBackSetting/a_setting_playback.cpp \
    MainFrame/Settings/PlayBackSetting/c_setting_playback.cpp \
    MainFrame/Settings/PlayBackSetting/p_setting_playback.cpp \
    MainFrame/Settings/VideoWallSetting/a_setting_videowall.cpp \
    MainFrame/Settings/VideoWallSetting/c_setting_videowall.cpp \
    MainFrame/Settings/VideoWallSetting/p_setting_videowall.cpp \
    MainFrame/Settings/a_settings.cpp \
    MainFrame/Settings/c_settings.cpp \
    MainFrame/Settings/p_settings.cpp \
    MainFrame/a_mainframe.cpp \
    MainFrame/c_mainframe.cpp \
    MainFrame/p_mainframe.cpp \
    PacAgentTemplate/a_classname.cpp \
    PacAgentTemplate/c_classname.cpp \
    PacAgentTemplate/p_classname.cpp \
    PacModel/abstraction.cpp \
    PacModel/control.cpp \
    PacModel/presentation.cpp \
    Site/changesite.cpp \
    Site/site.cpp \
    Site/sitechild.cpp \
    src/internal/sio_client_impl.cpp \
    src/internal/sio_packet.cpp \
    src/sio_client.cpp \
    src/sio_socket.cpp \
    TopControlBar/AppSelector/a_appselector.cpp \
    TopControlBar/AppSelector/c_appselector.cpp \
    TopControlBar/AppSelector/p_appselector.cpp \
    TopControlBar/ProfileSetting/a_profilesetting.cpp \
    TopControlBar/ProfileSetting/c_profilesetting.cpp \
    TopControlBar/ProfileSetting/p_profilesetting.cpp \
    TopControlBar/SiteSelector/a_siteselector.cpp \
    TopControlBar/SiteSelector/c_siteselector.cpp \
    TopControlBar/SiteSelector/p_siteselector.cpp \
    TopControlBar/a_topcontrolbar.cpp \
    TopControlBar/c_topcontrolbar.cpp \
    TopControlBar/p_topcontrolbar.cpp \
    VideoWall/VWRightControlBar/VWLayoutPage/VWLayoutSelector/a_vw_layoutselector.cpp \
    VideoWall/VWRightControlBar/VWLayoutPage/VWLayoutSelector/c_vw_layoutselector.cpp \
    VideoWall/VWRightControlBar/VWLayoutPage/VWLayoutSelector/p_vw_layoutselector.cpp \
    VideoWall/VWRightControlBar/VWLayoutPage/VWPageSelector/a_vw_pageselector.cpp \
    VideoWall/VWRightControlBar/VWLayoutPage/VWPageSelector/c_vw_pageselector.cpp \
    VideoWall/VWRightControlBar/VWLayoutPage/VWPageSelector/p_vw_pageselector.cpp \
    VideoWall/VWRightControlBar/VWLayoutPage/a_vw_layoutpage.cpp \
    VideoWall/VWRightControlBar/VWLayoutPage/c_vw_layoutpage.cpp \
    VideoWall/VWRightControlBar/VWLayoutPage/p_vw_layoutpage.cpp \
    VideoWall/VWRightControlBar/c_vw_rightcontrolbar.cpp \
    VideoWall/VWRightControlBar/p_vw_rightcontrolbar.cpp \
    VideoWall/VWWorkSpace/a_vw_workspace.cpp \
    VideoWall/VWWorkSpace/c_vw_workspace.cpp \
    VideoWall/VWWorkSpace/p_vw_workspace.cpp \
    VideoWall/a_videowall.cpp \
    VideoWall/c_videowall.cpp \
    VideoWall/p_videowall.cpp \
    Login/a_login.cpp \
    Login/c_login.cpp \
    Login/loginwidget.cpp \
    Login/p_login.cpp \
    MainFrame/Socket/a_socket.cpp \
    MainFrame/Socket/c_socket.cpp \
    MainFrame/Socket/p_socket.cpp \
    Player/Cam9Player/Cam9PlayerOverlay/a_cam9playeroverlay.cpp \
    Player/Cam9Player/Cam9PlayerOverlay/c_cam9playeroverlay.cpp \
    Player/Cam9Player/Cam9PlayerOverlay/p_cam9playeroverlay.cpp \
    Player/Cam9Player/a_cam9player.cpp \
    Player/Cam9Player/c_cam9player.cpp \
    Player/Cam9Player/p_cam9player.cpp \
    Player/cam9playerpool.cpp \
    Player/mpvwidget.cpp \
    PlayBack/PBRightControlBar/PBDataMapTab/PBPlayBackCalendar/a_pb_calendar.cpp \
    PlayBack/PBRightControlBar/PBDataMapTab/PBPlayBackCalendar/c_pb_calendar.cpp \
    PlayBack/PBRightControlBar/PBDataMapTab/PBPlayBackCalendar/p_pb_calendar.cpp \
    PlayBack/PBRightControlBar/PBDataMapTab/PBPlayListOfDay/a_pb_playlistofday.cpp \
    PlayBack/PBRightControlBar/PBDataMapTab/PBPlayListOfDay/c_pb_playlistofday.cpp \
    PlayBack/PBRightControlBar/PBDataMapTab/PBPlayListOfDay/p_pb_playlistofday.cpp \
    PlayBack/PBRightControlBar/PBDataMapTab/a_pb_datamaptab.cpp \
    PlayBack/PBRightControlBar/PBDataMapTab/c_pb_datamaptab.cpp \
    PlayBack/PBRightControlBar/PBDataMapTab/p_pb_datamaptab.cpp \
    PlayBack/PBRightControlBar/PBLayoutPage/PBLayoutSelector/a_pb_layoutselector.cpp \
    PlayBack/PBRightControlBar/PBLayoutPage/PBLayoutSelector/c_pb_layoutselector.cpp \
    PlayBack/PBRightControlBar/PBLayoutPage/PBLayoutSelector/p_pb_layoutselector.cpp \
    PlayBack/PBRightControlBar/PBLayoutPage/PBPageSelector/a_pb_pageselector.cpp \
    PlayBack/PBRightControlBar/PBLayoutPage/PBPageSelector/c_pb_pageselector.cpp \
    PlayBack/PBRightControlBar/PBLayoutPage/PBPageSelector/p_pb_pageselector.cpp \
    PlayBack/PBRightControlBar/PBLayoutPage/a_pb_layoutpage.cpp \
    PlayBack/PBRightControlBar/PBLayoutPage/c_pb_layoutpage.cpp \
    PlayBack/PBRightControlBar/PBLayoutPage/p_pb_layoutpage.cpp \
    PlayBack/PBRightControlBar/c_pb_rightcontrolbar.cpp \
    PlayBack/PBRightControlBar/p_pb_rightcontrolbar.cpp \
    PlayBack/PBWorkSpace/SeekBar/a_pb_seekbar.cpp \
    PlayBack/PBWorkSpace/SeekBar/c_pb_seekbar.cpp \
    PlayBack/PBWorkSpace/SeekBar/p_pb_seekbar.cpp \
    PlayBack/PBWorkSpace/a_pb_workspace.cpp \
    PlayBack/PBWorkSpace/c_pb_workspace.cpp \
    PlayBack/PBWorkSpace/p_pb_workspace.cpp \
    PlayBack/Recorder/a_pb_record.cpp \
    PlayBack/Recorder/c_pb_record.cpp \
    PlayBack/Recorder/datamapwidget.cpp \
    PlayBack/Recorder/downloadmanager.cpp \
    PlayBack/Recorder/p_pb_record.cpp \
    PlayBack/Recorder/recordvideovod.cpp \
    PlayBack/a_playback.cpp \
    PlayBack/c_playback.cpp \
    PlayBack/p_playback.cpp \
    PlayBack/videovod.cpp \
    PlayBack/videovodprofile.cpp \
    MainFrame/Network/a_network.cpp \
    MainFrame/Network/c_network.cpp \
    MainFrame/Network/p_network.cpp \
    AppSettings/appsetttings.cpp \
    DataMap/a_datamap.cpp \
    DataMap/c_datamap.cpp \
    DataMap/calendarcustomize.cpp \
    DataMap/datamapofcamera.cpp \
    DataMap/mypushbutton.cpp \
    DataMap/p_datamap.cpp \
    DataMap/seekbarwidget.cpp \
    Player/Cam9RTCPlayer/Cam9RTCPlayerOverlay/a_cam9rtcplayeroverlay.cpp \
    Player/Cam9RTCPlayer/Cam9RTCPlayerOverlay/c_cam9rtcplayeroverlay.cpp \
    Player/Cam9RTCPlayer/Cam9RTCPlayerOverlay/p_cam9rtcplayeroverlay.cpp \
    Player/Cam9RTCPlayer/a_cam9rtcplayer.cpp \
    Player/Cam9RTCPlayer/c_cam9rtcplayer.cpp \
    Player/Cam9RTCPlayer/p_cam9rtcplayer.cpp \
    Player/M3U8Playback/M3U8Linker.cpp \
    Player/M3U8Playback/m3u8toqueue.cpp \
    Player/M3U8Playback/PlaybackDownloader.cpp \
    Player/M3U8Playback/PlaybackImageQueue.cpp \
    Player/M3U8Playback/PlaybackMapQueue.cpp \
    Player/M3U8Playback/PlaybackPlayer.cpp \
    Player/M3U8Playback/PlaybackRender.cpp \
    Player/M3U8Playback/PlaybackRenderWidget.cpp \
    Player/M3U8Playback/PlaybackSocket.cpp \
    Player/M3U8Playback/PlaybackSplitter.cpp \
    Player/qtavplayer.cpp \
    MainFrame/AboutApp/a_about.cpp \
    MainFrame/AboutApp/c_about.cpp \
    MainFrame/AboutApp/p_about.cpp \
    Player/RTCPlayer/cam9imagequeue.cpp \
    Player/RTCPlayer/cam9m3u8queue.cpp \
    Player/RTCPlayer/cam9mapqueue.cpp \
    Player/RTCPlayer/LinkManager.cpp \
    Player/RTCPlayer/NbtpSocket.cpp \
    Player/RTCPlayer/RTCDownloader.cpp \
    Player/RTCPlayer/RTCPlayer.cpp \
    Player/RTCPlayer/RTCRender.cpp \
    Player/RTCPlayer/RTCRenderWidget.cpp \
    Player/RTCPlayer/RTCSocket.cpp \
    Player/RTCPlayer/RTCSplitter.cpp
 
HEADERS  += \
    Authentication/appcontext.h \
    Authentication/changepasswordwidget.h \
    Authentication/connector.h \
    Authentication/sessionuser.h \
    Authentication/user.h \
    Authentication/userapp.h \
    build/include/sio_client.h \
    build/include/sio_message.h \
    build/include/sio_socket.h \
    Common/appconfig.h \
    Common/AppListWidget.h \
    Common/appprofile.h \
    Common/appsettingwidget.h \
    Common/appstat.h \
    Common/generaldefine.h \
    Common/LayoutSet.h \
    Common/logutils.h \
    Common/messagewidget.h \
    Common/networkutils.h \
    Common/resources.h \
    Common/touchwidget.h \
    Common/utilities.h \
    MagicShow/ms_profilebook.h \
    MagicShow/ms_profilecellwidget.h \
    MagicShow/ms_profilegridwidget.h \
    MagicShow/ms_profilepage.h \
    MainFrame/Settings/GeneralSetting/a_setting_general.h \
    MainFrame/Settings/GeneralSetting/c_setting_general.h \
    MainFrame/Settings/GeneralSetting/p_setting_general.h \
    MainFrame/Settings/PlayBackSetting/a_setting_playback.h \
    MainFrame/Settings/PlayBackSetting/c_setting_playback.h \
    MainFrame/Settings/PlayBackSetting/p_setting_playback.h \
    MainFrame/Settings/VideoWallSetting/a_setting_videowall.h \
    MainFrame/Settings/VideoWallSetting/c_setting_videowall.h \
    MainFrame/Settings/VideoWallSetting/p_setting_videowall.h \
    MainFrame/Settings/a_settings.h \
    MainFrame/Settings/c_settings.h \
    MainFrame/Settings/p_settings.h \
    MainFrame/a_mainframe.h \
    MainFrame/c_mainframe.h \
    MainFrame/p_mainframe.h \
    PacAgentTemplate/a_classname.h \
    PacAgentTemplate/c_classname.h \
    PacAgentTemplate/p_classname.h \
    PacModel/abstraction.h \
    PacModel/control.h \
    PacModel/presentation.h \
    Site/changesite.h \
    Site/site.h \
    Site/sitechild.h \
    src/internal/sio_client_impl.h \
    src/internal/sio_packet.h \
    src/sio_client.h \
    src/sio_message.h \
    src/sio_socket.h \
    TopControlBar/AppSelector/a_appselector.h \
    TopControlBar/AppSelector/c_appselector.h \
    TopControlBar/AppSelector/p_appselector.h \
    TopControlBar/ProfileSetting/a_profilesetting.h \
    TopControlBar/ProfileSetting/c_profilesetting.h \
    TopControlBar/ProfileSetting/p_profilesetting.h \
    TopControlBar/SiteSelector/a_siteselector.h \
    TopControlBar/SiteSelector/c_siteselector.h \
    TopControlBar/SiteSelector/p_siteselector.h \
    TopControlBar/a_topcontrolbar.h \
    TopControlBar/c_topcontrolbar.cpp.Dr2375 \
    TopControlBar/c_topcontrolbar.h \
    TopControlBar/p_topcontrolbar.h \
    VideoWall/VWRightControlBar/VWLayoutPage/VWLayoutSelector/a_vw_layoutselector.h \
    VideoWall/VWRightControlBar/VWLayoutPage/VWLayoutSelector/c_vw_layoutselector.h \
    VideoWall/VWRightControlBar/VWLayoutPage/VWLayoutSelector/p_vw_layoutselector.h \
    VideoWall/VWRightControlBar/VWLayoutPage/VWPageSelector/a_vw_pageselector.h \
    VideoWall/VWRightControlBar/VWLayoutPage/VWPageSelector/c_vw_pageselector.h \
    VideoWall/VWRightControlBar/VWLayoutPage/VWPageSelector/p_vw_pageselector.h \
    VideoWall/VWRightControlBar/VWLayoutPage/a_vw_layoutpage.h \
    VideoWall/VWRightControlBar/VWLayoutPage/c_vw_layoutpage.h \
    VideoWall/VWRightControlBar/VWLayoutPage/p_vw_layoutpage.h \
    VideoWall/VWRightControlBar/c_vw_rightcontrolbar.h \
    VideoWall/VWRightControlBar/p_vw_rightcontrolbar.h \
    VideoWall/VWWorkSpace/a_vw_workspace.h \
    VideoWall/VWWorkSpace/c_vw_workspace.h \
    VideoWall/VWWorkSpace/p_vw_workspace.h \
    VideoWall/a_videowall.h \
    VideoWall/c_videowall.h \
    VideoWall/p_videowall.h \
    message.h \
    Login/a_login.h \
    Login/c_login.h \
    Login/loginwidget.h \
    Login/p_login.h \
    MainFrame/Socket/a_socket.h \
    MainFrame/Socket/c_socket.h \
    MainFrame/Socket/p_socket.h \
    Player/Cam9Player/Cam9PlayerOverlay/a_cam9playeroverlay.h \
    Player/Cam9Player/Cam9PlayerOverlay/c_cam9playeroverlay.h \
    Player/Cam9Player/Cam9PlayerOverlay/p_cam9playeroverlay.h \
    Player/Cam9Player/a_cam9player.h \
    Player/Cam9Player/c_cam9player.h \
    Player/Cam9Player/p_cam9player.h \
    Player/cam9playerpool.h \
    Player/mpvwidget.h \
    PlayBack/PBRightControlBar/PBDataMapTab/PBPlayBackCalendar/a_pb_calendar.h \
    PlayBack/PBRightControlBar/PBDataMapTab/PBPlayBackCalendar/c_pb_calendar.h \
    PlayBack/PBRightControlBar/PBDataMapTab/PBPlayBackCalendar/p_pb_calendar.h \
    PlayBack/PBRightControlBar/PBDataMapTab/PBPlayListOfDay/a_pb_playlistofday.h \
    PlayBack/PBRightControlBar/PBDataMapTab/PBPlayListOfDay/c_pb_playlistofday.h \
    PlayBack/PBRightControlBar/PBDataMapTab/PBPlayListOfDay/p_pb_playlistofday.h \
    PlayBack/PBRightControlBar/PBDataMapTab/a_pb_datamaptab.h \
    PlayBack/PBRightControlBar/PBDataMapTab/c_pb_datamaptab.h \
    PlayBack/PBRightControlBar/PBDataMapTab/p_pb_datamaptab.h \
    PlayBack/PBRightControlBar/PBLayoutPage/PBLayoutSelector/a_pb_layoutselector.h \
    PlayBack/PBRightControlBar/PBLayoutPage/PBLayoutSelector/c_pb_layoutselector.h \
    PlayBack/PBRightControlBar/PBLayoutPage/PBLayoutSelector/p_pb_layoutselector.h \
    PlayBack/PBRightControlBar/PBLayoutPage/PBPageSelector/a_pb_pageselector.h \
    PlayBack/PBRightControlBar/PBLayoutPage/PBPageSelector/c_pb_pageselector.h \
    PlayBack/PBRightControlBar/PBLayoutPage/PBPageSelector/p_pb_pageselector.h \
    PlayBack/PBRightControlBar/PBLayoutPage/a_pb_layoutpage.h \
    PlayBack/PBRightControlBar/PBLayoutPage/c_pb_layoutpage.h \
    PlayBack/PBRightControlBar/PBLayoutPage/p_pb_layoutpage.h \
    PlayBack/PBRightControlBar/c_pb_rightcontrolbar.h \
    PlayBack/PBRightControlBar/p_pb_rightcontrolbar.h \
    PlayBack/PBWorkSpace/SeekBar/a_pb_seekbar.h \
    PlayBack/PBWorkSpace/SeekBar/c_pb_seekbar.h \
    PlayBack/PBWorkSpace/SeekBar/p_pb_seekbar.h \
    PlayBack/PBWorkSpace/a_pb_workspace.h \
    PlayBack/PBWorkSpace/c_pb_workspace.h \
    PlayBack/PBWorkSpace/p_pb_workspace.h \
    PlayBack/Recorder/a_pb_record.h \
    PlayBack/Recorder/c_pb_record.h \
    PlayBack/Recorder/datamapwidget.h \
    PlayBack/Recorder/downloadmanager.h \
    PlayBack/Recorder/p_pb_record.h \
    PlayBack/Recorder/recordvideovod.h \
    PlayBack/a_playback.h \
    PlayBack/c_playback.h \
    PlayBack/p_playback.h \
    PlayBack/videovod.h \
    PlayBack/videovodprofile.h \
    MainFrame/Network/a_network.h \
    MainFrame/Network/c_network.h \
    MainFrame/Network/p_network.h \
    AppSettings/appsetttings.h \
    DataMap/a_datamap.h \
    DataMap/c_datamap.h \
    DataMap/calendarcustomize.h \
    DataMap/datamapofcamera.h \
    DataMap/mypushbutton.h \
    DataMap/p_datamap.h \
    DataMap/seekbarwidget.h \
    Player/Cam9RTCPlayer/Cam9RTCPlayerOverlay/a_cam9rtcplayeroverlay.h \
    Player/Cam9RTCPlayer/Cam9RTCPlayerOverlay/c_cam9rtcplayeroverlay.h \
    Player/Cam9RTCPlayer/Cam9RTCPlayerOverlay/p_cam9rtcplayeroverlay.h \
    Player/Cam9RTCPlayer/a_cam9rtcplayer.h \
    Player/Cam9RTCPlayer/c_cam9rtcplayer.h \
    Player/Cam9RTCPlayer/p_cam9rtcplayer.h \
    Player/M3U8Playback/M3U8Linker.h \
    Player/M3U8Playback/m3u8toqueue.h \
    Player/M3U8Playback/PlaybackDownloader.h \
    Player/M3U8Playback/PlaybackImageQueue.h \
    Player/M3U8Playback/PlaybackMapQueue.h \
    Player/M3U8Playback/PlaybackPlayer.h \
    Player/M3U8Playback/PlaybackRender.h \
    Player/M3U8Playback/PlaybackRenderWidget.h \
    Player/M3U8Playback/PlaybackSocket.h \
    Player/M3U8Playback/PlaybackSplitter.h \
    Player/qtavplayer.h \
    MainFrame/AboutApp/a_about.h \
    MainFrame/AboutApp/c_about.h \
    MainFrame/AboutApp/p_about.h \
    Player/RTCPlayer/cam9imagequeue.h \
    Player/RTCPlayer/cam9m3u8queue.h \
    Player/RTCPlayer/cam9mapqueue.h \
    Player/RTCPlayer/LinkManager.h \
    Player/RTCPlayer/NbtpSocket.h \
    Player/RTCPlayer/RTCDownloader.h \
    Player/RTCPlayer/RTCPlayer.h \
    Player/RTCPlayer/RTCRender.h \
    Player/RTCPlayer/RTCRenderWidget.h \
    Player/RTCPlayer/RTCSocket.h \
    Player/RTCPlayer/RTCSplitter.h

include(QtAwesome/QtAwesome.pri)

#macx {
#    QMAKE_CXXFLAGS += -std=c++11
#    _BOOST_PATH = /usr/local/Cellar/boost/1.64.0_1
#    INCLUDEPATH += "$${_BOOST_PATH}/include/"
#    LIBS += -L$${_BOOST_PATH}/lib
#    LIBS += -lboost_chrono-mt
#}

#CONFIG(release, debug|release): LIBS += -L$$PWD/build/lib/Release/ -lsioclient
#else:CONFIG(debug, debug|release): LIBS += -L$$PWD/build/lib/Debug/ -lsioclient

CONFIG(release, debug|release): LIBS += -L$$PWD/build/lib/Release/ -lboost_random
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/build/lib/Debug/ -lboost_random

CONFIG(release, debug|release): LIBS += -L$$PWD/build/lib/Release/ -lboost_system
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/build/lib/Debug/ -lboost_system

CONFIG(release, debug|release): LIBS += -L$$PWD/build/lib/Release/ -lboost_date_time
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/build/lib/Debug/ -lboost_date_time

RESOURCES += \
    images.qrc

SUBDIRS += \
    QtAwesome/QtAwesome.pro \
    QtAwesome/QtAwesome.pro

DISTFILES += \
    build/lib/Release/libboost_date_time.a \
    build/lib/Release/libboost_random.a \
    build/lib/Release/libboost_system.a \
    build/lib/Release/libcrypto.a \
    build/lib/Release/libsioclient.a \
    build/lib/Release/libsioclient_tls.a \
    build/lib/Release/libssl.a \

HEADERS += \
    Camera/caminfo.h \
    Camera/camitem.h \
    Camera/camprofile.h \
    Camera/camsite.h \
    Camera/camstatistic.h \
    Camera/camstream.h \
    Camera/monitoringcamera.h \
