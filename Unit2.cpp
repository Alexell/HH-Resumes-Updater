//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "IniFiles.hpp"
#include "Registry.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSettingsForm *SettingsForm;

TIniFile* Settings;
//---------------------------------------------------------------------------
__fastcall TSettingsForm::TSettingsForm(TComponent* Owner)
	: TForm(Owner)
{
	if (not FileExists(ExtractFilePath(Application->ExeName)+"WebView2Loader.dll")) {
		Application->MessageBox(L"Отсутствует WebView2Loader.dll", Application->Title.c_str(), MB_OK | MB_ICONEXCLAMATION);
		Application->Terminate();
	}

	Settings = new TIniFile(ExtractFilePath(Application->ExeName) + "settings.ini");
	if (Settings->ReadBool("Main", "Started", false) == true) {
		MainForm->Domain = SelectDomain(Settings->ReadInteger("Main", "CountryIndex", 0));
		bool minimized = false;
		if (Settings->ReadBool("Main", "AutoMinimize", false) == true) minimized = true;
		MainForm->OneStart(minimized);
		SettingsForm->Close();
	}
	else
		SettingsForm->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::SaveButtonClick(TObject *Sender)
{
	Settings = new TIniFile(ExtractFilePath(Application->ExeName) + "settings.ini");
	Settings->WriteBool("Main", "Started", true);
	Settings->WriteInteger("Main", "CountryIndex", CBox->ItemIndex);
	Settings->WriteBool("Main", "AutoRun", AutorunBox->Checked);
	Settings->WriteBool("Main", "AutoMinimize", MinimizeBox->Checked);
	if (AutorunBox->Checked == true) {
		TRegistry *reg = new TRegistry();
		reg->RootKey = HKEY_CURRENT_USER;
		reg->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Run", true);
		reg->WriteString("HH Resumes Updater", Application->ExeName);
		reg->CloseKey();
	} else {
		TRegistry *reg = new TRegistry();
		reg->RootKey = HKEY_CURRENT_USER;
		reg->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Run", true);
		reg->DeleteValue("HH Resumes Updater");
		reg->CloseKey();
	}
	MainForm->Domain = SelectDomain(CBox->ItemIndex);
	MainForm->OneStart(false);
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
void __fastcall TSettingsForm::FormShow(TObject *Sender)
{
	Settings = new TIniFile(ExtractFilePath(Application->ExeName) + "settings.ini");
	CBox->ItemIndex = Settings->ReadInteger("Main", "CountryIndex", 0);
	AutorunBox->Checked = Settings->ReadBool("Main", "AutoRun", 0);
	MinimizeBox->Checked = Settings->ReadBool("Main", "AutoMinimize", 0);
}
//---------------------------------------------------------------------------

