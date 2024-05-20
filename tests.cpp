
#include <QTest>
#include <qtest_widgets.h>
#include "headers/mainwindow.h"
#include <qtest_gui.h>

class TestTaskManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        // вызываем тесты
        testCreateEmptyTask();
        testCreateDuplicateTask();
        testCreatePastDateTask();
    }

    void testCreateEmptyTask()
    {
        MainWindow w;
        w.show();

        // вводим пустую строку в lineEdit
        w.getLineEdit()->clear();

        // нажимаем на кнопку "Добавить"
        QTest::mouseClick(w.getPushButton(), Qt::LeftButton);

        // проверяем, что окно с ошибкой появилось
        QWidget *errorWidget = w.findChild<QWidget *>("errorWidget");
        QVERIFY(errorWidget->isVisible());

        // проверяем, что текст ошибки соответствует ожидаемому
        QLabel *errorLabel = w.findChild<QLabel *>("errorLabel");
        QCOMPARE(errorLabel->text(), QString("Необходимо ввести текст задачи."));
    }

    void testCreateDuplicateTask()
    {
        // добавляем задачу
        MainWindow w;
        w.show();
        w.getLineEdit()->setText("test task");
        w.getDateTimeEdit()->setDate(QDate::currentDate());
        QTest::mouseClick(w.getPushButton(), Qt::LeftButton);

        // повторяем попытку добавить задачу
        w.getLineEdit()->setText("test task");
        w.getDateTimeEdit()->setDate(QDate::currentDate());
        QTest::mouseClick(w.getPushButton(), Qt::LeftButton);

        // проверяем, что окно с ошибкой появилось
        QWidget *errorWidget = w.findChild<QWidget *>("errorWidget");
        QVERIFY(errorWidget->isVisible());

        // проверяем, что текст ошибки соответствует ожидаемому
        QLabel *errorLabel = w.findChild<QLabel *>("errorLabel");
        QCOMPARE(errorLabel->text(), QString("Задача с таким названием и датой уже существует."));
    }

    void testCreatePastDateTask()
    {
        MainWindow w;
        w.show();

        // устанавливаем дату в прошлое
        w.getDateTimeEdit()->setDate(QDate::currentDate().addDays(-1));

        // нажимаем на кнопку "Добавить"
        QTest::mouseClick(w.getPushButton(), Qt::LeftButton);

        // проверяем, что окно с ошибкой появилось
        QWidget *errorWidget = w.findChild<QWidget *>("errorWidget");
        QVERIFY(errorWidget->isVisible());

        // проверяем, что текст ошибки соответствует ожидаемому
        QLabel *errorLabel = w.findChild<QLabel *>("errorLabel");
        QCOMPARE(errorLabel->text(), QString("Нельзя создать задачу с датой в прошлом."));
    }
};


#include "tests.moc"
