#ifndef DATAMAPOFCAMERA_H
#define DATAMAPOFCAMERA_H

#include <QObject>
#include <QMap>
#include <QHash>
#include <Common/networkutils.h>
#include <QString>
#include <QJsonObject>
struct 	DetailMap{
    long start_time;
    long  end_time;
    int value;
};

struct DataMapCamera{
    QList<DetailMap> detailMap;
    QList<QHash<QString, QList<QHash<QString, int>>>> hoursMap;
    QList<QHash<QString, int>> daysMap;
};

class DataMapOfCamera: public QObject
{
    Q_OBJECT
private:


public:
    DataMapCamera sdDataMapOfCamera;
    DataMapCamera hdDataMapOfCamera;
    DataMapOfCamera();
    const static QString baseAPI;
    static void loadDataMapOfCamera(long startTimeStamp, long endTimeStamp, int cameraId,
                                    std::function<void(DataMapOfCamera*)> onSuccess,
                                    std::function<void(QString)> onFailure);
    static DataMapOfCamera* parseDataMapCamera(QJsonObject jsonObject);

    static DetailMap parserDetailMap(QJsonObject jsonObject);

    QList<QHash<QString, int>> getHoursMapOfDay(bool isMain,QDate date);

    int getSateDataOfDay(bool isMain,QDate date);
    QList<QHash<QDate, int>>  getSateDataOfDates(bool isMain,QList<QDate> dates);

    QList<int>  getSateDataOfHours(bool isMain,QList<QString> hours, QDate dateSelected);

    int getStateHourOfDay(bool isMain,QDate date, int hour);


    DataMapCamera getSdDataMapOfCamera() const;

    void setSdDataMapOfCamera(const DataMapCamera &value);
    DataMapCamera getHdDataMapOfCamera() const;
    void setHdDataMapOfCamera(const DataMapCamera &value);
    QList<QDate> getListDaysMap(bool isMain);


    //seekbar datamap

    QList<int> defaultValueForList(QList<int>& list, int value);
    QList<int> createDefaultValueForList(QList<int>& list, int value);

    QList<int> createMapHourForSeekBar(bool isMain, QDate date, int hour);
    QList<int> calculatorDataMap(long timeStampStartOfHour, int duration, bool isMain, QList<int> &listHourMap);

};


#endif // DATAMAPOFCAMERA_H
