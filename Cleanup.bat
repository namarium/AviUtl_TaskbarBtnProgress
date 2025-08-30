@pushd %~dp0

@set "list=vs2019 vs2022"
@for %%v in (%list%) do (

@rem Solution
rd /s /q .\Solution\%%v\.vs
rd /s /q .\Solution\%%v\Debug
rd /s /q .\Solution\%%v\Release

@rem TaskbarBtnProgress
rd /s /q .\TaskbarBtnProgress\Project\%%v\Debug
rd /s /q .\TaskbarBtnProgress\Projects\%%v\Release

@rem __ExtraLibs__

)

@popd
