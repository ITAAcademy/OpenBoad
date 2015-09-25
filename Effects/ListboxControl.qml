import QtQuick 2.4

Rectangle {
    width: 100
    height: 32
    color: "#333"
    id: listboxControl
    property Listbox target: null

    Item
    {
        anchors.fill: parent

        Rectangle{
            height:32
            width:32
            anchors.left: parent.left
            border.width: 0
            color: "transparent"
            id:imgRect1
            Image{
                anchors.fill: parent
                id: imgAddEffect
                source: "qrc:/Content/plus_button_32.png"
                MouseArea
                {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked:
                    {
                        effectsControll.addEffect("newEffect ",1,1);
                        //brushControll.setCurentBrush(brush.mIndex);
                        root.update();
                    }
                    onEntered: {
                        imgRect1.border.color="green";
                    }
                    onExited: {
                        imgRect1.border.color="black";
                    }
                }
            }

        }
        Rectangle{
            color: "transparent"
            anchors.right: parent.right
            height:32
            width:32
            id:imgRect2
            border.width:0
            Image{
                id: imgRemoveEffect
                source: "qrc:/Content/cancel_32.png"
                MouseArea
                {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked:
                    {
                        //brushControll.setCurentBrush(brush.mIndex);
                        effectsControll.removeEffect(target.getSelectedIndex())
                        target.decSelectedIndex();
                        root.update();
                    }
                    onEntered: {
                        imgRect2.border.color="red";
                    }
                    onExited: {
                        imgRect2.border.color="black";
                    }
                }
            }
        }
    }



}

