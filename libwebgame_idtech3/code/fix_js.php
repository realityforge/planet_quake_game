<?php
	function generateOriginalOutputFixed($path)
	{
		$originalFilename = "$path/originaloutput.js";
		if ( ! file_exists($originalFilename))
			return;
		$script = file_get_contents($originalFilename);
		
		// original code looks like: 
		// var wasmTextFile = Module['wasmTextFile'] || 'originaloutput.wast';
		// var wasmBinaryFile = Module['wasmBinaryFile'] || 'originaloutput.wasm';
		// var asmjsCodeFile = Module['asmjsCodeFile'] || 'originaloutput.temp.asm.js';
		// wanna replace wasmTextFile and wasmBinaryFile, so it is using a global variable name, to remove the hardcoded paths
		
		//$lastChange = filemtime("$folder/build_a/originaloutput.wasm");
		//$script = str_replace("originaloutput.wasm", "$folder/build_a/originaloutput.wasm?" . $lastChange, $script); // force loading a new version whenever there is one
		
		
		$script = str_replace("var wasmTextFile", "//var wasmTextFile", $script);
		$script = str_replace("var wasmBinaryFile", "//var wasmBinaryFile", $script);
		
		$script = str_replace("run();", "myrun();", $script);
		$script = str_replace("if (Module['setStatus'])", "if (0)", $script);
		
		file_put_contents("$path/originaloutput_fixed.js", $script);
	}
	
	function renameFileToHash($filename)
	{
		if ( ! file_exists($filename))
			return false;
		$hash = md5_file($filename);
		$dirname = dirname($filename);
		$basename = basename($filename);
		$renameTo = $dirname . "/" . $hash . "_" . $basename; // something like originaloutput_fixed_.js
		if (file_exists($renameTo))
			unlink($filename);
		else
			rename($filename, $renameTo);
	}
	
	function generateFilesJson($path) {
		$files = glob("$path/*.*");
		$entries = [];
		foreach ($files as $file) {
			$entries[] = array(
				"filename" => basename($file),
				"hash" => md5_file($file)
			);
		}
		$files_json = json_encode($entries, JSON_PRETTY_PRINT);
		file_put_contents("$path/files.json", $files_json);
		echo $files_json;
	}
	
	//generateOriginalOutputFixed(".");
	//generateFilesJson("")
	
	// this works, but sux, i rather write the metadata md5 hash in files.json
	//renameFileToHash("originaloutput_fixed.js");
	//renameFileToHash("originaloutput.html");
	//renameFileToHash("originaloutput.js");
	//renameFileToHash("originaloutput.wasm");
	//renameFileToHash("originaloutput.wast");