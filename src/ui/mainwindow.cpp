#include "mainwindow.h"
#include "ui_mainwindow.h" 
#include "memory/memory.h"      
#include "sync/sync.h"          
#include "file/file.h"          
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QHeaderView>
#include <QFormLayout>
#include <QListWidgetItem>
#include <QDebug>
#include <QMessageBox> 
#include <QPainter>    
#include <QFrame>      
#include <QScrollBar>  
#include <QTime>       
#include <QThread>     
#include "process/process.h" 


// --- Constants ---
const int TOTAL_MEMORY_SIZE = 1024;
const int RR_TIME_QUANTUM = 2;
const int PC_BUFFER_SIZE = 5;

// --- Constructor & Destructor ---

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      // Initialize the backend modules
      scheduler(), 
      memoryManager(TOTAL_MEMORY_SIZE), 
      fileManager(),
      producerConsumer(PC_BUFFER_SIZE)
{
    ui->setupUi(this); 

    // Access the widgets from the UI file
    menuList = ui->menuList;
    stackedWidgets = ui->stackedWidgets;
    processWidget = ui->processWidget;
    syncWidget = ui->syncWidget;
    memoryWidget = ui->memoryWidget;
    fileWidget = ui->fileWidget;

    // Set up the aesthetic and functional components
    setupUI();
    setupConnections();
    
    // Initial UI Updates
    updateMemoryVisualization();
    updateFileList();
    updateBufferDisplay();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// =======================================================
// --- UI Setup Functions ---
// =======================================================

void MainWindow::setupUI()
{
    // 1. Navigation Menu Setup (QListWidget)
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/cpu.png"), "Process Scheduling"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/memory.png"), "Memory Management"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/sync.png"), "Process Synchronization"));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/file.png"), "File System"));
    menuList->setCurrentRow(0); // Start on the first tab

    // 2. Set styles for a beautiful experience
    setStyleSheet(R"(
        QMainWindow { background-color: #f0f0f0; }
        QListWidget { 
            background: #2c3e50; /* Dark sidebar */
            color: #ecf0f1; 
            border: none;
            max-width: 200px;
        }
        QListWidget::item { padding: 10px 5px; }
        QListWidget::item:selected { 
            background: #34495e; /* Slightly lighter on selection */
            border-left: 5px solid #3498db; /* Highlight bar */
        }
        QLabel#headerLabel { /* Use a specific ID for main titles */
            font-size: 20px; 
            font-weight: bold; 
            color: #2c3e50; 
            padding: 10px;
        }
        QPushButton { 
            background-color: #3498db; /* Blue for action buttons */
            color: white; 
            padding: 8px 16px; 
            border: none; 
            border-radius: 4px;
        }
        QPushButton:hover { background-color: #2980b9; }
        QGroupBox { 
            border: 1px solid #bdc3c7; 
            border-radius: 5px; 
            margin-top: 10px; 
            padding: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center; 
            padding: 0 5px;
            color: #2c3e50;
        }
    )");

    // 3. Populate Content Widgets
    setupProcessSchedulingUI(processWidget);
    setupMemoryManagementUI(memoryWidget);
    setupProcessSynchronizationUI(syncWidget);
    setupFileSystemUI(fileWidget);
}

