/// Copyright (c) 2023 namarium
/// Licensed under the MIT License.
/// https://opensource.org/licenses/MIT

#include "TaskbarBtnProgress.h"
#include "VersionInfo.h"
#include <shobjidl.h>	// ITaskbarList3

using namespace AviUtl::Filter;

/// <summary>
/// プラグイン名
/// </summary>
#define AUF_PLUGINNAME "TaskbarBtnProgress"

/// <summary>
/// プラグインの説明
/// </summary>
#define AUF_PLUGININFOTEXT "TaskbarBtn Progress Indicator"

namespace
{
	/// <summary>
	/// 保存中かどうか
	/// </summary>
	bool g_isSaving = false;

	/// <summary>
	/// タスクバーボタン作成メッセージ
	/// </summary>
	UINT g_uTBBC = 0;

	/// <summary>
	/// メインウィンドウのハンドル
	/// </summary>
	HWND g_hMainWnd = nullptr;

	/// <summary>
	/// タスクバーボタン
	/// </summary>
	ITaskbarList3* g_pTaskbarList = nullptr;

	/// <summary>
	/// メッセージ監視クラス
	/// </summary>
	ref class MsgMonitor sealed : public System::Windows::Forms::NativeWindow
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="hWnd">ウィンドウハンドル</param>
		inline explicit MsgMonitor(HWND hWnd)
		{
			AssignHandle(System::IntPtr(hWnd));
		}
	protected:
		/// <summary>
		/// ウィンドウプロシージャ
		/// </summary>
		/// <param name="m">メッセージ</param>
		[System::Security::Permissions::SecurityPermission(System::Security::Permissions::SecurityAction::Demand
			, Flags = System::Security::Permissions::SecurityPermissionFlag::UnmanagedCode)]
		inline void WndProc(System::Windows::Forms::Message% m) override
		{
			if (m.Msg == g_uTBBC)
			{	// タスクバーボタン作成
				auto hr = ::CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pTaskbarList));
				if (SUCCEEDED(hr)) {
					hr = g_pTaskbarList->HrInit();
					if (FAILED(hr)) {
						g_pTaskbarList->Release();
						g_pTaskbarList = nullptr;
					}
				}
			}
			else if (g_pTaskbarList && g_isSaving && m.Msg == WM_SETTEXT)
			{	// 最も後ろにある "[数字A/数字B]" の形式のテキストを見つける
				std::wstring txt = reinterpret_cast<wchar_t*>(m.LParam.ToPointer());
				auto startPos = txt.rfind('[');
				auto endPos = txt.rfind(']');
				if (startPos != std::wstring::npos && endPos != std::wstring::npos && startPos < endPos) {
					auto slashPos = txt.rfind('/', endPos);
					if (slashPos != std::wstring::npos && slashPos > startPos) {
						auto completed = std::stoul(txt.substr(startPos + 1, slashPos - startPos - 1));
						auto total = std::stoul(txt.substr(slashPos + 1, endPos - slashPos - 1));
						g_pTaskbarList->SetProgressValue(g_hMainWnd, completed, total);
					}
				}
			}
			__super::WndProc(m);
		}
	};

	/// <summary>
	/// メッセージ監視用
	/// </summary>
	msclr::auto_gcroot<MsgMonitor^> g_msgMonitor = nullptr;
}

/// <summary>
/// プラグインの初期化
/// </summary>
/// <param name="pFilter">フィルタテーブル構造体のアドレス</param>
/// <returns>
/// 1 なら成功
/// </returns>
int FilterInit(FilterPluginTable* pFilter)
{
	g_hMainWnd = ::GetWindow(static_cast<HWND>(pFilter->hWnd), GW_OWNER);
	if (g_hMainWnd) {
		g_msgMonitor = gcnew MsgMonitor(g_hMainWnd);
		g_uTBBC = ::RegisterWindowMessage(L"TaskbarButtonCreated");
		::ChangeWindowMessageFilterEx(g_hMainWnd, g_uTBBC, MSGFLT_ALLOW, nullptr);
	}
	return TRUE;
}

