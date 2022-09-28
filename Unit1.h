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
#include <WebView2.hpp>
#include <Winapi.ActiveX.hpp>
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
	void __fastcall StartTimerTimer(TObject *Sender);
	void __fastcall MainTimerTimer(TObject *Sender);
	void __fastcall LongTimerTimer(TObject *Sender);
	void __fastcall ApplicationEventsMinimize(TObject *Sender);
	void __fastcall TrayClick(TObject *Sender);
	void __fastcall WebExecuteScript(TCustomEdgeBrowser *Sender, HRESULT AResult, const UnicodeString AResultObjectAsJson);


private:	// User declarations
public:		// User declarations
	void Navigate(String URL);
	void ClearMemory();
	void ShowWeb();
	void HideWeb();
    void OneStart();
	String Domain;
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