void MainWindow::setupConnections()
{
    // Connect the list widget selection change to the stacked widget
    connect(menuList, &QListWidget::currentRowChanged, 
            stackedWidgets, &QStackedWidget::setCurrentIndex);

    // =======================================================
    // --- 1. Process Scheduling Connections ---
    // =======================================================
    connect(processWidget->findChild<QPushButton*>("addButton"), &QPushButton::clicked,
            this, &MainWindow::on_addButton_clicked);
            
    connect(processWidget->findChild<QPushButton*>("fcfsBtn"), &QPushButton::clicked,
            this, &MainWindow::runFCFS);

    connect(processWidget->findChild<QPushButton*>("sjfBtn"), &QPushButton::clicked,
            this, &MainWindow::runSJF);

    connect(processWidget->findChild<QPushButton*>("priorityBtn"), &QPushButton::clicked,
            this, &MainWindow::runPriorityScheduling);

    connect(processWidget->findChild<QPushButton*>("rrBtn"), &QPushButton::clicked,
            this, &MainWindow::runRoundRobin);
            
    connect(processWidget->findChild<QPushButton*>("resetBtn"), &QPushButton::clicked,
            this, &MainWindow::resetScheduler);

    updateProcessTable(scheduler.getProcesses());
    
    // ===============================================
    // --- 2. Memory Management Connections ---
    // ===============================================
    connect(memoryWidget->findChild<QPushButton*>("firstFitBtn"), &QPushButton::clicked,
            this, &MainWindow::on_firstFitBtn_clicked);

    connect(memoryWidget->findChild<QPushButton*>("bestFitBtn"), &QPushButton::clicked,
            this, &MainWindow::on_bestFitBtn_clicked);

    connect(memoryWidget->findChild<QPushButton*>("worstFitBtn"), &QPushButton::clicked,
            this, &MainWindow::on_worstFitBtn_clicked);

    connect(memoryWidget->findChild<QPushButton*>("deallocateBtn"), &QPushButton::clicked,
            this, &MainWindow::on_deallocateBtn_clicked);

    connect(this, &MainWindow::memoryUpdated, 
            this, &MainWindow::updateMemoryVisualization);


    // ========================================================
    // --- 3. Process Synchronization Connections ---
    // ========================================================
    connect(syncWidget->findChild<QPushButton*>("produceBtn"), &QPushButton::clicked,
            this, &MainWindow::on_produceBtn_clicked);

    connect(syncWidget->findChild<QPushButton*>("consumeBtn"), &QPushButton::clicked,
            this, &MainWindow::on_consumeBtn_clicked);

    // =============================================
    // --- 4. File System Connections ---
    // =============================================
    connect(fileWidget->findChild<QPushButton*>("createBtn"), &QPushButton::clicked,
            this, &MainWindow::on_createFileBtn_clicked);

    connect(fileWidget->findChild<QPushButton*>("writeBtn"), &QPushButton::clicked,
            this, &MainWindow::on_writeFileBtn_clicked);

    connect(fileWidget->findChild<QPushButton*>("readBtn"), &QPushButton::clicked,
            this, &MainWindow::on_readFileBtn_clicked);

    connect(fileWidget->findChild<QPushButton*>("deleteBtn"), &QPushButton::clicked,
            this, &MainWindow::on_deleteFileBtn_clicked);

    connect(fileWidget->findChild<QListWidget*>("fileList"), &QListWidget::currentRowChanged,
            this, &MainWindow::on_fileSelected);
            
}

void MainWindow::setupProcessSchedulingUI(QWidget *parent)
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(parent->layout());
    if (!layout) return;

    QLabel *header = parent->findChild<QLabel*>("processLabel");
    if (header) {
        header->setText("CPU Scheduling Algorithms");
        header->setObjectName("headerLabel");
    }

    // --- Input Group ---
    QGroupBox *inputGroup = new QGroupBox("Add New Process");
    QFormLayout *inputLayout = new QFormLayout(inputGroup);
    
    QLineEdit *pidInput = new QLineEdit("P1");
    QSpinBox *arrivalInput = new QSpinBox;
    QSpinBox *burstInput = new QSpinBox;
    QSpinBox *priorityInput = new QSpinBox;
    
    pidInput->setObjectName("pidInput");
    arrivalInput->setObjectName("arrivalInput");
    burstInput->setObjectName("burstInput");
    priorityInput->setObjectName("priorityInput");
    
    arrivalInput->setRange(0, 100);
    burstInput->setRange(1, 100);
    priorityInput->setRange(1, 10);
    priorityInput->setValue(5);
    
    QPushButton *addButton = new QPushButton("Add Process");
    addButton->setObjectName("addButton");
    
    inputLayout->addRow("PID:", pidInput); 
    inputLayout->addRow("Arrival Time:", arrivalInput);
    inputLayout->addRow("Burst Time:", burstInput);
    inputLayout->addRow("Priority (1=Highest):", priorityInput);
    inputLayout->addWidget(addButton);
    layout->addWidget(inputGroup);

    // --- Control Group ---
    QGroupBox *controlGroup = new QGroupBox("Run Scheduler");
    QHBoxLayout *controlLayout = new QHBoxLayout(controlGroup);
    
    QPushButton *fcfsBtn = new QPushButton("FCFS");
    QPushButton *sjfBtn = new QPushButton("SJF (Non-Preemptive)");
    QPushButton *priorityBtn = new QPushButton("Priority (Non-Preemptive)");
    QPushButton *rrBtn = new QPushButton(QString("Round Robin (Q=%1)").arg(RR_TIME_QUANTUM));
    QPushButton *resetBtn = new QPushButton("Reset");
    
    fcfsBtn->setObjectName("fcfsBtn");
    sjfBtn->setObjectName("sjfBtn");
    priorityBtn->setObjectName("priorityBtn");
    rrBtn->setObjectName("rrBtn");
    resetBtn->setObjectName("resetBtn");
    resetBtn->setStyleSheet("background-color: #f39c12;"); 

    controlLayout->addWidget(fcfsBtn);
    controlLayout->addWidget(sjfBtn);
    controlLayout->addWidget(priorityBtn);
    controlLayout->addWidget(rrBtn);
    controlLayout->addWidget(resetBtn);
    layout->addWidget(controlGroup);

    // --- Results Table ---
    QTableWidget *processTable = new QTableWidget(parent);
    processTable->setObjectName("processTable");
    processTable->setColumnCount(7);
    QStringList headers = {"PID", "Arrival", "Burst", "Priority", "Waiting Time", "Turnaround Time", "Status"};
    processTable->setHorizontalHeaderLabels(headers);
    processTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    processTable->setEditTriggers(QAbstractItemView::NoEditTriggers); 
    processTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(processTable, 1);
}

