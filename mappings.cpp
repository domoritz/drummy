#include "mappings.h"
#include <QKeySequence>
#include <QString>
#include <QDebug>
#include <QMap>

// class which holds mappings for keys

Mappings::Mappings()
{
    //create empty mapping
    emptyMappingItem.active=false;
    emptyMappingItem.character=QChar::Null;
    emptyMappingItem.key=QChar::Null;
    emptyMappingItem.name="";
    emptyMappingItem.shortName="";
    emptyMappingItem.number=0;

    fetch();
}

Mappings::~Mappings()
{
}

// fetches keymappings from settings
void Mappings::fetch() {

    omappings.clear();
    mappings.clear();

    int count = settings.beginReadArray("mappings");

    qDebug() << "Fetching" << count << "mappings";

    for (int i = 0; i < count; i++) {
        settings.setArrayIndex(i);

        MappingItem item;

        QChar key = settings.value("key","").toString()[0];
        item.name = settings.value("name","").toString();
        item.shortName = settings.value("shortname").toString();
        item.key = key;
        item.character = settings.value("char","").toString()[0];
        item.active = settings.value("active",true).toBool();
        item.number = i;

        mappings.insert(key,item);

        qDebug() << "fetched" << key;
    }
    settings.endArray();

    // create ordered mapping
    foreach (MappingItem item, mappings) {
        omappings[item.number] = item;
    }

    // set count
    this->count = count;
}

QChar Mappings::getCharForKeyIfActive(QChar key) {
    if (mappings[key].active) {
        return mappings[key].character;
        qDebug() << "Fetched"<<key<<", return"<<mappings[key].character;
    } else {
        return QChar::Null;
    }
};

// return nullpointer if no mapping was found
MappingItem *Mappings::getMappingForKey(QChar key) {
    if (mappings.contains(key)) {
        return &mappings[key];
        qDebug() << "Fetched"<<key;
    } else {
        return &emptyMappingItem;
    }
};

QHash<QChar,MappingItem>* Mappings::getMapping() {
    return &mappings;
}

QMap<int,MappingItem>* Mappings::getByNumberOrderedMapping() {
    return &omappings;
}

QString Mappings::stringifyMappings()
{
    QString out;
    int size = -15;

    foreach(MappingItem mapping, omappings) {
        out.append(QString("%1- %2\n").arg(mapping.shortName,size).arg(mapping.name));
    }

    return out;
}
