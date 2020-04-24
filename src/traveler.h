#ifndef TRAVELER_H
#define TRAVELER_H

#include <QObject>
#include "navigation.h"

// Travels through a navigation
class Traveler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Navigation* navigation READ navigation WRITE setNavigation NOTIFY navigationChanged)
    Q_PROPERTY(QGeoCoordinate position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(int navigationCoordinateIndex READ navigationCoordinateIndex NOTIFY navigationCoordinateIndexChanged)
    Q_PROPERTY(int navigationSegmentIndex READ navigationSegmentIndex NOTIFY navigationSegmentIndexChanged)

    Q_PROPERTY(QString nextRoadName READ nextRoadName NOTIFY nextRoadNameChanged)
    Q_PROPERTY(QString nextTurnText READ nextTurnText NOTIFY nextTurnTextChanged)
public:
    explicit Traveler(QObject *parent = nullptr);
    Navigation* navigation() {return navigation_;}
    void setNavigation(Navigation* navigation) {navigation_ = navigation; emit navigationChanged(navigation);}

    QGeoCoordinate position() {return position_;}
    void setPosition(QGeoCoordinate position);

    int navigationCoordinateIndex() { return currentCoordinateTarget_;}
    int navigationSegmentIndex() { return currentSegment_;}
    QString nextRoadName() {return nextRoadName_;}
    QString nextTurnText() {return nextTurnText_;}
signals:
    void navigationChanged(Navigation*);
    void positionChanged(QGeoCoordinate);
    void navigationCoordinateIndexChanged(int);
    void navigationSegmentIndexChanged(int);
    void nextRoadNameChanged(QString);
    void nextTurnTextChanged(QString);
public slots:

private:
    Navigation* navigation_;
    QGeoCoordinate position_;

    int currentSegment_;
    int currentCoordinateTarget_;
    QString nextRoadName_;
    QString nextTurnText_;

    void UpdateProgress();
    void UpdateSegmentDetails();
};

#endif // TRAVELER_H
