#include "calendarcustomize.h"



/*!
  Override a mouse event in our custom widget. Display a msg box to show today
   \param e The mouse event
 */
QDate CalendarCustomize::getDateSelected() const
{
    return dateSelected;
}

void CalendarCustomize::setDateSelected(const QDate &value)
{
    dateSelected = value;
}

QList<QDate> CalendarCustomize::getListDatesMax() const
{
    return listDatesMax;
}

void CalendarCustomize::setListDatesMax(const QList<QDate> &value)
{
    listDatesMax = value;
}

QList<QHash<QDate, int> > CalendarCustomize::getListStateOfDates() const
{
    return listStateOfDates;
}

void CalendarCustomize::setListStateOfDates(const QList<QHash<QDate, int> > &value)
{
    listStateOfDates = value;
}

