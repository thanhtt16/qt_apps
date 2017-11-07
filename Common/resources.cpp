
#include "resources.h"

Resources::Resources()
{
    mAwesome = new QtAwesome(qApp);
    mAwesome->initFontAwesome();

    mScreenSize = QApplication::desktop()->screenGeometry().size();
    
    mIconOptions.insert("color", QColor(255, 255, 255));
    mIconOptions.insert("color-disabled", QColor(255, 255, 255));
    mIconOptions.insert("color-active", QColor(255, 255, 255));
    mIconOptions.insert("color-selected", QColor(255, 255, 255));
    
    mExtraLargeRegularButtonFont = mAwesome->font(17);
    mExtraLargeBoldButtonFont = mAwesome->font(17);
    mExtraLargeBoldButtonFont.setBold(true);
    
    mLargeRegularButtonFont = mAwesome->font(15);
    mLargeBoldButtonFont = mAwesome->font(15);
    mLargeBoldButtonFont.setBold(true);
    
    mMediumRegularButtonFont = mAwesome->font(13);
    mMediumBoldButtonFont = mAwesome->font(13);
    mMediumBoldButtonFont.setBold(true);
    
    mSmallRegularButtonFont = mAwesome->font(11);
    mSmallBoldButtonFont = mAwesome->font(11);
    mSmallBoldButtonFont.setBold(true);
    
}

QVariantMap Resources::getIconOptions() {
    return mIconOptions;
}


QtAwesome* Resources::getAwesome() const {
    return mAwesome;
}


QSize Resources::getScreenSize() {
    return mScreenSize;
}

QFont Resources::getExtraLargeBoldButtonFont() {
    return mExtraLargeBoldButtonFont;
}

QFont Resources::getExtraLargeRegularButtonFont() {
    return mExtraLargeRegularButtonFont;
}

QFont Resources::getLargeBoldButtonFont() {
    return mLargeBoldButtonFont;
}

QFont Resources::getLargeRegularButtonFont() {
    return mLargeRegularButtonFont;
}

QFont Resources::getMediumBoldButtonFont() {
    return mMediumBoldButtonFont;
}

QFont Resources::getMediumRegularButtonFont() {
    return mMediumRegularButtonFont;
}

QFont Resources::getSmallBoldButtonFont() {
    return mSmallBoldButtonFont;
}

QFont Resources::getSmallRegularButtonFont() {
    return mSmallRegularButtonFont;
}

QFont Resources::getExtraSmallBoldButtonFont() {
    return mExtraSmallBoldButtonFont;
}

QFont Resources::getExtraSmallRegularButtonFont() {
    return mExtraSmallRegularButtonFont;
}


