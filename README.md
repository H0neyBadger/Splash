# Yet another BDSP randomizer
This project is a technical study.
It is not intended for general public distribution or high quality randomizer.

This project is basically fork of:
* https://github.com/skyline-dev/skyline
* https://github.com/tech-ticks/hyperbeam
and many other project based on skyline ....

Support:
* Brilliant Diamond v1.0.0

# Build
It requires DEVKITPRO environment
```
make all
```

# Install
copy/merge the atmosphere directory with the root of your nintendo switch SD card 
* atmosphere/exefs_patches/pokemon_bdsp_0100000011D90000/F87FC6075104EC4D9642A4AA6BB22216.ips
* atmosphere/contents/0100000011D90000/exefs/subsdk1.nso
```
dist/
└── 100
    └── atmosphere
        ├── contents
        │   └── 0100000011D90000
        │       └── exefs
        │           └── subsdk1.nso
        └── exefs_patches
            └── pokemon_bdsp_0100000011D90000
                └── F87FC6075104EC4D9642A4AA6BB22216.ips
```
