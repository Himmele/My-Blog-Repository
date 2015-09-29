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


//var artURL = "http://h70-2.raleigh.ibm.com/developerworks/search/dWSearchRouter.jsp?query=";
var artURL = "http://www.ibm.com/developerworks/search/dWSearchRouter.jsp?query=";

//var codeURL = "http://h70-2.raleigh.ibm.com/developerworks/search/krugleRouter.jsp?query=";
var codeURL = "http://www.ibm.com/developerworks/search/krugleRouter.jsp?query="; 

var artDIV = "article_results";
var codeDIV = "code_results";
var referDIV = "search_referrer_results";

var globalQuery;



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

		reqA.open("GET", artURL + query, true);
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

		reqC.open("GET", codeURL + query, true);
		reqC.onreadystatechange = getReadyStateHandler(reqC, processCodeResults);
		reqC.send("");
 	}

} // end of method: codesearch


function processArticleResults(result) 
{
	//alert("ARTICLE RESULTs");
	//alert("ARTICLE DATA - " + result);
   
	var contents = document.getElementById(artDIV);
	var referrer = document.getElementById(referDIV); 
   
	if (result != null) 
	{ 
		//alert("PUT");
	
		var displayQuery = globalQuery;
		displayQuery = displayQuery.replace(/\+/g, " ");
		displayQuery = unescape(displayQuery);

		contents.innerHTML = "" + result;
	
		//alert("Satish is a good guy");
		//alert("ARTICLE DIV - " + contents.innerHTML);

		//alert(displayQuery);

		referrer.style.display = 'block';
	}

} // end of method: processArticleResults


function processCodeResults(result) 
{
	//alert("CODE RESULTs");
	//alert("CODE DATA - " + result);
   
	var contents = document.getElementById(codeDIV); 
	var referrer = document.getElementById(referDIV);
   
	if (result != null) 
	{ 
		//alert("PUT");
	
		var displayQuery = globalQuery;
		displayQuery = displayQuery.replace(/\+/g, " ");
		displayQuery = unescape(displayQuery);

		contents.innerHTML = "" + result;
	
		//alert("Satish is a good guy");
		//alert("CODE DIV - " + contents.innerHTML);

		//alert(displayQuery);

		referrer.style.display = 'block';
	}

} // end of method: processCodeResults


// END - HIDE SCRIPT from disabled or old browsers -->
