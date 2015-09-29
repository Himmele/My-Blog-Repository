/*
	devpubs.js
	TechPubPerlModules
	
	JavaScript library for use by Developer Publications.
	
	Created by David Keith dkeith@apple.com
	Radar Component: RefLib Navigation
	
	Copyright (c) 2009 Apple Inc. All Rights Reserved.

*/


// JSLint Configuration
/*jslint plusplus: false, browser: true */
/*global window location escape localStorage */

// prototype.js
/*global $, $$, Template, Prototype, Hash, Ajax */

// navigation.js
/*global TOC: false */

// HTML File
/*global Book, RefLibCategorization, NavigationPage */


// Set default cookie expire date.
var COOKIE_EXPIRE_DATE = new Date();
COOKIE_EXPIRE_DATE.setFullYear(COOKIE_EXPIRE_DATE.getFullYear() + 1);

Prototype.Browser.Xcode = navigator.userAgent.indexOf('Xcode/') > -1;
Prototype.Browser.XcodeVersion = parseFloat(navigator.userAgent.slice(navigator.userAgent.indexOf('Xcode/') + 6, navigator.userAgent.length)); 

var Links = {
    openInNewWindow: function (linkList) {
		var target, i;
        if (!Prototype.Browser.Xcode || Prototype.Browser.XcodeVersion >= 1577) {
            target = Prototype.Browser.Xcode ? '__xcodeOpenInNewWindow': '_blank';
            for (i = 0; i < linkList.length; i++) {
                if (linkList[i].up().hasClassName('pediaLink')) {
                    linkList[i].href = linkList[i].href.split('#')[0]; //removed anchor hash as this causes document to scroll down
                } else {
                    linkList[i].target = target; // open all other links in new window
                }
            }
        }
        return linkList;
    },
    openInBrowser: function (linkList) {
		var target, i;
        if (!Prototype.Browser.Xcode || Prototype.Browser.XcodeVersion >= 1577) {
            target = Prototype.Browser.Xcode ? '__xcodeOpenInBrowser': '_blank';
            for (i = 0; i < linkList.length; i++) {
                if (linkList[i].up().hasClassName('pediaLink')) {
                    linkList[i].href = linkList[i].href.split('#')[0]; //removed anchor hash as this causes document to scroll down
                } else {
                    linkList[i].target = target; // open all other links in new window
                }
            }
        }
        return linkList;
    },
    reasignRelative: function (linkList, force, target) {
		var linkHREF, i;
        if (Prototype.Browser.IE) {
            return Links.reasignRelativeIE(linkList, force, target);
        } else {
            for (i = 0; i < linkList.length; i++) {
                linkHREF = linkList[i].getAttribute('href');
				if (linkHREF.indexOf('://') === -1 && !linkList[i].hasClassName("browserLink")) {
                    linkList[i].setAttribute('href', (Book.root + linkHREF));
                    if (linkList[i].descendantOf('toc')) {
                        linkList[i].setAttribute('id', 'tocEntry_' + i);
                        TOC.processLink(linkList[i]);
                    }
                    if (typeof target !== 'undefined') {
						linkList[i].target = target;
					}
                }
            }
        }
        return linkList; 
    },
    reasignRelativeIE: function (linkList, force, target) {
        var pageDir, i;
		pageDir = window.location.href.slice(0, window.location.href.replace(window.location.hash, '').lastIndexOf('/'));
        for (i = 0; i < linkList.length; i++) {
            if (force || (!linkList[i].hasClassName('urlLink') && !linkList[i].hasClassName("browserLink"))) {
                linkList[i].setAttribute('href', linkList[i].getAttribute('href').replace((pageDir + '/'), Book.root));
                if (linkList[i].descendantOf('toc')) {
                    linkList[i].setAttribute('id', 'tocEntry_' + i);
                    TOC.processLink(linkList[i]);
                }
                if (target) { 
					linkList[i].target = target;
				}
            }
        }
        return linkList;
    }
};

