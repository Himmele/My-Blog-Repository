function cw_Process() {
	try {
		var cu="http://tag.contextweb.com/TagPublish/GetAd.aspx";var ifurl="";var epid="";var cwod="";var esid="";var cp="527221";var crtg="";var cwfl="";var cn="1";var tppg="";var cf="728X90";var ct="81894";var asv="1";

		String.prototype.cwcontains = function(s) {
			return(this.toLowerCase().indexOf(s.toLowerCase()) != -1);
		};
		var _nxy = [-1,-1];
		var _cwd = document;
		var _cww = window;
		var _cwu = "undefined";
		var _cwn = navigator;
		var _cwl = "cwl";
		var cwfl;
		var pars="";
		if (typeof(_cww.cwfl) == _cwu) {
			cwfl = false;
		} else {
			cwfl = _cww.cwfl;
		}
		if (typeof(cwif) == _cwu) {
			var cwif = false;
		}

		var ifv = _ifv();
        var _tl = tl();
		if (!_inIf()) {
			if (typeof(cu) == _cwu) {
				var l = window.location;
				document.write("Thank you, your file's location is:  " + l);
				return;
			} else {
				aps();
				att(CWLD);
				att(tl0);
				writeAd();
			}
		}
	} catch(e) {
		if (cu.indexOf("?") == -1) {
			cu += pars;
		}
		document.write("<scr" + "ipt language='javascript' src='" + cu + (cu.indexOf("?") > -1 ? "&" : "?") +
				"jserr=" + escape(e.toString()) + "'></scr" + "ipt>");
	}
	function att(fn) {
		if (typeof(_cww.addEventListener) != _cwu) {
			_cww.addEventListener('beforeunload', fn, false);
		} else if (typeof(_cww.attachEvent) != _cwu) {
			_cww.attachEvent('onbeforeunload', fn);
		} else {
			var ool = _cww.onload;
			if (typeof _cww.onload != 'function') {
				_cww.onload = fn;
			} else {
				_cww.onload = function() {
					if (ool) {
						ool();
					}
					fn();
				}
			}
		}
	}

	;
	function tl0() {
		try {
			top.navigator.cwkv = null;
		} catch(e) {
		}
	}

	;
	function tl(key) {
		try {
			if (!(ifv == 0 || ifv == 2)) {
				return-1;
			}
			if (key == null) {
				key = "G";
			}
			var nav = top.navigator;
			if (nav.cwkv == null) {
				nav.cwkv = Object();
			}
			if (nav.cwkv[key] == null) {
				nav.cwkv[key] = 0;
			}
			return++(nav.cwkv[key]);
		} catch(e) {
			return-1;
		}
	}

	;
	function rnd() {
		return Math.floor(Math.random() * 10000000);
	}

	;
	function posXY() {
		try {
			var d = "cw_td_" + rnd();
			_cwd.write("<div id=\"" + d + "\" style=\"height:0;width:0;\"></div>");
			var e = document.getElementById(d);
			var xy = [-1,-1];
			xy = offXY(e);
			e.style.display = "none";
			if (_cww != top) {
				try {
					var ifrxy = ifXY();
					xy[0] += ifrxy[0];
					xy[1] += ifrxy[1];
				} catch(ex) {
					return "";
				}
			}
			return xy;
		} catch(e) {
			return "";
		}
	}

	;
	function ifXY() {
		var t = tpele();
		if (t == null) {
			throw-15;
		}
		var f = top.document.getElementsByTagName("IFRAME");
		for (var i = 0; i < f.length; i++) {
			var l = f[i].contentWindow.location;
			if (l == t) {
				return offXY(f[i]);
			}
		}
	}

	;
	function clXY() {
		try {
			var w = top;
			return[w.document.body.clientWidth,w.document.body.clientHeight];
		} catch(e) {
			return "";
		}
	}

	;
	function docXY() {
		try {
			return[top.window.document.body.offsetWidth,top.window.document.body.offsetHeight]
		} catch(e) {
			return "";
		}
	}

	;
	function offXY(o) {
		var x = 0;
		var y = 0;
		while (o != null && (typeof o.offsetTop != _cwu)) {
			y += o.offsetTop;
			x += o.offsetLeft;
			o = o.offsetParent;
		}
		return[x,y];
	}
	function aps() {
		pars = "?tagver=1";
		if (typeof(cd) != _cwu) {
			pars += "&cd=1";
		}
		if (typeof(ca) != _cwu) {
			pars += "&ca=" + ca;
		}
		if (typeof(cp) != _cwu) {
			pars += "&cp=" + cp;
		}
		pars += "&ct=" + ct;
		if (typeof(cn) != _cwu) {
			pars += "&cn=" + cn;
		}
		if (typeof(crtc) != _cwu) {
			pars += "&crtc=" + crtc;
		}
		if (typeof(crtg) != _cwu) {
			pars += "&crtg=" + crtg;
		}
		if (typeof(crtr) != _cwu) {
			pars += "&crtr=" + crtr;
		}
		if (typeof(crtuid) != _cwu) {
			pars += "&crtuid=" + crtuid;
		}
		if (typeof(cwod) != _cwu) {
			pars += "&cwod=" + cwod;
		}
		if (typeof(epid) != _cwu) {
			pars += "&epid=" + epid;
		}
		if (typeof(esid) != _cwu) {
			pars += "&esid=" + esid;
		}
        if (typeof(ifurl) != _cwu) {
            pars += "&ifurl=" + ifurl;
        }
		if (typeof(tppg) != _cwu) {
            pars += "&tppg=" + cwUe(tppg);
        }
		pars += "&cf=" + cwUe(cf);
	}

	function writeAd() {
		cu += pars;
		cu += "&rq=" + cwGlC();
		cu += "&dw=" + cwVw();
		cu += "&cwu=" + cwUe(cwGpU());
		cu += "&cwr=" + cwUe(cwGr());
		cu += "&mrnd=" + Math.floor(Math.random() * 100000000);
		cu += "&if=" + ifv;
		cu += "&tl=" + _tl;
		cu += "&pxy=" + posXY();
		cu += "&cxy=" + clXY();
		cu += "&dxy=" + docXY();
		cu += "&tz=" + getTZO();
		cu += "&ln=" + getLn();
		_cwd.writeln("<scr" + "ipt type=\"text/javascript\" src=\"" + cu + "\"></scr" + "ipt>");
	}

	;
	function getLn() {
		try {
			if (navigator) {
				var lns = [];
				if (navigator.language) {
					lns.push(navigator.language);
				}
				if (navigator.userLanguage) {
					lns.push(navigator.userLanguage);
				}
				if (navigator.browserLanguage) {
					lns.push(navigator.browserLanguage);
				}
				if (navigator.systemLanguage) {
					lns.push(navigator.systemLanguage)
				}
				return lns.join(',');
			}
		} catch(e) {
		}
		return '';
	}

	;
	function getTZO() {
		return new Date().getTimezoneOffset();
	}

	;
	function tpele() {
		try {
			var w = [];
			var c = _cww;
			var lm = 20;
			while (--lm > 0) {
				w.push(c);
				if ((c == top) || (c.parent == null)) {
					return w[w.length - 2].location;
				}
				c = c.parent;
			}
			return null;
		} catch(e) {
			return null;
		}
	}

	;
	function cwGlC() {
		var c = _cwl;
		var l = cp + "_" + cf + "_" + ct + "_" + _tl;
		var n = _cwn;
		if (typeof(n[c]) != _cwu) {
			if (!n[c][l]) {
				n[c][l] = 1;
			} else {
				n[c][l]++;
			}
		} else {
			n[c] = new Array();
			n[c][l] = 1;
		}
		return n[c][l];
	}

	;
	function cwUe(uri) {
		var st = escape(uri);
		st = st.replace(/\+/g, "%2B");
		st = st.replace(/\//g, "%2F");
		return st;
	}

	;
	function CWLD(ev) {
		var c = _cwl;
		for (x in _cwn[c]) {
			_cwn[c][x] = 0;
		}
	}
	function cwGr() {
		try {
		    if(ifv == 2 || ifv == 0)
			    return top.document.referrer;
		} catch(e) {}
		return "";
	}
	function cwGpU() {
		var R = '';
		var L = '';
		var F = '';
		var T = _cww;
		var d = _cwd;
		try {
		    if(ifv == 2 || ifv == 0)
			    return top.location;
		} catch(e) {}
		try {
			L = d.location;
			if (L == top.location) {
				R = L;
			} else {
				while (true) {
					R = T.document.location;
					if (T.document.referrer) {
						F = T.document.referrer;
					}
					if (T == T.parent) {
						break;
					} else if (T.document.referrer) {
						R = F;
					}
					T = T.parent;
				}
			}
			if (R == '' || R == null) {
				if (F) {
					R = F;
				} else {
					R = L;
				}
			}
		} catch(e) {
			if (F && '' != F) {
				R = F;
			} else {
				if (R == '') {
					R = L;
				}
			}
		}
		return R;
	}

	;
	function cwVw() {
		var w = 0;
		try {
			if (!document.body) {
				document.write('<div id="cwViz1" style="width:0px; height:0px; display:none; visibility:hidden;">o</div>');
			}
			if (self.innerHeight) {
				w = self.innerWidth;
			} else if (document.documentElement && document.documentElement.clientHeight) {
				w = document.documentElement.clientWidth;
			} else if (document.body) {
				w = document.body.clientWidth;
			}
		} catch(e) {
			w = -1;
		}
		return w;
	}

	;
	function _ifv() {
		try {
			if (_cww == top) {
				return 0;
			}
			if (_cwd.location.host == top.document.location.host) {
				return 2;
			}
			return 1;
		} catch(e) {
			if (top && parent == top) {
				return 1;
			} else {
				try {
					if(parent.document.referrer)
						return 1;
					} catch(e) {}
				return 3;
			}
		}
	}

	;
	function _isMT(d) {
		if ((!d || d == null) || (typeof d == "string" && (d == "null" || d == "undefined" || d.replace(/\s*/,
				"") == ""))) {
			return true;
		} else {
			return false;
		}
	}

	;
	function _inIf() {
		_evalQ();
		if (cwif || (ifv == 0) || (ifv == 2) || !cwfl) {
			return false;
		}
		return _ifrm();
		function _evalQ() {
			var query = location.search.slice(1);
			if (!_isMT(query) && query.cwcontains("cwif")) {
				eval(unescape(query));
			}
		}
	}

	;
	function _ifrm() {
		var L = cwfl + "?cwif='true';";
		_btf("cp", cp);
		_btf("ct", ct);
		_btf("cf", cf);
		_btf("cu", cu);
		_btf("cn", cn);
		var pw = _gtParIf(cwfl);
		if (pw) {
			pw.location.replace(L);
			return true;
		} else {
			return false;
		}
		function _btf(name, value) {
			if (!_isMT(value)) {
				L += name + "='" + value + "';";
			}
		}

		;
		function _gtParIf() {
			var pw = window;
			try {
				var pdom = cwfl.split("//")[1].split("/")[0];
				var wdom = pw.location.host;
				if (wdom == pdom) {
					return null;
				}
				while (pw.parent.window != pw.parent.parent.window) {
					wdom = pw.parent.location.host;
					if (wdom.cwcontains(pdom)) {
						break;
					}
					pw = pw.parent.window;
				}
			} catch(e) {
			}
			return pw;
		}
	}
}
;
cw_Process();