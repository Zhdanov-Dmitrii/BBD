#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "add.h"
#include "addstudent.h"
#include "book.h"
#include "student.h"
#include "podrobno.h"
#include "managementbd.h"
#include <QFile>
#include <Qdir>
#include <QTextStream>
#include <ctime>
#include <QMessageBox>
#include <QAccessible>
#include <QtAlgorithms>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    change = true;
    changeStudent = true;
    ui->setupUi(this);

    QRegExp Au("[а-яА-я]{1,50}");
    QRegExp Na("[0-9а-яА-яA-Za-z]{1,50}[0-9а-яА-яA-Za-z ]{1,50}");
    QRegExp Nu("[0-9]{1,20}");
    ui->author->setValidator(new QRegExpValidator(Au,this));
    ui->name->setValidator(new QRegExpValidator(Na,this));
    ui->data->setValidator(new QRegExpValidator(Nu,this));
    ui->Surname->setValidator(new QRegExpValidator(Au,this));
    ui->Name->setValidator(new QRegExpValidator(Au,this));
    ui->Class->setValidator(new QRegExpValidator(Na,this));
    ui->statusBar->showMessage("Готов к работе");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_edd_clicked()                                  //Добавить
{
    edd *wnd = new edd(this);
    wnd->show();

}

void MainWindow::on_search_clicked()                               //Поиск
{
    change = false;
    double start = clock();
    ui->statusBar->showMessage("Подождите, поиск выполняется");
    QString author = ui->author->text();                   //автор
    QString number = ui->data->text();                     //номер
    QString name = ui->name->text();                       //название
    QString der = ui->der->currentText();                  //деректория
    QString path = QApplication::applicationDirPath();     //путь

    QDir mdir(path);

    if (!mdir.exists("res"))
    {
        ui->statusBar->showMessage("Ошибка,не найдена папка ресурсов");
        return;
    }
    path += "/res";
    mdir.cd(path);
    if (!mdir.exists("book"))
    {
        ui->statusBar->showMessage("Ошибка,не найдена база данных");
        return;
    }
    path += "/book";
    mdir.cd(path);
    if (!mdir.exists(der))
    {
        ui->statusBar->showMessage("Ошибка");
        return;
    }
    path = path + '/' + der;
    mdir.cd(path);

    try
    {
        QStringList vec = ManagementBD::searchBook(der, author, name, number, ui->checkBox->isChecked());

        int ns = 0;
       //вывод результатов
       if(!vec.isEmpty())
       {
           ui->tableWidget->setRowCount(vec.size());
           ui->tableWidget->clearContents();
           for (int i = 0; i < vec.size(); i++)//печатаем в таблицу
           {
               book Book(vec.at(i));
               QStringList list = Book.tablePrint();
               for(int j = 0; j < list.size(); j++)
               {
                   QString b = list.at(j);
                   ui->tableWidget->setItem(ns, j, new QTableWidgetItem(b));   //печатаем в таблицу
               }
               ns++;
           }
           double finish = (clock() - start)/1000;// time
           QString status = "Поиск завершен. Найдено книг: " + QString::number(vec.size())+", затраченно: "+QString::number(finish)+" секунд";
           ui->tableWidget->resizeColumnsToContents();
           ui->statusBar->showMessage(status);
           QApplication::beep();
       }
       else
       {
           ui->tableWidget->clearContents();
           QApplication::beep();
           ui->statusBar->showMessage("Поиск не дал результатов. Обновите данные для поиска!");
       }
    }
    catch(QString error)
    {
        QMessageBox::critical(this, "Что-то пошло не так!", error);
    }

   change = true;
}

void MainWindow::on_podrobno_clicked()                             //Подробно
{
    int ns = ui->tableWidget->currentRow();
    try
    {
        QString number = ui->tableWidget->item(ns,2)->text();  //Номер
        QString der = ui->tableWidget->item(ns,6)->text();     //Фонд
        if(der != "Учебный фонд")
        {
            book Book(der, number);
            Podrobno *wnd = new Podrobno(this,Book.getPath());
            wnd->show();
        }
        else
        {
            QString author = ui->tableWidget->item(ns, 1)->text();
            QString name = ui->tableWidget->item(ns, 0)->text();
            book Book(der, author, name, number);
            Podrobno *wnd = new Podrobno(this,Book.getPath());
            wnd->show();
        }
    }
    catch(QString str)
    {
        QMessageBox::critical(this,"Ошибка!!!", str);
    }
}

