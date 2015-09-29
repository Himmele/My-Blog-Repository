<!-- START HIDE SCRIPT from disabled or old browsers

// **************************************************
// * FILE:  request_referrer_capture.js
// * DATE:  02.26.2008
// * 
// * AUTHOR: David Salinas
// *
// * Property of IBM. Copyright 2008
// * 
// **************************************************



function capture_referrer()
{
   //var referrer = "http://www.google.com/search?hl=en&q=java+api&btnG=Google+Search";
   //var referrer = "http://search.yahoo.com/search?p=java+api&fr=yfp-t-344&toggle=1&cop=mss&ei=UTF-8";
   //var referrer = "http://www.ibm.com/Search/?q=java+api&v=16&en=utf&lang=en&cc=us&Search=Search";
   //var referrer = "http://www.ibm.com/developerworks/search/searchResults.jsp?searchType=1&searchSite=dW&searchScope=dW&query=java+api&Search=Search";
   

   //alert("REFERRER: " + referrer);

   if (document.referrer && document.referrer != "") 
   { 
   	//document.write(document.referrer);
   	var q = document.referrer;
   	
	//var q = referrer;
	var engine = q;

   	var isG = engine.search(/google\.com\/search/i);
	var isY = engine.search(/yahoo\.com\/search/i);
	var isIBM = engine.search(/ibm\.com\/Search/i);
	var isDW = engine.search(/ibm\.com\/developerworks\/search/i);


   	if (isG != -1) 
   	{
		//alert("PROCESS GOOGLE");
		process_google(q);
   	}
	else if (isY != -1)
	{
		//alert("PROCESS YAHOO");
		process_yahoo(q);
	}
	else if (isIBM != -1)
	{
		//alert("PROCESS IBM");
		process_IBM(q);
	}
	else if (isDW != -1)
	{
		//alert("PROCESS DW");
		process_DW(q);
	}

   } 

} // end of method: capture_referrer



function process_google(query)
{
   var searchTerms;

   var i = query.search(/q=/);
   var q2 = query.substring(i+2);
   var j = q2.search(/&/);

   j = (j == -1) ? q2.length:j;
	   
   searchTerms = query.substring(i+2,i+2+j);
	   
   if (searchTerms.length != 0) 
   {
	searchQuery(searchTerms);
   }
   
} // end of method: process_google



function process_yahoo(query)
{
   var searchTerms;

   var i = query.search(/p=/);
   var q2 = query.substring(i+2);
   var j = q2.search(/&/);

   j = (j == -1) ? q2.length:j;
	   
   searchTerms = query.substring(i+2,i+2+j);
	   
   if (searchTerms.length != 0) 
   {
	searchQuery(searchTerms);
   }
   
} // end of method: process_yahoo



function process_IBM(query)
{
   var searchTerms;

   var i = query.search(/q=/);
   var q2 = query.substring(i+2);
   var j = q2.search(/&/);

   j = (j == -1) ? q2.length:j;
	   
   searchTerms = query.substring(i+2,i+2+j);
	   
   if (searchTerms.length != 0) 
   {
	searchQuery(searchTerms);
   }
   
} // end of method: process_IBM



function process_DW(query)
{
   var searchTerms;

   var i = query.search(/query=/);
   var q2 = query.substring(i+6);
   var j = q2.search(/&/);

   j = (j == -1) ? q2.length:j;
	   
   searchTerms = query.substring(i+6,i+6+j);
	   
   if (searchTerms.length != 0) 
   {
	searchQuery(searchTerms);
   }
   
} // end of method: process_DW


// END - HIDE SCRIPT from disabled or old browsers -->
