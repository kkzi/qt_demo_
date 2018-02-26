#ifndef TESTMODEL_H
#define TESTMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QTreeView>
#include "node.h"


class TestModel0 : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit TestModel0(Node *root, QObject *parent = Q_NULLPTR)
		: QAbstractItemModel(parent)
		, root_(root)
	{
		header_ = QString("序号,指令标识,指令名称,参数判发,指令判发准则,判读参数,指令参数,超时时间").split(",");
	}

	~TestModel0()
	{

	}

	int rowCount(const QModelIndex &parent = QModelIndex()) const
	{
		if (!parent.isValid()) return root_->children.size();

		auto node = (Node *)parent.internalPointer();
		return node->children.size();
	}

	int columnCount(const QModelIndex &parent = QModelIndex()) const
	{
		Q_UNUSED(parent);
		return header_.size();
	}

	QModelIndex parent(const QModelIndex &index) const
	{
		auto node = (Node *)index.internalPointer();
		auto row = root_->children.indexOf(node->parent);
		return createIndex(row, 0, node->parent);
	}

	bool hasChildren(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE
	{
		if (!parent.isValid()) return true;
		auto node = (Node *)parent.internalPointer();
		return node->children.size() > 0;
	}

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const
	{
		auto node = (Node *)parent.internalPointer();
		if (node == nullptr)
		{
			node = root_;
		}
		auto child = node->children.at(row);
		return createIndex(row, column, child);
	}

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE
	{
		if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
			return QAbstractItemModel::headerData(section, orientation, role);
		return section < header_.size() ? header_.at(section) : QVariant();
	}

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE
	{
		if (!index.isValid()) return QVariant();

		auto node = (Node *)index.internalPointer();
		auto hasChildren = node->children.size() > 0;
		auto isLevel1 = node->parent == root_;

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

private:
	Node * root_;
	QStringList header_;
};


class TestTable0 : public QTreeView
{
public:
	explicit TestTable0(Node *root, QWidget *parent = Q_NULLPTR)
		: QTreeView(parent)
	{
		auto model = new TestModel0(root);
		setModel(model);
		setRootIsDecorated(0);
		setSelectionBehavior(QTreeView::SelectRows);
		setAutoFillBackground(true);
		setIndentation(2);
		expandAll();
	}

};

#endif // TESTMODEL_H
