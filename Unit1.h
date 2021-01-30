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
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TCppWebBrowser *Web;
	TLabel *HelpLabel;
	TStatusBar *StatusBar;
	TTimer *StartTimer;
	TTimer *MainTimer;
	TTimer *LongTimer;
	void __fastcall StartTimerTimer(TObject *Sender);
	void __fastcall MainTimerTimer(TObject *Sender);
	void __fastcall LongTimerTimer(TObject *Sender);

private:	// User declarations
public:		// User declarations
	void Navigate(String URL);
    void UpdateResume();
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
