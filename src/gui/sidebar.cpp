#include "sidebar.h"
#include "draggable-label.h"

#include <QDebug>


SideBar::SideBar(QWidget* parent)
    : QWidget(parent)
{
    main_layout = new QVBoxLayout(this);
    main_layout->setAlignment(Qt::AlignTop);
    buildCats();
}


void SideBar::addCategory(const QString& name)
{
    Category cat;

    cat.header = new QPushButton(name);
    cat.container = new QWidget();

    cat.layout = new QVBoxLayout(cat.container);

    cat.layout->setContentsMargins(20,0,0,0);

    cat.container->setVisible(false);


    connect(cat.header, &QPushButton::clicked,
            this, &SideBar::toggleCategory);


    main_layout->addWidget(cat.header);
    main_layout->addWidget(cat.container);


    categories.append(cat);
}


SideBar::Category* SideBar::findCategory(const QString& name)
{
    for(auto& c : categories)
    {
        if(c.header->text() == name)
            return &c;
    }

    return nullptr;
}

void SideBar::addNode(std::string category_, std::string node_name)
{
    QString category = QString::fromStdString(category_);
    QString name = QString::fromStdString(node_name);

    Category* cat = findCategory(category);

    if (!cat)
    {
        addCategory(category);
        cat = findCategory(category);
    }

    DraggableLabel* node = new DraggableLabel(name);

    cat->layout->addWidget(node);
}

void SideBar::toggleCategory()
{
    QPushButton* button =
        qobject_cast<QPushButton*>(sender());


    Category* cat = findCategory(button->text());

    if(!cat)
        return;


    cat->expanded = !cat->expanded;
    cat->container->setVisible(cat->expanded);
}

void SideBar::buildCats(){
    addNode("util", "Input");
    addNode("util", "Output");
    addNode("simple", "Add");
}
