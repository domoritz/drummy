#ifndef MAPPINGS_H
#define MAPPINGS_H

#include <QSettings>
#include "mappingitem.h"
#include <QHash>

class Mappings :public QObject
{

public:
    Mappings();
    ~Mappings();
    void fetch();
    QChar getCharForKeyIfActive(QChar);
    MappingItem *getMappingForKeyIfActive(QChar);
    QMap<int,MappingItem> getByNumberOrderedMapping();
    QHash<QChar,MappingItem> getMapping();
    MappingItem emptyMappingItem;
public slots:

protected:
    QSettings settings;
    QMap<int,MappingItem> omappings;
    QHash<QChar,MappingItem> mappings;
};

#endif // MAPPINGS_H
