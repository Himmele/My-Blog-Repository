/*
// +------------------------------------------------------------------+
// | Inspired by Code that is Copyright (c) 2004 Bitflux GmbH         |
// |     http://blog.bitflux.ch/p1735.html                            |
// | And heavily modified by Jeff Minard                              |
// |     http://www.creatimation.net                                  |
// | And even more modified by Michael D. Adams                       |
// |     http://blogwaffe.com                                         |
// +------------------------------------------------------------------+
// | Dependent on SACK which is Copyright (c) 2005 Gregory Wild-Smith |
// |     http://www.twilightuniverse.com/projects/sack/               |
// +------------------------------------------------------------------+
// | Author: Thaya Kareeson                                           |
// |     http://omninoggin.com/                                       |
// +------------------------------------------------------------------+
// | Version: 1.2                                                     |
// +------------------------------------------------------------------+
// | License: GPL2                                                    |
// |     http://www.gnu.org/copyleft/gpl.html                         |
// +------------------------------------------------------------------+
*/

// Simon Willison http://simon.incutio.com/archive/2004/05/26/addLoadEvent

if ( 'function' != typeof addLoadEvent )
  addLoadEvent = function(func) {
    var oldonload=window.onload;
    if(typeof window.onload!='function')window.onload=func;
    else window.onload=function(){oldonload();func();}
  }

var commentPreview; var commentLast;
var afcpFormElement; var inputElement;var outputElement;var doitElement;var authorElement;var urlElement;

function commentPreviewInit() {
  doitElement = document.getElementById('afcp-preview');
  
  if (!doitElement)
  {
    return;
  }
  
  afcpFormElement = doitElement.parentNode;
  while ( 'form' != afcpFormElement.tagName.toLowerCase() )
    afcpFormElement = afcpFormElement.parentNode;
  // start off by disabling submit button
  disableSubmit();
  outputElement = document.getElementById('ajax-force-comment-preview');
  textAreaElement = afcpGetFormInput( 'comment' );
  // disable submit button & enable preview button again on comment change
  textAreaElement.onkeyup = function() {
    if ( !submitDisabled() && textAreaElement.value != commentLast ) {
      disableSubmit();
      outputElement.innerHTML = AjaxForceCommentPreviewVars.emptyString;
    }
  ;}
  textAreaElement.onchange = function() {
    if ( !submitDisabled() && textAreaElement.value != commentLast ) {
      disableSubmit();
      outputElement.innerHTML = AjaxForceCommentPreviewVars.emptyString;
    }
  ;}
  inputElement = afcpGetFormInput( 'comment' );
  authorElement = afcpGetFormInput( 'author' );
  
  urlElement = afcpGetFormInput( 'url' );
  if ( inputElement == null || outputElement == null || doitElement == null ) return;
  doitElement.onclick = commentPreviewAJAX;
  // set the result field to hidden, or to default string
  if ( '' == AjaxForceCommentPreviewVars.emptyString ) outputElement.style.display = 'none';
  else outputElement.innerHTML = AjaxForceCommentPreviewVars.emptyString;
}

addLoadEvent(commentPreviewInit);

function enableSubmit() {
  submitElement = afcpGetFormInput( 'submit' );
  submitElement.style.visibility = 'visible';
  submitElement.style.display = 'inline';
  doitElement.style.visibility = 'hidden';
  doitElement.style.display = 'none';
}

// Changed by Eli Bendersky:
// Doesn't really hide the submit button
//
function disableSubmit() {
  //~ submitElement = afcpGetFormInput( 'submit' );
  //~ submitElement.style.visibility = 'hidden';
  //~ submitElement.style.display = 'none';
  doitElement.style.visibility = 'visible';
  doitElement.style.display = 'inline';
}

function submitDisabled() {
  submitElement = afcpGetFormInput( 'submit' );
  if ( submitElement.style.display == 'none' ) {
    return true;
  }
  return false;
}

function afcpGetFormInput( el ) {
  for ( i=0; i < afcpFormElement.elements.length; i++ )
    if ( el == afcpFormElement.elements[i].name )
      return afcpFormElement.elements[i];
}

function commentPreviewAJAX() {
  var req = '';
  var newSack = false;
  commentPreview = new sack( AjaxForceCommentPreviewVars.url );
  var sep = commentPreview.argumentSeparator ? commentPreview.argumentSeparator : '&';
  commentPreview.method = 'POST';
  commentPreview.encodeURIString = false;
  commentPreview.onLoading = function() { outputElement.innerHTML = 'Loading.'; };
  commentPreview.onLoaded = function() { outputElement.innerHTML += '.'; };
  commentPreview.onInteractive = function() { outputElement.innerHTML += '.'; };
  commentPreview.onCompletion = function() {
    outputElement.innerHTML = commentPreview.response;
    // enable submit button once preview is completed
    enableSubmit();
  };

  req = commentPreview.encVar('text', inputElement.value);
  doitElement = document.getElementById('afcp-preview');
  if ( req ) {
    req += authorElement ? sep + commentPreview.encVar('author', authorElement.value) : '';
    req += urlElement ? sep + commentPreview.encVar('url', urlElement.value) : '';
  } else {
    if ( authorElement ) commentPreview.encVar('author', authorElement.value);
    if ( urlElement ) commentPreview.encVar('url', urlElement.value);
    req = new Array();
    for (key in commentPreview.vars)
      req[req.length] = key + "=" + commentPreview.vars[key][0];
    req = req.join(sep);
    newSack = true;
  }
  if ( '' != inputElement.value ) {
    commentPreview.runAJAX(newSack?'':req);
  }
  else if ( '' == inputElement.value ) {
    if( '' == AjaxForceCommentPreviewVars.emptyString ) { outputElement.innerHTML = ''; outputElement.style.display = "none"; }
    else outputElement.innerHTML = AjaxForceCommentPreviewVars.emptyString;
    // disable submit if content is empty
    disableSubmit();
  }
  commentLast = inputElement.value;
}
