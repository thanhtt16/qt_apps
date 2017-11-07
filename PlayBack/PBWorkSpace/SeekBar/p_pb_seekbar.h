#ifndef P_PBSeekBar_H
#define P_PBSeekBar_H

#include <PacModel/presentation.h>
#include <QCalendarWidget>
#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QVBoxLayout>
#include <QtMath>
#include "Common/generaldefine.h"
#include "Common/resources.h"
#include "c_pb_seekbar.h"
static struct IconButtonTop {
    QString mainmenu = "";
    QString playback = "";
    QString layout = "";
    QString fullscreen = "";
    QString profile = "";
    QString more = "";
    QString present = "";
    QString position = "";
    QString setting = "";
    QString closeControlPanel = "";
    QString forward = "";
    QString backward = "";
    QString pause = "";
    QString play = "";
    QString site = "";
} iconButtonTops;
class C_PBSeekBar;
class P_PBSeekBar : public Presentation {
    // init ui control
private:
public:
    int layoutCurrent = 0;
    double mSpeedAllVideo = 1.0;
    //  PlayerState playerState = Ps_Playing;
    QString mVersion;
    QString modeVideoVOD;
    int mIsSliderPressed = 0;
    // slider
    int mIsShowOneVideo = 0;
    int currentValueSlider;
    int lastValueSlider;
    int lengthVideoVOD = 3600;
    long mStartTimePlayVideoAllVOD = 0;
    int mlenghtPlayVideoAllVOD = 3600;
    long mLastTimeStampSelect = 0;

    QTimer *mUpdateSliderTimer;
    QSlider *mVideoSlider;
    QWidget *mBottomLeftWidget;
    QWidget *mBottomRightWidget;
    QHBoxLayout *rightHBoxLayout;
    QVBoxLayout *mBottomLeftLayout;
    QHBoxLayout *mBottomRightLayout;
    // backward
    QPushButton *mBackWardButton;
    QPushButton *mForWardButton;
    QPushButton *mPauseButton;
    QPushButton *mForwardByFrameButton;
    QPushButton *mForWard025xButton;
    QPushButton *mForWard05xButton;
    QPushButton *mForWard1xButton;
    QPushButton *mForWard2xButton;
    QPushButton *mForWard4xButton;
    QPushButton *mForWard8xButton;

    P_PBSeekBar(Control *ctrl, QWidget *zone);
    C_PBSeekBar *control() { return (C_PBSeekBar *)this->ctrl; }
    void changeControl(Control *ctrl);
    void update();
    void initLeft();
    void initRight();
    QObject *getZone(int zoneId);
    void initBotBackWard();
    void onReleaseSlider();
    void onPressSlider();

    void pauseVideoVOD();
    void setTimeRangeRequestVOD(TimeRange timeRange);
    void playListVideoVOD(TimeRange timeRange);
    void playOneVideoVOD(TimeRange timeRange);
    //  void setPlayerState(PlayerState state);
    void seekToNewPostion(long newPostion);
    void changedSpeedAllVideo(QVariant *dataStruct);
    void resetValueWhenAppShow();
    void stopWhenChangeApp();
public Q_SLOTS:
    void sliderChangeValue(int value);
    void updateValueSlider();

    void forwardByFrame();
    void forWardSpeed025x();
    void forWardSpeed05x();
    void forWardSpeed1x();
    void forWardSpeed2x();
    void forWardSpeed4x();
    void forWardSpeed8x();

    void selectedBackWard();
    void selectedForWard();
    void setBackgroundPlayBack(bool isByFrame, bool is025x, bool is05x, bool is1x,
                               bool is2x, bool is4x, bool is8x, bool is16x);
    void resetPlayerSpeed();
    void resetValueSeekBar();
    void disableSpeedButton(LayoutStruct layout);
    void enableSpeedButton();
    void disableSpeedButton();
};

#endif  // PRESENTATION_H
