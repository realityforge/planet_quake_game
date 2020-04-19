# Welcome to libwebgame!

This is a hacked up ioquake3 engine running on modern web technologies (WebGL, WebAssembly) in composition with PlayCanvas.

So you have PBR rendering, skeletal animation etc. with the game dynamics and networking of ioquake3/idtech3.

See a testing video here: https://www.youtube.com/watch?v=LcDTannSTCc

If you have questions, answers, ideas and/or whatever, dont hesitate to join our Discord server: https://discord.gg/mqBchQZ

Or register in our forum for worthwhile stuff: https://killtube.org/forum.php

For a quick overview, you need these dependencies:

1) A web server, which delivers the files to your players
2) Emscripten (the compiler which compiles the C/C++ code into a .wasm file)
3) SmartGit (well any git tool will do, but this one has the best IDE I've ever used and its free for non-commercial use)

And now one by one:

**1) Install XAMPP**

1) https://www.apachefriends.org/
2) Version doesn't matter too much, I used *XAMPP für Windows 7.1.9 (PHP 7.1.9)*
2) When installing, select at least Apache and PHP
3) Add to PATH: C:\xampp\php

Used for:

1) running the build.php script, which compiles all C/C++ code to object files and then linking it
2) delivering the generated website, which runs the .wasm libwebgame in the browser


**2) Install Emscripten**

https://s3.amazonaws.com/mozilla-games/emscripten/releases/emsdk-portable-64bit.zip

Unpack to 

```
	C:\emsdk-portable-64bit> .\emsdk.bat install git-1.9.4
	C:\emsdk-portable-64bit> .\emsdk.bat update
	C:\emsdk-portable-64bit> .\emsdk.bat install sdk-1.37.21-64bit
	C:\emsdk-portable-64bit> .\emsdk.bat activate sdk-1.37.21-64bit
```
	
Add to PATH:

```
C:\emsdk-portable-64bit
C:\emsdk-portable-64bit\bin
C:\emsdk-portable-64bit\emscripten\1.37.21
C:\emsdk-portable-64bit\node\4.1.1_64bit\bin
C:\emsdk-portable-64bit\python\2.7.5.3_64bit
C:\emsdk-portable-64bit\clang\e1.37.21_64bit
C:\emsdk-portable-64bit\clang\e1.37.21_64bit\binaryen\bin
```
	
In case you get this kind of error when linking the .wasm with emcc:

```
	Traceback (most recent call last):
	  File "C:\emsdk-portable-64bit\emscripten\1.37.1\\emcc", line 13, in <module>
		emcc.run()
	  File "C:\emsdk-portable-64bit\emscripten\1.37.1\emcc.py", line 1683, in run
		wasm_js_glue = open(os.path.join(shared.Settings.BINARYEN_ROOT, 'src', 'js', 'wasm.js-post.js')).read()
	IOError: [Errno 2] No such file or directory: u'/usr/bin\\src\\js\\wasm.js-post.js'
```

Add this to your ENV vars:

```
	BINARYEN = C:\emsdk-portable-64bit\clang\e1.37.21_64bit\binaryen
```




There is a bug in emscripten (some older version, which I still use). Need to search/replace the file library_openal.js with:
 
```
   function _alSource3f(source, param, v1, v2, v3) {
      if (!AL.currentContext) {
        return;
      }
      var src = AL.currentContext.src[source];
      if (!src) {
        AL.currentContext.err = 0xA001 /* AL_INVALID_NAME */;
        return;
      }
      switch (param) {
      case 0x1004 /* AL_POSITION */:
  	  // buggy: src.position has a setter which updates the 
        //src.position[0] = v1;
        //src.position[1] = v2;
        //src.position[2] = v3;
  	  src.position = [v1, v2, v3]
        break;
      case 0x1005 /* AL_DIRECTION */:
        src.direction[0] = v1;
        src.direction[1] = v2;
        src.direction[2] = v3;
        break;
      case 0x1006 /* AL_VELOCITY */:
        src.velocity[0] = v1;
        src.velocity[1] = v2;
        src.velocity[2] = v3;
        break;
      default:
        AL.currentContext.err = 0xA002 /* AL_INVALID_ENUM */;
        break;
      }
    }
```
 
Error boils down to this:
 
```
 anon = {
	_position: [0,0,0],

	get position() {
		console.log("get...")
		return this._position;
	},
	
	set position(val) {
		console.log("set...")
		this._position[0] = val[0];
		this._position[1] = val[1];
		this._position[2] = val[2];
	}
}

anon[0]
anon[0] = 123
```
 
emscripten uses a setter to update the linked NodePanner, but it is never called (instead, the getter was called)


**3) Install SmartGit**

1) http://www.syntevo.com/smartgit/
2) Add to PATH: C:\Program Files (x86)\SmartGit\git\mingw32\bin

Used for:

1) Easy contributing via visual git interface
2) emsdk needs a git.exe to fetch the latest available versions
