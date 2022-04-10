## UmaEnhance
English | [中文](README_zh.md)  
  
A third-party plugin with useful functions to customize & better Uma Musume (ウマ娘) gaming experience. 
## Usage
**Only support game downloaded from Windows DMM Game Player.**  
Download the `version.dll` file from release, and put this file under the game exe folder.  
If you want to change the plugin settings, create `uconfig.json` file in the same folder. You can copy the example following to the file:  
```
{
    "fps":-1,

    "saveRequestPack":false,
    "saveResponsePack":false,
    
    "forceClosingGame":false
}
```
You can also manually add other settings to it.  
### Available settings
#### `fps`
Change the game fps. Default `-1` (Unlimit)  
#### `saveRequestPack`
Save the MsgPack sent by cilent. Default `false`  
#### `saveResponsePack`
Save the MsgPack from server. Default `false`  
#### `forceClosingGame`
Force close the game to avoid unexpected memory error occurring when closing game. Default `false`
#### New features are in progress.
## Build
1. Install [vcpkg](vcpkg.io) , [Visual Studio 2022](https://visualstudio.microsoft.com/) (or latest)
2. Clone the repository
3. Build by Visual Studio
## Related repositories   
Part of the code is from:  
**[umamusume-localify](https://github.com/GEEKiDoS/umamusume-localify)**   
[EXNOA-CarrotJuicer](https://github.com/CNA-Bld/EXNOA-CarrotJuicer)    

## Special Thanks
[@GEEKiDoS](https://github.com/GEEKiDoS)  
[@akemimadoka](https://github.com/akemimadoka)  
[@chinosk114514](https://github.com/chinosk114514)  
[@CNA-Bld](https://github.com/CNA-Bld)  

## Disclaim
All resources and contents in this repository are for learning **ONLY**. The code and tools in this repository may violate Cygames official EULA, and we assumed that you have clearly understood the potential consequences once you use these tools. We are not responsible for **ANY** damage or trouble caused by the use of the tools in this repository.
