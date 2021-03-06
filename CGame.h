#ifndef _CGAME_H
#define _CGAME_H
////////////////////////////////////////////////////////////////////////////
// CGame : ゲームメインルーチン v2.00
// ・Win10動作確認済み
////////////////////////////////////////////////////////////////////////////

#include "./libfiles/CDSPro81.h"
#include "./libfiles/CDIPro81.h"
#include "./libfiles/CWindow.h"
#include "./libfiles/CBmsPro.h"
#include "./libfiles/CTimer.h"

#include "./libfiles/MusicGame.h"
#include "./libfiles/DirectXMovie.h"


#include "./libfiles/HighPrecisionTimer.hpp"
#include "./Dx9Wrapper_v0.2.0/Dx9Wrapper.h"
#include "./Dx9Wrapper_v0.2.0/MIDIinput.h"



#define MAXMUSICCNT				(128)		// 読み込める曲の最大数

#define JUDGEEFFECTCNT			(80)
#define RIPPLEEFFECTCNT			(70)

#define VIRTUALKEYCNT			(25)		// 仮想入力キーの数


#define SND_AMAOTO				BMS_MAXINDEX


#define ELEMENTSIZE(x) ((sizeof(x)/sizeof(x[0])))


typedef struct _EFFECTSTATE {
	int count;
	int x;
	int y;
}EFFECTSTATE;


// ステート
enum STATE {
	G_INIT = 0,	// ゲーム初期化
	G_GAMEINIT,	// 音楽ゲーム初期化(テクスチャの読み込みなど
	G_TITLEINIT,
	G_TITLE,		// タイトル画面
	G_TITLEFADEOUT,
	G_SELECTMUSICINIT,	// 曲選択処理の初期化
	G_SELECTMUSIC,		// 曲選択処理
	G_SELECTDIFFICULTYINIT,	// 難易度選択処理の初期化
	G_SELECTDIFFICULTY,		// 難易度選択処理
	G_SELECTDIFFFADEOUT,
	G_LOADING,
	G_PLAYSONGINIT,	// 曲演奏処理の初期化
	G_PLAYSONG,		// 曲演奏処理
	G_RESULTINIT,	// 演奏結果画面の初期化
	G_RESULT,		// 演奏結果画面
	G_RESULT2,		// 演奏結果画面(詳細
	G_PLAYSONGEND,	// 曲演奏結果の終了処理
	G_END		// 全てのゲーム終了処理
};


class CGame {
	// ライブラリなど
	CWindow			win;				// ウインドウ管理

	CDSPro81		ds;					// DirectSound管理
	CDIPro81		di;					// DirectInput全般
	DirectXFont		dty;		// テキスト関連 游ゴシック体
	DirectXFontAscii dtc;		// century gothic fontsize:72
	DirectXFontAscii dtcs;		// century gothic fontsize:30
	DirectXFontAscii dts;		// sans serif fontsize:14

	MyMusicGame		mg;					// 音楽ゲームの内部処理
	DirectXMovie	dm;					// 動画の再生クラス
	MIDIInput		mi;					// midiキーボードの入力

	HighPrecisionTimer hptimer;
	LogFile			logfile;

	CTimer			tm;					// 60FPS処理
	CBmsPro			bms[MAXMUSICCNT];	// BMSファイル管理用

	bool			bLostDevice;		// D3Dデバイスロスト状態フラグ


	// テクスチャ
	Texture			tex_judgeline;
	Texture			tex_judgelineeffect;
	Texture			tex_note;
	Texture			tex_diffbox_elementary;
	Texture			tex_diffbox_intermediate;
	Texture			tex_diffbox_advanced;
	Texture			tex_diffbox_master;
	Texture			tex_diffbox_professional;
	Texture			tex_diffbox_shade;
	Texture			tex_jacketbox;
	Texture			tex_scorebox;
	Texture			tex_bigjacketbox;
	Texture			tex_resultjacketbox;
	Texture			tex_jacketbox_shade;
	Texture			tex_bigjacketbox_shade;
	Texture			tex_resultjacketbox_shade;
	Texture			tex_judgestr_early2;
	Texture			tex_judgestr_early;
	Texture			tex_judgestr_juste;
	Texture			tex_judgestr_just;
	Texture			tex_judgestr_justl;
	Texture			tex_judgestr_late;
	Texture			tex_judgestr_late2;
	Texture			tex_judgestr_miss;
	Texture			tex_judgeeffect_just;
	Texture			tex_rippleglay;
	Texture			tex_backgnd_musicselection;
	Texture			tex_logo_amaoto;
	Texture			tex_logo_amatsubu;
	Texture			tex_backgnd_title;
	Texture			tex_backgnd_white;
	Texture			tex_buttoneffect;
	Texture			tex_ripplewhite;

