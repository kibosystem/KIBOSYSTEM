実行.js
START:
    App.OutputClear(); //出力消去
    App.RemoveAll(); //画面描画消去

    App.Exec("./Tesseract-OCR/tesseract.exe moji.png result -l jpn"); //文字認識実行

    App.ReadText("result.txt"); //文字列読み込み

    var strText0 = App.ReadString("result.txt", 0);
    var strText1 = App.ReadString("result.txt", 1);
    var strText2 = App.ReadString("result.txt", 2);

    App.DrawText("test0", strText0, 10, 10);
    App.DrawText("test1", strText1, 10, 210);
    App.DrawText("test2", strText2, 10, 410);    
    App.Render();
END:
