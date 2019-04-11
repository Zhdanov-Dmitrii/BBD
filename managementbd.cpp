/* Условные обозначения в названиях книг:
 * % -  есть вложения, нет в наличии
 * ^ -  есть вложения, есть в наличии
 * # -  нет вложений, есть в наличии
 *   -  нет в наличии, нет вложений
*/

#include "managementbd.h"
#include <ui_mainwindow.h>
#include <QFile>
#include <QTextStream>
#include <QtAlgorithms>
#include "book.h"
#include <QDebug>


ManagementBD::ManagementBD()
{

}

QList<ManagementBD::authorBook> ManagementBD::readFileSortAuthor(QString path)
{
    QList<authorBook> listAu; //список авторов

    //Открываем фаил
    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        if(!file.open(QFile::WriteOnly | QFile::Text))
        {
            QString error = "Не удалось открыть базу данных по адресу\""+path+"\"";
            throw error;
        }
        file.close();
        return listAu;
    }

    QTextStream stream(&file);

    int i = -1, j = -1; //счетчики
    QString buffer; //переменная для чтения
    //Читаем фаил
    while(!stream.atEnd())
    {
        buffer = stream.readLine();
        if(buffer.startsWith("\t\t"))
        {
            buffer = buffer.trimmed();
            numberBook buf;
            QChar t = buffer.at(buffer.size()-1);
            if(t == '#' || t == '^' || t == '%')
            {
                buffer.remove(t);
                buf.number = buffer;
                buf.convention = t;
            }
            else
                buf.number = buffer;
            listAu[i].listNa[j].listNu.push_back(buf);
        }
        else if(buffer.startsWith("\t")) {
            buffer = buffer.trimmed();
            j++;
            nameBook buf;
            buf.name = buffer;
            listAu[i].listNa.push_back(buf);
        }
        else {
            buffer = buffer.trimmed();
            i++;
            j = -1;
            authorBook buf;
            buf.author = buffer;
            listAu.push_back(buf);
        }
    }

    file.close();

    return listAu;
}

void ManagementBD::printFileSortAuthor(QList<authorBook> listAu, QString path)
{
    QFile file(path);

    if(listAu.size() == 0)
    {
        file.remove();
        return;
    }

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QString error = "Не удалось открыть базу данных для записи по адресу\""+path+"\"";
        throw error;
    }

    QTextStream stream(&file);

    //печатаем
    for(int i = 0; i < listAu.size(); i++)
    {
        stream << listAu.at(i).author << endl;
        for(int j = 0; j < listAu.at(i).listNa.size(); j++)
        {
            stream << "\t" << listAu.at(i).listNa.at(j).name << endl;
            for(int h = 0; h < listAu.at(i).listNa.at(j).listNu.size(); h++)
            {
                stream << "\t\t" << listAu.at(i).listNa.at(j).listNu.at(h).number
                            << listAu.at(i).listNa.at(j).listNu.at(h).convention << endl;
            }
        }
    }
    file.close();
}

QList<ManagementBD::nameBook> ManagementBD::readFileSortName(QString path)
{
    QList<nameBook> listNa;

    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        if(!file.open(QFile::WriteOnly | QFile::Text))
        {
            QString error = "Не удалось открыть базу данных по адресу\""+path+"\"";
            throw error;
        }
        file.close();
        return listNa;
    }

    QTextStream stream(&file);

    int j = -1; //счетчик
    //читаем фаил
    while(!stream.atEnd())
    {
        QString buffer = stream.readLine();
        if(buffer.startsWith('\t'))
        {
            buffer = buffer.trimmed();
            numberBook buf;
            QChar t = buffer.at(buffer.size()-1);
            if(t == '#' || t == '^' || t == '%')
            {
                buffer.remove(t);
                buf.number = buffer;
                buf.convention = t;
            }
            else
                buf.number = buffer;
            listNa[j].listNu.push_back(buf);
        }
        else
        {
            buffer = buffer.trimmed();
            j++;
            nameBook buf;
            buf.name = buffer;
            listNa.push_back(buf);
        }
    }
    file.close();

    return listNa;
}

void ManagementBD::printFileSortName(QList<nameBook> listNa, QString path)
{
    QFile file(path);

    if(listNa.size() == 0)
    {
        file.remove();
        return;
    }

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QString error = "Не удалось открыть базу данных для записи по адресу\""+path+"\"";
        throw error;
    }

    QTextStream stream(&file);

    //печатаем
    for(int i = 0; i < listNa.size(); i++)
    {
        stream << listNa.at(i).name << endl;
        for(int j = 0; j < listNa.at(i).listNu.size(); j++)
        {
            stream << '\t' << listNa.at(i).listNu.at(j).number
                    << listNa.at(i).listNu.at(j).convention << endl;
        }
    }
    file.close();
}

