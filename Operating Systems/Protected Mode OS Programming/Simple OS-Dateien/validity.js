<!--

// Setcomputing(r) 
// Programmer: Salman Quazi


// Start: Global System Variables
var isV5 = false;
var isIE = false;
var n = 0;
var messages = new Array(
	"Return to Home Page",
	"Search Setcomputing.com",
	"", // was deleted from the menu due to limitation of space
	"Assembler",
	"",
	"",
	"Get your (any) servers current status",
	"",
	"ASCII table",
	"Download/View a copy of the authors Resume",
	"About the author",
	"Read about Algorithms",
	"Learn more about C and C++",
	"Learn more about The C++ Standard Template Library",
	"Read about ASP and SQL (w/ TSQL)",
	"Concepts of Compiler Design",
 	"Search this page",
	"Perform Base Conversions",
	"Bitwise operations",
	"Read highly informative and interesting news from across the globe!"
)
// End: Global Variables

if (parseInt(navigator.appVersion) >= 5) isV5 = true;
if (navigator.appVersion.indexOf("MSIE") != -1) isIE = true;

function MouseOutMenu(Item) {
	if (isV5 && isIE) {
		var MenuItem = document.getElementById(Item);
		MenuItem.setAttribute("style", "");
	}
	eval(Item + ".style.borderColor = '#E7E7E7';");	
	window.status='';
}
function MouseOverMenu(Item) {
	if (isV5 && isIE) {
		var MenuItem = document.getElementById(Item);
		MenuItem.setAttribute("style", "border-width:2px;border-style:solid;border-color:#6699CC;");
		alert(MenuItem.nodeName());
	}
	eval(Item + ".style.borderColor = '#6699CC';");
	window.status = messages[(Item.substr(3,Item.length))-1];
}
function findinpage(str) {
	if (str == "" || str==null) return false;

	if (document.layers) {
		if (!window.find(str)) {while(window.find(str, false, true)) n++;}
		else n++;
		if (n == 0) alert("The word \""+ str +"\" was not found on this page.");
	}
	if (document.all) {
		var txt = window.document.body.createTextRange();
		var found = txt.findText(str);

		for (var i = 0; i <= n && found != false; i++) {
			txt.moveStart("character", 1);
			txt.moveEnd("textedit");
		}
		if (found) {
			txt.moveStart("character", -1);
			txt.findText(str);
			txt.select();
			txt.scrollIntoView();
			n++;
		}
		else {
			if (n > 0) {
				n = 0;
				findinpage(str);
			}
			else alert("The word \""+ str +"\" was not found on this page.");
		}
		return false;
	}
}
function showRef() {
	var cmd="http://foldoc.doc.ic.ac.uk/foldoc/foldoc.cgi?query=";
	var usr_cmd;
	try {
		usr_cmd=window.prompt("Enter a computing based keyword for lookup ","<enter keyword here>");
		if(usr_cmd==null) throw "null_error";
	}
	catch(er) {
		if(er == "null_error") { return; }
	}
	if(usr_cmd == "<enter keyword here>") usr_cmd="&action=Random";
	
	cmd+=usr_cmd;	
	window.location.href=cmd;
}
//-->