// =======================================================
// --- MEMORY MANAGEMENT VISUALIZATION CLASS (CRITICAL FIX) ---
// =======================================================

// FIX: Full class definition is required here, not just a forward declaration, 
// to resolve the 'invalid use of incomplete type' error for MemoryVisualizationFrame.
class MemoryVisualizationFrame : public QFrame
{
    Q_OBJECT 
public:
    MemoryVisualizationFrame(MemoryManager* manager, int totalSize, QWidget *parent = nullptr) 
        : QFrame(parent), memoryManager(manager), totalMemorySize(totalSize) {}

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);
        QPainter painter(this);
        int totalHeight = height();
        int totalWidth = width();
        
        // Draw the border
        painter.drawRect(0, 0, totalWidth - 1, totalHeight - 1);

        const auto& blocks = memoryManager->getMemoryBlocks();
        
        for (const auto& block : blocks)
        {
            // Calculate height and start position based on block size relative to total memory
            double ratio = (double)block.size / totalMemorySize;
            int h = static_cast<int>(totalHeight * ratio);
            int y = static_cast<int>(totalHeight * ((double)block.start / totalMemorySize));

            // Set colors based on allocation status
            if (block.isFree) {
                painter.setBrush(QBrush(QColor("#c8e6c9"))); // Light Green
                painter.setPen(QPen(QColor("#4caf50")));     // Dark Green
            } else {
                painter.setBrush(QBrush(QColor("#ffccbc"))); // Light Red/Orange
                painter.setPen(QPen(QColor("#ff5722")));     // Dark Red/Orange
            }

            // Draw the memory block rectangle
            painter.drawRect(0, y, totalWidth - 1, h);

            // Draw text label
            painter.setPen(Qt::black);
            QString text = block.isFree 
                           ? QString("FREE (%1 units)").arg(block.size)
                           : QString("%1 (%2 units)").arg(block.processName.c_str()).arg(block.size);
            
            QRect textRect(0, y, totalWidth - 1, h);
            painter.drawText(textRect, Qt::AlignCenter, text);

            // Draw start address 
            painter.setPen(Qt::darkGray);
            painter.drawText(QRect(-50, y - 10, totalWidth, 20), Qt::AlignLeft | Qt::AlignVCenter, QString::number(block.start));
        }
    }

private:
    MemoryManager* memoryManager;
    int totalMemorySize;
};

// Required wrapper to allow Qt to recognize the custom class paintEvent (Generated by moc)
#include "mainwindow.moc" 

