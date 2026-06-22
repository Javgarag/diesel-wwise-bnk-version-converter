# Diesel Wwise Soundbank Version Converter
Included in this solution is:
1. *DieselWwiseSoundbank*: Library for reading Wwise soundbanks from 2013, 2015 and 2022, which are the observed versions in modern Diesel engine titles; *PAYDAY 2* and *RAID: World War II*. Additionally, it is able to **convert** (migrate) older soundbanks to a newer version from the three mentioned before, so that soundbanks generated in older versions can play again.
2. *ConverterClient*: CLI tool for reading soundbanks and converting them between the supported Wwise versions. A rough memory representation of soundbank contents can be explored through any IDE's object inspector; you may also use [this ImHex pattern to explore the raw file](https://gist.github.com/Javgarag/df04809678301b729f175ee4ddba149b).

## Supported versions
- *2013*: 88
- *2015*: 113
- *2022*: 145

## Supported section headers
- *BKHD*
- *STMG*
- *DATA*
- *DIDX*
- *HIRC*: with the following objects..
    - Action
    - Actor Mixer
    - Attenuation
    - Audio Devices
    - Buses
    - Events
    - FX Share Sets
    - Layer Containers
    - Music Random Sequence Containers
    - Music Segments
    - Music Switches
    - Music Tracks
    - Random Sequence Containers
    - Sounds
    - Switch Containers
- *ENVS*
- *STID*

Format reading was mostly written using [bnnm](https://github.com/bnnm)'s work on [wwiser](https://github.com/bnnm/wwiser) as reference, and the publicly available Wwise versions and associated resources. 