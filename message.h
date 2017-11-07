#ifndef MESSAGE_H
#define MESSAGE_H

#include <QList>
#include <QString>

struct AppSize {
    int topHeight = 40;
    int rightWidth = 420;
};

struct AppMessage {

    QString VIDEO_WALL_USING_PLAYER = "Cam9RTCPlayer"; // Cam9RTCPlayer or Cam9Player
    const static int APP_VIDEO_WALL_MAX_NUMBER_OF_PLAYERS = 25;
    const static int APP_PLAY_BACK_MAX_NUMBER_OF_PLAYERS = 25;
    const static int UPDATE_CDN_TYPE_SELECTED = 1101;


    const static int LOGOUT = 2;
    const static int EXIT_APP = 3;
    const static int HIDE = 0;
    const static int SHOW = 1;
    const static int ENTER_FULLSCREEN_MODE = 4;
    const static int EXIT_FULLSCREEN_MODE = 5;

    const static int SHOW_INDICATOR = 6;
    const static int HIDE_INDICATOR = 7;
    const static int SHOW_ABOUT_APP = 8;
    const static int STOP_TIMER_RECHECK_NETWORK = 1989;

    const static int PLAYER_PLAY = 100;
    const static int PLAYER_STOP = 101;
    const static int PLAYER_PAUSE = 102;
    const static int CREATE_NEW_CAM9_RTC_PLAYER = 521;
    const static int CREATE_NEW_CAM9_PLAYER = 522;
    const static int GET_CAM9_PLAYER = 523;
    const static int GET_CAM9_RTC_PLAYER = 524;

    const static int PLAYER_NEW_LIVE_SOURCE_SET = 105;
    const static int PLAYER_NEW_VOD_SOURCE_SET = 106;
    const static int PLAYER_SOURCE_CLEAR = 107;
    const static int PLAYER_CHANGE_SOURCE = 108;
    const static int PLAYER_BEGIN_SHOW_FULLSCREEN = 109;
    const static int PLAYER_END_SHOW_FULLSCREEN = 110;
    const static int PLAYER_BEGIN_HIDE_FULLSCREEN = 111;
    const static int PLAYER_END_HIDE_FULLSCREEN = 112;
    const static int PLAYER_START_LOADING = 113;
    const static int PLAYER_STOP_LOADING = 114;
    const static int PLAYER_UPDATE_INFO = 115;
    const static int PLAYER_LOADING = 116;
    const static int PLAYER_PLAYING = 117;
    const static int PLAYER_STOPED = 118;
    const static int PLAYER_PAUSED = 119;
    const static int PLAYER_RECORD_NORMAL = 120;
    const static int PLAYER_RECORD_QUICK = 121;
    const static int PLAYER_TAKE_SCREENSHOT = 122;
    const static int PLAYER_PLAY_LIVE_SD = 123;
    const static int PLAYER_PLAY_LIVE_HD = 124;
    const static int PLAYER_PLAY_VOD_SD = 125;
    const static int PLAYER_PLAY_VOD_HD = 126;
    const static int PLAYER_PLAY_SD = 127;
    const static int PLAYER_PLAY_HD = 128;
    const static int PLAYER_ERROR = 139;
    const static int PLAYER_BLANK = 140;
    const static int PLAYER_PLAY_BACK_UPDATE_MODE = 141;
    const static int PLAYER_VIDEO_WALL_UPDATE_MODE = 142;

    const static int APP_PLAY_BACK_PlAYER_SHOW_FULL_SCREEN = 129;
    const static int APP_PLAY_BACK_PlAYER_HIDE_FULL_SCREEN = 130;
    const static int PLAYER_NEW_VOD_SOURCE_ONE_VIDEO_SET = 131;
    const static int APP_PLAY_BACK_RESET_PLAYER_SPEED = 132;
    const static int PLAYER_PLAY_SD_CLICK = 133;
    const static int PLAYER_PLAY_HD_CLICK = 134;
    const static int PLAYER_PLAY_EXIT_CLICK = 135;
    const static int PLAYER_PLAY_FULL_CLICK = 136;
    const static int PLAYER_EXIT_POP_OUT_MODE = 137;

