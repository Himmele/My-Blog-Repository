<!-- START HIDE SCRIPT from disabled or old browsers

// **************************************************
// * FILE:  search_counter.js
// * DATE:  02.26.2008
// * 
// * AUTHOR: David Salinas
// *
// * Property of IBM. Copyright 2008
// * 
// **************************************************


//var artURL = "http://www.ibm.com/developerworks/secure/dWGoogleSearchMaverickRouter.jsp?query=";
var artURL = "http://www.ibm.com/developerworks/utils/dWGoogleSearchMaverickRouter.jsp?query=";

//var codeURL = "http://h70-2.raleigh.ibm.com/developerworks/search/krugleRouter.jsp?query=";
var codeURL = "http://www.ibm.com/developerworks/search/krugleMaverickRouter.jsp?query="; 

var artDIV = "article_results";
var codeDIV = "code_results";
var referDIV = "search_referrer_results";

var globalQuery;

if(typeof dwCommon!="object"||dwCommon==null){dwCommon={};}
dwCommon.initSimpleShowHide=function(B){var C=B instanceof jQuery?B:jQuery(B);if(!C.hasClass("ibm-simple-show-hide")){return 
}var A=jQuery("div.ibm-hideable",C).slideUp();jQuery("p.ibm-show-hide-controls",C).css("display","block").after('<div class="ibm-rule">').find("a").click(function(){var D=jQuery(this);D.parent().find("a").removeClass("ibm-active").end().end().addClass("ibm-active");if((D.attr("href")==="#show") || (D.attr("href").indexOf("#show")!==-1)){A.slideDown()}else{if( (D.attr("href")==="#hide")||(D.attr("href").indexOf("#hide")!==-1)){A.slideUp()}}return false})};



function searchQuery(query) 
{	
	//alert("TERMS: " + query);     
	
	articlesearch(query);

	// KRUGLE RETIRED - 20081209
	//codesearch(query); 
	// KRUGLE RETIRED - 20081209
	
} // end of method: searchQuery


function articlesearch(query)
{
	globalQuery = query;      
	
	//alert("ARTICLE TERMS: " + query);     
 
	var reqA = newXMLHttpRequest(); 

	if (reqA) 
	{
		//alert("ARTICLE REQUEST");

		reqA.open("GET", artURL + query + "&rn=" + Math.random(), true);
		reqA.onreadystatechange = getReadyStateHandler(reqA, processArticleResults);
		reqA.send("");
 	}

} // end of method: articlesearch


function codesearch(query)
{
	globalQuery = query;      
	
	//alert("CODE TERMS: " + query);     
 
	var reqC = newXMLHttpRequest(); 

	if (reqC) 
	{
		//alert("CODE REQUEST");

		reqC.open("GET", codeURL + query + "&rn=" + Math.random(), true);
		reqC.onreadystatechange = getReadyStateHandler(reqC, processCodeResults);
		reqC.send("");
 	}

} // end of method: codesearch


function processArticleResults(result) 
{
	//alert("ARTICLE RESULTs");
	//alert("ARTICLE DATA - " + result);
   
	var contents = document.getElementById(artDIV);
	//var referrer = document.getElementById(referDIV); 
   
	if (result != null) 
	{ 
		//alert("PUT");
	
		var displayQuery = globalQuery;
		displayQuery = displayQuery.replace(/\+/g, " ");
		displayQuery = unescape(displayQuery);

		contents.innerHTML = "" + result;
		jQuery("#dw-related-searches-article").css("display","block");
		ibmCommon.initTwisty(jQuery('#dw-content-twisty'));
		// following line was trying to fix show hide in ie and did not seem to work
		dwCommon.initSimpleShowHide(jQuery('#dw-content-showhide'));
	         

		//alert("ARTICLE DIV - " + contents.innerHTML);

		//alert(displayQuery);

		//referrer.style.display = 'block';
	}

} // end of method: processArticleResults


function processCodeResults(result) 
{
	//alert("CODE RESULTs");
	//alert("CODE DATA - " + result);
   
	var contents = document.getElementById(codeDIV); 
	//var referrer = document.getElementById(referDIV);
   
	if (result != null) 
	{ 
		//alert("PUT");
	
		var displayQuery = globalQuery;
		displayQuery = displayQuery.replace(/\+/g, " ");
		displayQuery = unescape(displayQuery);

		contents.innerHTML = "" + result;
		ibmCommon.initTwisty(jQuery('#dw-code-twisty'));
		// following line was trying to fix show hide in ie and did not seem to work
		dwCommon.initSimpleShowHide(jQuery('#dw-code-showhide'));
	        //alert("CODE DIV -" + contents.innerHTML + "--" );
		

		//alert(displayQuery);

		//referrer.style.display = 'block';
	}

} // end of method: processCodeResults

// END  HIDE SCRIPT from disabled or old browsers 