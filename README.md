# malitop


Collect metrics via mali gpu registers from: https://github.com/ARM-software/libGPUCounters


## build
```
git clone https://github.com/junka/malitop.git
cd malitop
git submodule update --init
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=armtoolchain.cmake
make
```

## sample output
```
------------------------------------------------------------
 GPU Device 0: Mali-G78AE, Arch: valhall
------------------------------------------------------------
 Cores: 2, Engines 1
 Bus Width: 128
------------------------------------------------------------
Threads:
------------------------------------------------------------
 Frag      NonFrag   GeoPos    GeoVar
 0.00      0.00      0.00      0.00
------------------------------------------------------------
Utilization:
------------------------------------------------------------
 IRQ       FragQ     NonFragQ  Tiler     NonFrag   Frag      Core      ALU       Texture   LoadStore FragFPKB  Variable
 0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00
 FMAPipe   CVTPipe   SFUPipe   TexInBus  TexOutBus Any
 0.00      0.00      0.00      0.00      0.00      0.00
------------------------------------------------------------
Non-Frag:
------------------------------------------------------------
 QJob      QTask     QActiveCy ActiveCy  Thread    QUtil     Util      ThrhputCy QWaitRdCy QWaitIssCy QWaitDepCy QWaitFinCy
 0         0         0         0         0.00      0.00      0.00      0.00      0         0         0         0
 QWaitFluCy Task      Warp
 0         0         0
------------------------------------------------------------
MMU:
------------------------------------------------------------
 L2Hit     L2Read    Lookup    L3Read    L3Hit     S2Lookup  S2L3Read  S2L2Read  S2L3Hit   S2L2Hit
 0         0         0         0         0         0         0         0         0         0
------------------------------------------------------------
L2 Cache:
------------------------------------------------------------
 Lookup    RdLookup  WrLookup  Flush     Read      Write     Snp       L1Read    L1Write   SnpLookup IncSnp    RdMissRat
 0         0         0         0         0         0         0         0         0         0         0         0.00
 WrMissRat
 0.00
------------------------------------------------------------
External Bus:
------------------------------------------------------------
 WrBt      RdBt      Read      RdNoSnoop RdUnique  OTQ1      OTQ2      OTQ3      Lat0      Lat128    Lat192    Lat256
 0         0         0         0         0         0         0         0         0         0         0         0
 Lat320    Write     WrNoSnpF  WrNoSnpP  WrSnpFull WrSnpPart WrOTQ1    WrOTQ2    WrOTQ3    Lat384    RdOTQ4    WrOTQ4
 0         0         0         0         0         0         0         0         0         0.00      0.00      0.00
------------------------------------------------------------

```