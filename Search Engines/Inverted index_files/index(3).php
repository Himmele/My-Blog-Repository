// [[image:Erioll_world.svg|18px]] '''WikiMiniAtlas''' <br>
// Script to embed interactive maps into pages that have coordinate templates <br>
// also check my user page [[User:Dschwen]] for more tools<pre>
//
// Revision 14.9

jQuery(function() {
 var $ = jQuery,
 config = {
  width  : 600,
  height : 400,
  timeout : 5000,
  zoom : -1,
  quicklink : false,
  quicklinkurl : 'http://maps.google.com/maps?ll={latdegdec},{londegdec}&spn={span},{span}&q={latdegdec},{londegdec}',
  enabled : true,
  onlytitle : false,
  iframeurl : 'http://toolserver.org/~dschwen/wma/iframe.html',
  imgbase   : 'http://toolserver.org/~dschwen/wma/tiles/',
  buttonImage: 'http://commons.wikimedia.org/w/thumb.php?w=17&f=WMA_button2b.png'
 },
 strings = {
  buttonTooltip : {
   af:'Vertoon ligging op \'n interaktiwe kaart.',
   als:'Ort uf dr interaktivä Chartä zeigä',
   ar:'شاهد الموقع على الخريطة التفاعلية',
   'be-tarask':'паказаць месцазнаходжаньне на інтэрактыўнай мапе',
   'be-x-old':'паказаць месцазнаходжаньне на інтэрактыўнай мапе',
   bg:'покажи местоположението на интерактивната карта',
   bpy:'জীবন্ত মানচিত্রগর মা মাপাহান দেখাদিতই',
   br:'diskouez al lec\'hiadur war ur gartenn etrewezhiat',
   ca:'mostra la localització en un mapa interactiu',
   cs:'zobraz místo na interaktivní mapě',
   da:'vis beliggenhed på interaktivt kort',
   de:'Ort auf interaktiver Karte anzeigen',
   fa:'نمایش مکان در یک نقشه پویا',
   el:'εμφάνιση τοποθεσίας σε διαδραστικό χάρτη',
   en:'Show location on an interactive map',
   eo:'Montru lokigon sur interaktiva karto',
   eu:'erakutsi kokalekua mapa interaktibo batean',
   es:'mostrar el lugar en un mapa interactivo',
   fr:'Montrer la localisation sur une carte interactive',
   fur:'mostre la localizazion suntune mape interative',
   fy:'it plak op in oanpasbere kaart oanjaan',
   gl:'Amosar o lugar nun mapa interactivo',
   he:'הראה מיקום במפה האינטראקטיבית',
   hi:'सक्रिय नक्शे पर लोकेशन या स्थान दिखायें', 
   hr:'prikaži lokaciju na interaktivnom zemljovidu',
   hu:'Mutasd a helyet egy interaktív térképen!',
   hy:'ցուցադրել դիրքը ինտերակտիվ քարտեզի վրա',
   it:'mostra la località su una carta interattiva',
   is:'sýna staðsetningu á gagnvirku korti',
   id:'Tunjukkan letak di peta interaktif',
   ja:'インタラクティブ地図上に位置を表示',
   km:'បង្ហាញទីតាំងនៅលើផែនទីអន្តរកម្ម',
   ko:'인터랙티브 지도에 위치를 표시',
   lt:'Rodyti vietą interaktyviame žemėlapyje',
   lv:'Rādīt atrašanās vietu interaktīvajā kartē',
   mk:'прикажи положба на интерактивна карта',
   nl:'de locatie op een interactieve kaart tonen',
   no:'vis beliggenhet på interaktivt kart',
   nv:'kéyah tʼáá dah siʼą́ą́ ńtʼę́ę́ʼ beʼelyaaígíí',
   pl:'Pokaż lokalizację na mapie interaktywnej',
   pt:'mostrar a localidade num mapa interactivo',
   ro:'Arată locaţia pe o hartă interactivă',
   ru:'показать положение на интерактивной карте',
   sk:'zobraz miesto na interaktívnej mape',
   sl:'Prikaži lego na interaktivnem zemljevidu',
   sq:'trego vendndodhjen në hartë',
   fi:'näytä paikka interaktiivisella kartalla',
   sv:'visa platsen på en interaktiv karta',
   tr:'Yeri interaktif bir haritada göster',
   uk:'показати положення на інтерактивній карті',
   vi:'xem vị trí này trên bản đồ tương tác',
   vo:'Jonön topi su kaed itjäfidik',
   zh:'显示该地在地图上的位置',
   'zh-cn':'显示该地在地图上的位置',
   'zh-sg':'显示该地在地图上的位置',
   'zh-tw':'顯示該地在地圖上的位置',
   'zh-hk':'顯示該地在地圖上的位置'
  },
  close : {
   af:'Sluit',
   als:'Zuä machä',
   ar:'غلق',
   'be-tarask':'закрыць',
   'be-x-old':'закрыць',
   bg:'затвори',
   bpy:'জিপা',
   br:'serriñ',
   ca:'tanca',
   cs:'zavřít',
   da:'luk',
   de:'schließen',
   nv:'doo yishʼį́ nisin da',
   el:'έξοδος',
   en:'close',
   eo:'fermu', 
   eu:'itxi',
   es:'cerrar',
   fa:'بستن',
   fr:'Quitter',
   fur:'siere',
   fy:'ticht',
   gl:'pechar',
   he:'לסגור',
   hi:'बंद करें',
   hr:'zatvori',
   hu:'bezárás', 
   hy:'փակել',
   id:'tutup',
   is:'loka',
   it:'chiudi',
   ja:'閉じる',
   km:'បិទ',
   ko:'닫기',
   lt:'uždaryti',
   lv:'aizvērt',
   mk:'затвори',
   nl:'sluiten',
   no:'lukk',
   pl:'zamknij',
   pt:'fechar',
   ro:'închide',
   ru:'закрыть',
   sk:'zatvoriť',
   sl:'zapri',
   sq:'mbylle',
   fi:'sulje',
   sv:'stäng',
   tr:'kapat',
   uk:'закрити',
   vi:'đóng',
   vo:'färmükön',
   zh:'关闭',
   'zh-cn':'关闭',
   'zh-sg':'关闭',
   'zh-tw':'關閉',
   'zh-hk':'關閉'
  }
 },

 language = '', site = '',
 iframe = { div: null, iframe: null, closebutton: null },

 bodyc,
 coord_filter = /^([\d+-.]+)_([\d+-.]*)_?([\d+-.]*)_?([NS])_([\d+-.]+)_([\d+-.]*)_?([\d+-.]*)_?([EOW])/,
 coord_list = [],
 coord_highlight = -1,
 //region_index = 0,
 //coordinate_region = '',

 quicklinkbox = null,
 quicklinkdest = null;

 // check if we are in a right-to-left-script project
 function isRTL() {
  return /(^|\s)rtl(\s|$)/.test(document.body.className);
 }

 // get position on page
 function yPos(el) {
  return $(el).offset().top + $(el).outerHeight();
 }

 // show, move, and update iframe
 function showIFrame(e) {
  var wi = iframe, my = yPos(this),
      newurl = config.iframeurl + '?' + e.data.param;
  if( wi.iframe.attr('src') !== newurl ) {
   wi.iframe.attr( 'src', newurl );
  } else if( wi.div.css('display') !== 'none' ) {
   wi.div.hide();
   return false;
  }
  wi.div.css( 'top', my+'px' ).show();
  return false;
 }

 // fill in the map-service templates 
 function qlURL( lat, lon, zoom ) {
  var url  = config.quicklinkurl,
      span = Math.pow( 2.0, zoom) / 150.0;

  url = url.replace( /\{latdegdec\}/g, lat );
  url = url.replace( /\{londegdec\}/g, lon );
  url = url.replace( /\{span\}/g, span.toFixed(4) );

  return url;
 }

 function highlight(i) {
  if( coord_highlight >= 0 ) {
   $(coord_list[coord_highlight].obj).css('background-color','').find('span:visible').css('background-color','');
  }
  coord_highlight = i;
  if( coord_highlight >= 0 ) {
   $(coord_list[coord_highlight].obj).css('background-color','yellow').find('span:visible').css('background-color','yellow');
  }
 }

 function messageHub(e) {
  var i, d;
  e = e.originalEvent;
  d = e.data.split(',');
  switch(d[0]) { 
   case 'request' :
    for( i = 0; i < coord_list.length; ++i ) {
     e.source.postMessage( coord_list[i].lat + ',' + coord_list[i].lon + ',' + i + ',' +  coord_list[i].title,
      'http://toolserver.org' );
    }
    break;
   case 'unhighlight' :
    highlight(-1);   
    break;
   case 'scroll' :
    $("html:not(:animated),body:not(:animated)").animate({ scrollTop: $(coord_list[parseInt(d[1])].obj).offset().top - 20 }, 500 );
    iframe.div.css( { top: yPos( coord_list[parseInt(d[1])].obj ) + 'px'} );
    // make sure scroll target gets highlighted
    setTimeout( function() { highlight(parseInt(d[1])); }, 200 );
    break;
   case 'highlight' :
    highlight(parseInt(d[1]));
    break;
  }  
 }

 // parse url parameters into a hash
 function parseParams(url) {
  var map = {}, h, i, pair = url.substr(url.indexOf('?')+1).split('&');
  for( i=0; i<pair.length; ++i ) {
   h = pair[i].split('=');
   map[h[0]] = h[1];
  }
  return map;
 }

 // Insert the IFrame into the page.

 var wi = iframe,
     wc = config,
     ds_filter = /(dim|scale):([\d+-.]+)(km|)/,
     marker = { lat:0, lon:0 },
     link, links, params, coordinates,
     key, len, coord_title, coord_params, icons, startTime, zoomlevel, content, mapbutton;

 // apply settings
 if( typeof(wma_settings) === 'object' ) {
  for ( key in wma_settings) {
   if( typeof(wma_settings[key]) === typeof(wc[key]) ) {
    wc[key] = wma_settings[key];
   }
  }
 }

 if( wc.enabled === false ) { return; }

 site = window.location.host.substr( 0, window.location.host.indexOf('.') );
 language = wgUserLanguage;

 // remove icons from title coordinates
 coord_title = document.getElementById('coordinates') || document.getElementById('coordinates-title');
 if( coord_title ) {
  icons = coord_title.getElementsByTagName('a');
  len = icons.length;
  for( key = 0; key < len; key++ ) {
   if( typeof(icons[key]) === 'object' &&
       icons[key] !== null &&
       icons[key].className === 'image' ) {
    icons[key].parentNode.removeChild(icons[key]);
   }
  }
 }

 bodyc = $( wc.onlytitle ? '#coordinates,#coordinates-title' : 'html' );
 startTime = (new Date()).getTime();

 bodyc.find('a.external.text').each( function( key, link ) {
  // check for timeout (every 10 links only)
  if( key % 10 === 9 && (new Date()).getTime() > startTime + wc.timeout ) { 
   return false; // break out of each
  }

  if( link.href.match(/_globe:(?!earth)/i) !== null ) { 
   return true; // continue each
  }

  coordinates = link.href.replace( /−/g, '-' );
  coord_params = coordinates.match(/&params=([^&=<>|]{7,255})/);

  if(!coord_params) { return true; }
  coord_params = coord_params[1];
   
  if(!coord_filter.test(coord_params)) {
   return true;
  }
  coord_filter.exec(coord_params);
  marker.lat=(1.0*RegExp.$1) + ((RegExp.$2||0)/60.0) + ((RegExp.$3||0)/3600.0);
  if( RegExp.$4 === 'S' ) { marker.lat*=-1; }
  marker.lon=(1.0*RegExp.$5) + ((RegExp.$6||0)/60.0) + ((RegExp.$7||0)/3600.0);
  if( RegExp.$8 === 'W' ) { marker.lon*=-1; }

  // Find a sensible Zoom-level based on type
  zoomlevel = 1;
  if( coord_params.indexOf('_type:landmark') >= 0 ) {
   zoomlevel = 8;
  } else if( coord_params.indexOf('_type:city') >= 0 ) {
   zoomlevel = 4;
  }

  // If given use dim or scale for a zoomlevel
  if( ds_filter.test(coord_params) )
  {
   ds_filter.exec(coord_params);
   // wma shows dim approx 4e7m at zoom 0 or 1.5e8 is the scale of zoomlevel 0
   zoomlevel = (RegExp.$1 === 'dim' ?
    ( RegExp.$3 === 'km' ? Math.log( 4e4/RegExp.$2 ) : Math.log( 4e7/RegExp.$2 ) ) :
    Math.log( 1.5e8/RegExp.$2 ) ) / Math.log(2);
   if( zoomlevel > 10 ) { zoomlevel = 10; }
  }

  if( wc.zoom !== -1 ) { zoomlevel = wc.zoom; }

  // Test the unicode Symbol
  if( site === 'de' && link.parentNode.id !== 'coordinates' ) {
   mapbutton = $('<span>♁</span>').css('color','blue');
  } else {
   mapbutton = $('<img/>').attr('src', wc.buttonImage);
  }
  mapbutton.attr( {
   title: strings.buttonTooltip[language] || strings.buttonTooltip.en,
   alt: '' 
  } ).addClass('noprint').css('padding', isRTL() ? '0px 0px 0px 3px' : '0px 3px 0px 0px' ).css('cursor','pointer');

  link.parentNode.insertBefore(mapbutton[0],link);
  mapbutton.bind( 'click', { param:
   marker.lat + '_' + marker.lon + '_' +
   wc.width + '_' + wc.height + '_' +
   site + '_' + zoomlevel + '_' + language }, showIFrame );

  // store coordinates
  params = parseParams(coordinates);
  coord_list.push( { lat: marker.lat, lon: marker.lon, obj: link, title: params.title || params.pagename || '' } );

  if ( wc.quicklink ) {
   link.href = qlURL( marker.lat, marker.lon, zoomlevel );
  }
 } ); //end each

 // prepare quicklink menu box
 if ( coordinates !== null && wc.quicklink ) {
  quicklinkbox = document.createElement('div');
  // more to come :-)
 }

 // prepare iframe to house the map
 if ( coordinates !== null ) {
  wi.div = $('<div></div>').css( {
   width: (wc.width+2)+'px', height: (wc.height+2)+'px',
   margin: '0px', padding: '0px', 
   backgroundColor : 'white', border: '1px solid gray',
   position: 'absolute', top: '1em', zIndex: 13
  } ).css( isRTL() ? 'left' : 'right', '2em' ).hide();

  wi.closebutton = $('<img/>').attr( { 
   title : strings.close[language] || strings.close.en,
   src : 'http://upload.wikimedia.org/wikipedia/commons/d/d4/Button_hide.png'
  } ).css( {
   zIndex : 15, position : 'absolute', right : '11px', top : '9px', width : '18px', cursor : 'pointer'
  } ).click( function(e) { wi.div.hide() } );

  wi.iframe = $('<iframe></iframe>').attr( {
   scrolling: 'no',
   frameBorder : 0
  } ).css( {
   zIndex: 14, position: 'absolute', right: '1px', top: '1px',
   width: (wc.width)+'px', height: (wc.height)+'px',
   margin: '0px', padding: '0px'
  } );

  wi.div.append(wi.iframe);
  wi.div.append(wi.closebutton);

  $('#content,#mw_content').prepend(wi.div);
  $(window).bind('message',messageHub);
 }
});

//</pre>