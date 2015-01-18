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
    void setBatteryLevel(int level);

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

private:
    Ui::UserInterface *ui;
};

#endif // USERINTERFACE_H