var DevPubs = {
    init: function () {
        DevPubs.removeADCContent();
        Links.openInBrowser(Prototype.Browser.Xcode ? $$(".urlLink").concat($$(".browserLink")) : $$(".urlLink"));
        Book.root = $('INDEX') ? $('INDEX').href.slice(0, $('INDEX').href.lastIndexOf('/')) + '/': "";
    },
    hideInXcode: function () {
		var toHide, i;
        toHide = $$('.hideInXcode');
        for (i = 0; i < toHide.length; i++) {
            toHide[i].hide();
        }
        return toHide;
    },
    showInXcode: function () {
		var toShow, i;
        toShow = $$('.showInXcode');
        for (i = 0; i < toShow.length; i++) {
            toShow[i].show();
        }
        return toShow;
    },
    removeADCContent: function () {
        if (Prototype.Browser.Xcode || ($('adcHeader') && $('adcHeader').innerText === '')) {
            DevPubs.hideInXcode();
            DevPubs.showInXcode();
            if ($('header') && $('contents')) {
                $('header').addClassName('noADC');
                $('contents').addClassName('noADC');
                if ($('tocContainer')) {
					$('tocContainer').addClassName('noADC');
				}
				if (Prototype.Browser.Xcode || (!$('breadcrumbs'))) {
					if ($('tocContainer')) {
						$('tocContainer').addClassName('noBreadcrumbs');
					}
					$('contents').addClassName('noBreadcrumbs');
					if ($('breadcrumbs')) {
                        $('breadcrumbs').hide();
                    }
				}
			}
		}
        return $$('.noADC').concat($$('.hideInXcode').concat($$('.showInXcode')));
    },
    Window: function (id) {
		var box, header, contents, footer;
        box = document.createElement('div');
        header = document.createElement('div');
        contents = document.createElement('div');
        footer = document.createElement('div');
        box.setAttribute('id', id + 'Window');
        header.setAttribute('id', id + 'Header');
        contents.setAttribute('id', id + 'Body');
        footer.setAttribute('id', id + 'Footer');
        box.appendChild(header);
        box.appendChild(contents);
        box.appendChild(footer);
        box.style.display = 'none';
        document.body.appendChild(box);
        this.box = $(id + 'Window');
        this.header = $(id + 'Header');
        this.contents = $(id + 'Body');
        this.footer = $(id + 'Footer');
    }
};

var Cookie = { //Generic cookie management
    set: function (parameters) { 
        /*	Input: Object of cookie {cName: String, cValue: Object, cExpiry: Date, cDomain: String, cPath: String}
         *	value must have toString() method.
         *	Returns: Cookie String 
		 */
        if (typeof localStorage !== 'undefined') {
            try {
                localStorage.setItem(parameters.cName, parameters.cValue);
            } catch (e) {
                return e;
            }
            return parameters.cValue;
		} else {
			// Set the domain, file:// has no domain so we don't do this if the page is loaded from there
			if (parameters.cDomain === "" && window.location.domain) {
				parameters.cDomain = window.location.domain; //Set to current domain by default
			}

			// build and set the cookie string
			return (document.cookie = parameters.cName +
                '=' + escape(parameters.cValue) +
                (parameters.cExpiry ? '; expires=' + parameters.cExpiry.toGMTString() : '') +
                (parameters.cDomain ? '; domain=' + parameters.cDomain : '') +
                (parameters.cPath ? '; path=' + parameters.cPath : '; path=' +  window.location.pathname));
		}
    },
    get: function (cookie_name) {
        // Input: Sting of Cookie name to retrive
        // Returns: String of Cookie value
        if ((typeof localStorage !== 'undefined') && localStorage.getItem(cookie_name) !== 'null') {
			// Check localStorage first
			return localStorage.getItem(cookie_name);
		} else { // Otherwise data might be in a cookie
			var cookie_array, i, aCookie;
			
			// the browser returns all inscope cookies as 'myCookie1=myValue;myCookie2=anotherValue'
			// Split the string by the divider
			cookie_array = document.cookie.split(';'); 
			
			for (i = 0; i < cookie_array.length; i++) { 
				aCookie = cookie_array[i];
				while (aCookie.charAt(0) === ' ') {
					aCookie = aCookie.substring(1, aCookie.length);
				}
				if (aCookie.indexOf(cookie_name + '=') === 0) {
					if (typeof localStorage !== 'undefined') { // if localStorage is supported, move data there.
						localStorage.setItem(cookie_name, aCookie.substring(cookie_name.length + 1, aCookie.length));
						Cookie.forceExpire(cookie_name);
						return localStorage.getItem(cookie_name);
					} else {
						return aCookie.substring(cookie_name.length + 1, aCookie.length);
					}
				}
			}
			return null;
		}
	},
    expire: function (cookie_name) {
        // Input: Sting of Cookie name to expire
        // Returns: Expired Cookie String
		if (typeof localStorage !== 'undefined') {
			return localStorage.removeItem(cookie_name);
		} else {
			return Cookie.forceExpire(cookie_name);
		}
    },
	forceExpire: function (cookie_name) {
        // Input: Sting of Cookie name to expire
        // Returns: Expired Cookie String
		
		// to expire a cookie we just reset it with a date in the past.
		var past = new Date();
		past.setUTCFullYear(2000);
		return this.set({cName: cookie_name, cValue: null, cExpiry: past, cPath: "/" });
    }
};

