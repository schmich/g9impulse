setPreference -pref UserLevel:NOVICE
setPreference -pref MessageLevel:DETAILED
setPreference -pref ConcurrentMode:FALSE
setPreference -pref UseHighz:FALSE
setPreference -pref ConfigOnFailure:STOP
setPreference -pref StartupCLock:AUTO_CORRECTION
setPreference -pref AutoSignature:FALSE
setPreference -pref KeepSVF:FALSE
setPreference -pref svfUseTime:FALSE
setPreference -pref UserLevel:NOVICE
setPreference -pref MessageLevel:DETAILED
setPreference -pref ConcurrentMode:FALSE
setPreference -pref UseHighz:FALSE
setPreference -pref ConfigOnFailure:STOP
setPreference -pref StartupCLock:AUTO_CORRECTION
setPreference -pref AutoSignature:FALSE
setPreference -pref KeepSVF:FALSE
setPreference -pref svfUseTime:FALSE
setMode -bs
setPreference -pref UserLevel:Novice
setMode -pff
addConfigDevice -size 0 -name "PFFConfigDevice" -path "C:\dev_zuofu\395_vgs\ise\xsa-50\gpu/"
setAttribute -configdevice -attr size -value "0"
setAttribute -configdevice -attr reseveSize -value "0"
setSubmode -pffparallel
setAttribute -configdevice -attr activeCollection -value "GPU"
addCollection -name "GPU"
addDesign -version 0 -name "0"
addDeviceChain -index 0
addDevice -position 1 -file "C:\dev_zuofu\395_vgs\ise\XSA-50\GPU\gpuchip.bit"

addPromDevice -position 1 -size 128 -name "128k"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr swapBit -value "FALSE"
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -collection -attr dir -value "UP"
setMode -mpm
setMode -cf
setMode -dtconfig
setMode -bsfile
setMode -sm
setMode -ss
setMode -bs
setMode -pff
setMode -bs
setMode -ss
setMode -sm
setMode -bsfile
setMode -dtconfig
setMode -cf
setMode -mpm
setMode -pff
setCurrentDeviceChain -index 0
setMode -pff