void MainWindow::on_dat_clicked()                                  //Выдать
{
    QList<QTableWidgetSelectionRange> rangesList = ui->tableWidget->selectedRanges();
    QVector<int> rowIndexes;    // Список строк для удаления

    foreach (QTableWidgetSelectionRange range, rangesList)
    {
        for (int i = range.topRow(); i <= range.bottomRow(); i++)
        {
            // Заполняем список
            rowIndexes.append(i);
        }
    }


    for(int i = 0; i < rowIndexes.size(); i++)
    {
        int nS = ui->tableStudent->currentRow();
        int nB = rowIndexes[i];

        try
        {
            QString Surname = ui->tableStudent->item(nS,0)->text(); //фамилия
            QString Name = ui->tableStudent->item(nS,1)->text();    //Имя
            QString Class = ui->tableStudent->item(nS,2)->text();   //Класс
            Student student(Class, Surname, Name);

            QString number = ui->tableWidget->item(nB,2)->text();   //Номер
            QString der = ui->tableWidget->item(nB,6)->text();      //Фонд
            book Book;

            if(der != "Учебный фонд")
            {
                Book = book(der, number);
            }
            else
            {
                QString name = ui->tableWidget->item(nB, 0)->text();  //Название
                QString author = ui->tableWidget->item(nB, 1)->text();//Автор
                Book = book(der, author, name, number);
            }

            if(!Book.issuance(Surname, Name, Class))  //Выдаем книгу, см. класс book
            {
                QMessageBox::critical(this,"Ошибка","Возможно книги нет в наличии или она уже принадлежит пользователю");
                continue;
            }

            //печатаем студентов
            changeStudent = false;
            Student student1(Class, Surname, Name);
            QStringList list = student1.tablePrint(); //Получаем данные о пользователе
            for(int j = 0; j < list.size(); j++)
            {
                QString b = list.at(j);
                ui->tableStudent->setItem(nS, j, new QTableWidgetItem(b));   //печатаем в таблицу
            }
            ui->tableStudent->resizeColumnToContents(3); //Подгоняем размер столбца "Кол-во книг"
            changeStudent = true;

            //печатаем книги
            change = false;
            list = Book.tablePrint(); //Получаем данные о книге
            for(int j = 0; j < list.size(); j++)
            {
                QString b = list.at(j);
                ui->tableWidget->setItem(nB, j, new QTableWidgetItem(b));   //печатаем в таблицу
            }
            ui->tableWidget->resizeColumnToContents(3); //Подгоняем размер столбца "В наличии"
            change = true;

            QString status = "Книга: \""+ui->tableWidget->item(nB,0)->text()+"\" успешна выдана пользователю: \"" +Surname + " " + Name + " " + Class +"\"";
            ui->statusBar->showMessage(status); //Меняем статуБар
        }
        catch(QString error)
        {
            QMessageBox::critical(this,"Что-то пошло не так!", error);
        }
    }
    QApplication::beep(); //Издаем звук
}

