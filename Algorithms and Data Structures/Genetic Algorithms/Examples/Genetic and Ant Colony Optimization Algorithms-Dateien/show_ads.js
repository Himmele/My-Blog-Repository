google_ad_url = '';
google_random = (new Date()).getTime();
google_org_error_handler = window.onerror;

function quoted(str) {
  return (str != null) ? '"' + str + '"' : '""';
}

function google_encodeURIComponent(str) {
  if (typeof(encodeURIComponent) == 'function') {
    return encodeURIComponent(str);
  } else {
    return escape(str);
  }
}

function google_write_tracker(tracker_event) {
  var img_url = window.google_ad_url.replace(/pagead\/ads/, 'pagead/imp.gif');
  var img_src = img_url + '&event=' + tracker_event;
  var img_tag = '<i' + 'mg height="1" width="1" border="0" ' +
                'src=' + quoted(img_src) +
                ' />';
  document.write(img_tag);
}

function google_append_url(param, value) {
  if (value) {
    window.google_ad_url += '&' + param + '=' + value;
  }
}

function google_append_url_esc(param, value) {
  if (value) {
    google_append_url(param, google_encodeURIComponent(value));
  }
}

function google_append_color(param, value) {
  if (value && typeof(value) == 'object') {
    value = value[window.google_random % value.length];
  }
  google_append_url('color_' + param, value);
}

function google_show_ad() {
  var w = window;
  w.onerror = w.google_org_error_handler;
  w.google_ad_url = 'http://pagead2.googlesyndication.com/pagead/ads?';
  w.google_ad_client = w.google_ad_client.toLowerCase();
  if (w.google_ad_client.substring(0,3) != 'ca-') {
     w.google_ad_client = 'ca-' + w.google_ad_client;
  }
  w.google_ad_url += 'client=' + escape(w.google_ad_client) +
                     '&random=' + w.google_random;

  google_append_url('hl', w.google_language);
  google_append_url('gl', w.google_gl);
  google_append_url_esc('hints', w.google_hints);
  google_append_url('adsafe', w.google_safe);
  google_append_url('oe', w.google_encoding);
  google_append_url('lmt', w.google_last_modified_time);
  google_append_url_esc('alternate_ad_url', w.google_alternate_ad_url);
  google_append_url('alt_color', w.google_alternate_color);

  if (w.google_ad_format) {
    google_append_url_esc('format', w.google_ad_format.toLowerCase());
  }

  google_append_url('num_ads', w.google_max_num_ads);
  google_append_url('output', w.google_ad_output);
  google_append_url('adtest', w.google_adtest);
  if (w.google_ad_channel) {
    google_append_url_esc('channel', w.google_ad_channel.toLowerCase());
  }
  google_append_url_esc('url', w.google_page_url);
  google_append_color('bg', w.google_color_bg);
  google_append_color('text', w.google_color_text);
  google_append_color('link', w.google_color_link);
  google_append_color('url', w.google_color_url);
  google_append_color('border', w.google_color_border);
  google_append_color('line', w.google_color_line);
  google_append_url('kw_type', w.google_kw_type);
  google_append_url_esc('kw', w.google_kw);
  google_append_url_esc('contents', w.google_contents);
  google_append_url('num_radlinks', w.google_num_radlinks);
  google_append_url('max_radlink_len', w.google_max_radlink_len);
  google_append_url('rl_filtering', w.google_rl_filtering);
  google_append_url('ad_type', w.google_ad_type);
  google_append_url('image_size', w.google_image_size);
  google_append_url('feedback_link', w.google_feedback);

  w.google_ad_url = w.google_ad_url.substring(0, 1000);
  w.google_ad_url = w.google_ad_url.replace(/%\w?$/, '');

  if (google_ad_output == 'js' && w.google_ad_request_done) {
    document.write('<scr' + 'ipt language="JavaScript1.1"' +
                   ' src=' + quoted(google_ad_url) +
                   '></scr' + 'ipt>');
  } else if (google_ad_output == 'html') {
    if (w.name == 'google_ads_frame') {
      google_write_tracker('reboundredirect');
    } else {
      document.write('<ifr' + 'ame' +
                     ' name="google_ads_frame"' +
                     ' width=' + quoted(w.google_ad_width) +
                     ' height=' + quoted(w.google_ad_height) +
                     ' frameborder=' + quoted(w.google_ad_frameborder) +
                     ' src=' + quoted(w.google_ad_url) +
                     ' marginwidth="0"' +
                     ' marginheight="0"' +
                     ' vspace="0"' +
                     ' hspace="0"' +
                     ' allowtransparency="true"' +
                     ' scrolling="no">');
      google_write_tracker('noiframe');
      document.write('</ifr' + 'ame>');
    }
  }

  w.google_ad_frameborder = null;
  w.google_ad_format = null;
  w.google_page_url = null;
  w.google_language = null;
  w.google_gl = null;
  w.google_hints = null;
  w.google_safe = null;
  w.google_encoding = null;
  w.google_ad_output = null;
  w.google_max_num_ads = null;
  w.google_ad_channel = null;
  w.google_contents = null;
  w.google_alternate_ad_url = null;
  w.google_alternate_color = null;
  w.google_color_bg = null;
  w.google_color_text = null;
  w.google_color_link = null;
  w.google_color_url = null;
  w.google_color_border = null;
  w.google_color_line = null;
  w.google_adtest = null;
  w.google_kw_type = null;
  w.google_kw = null;
  w.google_num_radlinks = null;
  w.google_max_radlink_len = null;
  w.google_rl_filtering = null;
  w.google_ad_type = null;
  w.google_image_size = null;
  w.google_feedback = null;
}

function google_error_handler(message, url, line) {
  google_show_ad();
  return true;
}

window.onerror = google_error_handler;

if (window.google_ad_frameborder == null) {
  google_ad_frameborder = 0;
}

if (window.google_ad_output == null) {
  google_ad_output = 'html';
}

if (window.google_ad_format == null && window.google_ad_output == 'html') {
  google_ad_format = google_ad_width + 'x' + google_ad_height;
}

if (window.google_page_url == null) {
  google_page_url = document.referrer;
  if (window.top.location == document.location) {
    google_page_url = document.location;
    google_last_modified_time = Date.parse(document.lastModified) / 1000;
  }
}
google_show_ad();

