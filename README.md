# PriMech

## Intruduction

PriMech is a small scale Game engine.
Currently in early development stage.

This is a personal Project, but feel free to fork this repo and play around with it.

## Compiling PriMech / Installation

Open the Folder where you want to clone PriMech Engine.
Create a command line on that folder and clone this Repo recursively.
```console 
$ git clone --recursive https://github.com/BatistaCo-Pedro/Primech-Engine.git
```

Change Folders to the git Folder
```console 
$ cd Primech-Engine
```

Create VS2022 Project
```console 
$ generateProjects.bat
```

Open the VS2022 SLN
```console 
$ PriMech.sln
```

##### Quick Copy
```console 
$ mkdir C:\dev && cd C:\dev
$ git clone --recursive https://github.com/BatistaCo-Pedro/Primech-Engine.git
$ cd Primech-Engine && generateProjects.bat && PriMech.sln
```

The first time the Project is run it wont work, this occurs because Sandbox doesnt know PriMechs dll at first. 
The Dll is copied into Sandbox`s binary folder as a postbuild option from premake. 
The second time the program is run the program it will work flawlessly.
This Problem will be corrected in the future. 

## Using PriMech

There exists no Documentation yet, it will be added in the future.

## System requirements

PriMech only supports Windows momentarily and the automatic project creation solely works for Visual Studio 2022. 
The Eingine will be adapted to other OSs in the future.

## Dependancies

PriMechs Dependancies are always under a vendor Folder.
OpenGl and Logging libs: [PriMech/vendor](https://github.com/BatistaCo-Pedro/Primech-Engine/tree/master/PriMech/vendor)
Premake for auto project creation: [globalvendor](https://github.com/BatistaCo-Pedro/Primech-Engine/tree/master/globalVendor)
