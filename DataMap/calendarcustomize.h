#ifndef CALENDARCUSTOMIZE_H
#define CALENDARCUSTOMIZE_H

#include <QCalendarWidget>
#include <QMessageBox>
#include <QPainter>
#include <QDebug>
class CalendarCustomize:public QCalendarWidget
{
public:
    QList<QDate> listDatesMax;
    QList<QHash<QDate, int>> listStateOfDates;
    QDate dateSelected;
    CalendarCustomize(QWidget *parent): QCalendarWidget(parent){}
    ~CalendarCustomize(){}
    QList<QHash<QDate, int> > getListStateOfDates() const;
    void setListStateOfDates(const QList<QHash<QDate, int> > &value);

    QDate getDateSelected() const;
    void setDateSelected(const QDate &value);

    QList<QDate> getListDatesMax() const;
    void setListDatesMax(const QList<QDate> &value);
    QDate findDateMax(QList<QDate> listDates){
        QDate maxDate;
        if(listDates.size() != 0){
            maxDate = listDates.at(0);
            for (int index = 1; index < listDates.size(); ++index) {
                if(maxDate < listDates.at(index)){
                    maxDate = listDates.at(index);
                }
            }
        }
        return maxDate;
    }

protected:
    /*!
       Override the method of the parent class and add some indicators to show today and days with scheduled events
       \param painter default
       \param rect default
       \param date default
     */
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
    {
        painter->save();
        QPen pen;
        //mac dinh
//        QCalendarWidget::paintCell(painter, rect, date);
        //neu la ngay duoc chon
        if(date == dateSelected){
            pen.setColor(Qt::white);
            painter->setPen(pen);
            //            painter->drawText(rect, Qt::AlignLeft, "");
            painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
        }
        if(listDatesMax.size() != 0){
            int indexOfDay = listDatesMax.indexOf(date);
            if(indexOfDay == -1){
                //neu khong nam trong danh sach listdates max
                //                pen.setColor(QColor("#616161"));
                pen.setColor(QColor("#B6B6B6"));
                painter->setPen(pen);
                painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
            }else{
                pen.setColor(Qt::white);
                painter->setPen(pen);
                painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
            }
        }
        painter->restore();
    }
    void paintEvent(QPaintEvent *event) {
    }

};

#endif // CALENDARCUSTOMIZE_H
