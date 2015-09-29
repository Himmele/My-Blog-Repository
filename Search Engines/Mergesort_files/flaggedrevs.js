/* -- (c) Aaron Schulz, Daniel Arnold 2008 */

/* Every time you change this JS please bump $wgFlaggedRevStyleVersion in FlaggedRevs.php */

var FlaggedRevs = {
	'messages': {
		'diffToggleShow'	: '(show changes)',
		'diffToggleHide'	: '(hide changes)',
		'logToggleShow'		: '(show log)',
		'logToggleHide'		: '(hide log)',
		'logDetailsShow'	: '(show details)',
		'logDetailsHide'	: '(hide details)',
		'toggleShow'    	: '(+)',
		'toggleHide'    	: '(-)'
	},
	/* Dropdown collapse timer */
	'boxCollapseTimer': null,
	/* Hide rating/diff clutter */
	'enableShowhide': function() {
		// Rating detail box
		var toggle = document.getElementById('mw-fr-revisiontoggle');
		if( toggle ) {
			toggle.style.display = 'inline';
		}
		// Diff detail box
		toggle = document.getElementById('mw-fr-difftoggle');
		if( toggle ) {
			toggle.style.display = 'inline';
			var diff = document.getElementById('mw-fr-stablediff');
			if( diff ) {
				diff.style.display = 'none';
			}
		}
		// Log detail box
		toggle = document.getElementById('mw-fr-logtoggle');
		if( toggle ) {
			toggle.style.display = 'inline';
			var log = document.getElementById('mw-fr-logexcerpt');
			if( log ) {
				log.style.display = 'none';
			}
		}
	},
	
	/* Expands flag info box details */
	'showBoxDetails': function() {
		var ratings = document.getElementById('mw-fr-revisiondetails');
		if( ratings ) {
			ratings.style.display = 'block';
		}
	},
	
	/* Collapses flag info box details */
	'hideBoxDetails': function( event ) {
		var ratings = document.getElementById('mw-fr-revisiondetails');
		if( ratings ) {
			ratings.style.display = 'none';
		}
	},
	
	/* Toggles flag info box details for (+/-) control */
	'toggleBoxDetails': function() {
		var toggle = document.getElementById('mw-fr-revisiontoggle');
		if( !toggle ) return;
		var ratings = document.getElementById('mw-fr-revisiondetails');
		if( !ratings ) return;
		// Collapsed -> expand
		if( ratings.style.display == 'none' ) {
			this.showBoxDetails();
			toggle.innerHTML = this.messages.toggleHide;
		// Expanded -> collapse
		} else {
			this.hideBoxDetails();
			toggle.innerHTML = this.messages.toggleShow;
		}
	},

	/* Expands flag info box details on mouseOver */
	'onBoxMouseOver': function( event ) {
		window.clearTimeout( this.boxCollapseTimer );
		this.boxCollapseTimer = null;
		this.showBoxDetails();
	},

	/* Hides flag info box details on mouseOut *except* for event bubbling */
	'onBoxMouseOut': function( event ) {
		if( !this.isMouseOutBubble( event, 'mw-fr-revisiontag' ) ) {
			this.boxCollapseTimer = window.setTimeout( this.hideBoxDetails, 150 );
		}
	},
	
	/* Checks is mouseOut event is for a child of parentId */
	'isMouseOutBubble': function( event, parentId ) {
		var toNode = null;
		if( event.relatedTarget != undefined ) {
            toNode = event.relatedTarget; // FF/Opera/Safari
        } else {
            toNode = event.toElement; // IE
        }
		if( toNode ) {
			var nextParent = toNode.parentNode;
			while( nextParent ) {
				if( nextParent.id == parentId ) {
					return true; // event bubbling
				}
				nextParent = nextParent.parentNode; // next up
			}
		}
		return false;
	},
	
	/* Toggles diffs */
	'toggleDiff': function() {
		var diff = document.getElementById('mw-fr-stablediff');
		if( !diff ) return;
		var toggle = document.getElementById('mw-fr-difftoggle');
		if( !toggle ) return;
		if( diff.style.display == 'none' ) {
			diff.style.display = 'block';
			toggle.getElementsByTagName('a')[0].innerHTML = this.messages.diffToggleHide;
		} else {
			diff.style.display = 'none';
			toggle.getElementsByTagName('a')[0].innerHTML = this.messages.diffToggleShow;
		}
	},
	
	/* Toggles log excerpts */
	'toggleLog': function() {
		var log = document.getElementById('mw-fr-logexcerpt');
		if( !log ) return;
		var toggle = document.getElementById('mw-fr-logtoggle');
		if( !toggle ) return;
		if( log.style.display == 'none' ) {
			log.style.display = 'block';
			toggle.getElementsByTagName('a')[0].innerHTML = this.messages.logToggleHide;
		} else {
			log.style.display = 'none';
			toggle.getElementsByTagName('a')[0].innerHTML = this.messages.logToggleShow;
		}
	},
	
	/* Toggles log excerpts */
	'toggleLogDetails': function() {
		var log = document.getElementById('mw-fr-logexcerpt');
		if( !log ) return;
		var toggle = document.getElementById('mw-fr-logtoggle');
		if( !toggle ) return;
		if( log.style.display == 'none' ) {
			log.style.display = 'block';
			toggle.getElementsByTagName('a')[0].innerHTML = this.messages.logDetailsHide;
		} else {
			log.style.display = 'none';
			toggle.getElementsByTagName('a')[0].innerHTML = this.messages.logDetailsShow;
		}
	}
};

