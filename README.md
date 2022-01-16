# Yet another BDSP randomizer
This project is a technical study.
It is not intended for general public distribution or high quality randomizer.
This randomizer uses player's trainer ID as psedo seed to genrate randomness.

This project is basically fork of:
* https://github.com/skyline-dev/skyline
* https://github.com/tech-ticks/hyperbeam

and many other project based on skyline ....

Support:
* Brilliant Diamond v1.1.1

Features:
* Randomize starters
* Randomize encounter table
* Randomize trainer table
* Randomize Field items (and hidden items)
* Avoid impossible level pokemon encounter (i.e: Dragonite lvl3)

Most of 1.1.1's cheat code should work with this project.

TODO:
* configurable seed
* configurable lengedary
* (raise an issue if you have any suggestion) ...

# Build
It requires DEVKITPRO environment
```
make all -j4
```

# Install
copy/merge the atmosphere directory with the root of your nintendo switch SD card 
* atmosphere/exefs_patches/pokemon_bdsp_0100000011D90000/D9E96FB92878E3458AAE7E8D31AB32A9.ips
* atmosphere/contents/0100000011D90000/exefs/subsdk1.nso
```
dist/
└── 111
    └── atmosphere
        ├── contents
        │   └── 0100000011D90000
        │       └── exefs
        │           └── subsdk1.nso
        └── exefs_patches
            └── pokemon_bdsp_0100000011D90000
                └── D9E96FB92878E3458AAE7E8D31AB32A9.ips
```
