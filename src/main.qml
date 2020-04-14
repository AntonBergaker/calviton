import QtQuick 2.9

import com.calviton.navigator 1.0
import com.calviton.navigationtask 1.0
import com.calviton.navigationsegment 1.0
import QtQuick.Window 2.9
import QtLocation 5.11
import QtPositioning 5.11
import QtQuick.Controls 1.4

Item {
    id: base
    width: displayWidth
    height: displayHeight
    visible: true

    // this will provide automatic adaption to screen size and orientation
    property int orientationOverride: 0  // -90 , 0 , 90, 180
    readonly property bool orientationPortrait: Math.abs(orientationOverride % 180) == 90

    NavigationTask {
        id: task
    }

    Navigator {
        id: navigator;
    }

    Plugin {
        id: osmPlugin
        name: "osm"
    }

    RouteQuery {
        id: routeQuery
    }

    Rectangle {
        anchors.centerIn: parent
        width: (targetARM && orientationPortrait) ? base.height : base.width;
        height: (targetARM && orientationPortrait) ? base.width : base.height;
        rotation: parent.orientationOverride

        Rectangle {
            id: menu
            color: "#666"
            anchors.top: parent.top
            anchors.right: parent.right
            height: parent.height
            width: 60

            Column {
                width: parent.width
                height: parent.height
                spacing: 5
                padding: 5

                Button {
                    width: parent.width
                    height: 50
                    text: "Route"
                    onClicked: view.view = routeView
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 5
                }
            }
        }

        Rectangle {
            id: view
            property Item view: mapView
            width: parent.width - menu.width
            height: parent.height
            anchors.left: parent.left
            anchors.top: parent.top
            MapView {
                id: mapView
                visible: view.view === this
                anchors.fill: parent
            }
        }
    }
}
