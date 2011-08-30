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
    QChar getCharForKeyIfActive(QChar);
    MappingItem *getMappingForKey(QChar);
    QMap<int,MappingItem> *getByNumberOrderedMapping();
    QHash<QChar,MappingItem> *getMapping();
    MappingItem emptyMappingItem;
    int count;
public slots:
    void fetch();
    void clearUsage();

protected:
    QSettings settings;
    QMap<int,MappingItem> omappings;
    QHash<QChar,MappingItem> mappings;
};

#endif // MAPPINGS_H
