#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Main Window Creation
    QMainWindow::setWindowIcon(QIcon("icon.ico"));
    ui->setupUi(this);
    manager = new MailManager();
    copiesManager = new CopiesManager();
    //Mails TAB
    setMailsTab();
    //Copies TAB
    setCopiesTab();
    setCopiesPTab();
    //Definition of Export Menu
    setMenusBar();
}

MainWindow::~MainWindow()
{
    if (ui->viewSW->currentIndex() == 1){
        delete searchesWindow;
    }
    delete ui;
    delete manager;
    delete copiesManager;
}

void MainWindow::setMenusBar()
{
    //Export
    QList <QAction*> exports;
    QAction* exportCSV = new QAction("&CSV",this);
    QAction* exportOWN = new QAction("&Propierario",this);
    QIcon buttonExport("../Sources/export.ico");
    connect(exportCSV,SIGNAL(triggered()),this,SLOT(exportMailsCSV()));
    exportCSV->setIcon(buttonExport);
    connect(exportOWN,SIGNAL(triggered()),this,SLOT(exportMailsOWN()));
    exportOWN->setIcon(buttonExport);
    exports << exportCSV << exportOWN;
    ui->menubar->addMenu("&Exportar a")->addActions(exports);
    //Import
    QList <QAction*> imports;
    QAction* importCsv = new QAction("&CSV",this);
    QAction* importOwn = new QAction("&Propietario",this);
    QIcon buttonImport("../Sources/import.ico");
    connect(importCsv,SIGNAL(triggered()),this,SLOT(importCsvFile()));
    importCsv->setIcon(buttonImport);
    connect(importOwn,SIGNAL(triggered()),this,SLOT(importOwnFile()));
    importOwn->setIcon(buttonImport);
    imports << importCsv << importOwn;
    ui->menubar->addMenu("&Importar")->addActions(imports);

}

void MainWindow::setMailsTab()
{
    QPixmap addIcon("../Sources/add.ico");
    QIcon buttonAdd(addIcon);
    ui->composePB->setIcon(buttonAdd);
    ui->composePB->setIconSize(QSize(16,16));
    //Modify Icon
    QPixmap modifyIcon("../Sources/modify.ico");
    QIcon buttonModify(modifyIcon);
    ui->modifyPB->setIcon(buttonModify);
    ui->modifyPB->setIconSize(QSize(16,16));
    //Delete Icon
    QPixmap deleteIcon("../Sources/trash.ico");
    QIcon buttonDelete(deleteIcon);
    ui->deletePB->setIcon(buttonDelete);
    ui->deletePB->setIconSize(QSize(16,16));
    //Searches Icon
    QPixmap searchesIcon("../Sources/searches.ico");
    QIcon buttonSearches(searchesIcon);
    ui->searchesBT->setIcon(buttonSearches);
    ui->searchesBT->setIconSize(QSize(32,24));
    //TAB Mails
    QPixmap mailsTabIcon("../Sources/email.png");
    QIcon mailsTab(mailsTabIcon);
    ui->modulesTAB->setTabIcon(0,mailsTabIcon);//Logo
    QPixmap logo("../Sources/logo.png");
    ui->logoLB->setPixmap(logo);
    //Image Search.png
    QPixmap image("../Sources/search.png");
    ui->searchLE->addAction(image,QLineEdit::LeadingPosition);
}

void MainWindow::setCopiesTab()
{
    if (manager->fileHaveData(FILE_CSV)){
        ui->modulesTAB->setTabEnabled(1,true);
        ui->copiesTB->setEnabled(false);
    }
    QPixmap copiesTabIcon("../Sources/copies.ico");
    QIcon copiesTab(copiesTabIcon);
    ui->modulesTAB->setTabIcon(1,copiesTab);
    //Modify Icon
    QPixmap modifyIcon("../Sources/modify.ico");
    QIcon buttonModify(modifyIcon);
    ui->modifyCopy->setIcon(buttonModify);
    ui->modifyCopy->setIconSize(QSize(16,16));
    //Delete Icon
    QPixmap deleteIcon("../Sources/trash.ico");
    QIcon buttonDelete(deleteIcon);
    ui->deleteCopy->setIcon(buttonDelete);
    ui->deleteCopy->setIconSize(QSize(16,16));
    //Logo
    QPixmap logo("../Sources/logo.png");
    ui->logoLB_2->setPixmap(logo);
    //Image Search.png
    QPixmap image("../Sources/search.png");
    ui->searchCopy->addAction(image,QLineEdit::LeadingPosition);
}

