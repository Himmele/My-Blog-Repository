function msieversion()
// return Microsoft Internet Explorer (major) version number, or 0 for others.
// This function works by finding the "MSIE " string and extracting the version number
// following the space, up to the decimal point for the minor version, which is ignored.
{
    var ua = window.navigator.userAgent
    var msie = ua.indexOf ( "MSIE " )
    if ( msie > 0 )        // is Microsoft Internet Explorer; return version number
        return parseInt ( ua.substring ( msie+5, ua.indexOf ( ".", msie ) ) )
    else
        return 0    // is other browser
}

function mangleTerm(str)
{
	return str.toLowerCase().replace(/[- ]/, "_") + ".html";
}

function ref()
{
	str = mangleTerm(window.event.srcElement.innerText);
	window.open("popups/" + str, "popup", "scrollbars=1,resizable=1,width=400,height=200");
}

function scanrefs()
{
	//w = window.open("", "words");
	//w.document.clear();
	//w.document.writeln("<h1>Popup Words</h1>");

	if (msieversion() < 4)
		return;

	for (i = 0; i < document.all.length; i++)
	{
		elt = document.all(i);
		if (elt.className == "popup")
		{
			elt.onclick = ref;
			elt.className = "popup_ie";
			/*w.document.writeln("<a href=\"popups/" + 
				mangleTerm(elt.innerText) + 
				"\">" + elt.innerText + 
				"</a><br>");*/
		}
	}
}
