#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QMainWindow>
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
     * Updates the Insulin amount in the Label
     *
     * @param amount - new insulin amount
     */
    void insulinAmountInReservoirChanged(float amount);
    /**
     * Updates the Glucagon amount in the Label
     *
     * @param amount - new glucagon amount
     */
    void glucagonAmountInReservoirChanged(float amount);
    /**
     * Inserts the status message in to the QListWidgetItem
     *
     * @param message - string message to insert
     */
    void insertStatusLog(string& message);
    /**
     * Inserts the warning message in to the QListWidgetItem
     *
     * @param message - string message to insert
     */
    void insertWarningLog(string& message);
    /**
     * Inserts the critical message in to the QListWidgetItem
     *
     * @param message - string message to insert
     */
    void insertCriticalLog(string& message);

private:
    Ui::UserInterface *ui;
};

#endif // USERINTERFACE_H
