import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3
import QtGraphicalEffects 1.0

Item{
    id: root
    property string name: "value"
    property bool checkable: false
    property bool checked: false
    property color mColor: "#aaaaaa"
    property bool small: true

    property real value1: 0
    property real value2: 0
    property real maximum: blockTime
    property real minimum: 0
    property real maximum2: blockTime
    property real minimum2: 0
    property int xMax: size.width - handle1.width - 4
    property int xMax2: size.width - handle2.width - 4
    property int  glowRadius: 8
    function setValue1(n){
        value1=n;

    }
    function setValue2(n){
        value2=n;
    }


    onValue1Changed: {

        handle1.x = 2 + (root.value1 - root.minimum) * root.xMax / (root.maximum - root.minimum);
        // if (handle1.x>handle2.x)handle1.x=handle2.x;

    }
    onValue2Changed: {

        handle2.x = 2 + (root.value2 - root.minimum2) * root.xMax2 / (root.maximum2 - root.minimum2);
        //if (handle1.x>handle2.x)handle2.x=handle1.x;

    }
    signal release;
    Row{
        id:mainRow
        width: parent.width
        height: parent.height
        spacing: 10
        Text{
            id: name
            text: root.name
            y: parent.height/4
            width: contentWidth
            color: "white"
            font.pixelSize: 14
            style: Text.Outline;
            styleColor: "black"
        }

        Item {
            id: size; width: root.width - name.width*2 - out.width; height: root.height
            NumberAnimation { id: to; target: root; property: "glowRadius"; to: 4; duration: 200 }
            NumberAnimation { id: from; target: root; property: "glowRadius"; to: 2; duration: 200 }

            Rectangle {
                id: rect
                anchors.centerIn: size
                anchors.top: size.Top
                anchors.bottom: size.Bottom
                width: parent.width
                height: 3
                color: "gray"
                radius: 8

                RectangularGlow {
                    id: effect
                    x:handle1.x
                    width: handle2.x - handle1.x + handle2.width
                    height: parent.height
                    glowRadius: root.glowRadius
                    spread: 0.2
                    color: mColor
                    cornerRadius: rect.radius + glowRadius
                }
            }

            Rectangle {
                id: handle1; smooth: true
                z: 1
                y:mainRow.y+10;
                x: 2 + (root.value1 - root.minimum) * root.xMax / (root.maximum - root.minimum); width: size.height; height: size.height*1.5; radius: 100
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "white" }
                    GradientStop { position: .5; color: "red" }
                    GradientStop { position: 1.0; color: "red" }
                }

                /*RectangularGlow {
                    id: effect2
                    anchors.fill: handle1
                    glowRadius: root.glowRadius
                    spread: 0.2
                    color: if (!checkable) "white"; else "#CC0000"
                    cornerRadius: handle1.radius + root.glowRadius
                }*/
                MouseArea {
                    id: mouse_drag
                    anchors.fill: parent; drag.target: parent
                    hoverEnabled: true
                    drag.axis: Drag.XAxis; drag.minimumX: 2; drag.maximumX: root.xMax+2
                    onPositionChanged: {

                        root.value1 = (root.maximum - root.minimum) * (handle1.x-2) / root.xMax + root.minimum;
                        if (value1>value2)value1=value2;//BUG HERE. RESET VALUE IN 0 WHEN LOADING
                        effectsControll.setCurrentEffectProperty("alpha_start_time",value1);

                    }
                    property bool enter: false
                    onExited: {
                        if(pressed === false)
                            from.start();
                    }
                    onEntered:{
                        to.start();
                    }
                    onReleased: {
                        from.start();
                        root.release();
                        //console.log("LOGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG");
                    }
                    onDoubleClicked: {
                        if(checkable)
                        {
                            root.checked = !root.checked
                            if(root.checked)
                                effect2.color = "green";
                            else
                                effect2.color = "#CC0000";
                        }
                    }
                }
            }

            Rectangle {
                id: handle2; smooth: false
                z: 1
                y:mainRow.y-10;
                x: 20 + (root.value2 - root.minimum2) * root.xMax2 / (root.maximum2 - root.minimum2); width: size.height; height: size.height*1.5; radius: 100
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "green" }
                    GradientStop { position: .5; color: "green" }
                    GradientStop { position: 1.0; color: "white" }
                }
                /* RectangularGlow {
                    id: effect3
                    anchors.fill: handle2
                    glowRadius: root.glowRadius

                    spread: 0.2
                    color: if (!checkable) "green"; else "#CC0000"
                    cornerRadius: handle2.radius + root.glowRadius
                }*/
                MouseArea {
                    id: mouse_drag2
                    anchors.fill: parent; drag.target: parent
                    hoverEnabled: true
                    drag.axis: Drag.XAxis; drag.minimumX: 2; drag.maximumX: root.xMax2+2
                    onPositionChanged: {
                        root.value2 = (root.maximum2 - root.minimum2) * (handle2.x-2) / root.xMax2 + root.minimum2;
                        if (value1>value2)value2=value1;
                        effectsControll.setCurrentEffectProperty("alpha_end_time",value2);
                    }
                    property bool enter2: false
                    onExited: {
                        if(pressed == false)
                            from.start();
                    }
                    onEntered:{
                        to.start();
                    }
                    onReleased: {
                        from.start();
                        root.release();
                        //console.log("LOGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG");
                    }
                    onDoubleClicked: {
                        if(checkable)
                        {
                            root.checked = !root.checked
                            if(root.checked)
                                effect3.color = "green";
                            else
                                effect3.color = "#CC0000";
                        }
                    }
                }
            }
        }
        Rectangle{
            id:out
            width: 30
            x: size.x + size.width + 10
            Text {
                id: size_value2
                text: if(small) (root.value2/1000).toFixed(2);else Math.round(root.value2)
                width: 30
                color: "white"
                font.pixelSize: 14
                style: Text.Outline;
                styleColor: "black"
                anchors.bottom: parent.top

            }
            Text {
                id: size_value
                text: if(small) (root.value1/1000).toFixed(2);else Math.round(root.value1)
                width: 30
                color: "white"
                font.pixelSize: 14
                style: Text.Outline;
                styleColor: "black"
                anchors.top: parent.bottom
            }
        }
    }
    Component.onCompleted: {

        root.xMax = size.width - handle1.width - 4;
        root.xMax2 = size.width - handle2.width - 4;
        handle1.x = 2 + (root.value1 - root.minimum) * root.xMax / (root.maximum - root.minimum);
        handle2.x = 2 + (root.value2 - root.minimum2) * root.xMax2 / (root.maximum2 - root.minimum2);

    }
}

