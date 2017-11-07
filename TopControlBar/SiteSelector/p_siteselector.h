#ifndef P_SITESELECTOR_H
#define P_SITESELECTOR_H

#include <PacModel/presentation.h>
#include <QAbstractItemView>
#include <QEvent>
#include <QFont>
#include <QHoverEvent>
#include <QListView>
#include <QObject>
#include <QStringListModel>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include "Common/generaldefine.h"
#include "Site/sitechild.h"
#include "c_siteselector.h"
class C_SiteSelector;

static struct Strings {
    QStringList vietnamese() {
        QStringList list;
        list << "aAeEoOuUiIdDyY";
        list << "áàạảãâấầậẩẫăắằặẳẵ";
        list << "ÁÀẠẢÃÂẤẦẬẨẪĂẮẰẶẲẴ";
        list << "éèẹẻẽêếềệểễ";
        list << "ÉÈẸẺẼÊẾỀỆỂỄ";
        list << "óòọỏõôốồộổỗơớờợởỡ";
        list << "ÓÒỌỎÕÔỐỒỘỔỖƠỚỜỢỞỠ";
        list << "úùụủũưứừựửữ";
        list << "ÚÙỤỦŨƯỨỪỰỬỮ";
        list << "íìịỉĩ";
        list << "ÍÌỊỈĨ";
        list << "đ";
        list << "Đ";
        list << "ýỳỵỷỹ";
        list << "ÝỲỴỶỸ";
        return list;
    }

    QString clean(QString str) {
        str = str.remove("\"");
        str = str.remove("\\");
        str = str.remove(".");
        str = str.remove(",");
        str = str.remove("?");
        str = str.remove("!");
        str = str.remove("'");
        str = str.remove("`");
        str = str.remove("-");
        str = str.remove("_");
        str = str.remove("—");
        str = str.remove("@");
        str = str.remove("#");
        str = str.remove("$");
        str = str.remove("%");
        str = str.remove("^");
        str = str.remove("&");
        str = str.remove("*");
        str = str.remove("(");
        str = str.remove(")");
        str = str.remove("{");
        str = str.remove("}");
        str = str.remove("[");
        str = str.remove("]");
        str = str.remove("|");
        str = str.remove(";");
        str = str.remove(":");
        str = str.remove("/");
        str = str.remove(">");
        str = str.remove("<");
        str = str.remove("~");
        str = str.remove("=");
        str = str.remove("+");
        str = str.remove(str.fromUtf8("«"));
        str = str.remove(str.fromUtf8("»"));
        str = str.trimmed();
        return str;
    }

    QString removeVietnamese(QString vn) {
        vn = clean(vn);
        QStringList vns = vietnamese();
        QString ens = vns.at(0);
        for (int i = 1; i < vn.size(); i++) {
            if (i < vns.size()) {
                QString vnss = vns.at(i);
                for (int j = 0; j < vnss.size(); j++)
                    vn = vn.replace(vnss[j], ens[i - 1]);
            }
        }
        return vn;
    }
} strings;

class P_SiteSelector : public Presentation {
    // init ui control
private:
public:
    static int countInit;

    QWidget *zone;
    SizeTopControlBar dataSizeTop;
    bool isTimeRun = false;
    QString mCurrentKeyword;
    QStringList mListSearch;
    QTimer *mSearchTimer;
    QListView *mSiteListView;
    SiteChild *siteChild;
    C_SiteSelector *control() { return (C_SiteSelector *)this->ctrl; }
    P_SiteSelector(Control *ctrl, QWidget *zone);
    void changeControl(Control *ctrl);
    void update();

    QString mLastKeyword;
    int mMaxLengthKeyword = 2;
    QWidget *getZone(int zoneId);
    void newSiteSelected(Site *newSite);
    void show();
    void hide();
    void onSiteSelected();
    void displaySites();
    QList<Site *> userSites;

    QLineEdit *mSearchLineEdit;
    QStringList listSitesName;

    QHash<int, QString> mCurrentSitesOriginalHash;
    QHash<int, QString> mCurrentSitesMinimalHash;
    QHash<int, QString> mFilteredSitesOriginalHash;
    QHash<int, QString> mFilteredSitesMinimalHash;
    QList<int> keyMapIdSite;
    QHash<int, Site *> listSitesMap;
    bool mIsSearching = false;
    void sizeTopControlBar(QVariant *dataStruct);

    QStringListModel *listModel = Q_NULLPTR;
    QStringList list;

protected:
    void hoverEnter(QHoverEvent *event);
    void hoverLeave(QHoverEvent *event);
    void hoverMove(QHoverEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
private Q_SLOTS:
    void filterWithKeyword();
    void onSearch();
    void resetDisplay();
    void startSearch();
    void stopSearch();
};

#endif  // P_SITESELECTOR_H
