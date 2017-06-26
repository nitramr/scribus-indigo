Scribus-Indigo [![Build Status](https://travis-ci.org/nitramr/scribus-indigo.svg?branch=master)](https://travis-ci.org/nitramr/scribus-indigo) [![forum-thread](https://img.shields.io/badge/forum--thread-scribus-orange.svg)](http://forums.scribus.net/index.php/topic,1617.0.html)
==============
The Scribus-Indigo branch is an unofficial experimental user interface version of the original open source Scribus desktop publishing software. The project includes:
* a new application layout
* control elements 
* different color themes inclusive dark and bright icon sets
* workflow improvements

## Status
This is a experimental build of the Scribus DTP software that introduces and integrates [indigoDock](https://github.com/nitramr/indigoDock) in to Scribus trunk. 

## Snapshot

![dark theme](https://cloud.githubusercontent.com/assets/15112256/20462261/55a690be-af18-11e6-931b-a3e5226bcf53.png)

## Contributing
For Bugs: please open an issue in this repo [issue queue](https://github.com/nitramr/scribus-indigo/issues)  
For Patches: submit a PR 

## Discussion 
Most of the dicussion is occuring in the [Scribus UI/UX subforum](http://forums.scribus.net/index.php/topic,1617.0.html)

## Links
|Resource | Address |  
|:---------|:---------|  
|Website |http://www.scribus.net |  
|Wiki | http://wiki.scribus.net |  
|List | http://lists.scribus.net |  
|Forums | http://forums.scribus.net | 
|WebSVN | http://scribus.net/websvn |
|SVN | svn://scribus.net/trunk/Scribus |  
|IRC | irc://scribus@chat.freenode.net |  


## Source Code Compiling (Linux)

### QT 5

First you need Qt5 libraries to compile Scribus. Install them via following code line in terminal:

```C++
sudo apt-get install qt5-default
```

### Dependencies

Check if all necessary dependencies are installed on your system.
Best way to check it is via synaptics app. Open synaptics and search for following packages. If any package is missing install it directly via synaptics.

These packages are needed:
libboost-python-dev
libcups2-dev
libhunspell-dev
libhyphen-dev
liblcms2-dev
libpodofo-dev
libtiff5-dev
libxml2-dev
python-all-dev
zlib1g-dev
libgraphicsmagick++1-dev
libpoppler-dev
libpoppler-private-dev
libcairo2-dev
libwpg-dev
libmspub-dev
libcdr-dev
libvisio-dev
libharfbuzz-dev
libharfbuzz-icu0
coreutils
binutils
python-tk
qtdeclarative5-dev


### Cmake
The best way to compile Scribus is via Cmake.  

If Cmake is not installed on your system open terminal and type in following command and press enter:  

```
sudo apt-get install cmake
```

If Cmake is already installed do following steps.  
1. Download Scribus-Indigo to your local system or clone the git repository  
2. Create a new folder "Scribus" on your desktop  
3. Unzip the downloaded project into your Scribus folder on desktop (Desktop/Scribus/scribus-indigo-master)  
4. Create a new folder "bin" in your Scribus folder on desktop (Desktop/Scribus/bin)  
5. Open terminal and enter following commands to compile the source code:  

```
cd ~/Desktop/Scribus/bin
cmake --debug-output ../scribus-indigo-master
cmake . -DAPPLICATION_CONFIG_DIR=~/.config/scribus-indigo
make -j8
```

After compiling was successfully you have to install the compiled file and necessary resources. Enter following command:  

```
sudo make install
```

You can find Scribus in following location:  

```
/usr/local/bin/
```  

For uninstalling use following commands:  

```
cd ~/Desktop/Scribus/bin
sudo make uninstall
```
