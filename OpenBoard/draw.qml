import QtQuick 2.0
import "boarFunc.js" as JS
Item{
    Rectangle{
        color: "#000000"
        anchors.fill: parent
    }
    Canvas{
        id: mainDrawElm
        objectName: "mcanvas"
        anchors.fill: parent
        antialiasing: true
        //renderStrategy: Canvas.Threaded
        renderTarget: Canvas.Image
        property int index: 0
        property int animationIndex: 0
        property int scroll: -1

        property int row: 1
        property int rowMax: 1
        property var listOfObjects : []
        property var indexWords : 0;
        property var indexSTR : 0;
        property int lastHeight: mainDrawElm.height
        property int globalScroll: 0

        function clear(){
            JS.context.clearRect(0,0, mainDrawElm.width,mainDrawElm.height);
            JS.context.lineWidth = 3;
            JS.clear(lastHeight + 200);
            JS.context.translate(0, globalScroll);
            globalScroll = 0;
            // set line color
            JS.context.strokeStyle = '#ff0000';
        }
        function clearRectangle(x,y,width,height){
                    JS.context.clearRect(x,y,width,height);
                }

        function bdrawWrapText( a ){
            //console.debug("drawWrapText____OK" + a)
            JS.drawWrapText(a)
            mainDrawElm.markDirty();
        }
        function fillText( str, x, y )
        {
            //JS.init();
            //initFont()
            //if(JS.context != null)
                JS.context.fillText( str, x, y);
           // mainDrawElm.markDirty();
        }
        function init()
        {
            JS.init();
        }

        function setGradientStyle(x , y, width, height, colorMAS )
        {
            var grd = context.createLinearGradient(x, y, width, height);
            var mas = new Array(colorMAS);
            for( var i = 0; i < mas.length; i++)
                grd.addColorStop(parseFloat(colorMAS[i++]), colorMAS[i]);
            /*grd.addColorStop(0.85, "red");
            grd.addColorStop(1, "rgba(255, 255, 255, 0)");*/
            context.fillStyle = grd;
        }

        function isLastRow(scroll, LPI, y)
        {
            var img = JS.context.getImageData(0, 0, mainDrawElm.width, mainDrawElm.height);
            JS.clear(y);
            lastHeight = y;
            JS.context.translate(0, scroll);
            mainDrawElm.globalScroll += scroll*(-1);
            JS.context.drawImage(img, 0, LPI); // LPI - висота стрічки
        }

        function initFont(pt, font, style)
        {
           // console.debug(pt +  " " + font)
            JS.context.font = pt + "pt \"" + font + "\"";// "bold 10pt  \"font_name\" "
        }

        function setColor( col )
        {
            JS.context.fillStyle = col;
        }

        function drawFigure(x, y, w, h, type, fill)
        {
            if(type == 0) //line
            {
                JS.context.beginPath();
                JS.context.moveTo(x, y);
                JS.context.lineTo(w, h);
                JS.context.stroke();
            }
        }

        //function drawSymbol(st);
       // Text { text: cFontLoader.status == FontLoader.Ready ? 'Loaded' : 'Not loaded' }
        Timer{
            id: fps
            interval: 1000/120

            onTriggered: {
                mainDrawElm.requestPaint()
            }
            running: true
            repeat: true
            Component.onCompleted: fps.start()

        }

        Connections {
            target: forma
            onDrawTextChanged: console.log("The application data changed!")
        }

        onPaint: {
         //   JS.isLastRow();
/*            for( var i = 0; i < JS.animateList.length; i++)
                JS.fillAnimationText(JS.animateList[i]);
                */

        }
    }
    Component.onCompleted: {

    }
}
