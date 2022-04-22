## UmaEnhance
English | [中文](README_zh.md)  
  
A third-party plugin with useful functions to customize & better Uma Musume (ウマ娘) gaming experience. 
## Usage
**Only support game downloaded from Windows DMM Game Player.**  
Download the `version.dll` file from Releases, and put this file under the game exe folder.  
If you want to change the plugin settings, create `uconfig.json` file in the same folder. You can copy the example following to the file:  
```
{
    "fps":-1,

    "inspectMsgPack":true,
    "saveRequestPack":false,
    "saveResponsePack":false,
    
    "forceClosingGame":true
}
```
You can also manually add other settings to it.  
Check [Wiki](https://github.com/MiddleRed/UmaEnhance/wiki) to get more detailed information and usage about the plugin.
## Build
1. Install [vcpkg](https://vcpkg.io) , [Visual Studio 2022](https://visualstudio.microsoft.com/) (or latest)
2. Clone the repository
3. Build by Visual Studio
## Related repositories   
Part of the code is from:  
**[umamusume-localify](https://github.com/GEEKiDoS/umamusume-localify)**   
[EXNOA-CarrotJuicer](https://github.com/CNA-Bld/EXNOA-CarrotJuicer)    

## Special Thanks
[@GEEKiDoS](https://github.com/GEEKiDoS)  
[@CNA-Bld](https://github.com/CNA-Bld)  
[@akemimadoka](https://github.com/akemimadoka)  
[@chinosk114514](https://github.com/chinosk114514)  

## Disclaim
All resources and contents in this repository are for learning **ONLY**. The code and tools in this repository may violate Cygames official EULA, and we assumed that you have clearly understood the potential consequences once you use these tools. We are not responsible for **ANY** damage or trouble caused by the use of the tools in this repository.
