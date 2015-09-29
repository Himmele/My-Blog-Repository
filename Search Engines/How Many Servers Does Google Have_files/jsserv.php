
<!--
var mbl_recent_visitor='';
var mbl_current_visitor='';
if(typeof(mbl_jsserv_loaded)=='undefined'){var mbl_jsserv_loaded=true;function m_r_e(obj,w,f){if(window.addEventListener){obj.addEventListener(w,f,false);}else if(window.attachEvent){obj.attachEvent('on'+w,f);}else{}}
function m_rm_e(obj,w,f){if(window.addEventListener){obj.removeEventListener(w,f,false);}else if(window.attachEvent){obj.detachEvent('on'+w,f);}else{}}
function m_i_eu(url){if(!url){return false;}
if(typeof(url)!='string'){return true;}
url=url.toLowerCase();if(url.indexOf('https://')==0){url=url.substring(8);}
else if(url.indexOf('http://')==0){url=url.substring(7);}
else if(url.indexOf('ftp://')==0){url=url.substring(6);}
else if(url.indexOf('ssl://')==0){url=url.substring(6);}
else if(url.indexOf('mailto:')==0){return true;}
else{return false;}
if(url.indexOf('www.')==0){url=url.substring(4);}
var hn=document.location.hostname.toLowerCase();if(hn.indexOf('www.')==0){hn=hn.substring(4);}
if(hn==''){return true;}
if(url.indexOf(hn)!=0){return true;}
return false;}

var cs_mbl_isAOL_V8=false;if(navigator.userAgent.indexOf('AOL 8.0')>0){cs_mbl_isAOL_V8=true;}
var cs_mblID='2007060716072534',cs_url_trker='http://track2.mybloglog.com/tr/urltrk.php',mbl_isi='N2011050313372511',mblco_on='',mblco_pu='',mblco_n='',mblco_e='',mblco_u='',mblco_c='';function m_p_c(obj,flag){for(var j=0;j<=5;j++){if(flag==1){if(obj&&obj.nodeName.toUpperCase()=='A'){return obj;}}if(flag==2){if(obj&&obj.nodeName.toUpperCase()!='BODY'&&obj.onclick){return obj;}}if(!obj.parentNode){return null;}obj=obj.parentNode;}
return null;}
function m_is_u(s){if(!s){return false;}
var regexp=/(http|https):\/\/(\w+:{0,1}\w*@)?(\S+)(:[0-9]+)?(\/|\/([\w#!:.?+=&%@!\-\/]))?/i
return regexp.test(s);}
function m_trim(str,ch){while(str.substring(0,1)==ch){str=str.substring(1,str.length);}
while(str.substring(str.length-1,str.length)==ch){str=str.substring(0,str.length-1);}
return str;}
function m_ctx_c(e){var tg=null;if(e){tg=e.target;}else{tg=window.event.srcElement;}m_d_t(tg,false);}
function m_oclk(e){var tg=null;if(e){tg=e.target;}else{tg=window.event.srcElement;}m_d_t(tg,true);}
function m_d_t(tg,delay){var url=null,text=null,co_qstr='';try{if(tg){tgP=m_p_c(tg,1);if(tgP&&tgP.nodeName.toUpperCase()=='A'){tg=tgP;}
if(tg.nodeName.toUpperCase()=='A'){url=tg.href;if(tg.innerHTML){text=tg.innerHTML;}else if(tg.innerText){text=tg.innerText;}else if(tg.text){text=tg.text;}else{}}
else if(tg.nodeName.toUpperCase()=='AREA'){url=tg.href;text=tg.getAttribute('alt')?tg.getAttribute('alt'):tg.getAttribute('id');if(text==null||text==''){text=url;}}
else if(tg.nodeName.toUpperCase()=='INPUT'&&tg.type.toUpperCase()=='SUBMIT'){url=m_trim(tg.form.action,'/');url=m_trim(url,' ');text=tg.value;var co_name='',co_email='',co_url='',co_text='';if(url.toLowerCase().indexOf('comment')>0){if(tg.form.elements['author']){co_name=tg.form.elements['author'].value;}if(tg.form.elements['email']){co_email=tg.form.elements['email'].value;}if(tg.form.elements['url']){co_url=tg.form.elements['url'].value;}if(tg.form.elements['comment']){co_text=tg.form.elements['comment'].value;}else if(tg.form.elements['text']){co_text=tg.form.elements['text'].value;}else{}}
if(co_text!=''){co_qstr='&co_n='+escape(co_name)+'&co_e='+escape(co_email)+'&co_u='+escape(co_url)+'&co_c='+escape(co_text)+'&co_pt='+escape(document.title);}}else{}
if(url==null||text==null){return true;}
if(co_qstr!=''||m_i_eu(url)){if(cs_mbl_isAOL_V8&&(tg.target=='_new'||tg.target=='_blank')){return true;}url=escape(url);text=escape(text);m_trk(text,url,co_qstr,delay);}}}catch(err){}}
function m_trk(text,url,co_qstr,delay){try{var now=new Date();var trackURL=cs_url_trker+'?t=2&u='+url+'&te='+text+'&i='+cs_mblID+'&now='+now.valueOf()+'&d=20110503'+'&db='+'&v='+mbl_isi+co_qstr;var x=document.createElement('IMG');x.src=trackURL;if(delay){m_pse(900);}}catch(err){}}
function m_t_ol(){try{var now=new Date();var url=escape(document.location.href);var ext_referrer=document.referrer;if(ext_referrer!=''&&m_i_eu(ext_referrer)){var ext_referrer_para='&eref='+escape(ext_referrer);}else{var ext_referrer_para='';}
var trackURL=cs_url_trker+'?i='+cs_mblID+'&t=1&u='+url+'&a='+escape(navigator.userAgent)+'&d=20110503'+ext_referrer_para+'&db='+'&now='+now.valueOf()+'&v='+mbl_isi;var x=document.createElement('IMG');x.src=trackURL;var x2=document.createElement('IMG');x2.src='http://geo.yahoo.com/p?s=950300017&t='+now.valueOf();}catch(err){}
return true;}
function m_pse(numberMillis){var now=new Date();var exitTime=now.getTime()+numberMillis;while(true){now=new Date();if(now.getTime()>exitTime){return;}}}
var m_px=0,m_py=0,m_as_frms=new Array(),is_ie=document.all?true:false;function m_as_init(){var ad=document.getElementsByTagName('iframe');for(var i=0;i<ad.length;i++){if(ad[i].src.indexOf('googlesyndication.com')>-1){m_as_frms[m_as_frms.length]=new Array(ad[i],'http://pagead2.googlesyndication.com','Google AdSense');if(is_ie){ad[i].onfocus=m_trk_as;}}else if(ad[i].src.indexOf('ypn-js.overture.com')>-1){m_as_frms[m_as_frms.length]=new Array(ad[i],'http://ypn-js.overture.com','Yahoo! Publisher Network');if(is_ie){ad[i].onfocus=m_trk_as;}}else{}}
if(m_as_frms.length>0&&!is_ie){m_r_e(window,'beforeunload',m_as_ul);m_r_e(document,'mousemove',m_gm);}}
function m_gm(e){if(!e){var e=window.event;}
if(e.pageX||e.pageY){m_px=e.pageX;m_py=e.pageY;}
else if(e.clientX||e.clientY){m_px=e.clientX+document.body.scrollLeft+document.documentElement.scrollLeft;m_py=e.clientY+document.body.scrollTop+document.documentElement.scrollTop;}}
function m_gpos(obj){var curleft=curtop=0;if(obj.offsetParent){curleft=obj.offsetLeft;curtop=obj.offsetTop;while(obj=obj.offsetParent){curleft+=obj.offsetLeft;curtop+=obj.offsetTop;}}
return[curleft,curtop];}
function m_as_ul(e){try{var m_as_size='';for(var i=0;i<m_as_frms.length;i++){var ad_pos=m_gpos(m_as_frms[i][0]);var a_fx=(m_px>(ad_pos[0]-10)&&m_px<(parseInt(ad_pos[0])+parseInt(m_as_frms[i][0].width)+15));var a_fy=(m_py>(ad_pos[1]-10)&&m_py<(parseInt(ad_pos[1])+parseInt(m_as_frms[i][0].height)+10));if(a_fx&&a_fy){m_trk_as(m_as_frms[i][0]);break;}}}catch(err){}}
function m_trk_as(f){try{if(!f&&window.event.srcElement){var f=window.event.srcElement;}
for(var i=0;i<m_as_frms.length;i++){if(m_as_frms[i][0]==f){var a_si=m_as_frms[i][0].width+'x'+m_as_frms[i][0].height;m_trk(escape(m_as_frms[i][2]+' ('+a_si+')'),escape(m_as_frms[i][1]+'#'+a_si),'',true);break;}}
if(is_ie){window.focus();}}catch(err){}}
if(document.body){document.body.onclick=m_oclk;document.body.oncontextmenu=m_ctx_c;}else if(document){document.onclick=m_oclk;document.oncontextmenu=m_ctx_c;}else{}
m_t_ol();m_r_e(window,'load',m_as_init);
  }// --><!-- g2w1.mbl.re1.yahoo.com compressed/chunked Tue May  3 13:37:25 PDT 2011 -->
