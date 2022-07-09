SET PATH=D:\Keil_v5\ARM\ARMCC\Bin;D:\CIFLog\sdk;d:\jre7\bin\client;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;D:\Anaconda;D:\Anaconda\Library\mingw-w64\bin;D:\Anaconda\Library\usr\bin;D:\Anaconda\Library\bin;D:\Anaconda\Scripts;C:\Program Files\Common Files\Oracle\Java\javapath;C:\Program Files\Common Files\Siemens\Automation\Simatic OAM\bin;C:\Program Files (x86)\Common Files\Intel\Shared Libraries\redist\intel64_win\compiler;C:\Program Files (x86)\Common Files\Intel\Shared Libraries\redist\intel64\compiler;D:\Soft\Vmware\bin\;D:\Soft\XShell\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files\NVIDIA Corporation\NVIDIA NvDLISR;D:\Soft\Matlab\runtime\win64;D:\Soft\Matlab\bin;C:\Program Files\PuTTY\;C:\Program Files\Microsoft SQL Server\Client SDK\ODBC\110\Tools\Binn\;C:\Program Files (x86)\Microsoft SQL Server\120\Tools\Binn\;C:\Program Files\Microsoft SQL Server\120\Tools\Binn\;C:\Program Files\Microsoft SQL Server\120\DTS\Binn\;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;C:\Program Files (x86)\Microsoft SQL Server\Client SDK\ODBC\110\Tools\Binn\;C:\Program Files (x86)\Microsoft SQL Server\120\DTS\Binn\;C:\Program Files (x86)\Common Files\Siemens\;C:\Program Files (x86)\Common Files\Siemens\Interfaces;C:\Program Files (x86)\Common Files\Siemens\ACE\Interfaces;D:\Git\cmd;D:\Git\bin;D:\Graphviz\bin;C:\Users\93724\AppData\Local\Microsoft\WindowsApps;D:\Soft\Microsoft VS Code\bin;D:\PyCharm Community Edition 2021.3.1\bin;;D:\ntelliJ IDEA Community Edition 2021.3.1\bin;;C:\Users\93724\AppData\Local\GitHubDesktop\bin;D:\Graphviz\bin;
SET CPU_TYPE=STM32F407ZG
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=SPI
SET CPU_CLOCK=0x007A1200
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\main.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_it.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\system_stm32f4xx.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\led.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\key.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\spi.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\mt6825.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\delay.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\sys.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\usart.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via "..\obj\startup_stm32f40_41xxx._ia"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\misc.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_gpio.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_fsmc.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_rcc.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_spi.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_syscfg.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\obj\stm32f4xx_usart.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via "..\OBJ\SPI.lnp"
"D:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" "..\OBJ\SPI.axf" --i32combined --output "..\OBJ\SPI.hex"
