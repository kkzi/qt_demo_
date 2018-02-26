#ifndef TESTTABLE2_H
#define TESTTABLE2_H

#include <QAbstractTableModel>
#include <QBrush>
#include <QMenu>
#include <QTableView>
#include <QHeaderView>
#include <QDebug>
#include "node.h"

#pragma execution_character_set("utf-8")

class TestModel2 : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit TestModel2(Node *root, QObject *parent = Q_NULLPTR)
		: QAbstractTableModel(parent)
		, tree_(root)
		, list_(tree_->children)
		, header_(QString("序号,指令标识,指令名称,参数判发,指令判发准则,判读参数,指令参数,超时时间").split(","))
	{
		for (int i = 0; i < header_.size(); ++i)
		{
			setHeaderData(i, Qt::Horizontal, header_.at(i));
		}
	}

	~TestModel2()
	{

	}

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
	{
		if (role != Qt::DisplayRole)  return QAbstractTableModel::headerData(section, orientation, role);
		if (orientation == Qt::Horizontal) return header_.at(section);

		auto node = list_.at(section);
		return breakpoint_state_.value(node, false) ? "*" : QVariant();
	}

	QModelIndex index(int row, int column, const QModelIndex &parent) const
	{
		Q_UNUSED(parent);
		if (row >= list_.size()) return QModelIndex();
		auto node = list_.at(row);
		return createIndex(row, column, node);
	}

	QVariant data(const QModelIndex &index, int role) const
	{
		if (!index.isValid()) return QVariant();

		auto node = (Node *)index.internalPointer();
		auto hasChildren = node->children.size() > 0;
		auto isLevel1 = node->parent == tree_;

		if (role == Qt::BackgroundRole && hasChildren && isLevel1)
		{
			return QBrush(QColor(200, 200, 220));
		}

		if (role != Qt::DisplayRole) return QVariant();
		switch (index.column())
		{
		case 0:
			//return index.row() + 1;
			return isLevel1 ? index.row() + 1 : QVariant();
		case 1:
			return node->id;
		case 2:
			return node->name;
		case 3:
			return hasChildren ? "" : "Y";
		case 4:
			return hasChildren ? "" : "P=0";
		case 5:
			return hasChildren ? "" : "P=1";
		case 6:
			return hasChildren ? "" : "1";
		case 7:
			return hasChildren ? "" : "200";
		default: return QVariant();
		}
	}

	Qt::ItemFlags flags(const QModelIndex &index) const
	{
		auto node = (Node *)index.internalPointer();
		if (!node->children.isEmpty())
		{
			return QAbstractItemModel::flags(index) & ~Qt::ItemIsSelectable;
		}
		return QAbstractItemModel::flags(index);
	}

	int rowCount(const QModelIndex &parent = QModelIndex()) const
	{
		Q_UNUSED(parent);
		return list_.size();
	}

	int columnCount(const QModelIndex &parent = QModelIndex()) const
	{
		Q_UNUSED(parent);
		return header_.size();
	}

	QModelIndex parent(const QModelIndex &index) const
	{
		Q_UNUSED(index);
		return QModelIndex();
	}

	void showContextMenu(const QPoint &pos)
	{
		auto view = (QTableView *)sender();
		auto index = view->indexAt(pos);
		auto node = (Node *)index.internalPointer();

		QMenu menu;
		if (node->children.isEmpty())
		{
			bool broke = breakpoint_state_.value(node, false);
			auto row = list_.indexOf(node);
			if (broke)
			{
				menu.addAction("移除断点", [=]() {
					breakpoint_state_[node] = false;
					emit headerDataChanged(Qt::Vertical, row, row);
				});
			}
			else
			{
				menu.addAction("添加断点", [=]() {
					breakpoint_state_[node] = true;
					emit headerDataChanged(Qt::Vertical, row, row);
				});

			}
		}
		else
		{
			auto expanded = expand_state_.value(node, false);
			if (expanded)
			{
				menu.addAction("折叠", [=]() {
					beginRemoveRows(QModelIndex(), index.row() + 1, index.row() + node->children.size());
					for (int i = 0; i < node->children.size(); ++i)
					{
						auto idx = index.row() + 1;
						list_.removeAt(idx);
					}
					endRemoveRows();
					expand_state_[node] = false;

				});
			}
			else
			{
				menu.addAction("展开", [=]() {
					beginInsertRows(QModelIndex(), index.row(), index.row() + node->children.size() - 1);
					for (int i = 0; i < node->children.size(); ++i)
					{
						list_.insert(index.row() + i + 1, node->children.at(i));
					}
					endInsertRows();
					expand_state_[node] = true;
				});
			}
		}

		menu.exec(QCursor::pos());
	}

private:
	Node * tree_;
	QList<Node *> list_;
	QStringList header_;
	QMap<Node *, bool> expand_state_;
	QMap<Node *, bool> breakpoint_state_;
};




class TestTable2 : public QTableView
{
public:
	TestTable2(Node *root, QWidget *parent = nullptr)
		: QTableView(parent)
	{
		auto model = new TestModel2(root);
		verticalHeader()->setFixedWidth(14);
		horizontalHeader()->setHighlightSections(false);
		setFocusPolicy(Qt::NoFocus);
		setSelectionBehavior(QTableView::SelectRows);
		setContextMenuPolicy(Qt::CustomContextMenu);
		setModel(model);
		setColumnWidth(0, 60);
		connect(this, &TestTable2::customContextMenuRequested, model, &TestModel2::showContextMenu);
	}

private:
	QTableView * view_;
};


#endif // TESTTABLE2_H
