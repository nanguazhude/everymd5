#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "CRC32.hpp"
#include <memory>
#include <QtCore>
#include <QtWidgets>
#include <thread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_sButton_clicked(){
    const auto varDirName = QFileDialog::getExistingDirectory();
    if( varDirName.isEmpty() ){ return;  }
    ui->sEdit->setText( varDirName );
}

void MainWindow::on_doButton_clicked(){
    const auto varDirName = ui->sEdit->text() ;
    if( varDirName.isEmpty() ){ return;  }
    QDir varDir{ varDirName };
    if(false == varDir.mkpath("MD5.ans.utf8" ) ){
        return;
    }
    QDirIterator varI{ varDirName,QDir::NoDotAndDotDot | QDir::Files };
    QFile varWriteFile { varDir.absoluteFilePath( "MD5.ans.utf8/MD5.ans.utf8.txt") };
    if(false == varWriteFile.open( QFile::WriteOnly ) ){ return ; }
    QTextStream varStream(&varWriteFile);
    varStream.setCodec( QTextCodec::codecForName("UTF-8") );
    varStream.setGenerateByteOrderMark(true);
    varStream << QString::fromUtf8( u8R"_3_(文件名)_3_" )
              << QChar('\t')
              << QString::fromUtf8( u8R"_3_(MD5)_3_" )
              << QChar('\t')
              << QString::fromUtf8( u8R"_3_(SHA1)_3_" )
              << QChar('\t')
              << QString::fromUtf8( u8R"_3_(CRC32)_3_" )
              << endl;
    while (varI.hasNext()) {
        varI.next() ;
        QString varFileName ;
        QByteArray varMD5 ;
        QByteArray varSHA1 ;
        QByteArray varCRC32 ;
        {
            const auto varInfo = varI.fileInfo() ;
            varFileName = varInfo.fileName() ;
            const auto varAFileName = varInfo.absoluteFilePath();
            std::shared_ptr<QFile> varFiles[]={
                std::make_shared<QFile>(varAFileName),
                std::make_shared<QFile>(varAFileName),
                std::make_shared<QFile>(varAFileName)
            };

            if( false == varFiles[0]->open( QFile::ReadOnly ) ){
                continue;
            }
            if( false == varFiles[1]->open( QFile::ReadOnly ) ){
                continue;
            }
            if( false == varFiles[2]->open( QFile::ReadOnly ) ){
                continue;
            }

            std::thread varThread00{ [&varMD5 , varFile = varFiles[0] ](){
                        QCryptographicHash varHash{ QCryptographicHash::Md5 } ;
                        constexpr int varC = 1024 ;
                        char varTmpData[4+varC];
                        varTmpData[3+varC]=0;varTmpData[2+varC]=0;
                        varTmpData[1+varC]=0;varTmpData[0+varC]=0;
                        int varRead = 1;
                        while( (varRead = varFile->read(varTmpData,varC))>0 ){
                            varHash.addData( varTmpData ,varRead ); /*while*/}
                            varMD5 = varHash.result() ;
                         } };
        std::thread varThread01{  [&varSHA1 , varFile = varFiles[1] ](){
                    QCryptographicHash varHash{ QCryptographicHash::Sha1 } ;
                    constexpr int varC = 1024 ;
                    char varTmpData[4+varC];
                    varTmpData[3+varC]=0;varTmpData[2+varC]=0;
                    varTmpData[1+varC]=0;varTmpData[0+varC]=0;
                    int varRead = 1;
                    while( (varRead = varFile->read(varTmpData,varC))>0 ){
                        varHash.addData( varTmpData ,varRead ); /*while*/}
                        varSHA1 = varHash.result() ;
                    } };
    std::thread varThread02{  [&varCRC32 , varFile = varFiles[2] ](){
                CRC32 varHash  ;
                constexpr int varC = 1024 ;
                char varTmpData[4+varC];
                varTmpData[3+varC]=0;varTmpData[2+varC]=0;
                varTmpData[1+varC]=0;varTmpData[0+varC]=0;
                int varRead = 1;
                while( (varRead = varFile->read(varTmpData,varC))>0 ){
                    varHash.append( varTmpData ,varRead ); /*while*/}
                    varCRC32 = QByteArray::number( varHash.result(),16 )  ;
                } };

        varThread00.join();
        varThread01.join();
        varThread02.join();
    }

    varStream << varFileName
              << QChar('\t')
              << varMD5.toHex().toUpper()
              << QChar('\t')
              << varSHA1.toHex().toUpper()
              << QChar('\t')
              << varCRC32.toUpper()
              << endl;
}
}

/***************************************/



















