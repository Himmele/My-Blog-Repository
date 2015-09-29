/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ~ ©2011 ClickBank All Rights Reserved                                                                                ~
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

var hopfeed_affiliate = (hopfeed_affiliate == null ? null : hopfeed_affiliate);
var hopfeed_env = (hopfeed_env == null || hopfeed_env == '') ? 'prod' : hopfeed_env;

//we only want to rewrite the url if we're not in a dev environment & they passed an affi.
if(hopfeed_env != "dev") {
	
	// if they passed an affi, replace www w/ affi. else keep old hopfeed_path
	if(hopfeed_affiliate != null && hopfeed_affiliate != '' && hopfeed_affiliate != 'null') {
		hopfeed_path = hopfeed_path.replace("://www","://" + hopfeed_affiliate);
	}
	
}

var hopfeed_type = (hopfeed_type==null)? 'JS':hopfeed_type;
var hopfeed_cellpadding = (hopfeed_cellpadding==null)? 5:hopfeed_cellpadding;
var hopfeed_rows = (hopfeed_rows==null)? '': hopfeed_rows;
var hopfeed_cols = (hopfeed_cols==null)? '': hopfeed_cols;
var hopfeed_affiliate_tid = (hopfeed_affiliate_tid == null)? '': hopfeed_affiliate_tid;
var hopfeed_height = (hopfeed_height==null)? '': hopfeed_height;
var hopfeed_width = (hopfeed_width==null)? '': hopfeed_width;
var hopfeed_font = (hopfeed_font==null)? '': hopfeed_font;
var hopfeed_font_size = (hopfeed_font_size==null)? '': hopfeed_font_size;
var hopfeed_font_color = (hopfeed_font_color==null)? '': hopfeed_font_color;
var hopfeed_border_color = (hopfeed_border_color==null)? '#FFFFFF': hopfeed_border_color;
var hopfeed_link_font_color = (hopfeed_link_font_color==null)? '': hopfeed_link_font_color;
var hopfeed_link_font_hover_color = (hopfeed_link_font_hover_color==null)? '': hopfeed_link_font_hover_color;
var hopfeed_background_color = (hopfeed_background_color==null)? '': hopfeed_background_color;
var hopfeed_align = (hopfeed_align==null)? '': hopfeed_align;
var hopfeed_fill_slots = (hopfeed_fill_slots==null)? '': hopfeed_fill_slots;
var hopfeed_link_target = (hopfeed_link_target==null)? '_blank': hopfeed_link_target;

var src = hopfeed_path + '/serv/hopFeedServ.htm?type=' + hopfeed_type+ '&borderColor=' + escape(hopfeed_border_color.replace(/\+/g,'%2b')) + '&fillAllSlots=' + hopfeed_fill_slots + '&align=' + hopfeed_align + '&height='+hopfeed_height+'&width='+ hopfeed_width + '&linkFontHoverColor=' + escape(hopfeed_link_font_hover_color.replace(/\+/g,'%2b')) + '&linkFontColor=' + escape(hopfeed_link_font_color.replace(/\+/g,'%2b')) + '&backgroundColor=' + escape(hopfeed_background_color.replace(/\+/g,'%2b')) + '&font=' + escape(hopfeed_font.replace(/\+/g,'%2b'))+ '&fontSize=' + hopfeed_font_size + '&fontColor=' + escape(hopfeed_font_color.replace(/\+/g,'%2b'))+ '&rows='+hopfeed_rows+'&cols='+hopfeed_cols+'&keywords=' + escape(hopfeed_keywords.replace(/\+/g,'%2b')) +'&tid='+hopfeed_affiliate_tid+ (hopfeed_affiliate == null ? '': '&affiliate='+hopfeed_affiliate)+'&cellpadding='+hopfeed_cellpadding + '&hopfeedLinkTarget=' +hopfeed_link_target;

function show_js() {
	output = '<script type="text/javascript" src="' + src + '&type=JS"></script>';
	document.write(output);
}

function show_iframe(type) {
	var output = '<iframe src="' + src + '&type='+ type+'" height="'+(hopfeed_height)+'" width="'+ (hopfeed_width) +'" style="background:' + hopfeed_background_color + ';border:1px solid ' + hopfeed_border_color + '; padding:0px;margin-top:10px" frameborder="0" scrolling="no"></iframe>';
	document.write(output);
}

if (hopfeed_type == 'IFRAME' ) { 
	show_iframe(hopfeed_type);
} else {
	show_js(); 
}
