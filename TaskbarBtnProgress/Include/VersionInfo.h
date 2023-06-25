/// Copyright (c) 2023 namarium
/// Licensed under the MIT License.
/// https://opensource.org/licenses/MIT

#ifndef VERSIONINFO_H
#define VERSIONINFO_H

/// <summary>
/// バージョン文字列
/// </summary>
#define VERSIONINFO_DEFVER_STRING "0.0.63"

/// <summary>
/// バージョン番号
/// </summary>
namespace VersionInfo
{
	/// <summary>
	/// バージョン番号（Enum）
	/// </summary>
	enum VersionEnum : int
	{
		/// <summary>
		/// メジャー
		/// </summary>
		Major = 0,

		/// <summary>
		/// マイナー
		/// </summary>
		Minor = 0,

		/// <summary>
		/// ビルドカウント
		/// </summary>
		Build = 63
	};

	/// <summary>
	/// バージョン番号（std::tuple）
	/// </summary>
	constexpr auto VersionTuple = std::tuple{ VersionEnum::Major, VersionEnum::Minor, VersionEnum::Build };
}

#endif