void MainWindow::on_zabrat_clicked()                               //Забрать
{
    QList<QTableWidgetSelectionRange> rangesList = ui->tableWidget->selectedRanges();
    QVector<int> rowIndexes;    // Список строк для удаления

    foreach (QTableWidgetSelectionRange range, rangesList)
    {
        for (int i = range.topRow(); i <= range.bottomRow(); i++)
        {
            // Заполняем список
            rowIndexes.append(i);
        }
    }

    for(int i = 0; i < rowIndexes.size(); i++)
    {
        int nB = rowIndexes[i];
        if(ui->tableWidget->item(nB,6)->text() == "Учебный фонд")
        {
            int nS = ui->tableStudent->currentRow();
            if(nB != -1 && nS != -1 && ui->tableStudent->item(nS,0)!=0 && ui->tableStudent->item(nS,1)!=0 && ui->tableStudent->item(nS,2)!=0 && ui->tableWidget->item(nB,2)!=0 && ui->tableWidget->item(nB,6)!=0)
            {
                QString Surname = ui->tableStudent->item(nS,0)->text(); //фамилия
                QString Name = ui->tableStudent->item(nS,1)->text();    //Имя
                QString Class = ui->tableStudent->item(nS,2)->text();   //Класс
                Student student(Class, Surname, Name);

                QString number = ui->tableWidget->item(nB,2)->text();  //Номер
                QString der = ui->tableWidget->item(nB,6)->text();     //Фонд
                QString name = ui->tableWidget->item(nB, 0)->text();  //Название
                QString author = ui->tableWidget->item(nB, 1)->text();//Автор
                book Book(der, author, name, number);


                if(!Book.seizure(Surname, Name, Class))  //забираем книгу, см. класс book
                {
                    QMessageBox::critical(this,"Ошибка","Пользователю на принадлежит данная книга");
                    continue;
                }

                //печатаем студентов
                changeStudent = false;
                Student student1(Class, Surname, Name);
                QStringList list = student1.tablePrint(); //Получаем данные о пользователе
                for(int j = 0; j < list.size(); j++)
                {
                    QString b = list.at(j);
                    ui->tableStudent->setItem(nS, j, new QTableWidgetItem(b));   //печатаем в таблицу
                }
                ui->tableStudent->resizeColumnToContents(3); //Подгоняем размер столбца "Кол-во книг"
                changeStudent = true;

                //печатаем книги
                change = false;
                list = Book.tablePrint(); //Получаем данные о книге
                for(int j = 0; j < list.size(); j++)
                {
                    QString b = list.at(j);
                    ui->tableWidget->setItem(nB, j, new QTableWidgetItem(b));   //печатаем в таблицу
                }
                ui->tableWidget->resizeColumnToContents(3); //Подгоняем размер столбца "В наличии"
                change = false;

                QString status = "Книга: \""+ui->tableWidget->item(nB,0)->text()+"\" успешна изъята у пользователя: \""
                        +Surname + ' '+Name+' '+Class+"\"";
                ui->statusBar->showMessage(status);//Изменяем статусБар
                QApplication::beep(); //Издаем звук
            }
            else QMessageBox::critical(this,"Ошибка","Выберете ячейки с пользователем и книгой");
        }
        else
        {
            if(nB != -1)
            {
                if(ui->tableWidget->item(nB,2)!=0 && ui->tableWidget->item(nB,6)!=0)
                {
                    QString nal = ui->tableWidget->item(nB,3)->text();
                    if(nal != "Да")
                    {
                        //Выделяем Фамилию, Имя, Класс
                        int s = nal.indexOf(' ',0);
                        int f = nal.indexOf(' ',s+1);
                        int v = nal.size();
                        QString Surname = ui->tableWidget->item(nB,3)->text().remove(s,v-s);
                        QString Name = ui->tableWidget->item(nB,3)->text().remove(0,s+1);
                        Name.remove(Name.indexOf(' ',0),v-f);
                        QString Class = ui->tableWidget->item(nB,3)->text().remove(0,f+1);;

                        QString number = ui->tableWidget->item(nB,2)->text(); //Номер
                        QString der = ui->tableWidget->item(nB,6)->text();    //Деректория
                        book Book(der, number);

                        if(!Book.seizure(Surname, Name, Class)) //Забираем книгу, см. класс book
                        {
                            QMessageBox::critical(this,"Ошибка","Что-то пошло не так!");
                            continue;
                        }

                        //печатаем Книги
                        change = false;
                        QStringList list = Book.tablePrint(); //Получаем данные о книге
                        for(int j = 0; j < list.size(); j++)
                        {
                            QString b = list.at(j);
                            ui->tableWidget->setItem(nB, j, new QTableWidgetItem(b));   //печатаем в таблицу
                        }
                        ui->tableWidget->resizeColumnToContents(3); //Подгоняем размер стобца "В наличи"
                        change = true;

                        QString status = "Книга: \""+ui->tableWidget->item(nB,0)->text()+"\" успешна изъята у пользователя: \"" +Surname+' '+Name+' '+Class+"\"";
                        ui->statusBar->showMessage(status); //Изменяем статусБар
                        QApplication::beep(); //Издаем звук
                    }
                    else
                    {
                        QString error = "Книга находится в библиотеке";
                        QMessageBox::critical(this,"Ошибка",error);
                    }
                }
                else QMessageBox::critical(this,"Ошибка","Обновите данные в таблице");
            }
            else QMessageBox::critical(this,"Ошибка","Выберете ячейку с книгой");
        }
    }
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)   //Автоизменение
{
    int ns = ui->tableWidget->currentRow();
    if (ns!=-1 && ui->tableWidget->item(ns,2)!=0 && ui->tableWidget->item(ns,6)!=0)
        if (change == true && ui->tableWidget->currentRow()!=-1 && ui->tableWidget->item(ns,2)!=0 && ui->tableWidget->item(ns,6)!=0)
        {
            QString number = ui->tableWidget->item(row,2)->text();
            QString der = ui->tableWidget->item(row,6)->text();
            book Book;

            if(der != "Учебный фонд")
            {
                Book = book(der, number);
            }
            else
            {
                QString name = ui->tableWidget->item(row, 0)->text();  //Название
                QString author = ui->tableWidget->item(row, 1)->text();//Автор
                Book = book(der, author, name, number);
            }

            QString change = ui->tableWidget->item(row,column)->text();
            switch(column)
            {
            case(0):
                Book.setName(change);
                break;
            case(1):
                Book.setAuthor(change);
                break;
            case(2):
                Book.setNumber(change);
                break;
            case(3):
                QMessageBox::information(this, "Ошибка","Автоизменение номера не доступно");
                break;
            case(4):
                Book.setOtdel(change);
                break;
            case(5):
                Book.setIzd(change);
                break;
            case(6):
                Book.setDer(change);
                break;
            }

        }
}

