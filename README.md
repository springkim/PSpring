## <img src="img/mspaint.png" width="64">  PSpring
##### mspaint extension program.

Mspaint has two main problems. There is no rotation method and color memorization.
This PSpring project provides these two method extensions to mspaint.

### Example
<img src="img/example1.gif" width="720"><img src="img/example2.gif" width="720">

### Install

Run below command on Windows CMD.



<pre>
    <code>
taskkill /f /im mspaintex.exe >nul 2>&1 & powershell "[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12;(New-Object System.Net.WebClient).DownloadFile('https://github.com/springkim/PSpring/releases/download/bin/mspaintex.exe','%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\mspaintex.exe');" & pushd & cd "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup" & start mspaintex.exe & popd
    </code>
</pre>

```
taskkill /f /im mspaintex.exe >nul 2>&1 & powershell "[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12;(New-Object System.Net.WebClient).DownloadFile('https://github.com/springkim/PSpring/releases/download/bin/mspaintex.exe','%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\mspaintex.exe');" & pushd & cd "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup" & start mspaintex.exe & popd
```

