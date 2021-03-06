# Mountains
Load a mountains mesh and display it. It can compile to Windows or Mac native apps. Or, you can build mountains using Emscripten and run it in any modern browser that supports WebGL 2.

![vino map gif](docs/demonstration.gif)

## Installing
Clone this repository. 
```
git clone https://github.com/tomconder/mountains.git
```

Install Conan, a dependency and package manager for C++. If you have Python installed, use pip to [install conan](https://docs.conan.io/en/latest/installation.html). 
```
pip install conan
```


## Building on Windows using VSCode
When using MSYS MINGW ensure that you do not have conflicts with multiple compilers, cmake, ninja, etc.
Focus on the MINGW packages instead as they have the latest and appear to work when finding SDL2 Modules and OpenGL.
If there is an issue finding SDL2 libraries etc. it may have an issue with case sensitivity. (Just be aware)

## Running on Safari
In order to run this app in Safari you have to enable WebGL 2.

* From the menu bar, click 'Safari' then select 'Preferences'
* Select the 'Advanced' tab
* Check the box to 'Show Develop menu in menu bar'
* From the menu bar, click 'Develop' then select 'Experimental features'
* Click to enable 'WebGL 2.0'
