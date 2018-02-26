#ifndef _TESTTABLE_H_
#define _TESTTABLE_H_

#include <QListWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QDebug>
#include "node.h"


class TestTable1 : public QListWidget
{
    Q_OBJECT

public:
    TestTable1(Node *root, QWidget *parent = nullptr)
            : QListWidget(parent)
            , root_(root)
            , header_(QString("序号,指令标识,指令名称,参数判发,指令判发准则,判读参数,指令参数,超时时间").split(","))
    {
        setSelectionBehavior(QAbstractItemView::SelectRows);
        initHeader();
        initRecords();
        qDebug() << count();
    }

    void initHeader()
    {
        auto header = new QListWidgetItem;
        auto view = new QTableWidget;
        view->setRowCount(0);
        view->setColumnCount(header_.size());
        view->setHorizontalHeaderLabels(header_);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        header->setSizeHint(QSize(view->width(), 30));
        addItem(header);
        setItemWidget(header, view);
    }

    void initRecords()
    {
        for (auto node : root_->children)
        {
            auto table = new QTableWidget;
            table->setSelectionBehavior(QTableWidget::SelectRows);
            table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            table->setEditTriggers(QAbstractItemView::NoEditTriggers);
            table->setColumnCount(header_.size());
            table->horizontalHeader()->hide();
            table->verticalHeader()->hide();

            auto size = node->children.size();
            table->setRowCount(size + 1);
            table->setProperty("multirow", size > 0);

            addRecord(table, 0, node);
            for (int i = 0; i < size; ++i)
            {
                auto row = node->children.at(i);
                addRecord(table, i + 1, row);
            }

            auto item = new QListWidgetItem;
            item->setSizeHint(QSize(table->width(), table->rowCount() * 30));
            addItem(item);
            setItemWidget(item, table);
        }
    }

    void addRecord(QTableWidget *table, int row, Node *node)
    {
        //table->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        table->setItem(row, 1, new QTableWidgetItem(node->id));
        table->setItem(row, 2, new QTableWidgetItem(node->name));
        if (node->children.isEmpty())
        {
            table->setItem(row, 3, new QTableWidgetItem("Y"));
            table->setItem(row, 4, new QTableWidgetItem("P=0"));
            table->setItem(row, 5, new QTableWidgetItem("P=1"));
            table->setItem(row, 6, new QTableWidgetItem("1"));
            table->setItem(row, 7, new QTableWidgetItem("200"));
        }
    }


private:
    Node * root_;
    QStringList header_;
};


#endif // !_TESTTABLE_H
