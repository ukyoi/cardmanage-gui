#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "pages.h"
#include "func.h"

/* These are already included in "pages.h"
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
*/
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QHeaderView>
#include <QtGui/QStackedWidget>
#include <QtGui/QDialog>
#include <QDebug>



class MainWindow : public QWidget
{
    Q_OBJECT
    friend class ChargePage;
    friend class ConsumePage;

public:
    MainWindow();
    
    int initializeLoadData();
    void listUsers();
    User* getContentUserPtr();

private:
    QLabel *searchTitleLabel;
    QLineEdit *searchLineEdit;
    QTableWidget *userTableWidget;

    QListWidget *funcListWidget;

    QLabel *nameTitleLabel;
    QLabel *numberTitleLayout;
    QLabel *moneyTitleLabel;
    QLabel *nameValueLabel;
    QLabel *numberValueLabel;
    QLabel *moneyValueLabel;

    QStackedWidget *pageWidget;

    ConsumePage *consumePage;
    ChargePage *chargePage;
    AddUserPage *addUserPage;
    DeleteUserPage *deleteUserPage;

    
    
    void createUI();
    void createIcons();
    
    
private slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    
    void searchUser(const QString& subtext);
    void selectUser(int tableWidgetRow, int tableWidgetCol);
    void showUserDetail();
    void unselectUser();
    void setEnableOperations(bool enableOrNot);
    void showAddFinished(unsigned int userNum, QString userName, double userMoney);
    
public slots:
    void showAllUser();
    void showUsers(vector<unsigned int> indexVec);

};



void setDirs();


#endif
