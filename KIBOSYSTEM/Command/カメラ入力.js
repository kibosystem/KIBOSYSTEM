カメラ入力

START:
App.RemoveAll();
var g_nGain = App.GetPrivateProfileInt("CAMERA", "gain", 0, g_strIniFile); //ゲイン
var g_nExp = App.GetPrivateProfileInt("CAMERA", "exp", 0, g_strIniFile); //露光時間
ImageDevice.SetGain(g_nGain); //ゲイン設定
ImageDevice.SetExposure(g_nExp);//露光時間設定 msec単位
ImageDevice.SetTimer(10); //タイマー再開 100msec間隔
END:
