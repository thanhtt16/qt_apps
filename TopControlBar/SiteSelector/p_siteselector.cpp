#include "p_siteselector.h"

/**
     * Generic method to override for updating the presention.
     **/

int P_SiteSelector::countInit = 0;
P_SiteSelector::P_SiteSelector(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
    this->zone = zone;
    this->zone->hide();
    this->zone->setFixedSize(600, 400);
    mSearchTimer = new QTimer(this);
    // init gui object

    this->zone->setStyleSheet(
                "background-color: #FFFFFF; color: black; border:0px;");

    this->zone->setAttribute(Qt::WA_Hover, true);

    QVBoxLayout *mSelectionLayout = new QVBoxLayout();
    mSelectionLayout->setMargin(10);
    mSelectionLayout->setSpacing(20);
    mSelectionLayout->setAlignment(Qt::AlignCenter);
    this->zone->setLayout(mSelectionLayout);

    QWidget *mSearchWidget = new QWidget(this->zone);
    QVBoxLayout *mSearchLayout = new QVBoxLayout();
    mSearchWidget->setLayout(mSearchLayout);
    mSelectionLayout->addWidget(mSearchWidget);
    mSearchWidget->setStyleSheet("background:#EFEFEF");

    mSearchLineEdit = new QLineEdit(mSearchWidget);
    mSearchLineEdit->setMaximumHeight(30);
    mSearchLineEdit->setMinimumHeight(30);

    mSearchLineEdit->setStyleSheet("background : #ffffff;color:#4f4f4f");
    mSearchLineEdit->setTextMargins(5, 2, 5, 2);
    mSearchLineEdit->setPlaceholderText("Tìm kiếm...");
    mSearchLayout->addWidget(mSearchLineEdit);

    mSiteListView = new QListView(this->zone);
    mSiteListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mSiteListView->setStyleSheet("background-color: white; color: black;");
    mSiteListView->setStyleSheet(
                "QListView::item:hover{background-color:#9bdcd0}");
    mSiteListView->setSpacing(5);

    mSiteListView->setFont(Resources::instance().getLargeRegularButtonFont());
    displaySites();
    mSelectionLayout->addWidget(mSiteListView);
    mSiteListView->setMouseTracking(true);
    mSiteListView->setAttribute(Qt::WA_Hover);
    mSiteListView->installEventFilter(this);

    connect(mSearchLineEdit, &QLineEdit::editingFinished, this,
            &P_SiteSelector::filterWithKeyword);
    connect(mSearchLineEdit, &QLineEdit::textChanged, this,
            &P_SiteSelector::resetDisplay);
    connect(mSiteListView, &QListView::clicked, this,
            &P_SiteSelector::onSiteSelected);
}
void P_SiteSelector::resetDisplay() {
    if (mSearchLineEdit->text() == "") displaySites();
}

void P_SiteSelector::filterWithKeyword() {}

