#include "MainWindow.h"



vector<unsigned int> tableItemIndexVec; // Indecates  which element in userPtrVec the table item represent of.
bool ifSelectedContentUser;
unsigned int contentUserIndex;

    
    
MainWindow::MainWindow()
{
    searchTitleLabel=new QLabel(tr("Search: "));
    searchLineEdit=new QLineEdit;
    userTableWidget=new QTableWidget;

    funcListWidget=new QListWidget;

    nameTitleLabel=new QLabel(tr("User Name: "));
    numberTitleLayout=new QLabel(tr("ID Number: "));
    moneyTitleLabel=new QLabel(tr("Money: "));
    nameValueLabel=new QLabel;
    numberValueLabel=new QLabel;
    moneyValueLabel=new QLabel;

    pageWidget=new QStackedWidget;
    
    tableItemIndexVec.reserve(1024);
    ifSelectedContentUser=false;

    /*
    aboutButton=new QPushButton;
    quitButton=new QPushButton;
    */

    setWindowTitle(tr("Card Manager (GUI Edition)"));

    createUI();
    createIcons();
    
    connect(userTableWidget, SIGNAL(cellActivated(int, int)), this, SLOT(selectUser(int, int)) );
}

void MainWindow::createUI()
{
    setMinimumWidth(640);

    /* TODO:
     * This part should be modified to icon mode in the future.
     */
    // funcListWidget->setViewMode(QListView::IconMode);
    funcListWidget->setMaximumWidth(128);
    funcListWidget->setMovement(QListView::Static);
    // funcListWidget->setSpacing(12);
    
    
    QStringList tableDisplayList;
    // userTableWidget->setColumnCount(3);
    userTableWidget->setColumnCount(2);
    tableDisplayList << tr("User Name") << tr("ID Number") << tr("Money");
    userTableWidget->setHorizontalHeaderLabels(tableDisplayList);
    // userTableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    // userTableWidget->horizontalHeader()->show();
    userTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    userTableWidget->verticalHeader()->hide();
    userTableWidget->setShowGrid(false);
    
    
    connect( searchLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(searchUser(const QString&)) );

    
    QGridLayout *titleAndValueLayout=new QGridLayout;
    titleAndValueLayout->addWidget(nameTitleLabel, 0, 0);
    titleAndValueLayout->addWidget(nameValueLabel, 0, 1);
    titleAndValueLayout->addWidget(numberTitleLayout, 1, 0);
    titleAndValueLayout->addWidget(numberValueLabel, 1, 1);
    titleAndValueLayout->addWidget(moneyTitleLabel, 2, 0);
    titleAndValueLayout->addWidget(moneyValueLabel, 2, 1);

    QHBoxLayout *searchLayout=new QHBoxLayout;
    searchLayout->addWidget(searchTitleLabel);
    searchLayout->addWidget(searchLineEdit);

    QVBoxLayout *leftLayout=new QVBoxLayout;
    leftLayout->addLayout(searchLayout);
    leftLayout->addWidget(userTableWidget);

    QVBoxLayout *rightLayout=new QVBoxLayout;
    rightLayout->addLayout(titleAndValueLayout);
    rightLayout->addWidget(pageWidget);

    QHBoxLayout *funcPartLayout=new QHBoxLayout;
    funcPartLayout->addLayout(leftLayout);
    funcPartLayout->addWidget(funcListWidget);
    funcPartLayout->addLayout(rightLayout);

    /* Here is the page adding part:
     * The sequence of page adding should be be same with icon adding. */
    consumePage=new ConsumePage;
    chargePage=new ChargePage;
    addUserPage=new AddUserPage;
    deleteUserPage=new DeleteUserPage;
    pageWidget->addWidget(consumePage);
    pageWidget->addWidget(chargePage);
    pageWidget->addWidget(addUserPage);
    pageWidget->addWidget(deleteUserPage);
    pageWidget->setCurrentWidget(consumePage);
    
    connect(consumePage, SIGNAL(consumeFinished()), this, SLOT(showUserDetail()) );
    connect(chargePage, SIGNAL(chargeFinished()), this, SLOT(showUserDetail()) );
    connect(addUserPage, SIGNAL(addFinished(unsigned int, QString, double)), this, SLOT(showAddFinished(unsigned int , QString, double)) );
    connect(deleteUserPage, SIGNAL(deleteFinished()), this, SLOT(unselectUser()));
    connect(deleteUserPage, SIGNAL(deleteFinished()), this, SLOT(showAllUser()));

    setLayout(funcPartLayout);
}

