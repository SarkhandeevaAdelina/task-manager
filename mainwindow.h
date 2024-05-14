#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "taskwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void onTaskDeleted(TaskWidget *task);
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<TaskWidget*>* tasks;
    QString fileName;

    void loadTasks();
    void saveTasks();
};
#endif // MAINWINDOW_H
