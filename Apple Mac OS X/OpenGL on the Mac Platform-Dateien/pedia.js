/*
    peida.js
    TechPubPerlModules
    
    JavaScript library for use by Developer Publications.
    
    Copyright (c) 2009 Apple Inc. All Rights Reserved.

*/


// JSLint Configuration
/*jslint plusplus: false, browser: true */
/*global window location escape unescape localStorage self */

// prototype.js
/*global $, $$, Template, Prototype, Hash, Ajax */

// scriptaculous.js
/*global Effect */

// HTML page
/*global Book */

// devpubs.js
/*global DevPubs, Links */

var more = {en: "More&#8230;"};

var QuickLook = {
    win: null,
    greyout: document.createElement('div'),
    detect: function () {
        if (Book.isPedia) {
            DevPubs.hideInXcode();
            $("breadcrumbs").hide();
            QuickLook.createPediaLinksBox();
            Links.openInNewWindow($$("a"));
            return true;
        } else {
            return false;
        }
    },
    createCloseBox: function () {
        QuickLook.win.closeBox = document.createElement('img');
        QuickLook.win.closeBox.setAttribute('src', Book.resourcesURI + '/Images/QuickLook/closebox.png');
        QuickLook.win.closeBox.setAttribute('id', 'QuickLookCloseBox');
        QuickLook.win.closeBox.style.display = "none";
        document.body.appendChild(QuickLook.win.closeBox);
        $('QuickLookCloseBox').observe("click", QuickLook.hide);
        return QuickLook.win.closeBox;
    },
    createPediaLinksBox: function () {
        var pediaLinksBox = document.createElement('div'),
            pediaLinks = $$(".pediaPrerequisites").concat($$(".pediaRelated")),
            i;
        pediaLinksBox.setAttribute('id', "pediaLinks");
        for (i = 0; i < pediaLinks.length; i++) {
            pediaLinksBox.innerHTML += pediaLinks[i].innerHTML;
            pediaLinks[i].remove();
        }
        return $('pageTitle').insert({after: pediaLinksBox});
    },
    init: function () {
        QuickLook.greyout.setAttribute('id', 'QuickLookGreyout');
        QuickLook.greyout.style.display = 'none';
        document.body.appendChild(QuickLook.greyout);
        QuickLook.win = new DevPubs.Window('QuickLook');
        QuickLook.win.article = document.createElement('object');
        QuickLook.win.article.type = "text/html";
        QuickLook.win.contents.appendChild(QuickLook.win.article);
        QuickLook.createCloseBox();
        return QuickLook.win;
    },
    load: function (url) {
        var effect = new Effect.Appear(QuickLook.greyout, {position: 'front', scope: 'QuickLook', duration: 0.1});
        QuickLook.effect = new Effect.Parallel([
            new Effect.SlideDown(QuickLook.win.box, {sync: true}),
            new Effect.Appear(QuickLook.win.closeBox, {sync: true})
        ],
        {duration: 1, position: 'end', scope: 'QuickLook'}
        );
        QuickLook.win.article.data = url;
        QuickLook.win.article.innerHTML = "Unsupported Browser: <a href='" + url + "'>Click Here to view article</a>";
        Pedia.hide("force");
        return QuickLook.win;
    },
    hide: function () {
        QuickLook.effect = new Effect.Parallel([
            new Effect.SwitchOff(QuickLook.win.box, {sync: true}),
            new Effect.Fade(QuickLook.win.closeBox, {sync: true}),
            new Effect.Fade(QuickLook.greyout, {sync: true})
        ],
        {duration: 0.75}
        );
        return QuickLook.win;
    }
};

