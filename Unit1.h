//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "SHDocVw_OCX.h"
#include <Vcl.OleCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Vcl.Edge.hpp>
#include <Winapi.ActiveX.hpp>
#include <Winapi.WebView2.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *HelpLabel;
	TStatusBar *StatusBar;
	TTimer *StartTimer;
	TTimer *MainTimer;
	TTimer *LongTimer;
	TTrayIcon *Tray;
	TApplicationEvents *ApplicationEvents;
	TEdgeBrowser *Web;
	TMainMenu *MainMenu;
	TMenuItem *N1;
	void __fastcall StartTimerTimer(TObject *Sender);
	void __fastcall MainTimerTimer(TObject *Sender);
	void __fastcall LongTimerTimer(TObject *Sender);
	void __fastcall ApplicationEventsMinimize(TObject *Sender);
	void __fastcall TrayClick(TObject *Sender);
	void __fastcall WebExecuteScript(TCustomEdgeBrowser *Sender, HRESULT AResult, const UnicodeString AResultObjectAsJson);
	void __fastcall N1Click(TObject *Sender);


private:	// User declarations
public:		// User declarations
	void Navigate(String URL);
	void ClearMemory();
	void ShowWeb();
	void HideWeb();
	void OneStart(bool minimized);
	String Domain;
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
