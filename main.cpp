#include <QApplication>
#include <QCoreApplication>
#include <QDateTime>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include "MainFrame/c_mainframe.h"

void killApplication() {
    qApp->exit();
    QProcess process;
    process.execute(
                QString("kill %1").arg(QApplication::QCoreApplication::applicationPid()));
}

void showMessageNotFindAppConfig() {
    QMessageBox messageBox;
    messageBox.setButtonText(1, "Thoát");
    messageBox.setText(
                "Bạn chưa có file cấu hình!\nBạn hãy copy file cấu hình vào thư mục "
                "chứa chương trình và chạy lại.");
    if (messageBox.exec() == QMessageBox::Ok) {
        messageBox.close();
        killApplication();
    }
}


void warningWhenAnInstanceApplicationIsRunning() {
    QMessageBox messageBox;
    messageBox.setButtonText(1, "Thoát");
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setText(
                "Ứng dụng Video Wall đã được chạy.\n"
                "Ứng dụng chỉ được chạy khởi tạo một lần. Xin cảm ơn!");
    messageBox.exec();
}

int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationName("VideoWall");
    QCoreApplication::setOrganizationName("VP9");
    QCoreApplication::setOrganizationDomain("vp9.vn");
    QCoreApplication::setApplicationVersion("2017.10.05.0");
    QApplication a(argc, argv);


    setlocale(LC_NUMERIC, "C");
    NetworkUtils::instance();
    Resources::instance();
    QSystemSemaphore semaphore("<uniq id>", 1);  // create semaphore
    semaphore.acquire(); // Raise the semaphore, barring other instances to work with shared memory

#ifndef Q_OS_WIN32
    // in linux / unix shared memory is not freed when the application terminates abnormally,
    // so you need to get rid of the garbage
    QSharedMemory nix_fix_shared_memory("<uniq id 2>");
    if(nix_fix_shared_memory.attach()){
        nix_fix_shared_memory.detach();
    }
#endif

    QSharedMemory sharedMemory("<uniq id 2>");  // Create a copy of the shared memory
    bool is_running;            // variable to test the already running application
    if (sharedMemory.attach()){ // We are trying to attach a copy of the shared memory
        // To an existing segment
        is_running = true;      // If successful, it determines that there is already a running instance
    }else{
        sharedMemory.create(1); // Otherwise allocate 1 byte of memory
        is_running = false;     // And determines that another instance is not running
    }
    semaphore.release();
    // If you already run one instance of the application, then we inform the user about it
    // and complete the current instance of the application
    if(is_running){
        warningWhenAnInstanceApplicationIsRunning();
        exit(0);
    }

    AppProfile::copyConfigFile();
    AppProfile::openAppProfiles();
    if (AppProfile::getAppProfile() != Q_NULLPTR) {
        QWidget *window = new QWidget();
        C_MainFrame *cFrame = new C_MainFrame(Q_NULLPTR, window);
        AppMessage Message;
        cFrame->newAction(Message.PROJECT_RUN, 0);
    } else {
        showMessageNotFindAppConfig();
    }
    return a.exec();
}
