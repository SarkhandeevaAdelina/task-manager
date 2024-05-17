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
    // Конструктор класса TaskWidget
    explicit TaskWidget(const QString &text, const QString &time, bool important, QWidget *parent = nullptr, QList<TaskWidget*>* tasks = nullptr);


    // Получить текст задачи
    QString getText() const;

    // Получить время задачи
    QString getTime() const;

    // Проверить, выполнена ли задача
    bool isChecked() const;

    // Проверить, важная ли задача
    bool isImportant() const;

    // Удалить задачу
    void removeTask();

    // Загрузить задачи
    void loadTasks();

    // Сохранить задачи
    void saveTasks();

    // Установить флажок задачи
    void setChecked(bool checked);

    // Установить важность задачи
    void setImportant(bool important);

signals:
    // Сигнал удаления задачи
    void deleted(TaskWidget *task);

private slots:
    // Обработчик сигнала нажатия кнопки "Удалить"
    void onDeleteButtonClicked();

    // Обработчик сигнала нажатия кнопки "Важное"
    void onImportantButtonClicked();

private:
    QCheckBox *checkBox;         // Флажок задачи
    QLabel *textLabel;           // Текст задачи
    QLabel *timeLabel;           // Время задачи
    QPushButton *deleteButton;   // Кнопка "Удалить"
    QPushButton *importantButton; // Кнопка "Важное"
    QHBoxLayout *layout;         // Расположение виджетов в горизонтальном направлении
    QList<TaskWidget*>* tasks;  // Список указателей на виджеты задач
    bool important;              // Важность задачи
};

#endif // TASKWIDGET_H
