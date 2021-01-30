//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Mshtml.h"

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"
TForm1 *Form1;

TStringList * Sheet = new TStringList();
UnicodeString Send, Temp, Temp2;
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
	UrlMkSetSessionOption(URLMON_OPTION_USERAGENT, UserAgent[r], strlen(UserAgent[r]), 0); //из библиотеки urlmon.lib

	//Подавление всплывающих ошибок браузера
	Web->Silent = true;

	//Сбрасываем старую сессию (не куки)
	InternetSetOption(0, INTERNET_OPTION_END_BROWSER_SESSION, 0, 0); //из библиотеки wininet.lib

	Send = "https://hh.ru";
	Navigate(Send);
	StartTimer->Enabled = true;
	HelpLabel->Caption = "Авторизуйтесь на сайте в этом браузере, после чего программа начнет работу.";
	StatusBar->Panels->Items[0]->Text = "Статус: проверяю авторизацию...";
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

	if(Web->LocationURL.Pos("login"))
	{
		StatusBar->Panels->Items[0]->Text = "Статус: жду авторизации...";
	}

	Sheet->Text=Web->OleObject.OlePropertyGet("document").OlePropertyGet("body").OlePropertyGet("innerHTML");
	if(Sheet->Text.Pos("Мои резюме"))
	{
		StatusBar->Panels->Items[0]->Text = "Статус: авторизация успешна.";
		HelpLabel->Visible = false;
		StartTimer->Enabled = false;
		Send = "https://hh.ru/applicant/resumes?from=header_new";
		Navigate(Send);
		Step = 1;
		MainTimer->Enabled = true;
	}
	else
	{
		StatusBar->Panels->Items[0]->Text = "Статус: жду авторизации...";
		HelpLabel->Visible = true;
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
		StatusBar->Panels->Items[0]->Text = "Статус: работаю...";

		//Считаем кол-во резюме для поднятия
		int count = 0;
		Sheet->Text=Web->OleObject.OlePropertyGet("document").OlePropertyGet("body").OlePropertyGet("innerHTML");
		if(Sheet->Text.Pos("Мои резюме"))
		{
			HelpLabel->Caption = "";
			TStringList * TempList = new TStringList();
			TempList->Delimiter = L'>';
			TempList->DelimitedText = Sheet->Text;

			for(int i = 0; i < TempList->Count; i++)
			{
				if(TempList->Strings[i].Pos("applicant-resumes-update-button") && !TempList->Strings[i].Pos("applicant-resumes-update-button_disabled") && !TempList->Strings[i+1].Pos("applicant-resumes-update-button_disabled")) count++;
			}
			delete TempList;

			if(count > 0)
			{
				StatusBar->Panels->Items[1]->Text = "Можно поднять "+IntToStr(count)+" резюме.";
				Step = 2;
				return;
			}
			else
			{
				StatusBar->Panels->Items[1]->Text = "Поднятие недоступно. Ожидаем...";
				MainTimer->Enabled = false;
				Step = 0;
				LongTimer->Enabled = true;
				return;
			}
		}
		else
		{
			MainTimer->Enabled = false;
			Step = 0;
			StartTimer->Enabled = true;
			HelpLabel->Caption = "Возникла проблема с авторизацией на сайте.";
			return;
		}
	}

	if(Step == 2)
	{
        StatusBar->Panels->Items[1]->Text = "Поднимаю резюме в поиске...";
		UpdateResume();
		Step = 0;
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
		TComInterface<IHTMLDocument2> HTMLDoc; //из подключаемого "Mshtml.h"
		Web->Document->QueryInterface(IID_IHTMLDocument2, (LPVOID*)&HTMLDoc);
		if(HTMLDoc)
		{
			WideString wtext, wcode;
			TComInterface<IHTMLElementCollection> pElements;
			IHTMLElement *pElem = NULL;
			IHTMLElement *ppElem = NULL;
			HTMLDoc->get_all(&pElements);
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
					IDispatch* pDisp;

					pElements->item(varIndex, var2, &pDisp);
					if (SUCCEEDED(pDisp -> QueryInterface (IID_IHTMLElement, (LPVOID*)&pElem)) && pElem)
					{
						pElem->get_innerText(&wtext);
						if (wtext == "Поднять в поиске")
						{
							pElem->get_parentElement(&ppElem);
							ppElem->get_outerHTML(&wcode);
							if(wcode.Pos("<span class") == 1)
							{
								if(!wcode.Pos("button_disabled"))
								{
									try
									{
										pElem->click();
									}
									catch(...){}
									break;
								}
							}
						}
					}
				}
			}
			pElements->Release();
			pElem->Release();
			ppElem->Release();
		}
        HTMLDoc->Release();
	}
}
//---------------------------------------------------------------------------------


