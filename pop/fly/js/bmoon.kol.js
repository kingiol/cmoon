; function _execAfter(func, until) {
	if (eval(until)) {
		func();
	} else {
		setTimeout(function() {
			_execAfter(func, until);
		}, 100);
	}
}

; var bmoon = bmoon || {};
bmoon.kol = {
	version: "1.0",

	_loadJs: function(url) {
		var e = document.createElement("script");
		e.src = url;
		e.type="text/javascript";
		document.getElementsByTagName("head")[0].appendChild(e);
	},

	// {aname: 'xxx'}
	init: function(opts) {
		var o = bmoon.kol;

		if (o.inited) return o;
		o.inited = true;

		o.aname = opts.aname;

		if (typeof jQuery != 'function' || jQuery.fn.jquery < '1.4.2') {
			o._loadJs('http://js.kaiwuonline.com/b/chatb.js');
		} else if (typeof jQuery.cookie != 'function') {
			$('head').append('<script type="text/javascript" src="http://js.kaiwuonline.com/b/chatc.js" charset="utf-8"></script>');
		}
		
		// wait for chatx.js loaded
		_execAfter(function() {
			opts = $.extend({
				aname: 'unknown',
				js: 'http://js.kaiwuonline.com/b/client/lcs.js',
				css: 'http://css.kaiwuonline.com/b/client/lcs.css'
			}, opts || {});
			
			$('head').append('<link rel="stylesheet" href="' + opts.css + '" />');
			//$('head').append('<!--[if IE 6]><link rel="stylesheet" href="' + opts.css.substr(0, opts.css.length-4) + '_ie6.css" /><![endif]-->');
			var unode = $('meta[content*="utf"]');
			if (!unode.length) unode = $('meta[content*="UTF"]');
			if (unode.length) {
				$('head').append('<script type="text/javascript" src="'+opts.js+'" charset="utf-8"></script>');
			} else {
				$('head').append('<script type="text/javascript" src="'+opts.js.substr(0, opts.js.length-3)+'_gb.js" charset="gb2312"></script>');
			}
		}, "typeof jQuery == 'function' && typeof jQuery.cookie == 'function'");
		
		return o;
	},

	onready: function(opts) {
		var o = bmoon.kol.init(opts);

		_execAfter(function() {
			var client = new APE.Client();
			client.load({
				identifier: o.aname,
				transport: 2,
				complete: function(ape) {
					new liveCS(ape).initialize({
						aname: o.aname,
						defaultUI: opts.defaultUI,
						restoreUI: opts.restoreUI
					});
				}
			});
		}, "typeof liveCS == 'function'");
	}
};