QList<ManagementBD::numberBook> ManagementBD::readFileSortNumber(QString path)
{
    QList<numberBook> listNu; //список номеров
    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        if(!file.open(QFile::WriteOnly | QFile::Text))
        {
            QString error = "Не удалось открыть базу данных по адресу\""+path+"\"";
            throw error;
        }
        file.close();
        return listNu;
    }

    QTextStream stream(&file);


    //читаем файл
    while(!stream.atEnd())
    {
        QString buffer = stream.readLine();
        numberBook buf;
        QChar t = buffer.at(buffer.size()-1);
        if(t == '#' || t == '^' || t == '%')
        {
            buffer.remove(t);
            buf.number = buffer;
            buf.convention = t;
        }
        else
            buf.number = buffer;
        listNu.push_back(buf);

    }
    file.close();
    return listNu;
}

void ManagementBD::printFileSortNumber(QList<numberBook> listNu,QString path)
{
    QFile file(path);

    if(listNu.size() == 0)
    {
        file.remove();
        return;
    }

    //открываем фаил
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QString error = "Не удалось открыть базу данных для записи по адресу\""+path+"\"";
        throw error;
    }

    QTextStream stream(&file);

    //печатаем
    for(int i = 0; i < listNu.size(); i++)
        stream << listNu.at(i).number << listNu.at(i).convention << endl;
    file.close();
}

void ManagementBD::addBookFile(QString der,QString author, QString name, QString number, bool nal, bool content)
{
    //учебный фонд хранит данные так же, как хранятся пользователи, по этому алгоритмы совпадают
    //Class == author
    //Surname == name
    //Name == number
    if(der != "Учебный фонд")
    {
        QString path1 = QApplication::applicationDirPath() + "/res/book/" + der + '/';
        QString path = path1 + "FileSortAuthor.txt";

        //Калибруем условные обозначения
        QString Convention;
        if(content)
        {
            if(nal)
                Convention = '^';
            else
                Convention = '%';
        }
        else
        {
            if(nal)
                Convention = '#';
        }


        //переходим к первому файлу
        path = path1 + "FileSortNumber.txt";

        QList<numberBook> listNu1 = readFileSortNumber(path);

        numberBook newNu1;
        newNu1.number = number;
        newNu1.convention = Convention;
        QList<numberBook>::iterator itNu1;
        itNu1 = qBinaryFind(listNu1.begin(), listNu1.end(), newNu1,
                            [](numberBook a, numberBook b){return a.number.toLower() < b.number.toLower();});
        if(itNu1 != listNu1.end() && itNu1->number.toLower() == number.toLower())
        {
            QString error = "Книга с таким номером уже существует";
            throw error;
        }
        itNu1 = qUpperBound(listNu1.begin(), listNu1.end(), newNu1,
                           [](numberBook a, numberBook b){return a.number.toLower() < b.number.toLower();});
        listNu1.insert(itNu1, newNu1);
        printFileSortNumber(listNu1, path);

        //читаем второй фаил
        path = path1 + "FileSortAuthor.txt";
        QList<ManagementBD::authorBook> listAu = readFileSortAuthor(path);

        authorBook bufAu; // структура для сравнения
        bufAu.author = author;
        QList<authorBook>::iterator itAu = qLowerBound(listAu.begin(), listAu.end(), bufAu, [](authorBook a, authorBook b){return a.author.toLower() < b.author.toLower();}); // поиск автора
        if(itAu != listAu.end() && itAu->author.toLower() == author.toLower())
        {
            nameBook bufNa; // структура для сравнения
            bufNa.name = name;
            QList<nameBook>::iterator itNa = qLowerBound(itAu->listNa.begin(), itAu->listNa.end(), bufNa, [](nameBook a, nameBook b){return a.name.toLower() < b.name.toLower();}); // поиск для сравнения
            if(itNa != itAu->listNa.end() && itNa->name.toLower() == name.toLower())
            {
                numberBook newNu;
                newNu.number = number;
                newNu.convention = Convention;
                QList<numberBook>::iterator itNu;
                itNu = qUpperBound(itNa->listNu.begin(), itNa->listNu.end(), newNu,
                                   [](numberBook a, numberBook b){return a.number.toLower() < b.number.toLower();});
                itNa->listNu.insert(itNu, newNu);
            }
            else
            {
                numberBook newNu;
                newNu.number = number;
                newNu.convention = Convention;

                nameBook newNa;
                newNa.name = name;
                newNa.listNu.push_back(newNu);

                itNa = qUpperBound(itAu->listNa.begin(), itAu->listNa.end(), newNa,
                                   [](nameBook a, nameBook b){return a.name.toLower() < b.name.toLower();});
                itAu->listNa.insert(itNa, newNa);
            }
        }
        else
        {
            numberBook newNu;
            newNu.number = number;
            newNu.convention = Convention;

            nameBook newNa;
            newNa.name = name;
            newNa.listNu.push_back(newNu);

            authorBook newAu;
            newAu.author = author;
            newAu.listNa.push_back(newNa);

            itAu = qUpperBound(listAu.begin(), listAu.end(), newAu,
                                [](authorBook a, authorBook b){return a.author.toLower() < b.author.toLower();});
            listAu.insert(itAu, newAu);
        }

        printFileSortAuthor(listAu, path);


        //переходим к следующему файлу
        path = path1 + "FileSortName.txt";

        QList<nameBook> listNa = readFileSortName(path);

        //вставляем элемент
        nameBook bufNa; // структура для сравнения
        bufNa.name = name;
        QList<nameBook>::iterator itNa = qLowerBound(listNa.begin(), listNa.end(), bufNa,
                                                     [](nameBook a, nameBook b){return a.name.toLower() < b.name.toLower();});
        if(itNa != listNa.end() && itNa->name.toLower() == name.toLower())
        {
            numberBook newNu;
            newNu.number = number;
            newNu.convention = Convention;
            QList<numberBook>::iterator itNu;
            itNu = qUpperBound(itNa->listNu.begin(), itNa->listNu.end(), newNu,
                               [](numberBook a, numberBook b){return a.number.toLower() < b.number.toLower();});
            itNa->listNu.insert(itNu, newNu);
        }
        else
        {
            numberBook newNu;
            newNu.number = number;
            newNu.convention = Convention;

            nameBook newNa;
            newNa.name = name;
            newNa.listNu.push_back(newNu);

            itNa = qUpperBound(listNa.begin(), listNa.end(), newNa,
                               [](nameBook a, nameBook b){return a.name.toLower() < b.name.toLower();});
            listNa.insert(itNa, newNa);
        }

        printFileSortName(listNa, path);
    }
    else
    {
        QString path = QApplication::applicationDirPath() + "/res/book/" + der + "/fileSortBook.txt";

        QList<classStudent> listCl = readFileSortStudent(path);

        QList<classStudent>::iterator itCl;
        classStudent bufCl;//структура для поиска
        bufCl.Class = author;
        itCl = qLowerBound(listCl.begin(), listCl.end(), bufCl,
                           [](classStudent a, classStudent b){return a.Class.toLower() < b.Class.toLower();});
        if(itCl != listCl.end() && itCl->Class == author)
        {
            QList<surnameStudent>::iterator itSu;
            surnameStudent bufSu;//структура для поиска
            bufSu.surname = name;
            itSu = qLowerBound(itCl->listSu.begin(), itCl->listSu.end(), bufSu,
                               [](surnameStudent a, surnameStudent b){return a.surname.toLower() < b.surname.toLower();});
            if(itSu != itCl->listSu.end() && itSu->surname == name)
            {
                QStringList::iterator itNa;
                itNa = qLowerBound(itSu->listNa.begin(), itSu->listNa.end(), number,
                                   [](QString a, QString b){return a.toLower() < b.toLower();});
                if(itNa != itSu->listNa.end() && *(itNa) == number)
                {
                    QString error = "Данный пользователь уже существует";
                    throw error;
                }
                itNa = qUpperBound(itSu->listNa.begin(), itSu->listNa.end(), number,
                                   [](QString a, QString b){return a.toLower() < b.toLower();});
                itSu->listNa.insert(itNa, number);
            }
            else
            {
                itSu = qUpperBound(itCl->listSu.begin(), itCl->listSu.end(), bufSu,
                                   [](surnameStudent a, surnameStudent b){return a.surname.toLower() < b.surname.toLower();});
                bufSu.listNa.push_back(number);
                itCl->listSu.insert(itSu, bufSu);
            }
        }
        else
        {
            surnameStudent bufSu;
            bufSu.surname = name;
            bufSu.listNa.push_back(number);
            itCl = qUpperBound(listCl.begin(), listCl.end(), bufCl,
                               [](classStudent a, classStudent b){return a.Class.toLower() < b.Class.toLower();});
            bufCl.listSu.push_back(bufSu);
            listCl.insert(itCl, bufCl);
        }

        printFileSortStudent(listCl, path);
    }
}

