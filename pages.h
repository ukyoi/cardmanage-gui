#ifndef PAGES_H
#define PAGES_H

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QCheckBox>

#include <QDebug>

#include "func.h"


class ConsumePage : public QWidget
{
    Q_OBJECT
    friend class MainWindow;
public:
    ConsumePage(QWidget *parent=0);
    QLabel *tipLabel;
private:
    QLabel *moneyLabel;
    QLineEdit *moneyEdit;
    QPushButton *confirmButton;
    
public slots:
    int consume();
    
signals:
    void consumeFinished();
};



class ChargePage : public QWidget
{
    Q_OBJECT
    friend class MainWindow;
public:
    ChargePage(QWidget *parent=0);
    QLabel *tipLabel;
private:
    QLabel *moneyLabel;
    QLineEdit *moneyEdit;
    QPushButton *confirmButton;
    
public slots:
    int charge();
    
signals:
    void chargeFinished();
};



class AddUserPage : public QWidget
{
    Q_OBJECT
public:
    AddUserPage(QWidget *parent=0);
    QLineEdit *idEdit;
    QLineEdit *nameEdit;
    QLineEdit *moneyEdit;
    QLabel *tipLabel;
private:
    QLabel *idTitleLabel;
    QLabel *nameTitleLabel;
    QLabel *moneyTitleLabel;
    QPushButton *confirmButton;
private slots:
    int addIt();
    
signals:
    void addFinished(unsigned int userNum, QString userName, double userMoney);
};



class DeleteUserPage : public QWidget
{
    Q_OBJECT
public:
    DeleteUserPage(QWidget *parent=0);
    QPushButton *confirmButton;
    QCheckBox *confirmCheckBox;
    QLabel *tipLabel;
private:
    QLabel *alertLabel;

private slots:
    void confirmed(int state);
    void deleteIt();
signals:
    void deleteFinished();

};


// These are actually defined in MainWindow.cpp
extern vector<unsigned int> tableItemIndexVec; // Indecates  which element in userPtrVec the table item represent of.
extern bool ifSelectedContentUser;
extern unsigned int contentUserIndex;



#endif