void MainWindow::on_pushButton_clicked()                           //Удалить
{
    QList<QTableWidgetSelectionRange> rangesList = ui->tableWidget->selectedRanges();

    QVector<int> rowIndexes;    // Список строк для удаления

    foreach (QTableWidgetSelectionRange range, rangesList)
    {
        for (int i = range.topRow(); i <= range.bottomRow(); i++)
        {
            // Заполняем список
            rowIndexes.append(i);
        }
    }

    for(auto nB : rowIndexes)
    {
        try
        {
            QString der = ui->tableWidget->item(nB,6)->text();
            if(der == "Учебный фонд")
            {
                QString Author = ui->tableWidget->item(nB, 1)->text();
                QString Name = ui->tableWidget->item(nB, 0)->text();
                QString Class = ui->tableWidget->item(nB,2)->text();
                book Book(der, Author, Name,  Class);
                Book.Delete(true);
            }
            else
            {
                QString number = ui->tableWidget->item(nB,2)->text();
                book Book(der, number);
                Book.Delete(true);
            }
        }
        catch(QString error)
        {
            QMessageBox::critical(this,"Что-то пошло не так", error);
        }
    }
    QApplication::beep();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)        //Изменение сортировки
{
    ui->tableWidget->sortByColumn(index,Qt::AscendingOrder);
}

//Student

void MainWindow::on_eddStudent_clicked()                           //Добавить
{
    eddStudent *wnd = new eddStudent(this);
    wnd->show();
}

