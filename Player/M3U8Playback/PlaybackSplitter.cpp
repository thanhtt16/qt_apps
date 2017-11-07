#include "PlaybackSplitter.h"

PlaybackPlayer *PlaybackSplitter::getPRTCPlayer() const
{
    return pRTCPlayer;
}

PlaybackSplitter::PlaybackSplitter(PlaybackPlayer *rtcPlayer) {
    NALUStartCode.append((char)0);
    NALUStartCode.append((char)0);
    NALUStartCode.append((char)0);
    NALUStartCode.append((char)1);
    IDR_NAL_TYPE.append(101);

    this->pRTCPlayer = rtcPlayer;

    decoderOpen();
    this->setObjectName("Decoder");
}

PlaybackSplitter::~PlaybackSplitter() {
    decoderClose();
}

void PlaybackSplitter::startWorking() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName << "::RTCSplitter::startWorking" ;

    if (!isRunning()) {
        QThread::start();
    }
}

void PlaybackSplitter::stopWorking() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName << "::RTCSplitter::splitterStop" ;
}

void PlaybackSplitter::run() {
    bool ret;
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();

    PlaybackMapQueue& rtpQueue = this->pRTCPlayer->getRtpQueue();
    rtpQueue.setName("RTP Queue of " + this->pRTCPlayer->getPlayerName());


    while (1) {
        QByteArray rawData = dequeueWithTimeout(downloadTimeWait);
        if (!rawData.isEmpty()) {
            // find first h264 package
            long passedBytes = 0; // bypass first package /0x/0x/0x/1x
            while (passedBytes < rawData.size()) {
                int sign = rawData.mid(passedBytes, 3).toHex().toInt(&ret,16);
                if (sign == BEGIN_H264_SIGN) {
                    break;
                } else passedBytes++;
            }
            while (passedBytes < rawData.size()) {
                int beginByteIndex = passedBytes + 3;
                while (passedBytes < rawData.size()) {
                    int sign = rawData.mid(passedBytes, 3).toHex().toInt(&ret,16);
                    if (sign == BEGIN_H264_SIGN) {
                        break;
                    }
                    passedBytes++;
                }
                int endByteIndex = passedBytes;

                int lengthOfH264RawData = endByteIndex - beginByteIndex;

                QByteArray h264Raw = rawData.mid(beginByteIndex, lengthOfH264RawData);

                // h264 processing
                int nalType = h264Raw.mid(0, 1).toHex().toInt(&ret,16);
                switch (nalType) {
                case 103: // SPS Type
                    onReceiveRTPPackage(NALUStartCode + h264Raw);
                    break;
                case 104: // PPS Type
                    onReceiveRTPPackage(NALUStartCode + h264Raw);
                    break;
                case 101: // IDR Type
                    onReceiveRTPPackage(NALUStartCode + h264Raw);
                    break;
                case 97: { // P-Frame  Pictur Type
                    onReceiveRTPPackage(NALUStartCode + h264Raw);
                }
                    break;
                case 0: { // End Of 10 minute type
                    //qDebug() << playerName << " end of 10 minutes ";
                }
                    break;
                default:
                    qDebug() << playerName << " something went wrong... in RTP passedBytes=" << passedBytes << "/" << rawData.size() << " nalType=" << nalType;
                    //rtpIsOk = false;
                    break;
                }
            }
            this->sequenceOfTimeooutPackage = 0;
        }
        else { // no data after timeout
            this->sequenceOfTimeooutPackage++;
            if (this->sequenceOfTimeooutPackage >= 2) { // if no data received after 20 second then do reconnect
                this->sequenceOfTimeooutPackage = 0;
                //Q_EMIT shouldReopenSouce();
            }
        }
    }
    qDebug() << playerName << "::RTCSplitter::STOPPED ";
}

QByteArray PlaybackSplitter::dequeueWithTimeout(long timeout) {
    PlaybackMapQueue& rtpQueue = this->pRTCPlayer->getRtpQueue();
    QByteArray rtpData;
    while (timeout > 0) {
        rtpData = rtpQueue.dequeue();
        if (!rtpData.isEmpty()) {
            freeTime = 0;
            break;
        }

        if (rtpData.isEmpty()) { // waitting
            QThread::msleep(100);
            timeout = timeout - 100;

//            if (rtpQueue.getSize() == 0) { // if queue is empty, maybe the network is bad
//                freeTime++;
//                if (freeTime == 100) {
//                    freeTime = 0;
//                    //if (hasNetworkConnection) {
//                       Q_EMIT needToReconnect();
//                    //}
//                }
//                //Q_EMIT showLoadingSign();
//            } else {
//                freeTime = 0; // queue is not empty, network is still fine
//            }

        }
    }
    if (rtpData.isEmpty()) rtpQueue.next();
    return rtpData;
}

