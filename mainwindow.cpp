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

    // Загрузите задачи из файла
    loadTasks();
}

MainWindow::~MainWindow()
{
    // Сохраните задачи в файл перед закрытием приложения
    saveTasks();

    delete ui;
}

void MainWindow::loadTasks()
{
    QFile file(fileName);
    if (!file.exists()) {
        qWarning() << "Не удалось открыть файл задач для чтения: " << file.errorString();
        return;
    }

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

        TaskWidget *task = new TaskWidget(text, time);
        task->setChecked(checked);
        tasks.append(task);

        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(task->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, task);

    }
}

void MainWindow::saveTasks()
{
    QJsonArray jsonArray;
    for (TaskWidget *task : tasks) {
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

void MainWindow::onTaskDeleted(TaskWidget *task)
{
    QListWidgetItem *item = ui->listWidget->itemAt(task->pos());
    if (item)
    {
        delete item;
        delete task;
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString text = ui->lineEdit->text();
    QDateTime dateTime = ui->dateTimeEdit->dateTime();

    if (!text.isEmpty())
    {
        TaskWidget *task = new TaskWidget(text, dateTime.toString("hh:mm dd.MM.yyyy"), this);
        task->setChecked(false);
        tasks.append(task);

        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(task->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, task);

        connect(task, &TaskWidget::deleted, this, &MainWindow::onTaskDeleted);

        ui->lineEdit->clear();
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    }
}
