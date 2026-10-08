#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRandomGenerator>
#include <QMessageBox>
#include<QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dashboardFrame->setStyleSheet(
        "QFrame {"
        "background-color: #000000;"
        "border: 1px solid #2196F3;"
        "border-radius: 12px;"
        "}"
        );
    ui->dashboardFrame->hide();
    otpTimer = new QTimer(this);
    otpTimer->setSingleShot(true);

        connect(otpTimer, &QTimer::timeout,this, &MainWindow::otpExpired);
         connect(ui->verifyButton, &QPushButton::clicked,this, &MainWindow::verifyLogin);
        connect(ui->resendButton, &QPushButton::clicked,this, &MainWindow::resendOTP);
         connect(ui->logoutButton, &QPushButton::clicked,
                 this, &MainWindow::logout);
         connect(ui->showPasswordButton, &QPushButton::clicked,
                 this, [this]()
                 {
                     if (ui->passwordEdit->echoMode() == QLineEdit::Password)
                     {
                         ui->passwordEdit->setEchoMode(QLineEdit::Normal);
                         ui->showPasswordButton->setText("Hide");
                     }
                     else
                     {
                         ui->passwordEdit->setEchoMode(QLineEdit::Password);
                         ui->showPasswordButton->setText("Show");
                     }
                 });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::verifyLogin()
{
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    QString enteredOTP = ui->otpEdit->text();

    if (username != "admin" || password != "1234")
    {
        ui->statusLabel->setText("Invalid username or password!");
        return;
    }

    if (enteredOTP.isEmpty())
    {
        int otp = QRandomGenerator::global()->bounded(100000, 1000000);
              generatedOTP = otp;
        otpTimer->start(60000);

        ui->statusLabel->setText("OTP generated. Enter the OTP.");

        QMessageBox msgBox(this);

        msgBox.setWindowTitle("Two-Factor Authentication");
        msgBox.setText("Your OTP is:");
        msgBox.setInformativeText(QString::number(otp));
        msgBox.setStyleSheet(
            "QMessageBox {"
            "background-color: #000000;"
            "color: white;"
            "}"
            "QLabel {"
            "color: white;"
            "font-size: 18px;"
            "font-weight: bold;"
            "}"
            "QPushButton {"
            "background-color: #2196F3;"
            "color: white;"
            "padding: 6px 20px;"
            "border-radius: 5px;"
            "}"
            );

        msgBox.exec();
        ui->otpEdit->setProperty("generatedOTP", otp);
        return;
    }

    if (enteredOTP.toInt() == generatedOTP)
    {
        ui->statusLabel->setStyleSheet("color: #00FF00; font-weight: bold;");
        ui->statusLabel->setText("Login Successful! ✓");
        ui->dashboardFrame->show();
    }
    else
        {
        ui->statusLabel->setStyleSheet("color: #FF4444; font-weight: bold;");
        otpAttempts++;

            if (otpAttempts >= 3)
            {
                generatedOTP = 0;
                otpTimer->stop();
                ui->statusLabel->setText("Too many wrong attempts!");
                ui->otpEdit->clear();
            }
            else
            {
                ui->statusLabel->setText(
                    "Invalid OTP! Attempts left: " +
                    QString::number(3 - otpAttempts)
                    );
            }
        }
    }

void MainWindow::otpExpired()
{
    generatedOTP = 0;
    ui->otpEdit->clear();
    ui->statusLabel->setStyleSheet("color: #FF4444; font-weight: bold;");
    ui->statusLabel->setText("OTP expired! Please generate a new OTP.");
}
void MainWindow::resendOTP()
{
    int otp = QRandomGenerator::global()->bounded(100000, 1000000);

    generatedOTP = otp;
    otpTimer->start(60000);
    otpAttempts=0;

    ui->otpEdit->clear();
    ui->statusLabel->setText("New OTP generated.");



QMessageBox msgBox(this);

msgBox.setWindowTitle("New OTP");
msgBox.setText("Your new OTP is:");
msgBox.setInformativeText(QString::number(otp));

msgBox.setStyleSheet(
    "QMessageBox {"
    "background-color: #000000;"
    "color: white;"
    "}"
    "QLabel {"
    "color: white;"
    "font-size: 18px;"
    "font-weight: bold;"
    "}"
    "QPushButton {"
    "background-color: #2196F3;"
    "color: white;"
    "padding: 6px 20px;"
    "border-radius: 5px;"
    "}"
    );

msgBox.exec();
}void MainWindow::logout()
{
    ui->dashboardFrame->hide();
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    ui->otpEdit->clear();
    ui->statusLabel->clear();
}

