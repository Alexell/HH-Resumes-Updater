//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "IniFiles.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSettingsForm *SettingsForm;

TIniFile* Settings;
//---------------------------------------------------------------------------
__fastcall TSettingsForm::TSettingsForm(TComponent* Owner)
	: TForm(Owner)
{
	if (not FileExists(ExtractFilePath(Application->ExeName)+"WebView2Loader.dll"))
	{
		Application->MessageBox(L"Отсутствует WebView2Loader.dll", Application->Title.c_str(), MB_OK | MB_ICONEXCLAMATION);
		Application->Terminate();
	}

	Settings = new TIniFile(ExtractFilePath(Application->ExeName)+"settings.ini");
	if (Settings->ReadBool("Main","Started",false) == true) {
		MainForm->Domain = SelectDomain(Settings->ReadInteger("Main","CountryIndex",0));
		MainForm->OneStart();
		SettingsForm->Close();
	}
	else
		SettingsForm->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::SaveButtonClick(TObject *Sender)
{
	Settings = new TIniFile(ExtractFilePath(Application->ExeName)+"settings.ini");
	Settings->WriteBool("Main","Started",true);
	Settings->WriteInteger("Main","CountryIndex",CBox->ItemIndex);
	MainForm->Domain = SelectDomain(CBox->ItemIndex);
	MainForm->OneStart();
	SettingsForm->Close();
}
//---------------------------------------------------------------------------

String TSettingsForm::SelectDomain(int index)
{
	String dm = "https://hh.ru/";
	switch (index) {
		case 0:  dm = "https://hh.ru/"; break;
		case 1:  dm = "https://hh.kz/"; break;
		case 2:  dm = "https://rabota.by/"; break;
		case 3:  dm = "https://hh.uz/"; break;
		case 4:  dm = "https://hh1.az/"; break;
		case 5:  dm = "https://headhunter.ge/"; break;
		case 6:  dm = "https://headhunter.kg/"; break;
	}
	return dm;
}
//---------------------------------------------------------------------------
