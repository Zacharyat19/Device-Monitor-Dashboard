#include <QApplication>
#include "DashboardWindow.h"

int main(int argc, char *argv[]) 
{
    // Initialize the Qt application environment and event loop
    QApplication app(argc, argv);

    // Instantiate the main window, which internally builds the UI and sensors
    DashboardWindow window;
    
    // Set the global window title and display it
    window.setWindowTitle("Device Monitor Dashboard");
    window.show();

    // Hand control over to the Qt event loop
    return app.exec();
}