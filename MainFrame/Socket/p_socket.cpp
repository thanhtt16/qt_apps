#include "p_socket.h"

/**
     * Generic method to override for updating the presention.
     **/

P_Socket::P_Socket(Control *ctrl, QWidget *zone) : Presentation(ctrl) {
    // init gui object

    this->zone = zone;
    this->centerLayout = new QVBoxLayout();
    this->centerLayout->setSpacing(20);
    this->centerLayout->setAlignment(Qt::AlignCenter);
    this->zone->setLayout(centerLayout);
    this->zone->setStyleSheet("background-color: green");

    this->messageLabel = new QLabel(this->zone);
    this->loaderMovie = new QMovie(":/images/res/loader.gif");
    this->loaderMovie->setSpeed(100);
    this->loaderMovie->start();
    this->messageLabel->setAlignment(Qt::AlignCenter);
    this->messageLabel->setMovie(this->loaderMovie);
    this->centerLayout->addWidget(messageLabel);

    this->textLabel = new QLabel(this->zone);
    this->textLabel->setAlignment(Qt::AlignCenter);
    this->textLabel->setText("Socket Connecting...");
    this->centerLayout->addWidget(textLabel);
}

void P_Socket::update() {}

void P_Socket::showLoading() {

}

void P_Socket::hideLoading(){

}

QObject *P_Socket::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return Q_NULLPTR;
    default:
        return Q_NULLPTR;
    }
}

/**
     * Method to create a new presentation exactly equals (field by field) to
  *this presentation.
     * @return the created clone
     **/
//    public Presentation getClone(){
//        try{
//            return (Presentation)clone();
//        } catch(Exception e) {System.out.println("ERROR: can't duplicate a
//        presentation.");}
//        return null;
//    }