void ManagementBD::DeleteBookFile(QString der, QString author, QString name, QString number)
{
    //учебный фонд хранит данные так же, как хранятся пользователи, по этому алгоритмы совпадают
    //Class == author
    //Surname == name
    //Name == number
    if(der != "Учебный фонд")
    {
        QString path1 = QApplication::applicationDirPath() + "/res/book/" + der + '/';
        QString path = path1 + "FileSortAuthor.txt";
        QList<authorBook> listAu = readFileSortAuthor(path); //список авторов

        QList<authorBook>::iterator itAu;
        authorBook bufAu;
        bufAu.author = author;
        itAu = qBinaryFind(listAu.begin(), listAu.end(), bufAu,
                           [](authorBook a, authorBook b){return a.author.toLower() < b.author.toLower();});
        if(itAu != listAu.end() && itAu->author.toLower() == author.toLower())
        {
            QList<nameBook>::iterator itNa;
            nameBook bufNa;
            bufNa.name  = name;
            itNa = qBinaryFind(itAu->listNa.begin(), itAu->listNa.end(), bufNa,
                               [](nameBook a, nameBook b){return a.name.toLower() < b.name.toLower();});
            if(itNa != itAu->listNa.end() && itNa->name.toLower() == name.toLower())
            {
                QList<numberBook>::iterator itNu;
                numberBook bufNu;
                bufNu.number = number;
                itNu = qBinaryFind(itNa->listNu.begin(), itNa->listNu.end(), bufNu,
                                   [](numberBook a, numberBook b){return a.number.toLower() < b.number.toLower();});
                if(itNu != itNa->listNu.end() && itNu->number.toLower() == number.toLower())
                {
                    itNa->listNu.erase(itNu);
                    if(itNa->listNu.isEmpty())
                    {
                        itAu->listNa.erase(itNa);
                        if(itAu->listNa.isEmpty())
                        {
                            listAu.erase(itAu);
                        }
                    }
                }
                else
                {
                    QString error = "Не удалось удалить книгу\nНомер:" + number;
                    throw error;
                }
            }
            else
            {
                QString error = "Не удалось удалить книгу\nНазвание:" + name;
                throw error;
            }
        }
        else
        {
            QString error = "Не удалось удалить книгу\nАвтор:" + author;
            throw error;
        }
        printFileSortAuthor(listAu, path);


        //переходим к следующему файлу
        path = path1 + "FileSortName.txt";

        QList<nameBook> listNa = readFileSortName(path); // список названий

        QList<nameBook>::iterator itNa;
        nameBook bufNa;
        bufNa.name = name;
        itNa = qBinaryFind(listNa.begin(), listNa.end(), bufNa,
                           [](nameBook a, nameBook b){return a.name.toLower() < b.name.toLower();});

        if(itNa != listNa.end() && itNa->name.toLower() == name.toLower())
        {
            QList<numberBook>::iterator itNu;
            numberBook bufNu;
            bufNu.number = number;
            itNu = qBinaryFind(itNa->listNu.begin(), itNa->listNu.end(), bufNu,
                               [](numberBook a, numberBook b){return a.number.toLower() < b.number.toLower();});
            if(itNu != itNa->listNu.end() && itNu->number.toLower() == number.toLower())
            {
                itNa->listNu.erase(itNu);
                if(itNa->listNu.isEmpty())
                {
                    listNa.erase(itNa);
                }
            }
            else
            {
                QString error = "Не удалось удалить книгу \nНомер:" + number;
                throw error;
            }
        }
        else
        {
            QString error = "Не удалось удалить книгу \nНазвание:" + name;
            throw error;
        }

        printFileSortName(listNa, path);


        //переходим к последнему файлу
        path = path1 + "FileSortNumber.txt";

        QList<numberBook> listNu = readFileSortNumber(path); //список номеров

        numberBook bufNu; //структура для сравнения
        bufNu.number = number;
        QList<numberBook>::iterator itNu = qBinaryFind(listNu.begin(), listNu.end(), bufNu, [](numberBook a, numberBook b){return a.number.toLower() < b.number.toLower();});
        if(itNu != listNu.end() && itNu->number.toLower() == number.toLower())
            listNu.erase(itNu);

        printFileSortNumber(listNu, path);
    }
    else
    {
        QString path = QApplication::applicationDirPath() + "/res/book/" + der + "/" + "fileSortBook.txt";
        QList<classStudent> listCl = readFileSortStudent(path);

        QList<classStudent>::iterator itCl;
        classStudent bufCl;//структура для поиска
        bufCl.Class = author;
        itCl = qBinaryFind(listCl.begin(), listCl.end(), bufCl,
                           [](classStudent a, classStudent b){return a.Class.toLower() < b.Class.toLower();});

        if(itCl != listCl.end() && itCl->Class.toLower() == author.toLower())
        {
            QList<surnameStudent>::iterator itSu;
            surnameStudent bufSu;//структура для поиска
            bufSu.surname = name;
            itSu = qBinaryFind(itCl->listSu.begin(), itCl->listSu.end(), bufSu,
                               [](surnameStudent a, surnameStudent b){return a.surname.toLower() < b.surname.toLower();});

            if(itSu != itCl->listSu.end() && itSu->surname.toLower() == name.toLower())
            {
                QStringList::iterator itNa;
                itNa = qBinaryFind(itSu->listNa.begin(), itSu->listNa.end(), number,
                                   [](QString a, QString b){return a.toLower() < b.toLower();});
                QString bufNa = *itNa;
                if(itNa != itSu->listNa.end() && bufNa.toLower() == number.toLower())
                {
                    itSu->listNa.erase(itNa);
                    if(itSu->listNa.isEmpty())
                    {
                        itCl->listSu.erase(itSu);
                        if(itCl->listSu.isEmpty())
                        {
                            listCl.erase(itCl);
                        }
                    }
                }
                else
                {
                    QString error = "Не найден пользователь";
                    throw error;
                }
            }
            else
            {
                QString error = "Не найден пользователь";
                throw error;
            }
        }
        else
        {
            QString error = "Не найден пользователь";
            throw error;
        }


        printFileSortStudent(listCl, path);
    }
}

