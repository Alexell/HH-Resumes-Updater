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

UnicodeString Send;
int Step = 0;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	//Настройки
	Form1->Caption = Application->Title;

	//Подмена UserAgent
	char UserAgent[3][200] =
	{
	"Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1",
	"Mozilla/5.0 (iPhone; CPU iPhone OS 13_0 like Mac OS X) AppleWebKit/605.0.1 (KHTML, like Gecko) Version/13.0 Mobile/17A577 Safari/604.1",
	"Mozilla/5.0 (iPhone; CPU iPhone OS 14_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0.2 Mobile/15E148 Safari/604.1"
	};

	randomize();
	int r = random(3);
	UrlMkSetSessionOption(URLMON_OPTION_USERAGENT, UserAgent[r], strlen(UserAgent[r]), 0); //библиотека urlmon.lib

	//Подавление всплывающих ошибок браузера
	Web->Silent = true;

	//Сбрасываем старую сессию (не куки)
	InternetSetOption(0, INTERNET_OPTION_END_BROWSER_SESSION, 0, 0); //библиотека wininet.lib
	StatusBar->SimpleText = "Статус: открываю hh.ru ...";
	Send = "https://hh.ru";
	Navigate(Send);
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
	std::unique_ptr<TStringList>Sheet(new TStringList());
	Sheet->Text=Web->OleObject.OlePropertyGet("document").OlePropertyGet("body").OlePropertyGet("innerHTML");
	if(Sheet->Text.Pos("Мои резюме"))
	{
		StatusBar->SimpleText = "Статус: авторизация успешна. Открываю список резюме ...";
		HelpLabel->Visible = false;
		StartTimer->Enabled = false;
		Send = "https://hh.ru/applicant/resumes?from=header_new";
		Navigate(Send);
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
		Web->Refresh();
		Step = 1;
		return;
	}

	if(Step == 1)
	{
		//Считаем кол-во резюме для поднятия
		int count = 0;
		std::unique_ptr<TStringList>Sheet(new TStringList()); //#include <memory>
		Sheet->Text=Web->OleObject.OlePropertyGet("document").OlePropertyGet("body").OlePropertyGet("innerHTML");
		if(Sheet->Text.Pos("Мои резюме"))
		{
			HelpLabel->Caption = "";
			HelpLabel->Font->Color = clWindowText;
			HelpLabel->Font->Style = TFontStyles() >> fsBold;

			std::unique_ptr<TStringList>TempList(new TStringList());
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
				return;
			}
			else
			{
				StatusBar->SimpleText = "Статус: поднятие недоступно. Ожидаем ...";
				MainTimer->Enabled = false;
				Step = 0;
				ClearMemory();
				LongTimer->Enabled = true;
				return;
			}
		}
		else
		{
			MainTimer->Enabled = false;
			Step = 0;
			Send = "https://hh.ru/account/login";
			Navigate(Send);
			StartTimer->Enabled = true;
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
