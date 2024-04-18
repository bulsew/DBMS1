#include "widget.h"
#include"table.h"
#include <QApplication>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QDebug>



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    vector<tableRows> a1;

//    a1.push_back({"sname",type::VARCHAR,15,constrain::NONE,false,""});
//    a1.push_back({"sno",type::VARCHAR,8,constrain::NONE,false,""});
//    a1.push_back({"ssex",type::VARCHAR,2,constrain::NONE,false,""});
//    Table c(a1,"student");
//    c.dropTable();
    Table c("student");
    c.instertTOTable("'Tim'|'22301161'|'女'");
    c.show();



}

