#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QList>

class SideBar : public QWidget
{
    Q_OBJECT

public:
    SideBar(QWidget* parent = nullptr);

    void addCategory(const QString& name);
    void addNode(std::string category_, std::string node_name);
    void buildCats();

    struct Category
    {
        QPushButton* header;
        QWidget* container;
        QVBoxLayout* layout;
        bool expanded = false;
    };

    QVBoxLayout* main_layout;
    QList<Category> categories;
    Category* findCategory(const QString& name);

private slots:
    void toggleCategory();
};
