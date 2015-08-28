import QtQuick 2.4
import QtQuick.Controls 1.2
import QtGraphicalEffects 1.0



Rectangle{
    id: root
   //; color: "red"// "#343434"
    property int mIndex
    property int colIndex
    property int xChange: 0
    property bool bChangeSize: false
    property string title:  "value"
    property Repeater globalRep
    property string colorKey : "green"
    property ColorOverlay p_color_overlay
    property bool double_click : false


    RectangularGlow {
       id: shadow
       height: root.height
       width: height
      /* x:root.border.width*1.2
       y:x*/
       //anchors.right: root.right
       x:root.width -height -  4*root.border.width
        y: x
       visible: false
       color: "black"
       opacity: 0.5
      // scale: 0.8
       z: 1
       glowRadius: 5
       spread: 0.00002
       // cornerRadius: shadow.radius + glowRadius
    }
   /* RectangularGlow {
          id: effect
          anchors.fill: shadow
          glowRadius: 10
          spread: 0.00002
          color: "black"
         // opacity: 0.5
          cornerRadius: shadow.radius + glowRadius
      }*/

    Rectangle
    {
        id: background_rec
        z: 2
        property int  border_width : 5
    // border { color: "white" ; width: 2 }
     height: root.height// - 2*background_rec.border.width
     width: root.width
     color: "green"

    Image {
        id: background
//         x:root.border.width + 1
//         y:x
         //anchors.margins: background_rec.border.width

         height: root.height// - 2*background_rec.border.width
         width: height
       fillMode: Image.TileHorizontally
       source: "qrc:/iphone_toolbar_icons/black_tree.png"
       visible: true

       Image {
           id: icon
           source:  "image://imageProvider/" + root.colIndex + "+" + root.mIndex + "+ " + (Math.round(Math.random(9999999) * 10000));
           height: parent.height//root.height - root.border.width*2 - 1
           width: height
          // x:root.border.width + 1
           //y:root.border.width + 1
           visible:  true
          // anchors.fill: parent

           ColorOverlay {
               id: icon_coloroverlay
                  anchors.fill: icon
                  source: icon
                  color: "#00000000"
                  z: 1
                  enabled: false
              }
           BorderImage {
               width: root.height; height: width
               border { left: background_rec.border_width; top: background_rec.border_width;
                   right: background_rec.border_width; bottom: background_rec.border_width }
               horizontalTileMode: BorderImage.Stretch
               verticalTileMode: BorderImage.Stretch
               source: "qrc:/рамка.png"
               z: 2
           }
           Text {
               id: name
               anchors.margins: 3
               anchors.centerIn: parent
               text: root.title
               font.pixelSize: 1
               color: "white"
               style: Text.Outline;
               styleColor: "black"
               onTextChanged: {
                   name.font.pixelSize = (root.width*1.2)/text.length;
                   if(name.font.pixelSize > root.height*0.7)
                       name.font.pixelSize = root.height*0.7

               }
               z: 3
           }

           //z:1
       }

    }
    onScaleChanged: {
        shadow.height  = root.height*scale
    }
    NumberAnimation on scale {
        id:animation_scale_small;
         property int anim_time : 200
        running: false;
        to: 0.8;
        duration: anim_time
    }
    NumberAnimation on x {
        id: animation_scale_x;
        running: false;
        to: root.border.width  - width * 0.05;
        duration: animation_scale_small.duration
    }
    NumberAnimation on y {
        id: animation_scale_y;
        running: false;
        to: root.border.width  - height * 0.05;
        duration: animation_scale_x.duration
    }
    NumberAnimation on scale {
        id:animation_scale_normal;
        running: false;
        to: 1;
        duration: animation_scale_y.duration
    }
    }




    radius: 3
    clip: true
    color: "transparent"
    anchors.leftMargin: 15






    function updateTrackWhereIsBlock()
    {
        globalRep.updateModel()
    }



    function hideMenu()
    {
        context_menu.closeIt()
    }


    function repaint()
    {
        icon.source = "image://imageProvider/" + root.colIndex + "+" + root.mIndex + "+ " + Math.random(1000000);
    }


    // property Item  p_main222


 //   property int time_scale_valueRecX
   // property int time_scale_valueRecY
    /*radius: 0
    border.width: 0
    border.color: "green"
    anchors.topMargin: 0
    anchors.leftMargin: 0*/
z: 0
    onWidthChanged: {
        if (width < main222.minBlockWidth)
            width = main222.minBlockWidth

       /* if (width < height* timeControll.getScaleScrollChildren())
                icon.width = width;
        else*/
            icon.width = icon.height;

       /* var temp = width + timeControll.getBlockStartTime(colIndex,mIndex) > scroll.width
        if (temp)
           scroll.flickableItem.y = temp */

       // timeControll.setBlockTime(colIndex,mIndex,width)

        //// //console.log("333 timeControll.setBlockTime " + colIndex + " " + mIndex + " " + width)
         //timeControll.setBlockTime(mainwindow.columnIndex,mainwindow.blockIndex,block_width_value.value)
    }
    onXChanged: {
        timeControll.setBlockStartTime(root.colIndex,root.mIndex, x * main222.scaling)
    }





    Drag.active: mouseArea.drag.active
    Drag.hotSpot.x: 10
    Drag.hotSpot.y: 10
onYChanged: y=0;

    MouseArea {
        id: mouseArea
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        drag.target: root
        property int oldMouseX
        anchors.fill: parent
        enabled: !globalRep.isDrag
        hoverEnabled: true

        Timer {
            id: double_click_timer
               interval: 250; running: false; repeat: false
               onTriggered: root.double_click = false
           }

        onMouseXChanged: {
           // if (context_menu.visible === false) //123rr
            {
            if(globalRep.isDrag === false &&  mouseX > root.width * 0.9) //mouseX < root.width * 0.1 ||/
            {
                cursorShape = Qt.SizeHorCursor;
//            drop.visible = false;
//                drop.enabled = false;
            //    root.Drag.active =  false
              }
            else
            {
                cursorShape = Qt.ArrowCursor;

             //drop.enabled = true;
              //   drop.visible = true;
              //  root.Drag.active =  mouseArea.drag.active
            }

            xChange = oldMouseX - mouseX;

            if(bChangeSize)
            {
                root.width -= xChange;
                //timeControll.setTestWidth(bar_track.index,root.width, mIndex);

            }
            oldMouseX = mouseX;
            }
        }


       onPressed: {

           divider.y = (root.height + main222.p_columns.spacing) * root.colIndex
                   + time_scale.height - scroll.flickableItem.contentY
          divider.x =  root.x + root.width - divider.width/2 + tollbar.width

           divider.pos_to_append.x = root.colIndex
           divider.pos_to_append.y =  root.mIndex

           main222.dropEnteredBlockIndex = -1
           timeControll.setSelectedBlockPoint(root.colIndex,root.mIndex);
           main222.selectedBlockCol = root.colIndex

           main222.selectedBlockIndex = root.mIndex

          main222.selectedBlock = root;
           context_menu.globalRep = root.globalRep

           // console.log("AAAAAAAAAAAAAAAAAA "+ main222.selectedBlockCol +" " + main222.selectedBlockIndex)
            if(main222.selectedBlock !== null)
                main222.selectedBlock.hideMenu();
            timeControll.emitFocusFoundSignal();
            //console.log("AAAAAAAAAAAAAAA " + timeControll.getBlockStartTime(root.colIndex,root.mIndex))
             main222.p_scale_pointer.x = mouseX + root.x - scroll.flickableItem.contentX + main222.p_scale_pointer.width //1234

            main222.p_scale_pointer.x+=1


            main222.needToLightSelected = true
            for (var y=0; y< rep_columns.model; y++)
                 rep_columns.itemAt(y).abortColorize()

            icon_coloroverlay.color = "#8000FF00"


           // blocks.itemAt(i).icon_coloroverlay.color = "#00000000"
            //columns.childAt()
            // //console.log("onPressed: mIndex="+mIndex+" colIndex="+ colIndex + " time = " + timeControll.getBlockTime(colIndex,mIndex))
            if (mouse.button == Qt.RightButton)
            {
               context_menu.showIt(main222.p_scale_pointer.x, mouseY + root.colIndex * (root.height + 2)
                                 - scroll.flickableItem.contentY,root)
                drag.target = null
            }
        else
            {
                if (root.double_click )
                {
                    //context_menu.show(-1000, -1000,root.globalRep)
                   context_menu.showEditBlock()
                   root.double_click = false
                }
                else
                {
                     root.double_click = true
                    double_click_timer.running = true

                    context_menu.closeIt()
                drag.target = root
    // //console.log("onPressed");
            drop.visible = false;
             drop.enabled = false;
            if( mouseX > root.width * 0.9)
            {
                bChangeSize = true;
                mouseArea.drag.target = null
            }
            else
            {
            globalRep.isDrag = true;                
                cursorShape = Qt.OpenHandCursor
                animation_scale_small.running = true
                animation_scale_x.running = true
                animation_scale_y.running = true
                shadow.visible = true
                //root.border.color  = "transparent"





              /*   mouseY + root.colIndex * (root.height + 2)
                                                 - scroll.flickableItem.contentY  */

                 main222.maIsPressed = 1
                main222.clicked_blockId = root.mIndex
                main222.clicked_blockX = root.x
                main222.clicked_blockY = root.y
                root.z += 200

            }
            }
            // //console.log("   root.z= " +   root.z)
        }
        }
        onReleased: {
//root.border.color = "white"
            main222.p_scale_pointer.x = mouseX + root.x - scroll.flickableItem.contentX + main222.p_scale_pointer.width //1234
            animation_scale_normal.running = true


            if (globalRep.isDrag)
            {
                       root.z -= 200
              divider.visible = false
              //  if (main222.dropEnteredBlockIndex !== -1)
                {
                   if (main222.dropEntered)
                    {
                      // root.p_main222.maIsPressed = 0
                        //timeControll.reverseBlocks(root.colIndex,root.mIndex,main222.clicked_blockId)
        if (main222.selectedBlockIndex ===  divider.pos_to_append.y - 1 && !main222.left_rigth_entered)
          console.log("main222.selectedBlockIndex ===  divider.pos_to_append.y - 1 && main222.left_rigth_entered)")


    else
        {
            if (main222.selectedBlockIndex > divider.pos_to_append.y  && main222.left_rigth_entered)
                divider.pos_to_append.y += 1
            timeControll.moveBlockFromTo(main222.selectedBlockCol,
                                         main222.selectedBlockIndex,  divider.pos_to_append.y)
           console.log("moveBlock From " + main222.selectedBlockIndex + " To " +
                      divider.pos_to_append.y );
            main222.selectedBlockCol = root.colIndex
            main222.selectedBlockIndex =  divider.pos_to_append.y
            timeControll.setSelectedBlockPoint(main222.selectedBlockCol,main222.selectedBlockIndex)
        }
            }
                }
                globalRep.updateModel();
                 globalRep.isDrag = false
            }
            else
             if(bChangeSize)
            {
                mouseArea.drag.target = root;
                 timeControll.setBlockTime(colIndex, mIndex,root.width * main222.scaling);
                // item_col.width = timeControll.getMaxTrackTime()// item_col.childrenRect.width             
                 globalRep.updateModel();
                 bChangeSize = false;
    ///console.log("2222222222222");
            }

             drop.visible = true;
             drop.enabled = true;
            shadow.visible = false

             cursorShape = Qt.ArrowCursor;


        }
        onEntered: {
           // // //console.log(mouseX + " YY " + mouseY)
        }
    }






    DropArea {
        id: drop
        enabled : true
        visible: true
         anchors.fill: parent
    onEntered: {
         main222.dropEnteredBlockIndex = root.mIndex
        main222.left_rigth_entered = false
        main222.dropEntered = 1
        var temp = main222.selectedBlock.x - root.x - root.width/2
        if (temp > 0 )
            main222.block_zayshow_sprava = true
        else
            main222.block_zayshow_sprava = false
        divider.pos_to_append.x = root.colIndex



    }
    onExited: {
        if (root.mIndex === 1)
        main222.dropEnteredBlockIndex = main222.selectedBlockIndex
        icon_coloroverlay.color = "#00000000"
            }
    onPositionChanged: {
          {
            divider.visible = true
            var temp = main222.selectedBlock.x - root.x - root.width/2
            var move_y_pos = (root.height + main222.p_columns.spacing) * root.colIndex
                    + time_scale.height - scroll.flickableItem.contentY

              if (temp > 0  )
              {
                main222.block_zayshow_sprava = true
                    //divider.x = root.x + root.width - divider.width/2 + tollbar.width
                 /* divider.y = root.height + columns.spacing) * root.colIndex
                          + time_scale.height - scroll.flickableItem.contentY*/


                 divider.moveTo(root.x + root.width - divider.width/2 + tollbar.width,
                           move_y_pos )
                  console.log("OOOOOOOOOOOOOOOOOOOOOOOOO " + move_y_pos)

                  main222.left_rigth_entered = true
                  divider.pos_to_append.y = root.mIndex
                  console.log("onPositionChanged: right " + divider.pos_to_append.y)
              }
              else
              {
                      main222.block_zayshow_sprava = false
                  //divider.x = root.x - divider.width/2 + tollbar.width
                     divider.moveTo(root.x - divider.width/2 + tollbar.width,
                               move_y_pos )
                      console.log("OOOOOOOOOOOOOOOOOOOOOOOOO " + move_y_pos)

                main222.left_rigth_entered = false
                divider.pos_to_append.y = root.mIndex
                console.log("onPositionChanged: left " + divider.pos_to_append.y)

              }
          }
    }

    }
    Component.onCompleted: {
        root.p_color_overlay = icon_coloroverlay
    }
}


