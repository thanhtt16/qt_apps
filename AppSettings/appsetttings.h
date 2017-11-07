#ifndef APPSETTTINGS_H
#define APPSETTTINGS_H

#include <QObject>
#include <QSettings>
#include <Common/appprofile.h>
class AppSetttings: public QObject
{
    Q_OBJECT
public:
    static AppSetttings &instance() {
      static AppSetttings mInstance;
      return mInstance;
    }
    void createSettingsDefaults(int userId);

    AppSetttings();
    AppSetttings(AppSetttings const &) = delete;
    AppSetttings(AppSetttings &&) = delete;
    AppSetttings &operator=(AppSetttings const &) = delete;
    AppSetttings &operator=(AppSetttings &&) = delete;

};

#endif // APPSETTTINGS_H
