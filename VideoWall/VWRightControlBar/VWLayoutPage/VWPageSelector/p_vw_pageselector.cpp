#include "p_vw_pageselector.h"

int P_VWPageSelector::getNumberOfPages() const { return numberOfPages; }

void P_VWPageSelector::setNumberOfPages(int value) { numberOfPages = value; }

int P_VWPageSelector::getSelectedPage() const { return selectedPage; }

void P_VWPageSelector::setSelectedPage(int value) { selectedPage = value; }

P_VWPageSelector::P_VWPageSelector(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
    this->zone = zone;
    this->zone->setStyleSheet("background-color: #222");

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(4);
    mainLayout->setAlignment(Qt::AlignHCenter);
    this->zone->setLayout(mainLayout);

    QScrollArea *scrollArea = new QScrollArea(this->zone);
    scrollArea->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(scrollArea);

    pageWidget = new QWidget(scrollArea);
    scrollArea->setWidget(pageWidget);
    this->pageLayout = new QGridLayout();
    pageLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    pageLayout->setMargin(0);
    pageLayout->setSpacing(2);
    pageWidget->setLayout(this->pageLayout);

    waitingTimer = new QTimer();

}
void P_VWPageSelector::show(QVariant *attachment) { Q_UNUSED(attachment) }

void P_VWPageSelector::update(LayoutStruct selectedLayout) {
    this->lastSelectedLayout = selectedLayout;
    updateNumberOfPages(selectedLayout.numberOfCameras);
    clearPages();
    displayPages();

    // send the selected layout/page to workspace agent
    // re-calculate default selected page
    displaySelectedPage();
    lastSelectedLayout.selectedPage = this->selectedPage;
    // update
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<LayoutStruct>(lastSelectedLayout);
    control()->newUserAction(Message.APP_VIDEO_WALL_ZONE_PAGE_SELECTED,
                             dataStruct);
}


void P_VWPageSelector::updateDefaulPage(LayoutStruct layoutDefault) {
    this->lastSelectedLayout = layoutDefault;
    updateNumberOfPages(layoutDefault.numberOfCameras);
    clearPages();
    displayPages();
    // send the selected layout/page to workspace agent
    // re-calculate default selected page
    displaySelectedPage();
    lastSelectedLayout.selectedPage = this->selectedPage;
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<LayoutStruct>(this->lastSelectedLayout);
    control()->newUserAction(Message.APP_VIDEO_WALL_UPDATE_PAGE_DEFAULT,
                             dataStruct);
}

void P_VWPageSelector::clearPages() {
    while (buttons.size() > 0) {
        QPushButton *button = buttons.last();
        button->hide();
        pageLayout->removeWidget(button);
        buttons.removeLast();
        button->deleteLater();
    }
}

void P_VWPageSelector::displayPages() {
    buttons.clear();
    for (int index = 0; index < this->numberOfPages; ++index) {
        QPushButton *button = new QPushButton(pageWidget);
        button->setText(QString("%1").arg(index + 1));
        button->setStyleSheet(
                    "font-size: 18; background-color: white; color:#222; "
                    "border:1px solid #333; "
                    "padding: 5px "
                    "0px;");
        button->setFixedSize(60, 25);

        buttons.append(button);
        int r = index / col;
        int c = index - r * col;
        this->pageLayout->addWidget(button, r, c);
        connect(button, &QPushButton::clicked, this,
                &P_VWPageSelector::onPageSelected);
    }
    if (!buttons.isEmpty()) {
        int rows =
                this->numberOfPages / col + (this->numberOfPages % col > 0 ? 1 : 0);
        pageWidget->setFixedSize(appSize.rightWidth - 50,
                                 (rows) * (buttons.first()->height() + 5));
    } else {
        pageWidget->setFixedSize(appSize.rightWidth - 50, 0);
    }
}

