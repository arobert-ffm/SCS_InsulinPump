#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QMainWindow>
#include <QString>
#include <string>

using namespace std;



namespace Ui {
class UserInterface;
}

class UserInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = 0);
    ~UserInterface();

    static const int INSULIN    = 1;
    static const int GLUCAGON   = 2;

public slots:
    /**
     * Updates the Batteries power level in the Progressbar
     *
     * @param level - new battery power level
     */
    void batteryPowerLevelChanged(int level);
    /**
     * Updates the Batteries minimum power level in the spin box
     *
     * @param level - new battery power level
     */
    void minBatteryLevelChanged(int level);
    /**
     * Updates the Insulin amount in the Progressbar
     *
     * @param amount - new insulin amount
     */
    void insulinAmountInReservoirChanged(float amount);
    /**
     * Updates the Glucagon amount in the Progressbar
     *
     * @param amount - new glucagon amount
     */
    void glucagonAmountInReservoirChanged(float amount);
    /**
     * Updates the operation time in the Progressbar
     *
     * @param hours - new operation time in hours
     */
    void operationTimeChanged(int hours);
    /**
     * Updates the maximum operation time in the spin box
     *
     * @param hours - new maximum operation time in hours
     */
    void maxOperationTimeChanged(int hours);
    /**
     * Inserts the status message in to the QListWidgetItem
     *
     * @param message - string message to insert
     */
    void insertStatusLog(QString message);
    /**
     * Inserts the warning message in to the QListWidgetItem
     *
     * @param message - string message to insert
     */
    void insertWarningLog(QString message);
    /**
     * Inserts the critical message in to the QListWidgetItem
     *
     * @param message - string message to insert
     */
    void insertCriticalLog(QString message);

    /**
     * Updates the Bloodsugar in the UI
     *
     * @param amount - current bloodsugar
     */
    void updateBloodsugarLevel(int bloodsugarLevel, int hormone, int amountInjected);

private slots:
    /**
     * Refill the Insulinreservoir in the Pump
     */
    void on_mInsulinRefillButton_clicked();
    /**
     * Refill the Glucagonreservoir in the Pump
     */
    void on_mGlucagonRefillButton_clicked();

    /**
     * Updates the Time in the UI
     */
    void updateClock();

    /**
     * Testing onBatteryButtonClicked
     *
     * Reads the value from the slider and calls the SIGNAL method setBatteryLevel().
     */
    void on_mTestingBatteryButton_clicked();

    /**
     * Testing onMinBatLoadButtonClicked
     *
     * Reads the value from the slider and calls the SIGNAL method setMinBatteryLevel().
     */
    void on_mMinBatLoadButton_clicked();

    /**
     * Testing onGlucagonButtonClicked
     *
     * Reads the value from the slider and calls the SIGNAL method setGlucagonReservoirLevel().
     */
    void on_mTestingGlucagonButton_clicked();

    /**
     * Testing onInsulinButtonClicked
     *
     * Reads the value from the slider and calls the SIGNAL method setInsulinReservoirLevel().
     */
    void on_mTestingInsulinButton_clicked();

    /**
     * Testing onOpTimeButtonClicked
     *
     * Reads the value from the slider and calls the SIGNAL method setOperationTime();.
     */
    void on_mTestingOpTimeButton_clicked();

    /**
     * Testing onmMaxOpTimeButtonClicked
     *
     * Reads the value from the slider and calls the SIGNAL method setMaxOperationTime();.
     */
    void on_mMaxOpTimeButton_clicked();

signals:
    /**
     * Notifys the Pump to refill the Insulin Reservoir
     */
    void refillInsulinInPump();

    /**
     * Notifys the Pump to refill the Glucagon Reservoir
     */
    void refillGlucagonInPump();

    /**
     * Notifys the SLOT method of the Pump class, which changes the battery level.
     *
     * @param level - new battery level
     */
    void setBatteryPowerLevel(int level);

    /**
     * Notifys the SLOT method of the ControlSystem class, which changes the min battery level.
     *
     * @param level - new minimum battery level
     */
    void setMinBatteryLevel(int level);

    /**
     * Notifys the SLOT method of the Pump class, which changes the glucagonreservoir fill level.
     *
     * @param level - new glucagonreservoir fill level
     */
    void setGlucagonReservoirLevel(int level);

    /**
     * Notifys the SLOT method of the Pump class, which changes the Insulinreservoir fill level.
     *
     * @param level - new insulinreservoir fill level
     */
    void setInsulinReservoirLevel(int level);

    /**
     * Notifys the SLOT method of the Scheduler class, which changes the operation time.
     *
     * @param hours - new operation time in hours
     */
    void setOperationTime(int hours);

    /**
     * Notifys the SLOT method of the Scheduler class, which changes the max operation time.
     *
     * @param hours - new maximum operation time in hours
     */
    void setMaxOperationTime(int hours);

private:
    Ui::UserInterface *ui;
};

#endif // USERINTERFACE_H
