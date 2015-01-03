#include "userinterface.h"
#include <QApplication>

/**
 * Initiation of the Userinterface, Humanbody- and Insulinpumpsimulation.
 *
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    // Create User Interface
    QApplication a(argc, argv);
    UserInterface w;
    w.show();

    return a.exec();
}
