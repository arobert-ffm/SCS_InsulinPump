#include "UserInterface.h"
#include "ui_UserInterface.h"
#include "iostream"
#include <string>
#include <QString>

using namespace std;


UserInterface::UserInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserInterface)
{
    ui->setupUi(this);
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
    ui->mInsulinReservoirValue->setText(QString::number(amount));
}

/**
 * Updates the Glucagon amount in the Label
 *
 * @param amount - new glucagon amount
 */
void UserInterface::glucagonAmountInReservoirChanged(float amount)
{
    ui->mGlucagonReservoirValue->setText(QString::number(amount));
}

/**
 * Inserts the status message in to the QListWidgetItem
 *
 * @param message - string message to insert
 */
void UserInterface::insertStatusLog(string& message)
{
    ui->mMessageList->addItem(new QListWidgetItem("Status:\t" + QString::fromStdString(message)));
    ui->mMessageList->scrollToBottom();
}

/**
 * Inserts the warning message in to the QListWidgetItem
 *
 * @param message - string message to insert
 */
void UserInterface::insertWarningLog(string& message)
{
    QListWidgetItem* item = new QListWidgetItem("Warning:\t" + QString::fromStdString(message));
    item->setBackgroundColor(Qt::yellow);
    ui->mMessageList->addItem(item);
    ui->mMessageList->scrollToBottom();
}

/**
 * Inserts the critical message in to the QListWidgetItem
 *
 * @param message - string message to insert
 */
void UserInterface::insertCriticalLog(string& message)
{
    QListWidgetItem* item = new QListWidgetItem("Critical:\t" + QString::fromStdString(message));
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
