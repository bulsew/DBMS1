﻿#include "table.h"
static const string DBadd="D:\\testsssbin\\";
Table::Table(const string& tableName):tableName(tableName)
{
    add=DBadd+tableName;
    cout<<add<<endl;
    //未找到该表
    if(!readFromFile(tableName)) cerr<<"Error finding tabel."<<endl;
    line_num=lines.size();
    row_num=0;
    for (const auto& str : rows)
        row_num++;
}

Table::Table(const vector<tableRows>& newTable,const string& tableName):tableName(tableName),rows(newTable)
{
    CreateTable(rows,tableName);
    add=DBadd+tableName;
    line_num=0;
    //rows=newTable;
    row_num=0;
    for (const auto& str : rows)
        row_num++;
}

Table::~Table()
{

}

void Table::test()
{

}

bool Table::dropTable()
{
    QString add_Q=QString::fromStdString(add);
    QDir dir(add_Q);
    if (!dir.exists())
    {
        qDebug() << "Folder does not exist:" << add_Q;
        return false;
    }

    if (dir.removeRecursively())
    {
        qDebug() << "Folder deleted successfully:" << add_Q;
        return true;
    }
    else
    {
        qDebug() << "Error deleting folder:" << add_Q;
        return false;
    }
}

bool Table::instertTOTable(const string& content, const string& correspond)
{
    this_row.clear();
    this_row=rows;
    string newline="";
    vector<string> correspond_v=splitByPipe(correspond);//列名
    vector<string>  content_v=splitByPipe(content);//每列对应内容
    //先解析是否有对应的列要求
    if(correspond!="")//有要求插入（）
    {
        if(content_v.size()!=rows.size()||correspond_v.size()!=rows.size())//数目不一直接报错
        {
            cerr<<"no matching insert error"<<endl;
            return false;
        }
        else
        {
            for(unsigned j =0;j<correspond_v.size();++j)//对每个列名进行检查，不存在相关列名直接报错,将其插入当前列
            {
                unsigned int i=0;
                bool flag=true;
                while (i<rows.size()&&flag)
                {
                    rows[i].rowName==correspond_v[j]?flag=false:++i;
                }
                if(i>=rows.size())
                {
                    cerr<<"no such row!error"<<endl;
                    return false;
                }
                else
                {
                    if(!checkType(this_row[i].rowType,content_v[j]))//检查类型错误
                    {
                        cerr<<"type wrong!error"<<endl;
                        return false;
                    }
                    else
                    {
                        this_row[i].content=content_v[j];
                    }
                }
            }
        }
    }
    else //无要求插入，默认插入
    {
        if(content_v.size()!=rows.size())//数目不一直接报错
        {
            cerr<<" no matching insert error"<<endl;
            return false;
        }
        else
        {
            for(int i=0;i<row_num;++i)
            {
                if(!checkType(this_row[i].rowType,content_v[i]))//检查类型错误
                {
                    cerr<<"type wrong!error"<<endl;
                    return false;
                }
                else
                {
                    this_row[i].content=content_v[i];
                }
            }
        }
    }
    //将当前列数据写成newline插入lines
    for(unsigned int i=0;i<this_row.size();++i)
    {
        newline.append(this_row[i].content);
        if(i<this_row.size()-1)//最后一个属性后不打|
         newline.append("|");
    }
    //插入lines（内存暂存表结构）
    lines.push_back(newline);
    line_num++;
    //是否需要在此处保存？
    saveToFile();
    return true;
}

bool Table::deleteFromTable(delete_mode mode)
{
    if(mode==delete_mode::ALL)//清空整个表
    {
        ofstream file_C(add+"\\content.bin",ios::binary);
        if (!file_C.is_open())
        {
            cerr << "Error opening file " <<endl;
            return false;
        }
        else
        {
            lines.clear();
            file_C.write("",0);
            cout<<"delete successfully!"<<endl;
        }

        file_C.close();
        return true;
    }
    else if (mode==delete_mode::SELECT)//带限制的删除
    {
        return true;
    }
    else return false;

}

