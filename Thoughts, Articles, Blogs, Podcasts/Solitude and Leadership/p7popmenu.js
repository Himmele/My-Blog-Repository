/* 
  ------------------------------------------------
  PopMenu Magic menu scripts
  Copyright (c) 2004-2006 Project Seven Development
  www.projectseven.com
  Version: 1.0.3
  ------------------------------------------------
*/
var p7PMp,p7PMct;
function P7_setPM(){ //v1.0.3 by PVII-www.projectseven.com
 var i,d='',h="<sty"+"le type=\"text/css\">",tA=navigator.userAgent.toLowerCase();if(window.opera){
 if(tA.indexOf("opera 5")>-1||tA.indexOf("opera 6")>-1){return;}}if(document.getElementById){
 for(i=1;i<20;i++){d+='ul ';h+="\n#p7PMnav "+d+"{position:absolute;left:-9000px;}";}
 document.write(h+"\n<"+"/sty"+"le>");}}P7_setPM();
function P7_initPM(){ //v1.0 by PVII-www.projectseven.com
 var i,g,tD,tA,tU,pp,lvl,ev,tn=navigator.userAgent.toLowerCase();if(window.opera){
 if(tn.indexOf("opera 5")>-1||tn.indexOf("opera 6")>-1){return;}}else if(!document.getElementById){return;}
 p7PMp=arguments;p7PMct=new Array;tD=document.getElementById('p7PMnav');if(tD){tA=tD.getElementsByTagName('A');
 for(i=0;i<tA.length;i++){tA[i].p7PMcl=p7PMct.length;p7PMct[p7PMct.length]=tA[i];g=tA[i].parentNode.getElementsByTagName("UL");
 tA[i].p7PMsub=(g&&g[0])?g[0]:false;ev=tA[i].getAttribute("onmouseover");if(!ev||ev=='undefined'){tA[i].onmouseover=function(){
 P7_PMtrig(this);};}ev=tA[i].getAttribute("onfocus");if(!ev||ev=='undefined'){tA[i].onfocus=function(){P7_PMtrig(this);};}
 if(tA[i].p7PMsub){pp=tA[i].parentNode;lvl=0;while(pp){if(pp.tagName&&pp.tagName=="UL"){lvl++;}pp=pp.parentNode;}
 tA[i].p7PMlv=lvl;}}tD.onmouseout=P7_PMclose;P7_PMopen();}
}
function P7_PMtrig(a){ //v1.0.3 by PVII-www.projectseven.com
 var b,t;if(document.p7PMt){clearTimeout(document.p7PMt);}document.p7PMa=1;b=(a.p7PMsub)?'P7_PMshow(':'P7_PMtg(';
 t='document.p7PMt=setTimeout("'+b+a.p7PMcl+')",160)';eval (t);
}
function P7_PMshow(a,bp){ //v1.0.3 by PVII-www.projectseven.com
 var u,lv,oft,ofr,uw,uh,pp,aw,ah,adj,mR,mT,wW=0,wH,w1,w2,w3,sct,pw,lc,pwv,xx=0,yy=0,wP=true;
 var iem=(navigator.appVersion.indexOf("MSIE 5")>-1)?true:false,dce=document.documentElement,dby=document.body;document.p7PMa=1;
 if(!bp){P7_PMtg(a);}u=p7PMct[a].p7PMsub;if(u.p7pmax&&u.p7pmax==1){return;}u.p7pmax=1;lv=(p7PMp[0]==1&&p7PMct[a].p7PMlv==1)?true:false;
 p7PMct[a].className=p7PMct[a].className.replace("p7PMtrg","p7PMon");oft=parseInt(p7PMp[3]);ofr=parseInt(p7PMp[4]);
 uw=u.offsetWidth;uh=u.offsetHeight;pp=p7PMct[a];aw=pp.offsetWidth;ah=pp.offsetHeight;while(pp){xx+=(pp.offsetLeft)?pp.offsetLeft:0;
 yy+=(pp.offsetTop)?pp.offsetTop:0;if(window.opera||navigator.userAgent.indexOf("Safari")>-1){
 if(p7PMct[a].p7PMlv!=1&&pp.nodeName=="BODY"){yy-=(pp.offsetTop)?pp.offsetTop:0;}}pp=pp.offsetParent;}
 if(iem&&navigator.userAgent.indexOf("Mac")>-1){yy+=parseInt(dby.currentStyle.marginTop);}adj=parseInt((aw*ofr)/100);mR=(lv)?0:aw-adj;
 adj=parseInt((ah*oft)/100);mT=(lv)?0:(ah-adj)*-1;w3=dby.parentNode.scrollLeft;if(!w3){w3=dby.scrollLeft;}w3=(w3)?w3:0;
 if(dce&&dce.clientWidth){wW=dce.clientWidth+w3;}else if(dby){wW=dby.clientWidth+w3;}if(!wW){wW=0;wP=false;}wH=window.innerHeight;
 if(!wH){wH=dce.clientHeight;if(!wH||wH<=0){wH=dby.clientHeight;}}sct=dby.parentNode.scrollTop;if(!sct){sct=dby.scrollTop;if(!sct){
 sct=window.scrollY?window.scrollY:0;}}pw=xx+mR+uw;if(pw>wW&&wP){mR=uw*-1;mR+=10;if(lv){mR=(wW-xx)-uw;}}lc=xx+mR;if(lc<0){mR=xx*-1;}
 pw=yy+uh+ah+mT-sct;pwv=wH-pw;if(pwv<0){mT+=pwv;if(uh>wH){mT=(yy+ah-sct)*-1;}}u.style.marginLeft=mR+'px';u.style.marginTop=mT+'px';
 if(p7PMp[2]==1){if(!iem){P7_PManim(a,20);}}u.className="p7PMshow";
}
function P7_PMhide(u){ //v1.0.3 by PVII-www.projectseven.com
 var i,tt,ua;u.p7pmax=0;u.className="p7PMhide";ua=u.parentNode.firstChild;ua.className=ua.className.replace("p7PMon","p7PMtrg");
}
function P7_PMtg(a,b){ //v1.0.3 alpha by PVII-www.projectseven.com
 var i,u,tA,tU,pp;tA=p7PMct[a];pp=tA.parentNode;while(pp){if(pp.tagName=="UL"){break;}pp=pp.parentNode;}if(pp){
 tU=pp.getElementsByTagName("UL");for(i=tU.length-1;i>-1;i--){if(b!=1&&tA.p7PMsub==tU[i]){continue;}else{P7_PMhide(tU[i]);}}}
}
function P7_PMclose(evt){ //v1.0.3 by PVII-www.projectseven.com
 var pp,st,tS,m=true;evt=(evt)?evt:((event)?event:null);st=document.p7PMa;if(st!=-1){if(evt){
 tS=(evt.relatedTarget)?evt.relatedTarget:evt.toElement;if(tS){pp=tS.parentNode;while(pp){if(pp&&pp.id&&pp.id=="p7PMnav"){m=false;
 document.p7PMa=1;break;}pp=pp.parentNode;}}if(m){document.p7PMa=-1;if(document.p7PMt){clearTimeout(document.p7PMt);}
 document.p7PMt=setTimeout("P7_PMclr()",360);}}}
}
function P7_PMclr(){ //v1.0.3 by PVII-www.projectseven.com
 var i,tU,tUU;document.p7PMa=-1;tU=document.getElementById('p7PMnav');if(tU){tUU=tU.getElementsByTagName("UL");if(tUU){
 for(i=tUU.length-1;i>-1;i--){P7_PMhide(tUU[i]);}}}
}
function P7_PManim(a,st){ //v1.0.3 by PVII-www.projectseven.com
 var g=p7PMct[a].p7PMsub,sp=30,inc=20;st=(st>=100)?100:st;g.style.fontSize=st+"%";if(st<100){st+=inc;setTimeout("P7_PManim("+a+","+st+")",sp);}
}
function P7_PMmark(){document.p7PMop=arguments;}
function P7_PMopen(){ //v1.0.3 by PVII-www.projectseven.com
 var i,x,tA,op,pp,wH,tA,aU,r1,k=-1,kk=-1,mt=new Array(1,'','');if(document.p7PMop){mt=document.p7PMop;}op=mt[0];if(op<1){return;}
 tA=document.getElementById('p7PMnav').getElementsByTagName("A");wH=window.location.href;r1=/index\.[\S]*/i;for(i=0;i<tA.length;i++){
 if(tA[i].href){aU=tA[i].href.replace(r1,'');if(op>0){if(tA[i].href==wH||aU==wH){k=i;kk=-1;break;}}if(op==2){if(tA[i].firstChild){
 if(tA[i].firstChild.nodeValue==mt[1]){kk=i;}}}if(op==3 && tA[i].href.indexOf(mt[1])>-1){kk=i;}if(op==4){for(x=1;x<mt.length;x+=2){
 if(wH.indexOf(mt[x])>-1){if(tA[i].firstChild&&tA[i].firstChild.data){if(tA[i].firstChild.data==mt[x+1]){kk=i;break;}}}}}}}k=(kk>k)?kk:k;
 if(k>-1){pp=tA[k].parentNode;while(pp){if(pp.nodeName=="LI"){pp.firstChild.className="p7PMmark"+" "+pp.firstChild.className;}
 pp=pp.parentNode;}}if(kk>-1){document.p7PMad=1;}P7_PMadma();P7_PMadmb();
}
function P7_PMadma(){ //v1.0.3 by PVII-www.projectseven.com
 var s,ss,i,j,a,g,b,c,d,t,h,tA,b,tP,r1,r2,tI,bA,aA,tB=new Array(),bC='',x=0,ur=1,mt=document.p7PMad;g=document.getElementById("p7PMnav");
 b=document.getElementById("pmmcrumb");if(g&&b){c=b.getElementsByTagName("A");if(c&&c[0]){tP=c[0].parentNode.childNodes;r1=/<a/i;r2=/\/a>/i;
 tI=c[0].parentNode.innerHTML;j=tI.search(r1);bA=tI.substring(0,j);j=tI.search(r2);aA=tI.substring(j+3);bC+=(bA)?bA:'';s=(aA)?aA:' &gt ';
 if(!c[0].id||c[0].id!="pmmcn"){if(c[0].href!=window.location.href){tB[0]=c[0];x++;ur=2;}}tA=g.getElementsByTagName("A");for(i=0;i<tA.length;i++){
 if(tA[i].className.indexOf("p7PMmark")>-1){tB[x]=tA[i];x++;}}for(i=0;i<tB.length;i++){ss=(i>0)?s:'';a=(i==tB.length-1)?0:1;
 d=(i==0&&c[0].id)?'id="'+c[0].id+'" ':' ';t=tB[i].firstChild.nodeValue;if(a==1||mt==1||x<ur){bC+=ss+'<a '+d+'hr'+'ef="'+tB[i].href+'">'+t+'</a>';
 }else{bC+=ss+t;}}if(mt==1||i<ur){ss=(i>0)?s:'';bC+=ss+document.title;}c[0].parentNode.innerHTML=bC;}}
}
function P7_PMadmb(){ //v1.0.3 by PVII-www.projectseven.com
 var h='',g,i,tA,b,m=false;g=document.getElementById("p7PMnav");b=document.getElementById("pmmnext");if(g&&b){tA=g.getElementsByTagName("A");
 for(i=tA.length-1;i>-1;i--){if(tA[i].className.indexOf("p7PMmark")>-1){m=true;break;}}if(m){if(i<tA.length-1){i++;}else{i=0;}
 while(tA[i].href==window.location.href+"#"||tA[i].href=="javascript:;"){i++;if(i>tA.length-1){
 i=0;break;}}b.href=tA[i].href;b.innerHTML=tA[i].firstChild.nodeValue;}}	
}
