SET PATH=C:\Keil4ARM\ARM\BIN40;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\ProgramData\Oracle\Java\javapath;C:\Program Files (x86)\Intel\iCLS Client\;C:\Program Files\Intel\iCLS Client\;C:\Windows\System32;C:\Windows;C:\Windows\System32\wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files\TortoiseSVN\bin;C:\WINDOWS\system32\config\systemprofile\.dnx\bin;C:\Program Files\Microsoft DNX\Dnvm\;C:\Program Files\Microsoft SQL Server\130\Tools\Binn\;C:\Program Files (x86)\nodejs\;D:\AD\System;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin;C:\Program Files (x86)\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files\Microsoft SQL Server\100\DTS\Binn\;C:\Program Files (x86)\Microsoft SQL Server\100\Tools\Binn\VSShell\Common7\IDE\;C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\PrivateAssemblies\;C:\Program Files (x86)\Microsoft SQL Server\100\DTS\Binn\;C:\Program Files (x86)\Xoreax\IncrediBuild;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;C:\Program Files (x86)\ATMEL Corporation\AT91-ISP v1.13\Library\;C:\Program Files (x86)\ATMEL Corporation\AT91-ISP v1.13\sam-ba 2.9;C:\Users\white\AppData\Local\Microsoft\WindowsApps;C:\Users\white\AppData\Roaming\npm;
SET ARMCC41INC=C:\Keil4ARM\ARM\RV31\Inc
SET ARMCC41LIB=C:\Keil4ARM\ARM\RV31\Lib
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=HAIKANG
SET CPU_CLOCK=0x007A1200
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal_rcc.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal_gpio.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal_tim.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal_rcc_ex.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal_flash.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal_dma.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal_cortex.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal_iwdg.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal_flash_ex.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal_gpio_ex.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal_uart.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal_pwr.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal_tim_ex.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_hal_msp.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\stm32f1xx_it.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\main.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\weigand.__i"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\serial.__i"
"C:\Keil4ARM\ARM\BIN40\ArmAsm" --Via ".\output\startup_stm32f103xb._ia"
"C:\Keil4ARM\ARM\BIN40\ArmCC" --Via ".\output\system_stm32f1xx.__i"
"C:\Keil4ARM\ARM\BIN40\ArmLink" --Via ".\output\WGCable_KH.lnp"
C:\Keil4ARM\ARM\BIN40\fromelf.exe ".\output\WGCable_KH.axf" --i32combined --output ".\output\WGCable_KH.hex"
