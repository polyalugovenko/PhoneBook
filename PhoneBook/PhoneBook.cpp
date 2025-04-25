#include "PhoneBook.h"

PhoneBook::PhoneBook(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout;
    table = new QTableWidget();
    table->viewport()->installEventFilter(this);

    table->setColumnCount(7);
    table->setHorizontalHeaderLabels(QStringList{ "name", "surname", "patronymic", "address", "date of birth"," email", "phone number" });
    table->horizontalHeaderItem(6)->setToolTip("(212) 348-2626, +7 832-393-1000");
    table->horizontalHeaderItem(5)->setToolTip("email@mail.com");
    table->horizontalHeaderItem(2)->setToolTip("Ivanov/Ivan8/Ivan-Ivan/Ivan9-I");
    table->horizontalHeaderItem(1)->setToolTip("Ivanov/Ivan8/Ivan-Ivan/Ivan9-I");
    table->horizontalHeaderItem(0)->setToolTip("Ivanov/Ivan8/Ivan-Ivan/Ivan9-I");
    table->setSortingEnabled(true);

    table->setShowGrid(true);
    table->setGridStyle(Qt::SolidLine);

    connect(table, SIGNAL(cellPressed(int, int)), this, SLOT(restoreCell(int, int)));
    connect(table, SIGNAL(cellChanged(int, int)), this, SLOT(checkCell(int, int)));

    layout->addWidget(table);

    QVBoxLayout* layoutForButtons = new QVBoxLayout;

    button_add = new QPushButton("Add");
    button_search = new QPushButton("Find");
    button_del = new QPushButton("Delete");
    button_save = new QPushButton("Save");
    editText = new QLineEdit();
    editText->setFixedSize(250, 40);

    connect(button_add, SIGNAL(clicked()), SLOT(button_add_click()));
    connect(button_search, SIGNAL(clicked()), SLOT(button_search_click()));
    connect(button_del, SIGNAL(clicked()), SLOT(button_del_click()));
    connect(button_save, SIGNAL(clicked()), SLOT(button_save_click()));

    layoutForButtons->addWidget(button_add);
    layoutForButtons->addWidget(button_del);
    layoutForButtons->addWidget(button_save);
    layoutForButtons->addWidget(button_search);
    layoutForButtons->addWidget(editText);
    layoutForButtons->addStretch();

    layout->addLayout(layoutForButtons);
    setLayout(layout);
    setWindowTitle(tr("Phone Book"));
    loadData();
    setFixedSize(1180, 900);
}

void PhoneBook::button_add_click() {
    table->insertRow(table->rowCount());
    int row = table->rowCount() - 1;

    QDateEdit* date = new QDateEdit();
    date->setMaximumDate(QDate::currentDate());
    date->setMinimumDate(QDate(1900, 1, 1));
    date->setDate(QDate::currentDate());
    date->setDisplayFormat("dd.MM.yyyy");
    date->setCalendarPopup(true);

    table->setCellWidget(row, 4, date);
    QDate tmp = date->date();
    
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setData(Qt::DisplayRole, tmp);
    table->setItem(row, 4, item);
    connect(date, SIGNAL(dateChanged(QDate)), this, SLOT(editDate(QDate)));
}

void PhoneBook::button_del_click() {
    QModelIndexList selectedRows = table->selectionModel()->selectedRows();
    for (int i = selectedRows.size()-1; i >= 0; i--) {
        table->removeRow(selectedRows[i].row());
    }
}

void PhoneBook::button_search_click() {
    table->clearSelection();
    QString search_str = editText->text();
    if (search_str == "") return;
    for (int i = 0; i < table->rowCount(); i++) {
        for (int j = 0; j < table->columnCount(); j++) {
            if (table->item(i, j) == 0) continue;
            if (table->item(i, j)->text().toLower().indexOf(search_str.toLower()) != -1)
            {
                table->selectionModel()->select(table->model()->index(i, j),
                    QItemSelectionModel::Select);
            }
        }
    }
}

