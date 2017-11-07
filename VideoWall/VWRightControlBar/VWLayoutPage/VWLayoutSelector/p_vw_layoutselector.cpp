#include "p_vw_layoutselector.h"
#include "Common/LayoutSet.h"

P_VWLayoutSelector::P_VWLayoutSelector(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
    zone->setStyleSheet("background-color: #222");

    layoutSet = new LayoutSet(Q_NULLPTR);

    layout = new QHBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(5);
    zone->setLayout(layout);

    for (int index = 0; (index < layoutSet->layoutList.size()) &&
         (layoutSet->layoutList.at(index).numberOfCameras <=
          Message.APP_VIDEO_WALL_MAX_NUMBER_OF_PLAYERS);
         ++index) {
        QPushButton *button = new QPushButton(zone);
        QFont font = button->font();
        font.setPixelSize(38);
        button->setFont(font);
        button->setText(layoutSet->layoutList.at(index).label);
        button->setStyleSheet("background-color: black; color: white;");
        button->setMinimumHeight(button->width());
        buttons.append(button);
        layout->addWidget(button);
        connect(button, &QPushButton::clicked, this,
                &P_VWLayoutSelector::onSelectedLayout);
    }
    waitingTimer = new QTimer();
}

void P_VWLayoutSelector::onSelectedLayout() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button != Q_NULLPTR) {
        int clickedButtonIndex = buttons.indexOf(button);
        if (clickedButtonIndex >= 0 && clickedButtonIndex < buttons.size()) {
            if (selectedButtonIndex != clickedButtonIndex) {
                clearOldSelectedButton(this->selectedButtonIndex);
                selectedButtonIndex = clickedButtonIndex;
                highlightNewSelectedButton(clickedButtonIndex);
            }
        }
        lastButton = button;
        if (!isWaiting) {
            startWaiting();
        } else {
            isWaiting = false;
            waitingTimer->stop();
            disconnect(waitingTimer, &QTimer::timeout, this,
                       &P_VWLayoutSelector::onEndWaiting);
            startWaiting();
        }
    }
}

void P_VWLayoutSelector::startWaiting() {
    if (!isWaiting) {
        isWaiting = true;
        connect(waitingTimer, &QTimer::timeout, this,
                &P_VWLayoutSelector::onEndWaiting);
        waitingTimer->start(500);
    }
}

void P_VWLayoutSelector::onEndWaiting() {
    isWaiting = false;
    if (lastButton != Q_NULLPTR) {
        int indexOfButton = buttons.indexOf(lastButton);
        this->selectedButtonIndex = indexOfButton;
        QVariant *dataStruct = new QVariant();
        dataStruct->setValue<LayoutStruct>(layoutSet->layoutList.at(indexOfButton));
        control()->newUserAction(Message.APP_VIDEO_WALL_ZONE_LAYOUT_SELECTED,
                                 dataStruct);

    }
    waitingTimer->stop();
    disconnect(waitingTimer, &QTimer::timeout, this,
               &P_VWLayoutSelector::onEndWaiting);
}

void P_VWLayoutSelector::clearOldSelectedButton(int buttonIndex) {
    if (buttonIndex >= 0 && buttonIndex < buttons.size()) {
        buttons.at(buttonIndex)
                ->setStyleSheet("background-color: black; color: white;");
    }
}

void P_VWLayoutSelector::highlightNewSelectedButton(int buttonIndex) {
    if (buttonIndex >= 0 && buttonIndex < buttons.size()) {
        buttons.at(buttonIndex)
                ->setStyleSheet("background-color: red; color: white;");
    }
}

void P_VWLayoutSelector::calibrateLayoutSet() {
    CamSite *camSite = control()->appContext->getSiteCameras();
    if (camSite != Q_NULLPTR) {
        int numberOfCamerasOfSite = camSite->getTotalCamItem();
        for (int index = 1; (index < layoutSet->layoutList.size()) &&
             (layoutSet->layoutList.at(index).numberOfCameras <=
              Message.APP_VIDEO_WALL_MAX_NUMBER_OF_PLAYERS);
             ++index) {
            if (layoutSet->layoutList.at(index - 1).numberOfCameras <
                    numberOfCamerasOfSite) {
                buttons.at(index)->setEnabled(true);
                buttons.at(index)->setStyleSheet(
                            "background-color: black; color: white;");
            } else {
                buttons.at(index)->setDisabled(true);
                buttons.at(index)->setStyleSheet(
                            "background-color: black; color: #444;");
            }
        }
    }
}

void P_VWLayoutSelector::setDefaultLayout() {
    qDebug() << Q_FUNC_INFO;
    clearOldSelectedButton(this->selectedButtonIndex);

    calibrateLayoutSet();

    int selectedLayoutSaved = -1;
    int defaultLayoutIndex = 1;

    if (!checkLayoutSaved){
        checkLayoutSaved = true;
        QSettings settings;
        settings.beginGroup(QString::number(control()->appContext->getWorkingUser()->getUserId()));
        settings.beginGroup("videowall");
        selectedLayoutSaved = settings.value("selected_layout").toInt();
    }

    if(selectedLayoutSaved >= 0 && defaultLayoutIndex <= 4 && checkLayoutSaved == true){
        defaultLayoutIndex = selectedLayoutSaved;
    }

    highlightNewSelectedButton(defaultLayoutIndex);
    this->selectedButtonIndex = defaultLayoutIndex;

    QVariant *data = new QVariant();
    data->setValue<LayoutStruct>(layoutSet->layoutList.at(defaultLayoutIndex));
    control()->newUserAction(Message.APP_VIDEO_WALL_PAGE_DEFAULT_SET, data);

}

void P_VWLayoutSelector::show(QVariant *attachment) { Q_UNUSED(attachment) }

void P_VWLayoutSelector::update() {}

QObject *P_VWLayoutSelector::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return Q_NULLPTR;
    default:
        return Q_NULLPTR;
    }
}
