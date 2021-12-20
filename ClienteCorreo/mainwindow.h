#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "copiesmanager.h"
#include "mailmanager.h"
#include "searcheswindow.h"
#include "passwordbox.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setMenusBar();
    void setMailsTab();
    void setCopiesTab();
    void setCopiesPTab();

private slots:
    void on_composePB_clicked();

    void on_modifyPB_clicked();

    void on_searchBT_clicked();

    void on_searchLE_textChanged(const QString &arg1);

    void on_mailsTB_doubleClicked(const QModelIndex &index);

    void on_deletePB_clicked();

    void disableModifyDeleteBT();

    void disableModifyDeleteCopiesBT();

    void disableModifyDeleteCopiesPBT();

    void disableSearchBT();

    void disableSearchCopyBT();

    void disableSearchCopyPBT();

    void exportMailsCSV();

    void exportMailsOWN();

    void on_searchCopy_textChanged(const QString &arg1);

    void on_searchCopyBT_clicked();

    void on_searchPCopy_textChanged(const QString &arg1);

    void on_copiesTB_doubleClicked(const QModelIndex &index);

    void on_modifyCopy_clicked();

    void on_deleteCopy_clicked();

    void importCsvFile();

    void importOwnFile();

    void on_searchPCopyBT_clicked();

    void on_modifyPCopyBT_clicked();

    void on_copiesPTB_doubleClicked(const QModelIndex &index);

    void on_deletePCopyBT_clicked();

    void on_searchesBT_clicked();

    void changeIndexPage();

    void cipherCSV(const QString& password);

    void decipherCSV(const QString& password);

private:
    Ui::MainWindow *ui;
    MailManager* manager;
    CopiesManager* copiesManager;
    SearchesWindow* searchesWindow;
    PasswordBox* pb;
    unsigned int rowSelected,columnSelected;
};
#endif // MAINWINDOW_H
