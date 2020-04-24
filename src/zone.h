#ifndef ZONE_H
#define ZONE_H

#include <QObject>
#include <QtLocation>
#include <QList>

class Zone : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList coordinates READ coordinates CONSTANT)
    Q_PROPERTY(QGeoCoordinate averagePoint READ averagePoint CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)

public:
    explicit Zone(QObject *parent = nullptr) : QObject(parent) {}
    explicit Zone(const Zone &other) : QObject() {}
    explicit Zone(QList<QGeoCoordinate> coordinates, QString name, QObject *parent = nullptr);
    ~Zone();
    QVariantList coordinates();
    QGeoCoordinate averagePoint() {return averagePoint_;}
    QString name() {return name_;}
signals:

public slots:

private:
    QList<QGeoCoordinate> coordinates_;
    QString name_;
    QGeoCoordinate averagePoint_;
};

#endif // ZONE_H
