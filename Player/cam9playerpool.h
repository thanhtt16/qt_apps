#ifndef Cam9PlayerPOOL_H
#define Cam9PlayerPOOL_H

#include <QDebug>
#include <QList>
#include <QObject>
#include <QWidget>
#include "Cam9Player/c_cam9player.h"
#include "Cam9RTCPlayer/c_cam9rtcplayer.h"
#include "PacModel/control.h"
#include <QString>
#include "message.h"
class C_Cam9Player;
class C_Cam9RTCPlayer;

struct PLAYER_DATA_STRUCT{
    Control *control = Q_NULLPTR;
    QWidget *zone = Q_NULLPTR;
    QString appName = "";
    QString displayName = "";
};

Q_DECLARE_METATYPE(PLAYER_DATA_STRUCT)


class Cam9PlayerPool : public QObject {
    Q_OBJECT
private:
    QList<QVariant *> playerPoolVideowall;
    QList<QVariant *> playerPoolPlayback;

    C_Cam9Player *createNewPlayer(Control *control, QWidget *zone);
    C_Cam9RTCPlayer *createNewCam9RTCPlayer(Control *control, QWidget *zone);

protected:
    Cam9PlayerPool();
    ~Cam9PlayerPool();

public:
    AppMessage Message;

    static Cam9PlayerPool &instance() {
        static Cam9PlayerPool mInstance;
        return mInstance;
    }

    Cam9PlayerPool(Cam9PlayerPool const &) = delete;
    Cam9PlayerPool(Cam9PlayerPool &&) = delete;
    Cam9PlayerPool &operator=(Cam9PlayerPool const &) = delete;
    Cam9PlayerPool &operator=(Cam9PlayerPool &&) = delete;

    C_Cam9Player *getPlayer(Control *control, QWidget *zone, QString appName = "",
                            QString displayName = "");
    C_Cam9RTCPlayer *getCam9RTCPlayer(Control *control, QWidget *zone, QString appName = "",
                                      QString displayName = "");
    void playAll(QString appName);
    void newAction(int message, QVariant* attachment);

Q_SIGNALS:

public Q_SLOTS:
};

#endif  // C_Cam9PlayerPOOL_H
