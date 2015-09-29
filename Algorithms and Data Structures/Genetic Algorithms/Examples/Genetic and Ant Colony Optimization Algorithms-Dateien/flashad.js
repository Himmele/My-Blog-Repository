var FlashDetected = 0;
if (navigator.mimeTypes && navigator.mimeTypes["application/x-shockwave-flash"] &&
	navigator.mimeTypes["application/x-shockwave-flash"].enabledPlugin) {
	if (navigator.plugins && navigator.plugins["Shockwave Flash"])
		FlashDetected  = 1;
}
else if (navigator.userAgent && navigator.userAgent.indexOf("MSIE")>=0 &&
	(navigator.userAgent.indexOf("Windows 95")>=0 || 
	 navigator.userAgent.indexOf("Windows 98")>=0 || 
	 navigator.userAgent.indexOf("Windows NT")>=0)) {
	document.write('<SCRIPT LANGUAGE=VBScript\> \n');
	document.write('on error resume next \n');
	document.write('FlashDetected  = ( IsObject(CreateObject("ShockwaveFlash.ShockwaveFlash.3")))\n');
	document.write('if ( FlashDetected  <= 0 ) then FlashDetected  = ( IsObject(CreateObject("ShockwaveFlash.ShockwaveFlash.4")))\n');
	document.write('if ( FlashDetected  <= 0 ) then FlashDetected  = ( IsObject(CreateObject("ShockwaveFlash.ShockwaveFlash.5")))\n');
	document.write('</SCRIPT\> \n');

}

function FlashAdText(FlashAdMovie, FlashAdRedirURL, FlashAdAltImage, FlashAdSizeX, FlashAdSizeY)
{
	var FlashAdText = '';
	if ( FlashDetected  ) {
		FlashAdText += '<OBJECT classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"';
		FlashAdText += '  codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=3,0,0,0" ';
		FlashAdText += '  WIDTH=' + FlashAdSizeX + ' HEIGHT=' + FlashAdSizeY + '>';
		FlashAdText += '<PARAM NAME=movie VALUE="' + FlashAdMovie + '?clickTAG=' + FlashAdRedirURL + '&target=_blank">';
		FlashAdText += '<PARAM NAME=quality VALUE=high>'; 
		FlashAdText += ' <EMBED src="' + FlashAdMovie + '?clickTAG=' + FlashAdRedirURL + '&target=_blank" quality=high';
		FlashAdText += ' WIDTH=' + FlashAdSizeX + ' HEIGHT=' + FlashAdSizeY;
		FlashAdText += ' TYPE="application/x-shockwave-flash" PLUGINSPAGE="http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash">';
		FlashAdText += ' </EMBED>';
		FlashAdText += ' </OBJECT>';
	} else if (!(navigator.appName && navigator.appName.indexOf("Netscape")>=0 && navigator.appVersion.indexOf("2.")>=0)){
		FlashAdText += '<a href="' + FlashAdRedirURL + '"><IMG SRC="' + FlashAdAltImage + '" WIDTH=' + FlashAdSizeX + ' HEIGHT=' + FlashAdSizeY + ' BORDER=0></a>';
	}
	return FlashAdText;
}
