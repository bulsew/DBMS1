#include <QApplication>

#include<string>
#include"table.h"
#include<vector>

using namespace std;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

//    vector<tableRows> roows;
//    roows.push_back(tableRows("student",type::CHAR,20));
//    roows.push_back(tableRows("age",type::INT));
    Table("student").instertTOTable("29|'bdn'","age|student");





    return a.exec();
}
