
import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick 2.9
import QtQuick.Window 2.9
import QtLocation 5.11
import QtPositioning 5.11
import QtQuick.Controls 1.4
import com.crossdump.navigationsegment 1.0
import com.crossdump.zone 1.0
import com.crossdump.route 1.0


Rectangle {
    id: boxInstance
    signal leftClicked
    signal rightClicked
    property string headerText: ""
    property string headerIconSource: ""

    property bool footer: false

    property bool leftButtonVisible: false
    property alias leftButtonText: textLeftButton.text

    property bool rightButtonVisible: false
    property alias rightButtonText: textRightButton.text

    height: parent.height
    width: 450

    border.color: "#C4C4C4"

    // seperator between header and main thing
    Rectangle {
        anchors.top: parent.top
        anchors.topMargin: 40
        anchors.left: parent.left
        height: 1
        width: parent.width
        color: "#C4C4C4"
    }

    // Header
    Image {
        source: headerIconSource
        width: 32
        height: 32
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 8
    }

    Text {
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 45
        font.pixelSize: 20
        text: headerText
        color: "#707070"
    }
    // Header end

    // Footer start

    Rectangle {
        visible: footer
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 80
        anchors.left: parent.left
        height: 1
        width: parent.width
        color: "#C4C4C4"
    }

    /* Left button */
    Button{
        id:leftButton
        visible: leftButtonVisible
        width: 100
        height: 50
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.bottomMargin: 20

        Text {
            id: textLeftButton
            anchors.centerIn: parent
            font.pixelSize: 14
            font.bold: true
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
              leftClicked();
            }
        }
    }

    Button{
        id:rightButton
        visible: rightButtonVisible
        width: 100
        height: 50
        anchors.bottom: parent.bottom
        anchors.right:parent.right
        anchors.rightMargin: 30
        anchors.bottomMargin: 20

        Text {
            id: textRightButton
            anchors.centerIn: parent
            font.pixelSize: 14
            font.bold: true
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
              rightClicked();
            }
        }
    }

    // Main thing end
}
