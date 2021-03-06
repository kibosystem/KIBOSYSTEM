セカンドディスプレイによる画像計測

START:
var ID_USER1 = 0x401; //
var ID_USER2 = 0x402; //
var ID_USER3 = 0x403; //
var ID_USER4 = 0x404; //
var ID_USER5 = 0x405; //
var ID_USER6 = 0x406; //
var ID_USER7 = 0x407; //
var ID_USER8 = 0x408; //
var ID_USER9 = 0x409; //
var ID_USERA = 0x40A; //
var ID_USERB = 0x40B; //
var ID_USERC = 0x40C; //
var ID_USERD = 0x40D; //
var ID_USERE = 0x40E; //
var ID_USERF = 0x40F; //
var ID_CALIBRATION = 0x500;	    //キャリブレーション
var ID_INIT = 0x600;    //初期化処理
var ID_BINARY_DLG = 0x700;	    //2値化ダイアログ

var g_strIniFile = "KIBOSYSTEM.ini";

//撮影サイズ
var g_nW = App.GetPrivateProfileInt("CAMERA", "WIDTH" ,640,g_strIniFile);  //撮影画像幅
var g_nH = App.GetPrivateProfileInt("CAMERA", "HEIGHT",480,g_strIniFile);  //撮影画像高さ
var g_nViewCx = App.GetPrivateProfileInt("CAMERA", "VIEWWIDTH" , 640, g_strIniFile); //表示幅
var g_nViewCy = App.GetPrivateProfileInt("CAMERA", "VIEWHEIGHT", 480, g_strIniFile); //表示高さ

//中心
var g_nCenterX = g_nW / 2;  //撮影画像幅
var g_nCenterY = g_nH / 2;  //撮影画像高さ
var g_dbPer = App.GetPrivateProfileDbl("CALIBRATION", "PERPIXEL", 1, g_strIniFile); //レジストリ設定取得
var g_nGain = App.GetPrivateProfileInt("CAMERA","gain",0,g_strIniFile); //ゲイン
var g_nExp = App.GetPrivateProfileInt("CAMERA","exp",0,g_strIniFile); //露光時間

var VK_F1 = 0x70; //ファンクションキーF1のキー番号    
App.SetKeyFile(VK_F1, "Command\\カメラ入力.js");//F1キーにコマンドを対応させます。
App.SetKeyFile(VK_F1 + 1, "Command\\TEST.js");//F2キーにコマンドを対応させます。
App.SetKeyFile(VK_F1 + 2, "Command\\画像切り替え.js");//F3キーにコマンドを対応させます。

ImageDevice.Create(g_nW, g_nH);   //カメラ作成
if (ImageDevice.IsInitOK) { //カメラの初期化成功しました

    ImageDevice.SetDrawSize(g_nViewCx, g_nViewCy); //表示サイズ変更
    App.SetViewRate(g_nViewCx / g_nW, g_nViewCy / g_nH); //表示座標の倍率設定
    ImageDevice.SetImagePosition(0, 0); //カメラ0番画像表示位置を(0,0)にします。
    ImageDevice.FromFile("sample1.png");//サンプル画像読み込み

    App.SendMessage(ID_INIT);
  
    App.SetFontName("ＭＳ ゴシック");//ＭＳ ゴシック ,ＭＳ 明朝,etc
    App.SetFontSize(60); //高さをpixel単位指定 
    App.SetTextColor(0, 255, 0);

    ImageDevice.SetExposure( g_nExp);//露光時間設定 msec単位
    ImageDevice.SetGain(g_nGain); //ゲイン設定
    ImageDevice.SetTimer(1); //0番カメラタイマー再開 100msec間隔    
    ImageDevice.SetContrast(512);//コントラスト最大
    App.WriteProfileString("CAPTION","画像計測"); 
    App.SetWindowText("KIBO 2019.4.11 version ");
    App.ViewWidth(g_nViewCx); //画像表示部幅設定
    App.ShowWindow(3);

    App.TextOutput(1, "出力１データがここに表示されます。");
    App.TextOutput(2, "出力２データがここに表示されます。");
    App.TextOutput(3, "出力３データがここに表示されます。");
}

END:
