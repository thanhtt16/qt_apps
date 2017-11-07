#include "datamapofcamera.h"

const QString DataMapOfCamera::baseAPI = "/getChannelDataMapOfCamera";

DataMapCamera DataMapOfCamera::getSdDataMapOfCamera() const
{
    return sdDataMapOfCamera;
}

void DataMapOfCamera::setSdDataMapOfCamera(const DataMapCamera &value)
{
    sdDataMapOfCamera = value;
}

DataMapCamera DataMapOfCamera::getHdDataMapOfCamera() const
{
    return hdDataMapOfCamera;
}

void DataMapOfCamera::setHdDataMapOfCamera(const DataMapCamera &value)
{
    hdDataMapOfCamera = value;
}

QList<QDate> DataMapOfCamera::getListDaysMap(bool isMain)
{

    QList<QDate> listDaysMap;

    if(isMain){
        for (int index = 0; index < sdDataMapOfCamera.daysMap.size(); ++index) {
            QHash<QString, int> dayMap = sdDataMapOfCamera.daysMap.at(index);
            QString keyDay = dayMap.keys().first();
            listDaysMap.append(QDate::fromString(keyDay, "yyyy/M/d"));
        }

    }else{
        for (int index = 0; index < sdDataMapOfCamera.daysMap.size(); ++index) {
            QHash<QString, int> dayMap = sdDataMapOfCamera.daysMap.at(index);
            QString keyDay = dayMap.keys().first();
            listDaysMap.append(QDate::fromString(keyDay, "yyyy/M/d"));
        }
    }
    return listDaysMap;
}

DataMapOfCamera::DataMapOfCamera()
{
    //    https://core.vcam.viettel.vn/api/getChannelDataMapOfCamera?start_time=1506150028&end_time=1506582028&camera_id=6431

}

void DataMapOfCamera::loadDataMapOfCamera(long startTimeStamp, long endTimeStamp, int cameraId,
                                          std::function<void(DataMapOfCamera*)> onSuccess,
                                          std::function<void(QString)> onFailure){

    QMap<QString, QString> params;
    params.insert("start_time", QString("%1").arg(startTimeStamp));
    params.insert("end_time", QString("%1").arg(endTimeStamp));
    params.insert("camera_id", QString("%1").arg(cameraId));

    std::function<void(QJsonObject)> onFetchSuccess =
            [onSuccess](QJsonObject jsonObject) {

        DataMapOfCamera *dataMapOfCamera = parseDataMapCamera(jsonObject);


        //        dataMapOfCamera->getHoursMapOfDay(false,"2017/9/19");

        //        qDebug() << "THAO GET STATE" <<  dataMapOfCamera->getSateDataOfDay(false,"2017/9/19");
        //        qDebug() << "THAO GET STATE _ HOUR" << dataMapOfCamera->getStateHourOfDay(false,"2017/9/19",21);

        //        qDebug() <<"DATA_DAY_MAP_PARSER" <<dataMapOfCamera->sdDataMapOfCamera.daysMap.size();
        //        qDebug() <<"DATA_DETAIl_MAP_PARSER" <<dataMapOfCamera->sdDataMapOfCamera.detailMap.size();
        //        qDebug() <<"DATA_HOURS_MAP_PARSER" <<dataMapOfCamera->sdDataMapOfCamera.hoursMap.size();

        onSuccess(dataMapOfCamera);



    };

    std::function<void(QString)> onFetchFailure = [onFailure](QString message) {
        onFailure(message);
    };

    NetworkUtils::instance().getRequest(baseAPI, params, onFetchSuccess, onFetchFailure);
}

//state của giờ bằng 0 : không có dữ liệu :: bằng 1: có dữ liệu :: bằng 2 :: có đoạn có có đoạn không
QList<int> DataMapOfCamera::createMapHourForSeekBar(bool isMain, QDate date, int hour){
    QList<int> listHourMap;
    createDefaultValueForList(listHourMap, 0);
    if(isMain){
        if( hdDataMapOfCamera.detailMap.size() == 0) return listHourMap;
    }else{
        if( sdDataMapOfCamera.detailMap.size() == 0) return listHourMap;
    }

    if(getStateHourOfDay(isMain,date,hour) == 1){
//        qDebug() << "GIO CO DU LIEU  DAY DU == 1";
        defaultValueForList(listHourMap, 1); //toàn bộ giờ này có dữ liệu
    }

    if(getStateHourOfDay(isMain,date,hour) == 0){
//        qDebug() << "GIO KHONG CO DU LIEU  DAY DU == 0";
        defaultValueForList(listHourMap, 0); //toàn bộ giờ này có dữ liệu
    }

    if(getStateHourOfDay(isMain,date,hour) == 2){
//        qDebug() << "GIO KHONG CO DU LIEU  DAY DU == 2";
        QDateTime dateTime = QDateTime(date);

        qDebug() << "DATE CREARTE" <<dateTime;
        long timeStampStartOfHour =  dateTime.toTime_t() + hour * 3600;
        qDebug() << "timestamps" << timeStampStartOfHour;
        listHourMap = calculatorDataMap(timeStampStartOfHour, 3600 , isMain , listHourMap);
    }

    return listHourMap;
}


QList<int> DataMapOfCamera::calculatorDataMap(long timeStampStartOfHour, int duration, bool isMain, QList<int> &listHourMap){
    qDebug() << Q_FUNC_INFO;
    int indexFirst = 0;
    int indexLast = 0;
    int indexListHour = 0;
    long timeStampStartDetail = 0;
    long timeStampEndOfHour = timeStampStartOfHour + duration;
    DetailMap detailMap;
    if(isMain){
        if(hdDataMapOfCamera.detailMap.size() > 0 ){
            detailMap = hdDataMapOfCamera.detailMap.at(0);
            if(timeStampStartOfHour - detailMap.start_time < 0){
                for (int index = 0; index <= (detailMap.start_time-timeStampStartOfHour); ++index) {
                    if(indexListHour != 3600){
                        listHourMap.replace(indexListHour++,0);
                    }
                }
            }
        }


        for (int i = 0; i < hdDataMapOfCamera.detailMap.size(); ++i) {
            detailMap = hdDataMapOfCamera.detailMap.at(i);
            //neu timestamp nho hon chuoi json ban dau

            if ((timeStampStartOfHour >= detailMap.end_time)) {
                indexFirst++;
                if(indexFirst >= hdDataMapOfCamera.detailMap.size()) {
                    indexFirst = hdDataMapOfCamera.detailMap.size() - 1;
                    break;
                }
            }
            if ((detailMap.end_time < timeStampEndOfHour)) {
                indexLast++;
                if(indexLast >= hdDataMapOfCamera.detailMap.size()) {
                    indexLast = hdDataMapOfCamera.detailMap.size() - 1;
                    break;
                }
            }
        }
        qDebug() << "INDEX FIRST" << indexFirst << "LAST" << indexLast;

        for (int index = indexFirst; index <= indexLast; ++index) {
            detailMap = hdDataMapOfCamera.detailMap.at(index);
            for (int index = detailMap.start_time; index <= detailMap.end_time; ++index) {
                if(indexListHour != 3600){
                    listHourMap.replace(indexListHour++,detailMap.value);
                }

            }
        }
    }else{

        //truong hop chon timestamp nho hon json tra ve
        if(sdDataMapOfCamera.detailMap.size() > 0 ){
            detailMap = sdDataMapOfCamera.detailMap.at(0);
            if(timeStampStartOfHour - detailMap.start_time < 0){
                for (int index = 0; index <= (detailMap.start_time-timeStampStartOfHour); ++index) {
                    if(indexListHour != 3600){
                        listHourMap.replace(indexListHour++,0);
                    }
                }
            }
        }


        for (int i = 0; i < sdDataMapOfCamera.detailMap.size(); ++i) {
            detailMap = sdDataMapOfCamera.detailMap.at(i);
            if ((timeStampStartOfHour >= detailMap.end_time)) { //neu lớn hơn hoặc bằng tức nó nằm ở khối sau không phải khối hiện tại
                indexFirst++;
                if(indexFirst >= sdDataMapOfCamera.detailMap.size()) {
                    indexFirst = sdDataMapOfCamera.detailMap.size() - 1;
                    break;
                }
            }
            if ((detailMap.end_time < timeStampEndOfHour)) {
                indexLast++;
                if(indexLast >= sdDataMapOfCamera.detailMap.size()) {
                    indexLast = sdDataMapOfCamera.detailMap.size() - 1;
                    break;
                }
            }
        }

        qDebug() << "INDEX FIRST" << indexFirst << "LAST" << indexLast << "SIZE DETAIL" << sdDataMapOfCamera.detailMap.size() ;
        for (int index = indexFirst; index <= indexLast; ++index) {
            detailMap = sdDataMapOfCamera.detailMap.at(index);
            for (int index = detailMap.start_time; index <= detailMap.end_time; ++index) {
                if(indexListHour != 3600){
                    listHourMap.replace(indexListHour++,detailMap.value);
                }

            }
        }
    }
    return listHourMap;
}



