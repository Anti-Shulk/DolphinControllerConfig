# Dolphin Controller Configurator
Dolphin Controller Configurator is a gui Qt app designed to make configuring    [Dolphin Emulator's](https://dolphin-emu.org/) controllers quicker and possible with only a controller. It makes use of Dolphin Emulator's built in controller profiles and allows selecting them in an easy to navigate way.

## Features
- Passing Command-line arguments to Dolphin 
- Keyboard or Controller navigation
- Replacing the port number of Dolphin's configs

# Setup
Dolphin Controller Configurator can be downloaded under [**Releases**](https://github.com/Anti-Shulk/DolphinControllerConfig/releases)
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
kbm=Keyboard and mouse
none=No Controller
```
The key, which is on the left of the equals sign, holds the prefix for the name used for the configuration file. Configuration files are stored in the `dolphinconfigpath/Profiles/Wiimote` and `dolphinconfigpath/Profiles/GCPad` directories.
For example, there might be a profile called `nsprowiinumchuck.ini` in the `Wiimote` directory. The Real Controller (Prefix) for this file is `nspro`. 
The right side of the equals sign shows what that key will show up as in Dolphin Controller Configurator, so `xone` will show as `Xbox One Controller` in the above example.
There are 2 special Real Controllers, the `wii` and `gc`. These cannot be set up with a profile and will be used as Real Controllers in Dolphin. These controllers can still be used as an emulated controller in dolphin, but only if they are set up with an alternative name.
`none` can also be added as a real controller to allow setting a player to have no controller (Reccomended)

#### Profile Suffixes
This stores the second half of the profile name. 
Here is an example:
```ini
[ProfileSuffixes]
gceast=East Layout (A is right button)
gcwest=West Layout (A is bottom button)
wiinumchuck=Wiimote and Numchuck
wiisideways=Wiimote Sideways
default=Default Dolphin Layout
```
These can be used as the suffix for a profiles when combined with a real controller. For example, `nsprowiinumchuck` has a suffix of `wiinumchuck`, and will be displayed as `Nintendo Switch Pro Controller Wiimote and Numchuck` in Dolphin Controller Configurator. 

#### Profiles
As mentioned earlier, profiles are located in the dolphinconfigpath/Profiles/Wiimote and dolphinconfigpath/Profiles/GCPad directories. These can be edited manually, or using Dolphin in the Controllers Menu.
##### Dolphin example
![](https://i.imgur.com/NRmxym0.png)

When creating a profile in Dolphin, type in a name, and click save to store the profile in the Wiimote directory for Wiimotes or GCPad directory for GameCube Controllers. 
**There is another additional step that must be taken for profiles to show up**
Profiles must be manually edited to say "{PortNumber} all locations of the port number. There is usually only 1.

For example, a profile might say
`Device = WGInput/0/Xbox 360 Controller for Windows`
the 0 must be changed to "{PortNumber}" like so:
`Device = WGInput/{PortNumber}/Xbox 360 Controller for Windows`
This allows Dolphin Controller Configurator to change the port number in the interface. Profiles that do not have this change done will **not** show up.

# Main Interface
![](https://i.imgur.com/T2HMUY3.png)
The interface can be controlled using any Xinput Controller in Windows or any evdev/sdl controller on Linux. (idk about Mac can't test). Some controllers do not support Xinput, so a workaround will be explained in the Tips and Tricks section. Only the first controller is used by Dolphin Controller Configurator.

#### Dolphin Config Window
This setting determines whether or not Dolphin Emulator's configuration window will be visible. This launches Dolphin Emulator with --batch argument, but this is only possible if a specific game file is specified using -e or --exec.
When launching Dolphin Controller Configurator with command line arguments, those arguments will be fed to Dolphin. If -b or --batch is fed, the Dolphin Config Window will be set to disabled automatically in Dolphin Controller Configurator. For more information see https://wiki.dolphin-emu.org/index.php?title=Help:Contents

#### Player
This setting determines which player is selected. There are only 4 players, and they can either use a GameCube Controller or Wiimote for the emulated controller.

#### Real Controller
This setting determines what real controller the player will be using. Examples include Nintendo Switch Pro Controller, Dualshock 4, or even Keyboard and Mouse. This even includes real Wiimotes, GameCube Controllers, and None as mentioned earlier. 

#### Port
This setting determines what port number to be used by a controller. As mentioned earlier, Dolphin Controller Configurator will find all instances of "{PortNumber}" in the profile and replace every instance of it with the port selected here. 
This setting can be confusing to find out because some controllers may not show up the as the same device type meaning they may have the same port. For example, one controller might show up as `WGInput/0/Xbox 360 Controller for Windows` and another as `WGInput/0/Xbox One Game Controller`. Despite being used for different players, these would both have the same port number for Dolphin, so both should be set to 0 in Dolphin Controller Configurator. These two controllers should also be using different profiles.

#### Emulated Controller
Here you can select what Emulated Controller you would like to use. As of right now, the only options are Wii Remote and GameCube Controller. Under the hood this just means whether to check for controllers in `dolphinconfigpath/Profiles/Wiimote` or in `dolphinconfigpath/Profiles/GCPad`.

#### Profile
Dolphin Controller Config will read the controller profiles ending in .ini in `dolphinconfigpath/Profiles/Wiimote` or in `dolphinconfigpath/Profiles/GCPad` and check if they match the following criteria.

- Profile matches the specified Emulated Controller
- Profile matches the specified Real Controller. The profile name must start with the key found in Dolphin Controller Config Setting's [RealControllers]
- Profile ends with any suffix found in Dolphin Controller Config Settings
- Profile must contain {PortNumber} somehwere inside of the file

#### Launch
Just as the name suggests, this will launch Dolphin at the path specified in settings, and will pass any arguments that were fed to Dolphin Controller Configurator.

# Custom Controller Configs
Under [Releases](https://github.com/Anti-Shulk/DolphinControllerConfig/releases), there are a number of custom controller configs (Mostly for controllers that I personally own). These can be used directly or as inspiration if desired. Some of the profiles use complicated Dolphin settings allowing changing controller layouts during gameplay. These layouts have all had the sticks calibrated using my controllers, but my controllers are almost certainly different than yours, so it is probably worth recalibrating them.

### GameCube Layouts
GameCube Layouts are far more simple than Wiimote layouts, but there is still noteworthy info especially for controllers without analog support such as Nintendo Switch Pro Controllers.

#### `kbmdefault`
This simply usese the default dolphin layout for a keyboard and mouse, but only makes use of keyboard.

#### Layouts ending in `gceast`
Below is a chart showing the notable mappings for `gceast` layouts. Face buttons may or may not match button labels like with the Xbox One Controller
| GameCube Botton | Real Controller Button |
| --- | ----------- |
| A | Right button (B on Xbox, A on Pro-con and circle on DS4) |
| B | Bottom button |
| X | Top button |
| Y | Left button |
| Z | Right bumper |
| Left & Right trigger | Analog equivalent if controller supports. If not read below |

`ds4gceast` and `xonegceast` make use of analog triggers. This is used in some GameCube games such as Super Mario Sunshine to switch between first person and third person F.L.O.O.D., and in Super Smash Bros. Melee to use the light shield. Nintendo Switch Controllers do not have analog triggers, so a compromise has been made to allow holding the left bumper and the triggers to press them all the way down. Different layouts can be made from game to game depending on how the triggers are used if desired.

### Wiimote Layouts
There is quite a lot for Wiimote layouts such as motion controls, numchucks, holding wiimote vertically or horizontally and more.
#### `kbmdefault`
This simply usese the default dolphin layout for a keyboard and mouse. There is no special functionality for this layout
#### Layouts ending in `wiinumchuck`
By default, these layouts are setup to use a Wiimote vertically with a Numchuck plugged in, but this can be changed though special button combinations.
Below is a chart showing the notable mappings for `wiinumchuck` layouts. Face buttons won't match button labels
| Wiimote or Numchuck Botton | Real Controller Button |
| --- | ----------- |
| A | Right button (B on Xbox, A on Pro-con and circle on DS4) |
| B | Bottom button |
| 1 | Left button |
| 2 | Top button |
| C | Either bumper |
| Z | Either trigger |
| Dpad | By default used as if Wiimote is held vertically. Controlled using controller's dpad |
| Numchuck stick | Left stick |
| IR (Wii cursor) | Right stick or motion controls if suppported controller and enabled (not supported on Xbox) | 
| Recenter | Clicking in on the right stick |

The following are special button combinations 
| Combination | Action |
| --- | ----------- |
| Minus (or equivalent) and Plus (or equivalent) | Presses Wiimote home button
| Minus (or equivalent) and right button  | Toggle numchuck on or off - By defualt, the numchuck is on, but it can be "unplugged" with this button combination. Keep in mind that the Wiimote will stay vertical. When the numchuck is disabled, the wiimote dpad can be controlled using the left stick in addition to the real controller's dpad. |
| Minus (or equivalent) and bottom button  | Toggle wiimote vertical/sideways - By default, the Wiimote is set to vertical, but it can be turned sideways which will change the button layout and dpad directions. When this is pressed, 1 becomes button button, 2 becomes right button, A becomes Left button, and B becomes button. |
| Minus (or equivalent) and left button | Toggle motion controls (nspro/joy and ds4 only) - By default, motion controls are disabled, but if the DSU server is enabled and configured properly in BetterJoy or DS4 Windows, motion controls should be enabled including the IR/Wiimote cursor. This can be fun to mess around with. |

# Tips and Tricks
#### Use Beta or Dev builds of Dolphin Emulator instead of Dolphin 5.0
It is that simple. https://dolphin-emu.org/download/?ref=btn Huge advancements have been made since Dolphin 5.0
#### Using with game frontends
When I originially created this program, one of the biggest reasons was to use it with front ends like [Playnite](https://playnite.link/), [Big Box/Launchbox](https://www.launchbox-app.com/), [Emulation Station Desktop Edition](https://es-de.org/), [Steam](https://store.steampowered.com/), [Lutris](https://lutris.net/), [Game Hub](https://tkashkin.github.io/projects/gamehub/), and any others. This app should be usable with any of these. Simply pass the arguments such as --exec to Dolphin Controller Config, and these will be passed to Dolphin allowing launching directly into a game. 
#### Use WGInput instead of Xinput, SDL, or Dinput in Dolphin on Windows (My opinion)
There are lots of different controller options you might find in dolphin such as dinput, wginput, xinput, sdl, evdev, and it can be overwhelming. Many devices fall into more than one category, but obviously you can only use one at a time. When I test Dolphin, I check and make sure that when calibrating the sticks, it isn't chopped off at the edges. I also see if rumble is supported and motion controls as well. SDL on Nintendo Switch Pro Controllers gave me stick and motion issues (more info can be found in BetterJoy section below). Overall I believe WGInput is better than Xinput on Dolphin in most circumstances, but not for any particular reason. SDl on the other hand has given me issues, but may work well for DS4 Controllers espeically when not using DS4 Windows which will be talked about later. Dolphin Controller Configurator can configure with any of these layouts, but only supports Xinput for navigating the menus on Windows.

#### Xinput/WGInput for Windows
Nintendo Switch Pro Controllers, Nintendo Switch Joy-cons and Dualshock 4 and 5 controllers do not support Xinput out of the box on Windows (and many other controllers I am sure). They can still be used with dolphin, but using the following projects will allow for a lot of additional functionality including Xinput support which will allow them to be used with Dolphin Controller Configurator on Windows. They also allow for rumble, controller led control, motion controls, and more, so even when not using Dolphin Controller Config, I would reccomend using these projects with Dolphin reguardless.
#### BetterJoy
[BetterJoy](https://github.com/Davidobot/BetterJoy) is a very helpful tool for using Nintendo Switch Pro Controllers and Joy-cons on Windows. It even supports Motion controls through the DSU server. In a recent [Dolphin Emulator blog post](https://dolphin-emu.org/blog/2022/09/13/dolphin-progress-report-july-and-august-2022/), support for SDL was announced which supposedly works better than the DSU server, but I found that for motion, the DSU server still worked better with my controllers and had severe motion drifting on SDL. (You can use SDL with BetterJoy by enabling DS4 I believe)

##### Guide version 7.1
Be sure to read the README.md on the BetterJoy page and the README in the drivers folder. The below is just additional advice that I found worked best for me with Dolphin and Steam.
- When you install [BetterJoy](https://github.com/Davidobot/BetterJoy) from the releases page, and extract the zip folder, there should be a few folders inside. I intalled both ViGemBus and HIDGuardian and HIDGuardian. HIDGuardian should be installed by running as administrator. I have found HIDGuardian to be almost essential (espeically with Steam), but as the README in the Drivers folder says, it may cause headaches as well. 
- When running BetterJoy, I find it works better when running as Administrator as well.
- In BetterJoy, more settings can be found by clicking the small arrow on the right side of the application. In here, I turn on `UseHIDG` and `MotionServer`. I do not reccomed turning on `SwapAB` and `SwapXY`. HIDGuardian in particular is helpful espeically with Steam. I find that without it, there are two controllers connected which causes all sorts of weird things like random and it would even allow Steam to take control of my mouse. 
- If you have any other issues with Steam, such as the Pro Controller stealing your mouse cursor, try opening Controller/Base Configurations/Desktop Configurations in either Steam Settings or Steam Big Picture Settings menu (this may change with the new Big Picture mode) and changing/disabling some of the settings.
- In order to use the DSU server with Dolphin, it must be enabled under `Alternate Input Sources`. Be sure to name it `DSU` to match the provided controller profiles if desired. You can have more than one Alternative input port named DSU.
- BetterJoy only supports one controller over DSU as far as I can tell, but I cannot verify. It can be used in combination with DS4 Windows if both are using different port numbers.

The Dolphin Controller Config provided controller profiles are designed for the configuration talked about here.

#### DS4 Windows 
Unlike BetterJoy which is essenitially required, [DS4 Windows](https://ds4-windows.com/drivers/) is not quite as necessary for use with Dolphin. It should work with DS3, DS4 and Dual Sense  Controllers, but I only have a DS4 to test with. SDL on a Dual Shock works far better than it does through BetterJoy from my testing, but reguardless, the included layouts are setup to use a Dualshock controller with DS4 Windows under WGInput and DSU. 

##### Guide 
Be sure to read the DS4 Windows Page https://ds4-windows.com/get-started/. This is only for additinal tips and using DS4 Windows with Dolphin
- I would reccomend turning on `Hide DS4 Controller` in settings mostly to prevent double inputs in steam
- Much of the tips from BetterJoy also apply here
- The DSU Server is called UDP Server. If you would like to use it in combination with BetterJoy, be sure they are on different ports. I believe only 1 controller can be used
- In order to use the DSU server with Dolphin, it must be enabled under `Alternate Input Sources`. Be sure to name it `DSU` to match the provided controller profiles if you would like
- Use Smoothing may be helpful as well
- Both of these programs use very similar if not identical layouts in Dolphin. That means they also share port numbers, so a DS4 might be port 0 and an Pro-con might be port 1.
