#include "RTCSplitter.h"

RTCPlayer *RTCSplitter::getPRTCPlayer() const
{
    return pRTCPlayer;
}

RTCSplitter::RTCSplitter(RTCPlayer *rtcPlayer) {
    NALUStartCode.append((char)0);
    NALUStartCode.append((char)0);
    NALUStartCode.append((char)0);
    NALUStartCode.append((char)1);
    IDR_NAL_TYPE.append(101);
    /* register all the codecs*/

    av_log_set_level(AV_LOG_QUIET);
    av_register_all();
    avcodec_register_all();
    decoderOpen();
    this->pRTCPlayer = rtcPlayer;
    this->setObjectName("Decoder");
}

RTCSplitter::~RTCSplitter() {
    decoderClose();
}

void RTCSplitter::startWorking() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName << "::RTCSplitter::startWorking" ;
    if (!isRunning()) {
        QThread::start();
    }
    Cam9MapQueue& rtpQueue = this->pRTCPlayer->getRtpQueue();
    rtpQueue.setWorking(true);
    splitterMutex.lock();
    working = true;
    splitterMutex.unlock();
}

void RTCSplitter::stopWorking() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName << "::RTCSplitter::splitterStop" ;
    Cam9MapQueue& rtpQueue = this->pRTCPlayer->getRtpQueue();
    rtpQueue.setWorking(false);
    rtpQueue.empty();
    splitterMutex.lock();
    working = false;
    splitterMutex.unlock();
}

bool RTCSplitter::isWorking() {
    bool w;
    splitterMutex.lock();
    w = working;
    splitterMutex.unlock();
    return w;
}

