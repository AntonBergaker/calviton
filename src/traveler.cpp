#include <algorithm>
#include <QtPositioning/QGeoCoordinate>
#include "traveler.h"
#include "collisionhelper.h"
#include <vector>


Traveler::Traveler(QObject *parent) : QObject(parent)
{
    navigation_ = nullptr;
    targetZone_ = nullptr;

    ResetProgress(false);
}

void Traveler::ResetProgress(bool emitChanges) {
    nextRoadName_ = QStringLiteral("");
    nextTurnText_ = QStringLiteral("");
    nextTurnDistance_ = QStringLiteral("");
    nextTurnIcon_ = QStringLiteral("");
    currentSegmentCoordinateIndex_ = 0;
    currentCoordinateTarget_ = 0;
    currentSegment_ = -1;
    insideZone_ = false;

    if (emitChanges) {
        emit nextRoadNameChanged(nextRoadName_);
        emit nextTurnTextChanged(nextTurnText_);
        emit nextTurnDistanceChanged(nextTurnDistance_);
        emit nextTurnIconChanged(nextTurnIcon_);
        emit navigationCoordinateIndexChanged(currentCoordinateTarget_);
        emit navigationSegmentIndexChanged(currentSegment_);
    }
}
void Traveler::setNavigation(Navigation* navigation)
{
    ResetProgress(true);
    navigation_ = navigation;
    emit navigationChanged(navigation);
}

void Traveler::setPosition(QGeoCoordinate position)
{
    position_ = position;

    if (navigation_ != nullptr) {
        UpdateProgress();
    }

    if (targetZone_ != nullptr) {
        UpdateInsideZone();
    }

    emit positionChanged(position);
}

void Traveler::setTargetZone(Zone* zone) {
    targetZone_ = zone;
    insideZone_ = false;
    emit insideZoneChanged(insideZone_);
    emit targetZoneChanged(targetZone_);
}

void Traveler::UpdateProgress()
{
    int previousTarget = currentCoordinateTarget_;
    // Project the coordinates into a 2d space where each unit is one meter

    // TODO: make sure this works closer to the poles
    QGeoCoordinate oneUnitHorizontal = QGeoCoordinate(position_.latitude()+1, position_.longitude());
    QGeoCoordinate oneUnitVertical   = QGeoCoordinate(position_.latitude()  , position_.longitude()+1);

    // Get how much one unit is worth in meters, which we use as modifiers going forward.
    double /*metersHorizontal*/ mH = position_.distanceTo(oneUnitHorizontal);
    double /*metersVertical  */ mV = position_.distanceTo(oneUnitVertical);

    CollisionHelper::Point pointPosition = CollisionHelper::Point(position_, position_, mH, mV);

    QList<QGeoCoordinate> geoCoordinates = navigation_->coordinates();

    //Check for collisions with points
    for (int i=currentCoordinateTarget_;i < geoCoordinates.length();i++) {
        // convert into a x/y point
        CollisionHelper::Point targetPoint = CollisionHelper::Point(geoCoordinates[i], position_, mH, mV);

        // if we're without a 20m radius of the point it counts as going in

        if (CollisionHelper::CirclePointCollision(targetPoint, 20, pointPosition)) {
            currentCoordinateTarget_ = std::max(currentCoordinateTarget_, i);
        }
    }

    // Check for collisions with lines
    for (int i=currentCoordinateTarget_; i < geoCoordinates.length()-1;i++) {

        CollisionHelper::Point startPoint = CollisionHelper::Point(geoCoordinates[i], position_, mH, mV);
        CollisionHelper::Point endPoint = CollisionHelper::Point(geoCoordinates[i+1], position_, mH, mV);

        if (CollisionHelper::CircleLineCollision(pointPosition, 20, startPoint, endPoint)) {
            currentCoordinateTarget_ = std::max(currentCoordinateTarget_, i);
        }
    }

    if (previousTarget != currentCoordinateTarget_) {
        emit navigationCoordinateIndexChanged(currentCoordinateTarget_);

        int oldSegment = currentSegment_;
        // find the current segment we're at
        int leftToRemove = currentCoordinateTarget_;
        for (int i=0;i<navigation_->segments().count();i++) {
            int toRemove = navigation_->segments().at(i)->coordinateCount();
            if (leftToRemove - toRemove <= 0) {
                currentSegmentCoordinateIndex_ = leftToRemove;
                currentSegment_ = i;
                break;
            }
            leftToRemove -= toRemove;
        }
        if (oldSegment != currentSegment_) {
            UpdateSegmentDetails();
            emit navigationSegmentIndexChanged(currentSegment_);
        }

    }

    UpdateNextTurnDistance();
}

