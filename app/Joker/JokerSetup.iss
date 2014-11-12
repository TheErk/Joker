; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Joker"
#define MyAppVersion "1.1.5"
#define MyAppPublisher "Phonations"
#define MyAppURL "http://www.phonations.com/"
#define MyAppExeName "Joker.exe"

#define MyAppSrc "release\" + MyAppExeName
#if !FileExists(MyAppSrc)
#error "Unable to find MyAppSrc"
#endif

#define FileVerStr GetFileVersion(MyAppSrc)
#define StripBuild(str VerStr) Copy(VerStr, 1, RPos(".", VerStr)-1)
#define MyAppVerStr StripBuild(FileVerStr)
#define MyAppVerName MyAppName + " v" + MyAppVerStr

#define MyOutputDir GetEnv('JOKER_RELEASE_PATH')
#define QtPath GetEnv('QTDIR')
#define PortaudioPath GetEnv('PORTAUDIO_PATH')

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{E892D5E9-1FA6-4BFC-A749-3C942E0A5C56}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableDirPage=yes
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
LicenseFile="LICENSE.TXT"
OutputDir={#MyOutputDir}
OutputBaseFilename={#MyAppName}Setup_v{#MyAppVerStr}
;OutputBaseFilename={#MyAppName}Setup.exe
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: {#QtPath}\bin\Qt5Core.dll; DestDir: {app}; Flags: ignoreversion
Source: {#QtPath}\bin\Qt5Gui.dll; DestDir: "{app}"; Flags: ignoreversion
Source: {#QtPath}\bin\Qt5OpenGL.dll; DestDir: "{app}"; Flags: ignoreversion
Source: {#QtPath}\bin\Qt5Network.dll; DestDir: "{app}"; Flags: ignoreversion
Source: {#QtPath}\bin\Qt5SerialPort.dll; DestDir: "{app}"; Flags: ignoreversion
Source: {#QtPath}\bin\Qt5Test.dll; DestDir: "{app}"; Flags: ignoreversion
Source: {#QtPath}\bin\Qt5Widgets.dll; DestDir: "{app}"; Flags: ignoreversion
Source: {#QtPath}\bin\Qt5Xml.dll; DestDir: "{app}"; Flags: ignoreversion
Source: {#QtPath}\bin\libgcc_s_dw2-1.dll; DestDir: "{app}"; Flags: ignoreversion
Source: {#QtPath}\bin\libstdc++-6.dll; DestDir: "{app}"; Flags: ignoreversion
Source: {#QtPath}\bin\icuin51.dll; DestDir: "{app}"; Flags: ignoreversion
Source: {#QtPath}\bin\icuuc51.dll; DestDir: "{app}"; Flags: ignoreversion
Source: {#QtPath}\bin\libwinpthread-1.dll; DestDir: "{app}"; Flags: ignoreversion
Source: {#QtPath}\bin\icudt51.dll; DestDir: "{app}"; Flags: ignoreversion
Source: {#QtPath}\plugins\platforms\*; DestDir: "{app}\platforms"; Flags: ignoreversion

Source: "release\Joker.exe"; DestDir: "{app}"; Flags: ignoreversion
; dlls (PortAudio, SDL, etc.)
Source: "release\*.dll"; DestDir: "{app}"; Flags: ignoreversion
; Qt dlls (windeployqt needs to be run first)
Source: "release\accessible\*.dll"; DestDir: "{app}\accessible"; Flags: ignoreversion
Source: "release\bearer\*.dll"; DestDir: "{app}\bearer"; Flags: ignoreversion
Source: "release\iconengines\*.dll"; DestDir: "{app}\iconengines"; Flags: ignoreversion
Source: "release\imageformats\*.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "release\platforms\*.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
; translations
Source: "release\*.qm"; DestDir: "{app}"; Flags: ignoreversion
; images
Source: "release\*.png"; DestDir: "{app}"; Flags: ignoreversion
; fonts
Source: "release\*.ttf"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
