
<#
.Synopsis
    Ej3
.DESCRIPTION
    Monitorea cambios de un determinado directorio en donde se encuentra la ultima version del codigo fuente, ejecutando una serie de acciones cada vez que se detecte un cambio.
.PARAMETER -codigo
Ruta del directorio a monitorear.[OBLIGATORIO]
.PARAMETER -acciones
Lista de acciones separadas con coma a ejecutar cada vez que haya un cambio en el directorio a monitorear.
    Las acciones pueden ser:
    "listar": muestra por pantalla los nombres de los archivos que sufireron cambios (archivos creados, modificados, renombrados, borrrados)
    "peso": muestra por pantalla el peso de los archivos que sufrieron cambios.
    "compilar": compila los archivos dentro de "codigo"
.PARAMETER -salida
    Ruta del directorio utilizado por la accion "publicar"
.EXAMBLE
 ./pwdaemon.ps1 -codigo repo -acciones listar,peso,compilar,publicar -salida dirPublicacion
#>

Param(
    [Parameter(Mandatory=$true)]
    [string] $codigo,

    [Parameter(Mandatory=$true)]
    [string] $acciones,
    
    [Parameter]
    [string] $salida
)
<#Validate actions#>
$actions = $acciones.Split(',')
$countActions = 0
foreach($act in $actions) {
    switch ($act) {
        'listar' {}
        'peso' {}
        'compilar' {}
        'publicar' {}
        default {
            Write-Host "Accion no valida $_"
            exit
        }
    }
}

function Compile() {
    Param(
        [string] $rootPath
    )
    Write-Host "Se compilan los archivos de $rootPath en ./bin/output"    
    Get-ChildItem -Path $rootPath -Recurse | Get-Content | Out-File ( New-Item -Path "./bin/output" -Force )
}

function Publish() {
    Param(
        [string] $output
    )
    if(!(Test-Path -Path $output)) {
        New-Item $output -Type Directory
    }
    Write-Host "Se publican los archivos de ./bin/output en $output"   
    Copy-Iterm -Path ./bin/output -Destination $output
}

    $daemon = New-Object -TypeName System.IO.FileSystemWatcher -Property @{
        Path = $codigo
        IncludeSubdirectories = $true
    }

    $actionHandler = {
        <#$actions = $acciones.Split(',')#>
        $details = $event.SourceEventArgs
        $Name = $details.Name
        $FullPath = $details.FullPath
        $Path = $details.Path
        $Size = (Get-Item $FullPath).length
        $OldName = $details.OldName
    
        if ($actions.Contains('listar')) {
            Write-Host "File: $Name"
        }
    
        if ($actions.Contains('peso')) {
            Write-Host "Size: $Size"
        }   
    
        if ($actions.Contains('compilar')) {
            Compile $codigo
            if($actions.Contains("publicar")) {
                Publish $pushPath
            }
        }
    
    }

    $handlers = . {
        Register-ObjectEvent -InputObject $daemon -EventName Created -Action $actionHandler
        Register-ObjectEvent -InputObject $daemon -EventName Changed -Action $actionHandler
        Register-ObjectEvent -InputObject $daemon -EventName Deleted -Action $actionHandler
        Register-ObjectEvent -InputObject $daemon -EventName Renamed -Action $actionHandler
    }

    $daemon.EnableRaisingEvents = $true
    Write-Host "Daemon esta vivo! Escuchando $codigo"

Start-Process pwsh
while($true) {
    Wait-Event -Timeout 1
}

