#include "UserInterface.h"
#include "ui_UserInterface.h"
#include "iostream"
#include <string>
#include <QString>
#include <QTime>
#include <QTimer>

using namespace std;


UserInterface::UserInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserInterface)
{
    ui->setupUi(this);

    // Init Time
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateClock()));
    timer->start();
    updateClock();
}

UserInterface::~UserInterface()
{
    delete ui;
}

/**
 * Updates the Insulin amount in the Label
 *
 * @param amount - new insulin amount
 */
void UserInterface::insulinAmountInReservoirChanged(float amount)
{
    // Update Color
    if (amount < 50 && amount >= 25)
    {
        ui->mInsulinProgressBar->setStyleSheet(ui->mInsulinProgressBar->property("defaultStyleSheet").toString() +
                                                   "QProgressBar { border: 1px solid grey; border-radius: 4px; background-color: rgb(213, 213, 213); }" +
                                                    "QProgressBar::chunk { background: rgb(240, 240, 0); }");
    } else if(amount < 25)
    {
        ui->mInsulinProgressBar->setStyleSheet(ui->mInsulinProgressBar->property("defaultStyleSheet").toString() +
                                                   "QProgressBar { border: 1px solid grey; border-radius: 4px; background-color: rgb(213, 213, 213); }" +
                                                    "QProgressBar::chunk { background: rgb(255, 0, 0); }");
    } else
    {
        ui->mInsulinProgressBar->setStyleSheet(ui->mInsulinProgressBar->property("defaultStyleSheet").toString() +
                                                   "QProgressBar { border: 1px solid grey; border-radius: 4px; background-color: rgb(213, 213, 213); }" +
                                                    "QProgressBar::chunk { background: rgb(0, 210, 0); }");
    }

    // Set Amount
    ui->mInsulinProgressBar->setValue(amount);
}

/**
 * Updates the Glucagon amount in the Label
 *
 * @param amount - new glucagon amount
 */
void UserInterface::glucagonAmountInReservoirChanged(float amount)
{
    // Update Color
    if (amount < 50 && amount >= 25)
    {
        ui->mGlucagonProgressBar->setStyleSheet(ui->mGlucagonProgressBar->property("defaultStyleSheet").toString() +
                                                   "QProgressBar { border: 1px solid grey; border-radius: 4px; background-color: rgb(213, 213, 213); }" +
                                                    "QProgressBar::chunk { background: rgb(240, 240, 0); }");
    } else if(amount < 25)
    {
        ui->mGlucagonProgressBar->setStyleSheet(ui->mGlucagonProgressBar->property("defaultStyleSheet").toString() +
                                                   "QProgressBar { border: 1px solid grey; border-radius: 4px; background-color: rgb(213, 213, 213); }" +
                                                    "QProgressBar::chunk { background: rgb(255, 0, 0); }");
    } else
    {
        ui->mGlucagonProgressBar->setStyleSheet(ui->mGlucagonProgressBar->property("defaultStyleSheet").toString() +
                                                   "QProgressBar { border: 1px solid grey; border-radius: 4px; background-color: rgb(213, 213, 213); }" +
                                                    "QProgressBar::chunk { background: rgb(0, 210, 0); }");
    }

    // Set Amount
    ui->mGlucagonProgressBar->setValue(amount);
}

/**
 * Inserts the status message in to the QListWidgetItem
 *
 * @param message - string message to insert
 */
void UserInterface::insertStatusLog(QString message)
{
    ui->mMessageList->addItem(new QListWidgetItem("Status:\t" + message));
    ui->mMessageList->scrollToBottom();
}

/**
 * Inserts the warning message in to the QListWidgetItem
 *
 * @param message - string message to insert
 */
void UserInterface::insertWarningLog(QString message)
{
    QListWidgetItem* item = new QListWidgetItem("Warning:\t" + message);
    item->setBackgroundColor(Qt::yellow);
    ui->mMessageList->addItem(item);
    ui->mMessageList->scrollToBottom();
}

/**
 * Inserts the critical message in to the QListWidgetItem
 *
 * @param message - string message to insert
 */
void UserInterface::insertCriticalLog(QString message)
{
    QListWidgetItem* item = new QListWidgetItem("Critical:\t" + message);
    item->setBackgroundColor(Qt::red);
    ui->mMessageList->addItem(item);
    ui->mMessageList->scrollToBottom();
}

/**
 * Refill the Insulinreservoir in the Pump
 */
void UserInterface::on_mInsulinRefillButton_clicked()
{
    // Trigger Callback for refilling Insulin Reservoir in the Pump
    emit refillInsulinInPump();
}

/**
 * Refill the Glucagonreservoir in the Pump
 */
void UserInterface::on_mGlucagonRefillButton_clicked()
{
    // Trigger Callback for refilling Glucagon Reservoir in the Pump
    emit refillGlucagonInPump();
}

/**
 * Updates the Time in the UI
 */
void UserInterface::updateClock()
{
    // Get Time
    QTime time = QTime::currentTime();
    // Format and set Time
    QString text = time.toString("hh:mm:ss");
    ui->mTimeValue->setText(text);
}
