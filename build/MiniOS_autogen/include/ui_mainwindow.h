/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QListWidget *menuList;
    QStackedWidget *stackedWidgets;
    QWidget *processWidget;
    QVBoxLayout *verticalLayoutProcess;
    QLabel *processLabel;
    QWidget *syncWidget;
    QVBoxLayout *verticalLayoutSync;
    QLabel *syncLabel;
    QWidget *memoryWidget;
    QVBoxLayout *verticalLayoutMemory;
    QLabel *memoryLabel;
    QWidget *fileWidget;
    QVBoxLayout *verticalLayoutFile;
    QLabel *fileLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1000, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        menuList = new QListWidget(centralwidget);
        menuList->setObjectName(QString::fromUtf8("menuList"));

        horizontalLayout->addWidget(menuList);

        stackedWidgets = new QStackedWidget(centralwidget);
        stackedWidgets->setObjectName(QString::fromUtf8("stackedWidgets"));
        processWidget = new QWidget();
        processWidget->setObjectName(QString::fromUtf8("processWidget"));
        verticalLayoutProcess = new QVBoxLayout(processWidget);
        verticalLayoutProcess->setObjectName(QString::fromUtf8("verticalLayoutProcess"));
        processLabel = new QLabel(processWidget);
        processLabel->setObjectName(QString::fromUtf8("processLabel"));
        processLabel->setAlignment(Qt::AlignCenter);

        verticalLayoutProcess->addWidget(processLabel);

        stackedWidgets->addWidget(processWidget);
        syncWidget = new QWidget();
        syncWidget->setObjectName(QString::fromUtf8("syncWidget"));
        verticalLayoutSync = new QVBoxLayout(syncWidget);
        verticalLayoutSync->setObjectName(QString::fromUtf8("verticalLayoutSync"));
        syncLabel = new QLabel(syncWidget);
        syncLabel->setObjectName(QString::fromUtf8("syncLabel"));
        syncLabel->setAlignment(Qt::AlignCenter);

        verticalLayoutSync->addWidget(syncLabel);

        stackedWidgets->addWidget(syncWidget);
        memoryWidget = new QWidget();
        memoryWidget->setObjectName(QString::fromUtf8("memoryWidget"));
        verticalLayoutMemory = new QVBoxLayout(memoryWidget);
        verticalLayoutMemory->setObjectName(QString::fromUtf8("verticalLayoutMemory"));
        memoryLabel = new QLabel(memoryWidget);
        memoryLabel->setObjectName(QString::fromUtf8("memoryLabel"));
        memoryLabel->setAlignment(Qt::AlignCenter);

        verticalLayoutMemory->addWidget(memoryLabel);

        stackedWidgets->addWidget(memoryWidget);
        fileWidget = new QWidget();
        fileWidget->setObjectName(QString::fromUtf8("fileWidget"));
        verticalLayoutFile = new QVBoxLayout(fileWidget);
        verticalLayoutFile->setObjectName(QString::fromUtf8("verticalLayoutFile"));
        fileLabel = new QLabel(fileWidget);
        fileLabel->setObjectName(QString::fromUtf8("fileLabel"));
        fileLabel->setAlignment(Qt::AlignCenter);

        verticalLayoutFile->addWidget(fileLabel);

        stackedWidgets->addWidget(fileWidget);

        horizontalLayout->addWidget(stackedWidgets);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Mini OS Simulator", nullptr));
        processLabel->setText(QCoreApplication::translate("MainWindow", "Process Scheduling Module", nullptr));
        syncLabel->setText(QCoreApplication::translate("MainWindow", "Process Synchronization Module", nullptr));
        memoryLabel->setText(QCoreApplication::translate("MainWindow", "Memory Management Module", nullptr));
        fileLabel->setText(QCoreApplication::translate("MainWindow", "File System Module", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
