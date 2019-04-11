#include "student.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "book.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include "managementbd.h"
#include <QDebug>

Student::Student()
{

}

Student::Student(QString path)
{
    QFile student(path);
    if(!student.open(QFile::ReadOnly | QFile::Text))
    {
         QString error = "Классу Student не удается найти файл";
         throw error;
    }
    Path = path;
    QTextStream stream(&student);
    Surname = stream.readLine();
    Name = stream.readLine();
    Class = stream.readLine();
    Data = stream.readLine();
    Phone = stream.readLine();
    Address = stream.readLine();
    while(!stream.atEnd())
    {
        Book.push_back(stream.readLine());
    }
    Count = QString::number(Book.size());
    student.close();
}

Student::Student(QString surname, QString name, QString Clas, QString data, QString phone, QString address)
{
    Name = name;
    Surname = surname;
    Class = Clas;
    Data = data;
    Phone = phone;
    Address = address;
    Path = QApplication::applicationDirPath() +"/res/student/" + Class +' '+Surname + ' ' + Name + ".txt";
}

Student::Student(QString Clas, QString surname, QString name)
{
    Path = QApplication::applicationDirPath() +"/res/student/" + surname + ' ' + name + ' ' + Clas + ".txt";
    qDebug() << Path;
    QFile student(Path);
    if(!student.open(QFile::ReadOnly | QFile::Text))
    {
        QString error = "Классу Student не удается найти файл";
        throw error;
    }
    QTextStream stream(&student);
    Surname = stream.readLine();
    Name = stream.readLine();
    Class = stream.readLine();
    Data = stream.readLine();
    Phone = stream.readLine();
    Address = stream.readLine();
    while(!stream.atEnd())
    {
        Book.push_back(stream.readLine());
    }
    Count = QString::number(Book.size());
    student.close();
}

void Student::Delete()
{
    QFile student(Path);
    student.remove();

    ManagementBD::DeleteStudentFile(Class, Surname, Name);
}

void Student::print()
{
    QFile student(Path);
    if(!student.open(QFile::WriteOnly | QFile::Text))
    {
        QString error = "Классу Student не удается найти файл";
        throw error;
    }
    QTextStream stream(&student);
    stream << Surname << endl << Name << endl << Class << endl << Data << endl << Phone << endl << Address;
    if(!Book.isEmpty())
    {
        for (int i = 0; i < Book.size(); i++)
            stream << endl << Book.at(i);
    }
    student.close();
}

void Student::addStudent()
{
    QString path = QApplication::applicationDirPath();     //путь


    QDir mdir(path);

    if (!mdir.exists("res"))
        mdir.mkdir("res");
    path = path+"/res";
    mdir.cd(path);
    if (!mdir.exists("student"))
        mdir.mkdir("student");
    path = path+"/student/";
    mdir.cd(path);
    path += Surname + ' ' + Name + ' ' + Class + ".txt";
    Path = path;
    ManagementBD::addStudentFile(Class, Surname, Name);
    print();
}

void Student::DeleteStudent()
{
    if(Book.isEmpty())
    {
        Delete();
    }
    else
    {
        QString status = "У пользователя \"" + Surname + ' ' + Name + ' ' + Class +"\" остались в наличии книги!";
        QMessageBox msb;
        msb.setIcon(QMessageBox::Warning);
        msb.setText(status);
        msb.setInformativeText("Хотите вернуть их в библиотеку?\nВ противном случае пользователь удален не будет");
        msb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int ret = msb.exec();
        if (ret == QMessageBox::Yes)
        {
            for(int i = 0; i < Book.size(); i++)
            {
                book a(Book.at(i));
                a.setNal("Да");
                DeleteBook(Book.at(i));
            }
            Delete();
        }
    }
}

bool Student::addBook(QString path)
{
    if(Book.count(path) != 0)
        return false;

    Book.push_back(path);
    Count = QString::number(Book.size());

    print();
    return true;
}

bool Student::DeleteBook(QString path)
{
    if(Book.removeAll(path)==0)
        return false;
    print();
    return true;
}

bool Student::isBook(QString path)
{
    for(auto i : Book)
    {
        if(i == path)
            return true;
    }
    return false;
}

QStringList Student::tablePrint()
{
    QStringList list;
    list.push_back(Surname);
    list.push_back(Name);
    list.push_back(Class);
    list.push_back(Count);
    list.push_back(Data);
    list.push_back(Phone);
    list.push_back(Address);
    return list;
}

QStringList Student::podrobno()
{
    return Book;
}

void Student::setSurname(QString surname)
{
    Delete();
    Surname = surname;
    addStudent();
}

void Student::setName(QString name)
{
    Delete();
    Name = name;
    addStudent();
}

void Student::setClass(QString clas)
{
    Delete();
    Class = clas;
    addStudent();
}

void Student::setData(QString data)
{
    Data = data;
    print();
}

void Student::setPhone(QString phone)
{
    Phone = phone;
    print();
}

void Student::setAddress(QString address)
{
    Address = address;
    print();
}

QString Student::SNC()
{
    QString snc = Surname + ' ' + Name + ' ' + Class;
    return snc;
}

QString Student::getPath()
{
    return Path;
}
