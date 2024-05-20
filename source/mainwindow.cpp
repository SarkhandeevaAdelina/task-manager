#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// Конструктор класса MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this); // Установка пользовательского интерфейса

    setWindowTitle("TaskManager");

    // Устанавливаем в редактор дат текущую дату
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    // Ставим lineEdit в нормальный режим (чтобы не скрывал текст задач как пароли)
    ui->lineEdit->setEchoMode(QLineEdit::Normal);

    // Установите имя файла по умолчанию
    fileName = "tasks.json";
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked); // Подключение сигнала к слоту

    // Создайте список tasks
    tasks = new QList<TaskWidget*>;

    // Загрузите задачи из файла
    loadTasks();

    // Подключитесь к сигналу QApplication::aboutToQuit()
    QObject::connect(qApp, &QApplication::aboutToQuit, this, &MainWindow::saveTasks); // Подключение сигнала к слоту
}

// Деструктор класса MainWindow
MainWindow::~MainWindow()
{
    delete ui; // Удаление пользовательского интерфейса
}

// Функция загрузки задач из файла
void MainWindow::loadTasks()
{
    QFile file(fileName); // Создание объекта файла
    if (!file.open(QIODevice::ReadOnly)) { // Открытие файла в режиме чтения
        qWarning() << "Не удалось открыть файл задач для чтения: " << file.errorString(); // Вывод предупреждения
        return; // Возврат из функции
    }

    QByteArray data = file.readAll(); // Чтение всех данных из файла
    file.close(); // Закрытие файла

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data); // Преобразование данных из формата JSON в объект QJsonDocument
    if (jsonDoc.isNull()) { // Проверка на пустой объект
        qWarning() << "Не удалось распарсить файл задач: " << file.errorString(); // Вывод предупреждения
        return; // Возврат из функции
    }

    QJsonArray jsonArray = jsonDoc.array(); // Получение массива из объекта QJsonDocument
    for (const QJsonValue &value : jsonArray) { // Перебор элементов массива
        QJsonObject jsonObject = value.toObject(); // Преобразование элемента массива в объект QJsonObject

        QString text = jsonObject["text"].toString(); // Получение текста задачи из объекта QJsonObject
        QString time = jsonObject["time"].toString(); // Получение времени задачи из объекта QJsonObject
        bool checked = jsonObject["checked"].toBool(); // Получение флажка задачи из объекта QJsonObject

        // star

        bool important = jsonObject["important"].toBool(); // Получение флажка важности задачи из объекта QJsonObject
        TaskWidget *task = new TaskWidget(text, time, important, this, tasks); // Создание нового объекта TaskWidget

        task->setChecked(checked); // Установка флажка задачи

        tasks->append(task); // Добавление объекта TaskWidget в список tasks

        QListWidgetItem* item = new QListWidgetItem(ui->listWidget); // Создание нового объекта QListWidgetItem
        item->setSizeHint(task->sizeHint()); // Установка размера объекта QListWidgetItem
        ui->listWidget->addItem(item); // Добавление объекта QListWidgetItem в список listWidget
        ui->listWidget->setItemWidget(item, task); // Установка объекта TaskWidget в качестве виджета для объекта QListWidgetItem

        connect(task, &TaskWidget::deleted, this, &MainWindow::onTaskDeleted); // Подключение сигнала к слоту
    }
}

// Функция сохранения задач в файл
void MainWindow::saveTasks()
{
    QJsonArray jsonArray; // Создание пустого массива
    for (TaskWidget *task : *tasks) { // Перебор элементов списка tasks
        QJsonObject jsonObject; // Создание пустого объекта
        jsonObject["text"] = task->getText(); // Добавление текста задачи в объект
        jsonObject["time"] = task->getTime(); // Добавление времени задачи в объект
        jsonObject["checked"] = task->isChecked(); // Добавление флажка задачи в объект
        jsonObject["important"] = task->isImportant(); // Добавление флажка важности задачи в объект
        jsonArray.append(jsonObject); // Добавление объекта в массив
    }

    QJsonDocument jsonDoc(jsonArray); // Создание объекта QJsonDocument из массива
    QByteArray data = jsonDoc.toJson(QJsonDocument::Indented); // Преобразование объекта QJsonDocument в формат JSON

    QFile file(fileName); // Создание объекта файла
    if (!file.open(QIODevice::WriteOnly)) { // Открытие файла в режиме записи
        qWarning() << "Не удалось открыть файл задач для записи: " << file.errorString(); // Вывод предупреждения
        return; // Возврат из функции
    }
    file.write(data); // Запись данных в файл
    file.close(); // Закрытие файла
    qApp->processEvents(); // Обработка событий
}

// Функция удаления задачи из списка tasks
void MainWindow::onTaskDeleted(TaskWidget *task)
{
    for (int i = 0; i < ui->listWidget->count(); ++i) { // Перебор элементов списка listWidget
        QListWidgetItem *item = ui->listWidget->item(i); // Получение объекта QListWidgetItem
        QWidget *widget = ui->listWidget->itemWidget(item); // Получение виджета для объекта QListWidgetItem
        if (widget == task) { // Проверка на совпадение виджета с объектом TaskWidget
            delete item; // Удаление объекта QListWidgetItem
            delete task; // Удаление объекта TaskWidget
            tasks->removeOne(task); // Удаление объекта TaskWidget из списка tasks
            return; // Возврат из функции
        }
    }
}

