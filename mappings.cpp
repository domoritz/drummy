#include "mappings.h"
#include <QKeySequence>
#include <QString>
#include <QDebug>
#include <QMap>

// class which holds mappings for keys

Mappings::Mappings()
{
    //create empty mapping item
    emptyMappingItem.active=false;
    emptyMappingItem.enabled=false;
    emptyMappingItem.character=QChar::Null;
    emptyMappingItem.key=QChar::Null;
    emptyMappingItem.name="";
    emptyMappingItem.shortName="";
    emptyMappingItem.id=0;

    fetch();
}

Mappings::~Mappings()
{
}

// fetches keymappings from settings
void Mappings::fetch() {

    // clear old mappings
    orderedMapping.clear();
    orderedFilteredMapping.clear();
    mapping.clear();

    int count = settings.beginReadArray("mappings");

    for (int i = 0; i < count; i++) {
        settings.setArrayIndex(i);

        MappingItem item;

        QChar key = settings.value("key","").toString()[0];
        item.name = settings.value("name","").toString();
        item.shortName = settings.value("shortname").toString();
        item.key = key;
        item.character = settings.value("char","").toString()[0];
        item.active = settings.value("active",true).toBool();
        item.enabled = settings.value("enabled",true).toBool();
        item.id = i;

        mapping.insert(key,item);
    }
    settings.endArray();

    this->createOrderedMapping();
    this->createOrderedFilteredMapping();
}

void Mappings::createOrderedMapping()
{
    foreach (MappingItem item, mapping) {
        orderedMapping[item.id] = item;
    }
}

// depends on execution of createOrderedMapping
void Mappings::createOrderedFilteredMapping()
{
    orderedFilteredMapping = QMap<int,MappingItem>(orderedMapping);
    foreach (MappingItem item, mapping) {
        if (!item.enabled) {
            orderedFilteredMapping.remove(item.id);
            item.enabled = false;
        }
    }
}

int Mappings::activeCount() const
{
    return this->orderedFilteredMapping.count();
}

int Mappings::count() const
{
    return this->mapping.count();
}

int Mappings::getNumberInEnabledMapping(int id) {
    int number = 0;
    foreach (MappingItem item, orderedFilteredMapping) {
        if (item.id == id){
            return number;
        }
        number++;
    }
}

QChar Mappings::getCharForKeyIfActive(QChar key) {
    if (mapping[key].active) {
        return mapping[key].character;
        qDebug() << "Fetched"<<key<<", return"<<mapping[key].character;
    } else {
        return QChar::Null;
    }
};

// return empty mapping if no mapping was found
MappingItem *Mappings::getMappingForKey(QChar key) {
    if (mapping.contains(key)) {
        return &mapping[key];
        qDebug() << "Fetched"<<key;
    } else {
        return &emptyMappingItem;
    }
};

QHash<QChar,MappingItem>* Mappings::getMapping() {
    return &mapping;
}

QMap<int,MappingItem>* Mappings::getByNumberOrderedMapping() {
    return &orderedMapping;
}

QMap<int,MappingItem>* Mappings::getByNumberOrderedAndEnabledMapping() {
    return &orderedFilteredMapping;
}

QString Mappings::stringifyMappings()
{
    QString out;
    int size = -15;

    foreach(MappingItem mapping, orderedMapping) {
        out.append(QString("%1- %2\n").arg(mapping.shortName,size).arg(mapping.name));
    }

    return out;
}