/// <summary>
/// プラグインの終了処理
/// </summary>
/// <param name="pFilter">フィルタテーブル構造体のアドレス</param>
/// <returns>
/// 1 なら成功
/// </returns>
int FilterExit(FilterPluginTable* pFilter)
{
	
	if (g_msgMonitor.get()) {
		g_msgMonitor->ReleaseHandle();
		g_msgMonitor = nullptr;
	}
	if (g_pTaskbarList) {
		g_pTaskbarList->Release();
		g_pTaskbarList = nullptr;
	}
	return TRUE;
}

/// <summary>
/// フィルターのウィンドウプロシージャ
/// </summary>
int FilterWndProc(void* hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam, void* pEdit, FilterPluginTable* pFilter)
{
	using WndMsg = FilterPluginTable::WindowMessage;
	if (msg == WM_COMMAND && static_cast<WndMsg>(LOWORD(wParam)) == WndMsg::FilterButton)
	{	// ボタン押下時
		std::wstring msgTxt = L"Window：";
		msgTxt += g_hMainWnd ? L"OK" : L"Error";
		msgTxt += L"\nTaskbarBtn：";
		msgTxt += g_pTaskbarList ? L"OK" : L"Warning";
		::MessageBox(static_cast<HWND>(hWnd), msgTxt.c_str(), L"Information", MB_ICONINFORMATION);
	}
	return 0;
}

/// <summary>
/// セーブが開始される直前に呼ばれる関数へのポインタ (NULLなら呼ばれません)
/// </summary>
/// <param name="pFilter">フィルタテーブル構造体のアドレス</param>
/// <param name="startFrame">セーブする先頭フレーム</param>
/// <param name="endFrame">セーブする最終フレーム</param>
/// <param name="pEdit">エディットハンドル</param>
/// <returns>TRUE なら成功</returns>
int SaveStart(FilterPluginTable* pFilter, unsigned int startFrame, unsigned int endFrame, void* pEdit)
{
	g_isSaving = true;
	if (g_pTaskbarList) {
		g_pTaskbarList->SetProgressState(g_hMainWnd, TBPF_NORMAL);
	}
	return TRUE;
}

/// <summary>
/// セーブが終了した直前に呼ばれる関数へのポインタ (NULLなら呼ばれません)
/// </summary>
/// <param name="pFilter">フィルタテーブル構造体のアドレス</param>
/// <param name="pEdit">エディットハンドル</param>
/// <returns>TRUE なら成功</returns>
int SaveEnd(FilterPluginTable* pFilter, void* pEdit)
{
	g_isSaving = false;
	if (g_pTaskbarList) {
		g_pTaskbarList->SetProgressState(g_hMainWnd, TBPF_NOPROGRESS);
	}
	return TRUE;
}

/// <summary>
/// フィルターテーブル取得関数
/// </summary>
/// <returns>
/// フィルター構造体の参照
/// </returns>
TASKBARBTNPROGRESS_API FilterPluginTable& __stdcall GetFilterTable()
{
	static FilterPluginTable filter = {
		FilterPluginTable::FilterFlag::AlwaysActive
		| FilterPluginTable::FilterFlag::ExInformation
	};

	// プラグイン情報
	filter.pName = AUF_PLUGINNAME;
	filter.pInfoText = AUF_PLUGININFOTEXT "｜" VERSIONINFO_DEFVER_STRING;

	// コールバック
	filter.FilterInit = FilterInit;
	filter.FilterExit = FilterExit;
	filter.WndProc = FilterWndProc;
	filter.SaveStart = SaveStart;
	filter.SaveEnd = SaveEnd;

	// コントロール類
	static std::array<char*, 1> labelList{ "Check" };
	static std::array<int, 1> defaultValueList{ -1 };
	filter.Checkbox_Num = labelList.size();
	filter.pCheckbox_NameList = labelList.data();
	filter.pCheckbox_Default = defaultValueList.data();

	return filter;
}

/// <summary>
/// フィルターテーブル取得関数（YUY2版）
/// </summary>
/// <returns>
/// フィルター構造体の参照
/// </returns>
TASKBARBTNPROGRESS_API FilterPluginTable& __stdcall GetFilterTableYUY2()
{
	return GetFilterTable();
}