// Функция обработки нажатия на кнопку добавления задачи
void MainWindow::on_pushButton_clicked()
{
    qDebug() << "Button clicked"; // Вывод отладочной информации

    QString text = ui->lineEdit->text(); // Получение текста из поля ввода
    QDateTime dateTime = ui->dateTimeEdit->dateTime(); // Получение даты и времени из виджета dateTimeEdit

    QString formattedDateTime = dateTime.toString("dd.MM.yyyy hh:mm");


    for (TaskWidget *task : *tasks) {
        QDateTime taskDateTime = QDateTime::fromString(task->getTime(), "hh:mm dd.MM.yyyy");
        if (task->getText().trimmed() == text.trimmed() && taskDateTime.date() == dateTime.date()) {
            QMessageBox::warning(this, "Ошибка", "Задача с таким названием и датой уже существует.");
            return;
        }
    }

    if (dateTime < QDateTime::currentDateTime()) {
        QMessageBox::warning(this, "Ошибка", "Нельзя создать задачу с датой в прошлом.");
        return;
    }

    if (text.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Необходимо ввести текст задачи.");
        return;
    }
    else{
        TaskWidget *task = new TaskWidget(text, dateTime.toString("hh:mm dd.MM.yyyy"), false, this, tasks); // Создание нового объекта TaskWidget

        task->setChecked(false); // Установка флажка задачи

        QListWidgetItem* item = new QListWidgetItem(); // Создание нового объекта QListWidgetItem
        item->setSizeHint(task->sizeHint()); // Установка размера объекта QListWidgetItem
        ui->listWidget->addItem(item); // Добавление объекта QListWidgetItem в список listWidget
        ui->listWidget->setItemWidget(item, task); // Установка объекта TaskWidget в качестве виджета для объекта QListWidgetItem

        connect(task, &TaskWidget::deleted, this, &MainWindow::onTaskDeleted); // Подключение сигнала к слоту

        // Добавляем задачу в список tasks
        tasks->append(task);

        ui->lineEdit->clear(); // Очистка поля ввода
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime()); // Установка текущей даты и времени в виджет dateTimeEdit
    }
    MainWindow::on_pushButtonAll_clicked(); // Вызов функции отображения всех задач
}

// Функция обработки нажатия на кнопку поиска задач
void MainWindow::on_pushButtonFind_clicked()
{
    QString text = ui->lineEditFind->text(); // Получение текста из поля ввода поиска

    for (int i = 0; i < ui->listWidget->count(); ++i) { // Перебор элементов списка listWidget
        QListWidgetItem *item = ui->listWidget->item(i); // Получение объекта QListWidgetItem
        QWidget *widget = ui->listWidget->itemWidget(item); // Получение виджета для объекта QListWidgetItem
        TaskWidget *task = static_cast<TaskWidget*>(widget); // Преобразование виджета в объект TaskWidget

        if (task->getText().startsWith(text, Qt::CaseInsensitive)) // Проверка на совпадение текста задачи с поисковым запросом
        {
            item->setHidden(false); // Отображение задачи
        } else {
            item->setHidden(true); // Скрытие задачи
        }
    }
}

// Функция обработки нажатия на кнопку отображения задач на сегодня
void MainWindow::on_pushButtonToday_clicked()
{
    // Получите текущую дату
    QDate currentDate = QDate::currentDate();

    // Пройдитесь по всем элементам в listWidget
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        // Получите элемент списка
        QListWidgetItem *item = ui->listWidget->item(i);
        // Получите виджет, соответствующий элементу списка
        QWidget *widget = ui->listWidget->itemWidget(item);
        // Преобразуйте виджет в TaskWidget
        TaskWidget *task = static_cast<TaskWidget*>(widget);
        // Получите дату выполнения задачи
        QDate taskDate = QDateTime::fromString(task->getTime(), "hh:mm dd.MM.yyyy").date();

        // Проверьте, относится ли дата выполнения задачи к текущему дню
        if (taskDate.day() == currentDate.day() &&
            taskDate.month() == currentDate.month() &&
            taskDate.year() == currentDate.year()) {
            item->setHidden(false); // Отображение задачи
        } else {
            item->setHidden(true); // Скрытие задачи
        }
    }
}

// Функция обработки нажатия на кнопку отображения важных задач
void MainWindow::on_pushButtonImportant_clicked()
{
    for (int i = 0; i < ui->listWidget->count(); ++i) { // Перебор элементов списка listWidget
        QListWidgetItem *item = ui->listWidget->item(i); // Получение объекта QListWidgetItem
        QWidget *widget = ui->listWidget->itemWidget(item); // Получение виджета для объекта QListWidgetItem
        TaskWidget *task = static_cast<TaskWidget*>(widget); // Преобразование виджета в объект TaskWidget

        if (task->isImportant()) { // Проверка на важность задачи
            item->setHidden(false); // Отображение задачи
        } else {
            item->setHidden(true); // Скрытие задачи
        }
    }
}

// Функция обработки нажатия на кнопку отображения всех задач
void MainWindow::on_pushButtonAll_clicked()
{
    for (int i = 0; i < ui->listWidget->count(); ++i) { // Перебор элементов списка listWidget
        QListWidgetItem *item = ui->listWidget->item(i); // Получение объекта QListWidgetItem
        item->setHidden(false); // Отображение задачи
    }
}
