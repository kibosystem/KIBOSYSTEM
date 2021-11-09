//キャリブレーション
[CALIBRATION]
LENGTH : 長さ基準

START:
App.OutputClear(); //出力消去
App.RemoveAll(); //画面描画消去
ImageDevice.KillTimer();

//ここでキャリブレーション画像を読み込んでいますがカメラを使用する際はこの行を削除してください
//ImageDevice.FromFile("calibration.bmp");

var nCnum = 0;
App.TextOutput("キャリブレーション開始");
App.SendMessage(ID_CALIBRATION);//キャリブレーション実行1
App.Render();
END:
