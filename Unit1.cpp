//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <memory>
#include "Mshtml.h"

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"
TForm1 *Form1;

TCppWebBrowser *Web = NULL;
int Step = 0;
String LastTime = "";
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	//Настройки
	Form1->Caption = Application->Title;
	Tray->Hint = Application->Title;

	CreateWeb(true);

	StatusBar->SimpleText = "Статус: открываю hh.ru ...";
	Navigate("https://hh.ru");
	StartTimer->Enabled = true;
	HelpLabel->Caption = "Авторизуйтесь на сайте в этом браузере, после чего программа начнет работу.";
	StatusBar->SimpleText = "Статус: проверяю авторизацию ...";
}
//---------------------------------------------------------------------------

void TForm1::Navigate(String URL)
{
	try
	{
		Web->Navigate(WideString(URL).c_bstr());
	}
	catch(...){}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StartTimerTimer(TObject *Sender)
{
	std::auto_ptr<TStringList> Sheet(new TStringList()); //#include <memory>
	Sheet->Text = Web->OleObject.OlePropertyGet("document").OlePropertyGet("body").OlePropertyGet("innerHTML");
	if(Sheet->Text.Pos("Мои резюме"))
	{
		StatusBar->SimpleText = "Статус: авторизация успешна. Открываю список резюме ...";
		HelpLabel->Visible = false;
		StartTimer->Enabled = false;
		Navigate("https://hh.ru/applicant/resumes?from=header_new");
		Step = 1;
		MainTimer->Enabled = true;
	}
	else
	{
		StatusBar->SimpleText = "Статус: жду авторизации ...";
		HelpLabel->Visible = true;
	}

	if(Web->LocationURL.Pos("login"))
	{
		StatusBar->SimpleText = "Статус: жду авторизации пользователя ...";
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MainTimerTimer(TObject *Sender)
{
	if(Step == 0)
	{
		CreateWeb();
		Navigate("https://hh.ru/applicant/resumes?from=header_new");
		Step = 1;
		return;
	}

	if(Step == 1)
	{
		//Считаем кол-во резюме для поднятия
		int count = 0;
		std::auto_ptr<TStringList> Sheet(new TStringList());
		Sheet->Text = Web->OleObject.OlePropertyGet("document").OlePropertyGet("body").OlePropertyGet("innerHTML");
		if(Sheet->Text.Pos("Мои резюме"))
		{
			HelpLabel->Caption = "";
			HelpLabel->Font->Color = clWindowText;
			HelpLabel->Font->Style = TFontStyles() >> fsBold;

			std::auto_ptr<TStringList> TempList(new TStringList());
			TempList->Delimiter = L'>';
			TempList->DelimitedText = Sheet->Text;
			for(int i = 0; i < TempList->Count; i++)
			{
				if(TempList->Strings[i].Pos("applicant-resumes-update-button") && !TempList->Strings[i].Pos("applicant-resumes-update-button_disabled") && !TempList->Strings[i+1].Pos("applicant-resumes-update-button_disabled")) count++;
			}

			if(count > 0)
			{
				StatusBar->SimpleText = "Статус: могу поднять "+IntToStr(count)+" резюме.";
				Step = 2;
				if (count == 1) LastTime = Now().FormatString("hh:nn");
				return;
			}
			else
			{
				if (LastTime == "")
					StatusBar->SimpleText = "Статус: поднятие недоступно. Ожидаем ...";
				else
					StatusBar->SimpleText = "Статус: резюме подняты в "+LastTime+". Ожидаем ...";

				MainTimer->Enabled = false;
				Step = 0;
				DestroyWeb();
				LongTimer->Enabled = true;
				return;
			}
		}
		else
		{
			MainTimer->Enabled = false;
			Step = 0;
			LastTime = "";
			Navigate("https://hh.ru/account/login");
			StartTimer->Enabled = true;
			Form1->Height = 676;
			HelpLabel->Top = 606;
            Web->ClientHeight = 600;
			HelpLabel->Font->Style = TFontStyles() << fsBold;
			HelpLabel->Font->Color = clRed;
			HelpLabel->Caption = "Возникла проблема с авторизацией на сайте.";
			return;
		}
	}

	if(Step == 2)
	{
		StatusBar->SimpleText = "Статус: поднимаю резюме в поиске.";
		UpdateResume();
		Step = 1;
		return;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LongTimerTimer(TObject *Sender)
{
	LongTimer->Enabled = false;
	Step = 0;
	MainTimer->Enabled = true;
}
//---------------------------------------------------------------------------

//Функция нажатия кнопки "Поднять в поиске"
void TForm1::UpdateResume()
{
	if(Web->Document)
	{
		IHTMLDocument2 *HTMLDoc = NULL; //#include "Mshtml.h"
		Web->Document->QueryInterface(IID_IHTMLDocument2, (LPVOID*)&HTMLDoc);
		if(HTMLDoc)
		{
			IHTMLElementCollection *pElements = NULL;
			if(SUCCEEDED(HTMLDoc->get_all(&pElements)) && pElements)
			{
				IHTMLElement *pElem = NULL;
				IHTMLElement *ppElem = NULL;
				IDispatch* pDisp = NULL;
				long numelems;
				HRESULT hres = pElements->get_length(&numelems);
				if (hres == S_OK)
				{
					for (int i=0; i < numelems; i++)
					{
						VARIANT varIndex;
						varIndex.vt = VT_UINT;
						varIndex.lVal = i;
						VARIANT var2;
						VariantInit( &var2 );

						if(SUCCEEDED(pElements->item(varIndex, var2, &pDisp)) && pDisp)
						{
							if (SUCCEEDED(pDisp -> QueryInterface (IID_IHTMLElement, (LPVOID*)&pElem)) && pElem)
							{
								WideString wtext;
								pElem->get_innerText(&wtext);
								if (wtext == "Поднять в поиске")
								{
									WideString wcode;
									pElem->get_parentElement(&ppElem);
									ppElem->get_outerHTML(&wcode);
									ppElem->Release();
									if(wcode.Pos("<span class") == 1)
									{
										if(!wcode.Pos("button_disabled"))
										{
											pElem->click();
											break;
										}
									}
								}
								pElem->Release();
							}
							pDisp->Release();
						}
					}
				}
			}
			pElements->Release();
		}
		HTMLDoc->Release();
	}
}
//---------------------------------------------------------------------------------

void TForm1::ClearMemory()
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

void __fastcall TForm1::ApplicationEventsMinimize(TObject *Sender)
{
	ShowWindow(Handle,SW_HIDE);
	ShowWindow(Application->Handle,SW_HIDE);
	Application->Minimize();
	Tray->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TrayClick(TObject *Sender)
{
	Tray->Visible = false;
	Application->Restore();
	ShowWindow(Application->Handle,SW_SHOW);
	ShowWindow(Handle,SW_SHOW);
	Form1->FormStyle = fsStayOnTop;
    Form1->FormStyle = fsNormal;
}
//---------------------------------------------------------------------------

void TForm1::CreateWeb(bool show)
{
	Web = new TCppWebBrowser(Form1);
	Form1->InsertControl(Web);
	Web->Align = alTop;
	Web->ClientHeight = 0;

	//Меняем UserAgent
	char *ua = new char[140];
	strcpy(ua,AnsiString("Mozilla/5.0 (iPhone; CPU iPhone OS 14_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.2 Mobile/15E148 Safari/604.1").c_str());
	UrlMkSetSessionOption(URLMON_OPTION_USERAGENT, ua, strlen(ua), 0); //библиотека urlmon.lib
	delete []ua;

	Web->Silent = true; //подавление ошибок сценариев

	if (show)
	{
		Form1->Height = 676;
		HelpLabel->Top = 606;
		Web->ClientHeight = 600;
	}
}
//---------------------------------------------------------------------------

void TForm1::DestroyWeb()
{
	FreeAndNil(&Web);
	ClearMemory();
	HelpLabel->Top = 5;
	Form1->Height = 75;
}
//---------------------------------------------------------------------------