//set default value
QList<int> DataMapOfCamera::createDefaultValueForList(QList<int>& list, int value){
    for (int index = 0; index < 3600; ++index) {
        list.append(value);
    }
    return list;
}

//set default value
QList<int> DataMapOfCamera::defaultValueForList(QList<int>& list, int value){
    for (int index = 0; index < 3600; ++index) {
        list.replace(index,value);
    }
    return list;
}

DataMapOfCamera *DataMapOfCamera::parseDataMapCamera(QJsonObject jsonObject){

    DataMapOfCamera *dataMapOfcamera = new DataMapOfCamera();

    for (int index = 0; index < jsonObject.keys().size(); ++index) {
        QString key = jsonObject.keys().at(index);
        QJsonObject obj = jsonObject.value(key).toObject();

        if(key.contains("SD")){
            QJsonArray jsonDetailsMapArr = obj.take("detailsMap").toArray();
            for (int index = 0; index < jsonDetailsMapArr.size(); ++index) {
                QJsonObject jsonObj = jsonDetailsMapArr.at(index).toObject();
                DetailMap detailMap = parserDetailMap(jsonObj);
                dataMapOfcamera->sdDataMapOfCamera.detailMap.append(detailMap);
            }

            QJsonObject jsonHoursMap = obj.take("hoursMap").toObject();
            for (int index = 0; index < jsonHoursMap.keys().size(); ++index) {
                QString keyDay = jsonHoursMap.keys().at(index);
                QList<QHash<QString, int>> listHourOfDay;
                //take data hour of one day
                QJsonObject jsonObjectHourOfDay = jsonHoursMap.value(keyDay).toObject();
                for (int index = 0; index < jsonObjectHourOfDay.keys().size(); ++index) {
                    QHash<QString, int> hourMap;
                    QString keyhour = jsonObjectHourOfDay.keys().at(index);
                    int value = jsonObjectHourOfDay.value(keyhour).toInt();
                    hourMap.insert(keyhour, value);
                    listHourOfDay.append(hourMap);
                }

                QHash<QString, QList<QHash<QString, int>>> hoursOfDay;
                hoursOfDay.insert(keyDay, listHourOfDay);
                dataMapOfcamera->sdDataMapOfCamera.hoursMap.append(hoursOfDay);
            }


            QJsonValue jsonValueOfDay;
            QJsonObject jsonDaysMap = obj.take("daysMap").toObject();
            int sizeDaysMap = jsonDaysMap.keys().size();
            for (int index = 0; index < sizeDaysMap; ++index) {
                //                qDebug() << "+++++++++++DAYINDEX_SD" << index;
                QString keyDay = jsonDaysMap.keys().at(index);
                //                qDebug() << "DAY KEY" << keyDay;
                jsonValueOfDay = jsonDaysMap.value(keyDay);
                if(!jsonValueOfDay.isNull()){
                    QHash<QString, int> dayMap;
                    dayMap.insert(keyDay,jsonValueOfDay.toInt());
                    dataMapOfcamera->sdDataMapOfCamera.daysMap.append(dayMap);
                }
            }
        }

        if(key.contains("HD")){
            QJsonArray jsonDetailsMapArr = obj.take("detailsMap").toArray();
            for (int index = 0; index < jsonDetailsMapArr.size(); ++index) {
                QJsonObject jsonObj = jsonDetailsMapArr.at(index).toObject();
                DetailMap detailMap = parserDetailMap(jsonObj);
                dataMapOfcamera->hdDataMapOfCamera.detailMap.append(detailMap);
            }

            QJsonObject jsonHoursMap = obj.take("hoursMap").toObject();
            for (int index = 0; index < jsonHoursMap.keys().size(); ++index) {
                QString keyDay = jsonHoursMap.keys().at(index);
                QList<QHash<QString, int>> listHourOfDay;
                //take data hour of one day
                QJsonObject jsonObjectHourOfDay = jsonHoursMap.value(keyDay).toObject();
                for (int index = 0; index < jsonObjectHourOfDay.keys().size(); ++index) {
                    QHash<QString, int> hourMap;
                    QString keyhour = jsonObjectHourOfDay.keys().at(index);
                    int value = jsonObjectHourOfDay.value(keyhour).toInt();
                    hourMap.insert(keyhour, value);
                    listHourOfDay.append(hourMap);
                }

                QHash<QString, QList<QHash<QString, int>>> hoursOfDay;
                hoursOfDay.insert(keyDay, listHourOfDay);
                dataMapOfcamera->hdDataMapOfCamera.hoursMap.append(hoursOfDay);
            }


            QJsonValue jsonValueOfDay;
            QJsonObject jsonDaysMap = obj.take("daysMap").toObject();
            int sizeDaysMap = jsonDaysMap.keys().size();
            for (int index = 0; index < sizeDaysMap; ++index) {
                QString keyDay = jsonDaysMap.keys().at(index);
//                qDebug() << "DAY KEY" << keyDay;
                jsonValueOfDay = jsonDaysMap.value(keyDay);
                if(!jsonValueOfDay.isNull()){
                    QHash<QString, int> dayMap;
                    dayMap.insert(keyDay,jsonValueOfDay.toInt());
                    dataMapOfcamera->hdDataMapOfCamera.daysMap.append(dayMap);
                }
            }
        }

    }
    return dataMapOfcamera;
}


