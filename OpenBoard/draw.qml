import QtQuick 2.3

Item{
    Rectangle{
        color: "#000000"
        anchors.fill: parent
    }
    FontLoader {
        id: cFontLoader
        name: "myFont"
        source: "qrc:/font/Segoe_Script_Bold.ttf"

    }
    Canvas{
        id: mainDrawElm
        anchors.fill: parent
        antialiasing: true
        renderStrategy: Canvas.Threaded
        property int index: 0
        property int animationIndex: 0

        property int row: 1
       // Text { text: cFontLoader.status == FontLoader.Ready ? 'Loaded' : 'Not loaded' }
        Timer{
            id: fps
            interval: 100
            onTriggered: {
                mainDrawElm.requestPaint()
            }
            running: true
            repeat: true
            Component.onCompleted: fps.start()

        }
        MouseArea{
            anchors.fill: parent
            property int mx: -1
            property int my: -1
            id: mouse
            onClicked: {
                console.debug("click");
                function getPixel(x, y) {
                  var context = mainDrawElm.getContext("2d");
                  var imagedata = context.getImageData(0, 0, mainDrawElm.width, mainDrawElm.height);
                  var i = (y * imagedata.width + x) * 4;

                  return {r: imagedata.data[i], g: imagedata.data[i+1], b: imagedata.data[i+2], a: imagedata.data[i+3]};
                }

                var res = getPixel(mouseX, mouseY)
                //if()
                console.debug(res.r);
                if(res.r == 0 && res.g == 0 && res.b == 0)
                {
                    mx = mouseX;
                    my = mouseY;
                    console.debug(mx);

                }

            }
        }

        Connections {
            target: forma
            onDrawTextChanged: console.log("The application data changed!")
        }

        onPaint: {

            function setPixel(x, y, r, g, b, a) {
              var context = mainDrawElm.getContext("2d");
              var imagedata = context.getImageData(0, 0, canvas.width, canvas.height);
              var i = (y * imagedata.width + x) * 4;
              imagedata.data[i++] = r;
              imagedata.data[i++] = g;
              imagedata.data[i++] = b;
              imagedata.data[i] = a;
            }

            function getPixel(imagedata, x, y) {
              var context = mainDrawElm.getContext("2d");
              var imagedata = context.getImageData(0, 0, canvas.width, canvas.height);
              var i = (y * imagedata.width + x) * 4;

              return {r: imagedata.data[i], g: imagedata.data[i+1], b: imagedata.data[i+2], a: imagedata.data[i+3]};
            }

            function floodFill(x, y, color, borderColor){
                var ctx = mainDrawElm.getContext("2d");
                var imageData = ctx.getImageData(0, 0, 300, 300);
                var width = imageData.width;
                var height = imageData.height;
                var stack = [[x, y]];
                var pixel;
                var point = 0;
                while (stack.length > 0)
                {
                    pixel = stack.pop();
                    if (pixel[0] < 0 || pixel[0] >= width)
                        continue;
                    if (pixel[1] < 0 || pixel[1] >= height)
                        continue;

                    // Alpha
                    point = pixel[1] * 4 * width + pixel[0] * 4 + 3;

                    // Если это не рамка и ещё не закрасили
                    console.debug("color: " + imageData.data[point]);
                    if (imageData.data[point] != borderColor && imageData.data[point] != color)
                    {
                        // Закрашиваем
                        imageData.data[point] = color;

                        // Ставим соседей в стек на проверку
                        stack.push([
                            pixel[0] - 1,
                            pixel[1]
                        ]);
                        stack.push([
                            pixel[0] + 1,
                            pixel[1]
                        ]);
                        stack.push([
                            pixel[0],
                            pixel[1] - 1
                        ]);
                        stack.push([
                            pixel[0],
                            pixel[1] + 1
                        ]);
                    }
                }
                ctx.putImageData(imageData, 0, 0);
            }

            function setCharAt(str,index,chr) {
                if(index > str.length-1) return str;
                return str.substr(0,index) + chr + str.substr(index+1);
            }
            function animation(text, H)
            {

                var oneCharW = context.measureText(String(text).charAt(index)).width;

                var grd = context.createLinearGradient(0, H, animationIndex++, 0);
                grd.addColorStop(0, "white");
                grd.addColorStop(0.85, "white");
                grd.addColorStop(1, "rgba(255, 255, 255, 0)");
                context.fillStyle = grd;
                if(animationIndex > oneCharW + 3)
                {
                    animationIndex = 0;
                    index++;
                }
            }

            function wrapText(context, text, x, y, maxWidth, lineHeight) {
                    var words = (text + "  ").split(' ');
                    var line = '';
                    var oneCharW = context.measureText('W').width;
                    for(var n = 0; n < words.length; n++) {
                      var testLine = line + words[n] + ' ';
                      var metrics = context.measureText(testLine);
                      var testWidth = metrics.width;
                      // animation(text, lineHeight)
                      if (testWidth > maxWidth && n > 0) {
                          var kus = "";
                          var Width = context.measureText(line).width;
                          if(Width + 25 >= maxWidth)
                          {
                              var t_i;
                              for( t_i = 0; t_i < line.length; t_i++)
                              {
                                if(context.measureText(line.substring(0, t_i)).width > maxWidth - 20)
                                    break;
                              }
                              kus = line.substring(--t_i,line.length);
                              line = line.substring(0,t_i) + "-";
                              n--;
                              //setCharAt(line,line.length - (Width - maxWidth)/oneCharW + 1,'\0');

                          }
                          context.fillText(line, x, y);
                          if(Width + 25 >= maxWidth)
                            line = kus + " ";
                          else
                            line = words[n] + " ";
                          y+= lineHeight;
                      }
                      else {
                        line = testLine;
                      }
                    }
                    context.fillText(line, x, y);
                  }

                mainDrawElm.row = 1;
               // mainDrawElm.index = 0;
                var context = mainDrawElm.getContext("2d");
                var lineHeight = 25;
                var marginLeft = 20;
                var marginTop = 40;
                var maxWidth = mainDrawElm.width - marginLeft; //размер поле, где выводится текст
                context.reset();
                var pt = 36;
                var font = "LC Chalk";
               // console.debug(cFontLoader.status);
                context.font = forma.getFont();//pt + "pt \"" + font + "\"";
                context.fillStyle = forma.getColor();
                wrapText(context, forma.drawText, marginLeft, marginTop + pt, maxWidth, lineHeight + pt);

        }
    }
}