QStringList ManagementBD::searchBook(QString der, QString author, QString name, QString number, bool isNal)
{

    if(der != "Учебный фонд")
    {
        QString path = QApplication::applicationDirPath() + "/res/book/" + der + '/';
        QStringList res;
        if(!author.isEmpty()) // если указан автор
        {
            path += "FileSortAuthor.txt";

            QList<authorBook> listAu = readFileSortAuthor(path);

            authorBook bufAu; //структура для поиска
            bufAu.author = author;

            //поиск
            QList<authorBook>::iterator itAu = qLowerBound(listAu.begin(), listAu.end(), bufAu,
                                  [](authorBook a, authorBook b){return a.author.toLower() < b.author.toLower();});

            while(itAu != listAu.end())
            {
                if(!itAu->author.startsWith(author, Qt::CaseInsensitive))
                    break;

                if(!name.isEmpty()) // если указано название
                {
                    for(QList<nameBook>::iterator itNa = itAu->listNa.begin(); itNa != itAu->listNa.end(); itNa++)
                    {
                        if(itNa->name.startsWith(name, Qt::CaseInsensitive)) // если совпало название
                        {
                            if(number.isEmpty())//если указан номер
                            {
                                numberBook bufNu;
                                bufNu.number = number; //структура для поиска
                                QList<numberBook>::iterator itNu = qLowerBound(itNa->listNu.begin(), itNa->listNu.end(), bufNu,
                                                             [](numberBook a, numberBook b){return a.number.toLower() < b.number.toLower();});
                                while(itNu != itNa->listNu.end())
                                {
                                    bool nal = itNu->convention == "#" || itNu->convention == "^";
                                    if(!itNu->number.startsWith(number, Qt::CaseInsensitive)) // если не совпали номера
                                        break;
                                    if(!nal && !isNal)
                                    {
                                        itNu++;
                                        continue;
                                    }
                                    res.push_back(QApplication::applicationDirPath() + "/res/book/" + der + "/" +  itNu->number + ".txt");
                                    itNu++;
                                }
                            }
                            else // если не указан номер
                            {
                                for(QList<numberBook>::iterator itNu = itNa->listNu.begin(); itNu != itNa->listNu.end(); itNu++)
                                {
                                    bool nal = itNu->convention == "#" || itNu->convention == "^"; //если книга есть в наличии
                                    if(!isNal && !nal)
                                    {
                                        continue;
                                    }
                                    res.push_back(QApplication::applicationDirPath() + "/res/book/" + der + "/" +  itNu->number + ".txt");
                                }
                            }
                        }
                        else //если не совпали названия
                        {
                            if(number.isEmpty())//если указан номер
                            {
                                numberBook bufNu;
                                bufNu.number = number; //структура для поиска
                                QList<numberBook>::iterator itNu = qLowerBound(itNa->listNu.begin(), itNa->listNu.end(), bufNu,
                                                             [](numberBook a, numberBook b){return a.number.toLower() < b.number.toLower();});
                                while(itNu != itNa->listNu.end())
                                {
                                    bool nal = itNu->convention == "#" || itNu->convention == "^";
                                    bool content = itNu->convention == "^" || itNu->convention == "%";
                                    if(!itNu->number.startsWith(number, Qt::CaseInsensitive)) // если не совпали номера
                                        break;
                                    if((!nal && !isNal) || !content)
                                    {
                                        itNu++;
                                        continue;
                                    }
                                    book Book(der, itNu->number);
                                    if(Book.isContent(name))
                                        res.push_back(QApplication::applicationDirPath() + "/res/book/" + der + "/" +  itNu->number + ".txt");
                                    itNu++;
                                }
                            }
                            else // если не указан номер
                            {
                                for(QList<numberBook>::iterator itNu = itNa->listNu.begin(); itNu != itNa->listNu.end(); itNu++)
                                {
                                    bool nal = itNu->convention == "#" || itNu->convention == "^"; //если книга есть в наличии
                                    bool content = itNu->convention == "^" || itNu->convention == "%";
                                    if((!isNal && !nal) || !content)
                                    {
                                        continue;
                                    }
                                    book Book(der, itNu->number);
                                    if(Book.isContent(name))
                                        res.push_back(QApplication::applicationDirPath() + "/res/book/" + der + "/" +  itNu->number + ".txt");
                                }
                            }
                        }
                    }
                }
                else // если не указано название
                {
                    for(QList<nameBook>::iterator itNa = itAu->listNa.begin(); itNa != itAu->listNa.end(); itNa++)
                    {
                        if(!number.isEmpty())//если указан номер
                        {
                            numberBook bufNu;
                            bufNu.number = number; //структура для поиска
                            QList<numberBook>::iterator itNu = qLowerBound(itNa->listNu.begin(), itNa->listNu.end(), bufNu,
                                                         [](numberBook a, numberBook b){return a.number.toLower() < b.number.toLower();});
                            while(itNu != itNa->listNu.end())
                            {
                                bool nal = itNu->convention == "#" || itNu->convention == "^";
                                if(!itNu->number.startsWith(number, Qt::CaseInsensitive)) // если не совпали номера
                                    break;
                                if(!nal && !isNal)
                                {
                                    itNu++;
                                    continue;
                                }
                                res.push_back(QApplication::applicationDirPath() + "/res/book/" + der + "/" +  itNu->number + ".txt");
                                itNu++;
                            }
                        }
                        else // если не указан номер
                        {
                            for(QList<numberBook>::iterator itNu = itNa->listNu.begin(); itNu != itNa->listNu.end(); itNu++)
                            {
                                bool nal = itNu->convention == "#" || itNu->convention == "^"; //если книга есть в наличии
                                if(!isNal && !nal)
                                {
                                    continue;
                                }
                                res.push_back(QApplication::applicationDirPath() + "/res/book/" + der + "/" +  itNu->number + ".txt");
                            }
                        }
                    }
                }

                itAu++;
            }
        }
        else
        {
            if(!name.isEmpty()) // если указано название
            {
                path += "FileSortName.txt";
                QList<nameBook> listNa = readFileSortName(path);
                for(QList<nameBook>::iterator itNa = listNa.begin(); itNa != listNa.end(); itNa++)
                {
                    if(itNa->name.startsWith(name, Qt::CaseInsensitive)) // если совпало название
                    {
                        if(!number.isEmpty())//если указан номер
                        {
                            numberBook bufNu;
                            bufNu.number = number; //структура для поиска
                            QList<numberBook>::iterator itNu = qLowerBound(itNa->listNu.begin(), itNa->listNu.end(), bufNu,
                                                         [](numberBook a, numberBook b){return a.number.toLower() < b.number.toLower();});
                            while(itNu != itNa->listNu.end())
                            {
                                bool nal = itNu->convention == "#" || itNu->convention == "^";
                                if(!itNu->number.startsWith(number, Qt::CaseInsensitive)) // если не совпали номера
                                    break;
                                if(!nal && !isNal)
                                {
                                    itNu++;
                                    continue;
                                }
                                res.push_back(QApplication::applicationDirPath() + "/res/book/" + der + "/" +  itNu->number + ".txt");
                                itNu++;
                            }
                        }
                        else // если не указан номер
                        {
                            for(QList<numberBook>::iterator itNu = itNa->listNu.begin(); itNu != itNa->listNu.end(); itNu++)
                            {
                                bool nal = itNu->convention == "#" || itNu->convention == "^"; //если книга есть в наличии
                                if(!isNal && !nal)
                                {
                                    continue;
                                }
                                res.push_back(QApplication::applicationDirPath() + "/res/book/" + der + "/" +  itNu->number + ".txt");
                            }
                        }
                    }
                    else //если не совпали названия
                    {
                        if(!number.isEmpty())//если указан номер
                        {
                            numberBook bufNu;
                            bufNu.number = number; //структура для поиска
                            QList<numberBook>::iterator itNu = qLowerBound(itNa->listNu.begin(), itNa->listNu.end(), bufNu,
                                                         [](numberBook a, numberBook b){return a.number.toLower() < b.number.toLower();});
                            while(itNu != itNa->listNu.end())
                            {
                                bool nal = itNu->convention == "#" || itNu->convention == "^";
                                bool content = itNu->convention == "^" || itNu->convention == "%";
                                if(!itNu->number.startsWith(number, Qt::CaseInsensitive)) // если не совпали номера
                                    break;
                                if((!nal && !isNal) || !content)
                                {
                                    itNu++;
                                    continue;
                                }
                                book Book(der, itNu->number);
                                if(Book.isContent(name))
                                    res.push_back(QApplication::applicationDirPath() + "/res/book/" + der + "/" +  itNu->number + ".txt");
                                itNu++;
                            }
                        }
                        else // если не указан номер
                        {
                            for(QList<numberBook>::iterator itNu = itNa->listNu.begin(); itNu != itNa->listNu.end(); itNu++)
                            {
                                bool nal = itNu->convention == "#" || itNu->convention == "^"; //если книга есть в наличии
                                bool content = itNu->convention == "^" || itNu->convention == "%";
                                if(!((isNal || nal) && content))
                                {
                                    continue;
                                }
                                book Book(der, itNu->number);
                                if(Book.isContent(name))
                                {
                                    res.push_back(QApplication::applicationDirPath() + "/res/book/" + der + "/" +  itNu->number + ".txt");
                                }
                            }
                        }
                    }
                }
            }
            else // если не указано название
            {
                if(!number.isEmpty())
                {
                    path += "FileSortNumber.txt";
                    QList<numberBook> listNu = readFileSortNumber(path);
                    numberBook bufNu;
                    bufNu.number = number; //структура для поиска
                    QList<numberBook>::iterator itNu = qLowerBound(listNu.begin(), listNu.end(), bufNu,
                                                 [](numberBook a, numberBook b){return a.number.toLower() < b.number.toLower();});
                    while(itNu != listNu.end())
                    {
                        bool nal = itNu->convention == "#" || itNu->convention == "^";
                        if(!itNu->number.startsWith(number, Qt::CaseInsensitive)) // если не совпали номера
                            break;
                        if(!nal && !isNal)
                        {
                            itNu++;
                            continue;
                        }
                        res.push_back(QApplication::applicationDirPath() + "/res/book/" + der + "/" +  itNu->number + ".txt");
                        itNu++;
                    }
                }
                else
                {
                    QString error = "Введите данные для поиска";
                    throw error;
                }
            }
        }
        return res;
    }
    else
    {
        QStringList res;
        QString path = QApplication::applicationDirPath() +"/res/book/"+der+'/';
        QString path1 = path + "fileSortBook.txt";

        QList<classStudent> listCl = readFileSortStudent(path1);

        QList<classStudent>::iterator itCl;
        if(author.isEmpty())
            itCl = listCl.begin();
        else
        {
            classStudent bufCl;//структура для сравнения
            bufCl.Class = author;
            itCl = qLowerBound(listCl.begin(), listCl.end(), bufCl,
                               [](classStudent a, classStudent b){return a.Class.toLower() < b.Class.toLower();});
        }

        while(itCl != listCl.end())
        {
            if(!itCl->Class.startsWith(author, Qt::CaseInsensitive) && !author.isEmpty())
                break;

            qDebug() << itCl->Class;
            QList<surnameStudent>::iterator itSu;
            if(name.isEmpty())
                itSu = itCl->listSu.begin();
            else
            {
                surnameStudent bufSu;//структура для сравнения
                bufSu.surname = name;
                itSu = qLowerBound(itCl->listSu.begin(), itCl->listSu.end(), bufSu,
                                   [](surnameStudent a, surnameStudent b){return a.surname.toLower() < b.surname.toLower();});
            }

            while(itSu != itCl->listSu.end())
            {
                if(!itSu->surname.startsWith(name, Qt::CaseInsensitive) && !name.isEmpty())
                    break;

                QStringList::iterator itNa;
                if(number.isEmpty())
                    itNa = itSu->listNa.begin();
                else
                    itNa = qLowerBound(itSu->listNa.begin(), itSu->listNa.end(), number,
                                       [](QString a, QString b){return a.toLower() < b.toLower();});
                while(itNa != itSu->listNa.end())
                {
                    QString bufNa = *itNa;
                    if(!bufNa.startsWith(number, Qt::CaseInsensitive) && !number.isEmpty())
                        break;
                    QString pathRes = path + itSu->surname + ' ' + bufNa + ' ' + itCl->Class + ".txt";
                    book Book(pathRes);
                    QString strNal = Book.getNal();
                    bool b;
                    int nal = strNal.toInt(&b);
                    if((nal > 0 && b) || isNal)
                        res.push_back(pathRes);

                    itNa++;
                }

                itSu++;
            }


            itCl++;
        }

        return res;
    }
}