    const static int PLAYER_PLAY_VOD_RTC = 138;

    const static int PAGE_TRANSITION_BEGIN = 200;
    const static int PAGE_TRANSITION_START_ON_CLICK = 201;
    const static int PAGE_TRANSITION_START_AUTOMATICALLY = 202;
    const static int PAGE_TRANSITION_DELAY = 204;

    const static int PROJECT_RUN = 100001;
    const static int PROJECT_EXIT = 100002;

    const static int SITE_CHANGED = 100101;
    const static int SITE_NEW_SELECTED = 100102;

    const static int APP_SHOW = 200001;
    const static int APP_HIDE = 200002;
    const static int APP_SHOW_SETTINGS = 200003;
    const static int APP_HIDE_SETTINGS = 200004;
    const static int APP_UPDATE_INDEX_SETTINGS = 200005;

    const static int APP_CHANGED = 200104;
    const static int APP_NEW_SELECTED = 200105;
    const static int TOP_CONTROL_BAR_HIDE = 300001;
    const static int TOP_CONTROL_BAR_GET_SITE_LIST = 300002;
    const static int GET_SIZE_TOP_CONTROL_BAR = 300003;

    const static int APP_VIDEO_WALL_SHOW = 400000;
    const static int APP_VIDEO_WALL_RIGHT_BAR_SHOW = 400001;
    const static int APP_VIDEO_WALL_ZONE_LAYOUT_PAGE = 400200;
    const static int APP_VIDEO_WALL_ZONE_LAYOUT_SELECTOR = 400210;
    const static int APP_VIDEO_WALL_ZONE_PAGE_SELECTOR = 400220;
    const static int APP_VIDEO_WALL_ZONE_LAYOUT_SELECTED = 400211;
    const static int APP_VIDEO_WALL_ZONE_PAGE_SELECTED = 400212;
    const static int APP_VIDEO_WALL_ZONE_DATA_MAP = 400300;
    const static int APP_VIDEO_WALL_ZONE_SETTING = 400400;
    const static int APP_VIDEO_WALL_LAYOUT_DEFAULT_SET = 400500;
    const static int APP_VIDEO_WALL_PAGE_DEFAULT_SET = 400501;
    const static int APP_VIDEO_WALL_UPDATE_PAGE_DEFAULT = 400502;
    const static int APP_VIDEO_WALL_GET_CAMERA_WITH_LAYOUT_PAGE = 400503;


    const static int APP_VIDEO_WALL_GET_CAMERAS_OF_SITE_WITH_LAYOUT_PAGE = 400504;
    const static int APP_VIDEO_WALL_LOAD_CAMERAS_OF_SITE_WITH_LAYOUT_SUCCESS = 400505;

    const static int APP_VIDEO_WALL_GET_CAMERAS_OF_SITE_WITH_SITE_ID = 500506;
    const static int APP_VIDEO_WALL_LOAD_CAMERAS_OF_SITE_WITH_SITE_ID_SUCCESS = 400507;



    const static int APP_VIDEO_WALL_SHOW_SITE_TOP = 400;