//Trả về danh sách thông tin của giờ từ  0 -- 23 của một ngày tương ứng
QList<QHash<QString, int>> DataMapOfCamera::getHoursMapOfDay(bool isMain, QDate date){
    QString day = date.toString("yyyy/M/d");
    QList<QHash<QString, int>> hoursMapOfDay;
    if(isMain){
        for (int index = 0; index < hdDataMapOfCamera.hoursMap.size(); ++index) {
            QString keyDay = hdDataMapOfCamera.hoursMap.at(index).keys().first();
            if(day.compare(keyDay) == 0){
                hoursMapOfDay = hdDataMapOfCamera.hoursMap.at(index).value(day);
            }
        }
    }else{
        for (int index = 0; index < sdDataMapOfCamera.hoursMap.size(); ++index) {
            QString keyDay = sdDataMapOfCamera.hoursMap.at(index).keys().first();
            if(day.compare(keyDay) == 0){
                hoursMapOfDay = sdDataMapOfCamera.hoursMap.at(index).value(day);
            }
        }
    }
    return hoursMapOfDay;
}

//1 : xanh
//2 vang (vua co vua khong)
//0: do (khong co data)
//lấy trạng thái dữ liệu của một ngày
int DataMapOfCamera::getSateDataOfDay(bool isMain,QDate date){
    QString day = date.toString("yyyy/M/d");
    int stateResult = 0;
    if(isMain){
        for (int index = 0; index < hdDataMapOfCamera.daysMap.size(); ++index) {
            QString keyDay = hdDataMapOfCamera.daysMap.at(index).keys().first();
            if(day.compare(keyDay) == 0){
                stateResult = hdDataMapOfCamera.daysMap.at(index).value(day);
            }
        }
    }else{
        for (int index = 0; index < sdDataMapOfCamera.daysMap.size(); ++index) {
            QString keyDay = sdDataMapOfCamera.daysMap.at(index).keys().first();
            qDebug() << Q_FUNC_INFO << "THAO _ DATE GETSTATE" << day << "KEYDAY" << keyDay;

            if(day.compare(keyDay) == 0){
                stateResult = sdDataMapOfCamera.daysMap.at(index).value(day);
            }
        }
    }
    return stateResult;
}