void MainWindow::on_searchStudent_clicked()                        //Поиск
{
    double start = clock();

    QString Class = ui->Class->text();
    QString Surname = ui->Surname->text();
    QString Name = ui->Name->text();
    QString Path = QApplication::applicationDirPath();     //путь

    QDir mdir(Path);

    QStringList vec;

    try
    {
        vec = ManagementBD::searchStudent(Class, Surname, Name); // поиск
    }
    catch (QString error)
    {
        QMessageBox::critical(this, "Ошибка!!!",error + "\nВозможная причина ошибки - отсутствие записей пользователей");
    }

    qDebug() << "Кол-во найденных пользователей: " << vec.size();


    ui->tableStudent->clearContents();

    if (!mdir.exists("res"))
    {
        ui->statusBar->showMessage("Ошибка,не найдена папка ресурсов");
        return;
    }
    Path += "/res";
    mdir.cd(Path);
    if (!mdir.exists("student"))
    {
        ui->statusBar->showMessage("Ошибка,не найдена база данных");
        return;
    }
    Path += "/student";
    mdir.cd(Path);

    int ns = 0;//счет строк
    ui->tableStudent->setRowCount(vec.size());
    if(!vec.isEmpty())
    {
        changeStudent = false;
        for (int i = 0; i < vec.size(); i++)//печатаем в таблицу
        {
            try
            {
                Student student(vec.at(i));
                QStringList list = student.tablePrint();
                for(int j = 0; j < list.size(); j++)
                    ui->tableStudent->setItem(ns, j, new QTableWidgetItem(list[j]));   //печатаем в таблицу
                ns++;
            }
            catch(QString error)
            {
                QMessageBox::critical(this,"Что-то пошло не так", error);
            }
        }
        changeStudent = true;
        double finish = (clock() - start)/1000;
        QString status = "Поиск завершен. Найдено: " + QString::number(vec.size())+" пользователей, затраченно: "+QString::number(finish)+" секунд";
        ui->statusBar->showMessage(status);
    }
    else
    {
        ui->statusBar->showMessage("Поиск не дал результатов. Обновите данные для поиска!");
    }
    ui->tableStudent->resizeColumnsToContents();
    QApplication::beep();
}

void MainWindow::on_cleanStudent_clicked()                         //Удалить
{

    QList<QTableWidgetSelectionRange> rangesList = ui->tableStudent->selectedRanges();

    foreach (QTableWidgetSelectionRange range, rangesList)
    {
        for (int nS = range.topRow(); nS <= range.bottomRow(); nS++)
        {
            QString Surname = ui->tableStudent->item(nS,0)->text();
            QString Name = ui->tableStudent->item(nS,1)->text();
            QString Class = ui->tableStudent->item(nS,2)->text();
            Student student(Class, Surname, Name);
            student.DeleteStudent();
            QString status = "Пользователь: \""+Surname+' '+Name+' '+Class+"\" успешно удален!";
            ui->statusBar->showMessage(status);
        }
    }
    QApplication::beep();
}

void MainWindow::on_podrobno_2_clicked()                           //Подробно
{
    int nS = ui->tableStudent->currentRow();
    if(nS != -1)
    {
        QString Surname = ui->tableStudent->item(nS,0)->text();
        QString Name = ui->tableStudent->item(nS,1)->text();
        QString Class = ui->tableStudent->item(nS,2)->text();
        Student student(Class, Surname, Name);

        QStringList vec = student.podrobno();

        int ns = 0;
        ui->tableWidget->setRowCount(vec.size());
        ui->tableWidget->clearContents();
        for (int i = 0; i < vec.size(); i++)//печатаем в таблицу
        {
            book Book(vec.at(i));
            QStringList list = Book.tablePrint();
            for(int j = 0; j < list.size(); j++)
            {
                QString b = list.at(j);
                ui->tableWidget->setItem(ns, j, new QTableWidgetItem(b));   //печатаем в таблицу
            }
            ns++;
        }
        ui->tableWidget->resizeColumnsToContents();

        QString status = "Данные о книгах пользователя \""+Surname+' '+Name+' '+Class+"\" успешно получены!";
        ui->statusBar->showMessage(status);
    }
    else ui->statusBar->showMessage("Выберите данные в таблице");
    QApplication::beep();
}

