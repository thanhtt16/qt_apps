#ifndef GENERALDEFINE_H
#define GENERALDEFINE_H

#include <QDateTime>
#include <QDebug>
#include <QList>
#include <QObject>
#include <QString>
struct TypeTime {
  QString time;
  QString day;
};
struct TimeRange {
  long starttime = 0;
  long length = 0;
};
struct InfomationRecordVOD {
  QString url;
  QString filename;
};
struct DataTreeWidget {
  QList<QString> *mListDataHeader;
  QList<QString> *mListDataChild;
};
struct InfoVideo {
  int cameraid;
  QString name;
};
struct TimeStampRecord {
  long startTimeStampRecord;
  long endTimeStampRecord;
};

struct DataZoomVideo {
  QPoint pixelDelta;
  QPoint pos;
  qreal deg;
};
struct DataMoveVideo {
  QPoint pos;
  QPoint pressPos;
};

struct SizeTopControlBar {
  int widthBrand;
  int widthSite;
  int widthAppName;
  int widthTopBar;
  int widthProfileSetting;
};

struct DataGetClip{
    int cameraId;
    QString nameVideo;
    QString urlVideo;
};

Q_DECLARE_METATYPE(DataGetClip);
Q_DECLARE_METATYPE(SizeTopControlBar);
Q_DECLARE_METATYPE(DataZoomVideo)
Q_DECLARE_METATYPE(DataMoveVideo)

Q_DECLARE_METATYPE(TimeStampRecord)
Q_DECLARE_METATYPE(TimeRange)

struct PlayerStyleSheet {
  QString playingVodColor = "#1e63ee";
  QString playingLiveColor = "#ff3b30";
  QString stoppedColor = "#8e8e93";
  QString white = "#ffffff";
  QString black = "#000000";
  QString transparent = "#00000000";

  QString playingLiveCss = css(playingLiveColor, white);
  QString playingVodCss = css(playingVodColor, white);
  QString stoppedCss = css(stoppedColor, white);

  QString transCss = css(transparent, white);
  QString blackCss = css(black, white);

  QString coverCss = css("#50fea700", white);

  QString css(QString backgroundColor, QString color) {
    return QString("background-color: %1; color: %2;")
        .arg(backgroundColor)
        .arg(color);
  }

  QString border(QString backgroundColor, QString color, int borderWidth,
                 int borderRadius) {
    return QString("background-color: %1; color: %2; border-width: %3px;  "
                   "border-style: solid; border-radius: %4px;")
        .arg(backgroundColor)
        .arg(color)
        .arg(borderWidth)
        .arg(borderRadius);
  }

  QString leftNormalButton =
      "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
      "border-style: solid; border-top-left-radius: 3px; "
      "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
      "border-bottom-left-radius: 3px;";
  QString middleNormalButton =
      "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
      "border-style: solid; border-top-left-radius: 0px; "
      "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
      "border-bottom-left-radius: 0px;";
  QString rightNormalButton =
      "background-color: #F7F7F7; color: #6d6d6c; border-width: 0px;  "
      "border-style: solid; border-top-left-radius: 0px; "
      "border-top-right-radius: 3px; border-bottom-right-radius: 3px; "
      "border-bottom-left-radius: 0px;";

  QString leftSelectedButton =
      "background-color: #6d6d6c; color: #f7f7f7; border-width: 0px;  "
      "border-style: solid; border-top-left-radius: 3px; "
      "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
      "border-bottom-left-radius: 3px;";
  QString middleSelectedButton =
      "background-color: #6d6d6c; color: #f7f7f7; border-width: 0px;  "
      "border-style: solid; border-top-left-radius: 0px; "
      "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
      "border-bottom-left-radius: 0px;";
  QString rightSelectedButton =
      "background-color: #6d6d6c; color: #f7f7f7; border-width: 0px;  "
      "border-style: solid; border-top-left-radius: 0px; "
      "border-top-right-radius: 3px; border-bottom-right-radius: 3px; "
      "border-bottom-left-radius: 0px;";
};

struct PlayerMessage {
  QString noCamera = "Không có camera!";
  QString noSignal = "Không có tín hiệu!";
  QString noData = "Không có dữ liệu!";

  QString live = "LIVE";
  QString vod = "PLAYBACK";
  QString rec = "REC";
  QString calendar = "";
  QString screenshot = "";
  QString hideFullscreen = "✕";
  QString showFullscreen = "";
};



class GeneralDefine {
 public:
  static GeneralDefine &instance() {
    static GeneralDefine mInstance;
    return mInstance;
  }

  TypeTime convertSecondsToTexts(long seconds);
  QString checkDay(QString date);
  QString convertDate(long starttime);
  long convertDateToTimeStamp(QString date, QString hour, QString minute,
                              QString second);

 protected:
  GeneralDefine();
  ~GeneralDefine();
};

#endif  // GENERALDEFINE_H