//1 : xanh
//2 vang (vua co vua khong)
//0: do (khong co data)
//lấy trạng thái dữ liệu của một ngày
QList<QHash<QDate, int>>  DataMapOfCamera::getSateDataOfDates(bool isMain,QList<QDate> dates){
    QList<QHash<QDate, int>> stateDatesMap;
    for (int index = 0; index < dates.size(); ++index) {
        QHash<QDate, int> stateOneDate;
        stateOneDate.insert(dates.at(index), -1);
        stateDatesMap.append(stateOneDate);
    }

    QList<QString> listKeyDate;
    QString dayConvert;
    int stateResult = 0;
    if(isMain){

        for (int index = 0; index < hdDataMapOfCamera.daysMap.size(); ++index) {
            QString keyDay = hdDataMapOfCamera.daysMap.at(index).keys().first();
            listKeyDate.append(keyDay);
        }

        for (int i = 0; i < listKeyDate.size(); ++i) {
            int indexDayKey = dates.indexOf(QDate::fromString(listKeyDate.at(i), "yyyy/M/d"));
            if(indexDayKey != -1){
                dayConvert = dates.at(indexDayKey).toString("yyyy/M/d");
                int indexState = listKeyDate.indexOf(dayConvert);
                int state = hdDataMapOfCamera.daysMap.at(indexState).value(dayConvert);
                QHash<QDate, int> stateOneDate;
                stateOneDate.insert(dates.at(indexDayKey), state);
                stateDatesMap.replace(indexDayKey, stateOneDate);
            }
        }

    }else{
        for (int index = 0; index < sdDataMapOfCamera.daysMap.size(); ++index) {
            QString keyDay = sdDataMapOfCamera.daysMap.at(index).keys().first();
            listKeyDate.append(keyDay);
        }

        for (int i = 0; i < listKeyDate.size(); ++i) {
            int indexDayKey = dates.indexOf(QDate::fromString(listKeyDate.at(i), "yyyy/M/d"));
            if(indexDayKey != -1){
                dayConvert = dates.at(indexDayKey).toString("yyyy/M/d");
                int indexState = listKeyDate.indexOf(dayConvert);
                int state = sdDataMapOfCamera.daysMap.at(indexState).value(dayConvert);
                QHash<QDate, int> stateOneDate;
                stateOneDate.insert(dates.at(indexDayKey), state);
                stateDatesMap.replace(indexDayKey, stateOneDate);
            }
        }
    }

    return stateDatesMap;
}





//Lấy trạng thái của một giờ trong một ngày
//state của giờ bằng 0 : không có dữ liệu :: bằng 1: có dữ liệu :: bằng 2 :: có đoạn có có đoạn không
int DataMapOfCamera::getStateHourOfDay(bool isMain,QDate date, int hour){
    QString day = date.toString("yyyy/M/d");
    int stateResult = 0;
    QHash<QString, int> hourMap;
    QString keyDay;
    if(isMain){
        if(hdDataMapOfCamera.hoursMap.size() == 0) return 0;

        for (int index = 0; index < hdDataMapOfCamera.hoursMap.size(); ++index) {
            keyDay = hdDataMapOfCamera.hoursMap.at(index).keys().first();
            if(day.compare(keyDay) == 0){
                for(int i = 0; i < hdDataMapOfCamera.hoursMap.at(index).value(day).size();i++){
                    hourMap = hdDataMapOfCamera.hoursMap.at(index).value(day).at(i);
                    QString keyhour = hourMap.keys().first();
                    if( keyhour == QString::number(hour)){
                        stateResult = hourMap.value(keyhour);
                    }
                }
            }
        }
    }else{
        if(sdDataMapOfCamera.hoursMap.size() == 0) return 0;
        for (int index = 0; index < sdDataMapOfCamera.hoursMap.size(); ++index) {
            keyDay = sdDataMapOfCamera.hoursMap.at(index).keys().first();
            if(day.compare(keyDay) == 0){
                for(int i = 0; i < sdDataMapOfCamera.hoursMap.at(index).value(day).size();i++){
                    hourMap = sdDataMapOfCamera.hoursMap.at(index).value(day).at(i);
                    QString keyhour = hourMap.keys().first();
                    if( keyhour == QString::number(hour)){
                        stateResult = hourMap.value(keyhour);
                    }
                }
            }
        }
    }
    return stateResult;
}