QImage PlaybackSplitter::decode2Image(QByteArray h264Data) {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();

    QImage image;
    bool ret;
    int nalType = h264Data.mid(4, 1).toHex().toShort(&ret,16);
    if (h264Data.size() > 0) {
        // h264 processing
        bool ret;
        short nalType = h264Data.mid(4, 1).toHex().toShort(&ret,16);
        switch (nalType) {
        case 103: { // SPS Type
            decodeRTPPackage(h264Data);
        } break;
        case 104: { // PPS Type
            decodeRTPPackage(h264Data);
        }
            break;
        case 101: { // IDR Type
                image = decodeRTPPackage(h264Data);
        }
            break;
        case 97: { // P-Frame  Pictur Type
                image = decodeRTPPackage(h264Data);
        }
            break;
        default:
            qDebug() << playerName << " something went wrong... in H264";
            break;
        }
    }
    return image;
}

void PlaybackSplitter::onReceiveRTPPackage(QByteArray h264Package) {
//        QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
//        qDebug() << playerName << " Thread: " << this->thread()
//                 << "RTCSplitter onReceiveRTPPackage - packageIndex: " << rtpPackageIndex;
    PlaybackImageQueue & imageQueue = this->pRTCPlayer->getImageQueue();
    imageQueue.enqueue(decode2Image(h264Package));
}

QImage PlaybackSplitter::decodeRTPPackage(QByteArray rtpPackage) {
    QImage image;
    int rtpSize = rtpPackage.size();
    if (rtpSize > 0) {
        quint8 *data = new quint8[rtpSize];
        memcpy(data, rtpPackage.constData(), rtpSize);
        AVPacket avPacket;
        av_init_packet(&avPacket);
        avPacket.size = rtpSize;
        avPacket.data = data;

        int decodedFrameSize, decodedFrameCount;
        AVFrame *avFrame = av_frame_alloc();
        decodedFrameSize = avcodec_decode_video2(avCodecContext, avFrame, &decodedFrameCount, &avPacket);
        if (decodedFrameSize > 0 && decodedFrameCount > 0) {
            struct SwsContext *swsContext = sws_getContext(avCodecContext->width, avCodecContext->height,
                                                           avCodecContext->pix_fmt, avCodecContext->width,
                                                           avCodecContext->height, AV_PIX_FMT_RGB24, SWS_BICUBIC,
                                                           NULL, NULL, NULL);
            uint8_t *outBuffer = (uint8_t *)av_malloc(avpicture_get_size(
                                                          AV_PIX_FMT_RGB24, avCodecContext->width, avCodecContext->height));
            AVPicture avPicture;

            avpicture_fill(&avPicture, outBuffer, AV_PIX_FMT_RGB24,
                           avCodecContext->width, avCodecContext->height);

            sws_scale(swsContext, (const uint8_t *const *)avFrame->data,
                      avFrame->linesize, 0, avCodecContext->height,
                      avPicture.data, avPicture.linesize);

            image = QImage(avPicture.data[0],
                    avFrame->width,
                    avFrame->height,
                    avPicture.linesize[0],
                    QImage::Format_RGB888).copy();

            if (image.isNull() || image.height() <= 0) {
                //qDebug() << "invalid package of " << imageQueue.getName() << " = " << invalidPackage++ << "H264 package size = " << rtpSize ;
            }
            av_free(outBuffer);
            sws_freeContext(swsContext);
        }

        av_frame_unref(avFrame);
        av_frame_free(&avFrame);
        av_free_packet(&avPacket);
        delete[] data;
    }
    return image;
}

void PlaybackSplitter::decoderOpen() {
    av_log_set_level(AV_LOG_QUIET);
    av_register_all();
    avcodec_register_all();
    avCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!avCodec) {
        // TODO
    }
    avCodecContext = avcodec_alloc_context3(avCodec);
    if (!avCodecContext) {
        // TODO
    }
    if (avCodec->capabilities & CODEC_CAP_TRUNCATED) {
        avCodecContext->flags || CODEC_FLAG_TRUNCATED;
    }
    if (avcodec_open2(avCodecContext, avCodec, NULL) < 0) {
        // TODO
    }
    avCodecContext->thread_count = 4;
    avCodecContext->thread_type = 2;
}

void PlaybackSplitter::decoderClose() {
    if (avCodecContext != NULL) {
        avcodec_close(avCodecContext);
        avcodec_free_context(&avCodecContext);
        av_free(avCodecContext);
        delete avCodecContext;
    }
}

//void PlaybackSplitter::beginH264
