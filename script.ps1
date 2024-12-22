$instances = Read-Host "Enter the number of load"

$command = Read-Host "Enter the command to execute (e.g., cpu-load, disk-load, cpudisk-load)"

# Path to the executable
$executablePath = "C:\Users\367960\Osi\ConsoleApp\x64\Release\ConsoleApp.exe"

Write-Host "Executable Path: $executablePath"
Write-Host "Command: $command"
Write-Host "Instances: $instances"

for ($i = 1; $i -le $instances; $i++) {
    Start-Process -FilePath $executablePath -ArgumentList $command -NoNewWindow
}
