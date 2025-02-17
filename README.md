# RDF

Radio Direction Finder plugin for [EuroScope](https://www.euroscope.hu).

## Modifications
This is a modified version of the original RDF Plugin by Kingfu Chan, adapted for use in the VATSIM Germany Langen FIR package by Ben Böckmann. The modifications include:
- Changed default configuration settings
- Added predefined styles for Langen operations
- Added support to toggle RDF

All modifications are released under the same GPLv3 license as the original work.

## *TrackAudio* & *Audio for VATSIM Standalone Client* Support

Supports *Audio for VATSIM standalone client* through hidden window messaging. This has been used since the birth of AFV.

[*TrackAudio*](https://github.com/pierr3/TrackAudio) is a multi-platform Audio-For-VATSIM ATC client for Windows, macOS and Linux. This improved RDF plugin utilizes [*TrackAudio*'s WebSocket SDK](https://github.com/pierr3/TrackAudio/wiki/SDK-documentation) to synchronize active frequencies and to achieve radio-direction-finding.

> [!IMPORTANT]
> Since RDFPlugin version 1.4.1+, *TrackAudio* [1.3.0 Beta 5](https://github.com/pierr3/TrackAudio/releases/tag/1.3.0-beta.5) is required due to changes in *TrackAudio* SDK.

## More Customizations

+ (Existing feature) RGB settings for circle or line, and different color for concurrent transmission.
+ Random radio direction offsets to simulate measuring errors in real life.
+ Offers variable precision at different altitudes.
+ Hide radio-direction-finders for low altitude aircrafts.
+ Draw controllers as desired.
+ ASR-specific drawing parameters including colors, precision and filtering.
+ Tag item type **RDF state** to indicate previously transmitting aircraft.

## Integrate afv-bridge

Do the same work as [*afv-euroscope-bridge*](https://github.com/AndyTWF/afv-euroscope-bridge). Supports both *TrackAudio* and *Audio for VATSIM standalone client*. When a new radio station is set up for RX/TX, RDF detects its status and toggles respective channels in EuroScope.

## Configurations

There are two ways to modify the plugin settings - by settings file and by commandline functions.

This table shows all configurable items.

| Entry Name                | Command Line Keyword | Range       | Default Value   |
| ------------------------- | -------------------- | ----------- | --------------- |
| LogLevel                  |                      |             | None            |
| Endpoint                  |                      |             | 127.0.0.1:49080 |
| TrackAudioMode            |                      | -1, 0, 1    | 1               |
| RGB                       | RGB                  | RRR:GGG:BBB | 255:255:255     |
| ConcurrentTransmissionRGB | CTRGB                | RRR:GGG:BBB | 255:0:0         |
| Radius                    | RADIUS               | (0, +inf)   | 20              |
| Threshold                 | THRESHOLD            |             | -1              |
| Precision                 | PRECISION            | [0, +inf)   | 0               |
| LowAltitude               | ALTITUDE L_____      |             | 0               |
| HighAltitude              | ALTITUDE H_____      | [0, +inf)   | 0               |
| LowPrecision              | PRECISION L_____     |             | 0               |
| HighPrecision             | PRECISION H_____     | [0, +inf)   | 0               |
| DrawControllers           | CONTROLLER           | 0 or 1      | 0               |

For command line configurations, use `.RDF KEYWORD VALUE`, e.g. `.RDF CTRGB 0:255:255`. Replace "_____" with value in low/high altitude/precision directly, e.g. `.RDF ALTITUDE L10000`. All command line functions are case-insensitive.

In settings files the default is like the following:

```text
PLUGINS
<eventually existing configuration lines>
RDF Plugin for Euroscope:LogLevel:none
RDF Plugin for Euroscope:Endpoint:127.0.0.1:49080
RDF Plugin for Euroscope:TrackAudioMode:1
RDF Plugin for Euroscope:RGB:255:255:255
RDF Plugin for Euroscope:ConcurrentTransmissionRGB:255:0:0
RDF Plugin for Euroscope:Radius:20
RDF Plugin for Euroscope:Threshold:-1
RDF Plugin for Euroscope:Precision:0
RDF Plugin for Euroscope:LowAltitude:0
RDF Plugin for Euroscope:HighAltitude:0
RDF Plugin for Euroscope:LowPrecision:0
RDF Plugin for Euroscope:HighPrecision:0
RDF Plugin for Euroscope:DrawControllers:0
END
```

+ **LogLevel** is none by default. Accepted levels include none, error, warning, info, debug, verbose. Log levels other than none will automatically save an *RDFPlugin.log* file next to DLL file.
+ **Endpoint** should include address and port only. E.g. 127.0.0.1:49080 or localhost:49080, etc.
+ **TrackAudioMode** defines the behaviour between RDF and *TrackAudio*. -1 will disable all *TrackAudio* features; 0 will only enable radio-direction-finder; 1 will also update EuroScope channels when *TrackAudio* stations are updated.
+ **RGB, ConcurrentTransmissionRGB**, see [README](#readme-for-legacy-versions) below.
+ **Radius, Threshold, Precision, LowAltitude, HighAltitude, LowPrecision, HighPrecision** see [Random Offset Schematic](#random-offset-schematic) below.
+ **DrawControllers** is compatible with both *TrackAudio* and *Audio for VATSIM standalone client*. Other transimitting controllers will be drawn as well. 0 means OFF and other numeric value means ON.

## General Command Line Functions

`.RDF REFRESH`

+ Clear transmission records.
+ (*Audio for VATSIM standalone client*) set all channels to off (except primary & active ATIS).
+ (*TrackAudio*, when **TrackAudioMode** is not -1 or 0) refresh all channels to sync *TrackAudio*.

`.RDF RELOAD`

+ Reload settings in *Settings File Setup*.
+ Discard unsaved modifications and restore configurations per ASR.
+ Reset *TrackAudio* connection.
  
> [!TIP]
> To change the endpoint or mode for *TrackAudio* without exitting EuroScope, you may modify plugin settings file, reload settings file inside EuroScope, then run `.RDF RELOAD`.
> To change log level, the only way is to unload & reload RDFPlugin.dll inside EuroScope plugin setup dialog.

## Per ASR Configurations

Since v1.3.5, all configurations made by command line funcions, except those related to *TrackAudio*, will only be effective in current ASR and will be saved to ASR instead of plugin settings file. An RDF-configured ASR may contain the following lines, whose value should be consistent with the above:

```text
; These are not default values!
PLUGIN:RDF Plugin for Euroscope:RGB:255:255:255
PLUGIN:RDF Plugin for Euroscope:ConcurrentTransmissionRGB:255:255:0
PLUGIN:RDF Plugin for Euroscope:Radius:20
PLUGIN:RDF Plugin for Euroscope:Threshold:-1
PLUGIN:RDF Plugin for Euroscope:Precision:0
PLUGIN:RDF Plugin for Euroscope:LowAltitude:0
PLUGIN:RDF Plugin for Euroscope:HighAltitude:0
PLUGIN:RDF Plugin for Euroscope:LowPrecision:0
PLUGIN:RDF Plugin for Euroscope:HighPrecision:0
PLUGIN:RDF Plugin for Euroscope:DrawControllers:1
```

When an ASR is opened, the plugin will use the configurations in the sequence of **ASR > plugin settings file > default value**.

## Random Offset Schematic

+ **LowAltitude** in feet, is used to filter aircrafts. Only aircrafts not lower than this altitude will be radio-direction-found.
+ A circle will only be drawn within radar display area. Otherwise a line leading to the target is drawn.
+ Random offsets (when enabled) follow a normal distribution. 99.74% (-3σ ~ 3σ) of offsets are within given precision.
+ **Threshold < 0**:
  + **Radius** is in pixel. Circles are always drawn in fixed pixel radius.
  + **Precision** is used for random offset in nautical miles.
  + Low/High settings are ignored.
+ **Threshold >= 0**:
  + **Threshold** is in pixel. **Radius** is in nautical miles. **Precision** is in nautical miles.
  + Circle size will change according to zoom level. Circles are drawn only when its pixel radius is not less than **Threshold**. Otherwise a line leading to the target is drawn.
  + When **LowPrecision > 0**:
    + Deprecates **Radius**. All circle radius is determined by precision.
    + If **HighPrecision > 0 and HighAltitude > LowAltitude**:
      + Overrides **Precision**. Dynamaic precision is implemented taking aircraft altitude into account.
      + Precision (= radius) is linearly interpolated or extrapolated by altitude and low/high settings. `Precision = LowPrecision + (Altitude - LowAltitude) / (HighAltitude - LowAltitude) * (HighPrecision - LowPrecision)`
    + Otherwise **LowPrecision** precedes **Precision** when determining random offset.

## Known Issues

+ EuroScope may crash when using TopSky at the same time with certain TopSky settings due to conflicting API method to communicate with *Audio for VATSIM standalone client*. Goto *TopSkySettings.txt* and add `RDF_Mode=-1` to prevent such cases.
+ Do not simultaneously load this plugin along with the older version of RDF, or with the original *afv-euroscope-bridge* plugin, which may cause unexpected behavior.

> [!IMPORTANT]
> Check your .prf content and make sure *afv-euroscope-bridge* will not be loaded.
>
> And check your *TopSkySettings.txt* if you are using TopSky.
>
> Fail to obey these rules may result in accidental crash.

+ *Audio for VATSIM standalone client* doesn't provide callsign for RX/TX, so this plugin has to guess the corresponding callsign and it doesn't guarantee 100% correct toggles. But it shouldn't affect text receive and transmit function.
+ When using professional correlation mode (S or C) in EuroScope, it's possible some aircraft won't be radio-direction-found because the plugin doesn't know the callsign for an uncorrelated radar target.
+ For dual pilot situation where the transmitting pilot logs in as observer, this plugin will try to drop the last character of the observer callsign and find again if this dropped character is between A-Z. This feature may cause inaccurate radio-direction.
+ Because of new drawing behaviour introduced after *EuroScope v3.2.3*, switching between ASRs may not change the plugin drawing configurations. In such case, simply pan/zoom your view to update configurations per ASR.

## Credits

+ [pierr3/VectorAudio](https://github.com/pierr3/VectorAudio) & [pierr3/TrackAudio](https://github.com/pierr3/TrackAudio): initiative.
+ [chembergj/RDF](https://github.com/chembergj/RDF): basic drawings.
+ [AndyTWF/afv-euroscope-bridge](https://github.com/AndyTWF/afv-euroscope-bridge): *Audio for VATSIM standalone client* message handling.
+ [yhirose/cpp-httplib](https://github.com/yhirose/cpp-httplib): HTTP library.
+ [machinezone/IXWebSocket](https://github.com/machinezone/IXWebSocket): WebSocket library.
+ [nlohmann/json](https://github.com/nlohmann/json): json handling.
+ [SergiusTheBest/plog](https://github.com/SergiusTheBest/plog): logging library.

## Developing

[Vcpkg](https://vcpkg.io/), either standalone or bundled with Visual Studio v17.6+, is required. Run `vcpkg integrate install` in Visual Studio CMD/Powershell and build directly.

## [README for Legacy Versions](https://github.com/chembergj/RDF#rdf)
