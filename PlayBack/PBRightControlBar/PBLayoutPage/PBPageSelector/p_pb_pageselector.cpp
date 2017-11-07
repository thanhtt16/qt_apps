#include "p_pb_pageselector.h"

int P_PBPageSelector::getNumberOfPages() const { return numberOfPages; }

void P_PBPageSelector::setNumberOfPages(int value) { numberOfPages = value; }

int P_PBPageSelector::getSelectedPage() const { return selectedPage; }

void P_PBPageSelector::setSelectedPage(int value) { selectedPage = value; }

P_PBPageSelector::P_PBPageSelector(Control *ctrl, QWidget *zone)
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


}
void P_PBPageSelector::show(QVariant *attachment) { Q_UNUSED(attachment) }

void P_PBPageSelector::update(LayoutStruct selectedLayout) {
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
    dataStruct->setValue(lastSelectedLayout);
    control()->newUserAction(Message.APP_PLAY_BACK_ZONE_PAGE_SELECTED,
                             dataStruct);
}

void P_PBPageSelector::updateDefaultPage(LayoutStruct defaultLayout) {
    this->lastSelectedLayout = defaultLayout;
    updateNumberOfPages(defaultLayout.numberOfCameras);
    clearPages();
    displayPages();

    // send the selected layout/page to workspace agent
    // re-calculate default selected page
    displaySelectedPage();
    lastSelectedLayout.selectedPage = this->selectedPage;
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<LayoutStruct>(this->lastSelectedLayout);
    control()->newUserAction(Message.APP_PLAY_BACK_UPDATE_PAGE_DEFAULT,
                             dataStruct);
}

void P_PBPageSelector::clearPages() {
    while (buttons.size() > 0) {
        QPushButton *button = buttons.last();
        button->hide();
        pageLayout->removeWidget(button);
        buttons.removeLast();
        button->deleteLater();
    }
}

void P_PBPageSelector::displayPages() {
    buttons.clear();
    //  QList<CamItem *> listCamItems =
    //      control()->appContext->getListCamItemsOfSite();
    for (int index = 0; index < this->numberOfPages; ++index) {
        QPushButton *button = new QPushButton(this->zone);
        button->setText(QString("%1").arg(index + 1));

        //    if (this->lastSelectedLayout.numberOfCameras == 1) {
        //      if (index < listCamItems.size()) {
        //        CamItem *camItem = listCamItems.at(index);
        //        button->setToolTip(camItem->getPostion());
        //      }
        //    }

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
                &P_PBPageSelector::onPageSelected);
    }
}

void P_PBPageSelector::displaySelectedPage() {
    for (int index = 0; index < buttons.size(); ++index) {
        QPushButton *button = buttons.at(index);
        if (selectedPage == index + 1) {
            button->setStyleSheet("background-color: red; color: white;");
        } else {
            button->setStyleSheet("background-color: white; color: black;");
        }
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

void P_PBPageSelector::onPageSelected() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button != Q_NULLPTR) {
        int index = buttons.indexOf(button);
        if (index >= 0) {
            this->selectedPage = index + 1;
            displaySelectedPage();

            this->lastSelectedLayout.selectedPage = this->selectedPage;
            QVariant *dataStruct = new QVariant();
            dataStruct->setValue(this->lastSelectedLayout);
            control()->newUserAction(Message.APP_PLAY_BACK_ZONE_PAGE_SELECTED,
                                     dataStruct);
        }
    }
}

QObject *P_PBPageSelector::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return Q_NULLPTR;
    default:
        return Q_NULLPTR;
    }
}

void P_PBPageSelector::updateNumberOfPages(int numberOfCamerasPerPage) {
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
            settings.beginGroup("playback");
            selectedPageSaved = settings.value("selected_page").toInt();
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


void P_PBPageSelector::enabledPageButton() {
    qDebug() << Q_FUNC_INFO;

    for (int index = 0; index < buttons.size(); ++index) {
        buttons.at(index)->setEnabled(true);
        buttons.at(index)->setStyleSheet(
                    ("background-color: black; color: white;"));
    }
    displaySelectedPage();
}
void P_PBPageSelector::disabledPageButton() {
    qDebug() << Q_FUNC_INFO;

    for (int index = 0; index < buttons.size(); ++index) {
        buttons.at(index)->setDisabled(true);
        buttons.at(index)->setStyleSheet("background-color: black; color: #444;");
    }
}
