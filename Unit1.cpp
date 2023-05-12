//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <memory>

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"
TMainForm *MainForm;

int Step = 0;
String LastTime = "";
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
	//Настройки
	MainForm->Caption = Application->Title;
	Tray->Hint = Application->Title;
}
//---------------------------------------------------------------------------

void TMainForm::OneStart(bool minimized)
{
	MainForm->AlphaBlend = false;
	ShowWeb();
    MainForm->Position = poScreenCenter;

	StatusBar->SimpleText = "Статус: открываю hh.ru ...";
	Navigate(Domain+"locale?language=RU");
	StartTimer->Enabled = true;
	HelpLabel->Caption = "Авторизуйтесь на сайте в этом браузере, после чего программа начнет работу.";
	StatusBar->SimpleText = "Статус: проверяю авторизацию ...";
	if (minimized == true) {
		Application->Minimize();
		ShowWindow(Handle,SW_HIDE);
		Tray->Visible = true;
	}
}
//---------------------------------------------------------------------------

void TMainForm::Navigate(String URL)
{
	try
	{
		Web->Navigate(WideString(URL).c_bstr());
	}
	catch(...){}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StartTimerTimer(TObject *Sender)
{
	Web->ExecuteScript("function check_auth() { let link = document.querySelectorAll('.supernova-link'); link = Array.from( link ).filter( e => (/Мои резюме/i).test( e.textContent ) ); return(link.length); } check_auth();");
	if(Web->LocationURL.Pos("login"))
	{
		StatusBar->SimpleText = "Статус: жду авторизации пользователя ...";
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MainTimerTimer(TObject *Sender)
{
	// переход в "Мои резюме"
	if(Step == 0)
	{
		Navigate(Domain+"applicant/resumes");
		Step = 1;
		return;
	}

	// проверка авторизации
	if(Step == 1)
	{
		Web->ExecuteScript("function check_auth() { let link = document.querySelectorAll('.supernova-link'); link = Array.from( link ).filter( e => (/Мои резюме/i).test( e.textContent ) ); return(link.length); } check_auth();");
		return;
	}

	if(Step == 2)
	{
		//Считаем кол-во доступных резюме для поднятия
		Web->ExecuteScript("function check_resumes_col() { let link = document.querySelectorAll('.bloko-link'); link = Array.from( link ).filter( e => (/Поднять в поиске/i).test( e.textContent ) ); return(link.length); } check_resumes_col();");
		return;
	}

	if(Step == 3)
	{
		// поднимаем резюме в поиске
		StatusBar->SimpleText = "Статус: поднимаю резюме в поиске.";
		Web->ExecuteScript("function update_resumes() { let link = document.querySelectorAll('.bloko-link'); link = Array.from( link ).filter( e => (/Поднять в поиске/i).test( e.textContent ) ); link.forEach((el) => { el.click(); }); } update_resumes();");
		return;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LongTimerTimer(TObject *Sender)
{
	LongTimer->Enabled = false;
	Step = 0;
	MainTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void TMainForm::ClearMemory()
{
	try
	{
		THandle MainHandle;
		HANDLE ProcessHandle, ThreadHandle;
		DWORD ProcessID = GetCurrentProcessId();
		ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,false,ProcessID);
		SetProcessWorkingSetSize(ProcessHandle, DWORD(-1), DWORD(-1));
		CloseHandle(ProcessHandle);
	}
    catch(...){}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ApplicationEventsMinimize(TObject *Sender)
{
	Application->Minimize();
	ShowWindow(Handle,SW_HIDE);
	Tray->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TrayClick(TObject *Sender)
{
	Tray->Visible = false;
	Application->Restore();
	ShowWindow(Handle,SW_SHOW);
	MainForm->FormStyle = fsStayOnTop;
	MainForm->FormStyle = fsNormal;
}
//---------------------------------------------------------------------------

void TMainForm::ShowWeb()
{
	Web->Align = alTop;
	MainForm->Height = 685;
	HelpLabel->Top = 606;
	Web->ClientHeight = 600;
	Web->Visible = true;
}
//---------------------------------------------------------------------------

void TMainForm::HideWeb()
{
	Web->Visible = false;
	HelpLabel->Top = 5;
	MainForm->Height = 75;
	Web->Navigate(L"edge://");
	ClearMemory();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::WebExecuteScript(TCustomEdgeBrowser *Sender, HRESULT AResult,
		  const UnicodeString AResultObjectAsJson)
{
	if (AResultObjectAsJson != 'null') {
		if (Step == 0) {
			if (StrToInt(AResultObjectAsJson) > 0) {
				StatusBar->SimpleText = "Статус: авторизация успешна. Открываю список резюме ...";
				HelpLabel->Visible = false;
				StartTimer->Enabled = false;
				MainTimer->Enabled = true;
			}
			else
			{
				StatusBar->SimpleText = "Статус: жду авторизации ...";
				HelpLabel->Visible = true;
			}
		}

		if (Step == 1) {
			if (StrToInt(AResultObjectAsJson) > 0) {
				StatusBar->SimpleText = "Авторизация в порядке ...";
				Step = 2;
				return;
			}
			else
			{
				MainTimer->Enabled = false;
				Step = 0;
				LastTime = "";
				ShowWeb();
				StartTimer->Enabled = true;
				MainForm->Height = 685;
				HelpLabel->Top = 606;
				Web->ClientHeight = 600;
				HelpLabel->Font->Style = TFontStyles() << fsBold;
				HelpLabel->Font->Color = clRed;
				HelpLabel->Caption = "Возникла проблема с авторизацией на сайте.";
			}
		}

		if (Step == 2) {
			if (StrToInt(AResultObjectAsJson) > 0) {
				StatusBar->SimpleText = "Статус: могу поднять "+AResultObjectAsJson+" резюме.";
				Step = 3;
				return;
			}
			else
			{
				StatusBar->SimpleText = "Статус: поднятие недоступно. Ожидаем ...";
				MainTimer->Enabled = false;
				Step = 0;
				HideWeb();
				LongTimer->Enabled = true;
				return;
			}
		}

		if (Step == 3) {
			LastTime = Now().FormatString("hh:nn");
			StatusBar->SimpleText = "Статус: резюме подняты в "+LastTime+". Ожидаем ...";
			MainTimer->Enabled = false;
			Step = 0;
			HideWeb();
			LongTimer->Enabled = true;
			return;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::N1Click(TObject *Sender)
{
    SettingsForm->Show();
}
//---------------------------------------------------------------------------

