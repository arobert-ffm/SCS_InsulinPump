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

    // Init Testing section
    ui->mTestingBatterySlider->setMaximum(ui->mBatteryProgressBar->maximum());
    ui->mTestingGlucagonSlider->setMaximum(ui->mGlucagonProgressBar->maximum());
    ui->mTestingInsulinSlider->setMaximum(ui->mInsulinProgressBar->maximum());

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
 * Updates the Insulin amount in the Progressbar
 *
 * @param amount - new insulin amount
 */
void UserInterface::insulinAmountInReservoirChanged(float amount)
{
    // Update Color
    if (amount < 51 && amount >= 26)
    {
        ui->mInsulinProgressBar->setStyleSheet(ui->mInsulinProgressBar->property("defaultStyleSheet").toString() +
                                                   "QProgressBar { border: 1px solid grey; border-radius: 4px; background-color: rgb(213, 213, 213); }" +
                                                    "QProgressBar::chunk { background: rgb(240, 240, 0); }");
    } else if(amount < 26)
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
 * Updates the Glucagon amount in the Progressbar
 *
 * @param amount - new glucagon amount
 */
void UserInterface::glucagonAmountInReservoirChanged(float amount)
{
    // Update Color
    if (amount < 51 && amount >= 26)
    {
        ui->mGlucagonProgressBar->setStyleSheet(ui->mGlucagonProgressBar->property("defaultStyleSheet").toString() +
                                                   "QProgressBar { border: 1px solid grey; border-radius: 4px; background-color: rgb(213, 213, 213); }" +
                                                    "QProgressBar::chunk { background: rgb(240, 240, 0); }");
    } else if(amount < 26)
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
    // Create Timestamp
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    // Add Message and scroll to bottom
    ui->mMessageList->addItem(new QListWidgetItem(text + "\tStatus:\t" + message));
    ui->mMessageList->scrollToBottom();
}

/**
 * Inserts the warning message in to the QListWidgetItem
 *
 * @param message - string message to insert
 */
void UserInterface::insertWarningLog(QString message)
{
    // Create Timestamp
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    // Add Message and scroll to bottom
    QListWidgetItem* item = new QListWidgetItem(text + "\tWarning:\t" + message);
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
    // Create Timestamp
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    // Add Message and scroll to bottom
    QListWidgetItem* item = new QListWidgetItem(text + "\tCritical:\t" + message);
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

/**
 * Updates the Bloodsugar in the UI
 *
 * @param amount - current bloodsugar
 */
void UserInterface::updateBloodsugarLevel(int bloodsugarLevel, int hormone, int amountInjected)
{
    // Timestamp
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    // Insert Message
    if (hormone == INSULIN)
    {
        ui->mBloodsugarLog->addItem(new QListWidgetItem(text + "  injected " + QString::number(amountInjected) + " units Insulin"));
        ui->mBloodsugarLog->scrollToBottom();
    } else if(hormone == GLUCAGON)
    {
        ui->mBloodsugarLog->addItem(new QListWidgetItem(text + "  injected " + QString::number(amountInjected) + " units Glucagon"));
        ui->mBloodsugarLog->scrollToBottom();
    }
    // Remove oldest item if count is above 25
    if (ui->mBloodsugarLog->count() > 26)
    {
        ui->mBloodsugarLog->takeItem(0);
        //delete item;
    }
    ui->mBloodSugarValue->setValue(bloodsugarLevel);
}

/**
 * Testing onBatteryButtonClicked
 *
 * Reads the value from the slider and calls the SIGNAL method setBatteryLevel().
 */
void UserInterface::on_mTestingBatteryButton_clicked()
{
    emit setBatteryLevel(ui->mTestingBatterySlider->value());
}

/**
 * Testing onGlucagonButtonClicked
 *
 * Reads the value from the slider and calls the SIGNAL method setGlucagonReservoirLevel().
 */
void UserInterface::on_mTestingGlucagonButton_clicked()
{
    emit setGlucagonReservoirLevel(ui->mTestingGlucagonSlider->value());
}

/**
 * Testing onInsulinButtonClicked
 *
 * Reads the value from the slider and calls the SIGNAL method setInsulinReservoirLevel().
 */
void UserInterface::on_mTestingInsulinButton_clicked()
{
    emit setInsulinReservoirLevel(ui->mTestingInsulinSlider->value());
}
