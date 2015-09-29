
function sphinnit() {
	var check = window.parent.submit_url;
	if(!check) {
		var url1 = document.URL;
	} else {
		var url1 = window.parent.submit_url;
	}
	
	
	var url2 = 'http://sphinn.com/evb/vurl.php?url='+url1;
	document.write('<iframe name="sphinnit" width="50" height="68" scrolling="no" frameborder="0" allowtransparency="true" src="'+url2+'"></iframe>');

	}

sphinnit();