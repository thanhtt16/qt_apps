#ifndef P_Socket_H
#define P_Socket_H

#include <PacModel/presentation.h>
#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QMovie>
#include <QThread>
#include <QTimer>

class C_Socket;

class P_Socket : public Presentation {
private:

public:
    QWidget *zone  = Q_NULLPTR;
    P_Socket(Control *ctrl, QWidget *zone);
    void changeControl(Control *ctrl);
    void update();
    QVBoxLayout *centerLayout = Q_NULLPTR;
    // center
    QLabel *messageLabel = Q_NULLPTR;
    QLabel *textLabel = Q_NULLPTR;
    QMovie *loaderMovie = Q_NULLPTR;

    QObject *getZone(int zoneId);

    void showLoading();
    void hideLoading();
};

#endif  // PRESENTATION_H
