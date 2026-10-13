//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <memory>

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;

int Step = 0;
ULONGLONG ButtonScanStarted = 0;
int ResumesLeft = 0;
String LastTime = "";
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
	// настройки
	MainForm->Caption = Application->Title;
	Tray->Hint = Application->Title;
}
//---------------------------------------------------------------------------

void TMainForm::Start(bool minimized)
{
	ShowWeb();
	N2->Caption = "Скрыть браузер";
    MainForm->Position = poScreenCenter;
	MainForm->AlphaBlend = false;

	StatusBar->SimpleText = "Статус: открываю HeadHunter...";
	Navigate(Domain + "/locale?language=RU");
	Step = 1;
	MainTimer->Enabled = true;
	StatusBar->SimpleText = "Статус: проверяю авторизацию...";

	if (minimized == true) {
		Application->Minimize();
		ShowWindow(Handle, SW_HIDE);
		Tray->Visible = true;
	}
}
//---------------------------------------------------------------------------

void TMainForm::Navigate(String URL)
{
	try {
		Web->Navigate(WideString(URL).c_bstr());
	} catch(...) {}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AuthTimerTimer(TObject *Sender)
{
	AuthTimer->Enabled = false;
	Web->ExecuteScript(R"(
		(function() {
			return document.body.innerHTML.includes('Резюме и&nbsp;профиль') ? 1 : 0;
		})();
	)");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MainTimerTimer(TObject *Sender)
{
	// проверка авторизации
	if (Step == 1) {
		Web->ExecuteScript(R"(
			(function() {
				return document.body.innerHTML.includes('Резюме и&nbsp;профиль') ? 1 : 0;
			})();
		)");

		return;
	}

	// переход в "Мои резюме"
	if (Step == 2) {
		Navigate(Domain + "/applicant/my_resumes?hhtmFrom=profile");
		Step = 3;
        ButtonScanStarted = GetTickCount64();
		return;
	}

	if (Step == 3) {
		// считаем кол-во доступных резюме для поднятия
		MainTimer->Enabled = false;
        Web->ExecuteScript(R"HH(
(function() {

function candidates() {
    return Array.from(document.querySelectorAll('button[data-qa~="resume-update-button"]'))
        .filter(b => !b.disabled && b.getAttribute('aria-disabled') !== 'true');
}
function resumeKey(b) {
    for (let p = b.parentElement; p && p !== document.body; p = p.parentElement) {
        const links = p.querySelectorAll('[data-qa^="resume-card-link-"]');
        if (links.length === 1) return links[0].getAttribute('data-qa');
        if (links.length > 1) break;
    }
    return null;
}

if (!/^\/applicant\/my_resumes\/?$/.test(location.pathname)) return -2;
const matches = candidates();
if (matches.length) window.__hhClickedResumeKeys = new Set();
return matches.length;
})();
)HH");

		return;
	}

	if (Step == 4) {
		// поднимаем резюме в поиске
		StatusBar->SimpleText = "Статус: поднимаю резюме в поиске (осталось " + IntToStr(ResumesLeft) + ").";

		MainTimer->Enabled = false;
        Web->ExecuteScript(R"HH(
(function() {

function candidates() {
    return Array.from(document.querySelectorAll('button[data-qa~="resume-update-button"]'))
        .filter(b => !b.disabled && b.getAttribute('aria-disabled') !== 'true');
}
function resumeKey(b) {
    for (let p = b.parentElement; p && p !== document.body; p = p.parentElement) {
        const links = p.querySelectorAll('[data-qa^="resume-card-link-"]');
        if (links.length === 1) return links[0].getAttribute('data-qa');
        if (links.length > 1) break;
    }
    return null;
}

const seen = window.__hhClickedResumeKeys || (window.__hhClickedResumeKeys = new Set());
const close = document.querySelector('button[data-qa="bot-update-resume-modal__close-button"]');
if (close && close.getClientRects().length && !close.disabled) {
    close.click();
    return 0;
}
const btn = candidates().find(b => {
    const id = resumeKey(b);
    return id ? !seen.has(id) : b.dataset.hhUpdaterClicked !== '1';
});
if (!btn) return -1;
const id = resumeKey(btn);
if (id) seen.add(id);
btn.dataset.hhUpdaterClicked = '1';
btn.click();
setTimeout(() => {
    const b = document.querySelector('button[data-qa="bot-update-resume-modal__close-button"]');
    if (b && !b.disabled) b.click();
}, 700);
return 1;
})();
)HH");

		return;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LongTimerTimer(TObject *Sender)
{
	LongTimer->Enabled = false;
	Navigate(Domain + "/locale?language=RU");
    Step = 1;
	MainTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void TMainForm::ClearMemory()
{
	try {
		THandle MainHandle;
		HANDLE ProcessHandle, ThreadHandle;
		DWORD ProcessID = GetCurrentProcessId();
		ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, ProcessID);
		SetProcessWorkingSetSize(ProcessHandle, DWORD(-1), DWORD(-1));
		CloseHandle(ProcessHandle);
	} catch(...) {}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ApplicationEventsMinimize(TObject *Sender)
{
	MainForm->Visible = false;
	ShowWindow(Application->Handle, SW_HIDE);
	Tray->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TrayClick(TObject *Sender)
{
	MainForm->Visible = true;
	ShowWindow(Application->Handle, SW_SHOW);
	Application->Restore();
	Tray->Visible = false;
	SetForegroundWindow(MainForm->Handle);
    SetActiveWindow(MainForm->Handle);
}
//---------------------------------------------------------------------------

void TMainForm::ShowWeb()
{
	MainForm->Height = 685;
}
//---------------------------------------------------------------------------

void TMainForm::HideWeb()
{
	MainForm->Height = 75;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::WebExecuteScript(TCustomEdgeBrowser *Sender, HRESULT AResult,
		  const UnicodeString AResultObjectAsJson)
{
	int result = 0;
	try {
		result = StrToInt(AResultObjectAsJson);
	} catch (...) {
        if (Step == 3 || Step == 4) {
            StatusBar->SimpleText = "Статус: ошибка скрипта кнопок. Ожидаем...";
            Step = 1;
            LongTimer->Enabled = true;
        }
		return;
	}

	if (Step == 1) {
		if (result > 0) {
            MainTimer->Enabled = true;
			StatusBar->SimpleText = "Статус: авторизация успешна.";
			Step = 2;
		} else {
			MainTimer->Enabled = false;
			AuthTimer->Enabled = true;
			Step = 1;
			LastTime = "";
			StatusBar->SimpleText = "Статус: жду авторизации пользователя...";
		}
		return;
	}

	if (Step == 3) {
        ResumesLeft = result;
        if (result > 0) {
            StatusBar->SimpleText = "Статус: найдено доступных кнопок: " + IntToStr(result) + ".";
            Step = 4;
            MainTimer->Enabled = true;
        } else if (GetTickCount64() - ButtonScanStarted < 30000) {
            StatusBar->SimpleText = "Статус: ожидаю страницу всех резюме и кнопки (до 30 секунд)...";
            MainTimer->Enabled = true;
        } else {
            StatusBar->SimpleText = "Статус: страница всех резюме или её кнопки не найдены за 30 секунд. Ожидаем...";
            MainTimer->Enabled = false;
            Step = 1;
            ClearMemory();
            LongTimer->Enabled = true;
        }
        return;
    }
    if (Step == 4) {
        if (result < 0) {
            MainTimer->Enabled = false;
            StatusBar->SimpleText = "Статус: необработанных доступных кнопок больше нет. Проверьте результат на сайте.";
            Step = 1;
            LongTimer->Enabled = true;
            return;
        }
        if (result > 0 && ResumesLeft > 0) --ResumesLeft;
        StatusBar->SimpleText = "Статус: отправляю нажатия (осталось " + IntToStr(ResumesLeft) + ").";
        if (ResumesLeft <= 0) {
            MainTimer->Enabled = false;
            Step = 5;
        } else MainTimer->Enabled = true;
    }

	if (Step == 5) {
		LastTime = Now().FormatString("hh:nn");
		StatusBar->SimpleText = "Статус: нажатия отправлены в " + LastTime + ". Ожидаем...";
		Step = 1;
		ClearMemory();
		LongTimer->Enabled = true;
		return;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::N1Click(TObject *Sender)
{
	SettingsForm->Position = poMainFormCenter;
	SettingsForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::N2Click(TObject *Sender)
{
	if (MainForm->Height == 685) {
		HideWeb();
		N2->Caption = "Показать браузер";
	} else {
		ShowWeb();
		N2->Caption = "Скрыть браузер";
	}
}
//---------------------------------------------------------------------------

