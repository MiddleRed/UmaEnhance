## UmaEnhance
第三方赛马娘插件，集成多个实用功能，可以用来自定义赛马娘游戏内容和提升游戏体验
## 开始使用
**只支持从 Windows 平台 DMM 端下载的赛马娘**  
从 Releases 中下载最新版本的 `version.dll` 文件，并将其放置在游戏 exe 目录底下。  
如果你想改变插件的一些设置，在相同目录下创建一个 `uconfig.json` 文件。你可以复制粘贴下面给出的内容示范到此文件中：  
```
{
    "fps":-1,

    "inspectMsgPack":true,
    "saveRequestPack":false,
    "saveResponsePack":false,
    
    "forceClosingGame":true
}
```
你也可以手动添加一些其它的设置于此文件中。
更多插件设置及具体使用方法，请查看 [Wiki](https://github.com/MiddleRed/UmaEnhance/wiki) 。
## 生成
1. 安装 [vcpkg](vcpkg.io) , [Visual Studio 2022](https://visualstudio.microsoft.com/) （或者最新版）
2. Clone 此项目
3. 使用 Visual Studio 打开，生成
## 相关项目 
部分代码来源于：  
**[umamusume-localify](https://github.com/GEEKiDoS/umamusume-localify)**   
[EXNOA-CarrotJuicer](https://github.com/CNA-Bld/EXNOA-CarrotJuicer)    

## 特别感谢
[@GEEKiDoS](https://github.com/GEEKiDoS)  
[@CNA-Bld](https://github.com/CNA-Bld)  
[@akemimadoka](https://github.com/akemimadoka)  
[@chinosk114514](https://github.com/chinosk114514)  
## 免责声明
此项目内的所有资源和内容**仅供学习交流**使用。此项目里的相关工具可能会违反Cygames官方用户许可协议，一旦使用此项目中的任何工具，我们默认你已经清楚使用这些工具可能会造成的后果，我们不对因为使用本项目的任何工具而造成的**任何损失**负责。