void MainWindow::setupMemoryManagementUI(QWidget *parent)
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(parent->layout());
    if (!layout) return;

    QLabel *header = parent->findChild<QLabel*>("memoryLabel");
    if (header) {
        header->setText(QString("Memory Allocation (Total: %1 units)").arg(TOTAL_MEMORY_SIZE));
        header->setObjectName("headerLabel");
    }

    // --- Input & Control Group (Left Side) ---
    QGroupBox *controlGroup = new QGroupBox("Allocation Controls");
    QFormLayout *controlLayout = new QFormLayout(controlGroup);

    QLineEdit *processNameInput = new QLineEdit("P_A");
    QSpinBox *sizeInput = new QSpinBox;
    sizeInput->setRange(1, TOTAL_MEMORY_SIZE);
    
    processNameInput->setObjectName("memProcessNameInput");
    sizeInput->setObjectName("memSizeInput");

    QPushButton *firstFitBtn = new QPushButton("First Fit");
    QPushButton *bestFitBtn = new QPushButton("Best Fit");
    QPushButton *worstFitBtn = new QPushButton("Worst Fit");
    QPushButton *deallocateBtn = new QPushButton("Deallocate");
    
    firstFitBtn->setObjectName("firstFitBtn");
    bestFitBtn->setObjectName("bestFitBtn");
    worstFitBtn->setObjectName("worstFitBtn");
    deallocateBtn->setObjectName("deallocateBtn");
    deallocateBtn->setStyleSheet("background-color: #e74c3c;");

    controlLayout->addRow("Process Name:", processNameInput);
    controlLayout->addRow("Size (units):", sizeInput);
    controlLayout->addRow(firstFitBtn);
    controlLayout->addRow(bestFitBtn);
    controlLayout->addRow(worstFitBtn);
    controlLayout->addRow(deallocateBtn);


    // --- Memory Visualization (Right Side) ---
    MemoryVisualizationFrame *memoryVis = new MemoryVisualizationFrame(&memoryManager, TOTAL_MEMORY_SIZE);
    memoryVis->setObjectName("memoryVis");
    memoryVis->setFrameShape(QFrame::StyledPanel);
    memoryVis->setLineWidth(2);
    memoryVis->setStyleSheet("background-color: #ffffff; border: 1px solid #bdc3c7;");
    
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(controlGroup, 1);
    hLayout->addWidget(memoryVis, 3);
    
    layout->addLayout(hLayout, 1);
}

void MainWindow::setupProcessSynchronizationUI(QWidget *parent)
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(parent->layout());
    if (!layout) return;

    QLabel *header = parent->findChild<QLabel*>("syncLabel");
    if (header) {
        header->setText(QString("Producer-Consumer Problem (Buffer Size: %1)").arg(PC_BUFFER_SIZE));
        header->setObjectName("headerLabel");
    }

    // --- Buffer/Status Visualization ---
    QGroupBox *bufferGroup = new QGroupBox("Buffer Status");
    QHBoxLayout *bufferLayout = new QHBoxLayout(bufferGroup);
    
    QLabel *bufferDisplay = new QLabel("Buffer: [ ] (Empty)");
    bufferDisplay->setAlignment(Qt::AlignCenter);
    bufferDisplay->setObjectName("bufferDisplay");
    bufferDisplay->setFont(QFont("Monospace", 12, QFont::Bold));
    bufferLayout->addWidget(bufferDisplay);
    layout->addWidget(bufferGroup);

    // --- Controls ---
    QGroupBox *controlGroup = new QGroupBox("Actions");
    QHBoxLayout *controlLayout = new QHBoxLayout(controlGroup);
    
    QPushButton *produceBtn = new QPushButton("Produce Item");
    QPushButton *consumeBtn = new QPushButton("Consume Item");
    
    produceBtn->setObjectName("produceBtn");
    consumeBtn->setObjectName("consumeBtn");
    
    controlLayout->addWidget(produceBtn);
    controlLayout->addWidget(consumeBtn);
    layout->addWidget(controlGroup);

    // --- Activity Log ---
    QGroupBox *logGroup = new QGroupBox("Activity Log");
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);
    QTextEdit *logArea = new QTextEdit;
    logArea->setObjectName("syncLogArea");
    logArea->setReadOnly(true);
    logArea->setText("Synchronization activity will appear here...\n");
    logLayout->addWidget(logArea);
    layout->addWidget(logGroup, 1);
}

