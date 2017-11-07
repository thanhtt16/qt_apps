#ifndef RESOURCES_H
#define RESOURCES_H

#include <QObject>
#include <QApplication>
#include <QDebug>
#include <QFont>
#include <QDesktopWidget>
#include <QtAwesome.h>

class Resources
{
    public:
        
        static Resources &instance() {
            static Resources mInstance;
            return mInstance;
        }
        
        QtAwesome* getAwesome() const;
        QSize getScreenSize();
        QVariantMap getIconOptions();
        
        Resources(Resources const&) = delete;
        Resources(Resources &&) = delete;
        Resources& operator = (Resources const&) = delete;
        Resources& operator = (Resources &&) = delete;
        
        QFont getExtraSmallRegularButtonFont();
        QFont getExtraSmallBoldButtonFont();
        
        QFont getSmallRegularButtonFont();
        QFont getSmallBoldButtonFont();
        
        QFont getMediumRegularButtonFont();
        QFont getMediumBoldButtonFont();
        
        QFont getLargeRegularButtonFont();
        QFont getLargeBoldButtonFont();
        
        QFont getExtraLargeRegularButtonFont();
        QFont getExtraLargeBoldButtonFont();
        
    private:
        QtAwesome *mAwesome;
        QSize mScreenSize;
        QVariantMap mIconOptions;
        QFont mExtraLargeRegularButtonFont;
        QFont mExtraLargeBoldButtonFont;
        QFont mLargeRegularButtonFont;
        QFont mLargeBoldButtonFont;
        QFont mMediumRegularButtonFont;
        QFont mMediumBoldButtonFont;
        QFont mSmallRegularButtonFont;
        QFont mSmallBoldButtonFont;
        QFont mExtraSmallRegularButtonFont;
        QFont mExtraSmallBoldButtonFont;

    protected:
        Resources();
        
};

#endif // RESOURCES_H
