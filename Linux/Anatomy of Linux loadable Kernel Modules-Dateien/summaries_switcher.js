<!-- START HIDE SCRIPT from disabled or old browsers

// **************************************************
// * FILE:  summaries_switcher.js
// * DATE:  02.26.2008
// * 
// * AUTHOR: David Salinas
// *
// * Property of IBM. Copyright 2008
// * 
// **************************************************


var resultsCount = 10;

function summaries(div1, div2, name)
{
	var hideDiv = document.getElementById( div1+name );
	var showDiv = document.getElementById( div2+name );

	//var theDiv = document.getElementById( "coderesults" );
	
	hideDiv.style.display = 'none';
	showDiv.style.display = 'block';

	for (var i = 1; i < (resultsCount + 1); i++)
	{
		var sumy = document.getElementById( name+i );

		//alert("HERE " + sumy.id);

		if ( sumy.style.display == 'block' )
		{
			sumy.style.display = 'none';
		}
		else
		{
			sumy.style.display = 'block';	
		}
	}
	

} // end of method: summaries


function show_div(div1, div2)
{
	open_div(div1);
	close_div(div2);
} // end of method: show_div


function open_div(divA)
{
	div_elem1 = document.getElementById(divA);
	div_elem1.style.display = "block";
} // end of method: open_div


function close_div(divB)
{
	div_elem2 = document.getElementById(divB);
	div_elem2.style.display = "none";
} // end of method: close_div


// END - HIDE SCRIPT from disabled or old browsers -->
