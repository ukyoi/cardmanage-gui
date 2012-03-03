#ifndef FUNC_H
#define FUNC_H

#include <QtCore/QString>
#include <vector>
#include <string>
#include <fstream>



using namespace std;

int strToInt(string numStr); // Should be a pure-digits decmal number;
double strToDouble(string numStr); // Should be a pure-digits decimal .


class User
{
private:
    unsigned int number;
    string name;
    double money;
    
public:
    User(unsigned int newNumber, string newName, double newMoney);
    User(int newNumber, string newName, double newMoney);
    User(string newNumber, string newName, string newMoney);
    
    int consume(float consumeMoney);
    int charge(float chargeMoney);
    int changeName(string newName);
    
    unsigned int getNum();
    string getName();
    const char* getC_strName();
    double getMoney();
};

extern vector<User*> userPtrVec;
extern string applicationDirPathStr;
extern string userDataPathStr;

int readData(string filePath);
int readData();
int writeData(string filePath);
int writeData();

int addUser(unsigned int newNum, string newName, double newMoney);
int addUser(int newNum, string newName, double newMoney);
int deleteUser(unsigned int userNum);

#endif
