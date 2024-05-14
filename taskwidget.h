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
    explicit TaskWidget(const QString &text, const QString &time, QWidget *parent = nullptr, QList<TaskWidget*>* tasks = nullptr);

    QString getText() const;
    QString getTime() const;
    bool isChecked() const;

    void removeTask();
    void loadTasks();
    void saveTasks();
    void setChecked(bool checked);

signals:
    void deleted(TaskWidget *task);

private slots:
    void onDeleteButtonClicked();

private:
    QCheckBox *checkBox;
    QLabel *textLabel;
    QLabel *timeLabel;
    QPushButton *deleteButton;
    QHBoxLayout *layout;
    QList<TaskWidget*>* tasks;
};

#endif // TASKWIDGET_H