void MainWindow::createIcons()
{
    /* The sequence of icon adding should be same with page adding. */
    QListWidgetItem *consumeButton=new QListWidgetItem(funcListWidget);
    consumeButton->setText(tr("User Consume"));
    consumeButton->setTextAlignment(Qt::AlignHCenter);
    consumeButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *chargeButton=new QListWidgetItem(funcListWidget);
    chargeButton->setText(tr("User Charge"));
    chargeButton->setTextAlignment(Qt::AlignHCenter);
    consumeButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *addUserButton=new QListWidgetItem(funcListWidget);
    addUserButton->setText(tr("Add New User"));
    addUserButton->setTextAlignment(Qt::AlignHCenter);
    consumeButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *deleteUserButton=new QListWidgetItem(funcListWidget);
    deleteUserButton->setText(tr("Delete This User"));
    deleteUserButton->setTextAlignment(Qt::AlignHCenter);
    consumeButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


    connect( funcListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*, QListWidgetItem*)) );
}

void MainWindow::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    qDebug() << "changePage triggered.";
    if (!current)
        current=previous;
    pageWidget->setCurrentIndex(funcListWidget->row(current));
}


int MainWindow::initializeLoadData()
{
    QDialog *loadTipDialog=new QDialog(this);
    QVBoxLayout *loadTipLayout=new QVBoxLayout;
    QLabel *loadTipLabel=new QLabel(tr("Loading user Files."));
    loadTipLayout->addWidget(loadTipLabel);
    loadTipDialog->setLayout(loadTipLayout);
    loadTipDialog->open();
    
    int returnValue = readData();
    qDebug() << "readData return: " << returnValue;
    
    if (userPtrVec.size()>=4096) {
        loadTipLabel->setText(tr("The number of users is exceed the limit. That may cause data loss.<br>Please contact with the author of the program in time."));
        QPushButton *confirmButton=new QPushButton(tr("&OK"));
        loadTipLayout->addWidget(confirmButton);
        connect( confirmButton, SIGNAL(clicked()), loadTipDialog, SLOT(close()) );
        return -1; // Break the function.
    }
    if (returnValue>=0) {
        loadTipDialog->close();
        qDebug() << "First time load successfully!";
        return returnValue;
    } else {
        loadTipLabel->setText(tr("Open user file error. Please check if the disk has enough free space.<br>The program will exit when you push the OK button."));
        QPushButton *confirmButton=new QPushButton(tr("&OK"));
        loadTipLayout->addWidget(confirmButton);
        connect( confirmButton, SIGNAL(clicked()), this, SLOT(close()) );
        return returnValue;
    }
    
    return returnValue;
}

void MainWindow::searchUser(const QString& subtext)
{
    /**
     * Return a vector containing "index number"s of users.
     * An "index number" means: userPtrVec.at(index) is in accordance with subtext.
     * 
     * It will also call showUser function.
     */
    qDebug() << "searchUser triggered.";
    
    vector<unsigned int> resultIndex;
    
    for (unsigned int i=0; i<userPtrVec.size(); ++i) {
        
        // Check if it's included in a number:
        qApp->processEvents();
        QString contentNumQStr=QString("%1").arg( userPtrVec.at(i)->getNum() );
        if(contentNumQStr.indexOf(subtext)!=-1) {
            resultIndex.push_back(i);
            qDebug() << "pushed " << i;
            continue; // In case of pushing back one number twice.
        }
        
        // Check if it's included in a name:
        QString contentNameQStr=QString::fromStdString( userPtrVec.at(i)->getName() );
        if(contentNameQStr.indexOf(subtext)!=-1) {
            resultIndex.push_back(i);
            qDebug() << "pushed " << i;
        }
        
    }
    
    unselectUser();
    showUsers(resultIndex);
}