bool Table::alterTable(const Table::alter_mode &mode, const Table::alter_class &class_A, const string &content)
{
    if(mode==alter_mode::DROP&&class_A==alter_class::ROW)//删除列
    {

    }
    else if(mode==alter_mode::DROP&&class_A==alter_class::PRIMARY_KRY)//删除主键
    {

    }
    else if(mode==alter_mode::DROP&&class_A==alter_class::CONSTRAIN)//删除约束
    {

    }
    else if(mode==alter_mode::ADD&&class_A==alter_class::ROW)//增加列
    {

    }
    else if(mode==alter_mode::ADD&&class_A==alter_class::CONSTRAIN)//增加约束
    {

    }
//    else if(mode==alter_mode::DROP&&class_A==alter_class::ROW)增加索引
//    {

//    }
    else if(mode==alter_mode::RENAME&&class_A==alter_class::ROW)
    {

    }
    else if(mode==alter_mode::RENAME&&class_A==alter_class::TABLE)
    {

    }
    else if(mode==alter_mode::MODIFY&&class_A==alter_class::ROW)
    {

    }
    else if(mode==alter_mode::ALTER&&class_A==alter_class::ROW)
    {

    }
    else
    {
        cerr<<"wrong mode!!"<<endl;
        return false;
    }
    return true;
}

bool Table::updateTable(const vector<string>&rowname,const vector<string>&goal/*,constrains*/)
{
    //传入数据不符合规范，直接报错
    if(rowname.size()!=goal.size())
    {
        cerr<<"wrong data!"<<endl;
        readFromFile(tableName);//防止lines存在已经修改数据
        return false;
    }
    unsigned int size=rowname.size();
    for(unsigned int i=0;i<size;++i)//对每个项目（set）进行检查
    {
        //先查找是否有这样的列checkType(rows[j].rowType,goal[i])
        unsigned int j=0;
        for(;j<(unsigned int)row_num;++j)
        {
            if(rows[j].rowName==rowname[i])//找到了这样的列
            {
                if(checkType(rows[j].rowType,goal[i]))//检查update的目标类型是否合规
                {
                    break;
                }
                else
                {
                    cerr<<"wrong type!"<<endl;
                    readFromFile(tableName);//防止lines存在已经修改数据
                    return false;
                }
            }
        }
        if(j>=(unsigned int)row_num)//没有找到这样的列
        {
            cerr<<"no such row!"<<endl;
            readFromFile(tableName);//防止lines存在已经修改数据
            return false;
        }
        //再查找where限制的行，删除这个行并将content更新到新行上
        //注意，每个return false前加一个readFromFile(tableName);
        /*
         *
         *
         *
         *
         *
         *
         *
         *
         */

    }
    //保存
    saveToFile();
    return true;
}

void Table::checkLines(const string& in)//获得一列到类中，进行操作
{
    this_row.clear();
    vector<string> contents=splitByPipe(in);
    this_row=rows;
    for(unsigned int i=0;i<rows.size();++i)
    {
        this_row[i].content=contents[i];
    }
}
template<class inPut>
bool Table::translateFromConstrainMessageAndCheck(const string& message,const type&type,const inPut &inputs)
{
    istringstream iss(message);
    string line;
    int n;
    string content;
    vector<std::string> strings;

    // 读取第一行：整数n
    if (!(iss >> n))
    {
        std::cerr << "Invalid input: could not read the first line (integer n)." << std::endl;
        return false;
    }

    // 忽略第一行后面的换行符
    iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 读取第二行：字符串content
    if (!std::getline(iss, content))
    {
        std::cerr << "Invalid input: could not read the second line (content)." << std::endl;
        return false;
    }

    // 读取第三行及以后的n个字符串
    for (int i = 0; i < n && std::getline(iss, line); ++i)
    {
        strings.push_back(line);
    }

    // 检查是否读取了足够的字符串
    if (strings.size() != (unsigned int)n)
    {
        std::cerr << "Invalid input: did not read the expected number of strings." << std::endl;
        return false;
    }

    //对每一个限制进行处理
    vector<bool>checks;
    for(unsigned int i=0;i<(unsigned int)n;++i)
    {
        vector<string> a=splitByPipe(strings[i]);
        if(a[0]=="daYu")
        {
            if(!checkType(type,a[1]))
            {
                cerr<<"constrain type not match！"<<endl;
                return false;
            }

            daYu<inPut> c(static_cast<inPut>(stoi(a[1])));
            checks.push_back(c(inputs));
        }
        else if(a[0]=="xiaoYu")
        {

            if(!checkType(type,a[1]))
            {
                cerr<<"constrain type not match！"<<endl;
                return false;
            }

            xiaoYu<inPut> c(static_cast<inPut>(stoi(a[1])));
            checks.push_back(c(inputs));
        }
        else if(a[0]=="dengYu")
        {
            if(!checkType(type,a[1]))
            {
                cerr<<"constrain type not match！"<<endl;
                return false;
            }

            dengYu<inPut> c(static_cast<inPut>(stoi(a[1])));
            checks.push_back(c(inputs));

        }
        else if(a[0]=="xiaoYuDengYu")
        {
            if(!checkType(type,a[1]))
            {
                cerr<<"constrain type not match！"<<endl;
                return false;
            }
            xiaoYuDengYu<inPut> c(static_cast<inPut>(stoi(a[1])));
            checks.push_back(c(inputs));
        }
        else if(a[0]=="daYuDengYu")
        {

            if(!checkType(type,a[1]))
            {
                cerr<<"constrain type not match！"<<endl;
                return false;
            }
            daYuDengYu<inPut> c(static_cast<inPut>(stoi(a[1])));
            checks.push_back(c(inputs));
        }
        else if(a[0]=="between")
        {
            if(!checkType(type,a[1])||!checkType(type,a[2]))
            {
                cerr<<"constrain type not match！"<<endl;
                return false;
            }
            between<inPut> c(static_cast<inPut>(stoi(a[1])),static_cast<inPut>(stoi(a[2])));
            checks.push_back(c(inputs));
        }
        else if(a[0]=="in")
        {
            bool flag=true;
            vector<inPut> list;
            for(unsigned int j=2;j<=(unsigned int)n+1;++j)
            {
                flag*=checkType(type,a[j]);
                list.push_back(a[j]);
            }
            if(!flag)
            {
                cerr<<"constrain type not match！"<<endl;
                return false;
            }
            in<inPut> c(list);
            checks.push_back(c(inputs));
        }
        else if(a[0]=="notIn")
        {
            bool flag=true;
            vector<inPut> list;
            for(unsigned int j=2;j<=(unsigned int)n+1;++j)
            {
                flag*=checkType(type,a[j]);
                list.push_back(a[j]);
            }
            if(!flag)
            {
                cerr<<"constrain type not match！"<<endl;
                return false;
            }
            notIn<inPut> c(list);
            checks.push_back(c(inputs));
        }
        else
        {
            cerr<<"no such constrain"<<endl;
            return false;
        }
    }
    return evaluate(content,checks);
}



