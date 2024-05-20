#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  // Создание объекта приложения

    QTranslator translator;  // Создание объекта переводчика
    const QStringList uiLanguages = QLocale::system().uiLanguages();  // Получение списка языков интерфейса системы
    for (const QString &locale : uiLanguages) {  // Перебор языков интерфейса системы
        const QString baseName = "ToDoList_" + QLocale(locale).name();  // Создание базового имени файла перевода
        if (translator.load(":/i18n/" + baseName)) {  // Загрузка файла перевода
            a.installTranslator(&translator);  // Установка переводчика в приложение
            break;  // Выход из цикла после успешной загрузки файла перевода
        }
    }
    MainWindow w;  // Создание главного окна
    w.show();  // Отображение главного окна
    return a.exec();  // Запуск цикла обработки событий приложения
}
