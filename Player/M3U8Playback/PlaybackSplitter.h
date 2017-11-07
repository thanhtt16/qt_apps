#ifndef PLAYBACKSPLITTER_H
#define PLAYBACKSPLITTER_H

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
#include "PlaybackMapQueue.h"
#include "PlaybackImageQueue.h"
#include "PlaybackPlayer.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavcodec/mediacodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
}

class PlaybackPlayer;

class PlaybackSplitter : public QThread {
    Q_OBJECT

private:
    bool playerIsPlaying = false;
    QMutex decoderMutex;

    PlaybackPlayer *pRTCPlayer;

    struct SwsContext *swsContext;
    QByteArray nalData;
    QByteArray NALUStartCode;
    QByteArray IDR_NAL_TYPE;

    int BEGIN_H264_SIGN = 0001;

    bool isDropping = true;
    int dataIndex = 0;
    int rtpPackageIndex = 0;
    long sequenceOfTimeooutPackage = 0;
    int MIN_DOWNLOAD_TIME_WAIT = 1000; // milisecond
    long downloadTimeWait = 10000;
    int freeTime = 0;
    QByteArray keyFrameData;

    AVCodec *avCodec;
    AVCodecContext *avCodecContext;
    AVCodecParserContext *avParser;

public:
    QImage decodeRTPPackage(QByteArray rtpPackage);

    void decoderClose();
    void decoderOpen();

    explicit PlaybackSplitter(PlaybackPlayer *rtcPlayer);
    ~PlaybackSplitter();

    void run();

    void startWorking();
    void stopWorking();


    void setDropping(bool isDropping);
    bool getIsDropping() const;

    QImage decode2Image(QByteArray h264Data);
    QByteArray dequeueWithTimeout(long timeout);
    void calibrateWithRender();

    void onReceiveRTPPackage(QByteArray rtpPackage);
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

    PlaybackPlayer *getPRTCPlayer() const;

Q_SIGNALS:
    void splittedRTPPackage(int packageIndex, QByteArray rtpPakage);
    void socketLostConnection();
    void needToReconnect();
    void showLoadingSign();
    void shouldReopenSouce();
};

#endif  // PLAYBACKSPLITTER_H
