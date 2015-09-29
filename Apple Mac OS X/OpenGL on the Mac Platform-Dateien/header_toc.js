/*
    devpubs.js
    TechPubPerlModules
    
    JavaScript library for use by Developer Publications.
    
    
    Copyright (c) 2009 Apple Inc. All Rights Reserved.

*/


// JSLint Configuration
/*jslint plusplus: false, browser: true */
/*global window location escape localStorage */

// prototype.js
/*global $, $$, $F, Template, Prototype, Hash, Ajax, Event*/

// devpubs.js
/*global Cookie, Links, COOKIE_EXPIRE_DATE */

// HTML Page
/*global Book */


var LegacyNotice = {
    init: function () {
        if ($("legacyOuterWrapper")) {
            $("LegacyNoticeCloseBox").observe("click", LegacyNotice.close);
        }
    },
    close: function () {
        $("legacyOuterWrapper").hide();
    }
};

var JumpTo = {
    select: function () {
        window.location.hash = $F('jumpTo');
    },
    load: function () {
        var spaces = "&nbsp;&nbsp;&nbsp;&nbsp;",
            menuItem,
            textForMenu,
            i,
            jumpTos;
        
        if ($('jumpto_button')) {
            jumpTos = $$(".jump");
            if (jumpTos.length > 1) {
                for (i = 0; i < jumpTos.length; i++) {
                    jumpTos[i].id = "jumpTo_" + i;
                    menuItem = document.createElement('option');
                    textForMenu =  jumpTos[i].textContent ? jumpTos[i].textContent : jumpTos[i].innerText;
                    if (jumpTos[i].hasClassName("typeDef")) {
                        menuItem.innerHTML = spaces + textForMenu;
                        menuItem.value = jumpTos[i].id;
                    } else if (jumpTos[i].hasClassName("constantName")) {
                        menuItem.innerHTML = spaces + spaces + textForMenu;
                        menuItem.value = jumpTos[i].id;
                    } else if (jumpTos[i].hasClassName("instanceMethod")) {
                        menuItem.innerHTML = spaces + "-&nbsp;" + textForMenu;
                        menuItem.value = jumpTos[i].id;
                    } else if (jumpTos[i].hasClassName("classMethod")) {
                        menuItem.innerHTML = spaces + "+&nbsp;" + textForMenu;
                        menuItem.value = jumpTos[i].id;
                    } else if (jumpTos[i].hasClassName("method_property")) {
                        menuItem.innerHTML = spaces + textForMenu;
                        menuItem.value = jumpTos[i].id;
                    } else if (jumpTos[i].hasClassName("section2")) {
                        menuItem.innerHTML = "&nbsp;&nbsp;" + textForMenu;
                        menuItem.value = jumpTos[i].previous('a').name;
                    } else if (jumpTos[i].hasClassName("section3")) {
                        menuItem.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;" + textForMenu;
                        menuItem.value = jumpTos[i].previous('a').name;
                    } else if (jumpTos[i].hasClassName("section4")) {
                        menuItem.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + textForMenu;
                        menuItem.value = jumpTos[i].previous('a').name;
                    } else if (jumpTos[i].hasClassName("section5")) {
                        menuItem.innerHTML = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + textForMenu;
                        menuItem.value = jumpTos[i].previous('a').name;
                    } else {
                        menuItem.innerHTML = textForMenu;
                        menuItem.value = jumpTos[i].previous('a').name;
                    }
                    $("jumpTo").insert({bottom: menuItem});
                    if (window.location.hash === "#" + menuItem.value) {
                        menuItem.selected = true;
                    }
                }
                $("jumpto_button").show();
                $("jumpTo").observe("change", JumpTo.select);
            }
            return $("jumpTo");
        } else {
            return null;
        }
    }
};

