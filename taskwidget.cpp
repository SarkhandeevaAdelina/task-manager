#include "taskwidget.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

TaskWidget::TaskWidget(const QString &text, const QString &time, QWidget *parent, QList<TaskWidget*> *tasks)
    : QWidget(parent)
    , tasks(tasks)
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
    tasks->removeOne(this);
    deleteLater();
    emit deleted(this);
}

void TaskWidget::setChecked(bool checked)
{
    checkBox->setChecked(checked);
}