    const static int APP_PLAY_BACK_SHOW = 500000;
    const static int APP_PLAY_BACK_RIGHT_BAR_SHOW = 500001;
    const static int APP_PLAY_BACK_ZONE_LAYOUT_PAGE = 500200;
    const static int APP_PLAY_BACK_ZONE_LAYOUT_SELECTOR = 500210;
    const static int APP_PLAY_BACK_ZONE_PAGE_SELECTOR = 500220;
    const static int APP_PLAY_BACK_ZONE_LAYOUT_SELECTED = 500211;
    const static int APP_PLAY_BACK_ZONE_PAGE_SELECTED = 500212;
    const static int APP_PLAY_BACK_ZONE_CALENDAR_SELECTED = 500213;
    const static int APP_PLAY_BACK_CALENDAR_NEW_DAY_SELECTED = 500214;
    const static int APP_PLAY_BACK_CHANGE_SPEED_ALL_VIDEO = 500215;
    const static int APP_PLAY_BACK_CHANGE_SPEED_VIDEO = 500216;
    const static int APP_PLAY_BACK_ZONE_DATA_MAP = 500300;
    const static int APP_PLAY_BACK_ZONE_DATA_MAP_CALENDAR = 500310;
    const static int APP_PLAY_BACK_ZONE_DATA_MAP_CHANGE_SELECT_DATE = 500311;
    const static int APP_PLAY_BACK_ZONE_DATA_MAP_PLAYLIST_OF_DAY = 500320;
    const static int APP_PLAY_BACK_ZONE_SETTING = 500400;
    const static int APP_PLAY_BACK_SHOW_SITE_TOP = 500401;
    const static int APP_PLAY_BACK_LAYOUT_DEFAULT_SET = 500402;
    const static int APP_PLAY_BACK_PAGE_DEFAULT_SET = 500412;
    const static int APP_PLAY_BACK_UPDATE_PAGE_DEFAULT = 500413;
    const static int APP_PLAY_BACK_UPDATE_DATAMAP_CALENDAR = 500414;
    const static int APP_PLAY_BACK_UPDATE_DATAMAP_HOURS = 500415;
    const static int APP_PLAY_BACK_UPDATE_DATAMAP_SEEK_BAR = 500416;

    const static int APP_PLAY_BACK_CALENDAR_UPDATE_DATE_MAX = 500417;
    const static int APP_PLAY_BACK_CALENDAR_MAX_HISTORYDAY = 500218;





    const static int APP_PLAY_BACK_SEEK_BAR_TO_NEW_POSITION = 500403;

    const static int APP_PLAY_BACK_GET_CAMERA_WITH_LAYOUT_PAGE = 500407;

    const static int APP_PLAY_BACK_GET_CAMERAS_OF_SITE_WITH_LAYOUT_PAGE = 400511;
    const static int APP_PLAY_BACK_LOAD_CAMERAS_OF_SITE_WITH_LAYOUT_SUCCESS = 400512;

    const static int APP_PLAY_BACK_GET_CAMERAS_OF_SITE_WITH_SITE_ID = 400513;
    const static int APP_PLAY_BACK_GET_CAMERAS_OF_SITE_WITH_SITE_ID_SUCCESS = 400514;

    const static int APP_PLAY_BACK_GET_CLIP_RECORD = 400515;

    const static int APP_PLAY_BACK_GET_CLIP_RECORD_SUCCESS = 400516;

    const static int APP_PLAY_BACK_GET_ALL_CLIP_RECORD = 400517;

    const static int APP_PLAY_BACK_WAIT_BUILD_CLIP_RECORD = 400518;
    const static int APP_PLAY_BACK_GET_ALL_CLIP_RECORD_SUCCESS = 400519;
    const static int APP_PLAY_BACK_CANCEL_RECORD = 400520;

    const static int APP_PLAY_BACK_GET_CLIP_RECORD_ERROR = 400521;
    const static int APP_PLAY_BACK_GET_ALL_CLIP_RECORD_ERROR = 400522;


    const static int APP_PLAY_BACK_SET_PATH_SAVE_MEDIA = 500404;
    const static int WHEEL_EVENT_ZOOM_VIDEO = 500405;
    const static int EVENT_MOVE_ZOOM_VIDEO = 500406;

    const static int APP_MAGIC_SHOW_SHOW = 600002;

    // seekbar
    const static int APP_PLAY_BACK_UPDATE_TIME_STAMP_CURRENT = 600003;
    const static int APP_PLAY_BACK_PLAY_LIST_VIDEO_VOD = 600004;
    const static int APP_PLAY_BACK_PLAY_ONE_VIDEO_VOD = 600005;

