#!/usr/bin/php
<?
  //DynConf - © Adam Wright 2000
  //V1.0
  //Generically find all be_ passed vars and echo them with their content
  //Send header
  echo "//<PRE>\n";
  echo "//Beryllium Config File (From DynConf V1.0)\n";
  echo "//Save this file to your Beryllium directory and type \"exec MYFILENAME\" to use it\n";
  
  if (empty($HTTP_POST_VARS)) 
    die("Please use the form from the main webpage to call this script<BR>");
  for (reset($HTTP_POST_VARS); list($key, $value)=each($HTTP_POST_VARS);) {
    if (substr($key, 0, 3)=="be_")
      echo "$key $value\n";
  }

  echo "//End of Beryllium Config</PRE>";
?>