void P_SiteSelector::onSearch() {
    if (!mIsSearching) {
        mCurrentKeyword = mSearchLineEdit->text();
        mCurrentKeyword = mCurrentKeyword.toLower();

        if (mLastKeyword != mCurrentKeyword) {
            mIsSearching = true;
            mLastKeyword = mCurrentKeyword;
            mFilteredSitesMinimalHash.clear();
            mFilteredSitesOriginalHash.clear();
            QStringList tokens = mCurrentKeyword.trimmed().split(" ");
            for (int i = 0; i < keyMapIdSite.size(); ++i) {
                int key = keyMapIdSite.at(i);
                QString title = mCurrentSitesOriginalHash.value(key);
                QString compareTitle = mCurrentSitesMinimalHash.value(key);
                bool found = true;
                for (int index = 0; index < tokens.size(); ++index) {
                    QString token = tokens.at(index);
                    QString compareToken = strings.removeVietnamese(token.toLower());
                    if (!compareTitle.contains(compareToken)) {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    mFilteredSitesOriginalHash.insert(key, title);
                    mFilteredSitesMinimalHash.insert(key, compareTitle);
                }
            }
            QStringListModel *listModel = new QStringListModel();
            listModel->setStringList(mFilteredSitesOriginalHash.values());
            mSiteListView->setModel(listModel);
            mIsSearching = false;
        }
    }
    //  if (mSearchLineEdit->text() == "") displaySites();
}

void P_SiteSelector::update() {}

void P_SiteSelector::show() {
    if (control()->zone->isVisible()) {
        control()->zone->hide();
        stopSearch();
    } else {
        control()->zone->show();
        control()->zone->raise();
        startSearch();
    }
}
void P_SiteSelector::sizeTopControlBar(QVariant *dataStruct) {
    dataSizeTop = dataStruct->value<SizeTopControlBar>();
    int widthMove = dataSizeTop.widthBrand + 45;
    this->zone->move(widthMove, 40);
}
void P_SiteSelector::hide() { control()->zone->hide(); }

void P_SiteSelector::newSiteSelected(Site *newSite) {
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue(newSite);
    control()->newUserAction(Message.SITE_NEW_SELECTED, dataStruct);
    control()->appContext->setWorkingSite(newSite);
}

void P_SiteSelector::onSiteSelected() {
    QModelIndexList selectedRows =
            mSiteListView->selectionModel()->selectedRows();
    int row, key;
    if (selectedRows.size() > 0) {
        if (mFilteredSitesOriginalHash.size() > 0) {
            row = mSiteListView->selectionModel()->selectedRows().first().row();
            key = mFilteredSitesOriginalHash.keys().at(row);
            QString value = mFilteredSitesOriginalHash.values().at(row);
        } else {
            row = mSiteListView->selectionModel()->selectedRows().first().row();
            key = mCurrentSitesOriginalHash.keys().at(row);
            QString value = mCurrentSitesOriginalHash.values().at(row);
        }
    }
    Site *selectedSite = listSitesMap.value(key);
    Site *workingSite = control()->appContext->getWorkingSite();
    if (selectedSite->getSiteId() == workingSite->getSiteId())
        this->hide();
    else {
        this->newSiteSelected(selectedSite);
        this->hide();
    }
    mSearchLineEdit->setText("");
}

void P_SiteSelector::displaySites() {
    mCurrentSitesOriginalHash.clear();
    mCurrentSitesMinimalHash.clear();
    listSitesName.clear();
    keyMapIdSite.clear();

    this->userSites = control()->appContext->getUserSites()->getListSite();
    QString iconSite = "";
    for (int index = 0; index < userSites.size(); ++index) {
        Site *site = userSites.at(index);
        int siteId = site->getSiteId();
        QString siteName = iconSite + "   " + site->getSiteName();
        listSitesName.append(iconSite + "   " + site->getSiteName());
        mCurrentSitesOriginalHash.insert(siteId, siteName);
        QString siteNameMinimal = strings.removeVietnamese(siteName);
        mCurrentSitesMinimalHash.insert(siteId, siteNameMinimal.toLower());
        keyMapIdSite.append(siteId);
        listSitesMap.insert(siteId, site);
    }
    QStringListModel *listModel = new QStringListModel();
    listModel->setStringList(mCurrentSitesOriginalHash.values());
    mSiteListView->setModel(listModel);
}

QWidget *P_SiteSelector::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return Q_NULLPTR;
    default:
        return Q_NULLPTR;
    }
}

bool P_SiteSelector::eventFilter(QObject *watched, QEvent *event) {
    QListView *listview = qobject_cast<QListView *>(watched);
    if (listview == mSiteListView) {
        switch (event->type()) {
        case QEvent::HoverEnter:
            hoverEnter(static_cast<QHoverEvent *>(event));
            return true;
            break;
        case QEvent::HoverLeave:
            hoverLeave(static_cast<QHoverEvent *>(event));
            return true;
            break;
        case QEvent::HoverMove:
            hoverMove(static_cast<QHoverEvent *>(event));
            return true;
            break;
        default:
            break;
        }
    }
    return false;
}

void P_SiteSelector::hoverEnter(QHoverEvent *) {}

void P_SiteSelector::hoverLeave(QHoverEvent *) {
    control()->newUserAction(Message.SITE_SELECTOR_HIDE_ALL, Q_NULLPTR);

    control()->zone->hide();
}

void P_SiteSelector::hoverMove(QHoverEvent *) {}

void P_SiteSelector::startSearch() {
    if (!mSearchTimer->isActive()) {
        connect(mSearchTimer, &QTimer::timeout, this, &P_SiteSelector::onSearch);
        mSearchTimer->start(300);
    }
}

void P_SiteSelector::stopSearch() {
    if (mSearchTimer->isActive()) {
        disconnect(mSearchTimer, &QTimer::timeout, this, &P_SiteSelector::onSearch);
        mSearchTimer->stop();
    }
}