QList<int>  DataMapOfCamera::getSateDataOfHours(bool isMain,QList<QString> hours, QDate dateSelected){

    QList<int> mapStateHours;
    //default
    for (int index = 0; index < hours.size(); ++index) {
        mapStateHours.append(-1);
    }

    QList<QString> listKeyHous;
    int stateResult = 0;
    QString daySelectedString = dateSelected.toString("yyyy/M/d");
    QList<QHash<QString, int>> listStateHours;
    if(isMain){
        //lay danh sach states cua hours
        for (int index = 0; index < hdDataMapOfCamera.hoursMap.size(); ++index) {
            QHash<QString, QList<QHash<QString, int>>> dayMapListHours;
            dayMapListHours = hdDataMapOfCamera.hoursMap.at(index);
            QString keyday = dayMapListHours.keys().first();
            if(keyday ==  daySelectedString){
                listStateHours = hdDataMapOfCamera.hoursMap.at(index).value(keyday);
                break;
            }
        }

        QList<QString> keyhours;
        for (int index = 0; index < listStateHours.size(); ++index) {
            QHash<QString, int> mapStateHour;
            mapStateHour = listStateHours.at(index);
            QString keyhour = mapStateHour.keys().first();
            keyhours.append(keyhour);
        }

        // thay doi trang thai cua gio
        for (int index = 0; index < keyhours.size(); ++index) {
            int indexMapHour = keyhours.indexOf(QString::number(index));

            QHash<QString, int> mapStateHour;
            mapStateHour = listStateHours.at(indexMapHour);
            QString key = mapStateHour.keys().first();
            int state = mapStateHour.value(key);
            mapStateHours.replace(index, state);
        }

    }else{

        //lay danh sach states cua hours
        for (int index = 0; index < sdDataMapOfCamera.hoursMap.size(); ++index) {
            QHash<QString, QList<QHash<QString, int>>> dayMapListHours;
            dayMapListHours = sdDataMapOfCamera.hoursMap.at(index);
            QString keyday = dayMapListHours.keys().first();
            if(keyday ==  daySelectedString){
                listStateHours = sdDataMapOfCamera.hoursMap.at(index).value(keyday);
                break;
            }
        }

        QList<QString> keyhours;
        for (int index = 0; index < listStateHours.size(); ++index) {
            QHash<QString, int> mapStateHour;
            mapStateHour = listStateHours.at(index);
            QString keyhour = mapStateHour.keys().first();
            keyhours.append(keyhour);
        }

        // thay doi trang thai cua gio
        for (int index = 0; index < keyhours.size(); ++index) {
            int indexMapHour = keyhours.indexOf(QString::number(index));

            QHash<QString, int> mapStateHour;
            mapStateHour = listStateHours.at(indexMapHour);
            QString key = mapStateHour.keys().first();
            int state = mapStateHour.value(key);
            mapStateHours.replace(index, state);
        }

    }

    return mapStateHours;
}




DetailMap DataMapOfCamera::parserDetailMap(QJsonObject jsonObject){
    DetailMap detailMap;
    QJsonValue jsonValue;
    jsonValue = jsonObject.take("start_time");
    if(!jsonValue.isNull()){
        detailMap.start_time = (long)jsonValue.toDouble();
    }

    jsonValue = jsonObject.take("end_time");
    if(!jsonValue.isNull()){
        detailMap.end_time = (long)jsonValue.toDouble();
    }

    jsonValue = jsonObject.take("value");
    if(!jsonValue.isNull()){
        detailMap.value = jsonValue.toInt();
    }
    return detailMap;
}