QList<ManagementBD::classStudent> ManagementBD::readFileSortStudent(QString path)
{
    QList<classStudent> listCl;
    QFile file(path);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        if(!file.open(QFile::WriteOnly | QFile::Text))
        {
            QString error = "Не удалось открыть базу данных по адресу\""+path+"\"";
            throw error;
        }
        file.close();
        return listCl;
    }

    QTextStream stream(&file);
    int i = -1, j = -1; //счетчики
    QString buffer; //переменная для чтения
    //Читаем фаил
    while(!stream.atEnd())
    {
        buffer = stream.readLine();
        if(buffer.startsWith("\t\t"))
        {
            buffer = buffer.trimmed();
            listCl[i].listSu[j].listNa.push_back(buffer);
        }
        else if(buffer.startsWith("\t")) {
            buffer = buffer.trimmed();
            j++;
            surnameStudent buf;
            buf.surname = buffer;
            listCl[i].listSu.push_back(buf);
        }
        else {
            buffer = buffer.trimmed();
            i++;
            j = -1;
            classStudent buf;
            buf.Class = buffer;
            listCl.push_back(buf);
        }
    }

    file.close();

    return listCl;
}

void ManagementBD::printFileSortStudent(QList<classStudent> listCl, QString path)
{
    QFile file(path);

    if(listCl.size() == 0)
    {
        file.remove();
        return;
    }

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QString error = "Не удалось открыть базу данных для записи по адресу\""+path+"\"";
        throw error;
    }

    QTextStream stream(&file);

    //печатаем
    for(int i = 0; i < listCl.size(); i++)
    {
        stream << listCl.at(i).Class << endl;
        for(int j = 0; j < listCl.at(i).listSu.size(); j++)
        {
            stream << "\t" << listCl.at(i).listSu.at(j).surname << endl;
            for(int h = 0; h < listCl.at(i).listSu.at(j).listNa.size(); h++)
            {
                stream << "\t\t" << listCl.at(i).listSu.at(j).listNa.at(h) << endl;
            }
        }
    }
    file.close();
}

