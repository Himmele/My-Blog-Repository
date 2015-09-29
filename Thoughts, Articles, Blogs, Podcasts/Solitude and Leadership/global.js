/* Global Items */
$(document).ready(function() {

	$("#searchTips h3").mouseover(function(){
		$(this).addClass("hovered");
//		$("#searchTips p").addClass("visible");
	}).mouseout(function(){
		$(this).removeClass("hovered");
//		$("#searchTips p").removeClass("visible");
	});


	$("#searchTips").mouseover(function(){
		$("#searchTips p").addClass("visible");
	}).mouseout(function(){
		$("#searchTips p").removeClass("visible");
	});




	// "Special Columnist" pages
	if ($('div.page-columnist').length != 0) {
		$("body").addClass("body-columnist");
		$("body.body-columnist p span").parents("p").remove(); // axe all paragraphs that are ACTUALLY ShareThis paragraphs
		$(".olderColumns .post:first").remove();
	}




/*
	$("#searchTips p").hide();
	

	$("#searchTips h3").click(function(){
		$("#searchTips p").show();
		$(this).addClass("expanded");
	});

	
	$("#searchTips h3.expanded").click(function(){
		alert("to be closed...");
		$("#searchTips p").hide();
		$(this).removeClass("expanded");
	});
*/


});