void MainWindow::setupFileSystemUI(QWidget *parent)
{
    QVBoxLayout *rootLayout = qobject_cast<QVBoxLayout*>(parent->layout());
    if (!rootLayout) return;

    QLabel *header = parent->findChild<QLabel*>("fileLabel");
    if (header) {
        header->setText("Simple File System (CRUD & Permissions)");
        header->setObjectName("headerLabel");
    }
    
    QHBoxLayout *mainLayout = new QHBoxLayout;

    // --- File List (Left Panel) ---
    QGroupBox *fileListGroup = new QGroupBox("Existing Files");
    QVBoxLayout *listLayout = new QVBoxLayout(fileListGroup);
    QListWidget *fileList = new QListWidget;
    fileList->setObjectName("fileList");
    listLayout->addWidget(fileList);
    mainLayout->addWidget(fileListGroup, 1);

    // --- File Details/Actions (Right Panel) ---
    QGroupBox *actionsGroup = new QGroupBox("File Actions");
    QFormLayout *actionsLayout = new QFormLayout(actionsGroup);

    QLineEdit *fileNameInput = new QLineEdit;
    QLineEdit *permissionsInput = new QLineEdit("rw");
    QTextEdit *contentInput = new QTextEdit;
    
    fileNameInput->setObjectName("fileNameInput");
    permissionsInput->setObjectName("permissionsInput");
    permissionsInput->setMaxLength(2);
    contentInput->setObjectName("contentInput");
    contentInput->setPlaceholderText("Enter content here...");
    
    QPushButton *createBtn = new QPushButton("Create File");
    QPushButton *writeBtn = new QPushButton("Write Content");
    QPushButton *readBtn = new QPushButton("Read File");
    QPushButton *deleteBtn = new QPushButton("Delete File");

    createBtn->setObjectName("createBtn");
    writeBtn->setObjectName("writeBtn");
    readBtn->setObjectName("readBtn");
    deleteBtn->setObjectName("deleteBtn");
    deleteBtn->setStyleSheet("background-color: #e74c3c;");

    actionsLayout->addRow("File Name:", fileNameInput);
    actionsLayout->addRow("Permissions (r/w):", permissionsInput);
    actionsLayout->addRow("Content:", contentInput);
    actionsLayout->addWidget(createBtn);
    actionsLayout->addWidget(writeBtn);
    actionsLayout->addWidget(readBtn);
    actionsLayout->addWidget(deleteBtn);
    
    mainLayout->addWidget(actionsGroup, 2);

    rootLayout->addLayout(mainLayout, 1);
}

// =======================================================
// --- PROCESS SCHEDULING LOGIC ---
// =======================================================

void MainWindow::updateProcessTable(const std::vector<Process>& processes)
{
    QTableWidget *table = processWidget->findChild<QTableWidget*>("processTable");
    if (!table) return;

    QStringList headers = {"PID", "Arrival", "Burst", "Priority", "Waiting Time", "Turnaround Time", "Status"};
    table->setRowCount(0); 
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    
    double totalWait = 0;
    double totalTurnaround = 0;
    int completedCount = 0;

    for (const auto& p : processes)
    {
        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(QString("P%1").arg(p.pid)));
        table->setItem(row, 1, new QTableWidgetItem(QString::number(p.arrivalTime)));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(p.burstTime)));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(p.priority)));

        if (p.remainingTime == 0) {
            table->setItem(row, 4, new QTableWidgetItem(QString::number(p.waitingTime)));
            table->setItem(row, 5, new QTableWidgetItem(QString::number(p.turnaroundTime)));
            
            QTableWidgetItem *statusItem = new QTableWidgetItem("Completed");
            statusItem->setForeground(QBrush(Qt::darkGreen));
            table->setItem(row, 6, statusItem);
            
            totalWait += p.waitingTime;
            totalTurnaround += p.turnaroundTime;
            completedCount++;
        } else {
            table->setItem(row, 4, new QTableWidgetItem("-"));
            table->setItem(row, 5, new QTableWidgetItem("-"));
            table->setItem(row, 6, new QTableWidgetItem("Ready"));
        }
    }

    if (completedCount > 0)
    {
        double avgWait = totalWait / completedCount;
        double avgTurnaround = totalTurnaround / completedCount;
        
        int row = table->rowCount();
        table->insertRow(row);
        
        QTableWidgetItem *avgLabel = new QTableWidgetItem("AVERAGE");
        avgLabel->setFont(QFont("Arial", 9, QFont::Bold));
        avgLabel->setBackground(QBrush(QColor("#f0f0f0")));
        table->setItem(row, 0, avgLabel);
        
        for(int col = 1; col <= 3; ++col) {
            table->setItem(row, col, new QTableWidgetItem(""));
            table->item(row, col)->setBackground(QBrush(QColor("#f0f0f0")));
        }
        
        QTableWidgetItem *avgWaitItem = new QTableWidgetItem(QString::number(avgWait, 'f', 2));
        avgWaitItem->setBackground(QBrush(QColor("#e0f7fa")));
        avgWaitItem->setFont(QFont("Arial", 9, QFont::Bold));
        table->setItem(row, 4, avgWaitItem);
        
        QTableWidgetItem *avgTATItem = new QTableWidgetItem(QString::number(avgTurnaround, 'f', 2));
        avgTATItem->setBackground(QBrush(QColor("#e0f7fa")));
        avgTATItem->setFont(QFont("Arial", 9, QFont::Bold));
        table->setItem(row, 5, avgTATItem);

        table->setItem(row, 6, new QTableWidgetItem(""));
        table->item(row, 6)->setBackground(QBrush(QColor("#f0f0f0")));
    }
}

