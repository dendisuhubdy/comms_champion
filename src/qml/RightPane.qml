import QtQuick 2.1
import QtQuick.Controls 1.1
import "."

SplitView {
    orientation: Qt.Vertical

    Rectangle {
        id: msgDetails
        color: GlobalConstants.background
        height: (parent.height * 2) / 3
        width: parent.width
        
        GroupBox {
            title: "Message Details"
            anchors.fill: parent
        
            Rectangle {
                anchors.fill: parent
            }
        }
    }

    Rectangle {
        id: protocolStackDetails
        color: GlobalConstants.background
        width: parent.width
        
        GroupBox {
            title: "Protocol Details"
            anchors.fill: parent
        
            Rectangle {}
        }
    }
}
