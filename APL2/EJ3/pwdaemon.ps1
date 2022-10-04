
<#
.SYNOPSIS
    Ej3 Daemon
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
.EXAMPLE
 ./pwdaemon.ps1 -codigo repo -acciones listar,peso,compilar,publicar -salida dirPublicacion
#>

Param(
    [Parameter(Mandatory=$true)]
    [string] $codigo,

    [Parameter(Mandatory=$true)]
    [string[]] $acciones,
    
    [string] $salida
)

function Compile() {
    Param(
        [string] $rootPath
    )
    Write-Host "Se compilan los archivos de $rootPath en ./bin/output"    
    Get-ChildItem -Path $rootPath -Recurse | Get-Content | Out-File ( New-Item -Path "./bin/output" -Force )
    return
}

function Publish() {
    Param(
        [string] $output
    )
    if(!(Test-Path -Path $output)) {
        New-Item $output -Type Directory
    }
    Write-Host "Se publican los archivos de ./bin/output en $output"   
    Copy-Item -Path ./bin/output -Destination $output
    return
}


<#Validate actions#>
foreach($act in $acciones) {
    switch ($act) {
        'listar' {}
        'peso' {}
        'compilar' {
            Get-ChildItem -Path $codigo -Recurse | ForEach-Object {
                    $perm = ((ls -l $_.FullName) -Split " ")[0].Substring(1,1)
                    if(!($perm -eq 'r')) {
                        Write-Host "checkear los permisos de lectura de $_ en el directorio a monitorear"
                        exit
                    }
            }
            Compile $codigo
        
            if($acciones.Contains('publicar')) {
                if(!($PSBoundParameters.ContainsKey('salida'))) {
                    Write-Host "Para la accion publicar es necesario el parametro -salida"
                    exit
                }
                Publish $salida
            }
        }
        'publicar' {
            if(!($acciones.Contains('compilar'))) {
                Write-Host "Se necesita de la accion compilar para poder publicar"
                exit
            }
        }
        default {
            Write-Host "Accion no valida $_"
            exit
        }
    }
}

    $daemon = New-Object -TypeName System.IO.FileSystemWatcher -Property @{
        Path = $codigo
        IncludeSubdirectories = $true
    }

    $actionHandler = {
        $details = $event.SourceEventArgs
        $Name = $details.Name
        $Size = (Get-Item $FullPath).length
    
        if ($acciones.Contains('listar')) {
            Write-Host "File: $Name"
        }
    
        if ($acciones.Contains('peso')) {
            Write-Host "Size: $Size"
        }   
    
        if ($acciones.Contains('compilar')) {
            Compile $codigo
            if($acciones.Contains('publicar')) {
                Publish $salida
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

