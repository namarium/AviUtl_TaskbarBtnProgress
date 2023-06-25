@pushd %~dp0

@rem Solution
rd /s /q .\Solution\vs2019\.vs
rd /s /q .\Solution\vs2019\Debug
rd /s /q .\Solution\vs2019\Release

@rem TaskbarBtnProgress
rd /s /q .\TaskbarBtnProgress\Project\vs2019\Debug
rd /s /q .\TaskbarBtnProgress\Projects\vs2019\Release

@rem __ExtraLibs__


@popd
