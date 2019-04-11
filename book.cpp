/*
 * % -  есть вложения, нет в наличии
 * ^ -  есть вложения, есть в наличии
 * # -  нет вложений, есть в наличии
 *   -  нет в наличии, нет вложений
*/
#include "book.h"
#include "ui_mainwindow.h"
#include "student.h"
#include "managementbd.h"
#include <QFile>
#include <QTextStream>
#include <QDir>


book::book()
{

}

book::book(QString Name, QString Author, QString Number, QString Nal, QString Otdel, QString Izd, QString Data, QString Price, QString Der, QList<QString> Content)
{
    name = Name;
    author = Author;
    number = Number;
    nal = Nal;
    otdel = Otdel;
    izd = Izd;
    data = Data;
    price = Price;
    der = Der;
    content = Content;
    if(Der != "Учебный фонд")
    {
        path = QApplication::applicationDirPath()+"/res/book/" + Der+"/"+Number+ ".txt";
    }
    else
    {
        path = QApplication::applicationDirPath()+"/res/book/" + Der + "/" + Name + ' '  + Number + ' ' + Author + ".txt";
    }
}

book::book(QString Path)
{
    QFile Book(Path);
    if(!Book.open(QFile::ReadOnly|QFile::Text))
    {
        QString str = "Классу book не удалось найти фаил по пути \"" + Path + "\"";
        throw str;
        return;
    }
    path = Path;
    QTextStream stream(&Book);
    name = stream.readLine();
    author = stream.readLine();
    number = stream.readLine();
    nal  = stream.readLine();
    otdel = stream.readLine();
    izd = stream.readLine();
    data = stream.readLine();
    price = stream.readLine();
    der = stream.readLine();
    QString buffer = stream.readLine();
    while(buffer != "#")
    {
        relate.push_back(buffer);
        buffer = stream.readLine();
    }
    while(!stream.atEnd())
    {
        content.push_back(stream.readLine());
    }
    Book.close();
}

book::book(QString Der, QString Number)
{
    if(Der == "Учебный фонд")
    {
        QString error = "Для учебного фонда стоит выбрать другой  экземпляр";
        throw error;
    }
    QString Path = QApplication::applicationDirPath()+"/res/book/" + Der+"/"+Number+ ".txt";
    QFile Book(Path);
    if(!Book.open(QFile::Text|QFile::ReadOnly))
    {
        QString error = "Классу book не удалось получить доступ к файлу по адресу: \"" + path + "\"";
        throw error;
    }
    QTextStream stream(&Book);
    name = stream.readLine();
    author = stream.readLine();
    number = stream.readLine();
    nal  = stream.readLine();
    otdel = stream.readLine();
    izd = stream.readLine();
    data = stream.readLine();
    price = stream.readLine();
    der = stream.readLine();
    path = Path;
    QString buffer = stream.readLine();

    QString b2 = "#";
    while(buffer != b2)
    {
        relate.push_back(buffer);
        buffer = stream.readLine();
    }
    while(!stream.atEnd())
    {
        content.push_back(stream.readLine());
    }

    Book.close();
}

book::book(QString Der, QString Author, QString Name, QString Number)//конструктор для учебного фонда
{
    QString Path = QApplication::applicationDirPath()+"/res/book/" + Der + "/" + Name + ' '  + Number + ' ' + Author + ".txt";
    QFile Book(Path);
    if(!Book.open(QFile::Text|QFile::ReadOnly))
    {
        QString error = "Классу book не удалось получить доступ к файлу по адресу: \"" + path + "\"";
        throw error;
    }
    QTextStream stream(&Book);
    name = stream.readLine();
    author = stream.readLine();
    number = stream.readLine();
    nal  = stream.readLine();
    otdel = stream.readLine();
    izd = stream.readLine();
    data = stream.readLine();
    price = stream.readLine();
    der = stream.readLine();
    path = Path;
    QString buffer = stream.readLine();

    QString b2 = "#";
    while(buffer != b2)
    {
        relate.push_back(buffer);
        buffer = stream.readLine();
    }
    while(!stream.atEnd())
    {
        content.push_back(stream.readLine());
    }

    Book.close();
}

QString book::getPath(){return path;}

QString book::getAuthor(){return author;}

QString book::getName(){ return name;}

QString book::getNumber(){return number;}

QString book::getNal(){return nal;}

QString book::getOtdel(){return otdel;}

QString book::getIzd(){return izd;}

QString book::getData(){return data;}

QString book::getPrice(){return price;}

QList<QString> book::getContent(){return content;}

QString book::getDer(){return der;}

QList<QString> book::getRelate(){return relate;}

void book::print(QString Path) //печатаем в фаил
{
    QFile file(Path);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QString error = "Классу book не удалось записать данные в файл по адресу: \"" + Path + "\"";
        throw error;
    }
    QTextStream stream(&file);

    stream << name << endl;
    stream << author << endl;
    stream << number << endl;
    stream << nal << endl;
    stream << otdel << endl;
    stream << izd << endl;
    stream << data << endl;
    stream << price << endl;
    stream << der << endl;

    if(!relate.isEmpty())
        for(int i = 0; i < relate.size(); i++)
        {
            stream << relate.at(i) << endl;
        }

    stream << '#';
    for(int i = 0; i < content.size(); i++)
        stream << endl << content.at(i);
    file.close();
}

