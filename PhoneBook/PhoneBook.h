#pragma once
#include <QtWidgets/QWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QDateEdit>
#include <QLineEdit>
#include <QFile>
#include <qdebug.h>
#include "ui_PhoneBook.h"

class PhoneBook : public QWidget
{
    Q_OBJECT

public:
    PhoneBook(QWidget *parent = nullptr);
    ~PhoneBook();

private:
    QString lastChangedData;
    int selectedRowIndex;
    QTableWidget* table;
    QPushButton* button_add;
    QPushButton* button_del;
    QPushButton* button_save;
    QPushButton* button_search;
    QLineEdit* editText;

    bool validateName(QString str);
    bool validateEmail(QString &str);
    bool validatePhone(QString &str);

    void loadData();

    int indexRow();

private slots:
    void button_add_click();//
    void button_del_click();//
    void button_save_click();//
    void button_search_click();//
    void editDate(QDate date);
    void restoreCell(int row, int col);
    void checkCell(int row, int col);
};