bool Table::CreateTable(const vector<tableRows>& newTable,const string& tableName)
{
    QDir dir;
    //要改成常量
    QString tableDirPath = QString::fromStdString(DBadd+tableName);
    string tableDirPath1=DBadd+tableName;
    //创建表文件夹,文件夹内存放表相关信息文件
    if (!dir.exists(tableDirPath))
    {
          // 如果不存在，创建文件夹
          if (dir.mkpath(tableDirPath))
          {
              qDebug() << "Folder created successfully at" << tableDirPath;
          }
          else
          {
              qDebug() << "Error creating folder.";
              return false;
          }
      }
    else
    {
          qDebug() << "Folder already exists at" << tableDirPath;
           return false;
    }

//    //处理newTable结构体数据
//    vector<string> attribute;//属性值
//    cout<<"done"<<endl;
//    for (const auto& str : newTable)
//    {
//        attribute.push_back(str.rowName);
//        //
//        //
//        //
//        //
//        //
//        //
//        //
//        //
//        //
//        //
//    }

    //创建属性文件
    ofstream file_A(tableDirPath1+"\\attribute.bin",ios::binary);
    //ofstream file_A("aaaaaaa.bin",ios::binary);
    if (file_A.is_open())
    {
        cout<<"createFile"+tableDirPath1+"\\attribute.bin"<<endl;
        for (const auto& s: newTable)
        {
            file_A << s.rowName << "|"
                   << s.rowType << "|"
                   << s.isPrimaryKey << "|"
                  << s.forignKeyName << "|"
                << s.forignKeyTable << "|"
                   << s.default_content<< "|"
                      << s.constrainMessage<< "|"
                         <<  "\n";
        }
        file_A.close();
    }
    else
    {
        cerr << "Error opening file " <<tableDirPath1+"\\attribute"<<endl;
        return false;
    }
    //创建表文件
    ofstream file_C(tableDirPath1+"\\content.bin",ios::binary);
    if (file_C.is_open())
    {
        cout<<"创建文件"+tableDirPath1+"\\content.bin"<<endl;
        file_C.close();
    }
    else
    {
        cerr << "Error opening file " <<endl;
        return false;
    }
    return true;
}