QList<QString> book::podrobno() //подробная информация
{
    QStringList buffer;
    buffer << author;
    buffer << name;
    buffer << number;
    buffer << nal;
    buffer << otdel;
    buffer << izd;
    buffer << data;
    buffer << price;
    buffer << content;
    buffer << relate;
    return buffer;
}

void book::Delete(bool b) //удалить книгу
{
    if(b)//если удаление происходит через кнопку удалить, то книга изымается у всех пользователей
        for(int i = 0; i < relate.size(); i++)
        {
            QString pathS = relate.at(i);
            Student student(pathS);
            student.DeleteBook(path);
        }
    QFile Book(path);

    ManagementBD::DeleteBookFile(der, author, name, number);

    Book.remove();
}

QStringList book::tablePrint() //информация для таблицы
{
    QStringList list;
    list.push_back(name);
    list.push_back(author);
    list.push_back(number);
    list.push_back(nal);
    list.push_back(otdel);
    list.push_back(izd);
    list.push_back(der);
    return list;
}

void book::addBook() //добавить книгу
{
    QString Path = QApplication::applicationDirPath();     //путь к папке
    QDir mdir(Path);

    if (!mdir.exists("res")) //если отсутствует папка res
        mdir.mkdir("res");   //то создать ее
    Path += "/res";
    mdir.cd(Path);

    if (!mdir.exists("book"))//если отсутствует папка book
        mdir.mkdir("book");  //то создать ее
    Path += "/book";
    mdir.cd(Path);

    if (!mdir.exists(der))   //если отсутствует папка фонда
        mdir.mkdir(der);     //то создать ее
    Path += '/'+der;
    mdir.cd(Path);

    bool b;
    unsigned int nalInt = nal.toInt(&b);
    bool nalb, contentb;
    if(!content.isEmpty()) //если нет содержания
    {
        contentb = true;
        if(nal=="Да"||(b && nalInt != 0)) //если есть книги в библиотеке
        {
            nalb = true;
        }
        else //если нет книг в бибилиотеке
        {
            nalb = false;
        }
    }
    else // если есть содержание
    {
        contentb = false;
        if(nal=="Да"||(b && nalInt != 0)) //если есть книги в библиотеке
        {
            nalb = true;
        }
        else //если нет книг в библиотеке
        {
            nalb = false;
        }
    }

    //обновляем файлы сортировки
    ManagementBD::addBookFile(der, author, name, number, nalb, contentb);

    //печатаем в файл
    print(path);
}

void book::setOtdel(QString Otdel) //изменить отдел
{
    otdel = Otdel;

    print(path);
}

void book::setNal(QString Nal) //изменить наличие
{
    Delete(false);
    nal = Nal;
    addBook();
}

void book::setAuthor(QString Author) //Изменить автора
{
    Delete(false);
    author = Author;
    addBook();
}

void book::setDer(QString Der) //Изменить фонд
{
    Delete(false);
    der = Der;
    addBook();
}

void book::setName(QString Name) //Изменить Название
{
    Delete(false);
    name = Name;
    addBook();
}

void book::setNumber(QString Number) //Изменить Номер
{
    Delete(false);
    number = Number;
    addBook();
}

void book::setIzd(QString Izd) //изменить издательство
{
    izd = Izd;

    print(path);
}

void book::setData(QString Data) //изменить дату
{
    data = Data;

    print(path);
}

void book::setPrice(QString Price) //изменить цену
{
    price = Price;

    print(path);
}

void book::setContent(QStringList list) //изменить содержание
{
    Delete(false);
    content = list;
    addBook();
}

bool book::issuance(QString surname, QString name, QString clas) //выдать книгу
{
    Student student(clas, surname, name);

    if(der == "Учебный фонд")
    {
        if(nal != "0")
        {
            if(student.isBook(path))
                return false;
            relate.push_back(student.getPath());
            int k = nal.toInt()-1;
            setNal(QString::number(k));
            student.addBook(path);
            return true;
        }
        else
            return false;
    }
    else
    {
        if(nal == "Да")
        {
            relate.push_back(student.getPath());
            setNal(student.SNC());
            student.addBook(path);
            return true;
        }
        else
            return false;
    }
}

bool book::seizure(QString surname, QString name, QString clas) //Забрать книгу
{
    Student student(clas, surname, name);
    if(der == "Учебный фонд")
    {
        if(!student.DeleteBook(path))
        {
            return false;
        }
        relate.removeAll(student.getPath());
        setNal(QString::number(nal.toInt()+1));
        return true;
    }
    else
    {
        if(!student.DeleteBook(path))
        {
            return false;
        }
        relate.removeAll(student.getPath());
        setNal("Да");
        return true;
    }
}

bool book::isContent(QString Name) //поверка, есть ли в содержании книга
{
    for(int i = 0; i < content.size(); i++)
    {
        if(content[i].startsWith(Name, Qt::CaseInsensitive))
            return true;
    }
    return false;
}