/* Client-side access to querystring name=value pairs
    Version 1.3
    28 May 2008
    
    License (Simplified BSD):
    http://adamv.com/dev/javascript/qslicense.txt
*/
function Querystring(qs) { // optionally pass a querystring to parse
	var args, i, pair, name, value;
    this.params = {};
    
    if (!qs) {
		qs = location.search.substring(1, location.search.length);
	}
    if (qs.length === 0) {
		return null;
	}

// Turn <plus> back to <space>
// See: http://www.w3.org/TR/REC-html40/interact/forms.html#h-17.13.4.1
    args = qs.replace(/\+/g, ' ').split('&'); // parse out name/value pairs separated via &
    
// split out each name=value pair
    for (i = 0; i < args.length; i++) {
        pair = args[i].split('=');
        name = decodeURIComponent(pair[0]);
        //;;console.debug(pair);
        value = (pair.length === 2) ? decodeURIComponent(pair[1]) : name;
        
        this.params[name] = value;
    }
}

Querystring.prototype.get = function (key, default_) {
    var value = this.params[key];
    return (value !== null) ? value : default_;
};

Querystring.prototype.contains = function (key) {
    var value = this.params[key];
    return (value !== null);
};

//End http://adamv.com/dev/javascript/qslicense.txt
Prototype.Query = new Querystring();
Prototype.HashQuery = new Querystring(location.hash.replace(/#/,''));

var ToolTip = {
	/*
		Finds all <span>'s in the current document with a class of 'tooltip' and observes them for mouseover.
		
		When one is hovered over by the user we position a <div> with id 'tooltip' over the mouse
		and fill the contents of that div with the contents of the data-abstract attribute of the icon
	*/
    
    observeTips: function (needTips) {
        for (var i = 0; i < needTips.length; i++) {
            needTips[i].observe('mouseover', ToolTip.show);
            needTips[i].observe('mouseout', ToolTip.hide);
        }
        return needTips;
    },
    
    init: function () {
        // Disabling tooltips until Scroll to top fix is validated
        // return ToolTip.observeTips($$('span.tooltip'));
    },
    
    show: function (event) {
        // Makes the "tooltip" element visible and moves it to the 
        // (x,y) of the mouse event (plus some buffer zone)
        var element = event.findElement('span');
        $('tooltip').innerHTML = element.getAttribute('data-abstract') ? element.getAttribute('data-abstract') : element.down('.tooltipicon').getAttribute('data-abstract');
        ToolTip.position(event.pointerX(event), event.pointerY(event));
        if (Prototype.Browser.IE) {
            $('tooltip').style.display = "block";
        }
        return  $('tooltip').addClassName("isVisible");
        },
    hide: function () {
        if (Prototype.Browser.IE) {
            $('tooltip').style.display = "block";
        }
         return $('tooltip').removeClassName("isVisible");
    },
    position: function (x, y) {
        var tooltip = $('tooltip'),
            topOffset;
        if (!Prototype.Browser.IE) { 
            // Some versions of IE don't support maxWidth
            // but this is only here because of a shadow render bug in WebKit and Mozila
            // <rdar://problem/6825580> Shadows extend beyond the right edge of their containing box
            tooltip.style.maxWidth = (window.innerWidth - x - 45) + 'px';
        }
        topOffset = ((window.innerHeight - y - tooltip.getHeight() - 65) < 0) ? -30 : 10
        tooltip.style.left = (x + 4) + 'px';
        tooltip.style.top = (y + topOffset) + 'px';
        return tooltip;
    }
};

document.observe("dom:loaded", function () {
    DevPubs.init();
    ToolTip.init();
});