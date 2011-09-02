#ifndef MAPPINGITEM_H
#define MAPPINGITEM_H

#include <QString>
#include <QKeySequence>
#include <QChar>

class MappingItem
{
public:
    MappingItem();
    ~MappingItem();
    QString name;
    QString shortName;
    QChar key;
    QChar character;
    int number;
    bool active;
};

#endif // MAPPINGITEM_H
