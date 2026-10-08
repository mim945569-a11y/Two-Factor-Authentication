#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    int generatedOTP;
    QTimer *otpTimer;
    int otpAttempts;
private slots:
    void verifyLogin();
    void otpExpired();
    void resendOTP();
    void logout();
};
#endif // MAINWINDOW_H
