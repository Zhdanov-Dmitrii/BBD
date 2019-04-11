 #ifndef BOOK_H
#define BOOK_H

#include <QObject>
#include <QWidget>
#include "student.h"
#include <mainwindow.h>

namespace ui {
class book;
}

class book
{
private:
    QString name;
    QString author;
    QString number;
    QString nal;
    QString otdel;
    QString izd;
    QString data;
    QString price;
    QString der;
    QStringList content;
    QString path;
    QList<QString> relate;
    void print(QString path);

public:
    book();
    book(QString Name, QString Author, QString Number, QString Nal, QString Otdel, QString Izd, QString Data, QString Price, QString Der, QList<QString> Content);
    book(QString Path);
    book(QString Der, QString Number);
    book(QString Der, QString Author, QString Name, QString Number);

    QString getPath();
    QString getAuthor();
    QString getName();
    QString getNumber();
    QString getNal();
    QString getOtdel();
    QString getIzd();
    QString getData();
    QString getPrice();
    QString getDer();
    QList<QString> getContent();
    QList<QString> getRelate();
    QList<QString> podrobno();
    void Delete(bool b);
    void addBook();
    void setOtdel(QString Otdel);
    void setNal(QString Nal);
    void setName(QString Name);
    void setNumber(QString Number);
    void setAuthor(QString Author);
    void setDer(QString Der);
    void setIzd(QString Izd);
    void setData(QString Data);
    void setPrice(QString Price);
    void setContent(QStringList list);
    bool issuance(QString surname, QString name, QString clas);
    bool seizure(QString surname, QString name, QString clas);
    bool isContent(QString Name);
    QStringList tablePrint();
};

#endif // BOOK_H
