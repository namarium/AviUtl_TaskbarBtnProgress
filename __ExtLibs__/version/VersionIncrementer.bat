@if(0==0) /*
@cscript /nologo /E:JScript "%~f0" %*
@exit /b %errorlevel%&*/@end
WSH.Quit(function ()
{
	// 引数チェック
	if (WSH.Arguments.length == 0) {
		// 実行引数（保存先パス）が指定されていない。
		return -1;
	}
	try{
		var dbs = WSH.CreateObject("ADODB.Stream");
		// ヘッダー読み込み
		var headerPath = WSH.Arguments(0);
		dbs.Type = 2;
		dbs.charset = "utf-8";
		dbs.Open();
		dbs.LoadFromFile(headerPath);
		var code = dbs.ReadText(-1);
		dbs.Close();

		// ビルド番号を加算する
		if (/"(\d+)\.(\d+)\.(\d+)"/.test(code))
		{
			var major = parseInt(RegExp.$1, 10);
			var minor = parseInt(RegExp.$2, 10);
			var build = parseInt(RegExp.$3, 10)+1;

			// ビルド回数が1000回で、マイナーバージョン加算
			if (build >= 1000) {
				minor++;
				build = 1;
			}

			var verText = '"'+major+"."+minor+"."+build+'"';
			var code = code.replace(/"(\d+)\.(\d+)\.(\d+)"/, verText)
				.replace(/(Major = )(\d+)/, "$1"+major)
				.replace(/(Minor = )(\d+)/, "$1"+minor)
				.replace(/(Build = )(\d+)/, "$1"+build)
				.replace(/^\s+|\s+$/g, "");

			WSH.Echo("------------------------------");
			WSH.Echo("Version Incrementer: "+verText);
			WSH.Echo("------------------------------");
		}

		// BOM付きUTF-8で保存
		dbs.Open();
		dbs.WriteText(code, 1);
		dbs.SaveToFile(headerPath, 2);
		dbs.Close();
	}
	catch(e){
		WSH.Echo(e.name + ":" + e.message);
		return -1;
	}
	// 正常終了
	return 0;
}());
