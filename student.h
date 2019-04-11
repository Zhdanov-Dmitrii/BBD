#ifndef STUDENT_H
#define STUDENT_H

#include <QMainWindow>

class Student
{
private:
    QString Name;
    QString Surname;
    QString Class;
    QString Count;
    QString Data;
    QString Phone;
    QString Address;
    QStringList Book;
    QString Path;

    void Delete();
    void print();
public:
    Student();
    Student(QString path);
    Student(QString surname, QString name,QString Clas, QString data, QString phone, QString address);
    Student(QString Clas, QString surname, QString name);

    void addStudent();
    void DeleteStudent();
    bool addBook(QString path);
    bool DeleteBook(QString path);
    bool isBook(QString path);
    QStringList tablePrint();
    QStringList podrobno();
    void setSurname(QString surname);
    void setName(QString name);
    void setClass(QString clas);
    void setData(QString data);
    void setPhone(QString phone);
    void setAddress(QString address);
    QString SNC();
    QString getPath();
};

#endif // STUDENT_H
