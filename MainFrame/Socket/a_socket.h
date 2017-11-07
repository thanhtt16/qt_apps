#ifndef A_Socket_H
#define A_Socket_H

#include <QObject>
#include "PacModel/control.h"
#include <sio_client.h>
#include "Authentication/user.h"
#include "Camera/camitem.h"
#include "Common/appprofile.h"
#include "Common/networkutils.h"
#include "c_socket.h"
using namespace sio;
using namespace std;

class CamItem;
class AppProfile;
class C_Socket;

class A_Socket : public Abstraction {
    Q_OBJECT
private:


public:
    C_Socket *control() { return (C_Socket *)ctrl; }
    A_Socket(Control *ctrl);
    void changeControl(Control *ctrl);
    std::unique_ptr<client> _io;
    std::map<std::string, std::string> mOptions;
    QString mDeviceID = "{\"device_id\":\"%1\"}";
    QString joinRoomData = "{data: {\"user_id\": \"%1\", \"type\": 'client'}}";
    QString urlRecordCurrent = "";
    //    QString getClipData = "{data: {\"camera_id\": \"%1\",\"camera_name\":\"%2\", \"url\":\"%3\"}}";
    QString getClipData = "{data: {'camera_id': %1 ,'camera_name':'%2', 'url':'%3'}}";

    QString cancelRecord = "data: { url: \"%1\"}";

    void addSocketListener();
    void connectToSocket();
    //    void startGetClip(std::string const& cameraId,std::string const& cameraName,std::string const& urlVideo);
    void startGetClip(int cameraId,QString videoName,QString urlVideo);

    void cancelGetClip();

public Q_SLOTS:
    void onOpened(std::string const &nsp);
    void onClosed(client::close_reason const &reason);
    void onFailed();
    void startJoinClipRoom(std::string const& userId);

    void onUserJoined(std::string const &name, message::ptr const &data,
                      bool hasAck, message::list &ack_resp);
    void onJoinClipRoom(std::string const &name, message::ptr const &data,
                        bool hasAck, message::list &ack_resp);
    void onUpdateControl(std::string const &name, message::ptr const &data,
                         bool hasAck, message::list &ack_resp);
    void onCamOrderChanger(const string &name, const message::ptr &data,
                           bool hasAck, message::list &ack_resp);
    void onJoined();

    //getclip
    void onGetClipSuccess(std::string const &name, message::ptr const &data,
                          bool hasAck, message::list &ack_resp);

Q_SIGNALS:
    void socketConnected();
    void socketDisconnected();
    void socketJoined();
};

#endif  // ABSTRACTION_H
