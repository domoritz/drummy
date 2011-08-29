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
    QChar key;
    QChar character;
    bool active;
};

#endif // MAPPINGITEM_H
