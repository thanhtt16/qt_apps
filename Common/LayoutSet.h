#ifndef LAYOUTSET_H
#define LAYOUTSET_H

#include <QList>
#include <QObject>
#include <QString>
#include "message.h"

struct LayoutStruct {
  int code;
  QString label;
  int numberOfCameras;
  int selectedPage;
  int rows;
  int cols;
};
Q_DECLARE_METATYPE(LayoutStruct)
class LayoutSet : public QObject {
  Q_OBJECT

 public:
  LayoutSet(QObject *parent = 0);
  QList<LayoutStruct> layoutList;
  LayoutStruct getLayout(int layoutCode);
};

#endif  // LAYOUTSET_H