void MainWindow::on_tableStudent_cellChanged(int row, int column)  //Автоизменение
{
    if (changeStudent == true && ui->tableStudent->item(row,2)!=0 && ui->tableStudent->item(row,1)!=0 && ui->tableStudent->item(row,0)!=0)
    {
        QString Surname = ui->tableStudent->item(row,0)->text();
        QString Name = ui->tableStudent->item(row,1)->text();
        QString Class = ui->tableStudent->item(row,2)->text();
        Student student(Class, Surname, Name);
        QString change = ui->tableStudent->item(row,column)->text();
        switch(column)
        {
        case(0):
            student.setSurname(change);
            break;
        case(1):
            student.setName(change);
            break;
        case(2):
            student.setClass(change);
            break;
        case(3):
            break;
        case(4):
            student.setData(change);
            break;
        case(5):
            student.setPhone(change);
            break;
        case(6):
            student.setAddress(change);
            break;
        }
    }
}

void MainWindow::on_der_currentIndexChanged(const QString &arg1)   //Изменение Номер/Фонд
{
    if(arg1 == "Учебный фонд")
    {
        ui->label_number->setText("Класс");
        QStringList list;
        list << "Название" << "Автор" << "Класс" << "В наличии" << "Отдел" << "Издательство" << "Фонд";
        ui->tableWidget->setHorizontalHeaderLabels(list);
        derect = ui->der->currentText();
    }
    else
    {
        if(derect == "Учебный фонд")
        {
            ui->label_number->setText("Номер");
            QStringList list;
            list << "Название" << "Автор" << "Номер" << "В наличии" << "Отдел" << "Издательство" << "Фонд";
            ui->tableWidget->setHorizontalHeaderLabels(list);
            derect = ui->der->currentText();
        }
    }
}

void MainWindow::on_options_2_clicked()                            //Обновить
{
    int ns = ui->tableStudent->rowCount();

    QList<QStringList> list;

    for(int i = 0; i < ns; i++)
    {
        if(ui->tableStudent->item(i,0)!=0 && ui->tableStudent->item(i,1)!=0 && ui->tableStudent->item(i,2)!=0)
        {
            try
            {
                Student student(ui->tableStudent->item(i,2)->text(),ui->tableStudent->item(i,0)->text(), ui->tableStudent->item(i,1)->text());
                list.push_back(student.tablePrint());
            }
            catch(QString error)
            {
                QMessageBox::critical(this,"Ошибка!!!",error);
            }
        }
    }

    ui->tableStudent->setRowCount(list.size());
    ui->tableStudent->clearContents();
    changeStudent = false;
    for(int i = 0; i < list.size(); i++)
    {
        for(int j = 0; j < list[i].size(); j++)
        {
            ui->tableStudent->setItem(i, j, new QTableWidgetItem(list[i][j]));
        }
    }
    changeStudent = true;
    ui->tableStudent->resizeColumnsToContents();
}

void MainWindow::on_clean_2_clicked()                              //Очистить
{
    ui->tableWidget->clearContents();
    ui->tableStudent->clearContents();
    ui->tableWidget->setRowCount(20);
    ui->tableStudent->setRowCount(20);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableStudent->resizeColumnsToContents();
    QApplication::beep();
}

void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column) // подробно книги по двойному клику
{

    if (ui->tableWidget->item(row, 2)!=0 && ui->tableWidget->item(row, 6)!=0)
    {
        try
        {
            QString number = ui->tableWidget->item(row, 2)->text();  //Номер
            QString der = ui->tableWidget->item(row, 6)->text();     //Фонд
            book Book;

            if(der != "Учебный фонд")
            {
                Book = book(der, number);
            }
            else
            {
                QString name = ui->tableWidget->item(row, 0)->text();  //Название
                QString author = ui->tableWidget->item(row, 1)->text();//Автор
                Book = book(der, author, name, number);
            }
            Podrobno *wnd = new Podrobno(this,Book.getPath());
            wnd->show();
        }
        catch(QString str)
        {
            QMessageBox::critical(this,"Ошибка!!!", str);
        }
    }
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    ui->tableStudent->sortByColumn(index,Qt::AscendingOrder);
}
