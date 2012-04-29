#Drummy

## Record tabs from a song or your mind just by using your keyboard.
This is a c++/qt application which will help all the drummers out there who want to create tabs for songs.

If you want to know more, go to the [official website](http://domoritz.github.com/drummy/)!

## What to do?

You can either download this app and start recording or help me improve it (which I would really appreciate).

## If you want to help developing

1. [fork this project](http://help.github.com/fork-a-repo) on GitHub
2. [clone your fork to work on your local machine](http://help.github.com/remotes)
3. Set up your working environment and [learn how to use Qt](http://doc.qt.nokia.com/4.7/gettingstartedqt.html)
4. happy coding


## Developer notes
###Update language files
   
    lupdate -no-obsolete Drummy.pro

After updating the language file you have to build the `.qm` files out of the `.ts` files by executing

    lrelease Drummy.pro
    
## How to build

### Windows

coming soon...

### Linux

coming soon...

### Mac
	# create version file
    python git_sha.py
    
    # run qmake
    qmake Drummy.pro -r -spec macx-g++ CONFIG+=release
    
    #run make
    make -w
    
    #build app (for deployment)
    macdeployqt Drummy.app -verbose=2 -dmg

## FAQ