    // record
    const static int APP_PLAY_BACK_DOWNLOAD_VIDEO_SUCCESS = 600006;
    const static int APP_PLAY_BACK_LINK_RECORD_NOT_FOUND = 600007;
    const static int APP_PLAY_BACK_START_ANIMAITON_RECORD_NORMAL = 600008;
    const static int APP_PLAY_BACK_STOP_ANIMAITON_RECORD_NORMAL = 600009;
    const static int APP_PLAY_BACK_UPDATE_PROCESS_BAR = 600010;
    const static int APP_PLAY_BACK_CLOSE_MENU_RECORD_QUICK = 600011;
    const static int APP_PLAY_BACK_START_RECORD_QUICK = 600012;
    const static int APP_PLAY_BACK_CANCEL_SAVE_VIDEO = 600013;

    const static int APP_LOGIN_SHOW = 700001;

    const static int APP_CONTEXT_GET = 80000;
    const static int APP_CAM_SITE_GET = 80001;
    const static int APP_SOCKET_TO_CONNECT = 80002;
    const static int APP_SOCKET_CONNECTED = 80003;
    const static int APP_DATA_LOADED_SUCCESS = 80004;
    const static int APP_NETWORK_IS_REACHABLE = 80007;
    const static int APP_NETWORK_IS_UNREACHABLE = 80008;
    const static int APP_NETWORK_START_CHECKING = 80009;
    const static int APP_NETWORK_STOP_CHECKING = 80010;
    const static int APP_UPDATE_USER_SITES = 80011;


    const static int SITE_SELECTOR = 900100;
    const static int SITE_SELECTOR_SHOW = 900101;
    const static int SITE_SELECTOR_HIDE = 900102;

    const static int DISPLAY_LIST_APPS = 900103;
    const static int APP_SELECTOR = 900200;
    const static int APP_SELECTOR_SHOW = 900201;
    const static int APP_SELECTOR_HIDE = 900202;
    const static int TOP_BAR_OVER_LAY_CAN_HIDE = 900203;
    const static int TOP_BAR_OVER_LAY_CAN_NOT_HIDE = 900204;

    const static int PROFILE_SETTING = 900300;
    const static int PROFILE_SETTING_SHOW = 900301;
    const static int PROFILE_SETTING_HIDE = 900302;

    const static int PROFILE_SETTING_HIDE_ALL = 900304;
    const static int APP_SELECTOR_HIDE_ALL = 900305;
    const static int SITE_SELECTOR_HIDE_ALL = 900306;

    const static int APP_PLAY_BACK_UPDATE_MAX_DATE_CALENDAR = 900307;
    const static int APP_PLAY_BACK_UPDATE_TIME_PLAYLISTOFDAY = 900308;
    const static int APP_PLAY_BACK_SEEK_TO_NEW_POSITION = 900309;
    const static int APP_PLAY_BACK_SELECTED_TIME_SLOT = 900400;
    const static int APP_PLAY_BACK_UPDATE_TIMESTAMP_FOR_PLAYER = 900401;


