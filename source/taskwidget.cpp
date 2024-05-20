#include "taskwidget.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// Конструктор для класса TaskWidget, который принимает текст, время, флаг важности, родительский виджет и указатель на список задач
TaskWidget::TaskWidget(const QString &text, const QString &time, bool important, QWidget *parent, QList<TaskWidget*> *tasks)
    : QWidget(parent)  // Инициализация родительского класса QWidget
    , tasks(tasks)    // Инициализация указателя на список задач
    , important(important)  // Инициализация флага важности
{
    checkBox = new QCheckBox(this);  // Создание нового чекбокса, который будет дочерним элементом для этого виджета
    checkBox->setIconSize(QSize(50, 50));  // Установка размера иконки для чекбокса

    textLabel = new QLabel(text, this);  // Создание нового текстового поля, которое будет дочерним элементом для этого виджета
    QFont font1 = textLabel->font();  // Получение текущего шрифта для текстового поля
    font1.setPointSize(16);  // Установка размера шрифта в 16 пунктов
    textLabel->setFont(font1);  // Применение нового шрифта к текстовому полю

    timeLabel = new QLabel(time, this);  // Создание нового текстового поля для отображения времени, которое будет дочерним элементом для этого виджета
    QFont font2 = timeLabel->font();  // Получение текущего шрифта для текстового поля
    font2.setPointSize(16);  // Установка размера шрифта в 16 пунктов
    timeLabel->setFont(font1);  // Применение нового шрифта к текстовому полю

    deleteButton = new QPushButton(this);  // Создание новой кнопки для удаления задачи, которая будет дочерним элементом для этого виджета
    deleteButton->setText("\u00D7");  // Установка текста для кнопки в виде символа "×"
    QFont font3 = deleteButton->font();  // Получение текущего шрифта для кнопки
    font3.setPointSize(32);  // Установка размера шрифта в 32 пункта
    deleteButton->setFont(font3);  // Применение нового шрифта к кнопке

    importantButton = new QPushButton(this);  // Создание новой кнопки для изменения флага важности, которая будет дочерним элементом для этого виджета
    importantButton->setText("\u2605");  // Установка текста для кнопки в виде символа "★"
    QFont font4 = importantButton->font();  // Получение текущего шрифта для кнопки
    font4.setPointSize(24);  // Установка размера шрифта в 24 пункта
    font4.setBold(true);  // Установка шрифта полужирным
    importantButton->setFont(font4);  // Применение нового шрифта к кнопке
    importantButton->setCheckable(true);  // Установка флага, позволяющего кнопке быть отмеченной
    importantButton->setChecked(false);  // Установка флага, показывающего, что кнопка не отмечена
    //importantButton->setStyleSheet("QPushButton { color: gray; }");  // Установка стиля для кнопки в виде серого цвета текста
    if (important) {  // Проверка флага важности
            importantButton->setStyleSheet("QPushButton { color: lightmagenta; }");  // Установка стиля для кнопки в виде цвета "lightmagenta"
        } else {  // Иначе
            importantButton->setStyleSheet("QPushButton { color: gray; }");  // Установка стиля для кнопки в виде серого цвета текста
        }

    layout = new QHBoxLayout(this);  // Создание нового горизонтального макета для этого виджета
    layout->addWidget(checkBox);  // Добавление чекбокса в макет
    layout->addWidget(textLabel);  // Добавление текстового поля в макет
    layout->addWidget(timeLabel);  // Добавление текстового поля с временем в макет
    layout->addStretch();  // Добавление растяжки в макет, которая займет все доступное пространство
    layout->setContentsMargins(0, 0, 20, 0);  // Установка отступов для содержимого макета в 0 пикселей слева, 0 пикселей сверху, 20 пикселей справа и 0 пикселей снизу
    layout->addWidget(importantButton, 0, Qt::AlignRight | Qt::AlignVCenter);  // Добавление кнопки изменения флага важности в макет, выравнивание по правому краю и вертикальному центру
    layout->addWidget(deleteButton, 0, Qt::AlignRight | Qt::AlignVCenter);  // Добавление кнопки удаления задачи в макет, выравнивание по правому краю и вертикальному центру
    layout->setContentsMargins(0, 0, 0, 0);  // Установка отступов для содержимого макета в 0 пикселей

    connect(deleteButton, &QPushButton::clicked, this, &TaskWidget::onDeleteButtonClicked);  // Подключение сигнала кнопки удаления задачи к слоту-обработчику нажатия на кнопку
    connect(importantButton, &QPushButton::clicked, this, &TaskWidget::onImportantButtonClicked);  // Подключение сигнала кнопки изменения флага важности к слоту-обработчику нажатия на кнопку
}

// Метод для получения текста задачи
QString TaskWidget::getText() const
{
    return textLabel->text();  // Возврат текста из текстового поля
}

// Метод для получения времени задачи
QString TaskWidget::getTime() const
{
    return timeLabel->text();  // Возврат текста из текстового поля с временем
}

// Метод для проверки, отмечена ли задача
bool TaskWidget::isChecked() const
{
    return checkBox->isChecked();  // Возврат флага, показывающего, отмечена ли задача
}

// Метод для проверки, является ли задача важной
bool TaskWidget::isImportant() const
{
    return important;  // Возврат флага важности
}

// Метод-обработчик нажатия на кнопку удаления задачи
void TaskWidget::onDeleteButtonClicked()
{
    removeTask();  // Вызов метода удаления задачи
}

// Метод для удаления задачи из списка задач и виджета
void TaskWidget::removeTask()
{
    tasks->removeOne(this);  // Удаление этого виджета из списка задач
    deleteLater();  // Удаление этого виджета из памяти
    emit deleted(this);  // Генерация сигнала, показывающего, что этот виджет был удален
}

// Метод-обработчик нажатия на кнопку изменения флага важности
void TaskWidget::onImportantButtonClicked()
{
    important = !important;  // Изменение флага важности на противоположный
    importantButton->setChecked(important);  // Установка флага, показывающего, отмечена ли кнопка изменения флага важности
    // сохраните изменения важности задачи в файл

    if (important) {  // Проверка флага важности
            importantButton->setStyleSheet("QPushButton { color: lightmagenta; }");  // Установка стиля для кнопки в виде цвета "lightmagenta"
        } else {  // Иначе
            importantButton->setStyleSheet("QPushButton { color: gray; }");  // Установка стиля для кнопки в виде серого цвета текста
        }
}

// Метод для установки флага, показывающего, отмечена ли задача
void TaskWidget::setChecked(bool checked)
{
    checkBox->setChecked(checked);  // Установка флага, показывающего, отмечена ли задача
}

// Метод для установки флага важности задачи
void TaskWidget::setImportant(bool important)
{
    this->important = important;  // Установка флага важности
    importantButton->setChecked(important);  // Установка флага, показывающего, отмечена ли кнопка изменения флага важности
    // сохраните изменения важности задачи в файл
}
