実行.js
START:
    App.OutputClear(); //出力消去
    App.RemoveAll(); //画面描画消去
var nNum = App.SendMessage(ID_USER1);
//    var nNum = App.SendMessage(ID_USER2);
    if (nNum == 10) { //10個
        App.SetTextColor(0, 0, 255);
        App.DrawText("判定結果", "OK", 10, 10);
    } else {
        App.SetTextColor(255, 0, 0);
        App.DrawText("判定結果", "NG", 10, 10);
    }
    App.Render();
END:
