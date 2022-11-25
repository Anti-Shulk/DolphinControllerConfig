# Dolphin Controller Configurator
Dolphin Controller Configurator is a gui Qt app designed to make configuring    [Dolphin Emulator's](https://dolphin-emu.org/) controllers quicker and possible with only a controller. It makes use of Dolphin Emulator's built in controller profiles and allows selecting them in an easy to navigate way.

## Features
- Passing Command-line arguments to Dolphin 
- Keyboard or Controller navigation
- Replacing the port number of Dolphin config's

## Setup
Dolphin Controller Configurator can be downloaded under [Releases](https://github.com/Anti-Shulk/DolphinControllerConfig/releases)
Upon first launch, Dolphin Controller Configurator will generate a Settings.ini file in your user directory. 
```ini
[Setup]
setup=false

[RealControllers]
ex=Example Controller

[ProfileSuffixes]
examplesuffix=with example suffix

[Paths]
dolphinconfigpath=[Enter Dophin Config Path]
dolphinpath=[Enter Path to launch Dolphin]
```
On Windows this is located at 
`C:\Users\[Your Username]\AppData\Roaming\DolphinControllerConfig\Settings.ini`
Dolphin Controller Configurator will not lanuch into the main interface until `setup` is set to `true`. 
#### Paths
The `dolphinconfigpath` and `dolphinpath` are stored here. Usually on Windows, this is at 
`C:/Users/[Your Username]/Documents/Dolphin Emulator/Config`
These paths must use double back slashes or forward slashes. Do not add a double back slash or forward slash to the end. Examples:
`C:\\Users\\Justi\\Documents\\Dolphin Emulator\\Config`
`C:/Users/Justi/Documents/Dolphin Emulator/Config`
The `dolphinpath` is the path to the Dolphin Binary. On Windows, this should end in .exe. For example: 
`C:/Users/Justi/Documents/Dolphin-x64/Dolphin.exe`

#### Real Controllers
This setting stores all of the real controllers. More can be added by adding new lines. Here is an example:
```ini
[RealControllers]
xone=Xbox One Controller
ds4=Dualshock 4 Controller
nspro=Nintendo Switch Pro Controller
wii=Wii Remote
gc=GameCube Controller
keyboard=Keyboard
```
The key, which is on the left of the equals sign, holds the prefix for the name used for the configuration file. Configuration files are stored in the `dolphinconfigpath/Profiles/Wiimote` and `dolphinconfigpath/Profiles/GCPad` directories.
For example, there might be a profile called `nsprowiinumchuck.ini` in the `Wiimote` directory. The Real Controller (Prefix) for this file is `nspro`. 
The right side of the equals sign shows what that key will show up as in Dolphin Controller Configurator, so `xone` will show as `Xbox One Controller` in the above example.
There are 2 special Real Controllers, the "wii" and "gc". These cannot be set up with a profile and will be used as Real Controllers in Dolphin. These controllers can still be used as an emulated controller in dolphin, but only if they are set up with an alternative name.

#### Profile Suffixes
This stores the second half of the profile name. 
Here is an example:
```ini
[ProfileSuffixes]
gceast=East Layout (A is right button)
gcwest=West Layout (A is bottom button)
wiinumchuck=Wiimote and Numchuck
wiisideways=Wiimote Sideways
```
These can be used as the suffix for a profiles when combined with a real controller. For example, `nsprowiinumchuck` as a suffix of `wiinumchuck`, and will be displayed as `Nintendo Switch Pro Controller Wiimote and Numchuck` in Dolphin Controller Configurator. 

#### Profiles
As mentioned earlier, profiles are located in the dolphinconfigpath/Profiles/Wiimote and dolphinconfigpath/Profiles/GCPad directories. These can be edited manually, or using Dolphin in the Controllers Menu.
![](https://i.imgur.com/NRmxym0.png)
When creating a profile in Dolphin, type in a name, and click save to store the profile in the Wiimote directory for Wiimotes or GCPad directory for GameCube Controllers. 
**There is another additional step that must be taken for profiles to show up**
Profiles must be manually edited to say "{PortNumber}"" in the location of the port number. 
For example, a profile might say
`Device = WGInput/0/Xbox 360 Controller for Windows`
the 0 must be changed to "{PortNumber}" like so:
`Device = WGInput/{PortNumber}/Xbox 360 Controller for Windows`
This allows Dolphin Controller Configurator to change the port number in the interface. Profiles that do not have this change done will **not** show up.

## Main Interface
![](https://i.imgur.com/T2HMUY3.png)
The interface can be controlled using any Xinput Controller in Windows or any evdev/sdl controller on Linux. Many controllers do not support Xinput, and a workaround will be explained in the Tips and Tricks section.

#### Dolphin Config Window
This setting determines whether or not Dolphin Emulator's configuration window will be visible. This launches Dolphin Emulator with --batch argument, but this is only possible if a specific game file is specified using -e or --exec.
When launching Dolphin Controller Configurator with command line arguments, those arguments will be fed to Dolphin. If -b or --batch is fed, Dolphin Config Window will be set to disabled automatically in Dolphin Controller Configurator. For more information see https://wiki.dolphin-emu.org/index.php?title=Help:Contents

#### Player
This setting determines which player is selected. There are only 4 players, and they can either use a GameCube Controller or Wiimote for the emulated controller.

#### Real Controller
This setting determines what real controller the player will be using. Examples include Nintendo Switch Pro Controller, Dualshock 4, or even Keyboard and Mouse. This even includes real Wiimotes and GameCube Controllers as mentioned earlier.

#### Port
This setting determines what port number to be used by a controller. As mentioned earlier, Dolphin Controller Configurator will find all instances of "{PortNumber}" in the profile and replace every instance of it with the port selected here. 
This setting can be confusing to find out because some controllers may not show up the as the same device type meaning they may have the same port. For example, one controller might show up as `WGInput/0/Xbox 360 Controller for Windows` and another as `WGInput/0/Xbox One Game Controller`. Despite being used for different players, these would both have the same port number for Dolphin, so both should be set to 0 in Dolphin Controller Configurator. These two controllers should also be using different profiles.

#### Emulated Controller
Here you can select what Emulated Controller you would like to use. As of right now, the only options are Wii Remote and GameCube Controller. Under the hood this just means whether to check for controllers in `dolphinconfigpath/Profiles/Wiimote` or in `dolphinconfigpath/Profiles/GCPad`.

#### Profile
Dolphin Controller Config will read the controller profiles ending in .ini in `dolphinconfigpath/Profiles/Wiimote` or in `dolphinconfigpath/Profiles/GCPad` and check if they match the following criteria.

- Profile matches the specified Emulated Controller
- Profile matches the specified Real Controller. It's name must start with the key found in Dolphin Controller Config Setting's [RealControllers]
- Profile ends with any suffix found in Dolphin Controller Config Settings
- Profile must contain {PortNumber} somehwere inside of the file

#### Launch
Just as the name suggests, this will launch Dolphin at the path specified in settings, and with any arguments that were fed to Dolphin Controller Config.

## Custom Controller Configs
Under [Releases](https://github.com/Anti-Shulk/DolphinControllerConfig/releases), there are a number of custom controller configs (Mostly for controllers that I personally own). These can be used directly or as inspiration if desired. Some of the profiles use more complicated Dolphin Settings which I will explain here

## Tips and Tricks
### Xinput for Windows
Nintendo Switch Pro Controllers, Nintendo Switch Joy-cons and Dualshock 4 and 5 controllers do not support Xinput out of the box on windows. They can still be used with dolphin, but using the following projects will allow for a lot of additional functionality including Xinput support which will allow them to be used with Dolphin Controller Configurator on Windows. They also allow for rumble, controller led control, motion controls, and more, so even when not using Dolphin Controller Config, I would reccomend using these projects with Dolphin reguardless.
#### BetterJoy
https://github.com/Davidobot/BetterJoy
#### DS4 Windows 
https://ds4-windows.com/drivers/