var TOC = {
    initExpandable: function (sections) {
        var i,
            apple_ref,
            chapter;
        
        for (i = 0; i < sections.length; i++) {
            sections[i].observe("click", TOC.toggleSection);
            apple_ref = sections[i].down("a").getAttribute("href");
            chapter = apple_ref.slice(apple_ref.lastIndexOf("/") + 1, apple_ref.length);
            if (Cookie.get(chapter) === "open") {
                TOC.toggleSection({element: function () { 
                    return sections[i];
                }});
            }
        }
        return i;
    },
    createIcon: function (attributes) {
        var icon = document.createElement('img');
        icon.setAttribute("src", Book.resourcesURI + attributes.src);
        icon.setAttribute("height", "16");
        icon.setAttribute("width", "18");
        icon.setAttribute("alt", attributes.alt);
        icon.setAttribute("id", attributes.id);
        return icon;
    },
    changeCurrentLocation: function (event) {
        if (Book.currentLocation) {
            Book.currentLocation.removeClassName("currentLocation");
        }
        Book.currentLocation = Event.element(event);
        return Book.currentLocation.addClassName("currentLocation");
    },
    processLink: function (link) {
        //link.observe("click", TOC.changeCurrentLocation)
        //if (link.href == window.location.href){
            //link.addClassName("currentLocation");
            //Book.currentLocation = link;
            //if ($('tocEntry_133').up('.children') && !$('tocEntry_133').up('.children').hasClassName('open')){
            //   TOC.toggleSection({element: function () { return $('tocEntry_133').up('.children'); }});
            //}
        //} 
        return link;
    },
    processLinks: function () {
        if ($("Companion_link")) { // If TOC has Companion File link, move to header
            $("file_links").insert({top: $("Companion_link").remove()});
            $('Companion_link').down().insert({top: TOC.createIcon({
                src: "/Images/header_toc/docdownload.png",
                id: "companion_icon",
                alt: "Download Icon"
            })});
            Links.reasignRelative([$('Companion_link').down()], true);
        }
        if ($("PDF_link")) { // If TOC has PDF link, move to header
            $("file_links").insert({top: $("PDF_link").remove()});
            $('PDF_link').down().insert({top: TOC.createIcon({
                src: "/Images/header_toc/pdf.png",
                id: "pdf_icon",
                alt: "PDF Icon"
            })});
            Links.reasignRelative([$('PDF_link').down()], true);
        }
        return Links.reasignRelative($$("#toc a"));
    },
    init: function (transport) {
        $("toc").innerHTML = transport.responseText;
        $("title").insert({top: $("book_title").remove()}); //Insert Book Title
        if ($("book_subtitle")) { // Insert subtitle
            $("book_title").insert({bottom: " &mdash; " + $("book_subtitle").remove().innerHTML});
        }
        TOC.processLinks();
        return TOC.initExpandable($$("#toc .children"));
    },
    load: function () {
        if (Book.tocURI !== "") {
            if( Cookie.get("toc-visible") === "false" ) {
                TOC.toggle();
            }
            $('toc_button').observe("click", TOC.toggle);
            $('toc_button').show();
            return (new Ajax.Request(Book.tocURI, { 
                method:     'get',
                onSuccess:  TOC.init,
                onFailure: function (transport) {
                    console.log(transport);
                }
            }));
        } else {
            var title = document.createElement('h1');
            if (Book.bookTitle !== undefined && Book.bookTitle !== "") {
                title.innerHTML = Book.bookTitle;
            } else {
                title.innerHTML = document.title.split(":")[0];
            }
            return $("title").insert({top: title});
        }
    },
    toggle: function () {
        if ($('tocContainer').hasClassName('isShowingTOC')) {
            $('contents').removeClassName('isShowingTOC');
            $('tocContainer').removeClassName('isShowingTOC');
            $('toc_button').down("button").removeClassName("open");
            Cookie.set({cName: 'toc-visible', cValue: 'false', cExpiry: COOKIE_EXPIRE_DATE, cPath: "/"});
        } else {
            $('contents').addClassName('isShowingTOC');
            $('tocContainer').addClassName('isShowingTOC');
            $('toc_button').down("button").addClassName("open");
            Cookie.expire('toc-visible');
        }
        return $('tocContainer');
    },
    toggleSection: function (event) {
        if (event.element().hasClassName('children')) {
            var toc_expandable = $(event.element()),
                apple_ref = toc_expandable.down('a').getAttribute("href"),
                chapter = apple_ref.slice(apple_ref.lastIndexOf("/") + 1, apple_ref.length);
            if (toc_expandable.down(".collapsible").toggle('slide').visible()) {
                toc_expandable.addClassName('open');
                return Cookie.set({cName: chapter, cValue: 'open', cExpiry: COOKIE_EXPIRE_DATE, cPath: "/"});
            } else {
                toc_expandable.removeClassName('open');
                return Cookie.expire(chapter);
            }
        } else {
            return false;
        }
    }
};