	int				backbufferWidth;
	int				backbufferHeight;

	float			noteScale;

	std::array<Texture, BMS_MAXINDEX> tex_backgnd;
	std::array<Texture, MAXMUSICCNT> tex_jacket;


	// ゲームのメイン処理で使用する変数
	STATE			eState;				// ゲームのステート状況
	float			fScrMulti;			// 小節間の幅の倍率
	long			llStartTime;		// ゲーム開始時の時間(高解像度タイマー)
	long		llGlobalFreq;		// マシンの１秒間のカウント数(高解像度タイマー)
	double			dElapsedTime;		// 開始からの経過時間(秒)
	int				iStartNum[BMS_MAXNOTELANE+BMS_MAXINDEX];		// BMS演奏で計算開始する配列番号(処理を軽くするため)

	int				iCursor1;			// 現在選択中の項目
	int				iCursor2;			// 現在選択中の項目


	// ファイルロード
	int				iLoadMusicCnt;			// ロードした曲数

	// 入力状態
	bool			bOnVirtualKey[VIRTUALKEYCNT];		// 仮想入力キーが押されているか
	long			lHoldNote[VIRTUALKEYCNT];			// 現在判定中のHOLDノートID

	// 演出など
	int				iNoteEffectIndex[BMS_MAXNOTELANE];		// 次に使用される判定表示エフェクトのインデックス
	int				iNoteEffectCount[BMS_MAXNOTELANE][30];	// 判定表示エフェクトのカウンタ
	int				iRippleEffectIndex;						// 波紋エフェクトカウンタのインデックス
	EFFECTSTATE		iRippleEffectCount[50];					// 波紋エフェクトのカウント
	int				iBackMediaIndex;	// 現在表示中の背景メディアのインデックス番号
	bool			bIsBackMedia;		// 背景メディアが存在するかのフラグ
	bool			bFlag;				// 何かに使う

private:
	// 初期化
	bool Init(HINSTANCE hinst);		// 初期化＆ゲーム形成
	bool Clear(void);					// ロード済みデータの開放


	// ゲーム処理
	bool	InitGame();
	bool	InitPlayMusicMode(long musicID, DIFFICULTY diffID);							// 演奏モード初期化
	bool	InitTitle();
	int		RunTitle();																	// タイトル画面ループ
	int		RunPlayMusicMode(long musicID, bool demo = FALSE);							// 演奏モード実行ループ
	bool	InitMusicSelectionMode();													// 曲選択モード初期化
	int		RunMusicSelectionMode();													// 曲選択モード実行ループ
	bool	InitDifficultySectionMode();												// 難易度選択モード初期化
	int		RunDifficultySelectionMode();												// 難易度選択モード実行ループ
	bool	InitResultMode();
	int		RunResultMode();
	bool	ExitPlayMusicMode();
	bool	RunScreenTransition(double stime);
	bool	RunLoadingScreen();

	bool	LoadAllBmsHeader(const char *file);
	bool	InputVirtualKey(bool *virtualKey, MIDIKEYSTATE *midiKey, unsigned char *keyboard, bool IsUseMidi);	// 仮想入力キーへ入力


private:
	double	dGrooveRateCoef[BMS_MAXNOTELANE];	// ※CalcGrooveRate()で使います

public:
	// 公開関数
	CGame();
	virtual ~CGame();

	bool Run(HINSTANCE hinst);		// ゲームメインルーチン
};



#endif