void RTCSplitter::run() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    Cam9MapQueue& rtpQueue = this->pRTCPlayer->getRtpQueue();
    rtpQueue.setName("RTP Queue of " + this->pRTCPlayer->getPlayerName());
    int HEADER_LENGTH = 12;
    bool GOPStartedWithIFrame = false;
    bool IDRPictureStarted = false;
    bool IDRPictureMiddle = false;
    bool IDRPictureEnd = false;
    QByteArray IDRPicture;
    qDebug() << playerName <<  "::RTCSplitter::run ";
    while (1) {
        while (isWorking()) {
            QByteArray rawData = dequeueWithTimeout(downloadTimeWait);

            if (!rawData.isEmpty() && (rawData.size() > HEADER_LENGTH)) {
                long passedBytes = 0;
                bool rtpIsOk = true;
                while (isWorking() && passedBytes < rawData.size() && rtpIsOk) {
                    bool ret;
                    int payloadSize = rawData.mid(passedBytes + 0, 2).toHex().toInt(&ret,16);
                    long long timestring =  rawData.mid(passedBytes + 2, 8).toHex().toLongLong(0, 16);
                    double timestamp = reinterpret_cast<double&>(timestring);
                    QString timestampToString = QString::number(timestamp, 'g', 19);
                    long timestamplong = timestampToString.toLong();

                    int index = rawData.mid(passedBytes + 10, 2).toHex().toInt(&ret,16);
                    // Phần data h264 bị ngắn hơn chiều dài.
                    int lengthOfH264RawData = payloadSize - 10;

                    if (passedBytes + HEADER_LENGTH + lengthOfH264RawData > rawData.size()) {
                        rtpIsOk = false;
                        qDebug() << playerName << " something went wrong... in RTP passedBytes + HEADER_LENGTH + lengthOfH264RawData=" << (passedBytes + HEADER_LENGTH + lengthOfH264RawData) << "/" << rawData.size();
                        continue;
                        break;
                    }
                    QByteArray h264Raw = rawData.mid(passedBytes + HEADER_LENGTH, lengthOfH264RawData);
                    passedBytes = passedBytes + payloadSize + 2;

                    // h264 processing
                    int nalu_header_byte = h264Raw.mid(0, 1).toHex().toInt(&ret,16);
                    int nalType = nalu_header_byte & 0b11111;


                    if (nalType!=28 && IDRPicture.size()>0){
                        onReceiveRTPPackage(timestamp, NALUStartCode + IDRPicture);
                    }

                    switch (nalType) {
                    case 7: // SPS Type
                    {
                        GOPStartedWithIFrame = false;
                        decodeRTPPackage(NALUStartCode + h264Raw);
                    }
                        break;
                    case 8: // PPS Type
                    {
                        GOPStartedWithIFrame = false;
                        decodeRTPPackage(NALUStartCode + h264Raw);
                    }
                        break;
                    case 5: // IDR Type
                    {
                        GOPStartedWithIFrame = true;
                        onReceiveRTPPackage(timestamp, NALUStartCode + h264Raw);
                    }
                        break;

                    case 28: { // IDR Picture Type MULTIPLE PART (FU)
                        //B Frame?
                        // byte 2
                        //                        QString start = (FU_A_byte & 0b10000000) == 1 ? "START IDR" : "NOT START IDR" ;
                        //                        QString end = (FU_A_byte & 0b01000000) == 1 ? "END IDR" : "NOT END IDR ";
                        //                        QString reserverd = (FU_A_byte & 0b00100000) == 0 ? "Reserved GOI IDR" : " NOT Reserved KET THUC GOI IDR";
                        //                        qDebug() << "IDR Mutiple" << "CHECK START BIT" << start;
                        //                        qDebug() << "IDR Mutiple" << "CHECK END BIT" << end ;
                        //                        qDebug() << "IDR Mutiple" << "CHECK Reserved BIT" << reserverd ;

                        //                        qDebug() << "FU_A_byte" << FU_A_byte<< "CHECK START" << (FU_A_byte & 0b10000000) << "END" << (FU_A_byte & 0b01000000);



                        int FU_A_byte = h264Raw.mid(1, 1).toHex().toInt(&ret,16);
                        if (FU_A_byte&0x80) { //nếu dịch 7 bit
                            if(IDRPicture.size() > 0){
                                onReceiveRTPPackage(timestamp, NALUStartCode + IDRPicture);
                            }
                            //nếu bằng 1 tức start IDR
                            IDRPictureStarted = true;
                            IDRPictureMiddle = false;
                            //                            qDebug() << "START MUTIPLE IDR " <<FU_A_byte << "(FU_A_byte & 0b11111)" <<(FU_A_byte & 0b11111);


                            QByteArray IDR_NAL_TYPE_TEMP;
                            IDR_NAL_TYPE_TEMP.append((nalu_header_byte &0xe0) | (FU_A_byte & 0x1f));
                            //                            qDebug() << "IDR_NAL_TYPE_TEMP" << IDR_NAL_TYPE_TEMP;
                            IDRPicture  =  IDR_NAL_TYPE_TEMP + h264Raw.mid(2, payloadSize - 10);
                        } else
                        {
                            if (FU_A_byte& 0x40) { //nếu dịch 6 bit
                                // end of Fragment Unit
                                if (IDRPictureStarted == false) {
                                    rtpIsOk = false;
                                    break;
                                }
                                IDRPicture = IDRPicture + h264Raw.mid(2, payloadSize-10);
                                GOPStartedWithIFrame = true;
                                onReceiveRTPPackage(timestamp, NALUStartCode + IDRPicture);
                                IDRPicture.clear();
                                //                                qDebug() << "END MUTIPLE IDR " <<FU_A_byte << "SIZE PACKAGE SEND" << IDRPicture.size();
                                //                          qDebug() << index << "  -  IDR multipart ended";
                            } else {
                                //                                qDebug() << "MIDDEL MUTIPLE IDR" << FU_A_byte;
                                //nếu bằng 0
                                //The Reserved bit MUST be equal to 0 and MUST be ignored by the receiver.
                                if (IDRPictureStarted == false) {
                                    rtpIsOk = false;
                                    break;
                                }
                                IDRPictureMiddle = true;
                                IDRPicture = IDRPicture + h264Raw.mid(2, payloadSize-HEADER_LENGTH);
                            }
                        }
                    }
                        break;

                    case 1: { // P-Frame  Pictur Type
                        onReceiveRTPPackage(timestamp, NALUStartCode + h264Raw);
                        //                        if  (GOPStartedWithIFrame) {
                        //                            qDebug() << playerName<<" nalu_header_byte 3 high bits = " << (nalu_header_byte >> 5);
                        //                            if ((nalu_header_byte >> 5) > 3) {
                        //                                qDebug() <<playerName<< "=================================" << (nalu_header_byte >> 5);
                        //                              }

                        //                          }
                        //                        else  rtpQueue.enqueue(linkIndex, downloadFaildMessage);

                        //                          qDebug() << "Drop P-frame[" << index << "] because of missing I-frame. ";

                    }
                        break;

                    case 0: { // End Of 10 minute type
                        qDebug() << playerName << " end of 10 minutes ";
                        qDebug() << "nalu_header_byte" <<nalu_header_byte;

                    }
                        break;
                    default:
                        qDebug() << playerName << " something went wrong... in RTP passedBytes=" << passedBytes << "/" << rawData.size() << " nalType=" << nalType;
                        //rtpIsOk = false;
                        break;
                    }

                }
                this->sequenceOfTimeooutPackage = 0;
            } else { // no data after timeout
                if (this->sequenceOfTimeooutPackage >= 300000) { // if no data received after 20 second then do reconnect
                    this->sequenceOfTimeooutPackage = 0;
                    //                  Q_EMIT shouldReopenSouce();
                }
            }
        }
        QThread::msleep(1); // trong truong hop chay khong tai de cpu khong bi tang
    }

    qDebug() << playerName << "::RTCSplitter::STOPPED";
}

void RTCSplitter::liveWorker() {

}

QByteArray RTCSplitter::dequeueWithTimeout(long timeout) {
    Cam9MapQueue& rtpQueue = this->pRTCPlayer->getRtpQueue();
    QByteArray rtpData;

    while (isWorking() && (timeout > 0) ) {
        rtpData = rtpQueue.dequeue();
        if (!rtpData.isEmpty()) {
            break;
        }
        if(rtpQueue.getSize() > 10){
            break;
        }
        if (rtpData.isEmpty()) {
            QThread::msleep(50);
            timeout = timeout - 1;
            if (rtpQueue.getSize() > 25) {
                qDebug() << "RTCSplitter::dequeueWithTimeout::bypass rtp pkt at " << downloadTimeWait - timeout;
                break;
            }
        }
    }
    this->sequenceOfTimeooutPackage += downloadTimeWait-timeout;
    if (rtpData.isEmpty()) rtpQueue.next();
    return rtpData;
}

QImage RTCSplitter::decode2Image(QByteArray h264Data) {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();

    QImage image;
    if (h264Data.size() > 0) {
        // h264 processing
        bool ret;
        int nalu_header_byte = h264Data.mid(4, 1).toHex().toInt(&ret,16);
        int nalType = nalu_header_byte & 0b11111;
        switch (nalType) {
        case 7: { // SPS Type
            decodeRTPPackage(h264Data);
        } break;
        case 8: { // PPS Type
            decodeRTPPackage(h264Data);
        }
            break;
        case 5: { // IDR Typ
            image = decodeRTPPackage(h264Data);
        }
            break;
        case 1: { // P-Frame  Pictur Type
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

void RTCSplitter::onReceiveRTPPackage(double timestamp, QByteArray h264Package) {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    //        qDebug() << playerName << " Thread: " << this->thread()
    //                 << "RTCSplitter onReceiveRTPPackage - packageIndex: " << rtpPackageIndex;

    Cam9ImageQueue & imageQueue = this->pRTCPlayer->getImageQueue();
    QImage image = decode2Image(h264Package);
    if (!image.isNull())
        imageQueue.enqueue(timestamp, image);
}

QImage RTCSplitter::decodeRTPPackage(QByteArray rtpPackage) {
    QImage image;
    int rtpSize = rtpPackage.size();
    if (rtpSize > 0) {
        quint8 *data = new quint8[rtpSize];
        memcpy(data, rtpPackage.constData(), rtpSize);
        AVPacket avPacket;
        av_init_packet(&avPacket);

        avPacket.size = rtpSize;
        avPacket.data = data;

        AVFrame *avFrame = av_frame_alloc();
        if(avFrame == NULL){
            qDebug() << "Unable to allocate frames \n";
        }

        int decodedFrameSize = 0, decodedFrameCount = 0;

        decodedFrameSize = avcodec_decode_video2(avCodecContext, avFrame, &decodedFrameCount, &avPacket);
        if (decodedFrameSize > 0 && decodedFrameCount > 0) {
            int width = avCodecContext->width;
            int height = avCodecContext->height;

            if(width > 0 && height >0){
                struct SwsContext *swsContext = sws_getContext(width , height,
                                                               avCodecContext->pix_fmt, avCodecContext->width,
                                                               avCodecContext->height, AV_PIX_FMT_RGB24, SWS_BICUBIC,
                                                               NULL, NULL, NULL);

                uint8_t *outBuffer = (uint8_t *)av_malloc(avpicture_get_size(
                                                              AV_PIX_FMT_RGB24, avCodecContext->width, avCodecContext->height));
                AVPicture avPicture;
                avpicture_fill(&avPicture, outBuffer, AV_PIX_FMT_RGB24,
                               avCodecContext->width, avCodecContext->height);
                if(swsContext)    { //valid
                    sws_scale(swsContext, (const uint8_t *const *)avFrame->data,
                              avFrame->linesize, 0, avCodecContext->height,
                              avPicture.data, avPicture.linesize);

                    image = QImage(avPicture.data[0],
                            avFrame->width,
                            avFrame->height,
                            avPicture.linesize[0],
                            QImage::Format_RGB888).copy();

                    //                    QString rootPath =  QDir::homePath() + "/" + "imageVideoWall";
                    //                    if (!QDir(rootPath).exists()) {
                    //                        QDir().mkpath(rootPath);
                    //                    }

                    //                    QString filename =  rootPath + "/" + QString::number(QDateTime::currentMSecsSinceEpoch()) + ".png";
                    //                    qDebug() << "FILENAME" <<filename;
                    //                    image.save(filename);

                }

                if (image.isNull() || image.height() <= 0) {
                    qDebug() << "invalid package of H264 package size = " << rtpSize ;
                }
                av_free(outBuffer);
                sws_freeContext(swsContext);
            }
        }
        av_frame_unref(avFrame);
        av_frame_free(&avFrame);
        av_packet_unref(&avPacket);
        av_free_packet(&avPacket);
        delete[] data;
    }

    return image;
}

QImage RTCSplitter::decodeRTPPackage2(QByteArray rtpPackage) {

}

void RTCSplitter::decoderOpen() {

    avCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!avCodec) {
        // TODO
    }
    avCodecContext = avcodec_alloc_context3(avCodec);
    if (!avCodecContext) {
        // TODO
    }else{
        if (avCodec->capabilities & CODEC_CAP_TRUNCATED) {
            avCodecContext->flags || CODEC_FLAG_TRUNCATED;
        }
        if (avcodec_open2(avCodecContext, avCodec, NULL) < 0) {
            // TODO
        }
        avCodecContext->thread_count = 4;
        avCodecContext->thread_type = 2;
    }
}

void RTCSplitter::decoderClose() {
    if(avCodecContext && avCodecContext->internal && avCodecContext->refcounted_frames != 0) {
        avcodec_flush_buffers(avCodecContext);
        avcodec_free_context(&avCodecContext);
    }
}
