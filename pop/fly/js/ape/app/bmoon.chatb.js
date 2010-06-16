//; if (!bmoon) bmoon = {};
; var bmoon = bmoon || {};
bmoon.chat = {
	version: "1.0",
	ape: {},

	init: function(ape) {
		var o = bmoon.chat;

		if (o.inited) return o;
		o.inited = true;

		o.m = $('#chat-msg-text');
		o.btm = $('#chat-msg-submit');
		
		o.ape = ape;

		return o;
	},

	onready: function(ape) {
		var o = bmoon.chat.init(ape);

		o.bindClick();
	},

	bindClick: function() {
		var o = bmoon.chat.init();
		
		o.m.bind('keydown', 'ctrl+return', o.msgSend);
		o.btm.click(o.msgSend);
	},

	msgSend: function() {
		var o = bmoon.chat.init();
		
		var
		mv = o.m.val(),
		pipe = o.ape.lcsCurrentPipe;

		if (!mv.length) return;
		o.m.val('');

		if (pipe) {
			pipe.request.send("LCS_SEND", {msg: m});
		} else {
			o.ape.request.send("LCS_MSG", {uname: o.ape.lcsaname, msg: m});
		}
	}
};
