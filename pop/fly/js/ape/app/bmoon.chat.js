//; if (!bmoon) bmoon = {};
; var bmoon = bmoon || {};
bmoon.chat = {
	version: "1.0",
	inited: false,
	adminon: false,
	ape: {},

	init: function(ape) {
		var o = bmoon.chat,
		html = [
			'<div id="bchat">',
				'<div id="bchat-head"><a href="javascript: void(0);" id="bchat-trigger">留言</a></div>',
				'<div id="bchat-body">',
					'<div id="bchat-msgs"></div>',
					'<textarea cols="27" rows="3" id="bchat-input"></textarea>',
					'<div><a title="Control+Enter" href="javascript:" id="bchat-snd">发送</a></div>',
				'</div>',
			'</div>'
		].join('');

		if (o.inited) return;
		o.ape = ape;
		o.inited = 1;

        $('head').append('<link rel="stylesheet" href="http://css.bmoon.com/mchat.css" />');
        $('body').append(html);
        if ($.browser.msie && $.browser.version=="6.0") $('#bchat').css('position','absolute');

		$('#bchat-trigger').toggle(o.openChat, o.closeChat);
        $('#bchat-input').bind('keydown', 'ctrl+return', o.msgSend);
		$('#bchat-snd').click(o.msgSend);
	},

	openChat: function() {
		var o = bmoon.chat;

		$('#bchat-body').fadeIn();
        $('#bchat-input').focus();
	},

	closeChat: function() {
		var o = bmoon.chat;

		$('#bchat-body').fadeOut();
	},

	msgSend: function() {
		var o = bmoon.chat,
		m = $('#bchat-input').val(),
		pipe = o.ape.lcsCurrentPipe;

		$('#bchat-input').val('');

		if (!m.length) return;

		if (o.amdinon && pipe) {
			pipe.request.send("LCS_SEND", {msg: m});
		} else {
			o.ape.request.send("LCS_MSG", {aname: o.ape.lcsaname, msg: m});
		}
	},

	adminOn: function() {
		var o = bmoon.chat;

		o.adminon = true;
	},

	adminOff: function() {
		var o = bmoon.chat;

		o.adminon = false;
	}
};