void MainWindow::showUsers(vector<unsigned int> indexVec)
{
    userTableWidget->setRowCount(0);
    tableItemIndexVec=indexVec; // Set right index of the table.
    
    // TODO:
    /* The codec problem seems fixed in main.cpp by setting codec to locale.
     * But still need to check on windows.
     */
    for (unsigned int i=0; i<indexVec.size(); ++i) {
        QString nameQStr( (userPtrVec.at(indexVec.at(i))->getName()).c_str() );
        QString numberQStr=QString("%1").arg( userPtrVec.at(indexVec.at(i))->getNum() );
        QString moneyQStr=QString("%1").arg( userPtrVec.at(indexVec.at(i))->getMoney() );
        QTableWidgetItem *nameItem=new QTableWidgetItem(nameQStr);
        nameItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable); // Set it not editable.
        QTableWidgetItem *numItem=new QTableWidgetItem(numberQStr);
        numItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        //QTableWidgetItem *moneyItem=new QTableWidgetItem(moneyQStr);
        //moneyItem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        
        userTableWidget->insertRow(i);
        userTableWidget->setItem(i, 0, nameItem);
        userTableWidget->setItem(i, 1, numItem);
        //userTableWidget->setItem(i, 2, moneyItem);
    }
}
void MainWindow::showAllUser()
{
    searchLineEdit->setText("");
    vector<unsigned int> allUserVec;
    allUserVec.reserve(1024);
    for (unsigned int i=0; i<userPtrVec.size(); ++i)
        allUserVec.push_back(i);
    showUsers(allUserVec);
}

void MainWindow::selectUser(int tableWidgetRow, int /*tableWidgetCol*/)
{
    unselectUser(); // the purpose of unselecting user is to remove the tip show of each page.
    qDebug() << "A user is selected.";
    contentUserIndex=tableItemIndexVec.at(tableWidgetRow);
    qDebug() << "Content userIndex is " << contentUserIndex;
    ifSelectedContentUser=true;
    showUserDetail();
    setEnableOperations(true);
}

void MainWindow::showUserDetail()
{
    if (ifSelectedContentUser) {
        nameValueLabel->setText( QString::fromStdString( userPtrVec.at(contentUserIndex)->getName() ) );
        numberValueLabel->setText( QString::number( userPtrVec.at(contentUserIndex)->getNum() ) );
        moneyValueLabel->setText( QString::number( userPtrVec.at(contentUserIndex)->getMoney() ) );
    }
}

void MainWindow::unselectUser()
{
    /**
     * This function can also cancel all things after any operation
     * so that this function should be called after taking any operation related to user index change or adding/deleting user.
     */
    
    ifSelectedContentUser=false;
    setEnableOperations(false);
    qDebug() << "A user is unselected";
    
    nameValueLabel->setText("");
    numberValueLabel->setText("");
    moneyValueLabel->setText("");
    
    chargePage->tipLabel->setText("");
    chargePage->tipLabel->setStyleSheet("");
    chargePage->moneyEdit->setText("");
    
    consumePage->tipLabel->setText("");
    consumePage->tipLabel->setStyleSheet("");
    consumePage->moneyEdit->setText("");
    
    addUserPage->tipLabel->setText("");
    addUserPage->tipLabel->setStyleSheet("");
    addUserPage->idEdit->setText("");
    addUserPage->nameEdit->setText("");
    addUserPage->moneyEdit->setText("");
    
    deleteUserPage->tipLabel->setText("");
    deleteUserPage->tipLabel->setStyleSheet("");
    deleteUserPage->confirmCheckBox->setCheckState(Qt::Unchecked);
}

void MainWindow::setEnableOperations(bool enableOrNot) {
    chargePage->confirmButton->setEnabled(enableOrNot);
    consumePage->confirmButton->setEnabled(enableOrNot);
    // FIXME:
    if (deleteUserPage->confirmCheckBox->isChecked()) {
        deleteUserPage->confirmButton->setEnabled(enableOrNot);
    }
}

void MainWindow::showAddFinished(unsigned int userNum, QString userName, double userMoney)
{
    qDebug() << "showAddUserFinished() triggered.";
    unselectUser();
    
    nameValueLabel->setText(userName);
    moneyValueLabel->setText(QString::number(userMoney));
    numberValueLabel->setText(QString::number(userNum));
    showAllUser();
}

User* MainWindow::getContentUserPtr()
{
    return userPtrVec.at( tableItemIndexVec.at(contentUserIndex) );
}
    


void setDirs()
{
    /* TODO:
     * Need to check the behavior of this function on Windows system.
     */
    
    QString appDirPath=QCoreApplication::applicationDirPath();
    
    QString userDataPath(appDirPath);
    userDataPath.append('/');
    userDataPath.append("userData.txt");
    
    qDebug() << "The userData file is : " << userDataPath;
    
    #ifdef Q_OS_WIN
    appDirPath.replace(QString('/'), QString('\\'));
    userDataPath.replace(QString('/'), QString('\\'));
    qDebug() << "The programm running on windows system. The userData file is changed to : " << userDataPath;
    #endif
    
    applicationDirPathStr=appDirPath.toStdString();
    userDataPathStr=userDataPath.toStdString();
}