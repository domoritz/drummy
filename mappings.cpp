#include "mappings.h"
#include <QKeySequence>
#include <QString>
#include <QDebug>

// class which holds mappings for keys

Mappings::Mappings()
{
}

Mappings::~Mappings()
{

}

// fetches keymappings from settings
void Mappings::fetch() {
    int count = settings.beginReadArray("mappings");

    qDebug() << "Fetching" << count << "mappings";

    for (int i = 0; i < count; i++) {
        settings.setArrayIndex(i);

        MappingItem item;

        QChar key = settings.value("key","").toString()[0];
        item.name = settings.value("name","").toString();
        item.key = key;
        item.character = settings.value("char","").toString()[0];
        item.active = settings.value("active",true).toBool();

        mappings.insert(key,item);

        qDebug() << "fetched" << key;
    }
    settings.endArray();
}

QChar Mappings::getCharForKeyIfActive(QChar key) {
    if (mappings[key].active) {
        return mappings[key].character;
        qDebug() << "Fetched"<<key<<", return"<<mappings[key].character;
    } else {
        return QChar::Null;
    }
};
