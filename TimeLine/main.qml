import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.0


import "Block" as ContentBlock
import "ToolBar" as ContentToolBar


Rectangle {
    id: frama
    color: "#333333"
    //  width: 1000 ///main222.width + 20
    // height: 500 //main222.height + 20
    //anchors.margins : 20
    // onHeightChanged: // //console.log("HEIGHT = " + height)
    z: -150
    radius: 10
    MouseAreaForWindowDraging{
        id: framaMA
        anchors.fill:   frama
    }

    onActiveFocusChanged: {
        // context_menu.visible = false
        console.log("GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG")
    }
    /*
MouseAreaForWindowDraging{
anchors.left:   frama.left
width: 20
}
MouseAreaForWindowDraging{
 anchors.right:   frama.right
 width: 20
}
MouseAreaForWindowDraging{
 anchors.top:   frama.top
 height: 20
}
MouseAreaForWindowDraging{
 anchors.bottom:   frama.bottom
 height : 20
}*/
    property Rectangle p_main222

    Rectangle
    {
        Rectangle{
            id: cool_main
            y: -scroll.flickableItem.contentY
            z: 1000
            height: scroll.height + scroll.flickableItem.contentY + scroll.borderSize*2
            clip: true
            width: childrenRect.width
            color: "#222"
        }

        id:main222

        anchors.fill: parent
        property int margin_value : 20
        anchors.margins:  margin_value
        radius: 10
        color: "gray"

        Component.onCompleted: {
            frama.p_main222 = main222
        }
        width: childrenRect.width ///main222.width + 20
        //height: childrenRect.height //main222.height + 20
        property int max_time : 4000
        onMax_timeChanged: {
            if (max_time < 4000)
                max_time = 4000;
        }

        property int clicked_blockId : -1
        property int clicked_blockX : -1
        property int clicked_blockY : -1
        property int drop_blockX : -1
        property int drop_blockY : -1
        property int maIsPressed: 0
        // property int maxTrackTime: 0
        property int dropEntered: 0
        property int blocks_num : 0
        property int  prev_block_time : -1
        property bool left_rigth_entered : false
        property bool block_zayshow_sprava : false
        property Item p_toolbar
        property bool dovodka_block : false

        property bool force_resize_block : false
        onForce_resize_blockChanged:
        {
            timeControll.setForceResizeBlock(force_resize_block);

        }
        property bool able_create_empty_block : true
        onBlock_zayshow_spravaChanged: {
            // anim_
        }

        property int local_x : 0
        property Item selectedBlock: null
        property int selectedBlockCol : -1
        property int selectedBlockIndex : -1
        property int dropEnteredBlockIndex : -1
        property bool needUpdateModelWhereBlockDroped : false
        property int zdvigWhenNormalAnim : 0
        property bool exitedFromDropArea : false
        property bool drag_group : false
        property bool miss_drag : false
        property int press_mouseX : -1
        property int press_block_x : -1

        property variant blocks_of_groups : []

        onExitedFromDropAreaChanged: {
            //console.log("exitedFromDropArea = " + exitedFromDropArea)
        }

        property Item dropEnteredBlock
        property Item dropEnteredBlockItemGlobalRep
        property int prev_dropEnteredTrackIndex : -1
        property int dropEnteredTrackIndex : -1
        onDropEnteredTrackIndexChanged: {
            console.log("onDropEnteredTrackIndexChanged = " + dropEnteredTrackIndex)

            if (p_last_track >= 0)
                rep_columns.itemAt(p_last_track).setTrackBorder(0)
            if (dropEnteredTrackIndex >= 0)
                rep_columns.itemAt(dropEnteredTrackIndex).setTrackBorder(3)
            p_last_track = dropEnteredTrackIndex;
        }

        property real scaling :   timeControll.getScaleScrollChildren()
        onScalingChanged: {
            console.log("onScalingChanged = " + scaling);
        }

        property int minBlockWidth : 1000 / scaling
        property int isPlayPauseStop : 2
        // property int torepainting : 1
        property int saveScrollX : 0
        property int saveScalePointerX : 0


        property  bool needToLightSelected : false
        property Repeater p_rep_columns
        property Item p_item_col
        property Item p_timeControll
        property Item  p_trackbar
        property Item  p_scale_pointer: null
        property Item  p_context_menu
        property Item  p_divider
        property Item  p_columns
        property int  p_last_track : -1
        property Item  p_trackbar_which_block_dragged

        property int mX : 0

        property int prevPlayTime : 0
        property int play_time : 0



        focus: true


        property bool ctrl_pressed : false

        onCtrl_pressedChanged: {
            timeControll.setCtrlPressed(ctrl_pressed)
            console.log("AAAAAAAAAAAAAAAAAAAAA main222.ctrl_pressed = " + ctrl_pressed)
        }
        Keys.onPressed: {
            console.log("AAAAAAAAAAAAAAAAAAAAA " + event.key)
            if(event.modifiers & Qt.ControlModifier) {
                main222.ctrl_pressed = true
                ////console.log("AAAAAAAAAAAAAAAAAAAAA " + ctrl_pressed)
                if (event.key === Qt.Key_C || event.key === 67 )
                    timeControll.copyBlockToBuffer();
                else
                    if (event.key === Qt.Key_V || event.key === 86)
                        timeControll.pasteBlockFromBuffer();
                    else
                        if (event.key === Qt.Key_T || event.key === 84)
                        {
                            timeControll.setBlockTimeFromBuffer()
                            main222.selectedBlock. globalRep.updateModel()
                        }
                        else
                            if (event.key === Qt.Key_R|| event.key === 82)
                            {
                                timeControll.setBlockPositionSizeFromBuffer();
                            }
                            else
                                if (event.key === Qt.Key_E || event.key === 69)
                                {
                                    timeControll.setBlockEffectsFromBuffer();
                                }
                                else
                                    if ((event.key === Qt.Key_S || event.key === 1067 || event.key === 1030) && (event.modifiers & Qt.ShiftModifier))
                                    {
                                        timeControll.emitSaveProject();
                                        main222.ctrl_pressed = false;
                                    }
                                    else
                                        if ((event.key === Qt.Key_O  || event.key === 1065) && (event.modifiers & Qt.ShiftModifier)) //1065 - 'Щ'
                                        {
                                            timeControll.emitOpenProject();
                                         //     rep_columns.itemAt(main222.selectedBlockCol).setColorize(main222.selectedBlockIndex,"#8000FF00")
                                            rep_columns.itemAt(main222.selectedBlockCol).abortColorize()
                                            timeControll.setSelectedBlockPoint(-1,-1)
                                            main222.selectedBlock = null
                                            main222.selectedBlockCol = -1
                                            main222.selectedBlockIndex = -1
                                        }
                                        else
                                            if ((event.key === Qt.Key_N || event.key === 1058) && (event.modifiers & Qt.ShiftModifier))
                                                timeControll.emitNewProject();
                                            else
                                                if (event.key === 45)
                                                    timeControll.zoomPlus()
                                                else
                                                    if (event.key === 43)
                                                        timeControll.zoomMinus()
            }
            else
            {
                var step = 10;
                if (event.key === Qt.Key_Left)
                {
                    console.log("event.key === Qt.Key_Left")
                    if (timeControll.isBlockInGroup(main222.selectedBlockCol,main222.selectedBlockIndex))
                    {
                        timeControll.addBlockStartTimeGroup(main222.selectedBlockCol,main222.selectedBlockIndex, -step * main222.scaling)
                        // main222.updateTracksModel()
                    }
                    else
                    {
                        timeControll.addBlockStartTime(main222.selectedBlockCol,main222.selectedBlockIndex, -step * main222.scaling)
                        // rep_columns.itemAt(main222.selectedBlockCol).getBlock(main222.selectedBlockIndex).globalRep.updateModel()

                    }
                }
                else
                    if (event.key === Qt.Key_Right)
                    {
                        //console.log("event.key === Qt.Key_Right")
                        if (timeControll.isBlockInGroup(main222.selectedBlockCol,main222.selectedBlockIndex))
                        {
                            // console.log("event.key === Qt.Key_Right AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")
                            timeControll.addBlockStartTimeGroup(main222.selectedBlockCol,main222.selectedBlockIndex, step * main222.scaling)

                            //main222.updateTracksModel()
                        }
                        else
                        {
                            timeControll.addBlockStartTime(main222.selectedBlockCol,main222.selectedBlockIndex, step * main222.scaling)

                            ///main222.selectedBlock.globalRep.updateModel()
                            //rep_columns.itemAt(main222.selectedBlockCol).getBlock(main222.selectedBlockIndex).globalRep.updateModel()
                        }
                    }
            }

        }
        Keys.onReleased: {
            //ctrl release
            if( event.key === 16777249) {
                main222.ctrl_pressed = false
            }
        }


        function addTrack()     {
            timeControll.addNewTrack( )
            rep_columns.model +=1
        }

        function removeTrack()    {
            timeControll.removeLastTrack()
            rep_columns.model -=1
            // item_col.width = (timeControll.getMaxTrackTime() + 31) * main222.scaling

        }

        function play()    {
            if (isPlayPauseStop === 1)
            {
                //scroll.flickableItem.contentX =  main222.saveScrollX
                //scale_pointer.x = main222.saveScalePointerX
            }
            else
            {
                //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NEED RESTORE IN FUTURE CONVERT THIS TIME TO FPS


                var temp_time = (scale_pointer.x   +
                                 scroll.flickableItem.contentX - 20 )* main222.scaling // -20 is left border
                //  console.log("qqqqqqqqqqqqqqqqqqqqqqq    " + temp_time)
                timeControll.setPlayTime(temp_time);
                main222.prevPlayTime = temp_time
                main222.play_time =  temp_time
                main222.saveScrollX = scroll.flickableItem.contentX
                main222.saveScalePointerX = scale_pointer.x
            }

            isPlayPauseStop = 0


            scroll.enabled = false
            scale_pointer.enabled = false
            tollbar.p_button_RemoveTrack.enabled = false
            tollbar.p_button_AddTrack.enabled = false
            /*tollbar.p_button_play.enabled = false
            tollbar.p_button_stop.enabled = true
            tollbar.p_button_pause.enabled = true*/

        }
        function pause()
        {
            main222.saveScrollX = scroll.flickableItem.contentX
            main222.saveScalePointerX = scale_pointer.x
            isPlayPauseStop = 1

            scroll.enabled = true
            scale_pointer.enabled = true
            //  tollbar.p_button_RemoveTrack.enabled = true
            //  tollbar.p_button_AddTrack.enabled = true
            //  tollbar.p_button_play.enabled = true

            //tollbar.p_button_pause.enabled = false
        }

        function stop()
        {
            isPlayPauseStop = 2
            scroll.enabled = true
            scale_pointer.enabled = true
            tollbar.p_button_RemoveTrack.enabled = true
            tollbar.p_button_AddTrack.enabled = true
            //tollbar.p_button_play.enabled = true
            //tollbar.p_button_stop.enabled = false
            //tollbar.p_button_pause.enabled = false

            scroll.flickableItem.contentX = 0
            scale_pointer.x = 0// timeControll.getMaxTrackTime() + scale_pointer.width/2 - scroll.flickableItem.contentX;
            gc();
        }

        function setScalePointerPos(xx)    {
            scale_pointer.x = xx

        }

        function updateTracksModel()
        {
            console.log("func updateTracksModel()")
            scaling = timeControll.getScaleScrollChildren()
            rep_columns.model = 0
            rep_columns.model =  timeControll.getTracksNumber()
            // main222.p_trackbar.globalRep.updateModel()
            //  item_col.width = timeControll.getMaxTrackTime()
            item_col.width = (timeControll.getMaxTrackTime()) / main222.scaling + 31
            if (main222.needToLightSelected  && main222.selectedBlockIndex !== -1)
            {
                rep_columns.itemAt(main222.selectedBlockCol).setColorize(main222.selectedBlockIndex,"#8000FF00")
            }

        }

        function drawYellowRectangle (xx,yy,widthh,heightt) {
            yellow_rec.visible= true
            yellow_rec.x = 30 + xx/main222.scaling//@BAG@
            yellow_rec.y = yy
            yellow_rec.width = widthh/main222.scaling
            yellow_rec.height = heightt

            console.log("function drawYellowRectangle " + "( " + xx + "   ,  " + yy + " " + yellow_rec.width + "  " + heightt)

            //p_icon_coloroverlay
        }

        Connections {
            target: timeControll
            onMaxTrackTimeChanged: {

                var max_time = value/ main222.scaling
                main222.max_time = max_time

                item_col.width = (value) / main222.scaling +31

                /* if (main222.p_scale_pointer != null)
                    main222.p_scale_pointer.x += 1;*/

                var tt_pos = max_time - scroll.flickableItem.contentX + main222.p_scale_pointer.width
                if (main222.p_scale_pointer.x > tt_pos ) //741
                    main222.p_scale_pointer.x = tt_pos;
                console.log("function 1212`1212 tt_pos = " + tt_pos + " main222.p_scale_pointer.x = " + main222.p_scale_pointer.x + " value = " + max_time)

            }

            onSetBlockPlayTimeUntilFreezeSignal:
            {
                /* console.log("onSetBlockPlayTimeUntilFreezeSignal value = " + value/main222.scaling
                            + "  col = " + col + " ind = " + ind);*/
                rep_columns.itemAt(col).getBlock(ind).p_anim_pointer.x = value/main222.scaling
            }

            onPlaySignal: {
                main222.play()


            }
            onPauseSignal: {
                main222.pause()
            }
            onStopSignal: {
                main222.stop()


            }
            onMaxTrackSizeChange: {
                console.log( "onMaxTrackSizeChange")
                var max_time = timeControll.getMaxTrackTime();
                main222.p_columns.width =  max_time;
                item_col.width = (main222.p_columns.width) / main222.scaling + 31


            }

            onBorderColorChangedSignal :
            {
                console.log(col + "  " + ind + " " + color);
                rep_columns.itemAt(col).getBlock(ind).p_border_color = color; //!!!!!!!!!!!!!!!!!!!

            }



            onDrawRectangleSignal: {
                var rec = timeControll.getYellowRect()
                main222.drawYellowRectangle(rec.x,rec.y,rec.width,rec.height)
            }

            onRemoveRectangleSignal:{
                yellow_rec.visible = false
            }
            onUpdateSelectedBlock: {
                /* if (main222.ctrl_pressed)
           {
               var pp = point
               console.log("AAAAAA  "  +   rep_columns.itemAt(pp.x).getBlock(pp.y).p_border_color)

.rep_columns.itemAt(pp.x).getBlock(pp.y).p_border_color
                          = timeControll.getBlockBorderColor(pp.x,pp.y);
           }*/
            }



            onUpdateSignal:  {
                main222.play_time = timeControll.getPlayTime()
                //console.log("AAAAAAAAAAAAAA play_time = "+  main222.play_time)
                if (main222.isPlayPauseStop === 0 )
                {
                    var play_time = timeControll.getPlayTime();
                    scale_pointer.x += (play_time - main222.prevPlayTime)/main222.scaling;
                    // scale_pointer.x = timeControll.getPlayTime()/main222.scaling;
                    main222.prevPlayTime = play_time;
                    //  (timeControll.getPlayTime() - scroll.flickableItem.contentX)/main222.scaling + 15 ;
                    timeControll.calcPointedBlocksAtTime()
                    // //console.log("timer value: " +timeControll.getPlayTime())
                }
            }
            onUpdateTrackAt: {
                main222.p_trackbar.globalRep.updateModel()
            }

            onUpdateModel: {
                console.log("onUpdateModel !!!!!!!!!!!!!!")
                for(var i = 0; i < cool_main.children.length; i++) {
                    // console.log("destroying: " + i)
                    cool_main.children[i].destroy();
                }
                main222.updateTracksModel();

                // rep_columns +=1
                //rep_columns -= 1
                // rep_columns = timeControll.getTracksNumber()
                //console.log("AAAAAAAAAAAAAAAAAA " +  main222.scaling)
            }


            onResetProjectSignel: {
                timeControll.setScaleScrollChildren(0) //it have protection from small values, will set minimum scale
                main222.selectedBlockCol = -1
                main222.selectedBlockIndex = -1
                // rep_columns.model = timeControll.getTracksNumber()
                //console.log("1111111111111111111")
                // main222.p_trackbar.globalRep.updateModel()
                main222.p_scale_pointer.x = 26
                timeControll.setIsProjectChanged(false)

                main222.updateTracksModel()
            }

            onFocusLostSignal: {
                context_menu.closeIt()
                if (main222.ctrl_pressed)
                    main222.ctrl_pressed = false;
            }

            /*function updateBlockTime(col, index, time)
            {
                console.log("updateBlockTime  " + col + " " + index + "  rep_columns.count = "  + rep_columns.count);
                /*if (col >=0 && col < rep_columns.count)
                {
                     console.log("rep_columns.itemAt(col).count = " + rep_columns.itemAt(col).getCount()); //12345
                    if (index >= 0 && index < rep_columns.itemAt(col).getCount() )
                    {
                        console.log("succesfull 3");
                        rep_columns.itemAt(col).getBlock(index).width = time/main222.scaling;
                    }
                }

                rep_columns.itemAt(col).setBlockWidth(index,time/main222.scaling );
            }*/

            onBlockTimeSignel: {
                // console.log("onBlockTimeSignel");
                // updateBlockTime(col, index, time);
                // if (main222.maIsPressed === 1)
                {
                    //console.log("onBlockTimeSignel  " + col + " " + index + "  rep_columns.count = "  + rep_columns.count);
                    rep_columns.itemAt(col).setBlockWidth(index,time/main222.scaling );
                }
            }

        }





        Rectangle {
            id: time_scale
            property int division : 50
            width: 2000//item_col.width
            height: 20// item_col.height/10
            color: "gray"
            property int tX: 0
            x: scroll.flickableItem.contentX  //trackbar.x   //  item_col.x
            y: 0// main222.height -height - 20
            z: 2

            /* Component.onCompleted: {
            var rec = mapFromItem(time_scale,10,10)
            // //console.log(rec.x + " mapFromItem "  + rec.y)

        }*/
            Row {
                Repeater {
                    id: time_scale_rep
                    model:   time_scale.width/ time_scale.division
                    delegate:
                        Rectangle{
                        width: time_scale.division
                        height: time_scale.height
                        color: "black"
                        // x: time_scale.tX
                        Rectangle {
                            id: hor_line
                            width: time_scale.division
                            height: time_scale.height/20
                            color: "white"
                            y: time_scale.height/2
                            x:0
                        }
                        Rectangle {
                            id: ver_line
                            height: time_scale.height
                            width: hor_line.height
                            x: time_scale.division - width
                            color: "white"
                        }
                        Text {

                            text: {
                                if (main222.scaling >= 10)
                                    return Math.round(time_scale.division * index * main222.scaling)/1000 + " sec"
                                else
                                    return Math.round(time_scale.division * index * main222.scaling) + " ms"

                            }
                            y:   0
                            width:  time_scale.division
                            height: time_scale.height/2
                            color: "white"
                            anchors.right: ver_line.left


                            //font { pixelSize: 10 }

                        }

                        Component.onCompleted: {
                            /*  time_scale.tX += time_scale.division
                    // //console.log(" new division added " +  time_scale.tX)*/
                        }
                    }
                }
            }
        }

        Rectangle {
            id : divider
            color: "transparent"
            property point pos_to_append
            onPos_to_appendChanged: {
                //console.log("onPos_to_appendChanged " + pos_to_append.y)
                timeControll.setPosToAppend(pos_to_append)
            }

            property int anim_x_value : 0
            property int anim_y_value : 0
            property string d_color: "red"
            border { width: 3; color: d_color }
            Component.onCompleted: {
                main222.p_divider = divider
            }
            ParallelAnimation  {
                id: animation_move
                running: false
                NumberAnimation  {
                    target: divider
                    property: "x";
                    id: animation_move_x;
                    running: false;
                    to: divider.anim_x_value
                    duration: 100
                }
                NumberAnimation {
                    target: divider
                    property: "y";
                    id: animation_move_y;
                    running: false;
                    to: divider.anim_y_value
                    duration: 100
                }
            }
            function moveTo( xx,  yy)
            {
                divider.anim_x_value = xx
                divider.anim_y_value = yy
                animation_move.restart()
            }

            visible : false
            height : 70
            width : 30
            z: 600
            x: 100
            y: 20
            /* Image {
        source: "qrc:/iphone_toolbar_icons/arrowup.png"
        width: parent.width
        height: width
        }
        Rectangle {
            color: divider.d_color
            height: parent.height - 2*parent.width
            y: parent.width
            width: parent.width*0.5
            x: width/2
        }
        Image {
        source: "qrc:/iphone_toolbar_icons/arrowdown.png"
        width: parent.width
        height: width
        anchors.bottom: parent.bottom
        }*/
        }

        Image{
            id: scale_pointer
            source: "qrc:/iphone_toolbar_icons/arrow.png"
            height: time_scale.height
            width: height
            scale: 1.5
            x: 20 //absolute zdvig 20
            z: time_scale.z +1
            //property int trackbar_right
            Component.onCompleted: {
                main222.p_scale_pointer = scale_pointer
                //  x = 40
            }

            onYChanged: y = 0
            onXChanged: {
                if (x < 20)
                {
                    var new_val =scroll.flickableItem.contentX - (20 - x)
                    if (new_val < 0)
                        new_val = 0
                    scroll.flickableItem.contentX = new_val
                    x = 20
                }
                // else
                {
                    var zdvig = 20 - scroll.flickableItem.contentX
                    if (zdvig < 0)
                    {
                        zdvig = -width/2
                    }
                    if (x<zdvig)
                    {
                        scroll.flickableItem.contentX -= zdvig -scale_pointer.x
                        if (scroll.flickableItem.contentX < 0)
                        {
                            scroll.flickableItem.contentX = 0;
                        }
                        scale_pointer.x = zdvig
                    }
                    else
                    {
                        var atempa = 1
                        var contentWidth =   scroll.flickableItem.contentWidth - width/2 - atempa
                        if (x > contentWidth)
                            x = contentWidth
                        else
                        {


                            var real_time = timeControll.getMaxTrackTime() / main222.scaling - atempa //1234
                            var temp = scroll.width-30    // real_time - scroll.flickableItem.contentX + width
                            if (x  > temp )
                            {
                                //   //console.log("HAAAAAAAAAAA")

                                scroll.flickableItem.contentX += (x - temp)
                                var sad = real_time - scroll.width +50 // scroll.flickableItem.contentWidth - scroll.width + 10
                                if (scroll.flickableItem.contentX  >  sad)
                                    scroll.flickableItem.contentX = sad;
                                x = temp
                            }
                        }


                    }
                }
                /* var max_val = scroll.width - width
               if (x > max_val)
                   x = max_val;*/

                timeControll.setScalePointerPos((x  -20 + scroll.flickableItem.contentX)* main222.scaling);

            }
            MouseArea {
                id: spMA
                anchors.fill: parent
                drag.target : scale_pointer
                hoverEnabled: true

                onPressed: {
                    context_menu.closeIt();
                   // scale_pointer_info.visible = true
                }
                onMouseXChanged: {
                   /* if (mouseX < 0 || mouseX > width)
                        scale_pointer_info.visible = false*/
                }
                onMouseYChanged: {
                   /* if ( mouseY < 0 || mouseY > height )
                        scale_pointer_info.visible = false*/
                }

                onEntered: {
                    tooltip_timer.running = true
                }

                onExited:  {
                    scale_pointer_info.visible = false
                    tooltip_timer.running = false
                }

            }
            Timer {
                id: tooltip_timer
                interval: 50; running: false; repeat: false
                onTriggered: scale_pointer_info.visible = true
            }

        }
        Rectangle {
            id: scale_pointer_info
            width: 150
            height: 30
            color: "white"
            z: 201
            visible : false
            property int pos : scale_pointer.x - scale_pointer.width
            x: pos + 15
            y: scale_pointer.y + 10
            Label {
                anchors.fill: parent
                font.family: "Tahoma"
                font.pixelSize: 12
                color: "black"
                text: "global time: "+ scale_pointer_info.pos*main222.scaling + " ms\nlocal time: " + (scale_pointer_info.pos - main222.local_x)*main222.scaling+" ms" //1545
            }
        }

        Rectangle{
            id:scale_pointer_verline
            height: tollbar.height  //  scroll.height - main222.margin_value
            width: 2
            color: "#6E0000"
            // y: scale_pointer.height
            x: scale_pointer.width/2 + scale_pointer.x
            z: 200
            // height:400
            //  onHeightChanged: // //console.log("HEIGHT  pointer line = " + height)

        }


        Row { //scroll + toolbar
            id: toolbar_scroll
            width: parent.width
            height: parent.height - time_scale.height
            y : time_scale.height


            ScrollView  {
                id: scroll
                width: parent.width - tollbar.width
                height: parent.height + 1

                property int horizontalX: flickableItem.contentX
                property int verticalY: flickableItem.contentY

                property int borderSize: 10
                //property int baba: scroll.flickableItem.contentWidth
                onHorizontalXChanged:  {
                    time_scale.x = -flickableItem.contentX + 30
                    //context_menu.visible = false //123rr
                }
                onVerticalYChanged:  {
                    //context_menu.visible = false //123rr

                }
                style: ScrollViewStyle{
                    scrollBarBackground : Rectangle{
                        implicitHeight: scroll.borderSize
                        implicitWidth: scroll.borderSize
                        x:1
                        color: "#222"
                    }
                    decrementControl :
                        Rectangle{
                        height: 0
                        width: 20
                        color: "transparent"
                    }
                    incrementControl  :
                        Rectangle{
                        implicitHeight: 0
                        implicitWidth: 0
                        color: "transparent"
                    }
                    corner  :
                        Rectangle{
                        implicitHeight: scroll.borderSize
                        implicitWidth: scroll.borderSize
                        x:1
                        color: "#222"
                    }
                    handle:
                        Rectangle{
                        color: "#555"
                        y: scroll.borderSize/2
                        x: scroll.borderSize/2
                        implicitHeight: scroll.borderSize/2
                        radius: 10
                        implicitWidth: implicitHeight
                    }
                }

                Rectangle {
                    id: draw_wnd
                    visible: false
                    width: 250
                    x: 200
                    z: 1200
                    height: 500
                    color: "red"

                    GroupBox {
                        //title: "Tab Position"
                        ColumnLayout {
                            id: rb_col_layout
                            TextField   {
                                id: draw_x
                                width: 200
                                font { pixelSize: 14 }
                                y: 0
                                x:  5
                                text: ""
                            }
                            TextField   {
                                id: draw_y
                                width: 200
                                font { pixelSize: 14 }
                                // y: draw_x.height
                                x:  5
                                text: ""
                            }
                            TextField   {
                                id: draw_width
                                width: 200
                                font { pixelSize: 14 }
                                // y: 2*draw_y.height
                                x:  5
                                text: ""
                            }
                            TextField   {
                                id: draw_height
                                width: 200
                                font { pixelSize: 14 }
                                y: 3*draw_width.height
                                x:  5
                                text: ""
                            }
                            Button {
                                width:  draw_height.width
                                // y: 4*draw_height.height
                                text: " OK draw it"
                                onClicked: {
                                    /* timeControll.drawRectangle(draw_x.text,draw_y.text,
                                                     draw_width.text,draw_height.text)*/
                                    timeControll.drawRectangle(200,200,200,200)
                                    draw_wnd.visible= false
                                }
                            }
                        }
                    }
                }



                horizontalScrollBarPolicy :Qt.ScrollBarAlwaysOn
                verticalScrollBarPolicy  :Qt.ScrollBarAlwaysOn
                clip: true
                __wheelAreaScrollSpeed: 50

                Rectangle {
                    id: item_col
                    property Item p_columns
                    property Item p_trackbar
                    // width: childrenRect.width
                    height: childrenRect.height
                    color: "gray"
                    Rectangle {
                        id: yellow_rec
                        z:100
                        visible: false
                        border { width: 3; color: "yellow" }
                        color : "transparent"
                        Drag.active: yellow_rec_mouse.drag.active
                        property int oldX: x
                        property int oldY: y

                        onXChanged: {
                            if(Drag.active)
                            {
                                for( var i = 0; i < timeControll.getCurent_groupMembersSize(); i++)
                                {
                                    var it = rep_columns.itemAt(timeControll.getCurent_groupMembers(i).x).getBlock(timeControll.getCurent_groupMembers(i).y);
                                    it.x -= (oldX - x)/1.0;
                                }

                            }
                            oldX = x;
                        }
                        onYChanged: {
                            if(Drag.active)
                            {
                                for( var i = 0; i < timeControll.getCurent_groupMembersSize(); i++)
                                {
                                    var it = rep_columns.itemAt(timeControll.getCurent_groupMembers(i).x).getBlock(timeControll.getCurent_groupMembers(i).y);
                                    it.y -= (oldY - y)/1.0;
                                }

                            }
                            oldY = y;
                        }
                        MouseArea{
                            id: yellow_rec_mouse
                            anchors.right: yellow_rec.right
                            width: 30
                            height: parent.height
                            property int old: mouseX
                            property bool press: false
                            hoverEnabled: true

                            z: 1000
                            onEntered: {
                                // console.log("qweqweqwewqeqe")
                                if(!drag.active)
                                    cursorShape = Qt.SizeHorCursor;
                            }
                            onExited: {
                                if(!drag.active)
                                    cursorShape = Qt.ArrowCursor;
                            }

                            onPressed: {
                                press = true;

                            }
                            onPressAndHold: {
                                /*press = false;
                                drag.target = parent;
                                cursorShape = Qt.ClosedHandCursor;
                                console.debug(timeControll.getCurent_groupMembersSize());
                                for( var i = 0; i < timeControll.getCurent_groupMembersSize(); i++)
                                {
                                    var it = rep_columns.itemAt(timeControll.getCurent_groupMembers(i).x).getBlock(timeControll.getCurent_groupMembers(i).y);
                                    it.z = 2000;
                                }*/

                            }

                            onReleased: {
                                press = false;
                                main222.updateTracksModel();
                                drag.target = null;
                                for( var i = 0; i < timeControll.getCurent_groupMembersSize(); i++)
                                {
                                    var it = rep_columns.itemAt(timeControll.getCurent_groupMembers(i).x).getBlock(timeControll.getCurent_groupMembers(i).y);
                                    it.z = 0;
                                }
                            }

                            onMouseXChanged: {
                                console.log(mouseX);
                                if(press)
                                {
                                    yellow_rec.width = ((timeControll.tryResizeCurentGroup((mouseX - old)*main222.scaling))/main222.scaling);
                                    //if(timeControll.updateYellowRectangle(yellow_rec.x, yellow_rec.y, yellow_rec.width - (old - mouseX), yellow_rec.height))
                                }
                                old = mouseX
                            }
                        }
                    }
                    onWidthChanged: {
                        if (time_scale.width < width  ) //1234
                        {
                            time_scale.width = width  ;
                            time_scale_rep.model = time_scale.width/ time_scale.division + 2
                        }
                        ////console.log("122  item_col.width=" + item_col.width)
                    }

                    Column {
                        id: columns
                        width:  timeControll.getMaxTrackTime()
                        property Repeater globalRep
                        // spacing: 2
                        /*onSpacingChanged: {
                            timeControll.setSpacingBtwBlocks(columns.spacing)
                        }*/

                        Component.onCompleted: {
                            main222.p_columns = columns
                            //timeControll.setSpacingBtwBlocks(columns.spacing)
                        }
                        Repeater {
                            id: rep_columns
                            model: timeControll.getTracksNumber()
                            onModelChanged: {
                                if ( model < 0)
                                    model = 0;
                            }


                            Component.onCompleted: {
                                main222.p_rep_columns = rep_columns
                            }

                            delegate:
                                Row {
                                id: bar_track
                                property int cIndex : index
                                function abortColorize()
                                {
                                    for (var i=0; i< repka.model; i++)
                                        repka.itemAt(i).p_color_overlay.color = "#00000000"
                                    // blocks.itemAt(i).icon_coloroverlay.color = "#00000000"
                                }
                                function setColorize(indexa, color)
                                {
                                    if (indexa >=0 && indexa< repka.count)
                                        repka.itemAt(indexa).p_color_overlay.color = color
                                    //  // //console.log("GGGGGGGGGG " + repka.itemAt(indexa).mX)
                                    // // //console.log("GGGGGGGGGG " + repka.itemAt(indexa).x)
                                }
                                function getBlockX (indexa)
                                {
                                    // //console.log("repka.itemAt(indexa).x="+repka.itemAt(indexa).x)

                                    return repka.itemAt(indexa).x
                                }

                                function setBlockWidth (indexa, value)
                                {
                                    /*console.log(" function setBlockWidth (indexa = " + indexa +
                                                " value = " + value)*/

                                    repka.itemAt(indexa).width = value
                                }

                                function getCount()
                                {
                                    // //console.log("repka.itemAt(indexa).x="+repka.itemAt(indexa).x)

                                    return repka.count
                                }

                                function getBlock (indexa)
                                {
                                    // //console.log("repka.itemAt(indexa).x="+repka.itemAt(indexa).x)

                                    return repka.itemAt(indexa)
                                }

                                function getTrackBar()
                                {
                                    // //console.log("repka.itemAt(indexa).x="+repka.itemAt(indexa).x)

                                    return trackbar;
                                }

                                function enableTrackbarsButtons(value)
                                {
                                    trackbar.enableButtonsClick = value;
                                }

                                function setTrackBorder(value)
                                {
                                    // main222.p_last_main_root.border.width = 0; //-=-=-=
                                    //main222.p_last_main_root = p_main_root;
                                    p_main_root.border.width = value
                                }


                                property Item p_main_root: value


                                property int mIndex: index
                                ContentToolBar.TrackToolBar {
                                    id: trackbar
                                    width: 30
                                    property int mIndex: index
                                    height: 70// main_root.height
                                    z:12
                                    x: 0
                                    // y: parent.height/4
                                    //y:  mapFromItem(item_col, 0, 0).y
                                    colIndex : index
                                    Rectangle {
                                        id:main_root
                                        //x: 30
                                        property int col_ind : index
                                        width: main222.max_time
                                        height: 70
                                        color: "transparent"
                                        border { color: "lawngreen"; width: 0}
                                        z: blocks.z + 1
                                        Component.onCompleted: {
                                            bar_track.p_main_root = main_root;
                                        }
                                    }

                                    Row {
                                        id: blocks
                                        //  height: 220
                                        x: 30
                                        Repeater {
                                            id: repka
                                            Component.onCompleted: {
                                                trackbar.globalRep = repka
                                                trackbar.p_main222 = main222
                                            }
                                            property bool isDrag : false
                                            model:  timeControll.getTrackSize(trackbar.mIndex)//     bar_track.mIndex)
                                            function updateModel()      {
                                                // console.log("AAAAAAAAAAAAAAAAAA updateModel")
                                                model = 0
                                                model =  timeControll.getTrackSize(bar_track.mIndex)
                                                item_col.width = (timeControll.getMaxTrackTime()) / main222.scaling + 31
                                                if (main222.needToLightSelected && main222.selectedBlockIndex !== -1 )
                                                {
                                                    rep_columns.itemAt(main222.selectedBlockCol).setColorize(main222.selectedBlockIndex,"#8000FF00")

                                                }
                                                /*  for (var i=0; i < model; i++ )
                                                    repka.itemAt(i).p_anim_pointer.x = timeControll.getBlockPlayTimeUntilFreeze(main222.selectedBlockCol,main222.selectedBlockIndex)/main222.scaling*/

                                                timeControll.calcPointedBlocks();
                                                timeControll.setIsProjectChanged(true)
                                                gc();

                                            }
                                            function moveBlockForAnim(ind,   value) //left_right -1 or 1
                                            {
                                                repka.itemAt(ind).animRunX( value)
                                            }

                                            function moveBlocksForAnim( from, to,   value) //left_right -1 or 1
                                            {
                                                console.log("  function moveBlocksForAnim( from, to,   value)")
                                                for (var i = from; i <= to; i++)
                                                {
                                                    repka.itemAt(i).animRunX(repka.itemAt(i).x + value)
                                                }
                                            }
                                            delegate:
                                                ContentBlock.Block{
                                                id: cool
                                                globalRep : repka
                                                columnRep: rep_columns
                                                p_trackbar : trackbar
                                                p_bar_track : bar_track
                                                p_border_color: timeControll.getBlockBorderColor(colIndex, mIndex)
                                                height:  70
                                                mIndex: index
                                                p_divider: divider
                                                p_anim_pointer_x: timeControll.getBlockPlayTimeUntilFreeze(trackbar.mIndex,index)/main222.scaling
                                                colIndex:  bar_track.mIndex
                                                width:  timeControll.getBlockTime(colIndex, mIndex) / main222.scaling
                                                p_main222: main222

                                                title: timeControll.getBlockKey(colIndex,mIndex)

                                                /* time_scale_valueRecX : time_scale_value.mX
                                               time_scale_valueRecY : time_scale_value.mY*/

                                                /* p_mouse_area.onPositionChanged: {
                                                  time_scale_value.mX = scrollMA.mouseX
                                                    time_scale_value.mY = scrollMA.mouseY
                                               }*/
                                            }
                                            onModelChanged: {
                                                columns.width =  timeControll.getMaxTrackTime() //* main222.scaling


                                            }

                                        }
                                        Component.onCompleted: {

                                            /* var pnt2 = new Point(-1,-1)
                                           timeControll.setSelectedBlockPoint(pnt2)*/
                                            // main222.p_trackbar.globalRep.updateModel()
                                            /*   // //console.log("item_col.p_columns.globalRep " + item_col.p_columns.globalRep)
                                           // //console.log("cool.repka " + cool.repka)
                                           item_col.p_columns.globalRep = cool.repka
                                           // //console.log("REPKA " + item_col.p_columns.globalRep)*/
                                            //item_col.main_root = main_root
                                        }
                                    }

                                    //}
                                    Component.onCompleted: {
                                        trackbar.globalRep = repka
                                        main222.p_trackbar = trackbar
                                    }
                                }
                            }    //
                        }
                        onChildrenRectChanged:  {
                            width =  timeControll.getMaxTrackTime() / main222.scaling
                            // // //console.log(" timeControll.getMaxTestWidth() = " +  width)
                        }
                    } /* rep_columns end */
                }
            }
            ContentToolBar.ToolBar{
                id: tollbar
                z: 30
                x: scroll.width
                width: 30
                height: parent.height
                Component.onCompleted: {
                    main222.p_toolbar = tollbar
                }
            }

            ////////////////////////tuta
        }



    }

    ContentBlock.ContextMenu {
        id: context_menu
        x: 20
        y: 0
        // z: 500
        // visible: false
        Component.onCompleted: {
            //main222.p_context_menu = context_menu //123rr
            // Component.onCompleted:
            time_scale.x = -scroll.flickableItem.contentX + 30//trackbar.width + trackbar.x //12345

        }

    }

    Rectangle {
        anchors.fill: parent
        border.color: "#333333";
        color: "transparent"
        radius: 10
        border.width: main222.margin_value
    }

}

/*

    Image {
        x: 30
        source: "qrc:/iphone_toolbar_icons/arrowdown.png"
        height: 100
        width: 1000
    }*/


