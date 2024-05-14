#include "taskwidget.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

TaskWidget::TaskWidget(const QString &text, const QString &time, QWidget *parent, QList<TaskWidget*> *tasks)
    : QWidget(parent)
{
    checkBox = new QCheckBox(this);
    checkBox->setIconSize(QSize(50, 50));

    textLabel = new QLabel(text, this);
    QFont font1 = textLabel->font();
    font1.setPointSize(16);
    textLabel->setFont(font1);

    timeLabel = new QLabel(time, this);
    QFont font2 = timeLabel->font();
    font2.setPointSize(16);
    timeLabel->setFont(font1);

    deleteButton = new QPushButton(this);
    deleteButton->setText("\u00D7");
    QFont font3 = deleteButton->font();
    font3.setPointSize(32);
    deleteButton->setFont(font3);

    layout = new QHBoxLayout(this);
    layout->addWidget(checkBox);
    layout->addWidget(textLabel);
    layout->addWidget(timeLabel);
    layout->addStretch();
    layout->setContentsMargins(0, 0, 20, 0);
    layout->addWidget(deleteButton, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->setContentsMargins(0, 0, 0, 0);

    connect(deleteButton, &QPushButton::clicked, this, &TaskWidget::onDeleteButtonClicked);

    if (tasks) {
        tasks->append(this);
    }
}

QString TaskWidget::getText() const
{
    return textLabel->text();
}

QString TaskWidget::getTime() const
{
    return timeLabel->text();
}

bool TaskWidget::isChecked() const
{
    return checkBox->isChecked();
}

void TaskWidget::onDeleteButtonClicked()
{
    removeTask();
}

void TaskWidget::removeTask()
{
    int index = tasks->indexOf(this);
    if (index != -1) {
        tasks->removeAt(index);
        delete this;
    }
}

void TaskWidget::setChecked(bool checked)
{
    checkBox->setChecked(checked);
}


void TaskWidget::loadTasks()
{
    QFile file("tasks.json");

    if (file.exists()) {
        QByteArray jsonData = file.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        QJsonArray jsonArray = jsonDoc.array();
        file.close();

        for (const QJsonValue &value : jsonArray) {
            QJsonObject jsonObject = value.toObject();
            QString text = jsonObject["text"].toString();
            QString time = jsonObject["time"].toString();
            bool checked = jsonObject["checked"].toBool();

            TaskWidget *task = new TaskWidget(text, time, this, tasks);
            task->setChecked(checked);
            layout->insertWidget(1, task);
        }
    }
}

void TaskWidget::saveTasks()
{
    QJsonArray jsonArray;
    for (TaskWidget *task : *tasks) {
        QJsonObject jsonObject;
        jsonObject["text"] = task->getText();
        jsonObject["time"] = task->getTime();
        jsonObject["checked"] = task->isChecked();
        jsonArray.append(jsonObject);
    }

    QFile file("tasks.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument jsonDoc(jsonArray);
        file.write(jsonDoc.toJson());
        file.close();
    }
}