bool Table::saveToFile(save_mode mode)
{
    //写入表中数据
    if(mode==save_mode::BOTH||mode==save_mode::CONTENT)
    {
        ofstream file_C(add+"\\content.bin",ios::binary);
        if (!file_C.is_open())
        {
            cerr << "Error opening file " <<endl;
            return false;
        }
        for (const auto& str : lines)
        {
            file_C.write(str.c_str(), str.size());
            file_C.write("\n",1); // 写入分行符
        }
        file_C.close();
    }
    //写入属性
//    if(mode==save_mode::BOTH||mode==save_mode::ATTRIBUTE)
//    {
//        ofstream file_A(add+"\\attribute.bin",ios::binary);
//        if (!file_A.is_open())
//        {
//            cerr << "Error opening file " <<endl;
//            return false;
//        }
//        for (const auto& s : rows)//直接写入结构体
//        {
//            file_A.write(reinterpret_cast<const char*>(&s), sizeof(struct tableRows));
//        }
//        file_A.close();
//    }
    if(mode==save_mode::BOTH||mode==save_mode::ATTRIBUTE)
    {
        ofstream file_A(add+"\\attribute.bin",ios::binary);
        if (!file_A.is_open())
        {
            cerr << "Error opening file " <<endl;
            return false;
        }
        for (const auto& s : rows)//直接写入结构体
        {
            file_A << s.rowName << "|"
                   << s.rowType << "|"
                   << s.isPrimaryKey << "|"
                  << s.forignKeyName << "|"
                << s.forignKeyTable << "|"
                   << s.default_content<< "|"
                      << s.constrainMessage<< "|"
                         <<  "\n";
        }
        file_A.close();
    }
    return true;
}

bool Table::readFromFile(const string& tableName)
{
    const char delimiter = '\n';
    QDir dir;
    //要改成常量
    QString tableDirPath = QString::fromStdString(DBadd+tableName);
    //表不存在返回false
    if (!dir.exists(tableDirPath))
    {
        return false;
    }
    //创建文件输入流
    //1.读取表中数据
    lines.clear();
    ifstream file_C(add+"\\content.bin",ios::binary);
    if (file_C.is_open())
    {
        string str;
        while (std::getline(file_C, str, delimiter))
        { // 以分隔符为界读取字符串
            lines.push_back(str);
        }
        file_C.close();
    }
    else
    {
        cerr << "Error opening file: " << add+"\\content.bin" << endl;
        return false;
    }
    //2.读取表中属性
//    rows.clear();
//    ifstream file_A(add+"\\attribute.bin",ios::binary);
//    if (file_A.is_open())
//    {
//        tableRows tempStruct;
//        while (file_A.read(reinterpret_cast<char*>(&tempStruct), sizeof(tableRows)))
//        {
//            cout<<tempStruct.rowName<<endl;
//            //rows.push_back(tempStruct);
//        }
//        file_A.close();
//    }
//    else
//    {
//        cerr << "Error opening file: " << add+"\\content.bin" << endl;
//        return false;
//    }
    //2.读取表中属性
    rows.clear();
    ifstream file_A(add+"\\attribute.bin",ios::binary);
    if (file_A.is_open())
    {
        string line;
        while (getline(file_A, line))
        {
            tableRows row;
            //字符流
            stringstream ss(line);
            string item;
            //rowName
            getline(ss, item, '|');
            row.rowName = item;
            //rowType
            getline(ss, item, '|');
            row.rowType = static_cast<type>(stoi(item));
            //isPrimaryKey
            getline(ss, item, '|');
            row.isPrimaryKey = static_cast<bool>(stoi(item));
            //forignKeyName
            getline(ss, item, '|');
            row.forignKeyName = item;
            //forignKeyTable
            getline(ss, item, '|');
            row.forignKeyTable = item;
            //default_content
            getline(ss, item, '|');
            row.default_content = item;
            //constrainMessage
            getline(ss, item, '|');
            row.constrainMessage = item;
            //row.rowConstrain = static_cast<constrain>(stoi(item));

            rows.push_back(row);
        }
        file_A.close();
        cout << "Data read from file successfully." << endl;
    }
    else
    {
        cerr << "Unable to open file: "  << endl;
    }
    return true;
}
// 打印函数
void Table::show() {
    // 打印表头
    for (const auto& row : rows) {
        std::cout << row.rowName << "\t";
    }
    std::cout << std::endl;

    // 打印分隔线
    for (size_t i = 0; i < rows.size(); ++i) {
        std::cout << "----\t";
    }
    std::cout << std::endl;

    // 打印每一行数据
    for (const auto& line : lines) {
        std::istringstream iss(line);
        std::string value;
        while (std::getline(iss, value, '|')) {
            std::cout << value << "\t";
        }
        std::cout << std::endl;
    }
}
