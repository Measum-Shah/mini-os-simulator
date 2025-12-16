#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include "process/process.h"   
#include "memory/memory.h"     // CRITICAL: Must be included for MemoryManager::AllocationStrategy
#include "sync/sync.h"         
#include "file/file.h"         

namespace Ui {
class MainWindow;
}

// Custom class to handle the memory visualization painting
class MemoryVisualizationFrame;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void memoryUpdated();

private:
    // --- PRIVATE MEMBERS (UI & BACKEND) ---
    Ui::MainWindow *ui;
    
    QListWidget *menuList;
    QStackedWidget *stackedWidgets;
    QWidget *processWidget;
    QWidget *syncWidget;
    QWidget *memoryWidget;
    QWidget *fileWidget;

    // Backend Modules
    ProcessScheduler scheduler; 
    MemoryManager memoryManager; 
    FileManager fileManager;
    ProducerConsumer producerConsumer;
    
    // --- PRIVATE HELPER FUNCTIONS ---
    void setupUI();
    void setupConnections();
    
    void setupProcessSchedulingUI(QWidget *parent);
    void setupMemoryManagementUI(QWidget *parent);
    void setupProcessSynchronizationUI(QWidget *parent);
    void setupFileSystemUI(QWidget *parent);

    // Process Scheduling Helpers
    void updateProcessTable(const std::vector<Process>& processes);
    void getProcessInputs(QString &pid, int &arrival, int &burst, int &priority);
    
    // Memory Management Helpers
    // FIX: Updated signature to include the AllocationStrategy enum
    void runAllocation(MemoryManager::AllocationStrategy strategy, const QString& strategyName); 
    
    // Synchronization Helpers
    void logSyncActivity(const QString& message);
    
    // File System Helpers
    void updateFileList();


// --- PRIVATE SLOTS ---
private slots:
    // --- Process Scheduling Slots ---
    void on_addButton_clicked();
    void runFCFS();
    void runSJF();
    void runPriorityScheduling();
    void runRoundRobin();
    void resetScheduler();

    // --- Memory Management Slots ---
    void on_firstFitBtn_clicked();
    void on_bestFitBtn_clicked();
    void on_worstFitBtn_clicked();
    void on_deallocateBtn_clicked();
    void updateMemoryVisualization(); 

    // --- Process Synchronization Slots ---
    void on_produceBtn_clicked();
    void on_consumeBtn_clicked();
    void updateBufferDisplay();

    // --- File System Slots ---
    void on_createFileBtn_clicked();
    void on_writeFileBtn_clicked();
    void on_readFileBtn_clicked();
    void on_deleteFileBtn_clicked();
    void on_fileSelected(int currentRow);
};

#endif // MAINWINDOW_H