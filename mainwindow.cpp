#include "mainwindow.h"
#include <QStringList>
#include <QFile>
#include <QDebug>
#include <QTreeView>
#include "testtable0.h"
#include "testtable1.h"
#include "testtable2.h"


#pragma execution_character_set("utf-8")

Node *rootNode()
{
    auto node1 = new Node("OC80-0001", "CPU三机加电");
    auto node2 = new Node("PRESET_XH", "循环子序列1");
    auto node3 = new Node("PRESET_XH", "循环子序列2");
    auto node4 = new Node("PRESET_XH", "循环子序列3");
    auto node5 = new Node("OC80-0011", "CPU三机断电");
    auto node6 = new Node("PRESET_TZ", "跳转");
    auto node21 = new Node("PRESET_XH", "循环子序列21");
    node21->addChild(new Node("OC80-0002", "CPUA加电"));
    node21->addChild(new Node("OC80-0003", "CPUA断电"));
    node2->addChild(new Node("OC80-0002", "CPUA加电"));
    node2->addChild(new Node("0C80-0003", "CPUA断电"));
    //node2->addChild(node21);
    node3->addChild(new Node("OC80-0004", "CPUB加电"));
    node3->addChild(new Node("0C80-0005", "CPUB断电"));
    node4->addChild(new Node("OC80-0006", "CPUC加电"));
    node4->addChild(new Node("0C80-0007", "CPUC断电"));
    node6->addChild(new Node("0C80-0009", "存储复接主加电 "));
    node6->addChild(new Node("0C80-0011", "存储复接备加电 "));
    node6->addChild(new Node("0C80-0010", "存储复接主断电 "));
    node6->addChild(new Node("0C80-0012", "存储复接备断电 "));

    auto root = new Node;
    root->addChild(node1);
    root->addChild(node2);
	root->addChild(node3);
	root->addChild(node4);
	root->addChild(node5);
	root->addChild(node6);
    return root;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //auto view = new TestTable0(rootNode());
    //auto view = new TestTable1(rootNode());
    auto view = new TestTable2(rootNode());

	setWindowTitle("demo");
    setCentralWidget(view);
    resize(820, 500);

    QFile file("tree.css");
    file.open(QFile::ReadOnly | QFile::Text);
    view->setStyleSheet(file.readAll());
    file.close();
}

MainWindow::~MainWindow()
{

}

