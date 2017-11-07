#ifndef RTCSPLITTER_H
#define RTCSPLITTER_H

#include <QBuffer>
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QMutex>
#include <QObject>
#include <QPixmap>
#include <QQueue>
#include <QStack>
#include <QTimer>
#include <QThread>
#include "cam9mapqueue.h"
#include "cam9imagequeue.h"
#include "RTCPlayer.h"
#include <algorithm>
#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavcodec/mediacodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
}

class RTCPlayer;

class RTCSplitter : public QThread {
    Q_OBJECT

private:
    bool playerIsPlaying = false;
    QMutex splitterMutex;

    RTCPlayer *pRTCPlayer;

    struct SwsContext *swsContext;
    QByteArray nalData;
    QByteArray NALUStartCode;
    QByteArray IDR_NAL_TYPE;
    QByteArray P_NAL_TYPE;

    bool isDropping = true;
    int dataIndex = 0;
    int naluIndex = 0;
    int rtpPackageIndex = 0;
    long sequenceOfTimeooutPackage = 0;
    int MAX_DOWNLOAD_TIME_WAIT = 12000; // milisecond
    long downloadTimeWait = 600;
    int freeTime = 0;
    QByteArray keyFrameData;
    bool isFirstSPS = false;
    bool isFirstPPS = false;

    AVCodec *avCodec;
    AVCodecContext *avCodecContext;
    AVCodecParserContext *avParser;

    long timestampImageLast;
    bool working = false;

public:
    QImage decodeRTPPackage(QByteArray rtpPackage);
    QImage decodeRTPPackage2(QByteArray rtpPackage);

    void decoderClose();
    void decoderOpen();

    explicit RTCSplitter(RTCPlayer *rtcPlayer);
    ~RTCSplitter();

    void run();
    void liveWorker();
    void startWorking();
    void stopWorking();
    bool isWorking();

    void setDropping(bool isDropping);
    bool getIsDropping() const;
    void aggregation(QByteArray &rawData);
    QImage decode2Image(QByteArray h264Data);
    QByteArray dequeueWithTimeout(long timeout);
    QByteArray dequeueWithTimeout4Playback(long timeout);

    void calibrateWithRender();

    void onReceiveRTPPackage(double timestamp, QByteArray rtpPackage);

    QString byteArrayToString(QByteArray array, int length) {
        QString str = "";
        if (length > array.size()) {
          length = array.size();
        }
        for (int index = 0; index < length; ++index) {
          quint8 item = static_cast<quint8>(array.at(index));
          str += QString("%1").arg(item, 4);
        }
        return str;
    }

    QString byteArrayToStringReverse(QByteArray array, int length) {
        QString str = "";
        if (length > array.size()) {
          length = array.size();
        }
        for (int index = length - 1; index >= 0; --index) {
          quint8 item = static_cast<quint8>(array.at(array.size() - index - 1));
          str += QString("%1").arg(item, 4);
        }
        return str;
    }

    RTCPlayer *getPRTCPlayer() const;

Q_SIGNALS:
    void splittedRTPPackage(int packageIndex, QByteArray rtpPakage);
    void socketLostConnection();
    void needToReconnect();
    void showLoadingSign();
    void shouldReopenSouce();
};

#endif  // RTCSPLITTER_H
