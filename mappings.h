#ifndef MAPPINGS_H
#define MAPPINGS_H

#include <QSettings>
#include "mappingitem.h"
#include <QHash>
#include <QString>

class Mappings :public QObject
{


public:
    Mappings();
    ~Mappings();    
    QChar getCharForKeyIfActive(QChar);
    MappingItem *getMappingForKey(QChar);
    QMap<int,MappingItem> *getByNumberOrderedMapping();
    QMap<int,MappingItem> *getByNumberOrderedAndEnabledMapping();
    QHash<QChar,MappingItem> *getMapping();
    MappingItem emptyMappingItem;
    int count() const;
    int activeCount() const;
    int getNumberInEnabledMapping(int number);

public slots:
    void fetch();
    void clearUsage();
    QString stringifyMappings();

protected:
    QSettings settings;
    QMap<int,MappingItem> orderedMapping;
    QMap<int,MappingItem> orderedFilteredMapping;
    QHash<QChar,MappingItem> mapping;

private:
    void createOrderedMapping();
    void createOrderedFilteredMapping();
};

#endif // MAPPINGS_H
