#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class MessageWidget : public QWidget
{
    Q_OBJECT
    
private:
    QLabel *mTitleLabel;
    QLabel *mMessageLabel;
    QVBoxLayout *mMessageLayout;
    QSize mScreenSize;
    QPoint mTopPos;
    QPoint mCenPos;
    QPoint mBotPos;
public:
    MessageWidget(QWidget *parent);
    void initUI(QSize);
    void setMessage(QString);
    void setTitle(QString);
    
    void message(QString, QString);
    
    void showAnimation();
    void hideAnimation();
    
};

#endif // MESSAGEWIDGET_H
