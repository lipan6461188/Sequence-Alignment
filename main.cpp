#include <QApplication>
#include "Plotting.h"
#include "MainWindow.h"


int main(int argc, char *argv[])
{
    
    QApplication app(argc,argv);
   
    app.setWindowIcon(QIcon(":/images/icon.png"));
    
    MainWindow window;
    
    window.show();
    
    

    
    return app.exec();
}