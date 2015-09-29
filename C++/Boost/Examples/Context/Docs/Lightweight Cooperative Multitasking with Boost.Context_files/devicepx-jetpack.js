var DetectZoom={mediaQueryBinarySearch:function(property,unit,a,b,maxIter,epsilon){var matchMedia,head,style,div,binarySearch;binarySearch=function(a,b,maxIter){var mid=(a+b)/2;if(maxIter==0||b-a<epsilon)return mid;var query="("+property+":"+mid+unit+")";if(matchMedia(query).matches){return binarySearch(mid,b,maxIter-1);}else{return binarySearch(a,mid,maxIter-1);}};if(window.matchMedia){matchMedia=window.matchMedia;}else{head=document.getElementsByTagName('head')[0];style=document.createElement('style');div=document.createElement('div');div.className='mediaQueryBinarySearch';head.appendChild(style);div.style.display='none';document.body.appendChild(div);matchMedia=function(query){style.sheet.insertRule('@media '+query+
'{.mediaQueryBinarySearch '+
'{text-decoration: underline} }',0);var matched=getComputedStyle(div,null).textDecoration=='underline';style.sheet.deleteRule(0);return{matches:matched};};}
var r=binarySearch(a,b,maxIter);if(div){head.removeChild(style);document.body.removeChild(div);}
return r;},_zoomIe7:function(){var rect=document.body.getBoundingClientRect();var z=(rect.right-rect.left)/document.body.offsetWidth;z=Math.round(z*100)/100;return{zoom:z,devicePxPerCssPx:z};},_zoomIe8:function(){var zoom=screen.deviceXDPI/screen.logicalXDPI;return{zoom:zoom,devicePxPerCssPx:zoom};},_zoomWebkitMobile:function(){var devicePixelRatio=window.devicePixelRatio!=null?window.devicePixelRatio:1,deviceWidth;if(Math.abs(window.orientation)==90){deviceWidth=screen.height;}else{deviceWidth=screen.width;}
var z=deviceWidth/window.innerWidth;return{zoom:z,devicePxPerCssPx:z*devicePixelRatio};},_zoomWebkit:function(){var devicePixelRatio=window.devicePixelRatio!=null?window.devicePixelRatio:1;var container=document.createElement('div'),div=document.createElement('div');var important=function(str){return str.replace(/;/g," !important;");};container.setAttribute('style',important('width:0; height:0; overflow:hidden; visibility:hidden; position: absolute;'));div.innerHTML="1<br>2<br>3<br>4<br>5<br>6<br>7<br>8<br>9<br>0";div.setAttribute('style',important('font: 100px/1em sans-serif; -webkit-text-size-adjust: none; height: auto; width: 1em; padding: 0; overflow: visible;'));container.appendChild(div);document.body.appendChild(container);var z=1000/div.clientHeight;z=Math.round(z*100)/100;var r={zoom:z,devicePxPerCssPx:devicePixelRatio*z};document.body.removeChild(container);return r;},_zoomFF35:function(){var z=screen.width/
this.mediaQueryBinarySearch('min-device-width','px',0,6000,20,.0001);z=Math.round(z*100)/100;return{zoom:z,devicePxPerCssPx:z};},_zoomFF36:function(){var container=document.createElement('div'),outerDiv=document.createElement('div');container.setAttribute('style','width:0; height:0; overflow:hidden;'+
'visibility:hidden; position: absolute');outerDiv.style.width=outerDiv.style.height='500px';var div=outerDiv;for(var i=0;i<10;++i){var child=document.createElement('div');child.style.overflowY='scroll';div.appendChild(child);div=child;}
container.appendChild(outerDiv);document.body.appendChild(container);var outerDivWidth=outerDiv.clientWidth;var innerDivWidth=div.clientWidth;var scrollbarWidthCss=(outerDivWidth-innerDivWidth)/10;document.body.removeChild(container);var scrollbarWidthDevice=15;if(-1!=navigator.platform.indexOf('Win')){scrollbarWidthDevice=17;}
var z=scrollbarWidthDevice/scrollbarWidthCss;z=Math.round(z*100)/100;return{zoom:z,devicePxPerCssPx:z};},_zoomFF4:function(){var z=this.mediaQueryBinarySearch('min--moz-device-pixel-ratio','',0,10,20,.0001);z=Math.round(z*100)/100;return{zoom:z,devicePxPerCssPx:z};},_zoomOperaOlder:function(){var fixedDiv=document.createElement('div');fixedDiv.style.position='fixed';fixedDiv.style.width='100%';fixedDiv.style.height='100%';fixedDiv.style.top=fixedDiv.style.left='0';fixedDiv.style.visibility='hidden';document.body.appendChild(fixedDiv);var z=window.innerWidth/fixedDiv.offsetWidth;document.body.removeChild(fixedDiv);return{zoom:z,devicePxPerCssPx:z};},_zoomOpera11:function(){var z=window.outerWidth/window.innerWidth;z=Math.round(z*100)/100;return{zoom:z,devicePxPerCssPx:z};},ratios:function(){var r;if(!isNaN(screen.logicalXDPI)&&!isNaN(screen.systemXDPI)){return this._zoomIe8();}else if('ontouchstart'in window&&document.body.style.webkitTextSizeAdjust!=null){return this._zoomWebkitMobile();}else if(document.body.style.webkitTextSizeAdjust!=null){return this._zoomWebkit();}else if(-1!=navigator.userAgent.indexOf('Firefox/3.5')){return this._zoomFF35();}else if(-1!=navigator.userAgent.indexOf('Firefox/3.6')){return this._zoomFF36();}else if(-1!=navigator.appVersion.indexOf("MSIE 7.")){return this._zoomIe7();}else if(-1!=navigator.userAgent.indexOf('Opera')){var versionIdx=navigator.userAgent.indexOf('Version/');if(11.01<parseFloat(navigator.userAgent.substr(versionIdx+8)))
return this._zoomOpera11();else
return this._zoomOperaOlder();}else if(0.001<(r=this._zoomFF4()).zoom){return r;}else{return{zoom:1,devicePxPerCssPx:1}}},zoom:function(){return this.ratios().zoom;},device:function(){return this.ratios().devicePxPerCssPx;}};var wpcom_img_zoomer={zoomed:false,timer:null,interval:1000,imgNeedsSizeAtts:function(img){if(img.getAttribute('width')!==null||img.getAttribute('height')!==null)
return false;if(img.width<img.naturalWidth||img.height<img.naturalHeight)
return false;return true;},init:function(){var t=this;try{t.zoomImages();t.timer=setInterval(function(){t.zoomImages();},t.interval);}
catch(e){}},stop:function(){if(this.timer)
clearInterval(this.timer);},getScale:function(){var scale=DetectZoom.device();if(scale<=1.0)scale=1.0;else if(scale<=1.5)scale=1.5;else if(scale<=2.0)scale=2.0;else if(scale<=3.0)scale=3.0;else if(scale<=4.0)scale=4.0;else scale=5.0;return scale;},shouldZoom:function(scale){var t=this;if("innerWidth"in window&&!window.innerWidth)
return false;if(scale==1.0&&t.zoomed==false)
return false;return true;},zoomImages:function(){var t=this;var scale=t.getScale();if(!t.shouldZoom(scale)){return;}
t.zoomed=true;var imgs=document.getElementsByTagName("img");for(var i=0;i<imgs.length;i++){var imgScale=imgs[i].getAttribute("scale");if(imgScale==scale||imgScale=="0")
continue;var scaleFail=imgs[i].getAttribute("scale-fail");if(scaleFail&&scaleFail<=scale)
continue;if(!(imgs[i].width&&imgs[i].height))
continue;if(t.scaleImage(imgs[i],scale)){imgs[i].setAttribute("scale",scale);}
else{imgs[i].setAttribute("scale","0");}}},scaleImage:function(img,scale){var t=this;var newSrc=img.src;if(img.parentNode.className.match(/slideshow-slide/))
return false;if(img.src.match(/^https?:\/\/([^\/]*\.)?gravatar\.com\/.+[?&](s|size)=/)){newSrc=img.src.replace(/([?&](s|size)=)(\d+)/,function($0,$1,$2,$3){var originalAtt="originals",originalSize=img.getAttribute(originalAtt);if(originalSize===null){originalSize=$3;img.setAttribute(originalAtt,originalSize);if(t.imgNeedsSizeAtts(img)){img.width=img.width;img.height=img.height;}}
var size=img.clientWidth;var targetSize=Math.ceil(img.clientWidth*scale);targetSize=Math.max(targetSize,originalSize);targetSize=Math.min(targetSize,512);return $1+targetSize;});}
else if(img.src.match(/^https?:\/\/([^\/]+)\.files\.wordpress\.com\/.+[?&][wh]=/)){if(img.src.match(/[?&]crop/))
return false;var changedAttrs={};var matches=img.src.match(/([?&]([wh])=)(\d+)/g);for(var i=0;i<matches.length;i++){var lr=matches[i].split('=');var thisAttr=lr[0].replace(/[?&]/g,'');var thisVal=lr[1];var originalAtt='original'+thisAttr,originalSize=img.getAttribute(originalAtt);if(originalSize===null){originalSize=thisVal;img.setAttribute(originalAtt,originalSize);if(t.imgNeedsSizeAtts(img)){img.width=img.width;img.height=img.height;}}
var size=thisAttr=='w'?img.clientWidth:img.clientHeight;var naturalSize=(thisAttr=='w'?img.naturalWidth:img.naturalHeight);var targetSize=Math.ceil(size*scale);targetSize=Math.max(targetSize,originalSize);if(scale>img.getAttribute("scale")&&targetSize<=naturalSize)
targetSize=thisVal;if(naturalSize<thisVal)
targetSize=thisVal;if(targetSize!=thisVal)
changedAttrs[thisAttr]=targetSize;}
var w=changedAttrs.w||false;var h=changedAttrs.h||false;if(w){newSrc=img.src.replace(/([?&])w=\d+/g,function($0,$1){return $1+'w='+w;});}
if(h){newSrc=newSrc.replace(/([?&])h=\d+/g,function($0,$1){return $1+'h='+h;});}}
else if(img.src.match(/^https?:\/\/([^\/]+\.)*(wordpress|wp)\.com\/mshots\/.+[?&]w=\d+/)){newSrc=img.src.replace(/([?&]w=)(\d+)/,function($0,$1,$2){var originalAtt='originalw',originalSize=img.getAttribute(originalAtt);if(originalSize===null){originalSize=$2;img.setAttribute(originalAtt,originalSize);if(t.imgNeedsSizeAtts(img)){img.width=img.width;img.height=img.height;}}
var size=img.clientWidth;var targetSize=Math.ceil(size*scale);targetSize=Math.max(targetSize,originalSize);if(scale>img.getAttribute("scale")&&targetSize<=img.naturalWidth)
targetSize=$2;if($2!=targetSize)
return $1+targetSize;});}
else if(img.src.match(/^https?:\/\/([^\/.]+\.)*(wp|wordpress)\.com\/imgpress\?(.+)/)){var imgpressSafeFunctions=["zoom","url","h","w","fit","filter","brightness","contrast","colorize","smooth","unsharpmask"];var qs=RegExp.$3.split('&');for(var q in qs){q=qs[q].split('=')[0];if(imgpressSafeFunctions.indexOf(q)==-1){return false;}}
img.width=img.width;img.height=img.height;if(scale==1)
newSrc=img.src.replace(/\?(zoom=[^&]+&)?/,'?');else
newSrc=img.src.replace(/\?(zoom=[^&]+&)?/,'?zoom='+scale+'&');}
else if(img.src.match(/^https?:\/\/([^\/.]+\.)*(wp|wordpress)\.com\/latex\.php\?(latex|zoom)=(.+)/)||img.src.match(/^https?:\/\/i[\d]{1}\.wp\.com\/(.+)/)){img.width=img.width;img.height=img.height;if(scale==1)
newSrc=img.src.replace(/\?(zoom=[^&]+&)?/,'?');else
newSrc=img.src.replace(/\?(zoom=[^&]+&)?/,'?zoom='+scale+'&');}
else if(img.src.match(/^https?:\/\/[^\/]+\/.*[-@]([12])x\.(gif|jpeg|jpg|png)(\?|$)/)){img.width=img.width;img.height=img.height;var currentSize=RegExp.$1,newSize=currentSize;if(scale<=1)
newSize=1;else
newSize=2;if(currentSize!=newSize)
newSrc=img.src.replace(/([-@])[12]x\.(gif|jpeg|jpg|png)(\?|$)/,'$1'+newSize+'x.$2$3');}
else{return false;}
if(newSrc!=img.src){var prevSrc,origSrc=img.getAttribute("src-orig");if(!origSrc){origSrc=img.src;img.setAttribute("src-orig",origSrc);}
if(img.complete)
prevSrc=img.src;else
prevSrc=origSrc;img.onerror=function(){img.src=prevSrc;if(img.getAttribute("scale-fail")<scale)
img.setAttribute("scale-fail",scale);img.onerror=null;};img.src=newSrc;}
return true;}};wpcom_img_zoomer.init();