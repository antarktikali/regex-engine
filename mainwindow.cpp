#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "infixtopostfixconverter.h"
#include "nfa.h"
#include <string>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// The function is called when the Open File button is clicked.
// If a file is chosen, it reads the lines and adds them to a queue to be read
// later, and prints them to the text edit field.
void MainWindow::fileDialog()
{
    // Get the file path from the file dialog.
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"));

    QFile inputFile(filePath);
    // Try to open the file
    if(inputFile.open(QIODevice::ReadOnly))
    {
        // Clear the queue holding the lines.
        this->lines.clear();
        // Clear the text edit field.
        ui->textEdit->clear();

        // Read the lines of the file
        QTextStream textStream(&inputFile);
        while(!textStream.atEnd())
        {
            QString line = textStream.readLine();
            // Add the line to the queue
            this->lines.push_back(line.toStdString());
            // Add the line to the text edit field.
            ui->textEdit->append(line);
        }

        // Close the fileand show a message in the status bar
        inputFile.close();
        ui->statusBar->showMessage("File read successfull");
    } else {
        // Show an error message if the file was not read
        ui->statusBar->showMessage("Failed to read the file");
    }
}

// Function is called when the Match button is clicked for processing the text.
void MainWindow::on_pushButton_clicked()
{
    // Continue if there is a regular expression.
    if(ui->lineEdit->text().size() > 0)
    {
        // Convert the given infix expression to postfix.
        InfixToPostfixConverter infix(ui->lineEdit->text().toStdString());

        // Set a bool for checking if the conversion was successful.
        // If the conversion was unsuccessful, the following function would
        // send us an error message instead of a postfix expression.
        bool success = false;
        string expression = infix.getPosfixExpression(&success);

        // Conversion was successfull, the string is a postfix expression.
        if(success) {
            NFA nfa = NFA::postfixToNfa(expression);

            // Clear the text edit field.
            ui->textEdit->clear();

            // Set a counter to count the matched lines
            int counter = 0;
            // Set a timer to check how long did it take to process the text
            QTime timer(0,0,0,0);
            // Start the timer
            timer.restart();
            // Show a message in the status bar
            ui->statusBar->showMessage("Processing the text...");

            // Read all the lines from the queue
            for(string line : this->lines)
            {
                // Check if the NFA accepts the line
                if(nfa.accepts(line))
                {
                    // Set the background color to highlight the new line
                    ui->textEdit->setTextBackgroundColor(QColor("#ffffaa"));
                    // Increase the counter for matched lines
                    counter++;
                } else {
                    // If NFA doesn't accept, set the background back to white
                    ui->textEdit->setTextBackgroundColor(QColor("#ffffff"));
                }
                // Print the line to the text edit field.
                ui->textEdit->append(QString::fromStdString(line));
            }
            
            // Set the background color back to white in case the last line was
            // highlighted.
            ui->textEdit->setTextBackgroundColor(QColor("#ffffff"));

            // If the operation was successfull, print a message in the
            // status bar.
            ui->statusBar->showMessage("Operation completed in " +
                        QString::number(timer.elapsed()/1000.0) +
                            "sec. " + QString::number(counter) +
                                                "line(s) matched");
        } else {
            // If we received an error from the postfix conversion in the
            // beginning, only show that message in the status bar.
            ui->statusBar->showMessage("Error: " + QString::fromStdString(expression));
        }
    }
}
