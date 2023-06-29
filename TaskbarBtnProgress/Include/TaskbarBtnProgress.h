/// Copyright (c) 2023 namarium
/// Licensed under the MIT License.
/// https://opensource.org/licenses/MIT

#ifndef TASKBARBTNPROGRESS_H
#define TASKBARBTNPROGRESS_H

#include "aviutl/AviUtl.h"
#define TASKBARBTNPROGRESS_API __declspec(dllexport)

extern "C"
{
	/// <summary>
	/// フィルターテーブル取得関数
	/// </summary>
	/// <returns>
	/// フィルター構造体の参照
	/// </returns>
	TASKBARBTNPROGRESS_API AviUtl::Filter::FilterPluginTable& __stdcall GetFilterTable();

	/// <summary>
	/// フィルターテーブル取得関数（YUY2版）
	/// </summary>
	/// <returns>
	/// フィルター構造体の参照
	/// </returns>
	TASKBARBTNPROGRESS_API AviUtl::Filter::FilterPluginTable& __stdcall GetFilterTableYUY2();
}

#endif