void ManagementBD::addStudentFile(QString Class, QString Surname, QString Name)
{
    QList<classStudent> listCl = readFileSortStudent(QApplication::applicationDirPath() +"/res/student/fileSortStudent.txt");

    QList<classStudent>::iterator itCl;
    classStudent bufCl;//структура для поиска
    bufCl.Class = Class;
    itCl = qLowerBound(listCl.begin(), listCl.end(), bufCl,
                       [](classStudent a, classStudent b){return a.Class.toLower() < b.Class.toLower();});
    if(itCl != listCl.end() && itCl->Class == Class)
    {
        QList<surnameStudent>::iterator itSu;
        surnameStudent bufSu;//структура для поиска
        bufSu.surname = Surname;
        itSu = qLowerBound(itCl->listSu.begin(), itCl->listSu.end(), bufSu,
                           [](surnameStudent a, surnameStudent b){return a.surname.toLower() < b.surname.toLower();});
        if(itSu != itCl->listSu.end() && itSu->surname == Surname)
        {
            QStringList::iterator itNa;
            itNa = qLowerBound(itSu->listNa.begin(), itSu->listNa.end(), Name,
                               [](QString a, QString b){return a.toLower() < b.toLower();});
            if(itNa != itSu->listNa.end() && *(itNa) == Name)
            {
                QString error = "Данный пользователь уже существует";
                throw error;
            }
            itNa = qUpperBound(itSu->listNa.begin(), itSu->listNa.end(), Name,
                               [](QString a, QString b){return a.toLower() < b.toLower();});
            itSu->listNa.insert(itNa, Name);
        }
        else
        {
            itSu = qUpperBound(itCl->listSu.begin(), itCl->listSu.end(), bufSu,
                               [](surnameStudent a, surnameStudent b){return a.surname.toLower() < b.surname.toLower();});
            bufSu.listNa.push_back(Name);
            itCl->listSu.insert(itSu, bufSu);
        }
    }
    else
    {
        surnameStudent bufSu;
        bufSu.surname = Surname;
        bufSu.listNa.push_back(Name);
        itCl = qUpperBound(listCl.begin(), listCl.end(), bufCl,
                           [](classStudent a, classStudent b){return a.Class.toLower() < b.Class.toLower();});
        bufCl.listSu.push_back(bufSu);
        listCl.insert(itCl, bufCl);
    }

    printFileSortStudent(listCl, QApplication::applicationDirPath() +"/res/student/fileSortStudent.txt");
}

