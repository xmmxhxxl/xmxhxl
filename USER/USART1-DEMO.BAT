SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;D:\软件位置\虚拟机\bin\;E:\新建文;夹 (2)\python-3.8.2\Scripts\;;新;; (2)\python-3.8.2\;Program Files\Microsoft MPI\Bin\;D:\ide\app\oracle\product\10.2.0\server\bin;C:\ProgramData\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files (x86)\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files\Microsoft SQL Server\100\DTS\Binn\;d:\Java\jdk1.8.0_144\bin;d:\Java\jdk1.8.0_144\jre\bin;"E:\python练习\PyCharm Community Edition 2020.2.2;";C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Python 3.8;D:\python-3.7.0（64-32位）\python;D:;pip;D:\conda;D:\Program Files\PuTTY\;d:\Program Files (x86)\WinSCP\;D:\study\MySQL\mysql-5.7.27-winx64\mysql-5.7.27-winx64\bin;D:\Program Files\Git\mingw64\bin;D:\Program Files\Git\usr\bin;d:\Program Files\Git\cmd;C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility;C:\Users\admin\AppData\Local\Programs\Python\Python36\Scripts\;C:\Users\admin\AppData\Local\Programs\Python\Python36\;D:\Program Files\Python\Scripts\;D:\Program Files\Python\;D:\Programs\Python\Python37\Scripts\;D:\Programs\Python\Python37\;D:\python_apk\Scripts\;D:\python_apk\;D:\python-3.7.0（64-32位）\python\Scripts\;D:\python-3.7.0（64-32位）\python\;C:\Users\admin\AppData\Local\Microsoft\WindowsApps;C:\Program Files\Bandizip\;D:\新建文件夹 (2)\IntelliJ IDEA 2019.3.3\bin;;D:\pycharm_apk\PyCharm Community Edition 2020.2.1\bin;;C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Python 3.8;;d:\Program Files\JetBrains\WebStorm 2020.1\bin;;d:\Programs\Microsoft VS Code\bin;d:\Program Files\JetBrains\PyCharm 2019.3.3\bin;
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=USART1-DEMO
SET CPU_CLOCK=0x007A1200
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\startup_stm32f10x_hd._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_it.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\usart1.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_rcc.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_usart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\core_cm3.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\system_stm32f10x.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\STM32-DEMO.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\STM32-DEMO.axf" --i32combined --output ".\STM32-DEMO.hex"