    const static QString toString(int message) {
        switch (message) {
        case APP_PLAY_BACK_CLOSE_MENU_RECORD_QUICK:
            return "APP_PLAY_BACK_CLOSE_MENU_RECORD_QUICK";
        case APP_PLAY_BACK_LINK_RECORD_NOT_FOUND:
            return "APP_PLAY_BACK_LINK_RECORD_NOT_FOUND";
        case APP_PLAY_BACK_DOWNLOAD_VIDEO_SUCCESS:
            return "APP_PLAY_BACK_DOWNLOAD_VIDEO_SUCCESS";
        case PLAYER_PLAY:
            return "PLAYER_PLAY";
        case PLAYER_STOP:
            return "PLAYER_STOP";
        case PLAYER_SOURCE_CLEAR:
            return "PLAYER_SOURCE_CLEAR";
        case PLAYER_PAUSE:
            return "PLAYER_PAUSE";
        case PLAYER_NEW_LIVE_SOURCE_SET:
            return "PLAYER_NEW_SOURCE_SET";

        case HIDE:
            return "HIDE";
        case SHOW:
            return "SHOW";
        case PROJECT_RUN:
            return "PROJECT_RUN";
        case PROJECT_EXIT:
            return "PROJECT_EXIT";
        case SITE_CHANGED:
            return "SITE_CHANGED";
        case APP_SHOW:
            return "APP_SHOW";
        case APP_HIDE:
            return "APP_HIDE";

        case ENTER_FULLSCREEN_MODE:
            return "ENTER_FULLSCREEN_MODE";
        case EXIT_FULLSCREEN_MODE:
            return "EXIT_FULLSCREEN_MODE";

        case TOP_CONTROL_BAR_HIDE:
            return "TOP_CONTROL_BAR_HIDE";
        case TOP_CONTROL_BAR_GET_SITE_LIST:
            return "TOP_CONTROL_BAR_GET_SITE_LIST";
        case APP_VIDEO_WALL_SHOW:
            return "APP_VIDEO_WALL_SHOW";
        case APP_VIDEO_WALL_RIGHT_BAR_SHOW:
            return "APP_VIDEO_WALL_RIGHT_BAR_SHOW";
        case APP_VIDEO_WALL_ZONE_LAYOUT_PAGE:
            return "APP_VIDEO_WALL_ZONE_LAYOUT_PAGE";
        case APP_VIDEO_WALL_ZONE_LAYOUT_SELECTOR:
            return "APP_VIDEO_WALL_ZONE_LAYOUT_SELECTOR";
        case APP_VIDEO_WALL_ZONE_PAGE_SELECTOR:
            return "APP_VIDEO_WALL_ZONE_PAGE_SELECTOR";
        case APP_VIDEO_WALL_ZONE_LAYOUT_SELECTED:
            return "APP_VIDEO_WALL_ZONE_LAYOUT_SELECTED";
        case APP_VIDEO_WALL_ZONE_PAGE_SELECTED:
            return "APP_VIDEO_WALL_ZONE_PAGE_SELECTED";
        case APP_VIDEO_WALL_ZONE_DATA_MAP:
            return "APP_VIDEO_WALL_ZONE_DATA_MAP";
        case APP_VIDEO_WALL_ZONE_SETTING:
            return "APP_VIDEO_WALL_ZONE_SETTING";
        case APP_VIDEO_WALL_LAYOUT_DEFAULT_SET:
            return "APP_VIDEO_WALL_LAYOUT_DEFAULT_SET";

        case APP_PLAY_BACK_SHOW:
            return "APP_PLAY_BACK_SHOW";
        case APP_PLAY_BACK_ZONE_LAYOUT_PAGE:
            return "APP_PLAY_BACK_ZONE_LAYOUT_PAGE";
        case APP_PLAY_BACK_ZONE_LAYOUT_SELECTOR:
            return "APP_PLAY_BACK_ZONE_LAYOUT_SELECTOR";
        case APP_PLAY_BACK_ZONE_PAGE_SELECTOR:
            return "APP_PLAY_BACK_ZONE_PAGE_SELECTOR";
        case APP_PLAY_BACK_ZONE_LAYOUT_SELECTED:
            return "APP_PLAY_BACK_ZONE_LAYOUT_SELECTED";
        case APP_PLAY_BACK_ZONE_PAGE_SELECTED:
            return "APP_PLAY_BACK_ZONE_PAGE_SELECTED";
        case APP_PLAY_BACK_ZONE_DATA_MAP:
            return "APP_PLAY_BACK_ZONE_DATA_MAP";
        case APP_PLAY_BACK_ZONE_DATA_MAP_CALENDAR:
            return "APP_PLAY_BACK_ZONE_DATA_MAP_CALENDAR";
        case APP_PLAY_BACK_ZONE_DATA_MAP_CHANGE_SELECT_DATE:
            return "APP_PLAY_BACK_ZONE_DATA_MAP_CHANGE_SELECT_DATE";
        case APP_PLAY_BACK_ZONE_DATA_MAP_PLAYLIST_OF_DAY:
            return "APP_PLAY_BACK_ZONE_DATA_MAP_PLAYLIST_OF_DAY";
        case APP_PLAY_BACK_ZONE_SETTING:
            return "APP_PLAY_BACK_ZONE_SETTING";
        case APP_MAGIC_SHOW_SHOW:
            return "APP_MAGIC_SHOW_SHOW";
        case APP_LOGIN_SHOW:
            return "APP_LOGIN_SHOW";
        case APP_CONTEXT_GET:
            return "APP_CONTEXT_GET";
        case APP_CAM_SITE_GET:
            return "APP_CAM_SITE_GET";

        case SITE_SELECTOR:
            return "SITE_SELECTOR";
        case SITE_SELECTOR_SHOW:
            return "SITE_SELECTOR_SHOW";
        case SITE_SELECTOR_HIDE:
            return "SITE_SELECTOR_HIDE";

        case APP_SELECTOR:
            return "APP_SELECTOR";
        case APP_SELECTOR_SHOW:
            return "APP_SELECTOR_SHOW";
        case APP_SELECTOR_HIDE:
            return "APP_SELECTOR_HIDE";

        case PROFILE_SETTING:
            return "PROFILE_SETTING";
        case PROFILE_SETTING_SHOW:
            return "PROFILE_SETTING_SHOW";
        case PROFILE_SETTING_HIDE:
            return "PROFILE_SETTING_HIDE";

        case PROFILE_SETTING_HIDE_ALL:
            return "PROFILE_SETTING_HIDE_ALL";
        case APP_SELECTOR_HIDE_ALL:
            return "APP_SELECTOR_HIDE_ALL";
        case SITE_SELECTOR_HIDE_ALL:
            return "SITE_SELECTOR_HIDE_ALL";

        case APP_CHANGED:
            return "APP_CHANGED";
        case APP_NEW_SELECTED:
            return "APP_NEW_SELECTED";
        case APP_PLAY_BACK_UPDATE_MAX_DATE_CALENDAR:
            return "APP_PLAY_BACK_UPDATE_MAX_DATE_CALENDAR";
        case APP_PLAY_BACK_CALENDAR_NEW_DAY_SELECTED:
            return "APP_PLAY_BACK_CALENDAR_NEW_DAY_SELECTED";
        case APP_PLAY_BACK_SEEK_TO_NEW_POSITION:
            return "APP_PLAY_BACK_SEEK_TO_NEW_POSITION";
        case APP_PLAY_BACK_UPDATE_TIME_STAMP_CURRENT:
            return "APP_PLAY_BACK_UPDATE_TIME_STAMP_CURRENT";
        case APP_PLAY_BACK_PlAYER_SHOW_FULL_SCREEN:
            return "APP_PLAY_BACK_PlAYER_SHOW_FULL_SCREEN";
        case APP_PLAY_BACK_PlAYER_HIDE_FULL_SCREEN:
            return "APP_PLAY_BACK_PlAYER_HIDE_FULL_SCREEN";
        case APP_PLAY_BACK_PLAY_LIST_VIDEO_VOD:
            return "APP_PLAY_BACK_PLAY_LIST_VIDEO_VOD";
        case APP_PLAY_BACK_PLAY_ONE_VIDEO_VOD:
            return "APP_PLAY_BACK_PLAY_ONE_VIDEO_VOD";
        case APP_PLAY_BACK_CHANGE_SPEED_ALL_VIDEO:
            return "APP_PLAY_BACK_CHANGE_SPEED_ALL_VIDEO";
        case APP_PLAY_BACK_CHANGE_SPEED_VIDEO:
            return "APP_PLAY_BACK_CHANGE_SPEED_VIDEO";
        case APP_PLAY_BACK_START_ANIMAITON_RECORD_NORMAL: {
            return "APP_PLAY_BACK_START_ANIMAITON_RECORD_NORMAL";
        }
        case APP_PLAY_BACK_STOP_ANIMAITON_RECORD_NORMAL: {
            return "APP_PLAY_BACK_STOP_ANIMAITON_RECORD_NORMAL";
        }
        case APP_PLAY_BACK_RESET_PLAYER_SPEED: {
            return "APP_PLAY_BACK_RESET_PLAYER_SPEED";
        }
        case APP_PLAY_BACK_UPDATE_PROCESS_BAR:
            return "APP_PLAY_BACK_UPDATE_PROCESS_BAR";
        case PLAYER_PLAY_SD_CLICK:
            return "PLAYER_PLAY_SD_CLICK";
        case PLAYER_PLAY_HD_CLICK:
            return "PLAYER_PLAY_HD_CLICK";
        case PLAYER_PLAY_EXIT_CLICK:
            return "PLAYER_PLAY_EXIT_CLICK";
        case PLAYER_PLAY_FULL_CLICK:
            return "PLAYER_PLAY_FULL_CLICK";
        case PLAYER_BEGIN_HIDE_FULLSCREEN:
            return "PLAYER_BEGIN_HIDE_FULLSCREEN";
        case PLAYER_EXIT_POP_OUT_MODE:
            return "PLAYER_EXIT_POP_OUT_MODE";
        case TOP_BAR_OVER_LAY_CAN_HIDE: {
            return "TOP_BAR_OVER_LAY_CAN_HIDE";
        } break;
        case TOP_BAR_OVER_LAY_CAN_NOT_HIDE:
            return "TOP_BAR_OVER_LAY_CAN_NOT_HIDE";
        case APP_PLAY_BACK_UPDATE_TIME_PLAYLISTOFDAY:
            return "APP_PLAY_BACK_UPDATE_TIME_PLAYLISTOFDAY";
        case PAGE_TRANSITION_BEGIN:
            return "PAGE_TRANSITION_BEGIN";
        case PAGE_TRANSITION_START_ON_CLICK:
            return "PAGE_TRANSITION_START_ON_CLICK";
        case PAGE_TRANSITION_START_AUTOMATICALLY:
            return "PAGE_TRANSITION_START_AUTOMATICALLY";
        case APP_PLAY_BACK_SELECTED_TIME_SLOT:
            return "APP_PLAY_BACK_SELECTED_TIME_SLOT";
        case APP_PLAY_BACK_SEEK_BAR_TO_NEW_POSITION:
            return "APP_PLAY_BACK_SEEK_BAR_TO_NEW_POSITION";
        case APP_PLAY_BACK_SET_PATH_SAVE_MEDIA:
            return "APP_PLAY_BACK_SET_PATH_SAVE_MEDIA";
        case PAGE_TRANSITION_DELAY:
            return "PAGE_TRANSITION_DELAY";
        case DISPLAY_LIST_APPS:
            return "DISPLAY_LIST_APPS";
        case WHEEL_EVENT_ZOOM_VIDEO:
            return "WHEEL_EVENT_ZOOM_VIDEO";
        case EVENT_MOVE_ZOOM_VIDEO:
            return "EVENT_MOVE_ZOOM_VIDEO";
        case GET_SIZE_TOP_CONTROL_BAR:
            return "GET_SIZE_TOP_CONTROL_BAR";
        case APP_PLAY_BACK_LAYOUT_DEFAULT_SET:
            return "APP_PLAY_BACK_LAYOUT_DEFAULT_SET";
        default:
            return "System hide message or unknown message " +
                    QString::number(message);
        }
    }

    const static QString getAppBackgroundColor() { return "#222"; }
};

#endif  // MESSAGE_H
