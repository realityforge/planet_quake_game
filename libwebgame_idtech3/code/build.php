#!php
<?php
	function cc() {
		//return "c:\\\\MinGW\\\\bin\\\\g++.exe";
		//return "c:\\\\MinGW\\\\bin\\\\mingw32-g++.exe";
		//return "\"C:\\\\Program Files (x86)\\\\CodeBlocks\\\\MinGW\\\\bin\\\\mingw32-g++.exe\"";
		//return "C:/msys64/mingw64/bin/g++.exe";
		return "emcc";
		//return "cl /nologo /EHsc";
	}
	
	function mkdir_($path) {
		@mkdir($path, 0777, true);
	}

	function needsRebuild($object, $source) {
		if ( ! file_exists($object)) return true;
		if ( ! file_exists($source)) return true;
		//echo sprintf("object=$object source=$source %d %d\n", filemtime($object),  filemtime($source));
		return filemtime($object) < filemtime($source);
	}
	
	$files_success = 0;
	$files_error = 0;
	$failedCommands = []; // like ["emcc ...", "emcc ..."]
	
	class Lib {
		var $files = array();
		var $archive = "";
		var $to;
		var $options;
		var $ignore = array();
		var $forceRebuild = false;
		var $uber;
		var $allNeededObjectFiles = array(); // without ignored files, will be accessed by Target to link the .exe
		function to($to)				{ $this->to = $to;										return $this;	}
		function options($options)		{ $this->options .= " $options";						return $this;	}
		function makeStaticLib($archive){ $this->archive = $archive;							return $this;	}
		function ignoreFiles($ignore)	{ $this->ignore = array_merge($this->ignore, $ignore);	return $this;	}
		function OPTION($option)		{ $this->options .= " $option ";						return $this;	}
		function I($path)				{ return $this->options(" -I$path ");									}
		function D($keyval)				{ return $this->options(" -D$keyval ");									}
		function forceRebuild($state)	{ $this->forceRebuild = $state;							return $this;	}
		function buildArchive()			{ return $this->archive != "";											}
		function __toString()			{ return "(Lib archive={$this->archive} to={$this->to})";				}
		function c_to_o($c_filename) {
			//return $this->to . "/" . str_replace(".c", ".o", basename($c_filename));
			return $this->to . "/" . basename($c_filename) . ".obj";
			//echo "c_filename: $c_filename\n";
			//return $this->to . "/" . $c_filename . ".o";
		}
		function buildFile($file) {
			if (in_array(basename($file), $this->ignore)) {
				echo "(Ignore $file)\n";
				return;
			}
			$output = $this->c_to_o($file);
			$this->allNeededObjectFiles[] = $output;
		
			mkdir_(dirname($output));
			// MICROSOFT COMPILER: $cmd = cc() . " {$this->options} /Fo$output /c $file";
			$cmd = cc() . " {$this->options} -o $output -c $file";
			//echo "\n\nCMD: $cmd\n\n";
			$build = needsRebuild($output, $file) || $this->forceRebuild;
			$state = $build ? "BUILD" : " SKIP ";
			echo "(Compile $state CC $file -o $output)\n";
			if ( ! $build)
				return;
			system($cmd);
			$compilationSucceeded = file_exists($output);
			
			if ($compilationSucceeded) {
				$GLOBALS["files_success"]++;
			} else {
				echo "ERROR: CMD: $cmd\n";
				$GLOBALS["failedCommands"][] = $cmd;
				$GLOBALS["files_error"]++;
			}
			
			//if ($this->buildArchive() && $compilationSucceeded) {
			//		$destination = /*$this->to . "/" . */$this->archive;
			//		$cmd = "emar rc $destination $output";
			//		echo "(Archive $cmd)\n";
			//		system($cmd);
			//}
		}
		function build() {
			foreach ($this->files as $file)
				$this->buildFile($file);
		}
		function addFiles($glob) {
			$newFiles = glob($glob);
			if (count($newFiles) == 0)
				print("Warning: no files for $glob\n");
			// a folder might have *.cpp extension, so filter to files only
			$newFiles = array_filter($newFiles, 'is_file');
			$this->files = array_merge($this->files, $newFiles);
			//print("Found files: {$this->to}: " . count($this->files) . "\n");
			return $this;
		}
	}
	
	class Target {
		var $libs = array();
		var $name;
		function Target($name) { $this->name = $name; }
		function __toString() { return "(Target {$this->name})"; }
		function addLib($lib) {
			$lib->uber = $this;
			$lib->options("-O2");
			array_push($this->libs, $lib);
			return $this;
		}
		function build() {
			foreach ($this->libs as $lib)
				$lib->build();
			//call_user_func($finalCallback, $this);
			return $this;
		}
		function link($options = "") {
			$deps = "";
			foreach ($this->libs as $lib)
				if ($lib->archive != "")
					$deps[] = $lib->to . "/" . $lib->archive;
				else
					foreach ($lib->allNeededObjectFiles as $obj)
						$deps[] = "$obj ";
						
			$objects = implode(" ", $deps);
			
			$rebuild = "SKIP";
			foreach ($deps as $dep)
				if (needsRebuild($this->name . ".exe", $dep)) {
					$rebuild = "DOIT";
					echo "(LINK \"Need rebuild because $dep is newer than {$this->name}.exe!\")\n";
				}
			
			//echo "objects: $objects\n";
			//$cmd = cc() . " -Wl,--whole-archive $objects -o " . $this->name . ".exe" . " $options ";
			$cmd = cc() . " $objects -o " . $this->name . ".exe" . " $options";
			echo "(LINK $rebuild $cmd)\n";
			if ($rebuild == "DOIT")
				system($cmd);
		}
		function forceRebuild() {
			foreach ($this->libs as $lib)
				$lib->forceRebuild(1);
			return $this;
		}
	}
	
	class LibGame extends Lib {
		function finish() {
			//$this->forceRebuild = true;

			//$this->options("-std=c++11");
			//$this->options("-Wno-deprecated-declarations");
			//$this->options("-O1"); // optimize for speed and size
			//$this->options("-std=c++14"); // otherwise intptr error stuff
            //
			//$this->options("-fno-tree-vectorize");
			//$this->options("-m64");
			$this->I(".");
			//$this->I("duktape");
			//$this->I("-Wno-write-strings");
			//$this->I("-fpermissive");
			// nope: -DFEATURE_LUASQL -DFEATURE_OMNIBOT -DBUNDLED_LUA -DFEATURE_LUA
			//$this->options("-DAL_LIBTYPE_STATIC -DBUNDLED_GLEW -DBUNDLED_SDL -DCURL_STATICLIB -DFEATURE_ANTICHEAT -DFEATURE_AUTOUPDATE -DFEATURE_DBMS -DFEATURE_EDV -DFEATURE_FREETYPE -DFEATURE_GETTEXT -DFEATURE_IRC_CLIENT -DFEATURE_IRC_SERVER -DFEATURE_MULTIVIEW -DFEATURE_OGG_VORBIS -DFEATURE_OPENAL -DFEATURE_RATING -DFEATURE_SERVERMDX -DFEATURE_THEORA -DGAMEDLL -DGLEW_STATIC -DHAVE_SDL -DOC_X86_ASM -DSQLITE_HAVE_ISNAN=1 -DSQLITE_THREADSAFE=0 -DUSE_RENDERER_DLOPEN -Dqagame_mp_i386_EXPORTS -m32");
			
			return $this;
		}
	}
	
	$targets = new Target("game");
	
	$lib_client = new Lib();
	$lib_client->to("build/libwebgame")->addFiles("*.c")->makeStaticLib("build/libwebgame.a")->I(".")->I("kung");
	$targets->addLib($lib_client);
	
	$lib_client_cpp = new Lib();
	$lib_client_cpp->to("build/libwebgame")->addFiles("*.cpp")->makeStaticLib("build/libwebgame.a")->I(".")->options("-std=c++14");
	$targets->addLib($lib_client_cpp);
	
	$targets->build();
	
	printf("Stats: success=%d errors=%d\n", $GLOBALS["files_success"], $GLOBALS["files_error"]);
		
	// creates a .wast file which is > 70MB, which slows the loading down to like 20 seconds
	//system('emcc build/duktape.a build/sys.a build/sdl.a build/imgui.a build\kung.a build/qcommon.a build/renderergl2.a build/renderercommon.a build/client.a build/server.a build/glsl.a build/game.a build/cgame.a build/zlib.a build/jpeg-8c.a -o c:\xampp\htdocs\webquake\webquake.html -s WASM=1 -g -s TOTAL_MEMORY=209715200 -s ASSERTIONS=2 -s LEGACY_GL_EMULATION=0'); // == 32*1024*1024, 64=67108864, 100=104857600, 200=209715200
	
	//$outdir = "c:/xampp/htdocs/WebExe/build_a";
	//$outdir = "c:/xampp/htdocs/WebFiles";
	$outdir = "c:/xampp/htdocs/libwebgame_assets";
	
	if (strtoupper(substr(PHP_OS, 0, 3)) != 'WIN') {
		$outdir = "../../public_html/originaloutput";
		//$outputfile = "/home/emscripten/public_html/libwebgame/webquake.html";
		//$outputfile = "/var/www/sheepshooter/webquake.html";
		//$outputfile = "/var/www/imgui/originaloutput/originaloutput.html";
		//$outputfile = "/mnt/c/linux/varwww/imgui/originaloutput/originaloutput.html";
	}

	mkdir_($outdir);
	
	// rebuild libwebgame.a each time
	$objs = implode(" ", $lib_client_cpp->allNeededObjectFiles);
	//echo $objs;
	unlink("build/libwebgame.a");
	system("emar rc build/libwebgame.a $objs");
	
	$linkcmd = 'emcc build/libwebgame.a -o libwebgame.js -s WASM=1 -s TOTAL_MEMORY=268435456 -O2 -g';
	echo "$linkcmd\n";
	system($linkcmd, $retlinking); // == 32*1024*1024, 64=67108864, 100=104857600, 200=209715200, 256=268435456
	echo "retlinking=$retlinking\n";
	
	foreach ($GLOBALS["failedCommands"] as $cmd) {
		echo "Failed command: " . $cmd . "\n";
	}
	
	$libwebgame = file_get_contents("libwebgame.js");
	$libwebgame = str_replace("exit(ret, /* implicit = */ true);", "// DO NOT QUIT", $libwebgame);
	file_put_contents("libwebgame.js", $libwebgame);

	function copy_into_proper_folders() {
		if (is_dir("../libwebgame_html5/")) {
			echo "COPY INTO ../libwebgame_html5/\n";
			copy("libwebgame.js"  , "../libwebgame_html5/libwebgame.js");
			copy("libwebgame.wasm", "../libwebgame_html5/libwebgame.wasm");
			copy("libwebgame.wast", "../libwebgame_html5/libwebgame.wast");
		}
		
		if (is_dir("../../libwebgame_html5/")) {
			echo "COPY INTO ../../libwebgame_html5/\n";
			copy("libwebgame.js"  , "../../libwebgame_html5/libwebgame.js");
			copy("libwebgame.wasm", "../../libwebgame_html5/libwebgame.wasm");
			copy("libwebgame.wast", "../../libwebgame_html5/libwebgame.wast");
		}
		
		if (is_dir("../libwebgame_node/")) {
			echo "COPY INTO ../libwebgame_node/\n";
			copy("libwebgame.js"  , "../libwebgame_node/libwebgame.js");
			copy("libwebgame.wasm", "../libwebgame_node/libwebgame.wasm");
			copy("libwebgame.wast", "../libwebgame_node/libwebgame.wast");
		}

		if (is_dir("../../public_html/libwebgame_html5/")) {
			echo "COPY INTO ../../public_html/libwebgame_html5/\n";
			copy("libwebgame.js"  , "../../public_html/libwebgame_html5/libwebgame.js");
			copy("libwebgame.wasm", "../../public_html/libwebgame_html5/libwebgame.wasm");
			copy("libwebgame.wast", "../../public_html/libwebgame_html5/libwebgame.wast");
		}
		
		if (is_dir("../../libwebgame_node/")) {
			echo "COPY INTO ../../libwebgame_node/\n";
			copy("libwebgame.js"  , "../../libwebgame_node/libwebgame.js");
			copy("libwebgame.wasm", "../../libwebgame_node/libwebgame.wasm");
			copy("libwebgame.wast", "../../libwebgame_node/libwebgame.wast");
		}
	}

	copy_into_proper_folders();