void PhoneBook::editDate(QDate date) {
    int row = indexRow();
    if (row == -1) return;
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setData(Qt::DisplayRole, date);
    table->setItem(row, 4, item);
}

int PhoneBook::indexRow()
{
    QList<QTableWidgetSelectionRange> rangeList = table->selectedRanges();
    foreach(QTableWidgetSelectionRange range, rangeList) 
    {
        return range.topRow();
    }
}

void PhoneBook::checkCell(int row, int col)
{
    if (col == 3 || col == 4 || table->item(row, col) == 0) return;

    QString cellText = table->item(row, col)->text();
    bool flag = 0;

    switch (col) {
        case 0: flag = validateName(cellText); break;
        case 1: flag = validateName(cellText); break;
        case 2: flag = validateName(cellText); break;
        case 5: flag = validateEmail(cellText); break;
        case 6: flag = validatePhone(cellText); break;
    }

    if (!flag) {
        table->item(row, col)->setText(lastChangedData);
    }
}
void PhoneBook::restoreCell(int row, int col) {
    if (col == 3 || col == 4 || table->item(row, col) == 0) return;
    lastChangedData = table->item(row, col)->text();
}

bool PhoneBook::validateName(QString str) {
    QRegExp name("^[A-ZА-Я][а-я0-9a-z]{1,20}([\\-][A-ZА-Я]([а-яa-z0-9]{1,20})?)?$");
   return name.exactMatch(str);
}
bool PhoneBook::validateEmail(QString &str) {
    QRegExp email("^\\w+([.-]?\\w+)@\\w+([.-]?\\w+)(.\\w+)$");
    return email.exactMatch(str);
}
bool PhoneBook::validatePhone(QString &str) {
    QRegExp phone("^(((8|\\+7)[\\- ]?)?(\\(?\\d{3}\\)?[\\- ]?)?[\\d\\- ]{7,10}[,])+$");
    return phone.exactMatch(str);
}


void PhoneBook::button_save_click() 
{
    QFile file("C:/Users/User/Desktop/PhoneBook/PhoneBook/one.txt");
    if (!file.open(QFile::WriteOnly |
        QFile::Text))
    {
        qDebug() << " Could not open the file for reading";
        return;
    }
    QTextStream out(&file);
    for (int i = 0; i < table->rowCount(); i++) {
        for (int j = 0; j < table->columnCount(); j++) {
            if (table->item(i, j) == 0) out << " ;";
            else 
            {
                out << table->item(i, j)->text() << ";";
            }
        }
        out << "\n";
    }
    file.close();
}

void PhoneBook::loadData()
{
    QFile file("C:/Users/User/Desktop/PhoneBook/PhoneBook/one.txt");
    if (!file.open(QFile::ReadOnly |
        QFile::Text))
    {
        qDebug() << " Could not open the file for reading";
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString string_data;
        string_data = in.readLine();
        QStringList datas = string_data.split(";");
        table->insertRow(table->rowCount());
        int row = table->rowCount() - 1;
        for (int i = 0; i < table->columnCount(); i++)
        {
            if (i == 4) continue;
            QTableWidgetItem* tmpItem = new QTableWidgetItem(datas[i]);
            table->setItem(row, i, tmpItem);
        }

        QDateEdit* date = new QDateEdit();
        date->setMaximumDate(QDate::currentDate());
        date->setMinimumDate(QDate(1900, 1, 1));
        date->setDate(QDate::fromString(datas[4].replace("-", "."), "yyyy.MM.dd"));
        date->setDisplayFormat("dd.MM.yyyy");
        date->setCalendarPopup(true);

        table->setCellWidget(row, 4, date);
        QDate tmp = date->date();
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, tmp);
        table->setItem(row, 4, item);
        connect(date, SIGNAL(dateChanged(QDate)), this, SLOT(editDate(QDate)));
    }

    file.close();
}



PhoneBook::~PhoneBook()
{}
