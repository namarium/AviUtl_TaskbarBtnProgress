<!-- :
@setlocal&pushd %~dp0
@set solution_path=.\Solution\vs2019\TaskbarBtnProgress.sln
@cscript //nologo "%~f0?.wsf" %*||pause
@popd&endlocal&exit /b %errorlevel%
-->
<job>
<object id="shell" progid="WScript.Shell"/>
<object id="fso" progid="Scripting.FileSystemObject"/>
<script language="JScript">
WSH.Quit(function()
{
	try{
		var procEnv = shell.Environment("Process");
		var slnPath = fso.GetAbsolutePathName(procEnv("solution_path"));
		shell.Run('%comspec% /c start "" ' + slnPath, 0);
	}
	catch(e){
		WSH.Echo(e.name + ":" + e.message);
		return -1;
	}
	return 0;
}());
</script>
</job>