void Traveler::UpdateInsideZone()
{
    if (targetZone_ == nullptr) {
        return;
    }

    std::vector<CollisionHelper::Point> points = std::vector<CollisionHelper::Point>();
    for (QGeoCoordinate coord : targetZone_->bounds()) {
        points.push_back(CollisionHelper::Point(coord.latitude(), coord.longitude()));
    }

    bool preValue = insideZone_;

    CollisionHelper::Point point = CollisionHelper::Point(position_.latitude(), position_.longitude());
    insideZone_ = CollisionHelper::PointInPolygon(point, points);

    if (preValue != insideZone_) {
        emit insideZoneChanged(insideZone_);
    }
}


QString GetManueverText(QGeoManeuver::InstructionDirection direction)
{
    switch (direction) {
        case QGeoManeuver::DirectionHardRight:
        case QGeoManeuver::DirectionBearRight:
        case QGeoManeuver::DirectionRight:
            return QStringLiteral("Turn right");

        case QGeoManeuver::DirectionLightRight:
            return QStringLiteral("Turn slightly right");

        case QGeoManeuver::DirectionHardLeft:
        case QGeoManeuver::DirectionBearLeft:
        case QGeoManeuver::DirectionLeft:
            return QStringLiteral("Turn left");

        case QGeoManeuver::DirectionLightLeft:
            return QStringLiteral("Turn slightly left");

        case QGeoManeuver::DirectionForward:
            return QStringLiteral("Go forward");

        case QGeoManeuver::DirectionUTurnLeft:
        case QGeoManeuver::DirectionUTurnRight:
            return QStringLiteral("Make a u-turn");

        case QGeoManeuver::NoDirection:
            return QStringLiteral("");
    }
    return QStringLiteral("");
}

QString GetManueverIconSource(QGeoManeuver::InstructionDirection direction)
{
    switch (direction) {
        case QGeoManeuver::DirectionHardRight:
        case QGeoManeuver::DirectionBearRight:
        case QGeoManeuver::DirectionRight:
        case QGeoManeuver::DirectionLightRight:
            return QStringLiteral("navigate-turn-right.png");

        case QGeoManeuver::DirectionHardLeft:
        case QGeoManeuver::DirectionBearLeft:
        case QGeoManeuver::DirectionLeft:
        case QGeoManeuver::DirectionLightLeft:
            return QStringLiteral("navigate-turn-left.png");

        case QGeoManeuver::DirectionForward:
            return QStringLiteral("navigate-go-forward.png");

        default:
            return QStringLiteral("navigate-go-forward.png");
    }

    return QStringLiteral("navigate-go-forward.png");
}

void Traveler::UpdateSegmentDetails()
{
    if (currentSegment_ == -1) {
        return;
    }
    if (navigation()->segments().count() <= currentSegment_+1) {
        nextRoadName_ = QStringLiteral("Destination");
        nextTurnText_ = QStringLiteral("");
    } else {

        NavigationSegment* next = navigation()->segments().at(currentSegment_+1);

        nextRoadName_ = next->maneuverRoadName();

        if (nextRoadName_ == QStringLiteral("") && currentSegment_ == navigation()->segments().count()-2) {
            nextRoadName_ = "your Destination";
        }

        nextTurnText_ = GetManueverText(next->maneuverTurnDirection());
        nextTurnIcon_ = GetManueverIconSource(next->maneuverTurnDirection());
    }

    emit nextRoadNameChanged(nextRoadName_);
    emit nextTurnTextChanged(nextTurnText_);
    emit nextTurnIconChanged(nextTurnIcon_);
}

void Traveler::UpdateNextTurnDistance()
{
    if (currentSegment_ == -1) {
        return;
    }
    int totalMeters = 0;
    NavigationSegment* segment = navigation()->segments().at(currentSegment_);
    for (int i=currentSegmentCoordinateIndex_ + 1;i < segment->coordinateCount(); i++)
    {
        QGeoCoordinate start = segment->coordinates().at(i-1);
        QGeoCoordinate end = segment->coordinates().at(i);
        totalMeters += start.distanceTo(end);
    }

    // Round to closest 50m

    totalMeters = (totalMeters/50) * 50;

    nextTurnDistance_ = QString::number(totalMeters) + QStringLiteral(" m");
    emit nextTurnDistanceChanged(nextTurnDistance_);
}