void MainWindow::getProcessInputs(QString &pid, int &arrival, int &burst, int &priority)
{
    pid = processWidget->findChild<QLineEdit*>("pidInput")->text();
    arrival = processWidget->findChild<QSpinBox*>("arrivalInput")->value();
    burst = processWidget->findChild<QSpinBox*>("burstInput")->value();
    priority = processWidget->findChild<QSpinBox*>("priorityInput")->value();
}

void MainWindow::on_addButton_clicked()
{
    QString pidStr;
    int arrival, burst, priority;
    getProcessInputs(pidStr, arrival, burst, priority);

    if (pidStr.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Process ID cannot be empty.");
        return;
    }
    
    int intPid = 1;
    if (!scheduler.getProcesses().empty()) {
        intPid = scheduler.getProcesses().back().pid + 1;
    }

    scheduler.addProcess(intPid, arrival, burst, priority);
    
    processWidget->findChild<QLineEdit*>("pidInput")->setText(QString("P%1").arg(intPid + 1));

    updateProcessTable(scheduler.getProcesses());
    QMessageBox::information(this, "Success", QString("Process P%1 added (Burst: %2, Arrival: %3).").arg(intPid).arg(burst).arg(arrival));
}

void MainWindow::runFCFS()
{
    if (scheduler.getProcesses().empty()) {
        QMessageBox::information(this, "Scheduler", "No processes to run. Please add processes first.");
        return;
    }
    ProcessScheduler simScheduler = scheduler;
    simScheduler.FCFS();
    updateProcessTable(simScheduler.getProcesses());
}

void MainWindow::runSJF()
{
    if (scheduler.getProcesses().empty()) {
        QMessageBox::information(this, "Scheduler", "No processes to run. Please add processes first.");
        return;
    }
    ProcessScheduler simScheduler = scheduler;
    simScheduler.SJF();
    updateProcessTable(simScheduler.getProcesses());
}

void MainWindow::runPriorityScheduling()
{
    if (scheduler.getProcesses().empty()) {
        QMessageBox::information(this, "Scheduler", "No processes to run. Please add processes first.");
        return;
    }
    ProcessScheduler simScheduler = scheduler;
    simScheduler.PriorityScheduling();
    updateProcessTable(simScheduler.getProcesses());
}

void MainWindow::runRoundRobin()
{
    if (scheduler.getProcesses().empty()) {
        QMessageBox::information(this, "Scheduler", "No processes to run. Please add processes first.");
        return;
    }
    ProcessScheduler simScheduler = scheduler;
    simScheduler.RoundRobin(RR_TIME_QUANTUM);
    updateProcessTable(simScheduler.getProcesses());
}

void MainWindow::resetScheduler()
{
    scheduler = ProcessScheduler();
    
    processWidget->findChild<QLineEdit*>("pidInput")->setText("P1");
    processWidget->findChild<QSpinBox*>("arrivalInput")->setValue(0);
    processWidget->findChild<QSpinBox*>("burstInput")->setValue(1);
    processWidget->findChild<QSpinBox*>("priorityInput")->setValue(5);

    updateProcessTable(scheduler.getProcesses());
    QMessageBox::information(this, "Scheduler Reset", "All processes have been removed and the scheduler is ready.");
}


// =======================================================
// --- MEMORY MANAGEMENT LOGIC (Implementation) ---
// =======================================================

void MainWindow::updateMemoryVisualization()
{
    QFrame *visFrame = memoryWidget->findChild<QFrame*>("memoryVis");
    if (visFrame) {
        visFrame->update(); 
    }
}