void ManagementBD::DeleteStudentFile(QString Class, QString Surname, QString Name)
{
    QString path = QApplication::applicationDirPath() +"/res/student/fileSortStudent.txt";
    QList<classStudent> listCl = readFileSortStudent(path);

    QList<classStudent>::iterator itCl;
    classStudent bufCl;//структура для поиска
    bufCl.Class = Class;
    itCl = qBinaryFind(listCl.begin(), listCl.end(), bufCl,
                       [](classStudent a, classStudent b){return a.Class.toLower() < b.Class.toLower();});

    if(itCl != listCl.end() && itCl->Class.toLower() == Class.toLower())
    {
        QList<surnameStudent>::iterator itSu;
        surnameStudent bufSu;//структура для поиска
        bufSu.surname = Surname;
        itSu = qBinaryFind(itCl->listSu.begin(), itCl->listSu.end(), bufSu,
                           [](surnameStudent a, surnameStudent b){return a.surname.toLower() < b.surname.toLower();});

        if(itSu != itCl->listSu.end() && itSu->surname.toLower() == Surname.toLower())
        {
            QStringList::iterator itNa;
            itNa = qBinaryFind(itSu->listNa.begin(), itSu->listNa.end(), Name,
                               [](QString a, QString b){return a.toLower() < b.toLower();});
            QString bufNa = *itNa;
            if(itNa != itSu->listNa.end() && bufNa.toLower() == Name.toLower())
            {
                itSu->listNa.erase(itNa);
                if(itSu->listNa.isEmpty())
                {
                    itCl->listSu.erase(itSu);
                    if(itCl->listSu.isEmpty())
                    {
                        listCl.erase(itCl);
                    }
                }
            }
            else
            {
                QString error = "Не найден пользователь";
                throw error;
            }
        }
        else
        {
            QString error = "Не найден пользователь";
            throw error;
        }
    }
    else
    {
        QString error = "Не найден пользователь";
        throw error;
    }


    printFileSortStudent(listCl, path);

}

