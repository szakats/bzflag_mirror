-file ../../bin/bzflag.exe bzflag.exe
-file ../../bin/bzfs.exe bzfs.exe
#-file ../../bin/bzfls.exe bzfls.exe
-file bin/bzfuinst.exe bzfuinst.exe
-pushdir data
-file ../../data/config.bzc config.bzc
-file ../../data/menu.bzc menu.bzc
-file ../../data/view.bzc view.bzc
-file ../../data/background.bzg background.bzg
-file ../../data/basicconsole.bzg basicconsole.bzg
-file ../../data/clouds.bzg clouds.bzg
-file ../../data/console.bzg console.bzg
-file ../../data/explosion.bzg explosion.bzg
-file ../../data/flag.bzg flag.bzg
-file ../../data/ground.bzg ground.bzg
-file ../../data/logo.bzg logo.bzg
-file ../../data/menu.bzg menu.bzg
-file ../../data/menuptr.bzg menuptr.bzg
-file ../../data/models.bzg models.bzg
-file ../../data/radar.bzg radar.bzg
-file ../../data/shot.bzg shot.bzg
-file ../../data/shot-gm.bzg shot-gm.bzg
-file ../../data/shot-l.bzg shot-l.bzg
-file ../../data/shot-sw.bzg shot-sw.bzg
-file ../../data/stars.bzg stars.bzg
-file ../../data/tank1.bzg tank1.bzg
-file ../../data/tank2.bzg tank2.bzg
-file ../../data/tank3.bzg tank3.bzg
-file ../../data/bbolt.png bbolt.png
-file ../../data/bzflag.fti bzflag.fti
-file ../../data/bzflag.ftr bzflag.ftr
-file ../../data/bmetal.png bmetal.png
-file ../../data/boxwall.png boxwall.png
-file ../../data/caution.png caution.png
-file ../../data/clouds.png clouds.png
-file ../../data/explode1.png explode1.png
-file ../../data/fixedbr.png fixedbr.png
-file ../../data/fixedmr.png fixedmr.png
-file ../../data/flag.png flag.png
-file ../../data/flage.png flage.png
-file ../../data/gbolt.png gbolt.png
-file ../../data/ground.png ground.png
-file ../../data/helvbi.png helvbi.png
-file ../../data/helvbr.png helvbr.png
-file ../../data/laser.png laser.png
-file ../../data/missile.png missile.png
-file ../../data/mountain1.png mountain1.png
-file ../../data/mountain2.png mountain2.png
-file ../../data/mountain3.png mountain3.png
-file ../../data/mountain4.png mountain4.png
-file ../../data/mountain5.png mountain5.png
-file ../../data/mountain6.png mountain6.png
-file ../../data/panel.png panel.png
-file ../../data/pbolt.png pbolt.png
-file ../../data/pyrwall.png pyrwall.png
-file ../../data/rbolt.png rbolt.png
-file ../../data/roof.png roof.png
-file ../../data/timesbi.png timesbi.png
-file ../../data/timesbr.png timesbr.png
-file ../../data/title.png title.png
-file ../../data/wall.png wall.png
-file ../../data/ybolt.png ybolt.png
-file ../../data/boom.ogg boom.ogg
-file ../../data/explosion.ogg explosion.ogg
-file ../../data/fire.ogg fire.ogg
-file ../../data/flag_alert.ogg flag_alert.ogg
-file ../../data/flag_drop.ogg flag_drop.ogg
-file ../../data/flag_grab.ogg flag_grab.ogg
-file ../../data/flag_lost.ogg flag_lost.ogg
-file ../../data/flag_won.ogg flag_won.ogg
-file ../../data/laser.ogg laser.ogg
-file ../../data/pop.ogg pop.ogg
-file ../../data/ricochet.ogg ricochet.ogg
-file ../../data/shock.ogg shock.ogg
-file ../../data/teleport.ogg teleport.ogg
-file ../../data/phantom.ogg phantom.ogg
-file ../../data/missile.ogg missile.ogg
-file ../../data/jump.ogg jump.ogg
-file ../../data/land.ogg land.ogg
-file ../../data/killteam.ogg killteam.ogg
-file ../../data/lock.ogg lock.ogg
-popdir
-pushdir doc
-file ../../LICENSE LICENSE
-file README.win32.html README.HTM
-readme README.HTM
-popdir
-shortcut bzflag.exe bzflag "" "%i"
-regkey "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall" "bzflag"
-regstring "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\bzflag" "DisplayName" "bzflag"
-regstring "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\bzflag" "UninstallString" "%i\bzfuinst.exe %i"