FlaggedRevs.setCheckTrigger = function() {
	var checkbox = document.getElementById("wpReviewEdit");
	if( checkbox ) {
		checkbox.onclick = FlaggedRevs.updateSaveButton;
	}
};

FlaggedRevs.updateSaveButton = function() {
	var checkbox = document.getElementById("wpReviewEdit");
	var save = document.getElementById("wpSave");
	if( checkbox && save ) {
		// Review pending changes
		if ( checkbox.checked ) {
			save.value = FlaggedRevs.messages.saveArticle;
			save.title = FlaggedRevs.messages.tooltipSave;
		// Submit for review
		} else {
			save.value = FlaggedRevs.messages.submitArticle;
			save.title = FlaggedRevs.messages.tooltipSubmit;
		}
	}
};

FlaggedRevs.getRevisionContents = function() {
	//get the contents div and replace it with actual parsed article contents via an API call.
	var contentsDiv = document.getElementById("mw-fr-revisioncontents");
	var prevLink = document.getElementById("differences-prevlink");
	var nextLink = document.getElementById("differences-nextlink");
	var timeoutId = null;
	if( contentsDiv ) {
		var diffUIParams = document.getElementById("mw-fr-diff-dataform");
		var oldRevId = diffUIParams.getElementsByTagName('input')[1].value;
		var origContents = contentsDiv.innerHTML;
		contentsDiv.innerHTML = "<span class='loading mw-small-spinner spinner'></span><span class='loading' >" + wgRevContents.waiting + "</span>";
		var requestArgs = 'action=parse&prop=text|categorieshtml|languageshtml&format=xml';
		if ( window.wgCurRevisionId == oldRevId && window.wgPageName ) {
			requestArgs += '&page=' + encodeURIComponent( window.wgPageName );
		} else {
			requestArgs += '&oldid=' + oldRevId;
		}

		var call = jQuery.ajax({
				url		: wgScriptPath + '/api.php',
				type	: "GET",
				data	: requestArgs,
				dataType: "xml",
				success	: function( result ) {
					contentsDiv.innerHTML = "";
					contents = jQuery(result).find("text");
					if ( contents && contents.text() ) {
						contentsDiv.innerHTML += contents.text();
					} else {
						contentsDiv.innerHTML = wgRevContents.error + " " + origContents;
					}
					categoryhtml = jQuery(result).find("categorieshtml");
					if ( categoryhtml && categoryhtml.text() ) {
						contentsDiv.innerHTML += categoryhtml.text();
					}
					languageshtml = jQuery(result).find("languageshtml");
					if ( languageshtml && languageshtml.text() ) {
						contentsDiv.innerHTML += "<div class='langlinks' >" + languageshtml.text() + "</div>";
					}
					
				},
				error	: function(xmlHttpRequest, textStatus, errThrown) {
					contentsDiv.innerHTML = wgRevContents.error + " " + origContents;
				}
			});
	}
	if ( prevLink ) {
		prevLink.onclick = function() {
			if ( call ) {
				call.abort();
			}
		};
	}
	if ( nextLink ) {
		nextLink.onclick = function() {
			if ( call ) {
				call.abort();
			}
		};
	}
};

FlaggedRevs.setJSTriggers = function() {
	FlaggedRevs.enableShowhide();
	FlaggedRevs.setCheckTrigger();
	FlaggedRevs.getRevisionContents();
};

window.onload = FlaggedRevs.setJSTriggers;
