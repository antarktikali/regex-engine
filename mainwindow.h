#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void fileDialog();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<std::string> lines;
};

#endif // MAINWINDOW_H
