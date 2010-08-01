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
	init: function(opt) {
		var o = bmoon.kol;

		if (o.inited) return o;
		o.inited = true;

		o.aname = opt.aname;

		if (typeof jQuery != 'function') {
			o._loadJs('http://js.kaiwuonline.com/b/chatb.js');
		} else if (typeof jQuery.cookie != 'function') {
			$('head').append('<script type="text/javascript" src="http://js.kaiwuonline.com/b/chatc.js"></script>');
		}
		
		// wait for chatx.js loaded
		_execAfter(function() {
			$('head').append('<link rel="stylesheet" href="http://css.kaiwuonline.com/b/chat.css" />');
			$('head').append('<!--[if IE 6]><link rel="stylesheet" href="http://css.kaiwuonline.com/b/chat_ie6.css" /><![endif]-->');
			$('head').append('<script type="text/javascript" src="http://js.kaiwuonline.com/b/chat.js"></script>');
		}, "typeof jQuery == 'function' && typeof jQuery.cookie == 'function'");
		
		return o;
	},

	onready: function(opt) {
		var o = bmoon.kol.init(opt);

		_execAfter(function() {
			var client = new APE.Client();
			client.load({
				identifier: o.aname,
				transport: 2,
				complete: function(ape) {
					new liveCS(ape).initialize({aname: o.aname});
				}
			});
		}, "typeof liveCS == 'function'");
	}
};