void MainWindow::setCopiesPTab()
{
    if (manager->fileHaveData(FILE_OWNER)){
        ui->modulesTAB->setTabEnabled(2,true);
        ui->copiesPTB->setEnabled(false);
    }
    QPixmap copiesTabIcon("../Sources/copies.ico");
    QIcon copiesTab(copiesTabIcon);
    ui->modulesTAB->setTabIcon(2,copiesTab);
    //Modify Icon
    QPixmap modifyIcon("../Sources/modify.ico");
    QIcon buttonModify(modifyIcon);
    ui->modifyPCopyBT->setIcon(buttonModify);
    ui->modifyPCopyBT->setIconSize(QSize(16,16));
    //Delete Icon
    QPixmap deleteIcon("../Sources/trash.ico");
    QIcon buttonDelete(deleteIcon);
    ui->deletePCopyBT->setIcon(buttonDelete);
    ui->deletePCopyBT->setIconSize(QSize(16,16));
    //Logo
    QPixmap logo("../Sources/logo.png");
    ui->logoLB_3->setPixmap(logo);
    //Image Search.png
    QPixmap image("../Sources/search.png");
    ui->searchPCopy->addAction(image,QLineEdit::LeadingPosition);
}

void MainWindow::on_composePB_clicked()
{
    manager->composeMail();
}

void MainWindow::on_modifyPB_clicked()
{
    unsigned int id = ui->mailsTB->item(rowSelected,0)->text().toULong();
    manager->modifyMail(id,ui->mailsTB->item(rowSelected,SENDER)->text());
    disableModifyDeleteBT();
}

void MainWindow::on_searchLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    disableSearchBT();
}

void MainWindow::disableSearchBT()
{
    if (!ui->searchLE->text().isEmpty()){
        ui->searchBT->setEnabled(true);
    }
    else{
        ui->searchBT->setEnabled(false);
    }
}

void MainWindow::disableSearchCopyBT()
{
    if (!ui->searchCopy->text().isEmpty()){
        ui->searchCopyBT->setEnabled(true);
    }
    else{
        ui->searchCopyBT->setEnabled(false);
    }
}

void MainWindow::disableSearchCopyPBT()
{
    if (!ui->searchPCopy->text().isEmpty()){
        ui->searchPCopyBT->setEnabled(true);
    }
    else{
        ui->searchPCopyBT->setEnabled(false);
    }
}

void MainWindow::cipherCSV(const QString& password)
{
    manager->cipherMails(password.toStdString());
}

void MainWindow::decipherCSV(const QString &password)
{
    Q_UNUSED(password);
    manager->decipherMails();
    manager->importCsv(CSV_FILE);
}

void MainWindow::exportMailsCSV()
{
    if (ui->cipherCB->isChecked()){
        pb = new PasswordBox();
        pb->setModal(true);
        pb->setWindowTitle("Atención");
        pb->changePage(0);
        connect(pb,SIGNAL(cipher(const QString&)),this,SLOT(cipherCSV(const QString&)));
        pb->exec();
    }
    else{
        manager->exportMails(FILE_CSV);
    }
    ui->modulesTAB->setTabEnabled(1,true);
}

void MainWindow::exportMailsOWN()
{
    manager->exportMails(FILE_OWNER);
    ui->modulesTAB->setTabEnabled(2,true);
}

void MainWindow::on_searchBT_clicked()
{
    manager->searchMail(ui->mailsTB,ui->searchLE->text());
    ui->mailsTB->setEnabled(true);
}
void MainWindow::on_mailsTB_doubleClicked(const QModelIndex &index)
{
    rowSelected = index.row();
    ui->deletePB->setEnabled(true);
    ui->modifyPB->setEnabled(true);
}

void MainWindow::on_deletePB_clicked()
{
    unsigned int id = ui->mailsTB->item(rowSelected,0)->text().toULong();
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Atención",
        "¿Seguro que quiere eliminar este correo?",QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes){
        manager->deleteEmail(id,ui->searchLE->text());
    }
    else{
        QMessageBox message;
        message.setIcon(QMessageBox::Information);
        message.setText("Se ha conservado el correo.");
        message.exec();
    }
    ui->searchLE->setEnabled(true);
    disableModifyDeleteBT();
    ui->searchLE->clear();
    ui->mailsTB->clear();
}

void MainWindow::disableModifyDeleteBT()
{
    if (ui->searchLE->isEnabled()){
        ui->deletePB->setEnabled(false);
        ui->modifyPB->setEnabled(false);
    }
    else{
        ui->deletePB->setEnabled(true);
        ui->modifyPB->setEnabled(true);
    }
}

void MainWindow::disableModifyDeleteCopiesBT()
{
    if (ui->searchCopy->isEnabled()){
        ui->deleteCopy->setEnabled(true);
        ui->modifyCopy->setEnabled(true);
    }
    else{
        ui->deleteCopy->setEnabled(false);
        ui->modifyCopy->setEnabled(false);
    }
}