void P_VWPageSelector::displaySelectedPage() {
    for (int index = 0; index < buttons.size(); ++index) {
        QPushButton *button = buttons.at(index);
        if (selectedPage == index + 1) {
            button->setStyleSheet("background-color: red; color: white");
        } else {
            button->setStyleSheet("background-color: white; color: black");
        }
    }
}

void P_VWPageSelector::onPageSelected() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button != Q_NULLPTR) {
        int index = buttons.indexOf(button);
        if (index >= 0) {
            this->selectedPage = index + 1;
            displaySelectedPage();
        }
        lastButton = button;

        if (!isWaiting) {
            startWaiting();
        } else {
            isWaiting = false;
            waitingTimer->stop();
            disconnect(waitingTimer, &QTimer::timeout, this,
                       &P_VWPageSelector::onEndWaiting);
            startWaiting();
        }
    }
    lastButton = button;

    if (!isWaiting) {
        isWaiting = true;
        connect(waitingTimer, &QTimer::timeout, this,
                &P_VWPageSelector::onEndWaiting);
        waitingTimer->start(0);
    }
}

void P_VWPageSelector::startWaiting() {
    if (!isWaiting) {
        isWaiting = true;
        connect(waitingTimer, &QTimer::timeout, this,
                &P_VWPageSelector::onEndWaiting);
        waitingTimer->start(0);
    }
}

void P_VWPageSelector::onEndWaiting() {
    isWaiting = false;
    if (lastButton != Q_NULLPTR) {
        int indexOfButton = buttons.indexOf(lastButton);
        this->selectedPage = indexOfButton + 1;
        this->lastSelectedLayout.selectedPage = this->selectedPage;
        QVariant *dataStruct = new QVariant();
        dataStruct->setValue(this->lastSelectedLayout);
        control()->newUserAction(Message.APP_VIDEO_WALL_ZONE_PAGE_SELECTED,
                                 dataStruct);
        lastButton = Q_NULLPTR;
    }
    waitingTimer->stop();
    disconnect(waitingTimer, &QTimer::timeout, this,
               &P_VWPageSelector::onEndWaiting);
}

QObject *P_VWPageSelector::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return Q_NULLPTR;
    default:
        return Q_NULLPTR;
    }
}

void P_VWPageSelector::updateNumberOfPages(int numberOfCamerasPerPage) {
    CamSite *camSite = control()->appContext->getSiteCameras();
    if (camSite != Q_NULLPTR) {
        int numberOfCameras = camSite->getTotalCamItem();

        int totalPage = numberOfCameras / numberOfCamerasPerPage;
        if (numberOfCameras % numberOfCamerasPerPage != 0) {
            totalPage += 1;
        }
        int selectedPageSaved = -1;
        //chi load page saved ban dau
        if(!checkPageSaved){
            checkPageSaved = true;
            QSettings settings;
            settings.beginGroup(QString::number(control()->appContext->getWorkingUser()->getUserId()));
            settings.beginGroup("videowall");
            selectedPageSaved = settings.value("selected_page").toInt();
            qDebug() << "selectedPageSaved" << selectedPageSaved;
        }

        this->numberOfPages = totalPage;

        if (this->numberOfPages > 0){
            this->selectedPage = 1;
            if (selectedPageSaved > 0 && selectedPageSaved <= this->numberOfPages && checkPageSaved == true){
                this->selectedPage = selectedPageSaved;
            }
        }
    }
    else{
        this->selectedPage = 0;
    }
}

void P_VWPageSelector::pageTransitionBegin() {
    qDebug() << Q_FUNC_INFO;
    if (this->selectedPage >= this->numberOfPages) {
        this->selectedPage = 0;
    }
    this->selectedPage = this->selectedPage + 1;

    displaySelectedPage();
    this->lastSelectedLayout.selectedPage = this->selectedPage;
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue(this->lastSelectedLayout);
    control()->newUserAction(Message.APP_VIDEO_WALL_ZONE_PAGE_SELECTED,
                             dataStruct);
}