QStringList ManagementBD::searchStudent(QString Class, QString Surname, QString Name)
{
    QStringList res;
    QString path = QApplication::applicationDirPath() +"/res/student/";
    QString path1 = path + "fileSortStudent.txt";

    QList<classStudent> listCl = readFileSortStudent(path1);

    QList<classStudent>::iterator itCl;
    if(Class.isEmpty())
        itCl = listCl.begin();
    else
    {
        classStudent bufCl;//структура для сравнения
        bufCl.Class = Class;
        itCl = qLowerBound(listCl.begin(), listCl.end(), bufCl,
                           [](classStudent a, classStudent b){return a.Class.toLower() < b.Class.toLower();});
    }

    while(itCl != listCl.end())
    {
        if(!itCl->Class.startsWith(Class, Qt::CaseInsensitive) && !Class.isEmpty())
            break;

        QList<surnameStudent>::iterator itSu;
        if(Surname.isEmpty())
            itSu = itCl->listSu.begin();
        else
        {
            surnameStudent bufSu;//структура для сравнения
            bufSu.surname = Surname;
            itSu = qLowerBound(itCl->listSu.begin(), itCl->listSu.end(), bufSu,
                               [](surnameStudent a, surnameStudent b){return a.surname.toLower() < b.surname.toLower();});
        }

        while(itSu != itCl->listSu.end())
        {
            if(!itSu->surname.startsWith(Surname, Qt::CaseInsensitive) && !Surname.isEmpty())
                break;

            QStringList::iterator itNa;
            if(Name.isEmpty())
                itNa = itSu->listNa.begin();
            else
                itNa = qLowerBound(itSu->listNa.begin(), itSu->listNa.end(), Name,
                                   [](QString a, QString b){return a.toLower() < b.toLower();});
            while(itNa != itSu->listNa.end())
            {
                QString bufNa = *itNa;
                if(!bufNa.startsWith(Name, Qt::CaseInsensitive) && !Name.isEmpty())
                    break;
                QString pathRes = path + itSu->surname + ' ' + bufNa + ' ' + itCl->Class + ".txt";
                res.push_back(pathRes);

                itNa++;
            }

            itSu++;
        }


        itCl++;
    }

    return res;
}
