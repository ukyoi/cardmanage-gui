#include <QtGui/QApplication>
#include "MainWindow.h"
#include <QtCore/QTextCodec>
#include <QtCore/QTranslator>
#include <QDebug>



int main(int argc, char *argv[])
{
    
    QApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    
    QTranslator translator;
    translator.load("translate_zh");
    app.installTranslator(&translator);
    
    setDirs();
    
    MainWindow *mainWindow=new MainWindow;
    mainWindow->show();
    
    mainWindow->initializeLoadData();
    mainWindow->showAllUser();
    
    
    return app.exec();
}