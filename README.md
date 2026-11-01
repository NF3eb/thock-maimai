# Thock maimai

Windows 原生键盘音效与全屏烟花应用，使用 Win32、Direct3D 11X 和 Audio2。项目最初基于 [Thock 1.23.0](https://github.com/kamillobinski/thock/tree/1.23.0)，fork自 [Thock maimai v1.1.1](https://github.com/wzk112/thock-maimai) ，此fork版本仅维护 Windows 版本。

# 下载与使用

在 [Releases](https://github.com/wzk112/thock-maimai/releases) 下载 `Thock-maimai-Windows-x64.exe`，双击即可。**只需要这一个 EXE**：原烟花贴图、七种声音、动画参数与着色器都编入程序，直接从内存读取，不需要旁边的素材文件夹，不自解压，也不需要另装 Thock 或 .NET。

Windows 10 / 11，64 位。首次打开设置，关闭窗口后留在托盘，右键托盘可退出。应用未签名，Windows 可能提示来源未知。

**混合**模式下，按键声音如下：
| 按键 | 音效 |
| --- | --- |
|修饰键：Shift Ctrl Alt Win|正解音|
|Enter 小键盘Enter|绝赞大p|
|Esc Tab CapsLock NumLock Insert Menu ScrollLock PauseBreak 小键盘+-*/. |绝赞小p|
|截屏PrintScreen|烟花|
|Space Backspace Delete Pageup Pagedown End Home|带套tap|
|0-9 小键盘0-9 F1-F12 上下左右|touch|
|鼠标左键|touch|
|鼠标右键|tap|
|鼠标中键|绝赞小p|
|鼠标侧键|正解音|
|其他按键|tap|

如需更改按键->音效的关系，可更改`core.h`中的`route`和`mixedMouseSound`。
不选择**混合**模式，则所有按键使用选定的音效。

默认键盘和鼠标音量均为 10%，烟花关闭，**HDR 关闭**。音效、烟花、键盘音量、鼠标音量和显示器范围均可调整。长按不重复触发，设置窗口内的键盘输入不会播放音效。对应音量为 0 或关闭对应音效开关会停止该输入源的声音和烟花；仅关闭烟花则继续播放声音。音频设备不可用时仍可显示烟花。

每行音量旁的“同步”按钮会将当前行音量复制到另一输入源；键盘行的“同步”使鼠标音量等于键盘音量，鼠标行的“同步”使键盘音量等于鼠标音量。

默认只在鼠标所在屏幕显示烟花。烟花结束后停止绘制并释放叠加窗口，无常驻预热图层。HDR 暂不作为本版实机验收项，保持默认关闭；普通显示器使用 SDR。不改变系统亮度。

**随机位置烟花**：在 Windows 设置窗口勾选。开启后每次触发都会重新选择屏幕内的位置，中心距离四边至少为对应屏幕尺寸的 20%；同一朵烟花播放过程中位置固定，正在播放的烟花不因切换开关而跳动。关闭后恢复屏幕中央，设置会自动保存。默认关闭。

按键只在内存中用于触发，不记录输入内容、不上传。运行不需要网络。

偏好设置保存于 `%LOCALAPPDATA%/ThockMaimai/settings.json`。本fork版本与原版本有所区别：
- 原元素`enabled`更名为`keyboardenabled`，用于控制是否启用键盘按键音效
- 原元素`volume`拆分为`keyboardvolume`和`mousevolume`，均使用整数
- 新增元素`mouseEnabled`用于控制是否启用鼠标点击音效

**兼容旧配置**：使用本fork版本打开旧版本配置时，如果只有`enabled`，仍会正确读取键盘开关；如果只有`volume`，键盘和鼠标音量都会设置为该值。旧的`volume`小数会自动换算为整数（0.35->35）并分别存储。因此注意打开本fork版本后再打开原版本会使**音量达到最大**。
```json
{
  "allDisplays": false,
  "assetPath": "",
  "fireworks": false,
  "hdr": false,
  "keyboardenabled": true,
  "mouseEnabled": true,
  "randomPosition": false,
  "pack": 0,
  "keyboardvolume": 10,
  "mousevolume": 10
}
```

# 自定义素材

已经内置完整素材，不必导入。需要替换时，点击“选择素材文件夹”；“恢复内置素材”会切回 EXE 内的数据。旧自定义文件夹失效时，程序自动恢复内置素材。

仓库的 `assets/` 是 Windows 构建输入。需要一套可自由分发的演示素材时，可以运行：

```powershell
python tools/generate_demo.py private-assets/windows
```

# Windows 构建与检查

需要 Visual Studio 2022 的 C++ 桌面工具、Windows SDK、CMake 和 Python 3。

```powershell
cmake -S windows -B build -A x64
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
& 'build/Release/Thock maimai.exe' --self-test
& 'build/Release/Thock maimai.exe' --integration-test
```

构建时由 `tools/embed_resources.py` 生成 RCDATA 资源表。也可以用 `-DTHOCK_ASSET_DIR=...` 指定其他素材。无需打包器、额外 DLL 或运行时下载。

`--self-test` 在 Windows 上验证原素材解码、HLSL 编译、透明画面、HDR 数值和叠加窗口，输出 `self-test.json`、`preview-sdr.png`。`--integration-test` 在应用创建的测试窗口中使用 Windows 输入 API 验证真实消息路径、模式切换、长按去重、开关、静音、窗口清理和恢复，输出 `integration-test.json`。测试配置与结果保存在测试 EXE 旁，不更改日常偏好设置。

CI 会把 EXE 单独复制到带空格和中文的空目录，再运行上述检查。CI 无可用音频输出设备时，会明确记录 `audioDeviceAvailable: false`，不会将解码或调用请求宣称为扬声器播放验证。测试不代替真人听音和实体 HDR 亮度测量。独占全屏游戏、安全桌面等环境可能不显示普通桌面叠加窗口。

# 代码与素材许可

程序、烟花模拟、动画曲线处理、透明窗口、HDR 后处理、输入与音频逻辑、资源嵌入和打包工具均公开。Thock 原作者为 **Kamil Łobiński**，Thock maimai 原作者为[wzk112](https://github.com/wzk112)，原 MIT 许可保留；新增应用代码同样采用 MIT。Windows JSON 解析使用 [nlohmann/json 3.12.0](https://github.com/nlohmann/json)，许可位于 `windows/vendor/json.LICENSE`。

游戏素材由项目维护者确认已取得公开再分发授权后纳入仓库和 Release。**游戏图片、音频、参数及由原游戏转换的着色器不重新授予 MIT 许可**，仍保留各自原权利归属，详见素材目录的 `ASSET-LICENSE.txt`。本项目与 SEGA 无关联，不代表官方认可。HDR 为本程序新增高光增强，并非原游戏素材自带 HDR。

技术参考：[DirectComposition](https://learn.microsoft.com/en-us/windows/win32/api/dxgi1_2/nf-dxgi1_2-idxgifactory2-createswapchainforcomposition)、[Windows Raw Input](https://learn.microsoft.com/en-us/windows/win32/inputdev/using-raw-input)。
