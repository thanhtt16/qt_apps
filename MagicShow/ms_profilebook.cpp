#include "ms_profilebook.h"

class QDir;

ProfileBook::ProfileBook(QObject *parent) : QObject(parent)
{
    
}

QList<ProfileBook*>* ProfileBook::loadProfileBookOf(int userId, int siteId) {
    QList<ProfileBook*> *profileBooks = new QList<ProfileBook*>();
    QString folder = QDir::homePath();
    folder = folder.append("/.videowall/profiles").append("/").append(QString::number(userId));
//    qDebug() << folder;
    if (!QDir(folder).exists()) {
        QDir().mkpath(folder);
    }
    
    QString path = folder + QString("/%1.json").arg(siteId);
//    qDebug() << "LOADING_FILES_FROM" << path;
    QFile jsonFile(path);
    if (jsonFile.exists()) {
        jsonFile.open(QFile::ReadOnly);
        QString jsonString = QTextStream(&jsonFile).readAll();
        jsonFile.close();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonArray jsonArray = jsonDocument.array();
        for (int index = 0; index < jsonArray.size(); ++index) {
            QJsonObject jsonObject = jsonArray[index].toObject();
//            qDebug() << "OBJECT" << jsonObject;
            ProfileBook *profileBook = ProfileBook::parseFrom(jsonObject);
            profileBooks->append(profileBook);
        }
//        qDebug() << "LOADED_PROFILES" << profileBooks->size();
    }
    return profileBooks;
}

ProfileBook* ProfileBook::parseFrom(QJsonObject jsonObject) {
    ProfileBook *profileBook = new ProfileBook(NULL);
    QJsonValue jsonValue;
    jsonValue = jsonObject.take("name");
    if (!jsonValue.isNull()) {
        profileBook->mName = jsonValue.toString();
    }
    jsonValue = jsonObject.take("user");
    if (!jsonValue.isNull()) {
        profileBook->mUserId = jsonValue.toInt();
    }
    jsonValue = jsonObject.take("site");
    if (!jsonValue.isNull()) {
        profileBook->mSiteId = jsonValue.toInt();
    }
    jsonValue = jsonObject.take("page");
    if (!jsonValue.isNull()) {
        profileBook->mCurrentPage = jsonValue.toInt();
    }
    QList<ProfilePage*> profilePages;
    QJsonArray jsonArray = jsonObject.take("pages").toArray();
    for (int index = 0; index < jsonArray.size(); ++index) {
        QJsonObject pageJsonObject = jsonArray[index].toObject();
        ProfilePage *profilePage = ProfilePage::parseFrom(pageJsonObject);
        profilePages.append(profilePage);
    }
    profileBook->mProfilePages = profilePages;
    return profileBook;
}

QJsonObject ProfileBook::toJsonObject() {
    QJsonObject jsonObject;
    jsonObject.insert("name", mName);
    jsonObject.insert("user", mSiteId);
    jsonObject.insert("site", mSiteId);
    jsonObject.insert("page", mCurrentPage);
    QJsonArray jsonArray;
    for (int index = 0; index < mProfilePages.size(); ++index) {
        QJsonObject jsObj = mProfilePages.at(index)->toJsonObject();
        jsonArray.append(jsObj);
    }
    jsonObject.insert("pages", jsonArray);
    return jsonObject;
}

void ProfileBook::saveProfileBookTo(QList<ProfileBook *> *profileBooks, int userId, int siteId) {
    QString folder = QDir::homePath();
//    QString folder = "/home/duongnartist/Desktop";
    folder = folder.append("/.videowall/profiles").append("/").append(QString::number(userId));

    if (!QDir(folder).exists()) {
        QDir().mkpath(folder);
    }

    QJsonArray jsonArray;
    for (int index = 0; index < profileBooks->size(); ++index) {
        ProfileBook *profileBook = profileBooks->at(index);
        jsonArray.append(profileBook->toJsonObject());
    }
    
    QString path = folder + QString("/%1.json").arg(siteId);
//    qDebug() << "SAVE_ALL_PROFILES" << profileBooks->size() << "TO" << path;
    
    QFile file(path);
    file.open(QFile::ReadWrite | QFile::Truncate);
    QString jsonString = QString(QJsonDocument(jsonArray).toJson(QJsonDocument::Compact));
    QTextStream(&file) << jsonString;
    file.close();
}

QList<CamSite*>* ProfileBook::getCamSites(CamSite *sourceCamSite) {
    QList<CamSite*> *camSites = new QList<CamSite*>();
    for (int index = 0; index < mProfilePages.size(); ++index) {
        ProfilePage *profilePage = mProfilePages.at(index);
        CamSite *camSite = profilePage->toCamSite(sourceCamSite);
        camSites->append(camSite);
    }
    return camSites;
}

QString ProfileBook::getName() const {
    return mName;
}

void ProfileBook::setName(QString name) {
    mName = name;
}

int ProfileBook::getSiteId() const {
    return mSiteId;
}

void ProfileBook::setSiteId(int siteId) {
    mSiteId = siteId;
}

int ProfileBook::getUserId() const {
    return mUserId;
}

void ProfileBook::setUserId(int userId) {
    mUserId = userId;
}

int ProfileBook::getCurrentPage() const {
    return mCurrentPage;
}

void ProfileBook::setCurrentPage(int currentPage) {
    mCurrentPage = currentPage;
}

QList<ProfilePage*> ProfileBook::getProfilePages() {
    return mProfilePages;
}

void ProfileBook::setProfilePages(QList<ProfilePage*> profilePages) {
    this->mProfilePages = profilePages;
}
