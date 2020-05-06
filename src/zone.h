#ifndef ZONE_H
#define ZONE_H

#include <QObject>
#include <QtLocation>
#include <QList>

class Zone : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList coordinates READ coordinatesVariant CONSTANT)
    Q_PROPERTY(QVariantList bounds READ boundsVariant CONSTANT)
    Q_PROPERTY(QGeoCoordinate averagePoint READ averagePoint CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)

public:
    explicit Zone(QObject *parent = nullptr) : QObject(parent) {}
    explicit Zone(const Zone &other);
    explicit Zone(QList<QGeoCoordinate> coordinates, QList<QGeoCoordinate> bounds, QString name, QObject *parent = nullptr);
    ~Zone();
    QList<QGeoCoordinate> coordinates() {return coordinates_;}
    QVariantList coordinatesVariant();
    QList<QGeoCoordinate> bounds() {return bounds_;}
    QVariantList boundsVariant();
    QGeoCoordinate averagePoint() {return averagePoint_;}
    QString name() {return name_;}
signals:

public slots:

private:
    QList<QGeoCoordinate> coordinates_;
    QList<QGeoCoordinate> bounds_;
    QString name_;
    QGeoCoordinate averagePoint_;
};

#endif // ZONE_H
