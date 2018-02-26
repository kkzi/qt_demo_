#ifndef _NODE_H_
#define _NODE_H_

#pragma execution_character_set("utf-8")


struct Node
{
    QString id;
    QString name;
    Node *parent;
    QList<Node *> children;

    Node()
        : Node("", "")
    {

    }

    Node(const QString &id, const QString &name) : id(id), name(name), parent(nullptr)
    {

    }

    ~Node()
    {
        qDeleteAll(children);
        children.clear();
    };

    void addChild(Node *node)
    {
        children << node;
        node->parent = this;
    }
};



#endif // !_NODE_H_
