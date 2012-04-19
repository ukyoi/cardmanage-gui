#include "pages.h"
#include "func.h"

#define SET_GREEN_BG setStyleSheet("background-color: rgb(127, 255, 127);");
#define SET_RED_BG setStyleSheet("background-color: rgb(255, 127, 127);");
#define SET_BLUE_BG setStyleSheet("background-color: rgb(127, 127, 255);");



ConsumePage::ConsumePage(QWidget *parent) : QWidget(parent)
{
    // TODO:
    moneyLabel=new QLabel(tr("Consume Balance: "));
    moneyEdit=new QLineEdit;
    tipLabel=new QLabel;
    confirmButton=new QPushButton(tr("Consume Now"));
    confirmButton->setEnabled(false);

    QHBoxLayout *enterLayout=new QHBoxLayout;
    enterLayout->addWidget(moneyLabel);
    enterLayout->addWidget(moneyEdit);

    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addLayout(enterLayout);
    mainLayout->addWidget(tipLabel);
    mainLayout->addWidget(confirmButton);

    setLayout(mainLayout);
    
    connect( confirmButton, SIGNAL(clicked()), this, SLOT(consume()) );
}

// TODO:
int ConsumePage::consume()
{
    /**
     * return 1 means no enough money
     */
    User* toConsume=userPtrVec.at(contentUserIndex);
    int result=toConsume->consume( moneyEdit->text().toDouble() );
    if (result!=0) {
        tipLabel->setText(tr("Consume failed!<br>Check if has enough money."));
        tipLabel->SET_RED_BG;
        return 1;
    }
    emit consumeFinished();
    tipLabel->setText(tr("Consume successfully."));
    tipLabel->SET_GREEN_BG;
    return 0;
}


ChargePage::ChargePage(QWidget *parent) : QWidget(parent)
{
    moneyLabel=new QLabel(tr("Charge Balance: "));
    moneyEdit=new QLineEdit;
    tipLabel=new QLabel;
    confirmButton=new QPushButton(tr("Charge Now"));
    confirmButton->setEnabled(false);

    QHBoxLayout *enterLayout=new QHBoxLayout;
    enterLayout->addWidget(moneyLabel);
    enterLayout->addWidget(moneyEdit);

    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addLayout(enterLayout);
    mainLayout->addWidget(tipLabel);
    mainLayout->addWidget(confirmButton);

    setLayout(mainLayout);
    
    connect( confirmButton, SIGNAL(clicked()), this, SLOT(charge()) );
}

// TODO:
int ChargePage::charge()
{
    User* toCharge=userPtrVec.at(contentUserIndex);
    int result=toCharge->charge( moneyEdit->text().toDouble() );
    if (result!=0) {
        tipLabel->setText(tr("Charge failed!<br>Check if it has enough free disk space."));
        tipLabel->SET_RED_BG;
        return -1;
    }
    emit chargeFinished();
    tipLabel->setText(tr("Charge successfully."));
    tipLabel->SET_GREEN_BG;
    return 0;
}


AddUserPage::AddUserPage(QWidget *parent) : QWidget(parent)
{
    idTitleLabel=new QLabel(tr("ID Number:"));
    idEdit=new QLineEdit;
    nameTitleLabel=new QLabel(tr("User Name:"));
    nameEdit=new QLineEdit;
    moneyTitleLabel=new QLabel(tr("Primary Balance:"));
    moneyEdit=new QLineEdit;
    tipLabel=new QLabel;
    confirmButton=new QPushButton(tr("Add This User"));

    QGridLayout *gridLayout=new QGridLayout;
    gridLayout->addWidget(idTitleLabel, 0, 0);
    gridLayout->addWidget(idEdit, 0, 1);
    gridLayout->addWidget(nameTitleLabel, 1, 0);
    gridLayout->addWidget(nameEdit, 1, 1);
    gridLayout->addWidget(moneyTitleLabel, 2, 0);
    gridLayout->addWidget(moneyEdit, 2, 1);

    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(tipLabel);
    mainLayout->addWidget(confirmButton);

    setLayout(mainLayout);
    
    connect( confirmButton, SIGNAL(clicked()), this, SLOT(addIt()) );
}
int AddUserPage::addIt()
{
    // FIXME:
    unsigned int userNum=(unsigned int)(idEdit->text().toInt());
    int result=addUser( userNum, nameEdit->text().toStdString(), moneyEdit->text().toDouble() );
    if (result==1) {
        tipLabel->setText(tr("The user number is already exists!"));
        tipLabel->SET_BLUE_BG
        return -1;
    }
    if (result<0) {
        tipLabel->setText(tr("Add user failed!"));
        return -1;
    }
        
    // Return 0 means successfully.
    if (result==0) {
        emit addFinished( userNum, nameEdit->text(), moneyEdit->text().toDouble() );
        tipLabel->setText(tr("Add user successfully!"));
        tipLabel->SET_GREEN_BG;
    }
    return result;
}


DeleteUserPage::DeleteUserPage(QWidget *parent) : QWidget(parent)
{
    alertLabel=new QLabel(tr("<font color=red>Are you sure to delete this user's informations?<br>User informations can not be recovered if you deleted them.<font>"));
    tipLabel=new QLabel();
    confirmCheckBox=new QCheckBox(tr("Confirm to Delete the User."));
    confirmButton=new QPushButton(tr("Delete This User"));
    confirmButton->setEnabled(false);
    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addWidget(alertLabel);
    mainLayout->addWidget(confirmCheckBox);
    mainLayout->addWidget(confirmButton);
    mainLayout->addWidget(tipLabel);

    connect( confirmCheckBox, SIGNAL(stateChanged(int)), this, SLOT(confirmed(int)) );
    connect( confirmButton, SIGNAL(clicked()), this, SLOT(deleteIt()) );

    setLayout(mainLayout);
}

void DeleteUserPage::confirmed(int state)
{
    if (state!=0) {
        qDebug() << "Cheched.";
        if (ifSelectedContentUser) {
            confirmButton->setEnabled(true);
        }
    } else {
        qDebug() << "Not Checked.";
        confirmButton->setEnabled(false);
    }
}

// TODO:
void DeleteUserPage::deleteIt()
{
    int result;
    if (ifSelectedContentUser) {
        result=deleteUser(contentUserIndex);
    }
    
    if (result!=0) {
    tipLabel->setText(tr("Delete failed. Please check your disk space."));
    tipLabel->SET_RED_BG;
    }
    
    emit deleteFinished();
    tipLabel->setText(tr("Delete successfully!"));
    tipLabel->SET_GREEN_BG;
}