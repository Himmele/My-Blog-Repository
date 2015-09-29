Object.extend(Event,{_domReady:function(){if(arguments.callee.done)return;arguments.callee.done=true;if(this._timer)clearInterval(this._timer);AC.isDomReady=true;if(this._readyCallbacks)this._readyCallbacks.each(function(f){f()});this._readyCallbacks=null;},onDOMReady:function(f){if(AC.isDomReady){f();}
else{if(!this._readyCallbacks){var domReady=this._domReady.bind(this);if(document.addEventListener)
document.addEventListener("DOMContentLoaded",domReady,false);if(/WebKit/i.test(navigator.userAgent)){this._timer=setInterval(function(){if(/loaded|complete/.test(document.readyState))domReady();},10);}
Event.observe(window,'load',domReady);Event._readyCallbacks=[];}
Event._readyCallbacks.push(f);}}});if(typeof(AC)=="undefined")AC={};AC.decorateSearchInput=function(field,options){var searchField=$(field);var standIn=null;var results=0;var placeholder='';var autosave='';if(options){if(options.results){results=options.results;}
if(options.placeholder){placeholder=options.placeholder;}
if(options.autosave){autosave=options.autosave;}}
if(AC.Detector.isWebKit()){if(AC.Detector.isWin()){searchField.addClassName('not-round');}
searchField.setAttribute('type','search');if(!searchField.getAttribute('results')){searchField.setAttribute('results',results);}
if(null!=placeholder){searchField.setAttribute('placeholder',placeholder);searchField.setAttribute('autosave',autosave);}}else{searchField.setAttribute('autocomplete','off');standIn=document.createElement('input');searchField.parentNode.replaceChild(standIn,searchField)
var left=document.createElement('span');Element.addClassName(left,'left');var right=document.createElement('span');Element.addClassName(right,'right');var reset=document.createElement('div');Element.addClassName(reset,'reset');var wrapper=document.createElement('div');Element.addClassName(wrapper,'search-wrapper');var alreadyHasPlaceholder=field.value==placeholder;var isEmpty=field.value.length==0;if(alreadyHasPlaceholder||isEmpty){searchField.value=placeholder;Element.addClassName(wrapper,'blurred');Element.addClassName(wrapper,'empty');}
wrapper.appendChild(left);wrapper.appendChild(searchField);wrapper.appendChild(right);wrapper.appendChild(reset);var focus=function(){var blurred=Element.hasClassName(wrapper,'blurred');if(searchField.value==placeholder&&blurred){searchField.value='';}
Element.removeClassName(wrapper,'blurred');}
Event.observe(searchField,'focus',focus);var blur=function(){if(searchField.value==''){Element.addClassName(wrapper,'empty');searchField.value=placeholder;}
Element.addClassName(wrapper,'blurred');}
Event.observe(searchField,'blur',blur);var toggleReset=function(){if(searchField.value.length>=0){Element.removeClassName(wrapper,'empty');}}
Event.observe(searchField,'keydown',toggleReset);var resetField=function(){return(function(evt){var escaped=false;if(evt.type=='keydown'){if(evt.keyCode!=27){return;}else{escaped=true;}}
searchField.blur();searchField.value='';Element.addClassName(wrapper,'empty');searchField.focus();})}
Event.observe(reset,'mousedown',resetField());Event.observe(searchField,'keydown',resetField());if(standIn){standIn.parentNode.replaceChild(wrapper,standIn);}}}
var Element2={};Element2.Methods={getInnerDimensions:function(element){element=$(element);var d=Element.getDimensions(element);var innerHeight=d.height;var styleOf=Element.getStyle;innerHeight-=styleOf(element,'border-top-width')&&styleOf(element,'border-top-width')!='medium'?parseInt(styleOf(element,'border-top-width'),10):0;innerHeight-=styleOf(element,'border-bottom-width')&&styleOf(element,'border-bottom-width')!='medium'?parseInt(styleOf(element,'border-bottom-width'),10):0;innerHeight-=styleOf(element,'padding-top')?parseInt(styleOf(element,'padding-top'),10):0;innerHeight-=styleOf(element,'padding-bottom')?parseInt(styleOf(element,'padding-bottom'),10):0;var innerWidth=d.width;innerWidth-=styleOf(element,'border-left-width')&&styleOf(element,'border-left-width')!='medium'?parseInt(styleOf(element,'border-left-width'),10):0;innerWidth-=styleOf(element,'border-right-width')&&styleOf(element,'border-right-width')!='medium'?parseInt(styleOf(element,'border-right-width'),10):0;innerWidth-=styleOf(element,'padding-left')?parseInt(styleOf(element,'padding-left'),10):0;innerWidth-=styleOf(element,'padding-right')?parseInt(styleOf(element,'padding-right'),10):0;return{width:innerWidth,height:innerHeight};},getOuterDimensions:function(element){element=$(element);var clone=element.cloneNode(true);document.body.appendChild(clone);Element.setStyle(clone,{position:"absolute",visibility:"hidden"});var d=Element.getDimensions(clone);var outerHeight=d.height;var styleOf=Element.getStyle;outerHeight+=styleOf(clone,'margin-top')?parseInt(styleOf(clone,'margin-top'),10):0;outerHeight+=styleOf(clone,'margin-bottom')?parseInt(styleOf(clone,'margin-bottom'),10):0;var outerWidth=d.width;outerWidth+=styleOf(clone,'margin-left')?parseInt(styleOf(clone,'margin-left'),10):0;outerWidth+=styleOf(clone,'margin-right')?parseInt(styleOf(clone,'margin-right'),10):0;Element.remove(clone);return{width:outerWidth,height:outerHeight};},removeAllChildNodes:function(element){element=$(element);if(!element){return;}
while(element.hasChildNodes()){element.removeChild(element.lastChild);}}};Object.extend(Element,Element2.Methods);if(typeof(AC)=="undefined"){AC={};}
if(typeof(AC.Tracking)=="undefined"){AC.Tracking={};}
AC.Tracking.getLinkClicked=function(target)
{if(!target){return null;}
while(target.nodeName.toLowerCase()!='a'&&target.nodeName.toLowerCase()!='body'){target=target.parentNode;}
if(!target.href){target=null;}
return target;}
AC.Tracking.trackLinksWithin=function(container,test,title,properties,options)
{$(container).observe('mousedown',function(evt){var target=AC.Tracking.getLinkClicked(Event.element(evt));if(target&&test(target)){if(options&&options.beforeTrack){var altered=options.beforeTrack(target,title,properties);if(altered){title=altered.title;properties=altered.properties;}}
AC.Tracking.trackClick(properties,this,'o',title);}});}
AC.Tracking.tagLinksWithin=function(container,key,value,test)
{$(container).observe('mousedown',function(evt){var link=Event.element(evt);if(!link){return;}
while(link.nodeName.toLowerCase()!='a'&&link.nodeName.toLowerCase()!='body'){link=link.parentNode;}
if(link.href&&test(link)){AC.Tracking.tagLink(link,key,value);}
link=null;})}
AC.Tracking.tagLink=function(link,key,value)
{var href=link.getAttribute('href');if(href.match(/\?/)){var params=href.toQueryParams();params[key]=value;href=href.split(/\?/)[0]+'?'+$H(params).toQueryString();}else{href+='?'+key+'='+value;}
link.setAttribute('href',href);}
AC.Tracking.s_vi=function()
{var cookies=document.cookie.split(';'),s_vi=null,match;for(var i=0,cookie;(cookie=cookies[i]);i++){match=cookie.match(/^\s*s_vi=\[CS\]v1\|(.+)\[CE\]\s*$/);if(match){s_vi=match[1];break;}}
return s_vi;}
AC.Tracking.track=function(trackingMethod,properties,options)
{if(typeof(s_gi)=='undefined'||!s_gi){return;}
options=options||{};if(typeof(s_account)!='undefined'){s=s_gi(s_account)}else if(options.s_account){s=s_gi(options.s_account);}else{return;}
if(trackingMethod==s.tl){var linkTrackVars=''
for(var key in properties){linkTrackVars+=key+',';}
linkTrackVars=linkTrackVars.replace(/,$/,'');s.linkTrackVars=linkTrackVars;}else{s.linkTrackVars='';}
s.prop4="";s.g_prop4="";s.prop6="";s.g_prop6="";s.pageName="";s.g_pageName="";s.pageURL="";s.g_pageURL="";s.g_channel="";var sanitize=function(value){if(typeof(value)=="string"){return value.replace(/[\'\"\“\”\‘\’]/g,'');}else{return value;}}
for(var key in properties){s[key]=sanitize(properties[key]);if(key=='events'){s.linkTrackEvents=sanitize(properties[key]);}}
if(trackingMethod==s.t){void(s.t());}else{s.tl(options.obj,options.linkType,sanitize(options.title));}
for(var key in properties){if(key!='pageName'){s[key]='';}
if(key=='events'){s.linkTrackEvents='None';}}},AC.Tracking.trackClick=function(properties,obj,linkType,title,options)
{var options={obj:obj,linkType:linkType,title:title};AC.Tracking.track(s.tl,properties,options);},AC.Tracking.trackPage=function(properties,options)
{AC.Tracking.track(s.t,properties,options);}
Element.Methods.childNodeWithNodeTypeAtIndex=function(element,nodeType,index){var node=element.firstChild;if(!node)return null;var i=0;while(node){if(node.nodeType===nodeType){if(index===i){return node;}
i++;}
node=node.nextSibling;}
return null;};String.prototype.lastPathComponent=function(){var index=this.lastIndexOf("/");if(index!=-1){return this.substring(index+1,this.length-1);}
else return null;}
String.prototype.stringByDeletingLastPathComponent=function(){var index=this.lastIndexOf("/");if(index!=-1){return this.slice(0,index);}
else return null;}
String.prototype.stringByAppendingPathComponent=function(value){return(this.lastIndexOf("/")!==(this.length-1))?(this+"/"+value):(this+value);}
String.prototype.stringByRemovingPrefix=function(value){var index=this.indexOf(value);if(index>-1){var result=this.substring(index+value.length,this.length);return result;}
else{return this;}}
String.prototype.pathExtension=function(){var lastPathComponent=this.lastPathComponent();var index=lastPathComponent.lastIndexOf(".");if(index!=-1){return lastPathComponent.slice(index,lastPathComponent.length);}
else return"";}
Array.prototype.addObjectsFromArray=function(array){if(array.constructor===Array){this.push.apply(this,array);}
else{for(var i=0,item;(item=array[i]);i++){this[this.length]=item;}}}
Array.prototype.item=function(index){return this[index];}
document._importNode=function(node,allChildren){if(node.nodeType===Node.ELEMENT_NODE){var newNode=document.createElement(node.nodeName);var i,il;if(node.attributes&&node.attributes.length>0)
var nodeAttributes=node.attributes;var iNodeName,iNodeValue;for(i=0,il=node.attributes.length;i<il;){iNodeName=nodeAttributes[i].nodeName;iNodeValue=node.getAttribute(nodeAttributes[i++].nodeName);if(iNodeName==="class"){newNode.setAttribute("className",iNodeValue);}
newNode.setAttribute(iNodeName,iNodeValue);}
if(allChildren&&node.childNodes&&node.childNodes.length>0){for(i=0,il=node.childNodes.length;i<il;i++){if(newNode.tagName==="NOSCRIPT"){continue;}
newNode.appendChild(document._importNode(node.childNodes[i],allChildren));}}
return newNode;}
else if(node.nodeType===Node.TEXT_NODE){return document.createTextNode(node.nodeValue);}
else if(node.nodeType===Node.COMMENT_NODE){return document.createComment(node.nodeValue);}
else if(node.nodeType===Node.CDATA_SECTION_NODE){return document.createCDATASection(node.nodeValue);}
else return null;};if(!document.importNode){document.importNode=document._importNode;}
if(AC.Detector.isIEStrict()){Element.Methods.hasAttribute=function(element,attributeName){if(attributeName=="class")attributeName="className";else if(attributeName=="for")attributeName="htmlFor";var result=element.getAttribute(attributeName);return((result!=null)&&(result!==""));};document._getElementsByName=document.getElementsByName;document._HTMLElementsWithName=["a","apple","button","form","frame","iframe","img","input","object","map","meta","param","textarea","select"];document.getElementsByName=function(name){var _HTMLElementsWithName=this._HTMLElementsWithName;var result=[],ieResult,i,iNode;for(var e=0,element;(element=_HTMLElementsWithName[e]);e++){ieResult=document.getElementsByTagName(element);for(i=0;(iNode=ieResult[i]);i++){if(iNode.name===name){result[result.length]=iNode;}}}
return result;}}