#ifndef MANAGEMENTBD_H
#define MANAGEMENTBD_H

#include <QObject>

namespace ui {
class ManagementBD;
}

class ManagementBD
{
public:
    ManagementBD();
    static void DeleteBookFile(QString der, QString author, QString name, QString number);
    static void addBookFile(QString der,QString author, QString name, QString number, bool nal, bool content);
    static QStringList searchBook(QString der, QString author, QString name, QString number, bool nal);

    static void DeleteStudentFile(QString Class, QString Surname, QString Name);
    static void addStudentFile(QString Class, QString Surname, QString Name);
    static QStringList searchStudent(QString Class, QString Surname, QString Name);

private:
    struct numberBook
    {
        QString number;
        QString convention;
    };
    struct nameBook
    {
        QString name;
        QList<numberBook> listNu;
    };
    struct authorBook
    {
        QString author;
        QList<nameBook> listNa;
    };

    static QList<authorBook> readFileSortAuthor(QString path);
    static void printFileSortAuthor(QList<authorBook> listAu, QString path);

    static QList<nameBook> readFileSortName(QString path);
    static void printFileSortName(QList<nameBook> listNa, QString path);

    static QList<numberBook> readFileSortNumber(QString path);
    static void printFileSortNumber(QList<numberBook> listNu, QString path);

    struct surnameStudent
    {
        QString surname;
        QStringList listNa;
    };
    struct classStudent
    {
        QString Class;
        QList<surnameStudent> listSu;
    };

    static QList<classStudent> readFileSortStudent(QString path);
    static void printFileSortStudent(QList<classStudent> listCl, QString path);
};

#endif // MANAGEMENTBD_H