var Page = {
    init: function () {
        if (Book.nextPage) {
            $('nextPage').observe("click", Page.next);
            $('page_buttons').show();
            $('pageNavigationLinks').show();
        } else {
            $('nextPage').disabled = true;
            $('nextLink').hide();
        }
        if (Book.previousPage) {
            $('previousPage').observe("click", Page.previous);
            $('page_buttons').show();
             $('pageNavigationLinks').show();
        } else {
            $('previousPage').disabled = true;
            $('previousLink').hide();
        }
    },
    next: function () {
        if (Book.nextPage) {
            return (window.location = Book.nextPage);
        } else {
            return null;
        }
    },
    previous: function () {
        if (Book.previousPage) {
            return (window.location = Book.previousPage);
        } else {
            return null;
        }
    }
};

var Key = {
    listen: function () {
        window.onkeypress = Key.press;
        window.onkeydown = Key.down;
        window.onkeyup = null;
        if ($('gh-adcsearch')) {
            $('gh-adcsearch').observe('focus', Key.stopListening);
        }
    },
    stopListening: function () {
        window.onkeypress = null;
        window.onkeydown = null;
        window.onkeyup = Key.up;
        if ($('gh-adcsearch')) {
            $('gh-adcsearch').observe('blur', Key.listen);
        }
    },
    down: function (event) {
        var keyDown = event.keyCode;
        switch (keyDown) {
        case 91: // command
            return Key.stopListening();
        case 16: // shift
            return Key.stopListening();
        case 17: // control
            return Key.stopListening();
        case 18: // option
            return Key.stopListening();
        case 20: // caps-lock
            return Key.stopListening();
        default:
            return event;
        }
    },
    up: function (event) {
        var keyDown = event.keyCode;
        switch (keyDown) {
        case 91: // command
            return Key.listen();
        case 16: // shift
            return Key.listen();
        case 17: // control
            return Key.listen();
        case 18: // option
            return Key.listen();
        case 20: // caps-lock
            return Key.listen();
        default:
            return event;
        }
    },
    press: function (event) {
        var keyPressed = event.charCode ? event.charCode : event.keyCode;
        switch (keyPressed) {
        case 44: // ,  < Previous Page
            return Page.previous();
        case 46: // . > Next Page
            return Page.next();
        case 47: // / ? toc
            return TOC.toggle();
        case 106: // j Jump To
            if ($('jumpto_button').visible()) {
                return $('jumpTo').focus();
            } else {
                return event;
            }
            break;
        default:
            return event;
        }
    }
};

document.observe("dom:loaded", function () {
    TOC.load();
    JumpTo.load();
    Page.init();
    Key.listen();
    LegacyNotice.init();
    $('contents').focus();
    if (!Prototype.Browser.WebKit) {
        document.body.style.overflow = "hidden"; 
        //Causes issues with keyboard navigation in WebKit, lack of css causes double scroll bars in other browsers
    }
});