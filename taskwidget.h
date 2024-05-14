#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

class TaskWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskWidget(const QString &text, const QString &time, QWidget *parent = nullptr, QList<TaskWidget*> *tasks = nullptr);

    QString getText() const;
    QString getTime() const;
    bool isChecked() const;

    void setChecked(bool checked);
    void removeTask();
    void loadTasks();
    void saveTasks();

signals:
    void deleted(TaskWidget *task);

private slots:
    void onDeleteButtonClicked();

private:
    QList<TaskWidget*>* tasks;
    QCheckBox *checkBox;
    QLabel *textLabel;
    QLabel *timeLabel;
    QPushButton *deleteButton;
    QHBoxLayout *layout;
};

#endif // TASKWIDGET_H
