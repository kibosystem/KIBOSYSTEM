#pragma once

//タイマーイベント
enum{
	 TIMER_SCRIPT = 1		//スクリプト連続実行タイマー
	,TIMER_REPEAT_SCRIPT	//繰り返し専用スクリプトの実行
	,TIMER_IMAGE_DEVICE		//画像入力機器用タイマーID
	,TIMER_CLOSE			//タイマーによる終了待ちの場合
	,TIMER_DUMY				//dumy番号
	,TIMER_FORCED_CLOSE		//強制的な終了

};

//ステータスバー情報
enum{
	STATUS_CAMERA = 0,		//カメラ座標
	STATUS_RGB,		//RGB
	STATUS_STAGE,	//ステージ
	STATUS_CAPS,	//cap
	STATUS_NUM,		//num
	STATUS_SCRL,	//scroll
	STATUS_MAX
};

