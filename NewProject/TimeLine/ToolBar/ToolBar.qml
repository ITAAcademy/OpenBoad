import QtQuick 2.0

Rectangle {
    id: root
    color: "gray"
   // onHeightChanged: // console.log("HEIGHT  toolbar = " + height)
    property Repeater globalRep
    Column{
        width: parent.width
        height: parent.height
     /*   Buutton{
            id: butAdd
            title_text: ""
            height: parent.width
            width: parent.width
          icon_source : "qrc:/iphone_toolbar_icons/plus.png"

            onClicked: {
               timeControll.addNewTest("vasia");
                 repka.updateModel() ;
                // console.log("CLICK2")
            }
        }*/
        Buutton{
            id: butRemoveTrack
            title_text: ""
            height: parent.width
            width: parent.width
          icon_source : "qrc:/iphone_toolbar_icons/delete.png"
            onClicked: {
                main222.needToLightSelected = false
                main222.removeTrack();
            }
        }

        Buutton{
            id: butAddTrack
            title_text: ""
            height: parent.width
            width: parent.width
          icon_source : "qrc:/iphone_toolbar_icons/eject.png"
          rotation: 180
            onClicked: {

                //for (var i = 0; i< 15; i++)
                {
                    main222.needToLightSelected = false;
                    main222.addTrack() ;
                 //  console.log("AAAAAAAAAAAAding track : "  + timeControll.getTracksNumber())
                }
            }
        }
        Buutton{
            id: butPlay
            title_text: ""
            height: parent.width
            width: parent.width
          icon_source : "qrc:/iphone_toolbar_icons/playG.png"
            onClicked: {
                    timeControll.play()
            }
        }
        Buutton{
            id: butPause
            title_text: ""
            height: parent.width
            width: parent.width
          icon_source : "qrc:/iphone_toolbar_icons/pauseG.png"
            onClicked: {
                    timeControll.pause()
            }
        }
        Buutton{
            id: butStop
            title_text: ""
            height: parent.width
            width: parent.width
          icon_source : "qrc:/iphone_toolbar_icons/stopG.png"
            onClicked: {
                    timeControll.stop()
            }
        }

    }
}

