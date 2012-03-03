#include "func.h"
#include <iostream>



using namespace std;

vector<User*> userPtrVec;
string applicationDirPathStr;
string userDataPathStr;

int strToInt(string numStr)
{
    /**
     * Convert a string to decimal.
     * The string only should be a normal decimal integer.
     */
    
    int result=0;
    for ( unsigned int i=0; ( numStr[i]>='0' && numStr[i]<='9' && i<numStr.size() ); ++i ) {
        result*=10;
        result+=(numStr[i]-'0');
    }
    return result;
}

double strToDouble(string numStr)
{
    /**
     * Convert a string to double float.
     * The string only should be like xxx.xxx
     */
    
    double result=strToInt(numStr);
    int dot=numStr.find('.');
    if (dot==-1)
        return result;
    int divFactor=1;
    for (unsigned int i=++dot; i<numStr.size(); ++i) {
        divFactor*=10;
        result+=( (double)(numStr[i]-'0')/divFactor );
    }
    return result;
}




// About class User:
User::User(unsigned int newNumber, string newName, double newMoney)
{
    number=newNumber;
    name=newName;
    money=newMoney;
}
User::User(int newNumber, string newName, double newMoney)
{
    number=(unsigned int)newNumber;
    name=newName;
    money=newMoney;
}
User::User(string newNumber, string newName, string newMoney)
{
    number=(unsigned int)strToInt(newNumber);
    name=newName;
    money=strToDouble(newMoney);
}


unsigned int User::getNum()
{
    return number;
}

string User::getName()
{
    return name;
}

const char* User::getC_strName()
{
    return name.c_str();
}

double User::getMoney()
{
    return money;
}

int User::consume(float consumeMoney)
{
    /**
     * return 1 means no enough money
     */
    if (money<consumeMoney)
        return 1;
    money-=consumeMoney;
    return writeData();
}

int User::charge(float chargeMoney)
{
    money+=chargeMoney;
    return writeData();
}

int User::changeName(string newName)
{
    name=newName;
    return writeData();
}




// About independent functions:
int readData(string filePath)
{
    /**
     * Read users' file from userData.
     * return 0 means read successfully, -1 means open error, 1 means it's the first time to run the programm.
     */
    
    userPtrVec.reserve(1024);
    
    string wholeLine, tempNum, tempName, tempMoney;
    int start=0, end;
    ifstream userFile(filePath.c_str());
    
    if (userFile.fail()) {
        ofstream newFile(filePath.c_str());
        if (newFile.fail()) {
            return -1;
        } else {
            newFile.close();
            return 1;
        }
    }
    
    User* userPtr=NULL;
    while (!userFile.eof()) {
        getline(userFile, wholeLine);
        if ( (wholeLine.find("//")!=0) && (!wholeLine.empty()) ) {
            end=wholeLine.find('|');
            tempNum=wholeLine.substr(0, end);
            
            start=++end;
            end=wholeLine.find('|', end);
            tempName=wholeLine.substr(start, end-start);
            
            start=++end;
            end=wholeLine.size();
            tempMoney=wholeLine.substr(start, end);
            
            
            userPtr=new User(tempNum, tempName, tempMoney);
            userPtrVec.push_back(userPtr);
            //debug:
            //cout << userPtr->getNum() << endl;
        }
    }
    userFile.close();
    
    
    //Debug:
    /*
    for (unsigned int i=0; i<userPtrVec.size(); ++i) {
        std::cout << userPtrVec.at(i)->getNum() << '|' << userPtrVec.at(i)->getName() << '|' << userPtrVec.at(i)->getMoney() << endl;
    }
    */
    
    return 0;
}
int readData()
{
    /* This function only can be called after called setDirs() */
    return readData(userDataPathStr);
}

int writeData(string filePath)
{
    /**
     * Write users' data to a file.
     * "filePath" should be a absolute path.
     */
    
    ofstream userFile(filePath.c_str());
    if (!userFile.good()) {
        return -1;
    }
    
    userFile
    << "// 这个文件包含了所有用户数据，需要迁移数据到其他计算机时，可以将其复制到程序的目录下方。" << endl
    << "// 尽管您可以手动编辑它，但请尽量不要这样做。安全起见，这个文件应该由主程序来修改。" << endl
    << endl
    << "// 如果您一定要编辑……请记住用户顺序必须按照卡号从小到大排列，且格式为：“卡号|姓名|余额”。"<< endl
    << endl
    << fixed;
    
    for (unsigned int i=0; i!=userPtrVec.size(); i++) {
        userFile << userPtrVec.at(i)->getNum() << '|'
        << userPtrVec.at(i)->getName() << '|'
        << userPtrVec.at(i)->getMoney() << endl;
    }
    
    return 0;
}
int writeData()
{
    /* This function only can be called after called setDirs() */
    return writeData(userDataPathStr);
}


int addUser(unsigned int newNumber, string newName, double newMoney)
{
    /**
     * return 1 means the number is already fulled.
     */
    // TODO:
    User *newUserPtr=new User(newNumber, newName, newMoney);
    bool alreadyInserted=false;
    for (unsigned int i=0; i<userPtrVec.size(); ++i) {
        cout << "userPtrVec.size() is " << userPtrVec.size() << endl;
        if ( newNumber == userPtrVec.at(i)->getNum() ) {
            return 1; // Break the function.
        }
        if ( newNumber < userPtrVec.at(i)->getNum() ) {
            vector<User*>::iterator toAdd=userPtrVec.begin();
            toAdd+=i;
            userPtrVec.insert(toAdd, newUserPtr);
            alreadyInserted=true;
            break; // Break the loop
        }
    }
    if (!alreadyInserted) {
        userPtrVec.push_back(newUserPtr);
    }
    return writeData();
}


int deleteUser(unsigned int userNum)
{
    delete userPtrVec.at(userNum);
    vector<User*>::iterator toDelete=userPtrVec.begin();
    toDelete+=userNum;
    userPtrVec.erase(toDelete);
    return writeData();
}