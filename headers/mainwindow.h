#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>
#include "taskwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Конструктор класса MainWindow
    MainWindow(QWidget *parent = nullptr);

    // Деструктор класса MainWindow
    ~MainWindow();

    // Обработчик сигнала удаления задачи
    void onTaskDeleted(TaskWidget *task);

    // Обработчик сигнала нажатия кнопки "Добавить"
    void on_pushButton_clicked();

private slots:
    void on_pushButtonFind_clicked();

    void on_pushButtonToday_clicked();

    void on_pushButtonImportant_clicked();

    void on_pushButtonAll_clicked();

private:
    Ui::MainWindow *ui;      // Указатель на графический интерфейс
    QList<TaskWidget*>* tasks;  // Список указателей на виджеты задач
    QString fileName;         // Имя файла для сохранения и загрузки задач

    // Загрузка задач из файла
    void loadTasks();

    // Сохранение задач в файл
    void saveTasks();
};
#endif // MAINWINDOW_H