void MainWindow::disableModifyDeleteCopiesPBT()
{
    if (ui->searchPCopy->isEnabled()){
        ui->deletePCopyBT->setEnabled(true);
        ui->modifyPCopyBT->setEnabled(true);
    }
    else{
        ui->deletePCopyBT->setEnabled(false);
        ui->modifyPCopyBT->setEnabled(false);
    }
}

void MainWindow::on_searchCopy_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    disableSearchCopyBT();
}

void MainWindow::on_searchCopyBT_clicked()
{
    if (!copiesManager->showCopiesTable(ui->copiesTB,ui->searchCopy->text().toUInt())){
        QMessageBox::critical(0,"Error","No existe correo con ese ID.");
        ui->copiesTB->clearContents();
        ui->copiesTB->setEnabled(false);
    }
    else{
        ui->copiesTB->setEnabled(true);
    }
    ui->searchCopy->clear();
}

void MainWindow::on_searchPCopy_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    disableSearchCopyPBT();
}

void MainWindow::on_searchPCopyBT_clicked()
{
    if (!copiesManager->showCopiesPTable(ui->copiesPTB,ui->searchPCopy->text().toUInt())){
        QMessageBox::critical(0,"Error","No existe correo con ese ID.");
        ui->copiesPTB->clearContents();
        ui->copiesPTB->setEnabled(false);
    }
    else{
        ui->copiesPTB->setEnabled(true);
    }
    ui->searchPCopy->clear();
}

void MainWindow::on_copiesTB_doubleClicked(const QModelIndex &index)
{
    columnSelected = index.column();
    ui->deleteCopy->setEnabled(true);
    ui->modifyCopy->setEnabled(true);
}

void MainWindow::on_copiesPTB_doubleClicked(const QModelIndex &index)
{
    columnSelected = index.column();
    ui->deletePCopyBT->setEnabled(true);
    ui->modifyPCopyBT->setEnabled(true);
}

void MainWindow::on_modifyCopy_clicked()
{
    unsigned int id = ui->copiesTB->item(ID,columnSelected)->text().toUInt();
    copiesManager->modifyCopy(id,FILE_CSV);
    disableModifyDeleteCopiesBT();
}

void MainWindow::on_modifyPCopyBT_clicked()
{
    unsigned int id = ui->copiesPTB->item(ID,columnSelected)->text().toUInt();
    copiesManager->modifyCopy(id,FILE_OWNER);
    disableModifyDeleteCopiesPBT();
}

void MainWindow::on_deleteCopy_clicked()
{
    unsigned int id = ui->copiesTB->item(ID,columnSelected)->text().toUInt();
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Atención",
        "¿Seguro que quiere eliminar esta copia?",QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes){
        copiesManager->deleteCopy(id,FILE_CSV);
    }
    else{
        QMessageBox message;
        message.setIcon(QMessageBox::Information);
        message.setText("Se ha conservado el correo.");
        message.exec();
    }
    disableModifyDeleteCopiesBT();
}

void MainWindow::on_deletePCopyBT_clicked()
{
    unsigned int id = ui->copiesPTB->item(ID,columnSelected)->text().toUInt();
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Atención",
        "¿Seguro que quiere eliminar esta copia?",QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes){
        copiesManager->deleteCopy(id,FILE_OWNER);
    }
    else{
        QMessageBox message;
        message.setIcon(QMessageBox::Information);
        message.setText("Se ha conservado el correo.");
        message.exec();
    }
    disableModifyDeleteCopiesBT();
}

void MainWindow::importCsvFile()
{
    QString name;
    std::fstream file;
    name = QFileDialog::getOpenFileName(this,"Seleccione el archivo","../Files/","CSV files (*.csv)");
    file.open(CSV_FILE,std::ios::in | std::ios::out);
    if (file.is_open()){
        int isEncrypted;
        file >> isEncrypted;
        if (isEncrypted == -1){
            std::string password;
            CSVFile csv;
            file >> password;
            file.close();
            pb = new PasswordBox();
            pb->setModal(true);
            pb->changePage(1);
            pb->setPasswordToDecipher(csv.desencrypt(password));
            connect(pb,SIGNAL(decipher(const QString&)),this,SLOT(decipherCSV(const QString&)));
            pb->exec();
        }
        else{
            manager->importCsv(name);
        }
    }
}

void MainWindow::importOwnFile()
{
    manager->importOwner();
}

void MainWindow::on_searchesBT_clicked()
{
    searchesWindow = new SearchesWindow();
    searchesWindow->setIndex(manager->getIndex());
    ui->viewSW->insertWidget(1,searchesWindow);
    ui->viewSW->setCurrentIndex(1);
    connect(searchesWindow,SIGNAL(destroyed()),this,SLOT(changeIndexPage()));
}

void MainWindow::changeIndexPage()
{
    ui->viewSW->setCurrentIndex(0);
}