var Pedia = {
    visable: false,
    win: null,
    init: function () {// adds onclick's, returns the number of pedia links on the current page
        var pediaLinks = $$('.pediaLink'),
            i;
        for (i = 0; i < pediaLinks.length; i++) {
            pediaLinks[i].observe("click", Pedia.click);
        }
        if (i > 0) {
            Pedia.win = new DevPubs.Window('pedia');
            QuickLook.init();
            document.observe('click', Pedia.hide);
        }
        return i;   
    },
    addMoreLink: function (insertLocation) {
        if (Pedia.toShow.select('a').length > 0) {
            var moreLink = document.createElement('a'),
                pediaHref = Pedia.toShow.down("a").href;
            moreLink.setAttribute('id', 'peidaMore');
            pediaHref = pediaHref.split('#')[0];
            moreLink.innerHTML = more['en'];
            insertLocation.insert(moreLink);
            if (navigator.platform.indexOf('Win') > -1) {//All current windows browsers have issues with embeded objects that contain HTML
                $('peidaMore').href = pediaHref;
                $('peidaMore').target = '_blank';
            } else {
                $('peidaMore').observe('click', function (event) {
                    QuickLook.load(pediaHref);
                });
            }
            
        }
        return insertLocation;
    },
    position: function (addtional) {
        var offset =  document.viewport.getWidth() - Pedia.visable.viewportOffset().left - Pedia.win.box.getWidth() - 40,
            offsetTop;
        offset = (offset > -10) ? -10 : (offset < -240) ? -240 : offset;
        offset += addtional; // FIXME: For some reason this is required in Safari 4b
        if ((document.viewport.getHeight() - Pedia.visable.viewportOffset().top - Pedia.win.box.getHeight() - 50) > 0) {
            Pedia.toShow.addClassName('activePedia');
            return Pedia.win.box.clonePosition(Pedia.visable, {setWidth: false, setHeight: false, offsetLeft: offset, offsetTop: Pedia.visable.getHeight()});
        } else {
            Pedia.toShow.addClassName('activePedia-bottom');
            offsetTop = -1 * (Pedia.win.box.getHeight());
            return Pedia.win.box.clonePosition(Pedia.visable, {setWidth: false, setHeight: false, offsetLeft: offset, offsetTop: (offsetTop + 1)});
        }

    },
    click: function (event) {//click event for pedia links, displays and returns pedia window
        event.stop();
        Pedia.toShow = $(Event.element(event)).hasClassName('pediaLink')? $(Event.element(event)): $(Event.element(event)).up('.pediaLink');
        if (Pedia.toShow === Pedia.visable) {
            Pedia.hide({type: 'force'});
        } else {
            Pedia.hide({type: 'force'}, function () {
                Pedia.show();
            });
        }
    },
    show: function () {
        Pedia.visable = Pedia.toShow;
        Pedia.win.header.innerHTML = Pedia.toShow.getAttribute('data-header');
        Pedia.win.contents.innerHTML = Pedia.toShow.getAttribute('data-contents');
        Pedia.addMoreLink(Pedia.win.contents);
        Pedia.position(-4);
        Pedia.win.box.appear({duration: 0.4, queue: {position: 'end', scope: 'Pedia'}});
        window.onresize = Pedia.hide;
        $('contents').observe('scroll', Pedia.hide);
        return Pedia.visable;
    },
    hide: function (event, afterFinish) {// hides the currenlty visable pedia window, returns boolean
        afterFinish = afterFinish ? afterFinish : Prototype.emptyFunction;
        if (Pedia.visable) {
            if (event.type === 'resize' || event.type === 'scroll' || event.type === 'force' ||
              (event.element && !event.element().descendantOf(Pedia.win.box))
              ) {
                Pedia.win.box.fade({duration: 0.25, queue: {position: 'front', scope: 'Pedia'}, afterFinish: afterFinish});
                if (Pedia.visable.hasClassName('activePedia')) {
                    Pedia.visable.removeClassName('activePedia');
                } else {
                    Pedia.visable.removeClassName('activePedia-bottom');
                }
                Pedia.visable = false;
                window.onresize = null;
                $('contents').onscroll = null;
            }
        } else {
            afterFinish();
        }
        return $$('activePedia').length === 0;
    }
};

document.observe('dom:loaded', function () {
    if (!QuickLook.detect()) {
        Pedia.init();
    }
});