// FIX: This signature now correctly matches the updated mainwindow.h declaration.
void MainWindow::runAllocation(MemoryManager::AllocationStrategy strategy, const QString& strategyName)
{
    QString processName = memoryWidget->findChild<QLineEdit*>("memProcessNameInput")->text();
    int size = memoryWidget->findChild<QSpinBox*>("memSizeInput")->value();

    if (processName.isEmpty() || size <= 0) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid Process Name and Size.");
        return;
    }
    
    std::string name = processName.toStdString();
    
    // Check if the process already exists
    for(const auto& block : memoryManager.getMemoryBlocks()) {
        if (!block.isFree && block.processName == name) {
            QMessageBox::warning(this, "Error", QString("Process '%1' is already allocated.").arg(processName));
            return;
        }
    }

    switch (strategy) {
        case MemoryManager::FIRST_FIT:
            memoryManager.allocateFirstFit(size, name);
            break;
        case MemoryManager::BEST_FIT:
            memoryManager.allocateBestFit(size, name);
            break;
        case MemoryManager::WORST_FIT:
            memoryManager.allocateWorstFit(size, name);
            break;
    }

    QMessageBox::information(this, "Attempt Complete", QString("Attempted %1 allocation for %2 (%3 units). Check visualization and console log for status.").arg(strategyName).arg(processName).arg(size));

    emit memoryUpdated(); 
}


// FIX: These calls now correctly match the updated function signature.
void MainWindow::on_firstFitBtn_clicked()
{
    runAllocation(MemoryManager::FIRST_FIT, "First Fit");
}

// FIX: These calls now correctly match the updated function signature.
void MainWindow::on_bestFitBtn_clicked()
{
    runAllocation(MemoryManager::BEST_FIT, "Best Fit");
}

// FIX: These calls now correctly match the updated function signature.
void MainWindow::on_worstFitBtn_clicked()
{
    runAllocation(MemoryManager::WORST_FIT, "Worst Fit");
}

void MainWindow::on_deallocateBtn_clicked()
{
    QString processName = memoryWidget->findChild<QLineEdit*>("memProcessNameInput")->text();

    if (processName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter the Process Name to deallocate.");
        return;
    }

    std::string name = processName.toStdString();
    
    memoryManager.deallocate(name);

    QMessageBox::information(this, "Deallocation Attempt", QString("Attempted to deallocate process '%1'. Check visualization.").arg(processName));
    emit memoryUpdated();
}


// =======================================================
// --- PROCESS SYNCHRONIZATION LOGIC ---
// =======================================================

