#ifndef MAPPINGS_H
#define MAPPINGS_H

#include <QSettings>
#include "mappingitem.h"
#include <QHash>

class Mappings
{
public:
    Mappings();
    ~Mappings();
    void fetch();
    QChar getCharForKeyIfActive(QChar);
    QHash<QChar,MappingItem> mappings;
protected:
    QSettings settings;
};

#endif // MAPPINGS_H
