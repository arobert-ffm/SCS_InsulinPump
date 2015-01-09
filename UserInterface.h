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

signals:
    /**
     * Notifys the Pump to refill the Insulin Reservoir
     */
    void refillInsulinInPump();
    /**
     * Notifys the Pump to refill the Glucagon Reservoir
     */
    void refillGlucagonInPump();

private:
    Ui::UserInterface *ui;
};

#endif // USERINTERFACE_H