// Helper to log activity in the QTextEdit
void MainWindow::logSyncActivity(const QString& message)
{
    QTextEdit *logArea = syncWidget->findChild<QTextEdit*>("syncLogArea");
    if (logArea) {
        logArea->append(QString("[%1] %2").arg(QTime::currentTime().toString("hh:mm:ss")).arg(message));
        QScrollBar *sb = logArea->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
}

// Helper to update the buffer visualization
void MainWindow::updateBufferDisplay()
{
    QLabel *bufferDisplay = syncWidget->findChild<QLabel*>("bufferDisplay");
    if (!bufferDisplay) return;
    
    bufferDisplay->setText("Buffer state available in console log via producer/consumer actions.");
}


void MainWindow::on_produceBtn_clicked()
{
    static int itemID = 1; 
    
    logSyncActivity(QString("Attempting to PRODUCE item %1...").arg(itemID));
    QMessageBox::information(this, "Synchronization Warning", "Since the thread logic is handled by pthreads in the backend, rapid clicking may cause the UI to briefly freeze if the buffer blocks.");
    
    try {
        producerConsumer.produce(itemID);
        logSyncActivity(QString("SUCCESS: Produced item %1.").arg(itemID));
        itemID++;
    } catch (...) {
        logSyncActivity("ERROR: Production failed or blocked.");
    }

    updateBufferDisplay();
}

void MainWindow::on_consumeBtn_clicked()
{
    logSyncActivity("Attempting to CONSUME item...");
    QMessageBox::information(this, "Synchronization Warning", "Since the thread logic is handled by pthreads in the backend, rapid clicking may cause the UI to briefly freeze if the buffer blocks.");

    int consumedItem = -1;

    try {
        consumedItem = producerConsumer.consume();
        logSyncActivity(QString("SUCCESS: Consumed item %1.").arg(consumedItem));
    } catch (...) {
        logSyncActivity("ERROR: Consumption failed or blocked.");
    }
    
    updateBufferDisplay();
}


// =======================================================
// --- FILE SYSTEM LOGIC ---
// =======================================================

void MainWindow::updateFileList()
{
    QListWidget *fileList = fileWidget->findChild<QListWidget*>("fileList");
    if (!fileList) return;

    fileList->clear();
    const auto& files = fileManager.getFiles();

    for (const auto& f : files)
    {
        QString itemText = QString("%1 (%2)").arg(f.name.c_str()).arg(f.permissions.c_str());
        QListWidgetItem *item = new QListWidgetItem(itemText); 
        item->setData(Qt::UserRole, QString::fromStdString(f.name)); 
        fileList->addItem(item);
    }
}

void MainWindow::on_fileSelected(int currentRow)
{
    QListWidget *fileList = fileWidget->findChild<QListWidget*>("fileList");
    QLineEdit *fileNameInput = fileWidget->findChild<QLineEdit*>("fileNameInput");
    QLineEdit *permissionsInput = fileWidget->findChild<QLineEdit*>("permissionsInput");
    QTextEdit *contentInput = fileWidget->findChild<QTextEdit*>("contentInput");
    
    if (!fileList || currentRow < 0) {
        fileNameInput->clear();
        permissionsInput->setText("rw");
        contentInput->clear();
        return;
    }

    QListWidgetItem *item = fileList->currentItem();
    QString name = item->data(Qt::UserRole).toString(); 
    
    std::string fileName = name.toStdString();
    const auto& files = fileManager.getFiles();
    const File* selectedFile = nullptr;
    for(const auto& f : files) {
        if (f.name == fileName) {
            selectedFile = &f;
            break;
        }
    }

    if (selectedFile) {
        fileNameInput->setText(name);
        permissionsInput->setText(QString::fromStdString(selectedFile->permissions));
        // Clear content when selecting a new file to avoid confusion, content is read separately
        contentInput->clear();
    }
}

void MainWindow::on_createFileBtn_clicked()
{
    QLineEdit *nameInput = fileWidget->findChild<QLineEdit*>("fileNameInput");
    QLineEdit *permInput = fileWidget->findChild<QLineEdit*>("permissionsInput");
    
    QString name = nameInput->text();
    QString permissions = permInput->text();

    if (name.isEmpty() || permissions.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "File Name and Permissions cannot be empty.");
        return;
    }

    fileManager.createFile(name.toStdString(), permissions.toStdString());
    updateFileList();
    QMessageBox::information(this, "File System", QString("File '%1' creation attempt complete (Check console log).").arg(name));
}

void MainWindow::on_writeFileBtn_clicked()
{
    QLineEdit *nameInput = fileWidget->findChild<QLineEdit*>("fileNameInput");
    QTextEdit *contentInput = fileWidget->findChild<QTextEdit*>("contentInput");
    
    QString name = nameInput->text();
    QString content = contentInput->toPlainText();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select a file or enter a name to write to.");
        return;
    }

    fileManager.writeFile(name.toStdString(), content.toStdString());
    QMessageBox::information(this, "File System", QString("Attempted to write to file '%1'. Check console/logs for permission result.").arg(name));
}

void MainWindow::on_readFileBtn_clicked()
{
    QLineEdit *nameInput = fileWidget->findChild<QLineEdit*>("fileNameInput");
    QTextEdit *contentInput = fileWidget->findChild<QTextEdit*>("contentInput");
    
    QString name = nameInput->text();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select a file or enter a name to read.");
        return;
    }

    std::string content = fileManager.readFile(name.toStdString());
    
    if (!content.empty()) {
        contentInput->setText(QString::fromStdString(content));
        QMessageBox::information(this, "File System", QString("File '%1' read successfully.").arg(name));
    } else {
        contentInput->setText("--- Read Failed (Check Permissions/Existence) ---");
        QMessageBox::warning(this, "File System", QString("Failed to read file '%1'. Check console/logs for error.").arg(name));
    }
}

void MainWindow::on_deleteFileBtn_clicked()
{
    QLineEdit *nameInput = fileWidget->findChild<QLineEdit*>("fileNameInput");
    QString name = nameInput->text();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select a file or enter a name to delete.");
        return;
    }

    fileManager.deleteFile(name.toStdString());
    updateFileList();
    QMessageBox::information(this, "File System", QString("Attempted to delete file '%1'. Check console/logs for status.").arg(name));
}