#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Установите имя файла по умолчанию
    fileName = "tasks.json";
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);

    // Создайте список tasks
    tasks = new QList<TaskWidget*>;

    // Загрузите задачи из файла
    loadTasks();
}

MainWindow::~MainWindow()
{
    // Сохраните задачи в файл перед закрытием приложения
    saveTasks();

    delete ui;
}

//void MainWindow::loadTasks()
//{
//    QFile file(fileName);
//    if (!file.open(QIODevice::ReadOnly)) {
//        qWarning() << "Не удалось открыть файл задач для чтения: " << file.errorString();
//        return;
//    }

//    QByteArray data = file.readAll();
//    file.close();

//    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
//    if (jsonDoc.isNull()) {
//        qWarning() << "Не удалось распарсить файл задач: " << file.errorString();
//        return;
//    }

//    QJsonArray jsonArray = jsonDoc.array();
//    for (const QJsonValue &value : jsonArray) {
//        QJsonObject jsonObject = value.toObject();

//        QString text = jsonObject["text"].toString();
//        QString time = jsonObject["time"].toString();
//        bool checked = jsonObject["checked"].toBool();

//        TaskWidget *task = new TaskWidget(text, time, this, tasks);
//        task->setChecked(checked);

//        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
//        item->setSizeHint(task->sizeHint());
//        ui->listWidget->addItem(item);
//        ui->listWidget->setItemWidget(item, task);
//    }
//}

void MainWindow::loadTasks()
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть файл задач для чтения: " << file.errorString();
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (jsonDoc.isNull()) {
        qWarning() << "Не удалось распарсить файл задач: " << file.errorString();
        return;
    }

    QJsonArray jsonArray = jsonDoc.array();
    for (const QJsonValue &value : jsonArray) {
        QJsonObject jsonObject = value.toObject();

        QString text = jsonObject["text"].toString();
        QString time = jsonObject["time"].toString();
        bool checked = jsonObject["checked"].toBool();

        TaskWidget *task = new TaskWidget(text, time, this, tasks);
        task->setChecked(checked);
        tasks->append(task);

        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(task->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, task);

        connect(task, &TaskWidget::deleted, this, &MainWindow::onTaskDeleted);
    }
}


void MainWindow::saveTasks()
{
    QJsonArray jsonArray;
    for (TaskWidget *task : *tasks) {
        QJsonObject jsonObject;
        jsonObject["text"] = task->getText();
        jsonObject["time"] = task->getTime();
        jsonObject["checked"] = task->isChecked();

        jsonArray.append(jsonObject);
    }

    QJsonDocument jsonDoc(jsonArray);
    QByteArray data = jsonDoc.toJson(QJsonDocument::Indented);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Не удалось открыть файл задач для записи: " << file.errorString();
        return;
    }

    file.write(data);
    file.close();
}

//void MainWindow::onTaskDeleted(TaskWidget *task)
//{
//    for (int i = 0; i < ui->listWidget->count(); ++i) {
//        QListWidgetItem *item = ui->listWidget->item(i);
//        QWidget *widget = ui->listWidget->itemWidget(item);
//        if (widget == task) {
//            delete item;
//            delete task;
//            return;
//        }
//    }
//}

void MainWindow::onTaskDeleted(TaskWidget *task)
{
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        QWidget *widget = ui->listWidget->itemWidget(item);
        if (widget == task) {
            delete item;
            delete task;
            return;
        }
    }
}


//void MainWindow::on_pushButton_clicked()
//{
//    qDebug() << "Button clicked";

//    QString text = ui->lineEdit->text();
//    QDateTime dateTime = ui->dateTimeEdit->dateTime();

//    if (!text.isEmpty())
//    {
//        TaskWidget *task = new TaskWidget(text, dateTime.toString("hh:mm dd.MM.yyyy"), this, tasks);
//        task->setChecked(false);

//        //QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
//        QListWidgetItem* item = new QListWidgetItem();

//        item->setSizeHint(task->sizeHint());
//        ui->listWidget->addItem(item);
//        ui->listWidget->setItemWidget(item, task);

//        connect(task, &TaskWidget::deleted, this, &MainWindow::onTaskDeleted);

//        ui->lineEdit->clear();
//        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
//    }
//}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << "Button clicked";

    QString text = ui->lineEdit->text();
    QDateTime dateTime = ui->dateTimeEdit->dateTime();

    if (!text.isEmpty())
    {
        TaskWidget *task = new TaskWidget(text, dateTime.toString("hh:mm dd.MM.yyyy"), this, tasks);
        task->setChecked(false);

        QListWidgetItem* item = new QListWidgetItem();
        item->setSizeHint(task->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, task);

        connect(task, &TaskWidget::deleted, this, &MainWindow::onTaskDeleted);

        ui->lineEdit->clear();
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    }
}
